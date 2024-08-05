/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <atomic.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


#include "musl_log.h"
#include "musl_fdsan.h"
#include "libc.h"
#include "pthread_impl.h"
#include "hilog_adapter.h"

#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#define MUSL_FDSAN_ERROR(fmt, ap) HiLogAdapterVaList(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, "MUSL-FDSAN", fmt, ap)
#else
#define MUSL_FDSAN_ERROR(fmt, ap)
#endif

const char *fdsan_parameter_name = "musl.debug.fdsan";
#define ALIGN(x,y) ((x)+(y)-1 & -(y))
extern int __close(int fd);

static struct FdTable g_fd_table = {
	.error_level = FDSAN_ERROR_LEVEL_WARN_ALWAYS,
	.overflow = NULL,
};

struct FdTable* __get_fdtable()
{
	return &g_fd_table;
}

static struct FdEntry* get_fd_entry(size_t idx)
{
	struct FdEntry *entries = __get_fdtable()->entries;
	if (idx < FdTableSize) {
		return &entries[idx];
	}
	// Try to create the overflow table ourselves.
	struct FdTableOverflow* local_overflow = atomic_load(&__get_fdtable()->overflow);
	if (__predict_false(!local_overflow)) {
		struct rlimit rlim = { .rlim_max = 32768 };
		getrlimit(RLIMIT_NOFILE, &rlim);
		rlim_t max = rlim.rlim_max;

		if (max == RLIM_INFINITY) {
			max = 32768; // Max fd size
		}

		if (idx > max) {
			return NULL;
		}
		size_t required_count = max - FdTableSize;
		size_t required_size = sizeof(struct FdTableOverflow) + required_count * sizeof(struct FdEntry);
		size_t aligned_size = ALIGN(required_size, PAGE_SIZE);
		size_t aligned_count = (aligned_size - sizeof(struct FdTableOverflow)) / sizeof(struct FdEntry);
		void* allocation =
				mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (allocation == MAP_FAILED) {
			MUSL_LOGE("fdsan: mmap failed");
		}

		struct FdTableOverflow* new_overflow = (struct FdTableOverflow*)(allocation);
		new_overflow->len = aligned_count;

		if (atomic_compare_exchange_strong(&__get_fdtable()->overflow, &local_overflow, new_overflow)) {
			local_overflow = new_overflow;
		} else {
			// Another thread had mmaped.
			munmap(allocation, aligned_size);
		}
	}

	size_t offset = idx - FdTableSize;
	if (local_overflow->len <= offset) {
		return NULL;
	}
	return &local_overflow->entries[offset];
}

void __init_fdsan()
{
	enum fdsan_error_level default_level = FDSAN_ERROR_LEVEL_WARN_ALWAYS;
	fdsan_set_error_level_from_param(default_level);
}

// Exposed to the platform to allow crash_dump to print out the fd table.
void* fdsan_get_fd_table()
{
	return __get_fdtable();
}

static struct FdEntry* GetFdEntry(int fd)
{
	if (fd < 0) {
		return NULL;
	}
	return get_fd_entry(fd);
}

static void fdsan_error(const char* fmt, ...)
{
	struct FdTable* fd_table = __get_fdtable();

	enum fdsan_error_level error_level = atomic_load(&fd_table->error_level);
	if (error_level == FDSAN_ERROR_LEVEL_DISABLED) {
		return;
	}
	va_list va;
	va_start(va, fmt);
	switch (error_level) {
		case FDSAN_ERROR_LEVEL_WARN_ONCE:
			MUSL_FDSAN_ERROR(fmt, va);
			atomic_compare_exchange_strong(&fd_table->error_level, &error_level, FDSAN_ERROR_LEVEL_DISABLED);
		case FDSAN_ERROR_LEVEL_WARN_ALWAYS:
			MUSL_FDSAN_ERROR(fmt, va);
			break;
		case FDSAN_ERROR_LEVEL_FATAL:
			MUSL_FDSAN_ERROR(fmt, va);
			abort();
		case FDSAN_ERROR_LEVEL_DISABLED:
			break;
	}
	va_end(va);
}

uint64_t fdsan_create_owner_tag(enum fdsan_owner_type type, uint64_t tag)
{
	if (tag == 0) {
		return 0;
	}

	if (__predict_false((type & 0xff) != type)) {
		MUSL_LOGE("invalid fdsan_owner_type value: %x", type);
		abort();
	}

	uint64_t result = (uint64_t)(type) << 56;
	uint64_t mask = ((uint64_t)(1) << 56) - 1;
	result |= tag & mask;
	return result;
}

const char* fdsan_get_tag_type(uint64_t tag)
{
	uint64_t type = tag >> 56;
	uint64_t high_bits = tag >> 48;
	switch (type) {
		case FDSAN_OWNER_TYPE_FILE:
			return "FILE*";
		case FDSAN_OWNER_TYPE_DIRECTORY:
			return "DIR*";
		case FDSAN_OWNER_TYPE_UNIQUE_FD:
			return "unique_fd";
		case FDSAN_OWNER_TYPE_ZIP_ARCHIVE:
			return "ZipArchive";
		case FDSAN_OWNER_TYPE_MAX:
			if (high_bits == (1 << 16) - 1) {
				return "native object of unknown type";
			}
			return "object of unknown type";
		case FDSAN_OWNER_TYPE_DEFAULT:
		default:
			return "native object of unknown type";
	}
}

uint64_t fdsan_get_tag_value(uint64_t tag)
{
	// Lop off the most significant byte and sign extend.
	return (uint64_t)((int64_t)(tag << 8) >> 8);
}

void fdsan_exchange_owner_tag(int fd, uint64_t expected_tag, uint64_t new_tag)
{
	if (__pthread_self()->by_vfork) {
		return;
	}
	struct FdEntry* fde = GetFdEntry(fd);
	if (!fde) {
		return;
	}

	uint64_t tag = expected_tag;
	if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, new_tag)) {
		if (expected_tag && tag) {
			fdsan_error("failed to exchange ownership of file descriptor: fd %{public}d,            \
						was owned by %{public}s 0x%{public}016lx,                                   \
						was expected to be owned by %{public}s 0x%{public}016lx",                   \
						fd, fdsan_get_tag_type(tag), fdsan_get_tag_value(tag),
						fdsan_get_tag_type(expected_tag), fdsan_get_tag_value(expected_tag));
		} else if (expected_tag && !tag) {
			fdsan_error("failed to exchange ownership of file descriptor: fd %{public}d is unowned, \
						was expected to be owned by %{public}s 0x%{public}016lx",                   \
						fd, fdsan_get_tag_type(expected_tag), fdsan_get_tag_value(expected_tag));
		} else if (!expected_tag && tag) {
			fdsan_error("failed to exchange ownership of file descriptor: fd %{public}d,            \
						was owned by %{public}s 0x%{public}016lx, was expected to be unowned",      \
						fd, fdsan_get_tag_type(tag), fdsan_get_tag_value(tag));
		} else if (!expected_tag && !tag) {
			// expected == actual == 0 but cas failed? 
			MUSL_LOGE("fdsan compare and set failed unexpectedly while exchanging owner tag");
		}
	}
}

int fdsan_close_with_tag(int fd, uint64_t expected_tag)
{
	if (__pthread_self()->by_vfork) {
		 return __close(fd);
	}
	struct FdEntry* fde = GetFdEntry(fd);
	if (!fde) {
		return __close(fd);
	}

	uint64_t tag = expected_tag;
	if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, 0)) {
		const char* expected_type = fdsan_get_tag_type(expected_tag);
		uint64_t expected_owner = fdsan_get_tag_value(expected_tag);
		const char* actual_type = fdsan_get_tag_type(tag);
		uint64_t actual_owner = fdsan_get_tag_value(tag);
		if (expected_tag && tag) {
			fdsan_error("attempted to close file descriptor %{public}d,                         \
						expected to be owned by %{public}s 0x%{public}016lx,                    \
						actually owned by %{public}s 0x%{public}016lx",                         \
						fd, expected_type, expected_owner, actual_type, actual_owner);
		} else if (expected_tag && !tag) {
			fdsan_error("attempted to close file descriptor %{public}d,                         \
						expected to be owned by %{public}s 0x%{public}016lx, actually unowned", \
						fd, expected_type, expected_owner);
		} else if (!expected_tag && tag) {
			fdsan_error("attempted to close file descriptor %{public}d,                         \
						expected to be unowned, actually owned by %{public}s 0x%{public}016lx", \
						fd, actual_type, actual_owner);
		} else if (!expected_tag && !tag) {
			// expected == actual == 0 but cas failed?
			MUSL_LOGE("fdsan compare and set failed unexpectedly while closing");
			abort();
		}
	}

	int rc = __close(fd);
	// If we were expecting to close with a tag, abort on EBADF.
	if (expected_tag && rc == -1 && errno == EBADF) {
		fdsan_error("EBADF: close failed for fd %{public}d with expected tag: 0x%{public}016lx", fd, expected_tag);
	}
	return rc;
}

uint64_t fdsan_get_owner_tag(int fd)
{
  struct FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
	return 0;
  }
  return fde->close_tag;
}

enum fdsan_error_level fdsan_get_error_level()
{
	return __get_fdtable()->error_level;
}

enum fdsan_error_level fdsan_set_error_level(enum fdsan_error_level new_level)
{
	 if (__pthread_self()->by_vfork) {
		 return fdsan_get_error_level();
	 }

	return atomic_exchange(&__get_fdtable()->error_level, new_level);
}

enum fdsan_error_level fdsan_set_error_level_from_param(enum fdsan_error_level default_level)
{
#ifdef OHOS_ENABLE_PARAMETER
		static CachedHandle param_handler = NULL;
		if (param_handler == NULL) {
				param_handler = CachedParameterCreate(fdsan_parameter_name, "0");
		}
		char *param_value = CachedParameterGet(param_handler);
		if (param_value == NULL) {
				return fdsan_set_error_level(default_level);
		} else if (strcmp(param_value, "fatal") == 0) {
				return fdsan_set_error_level(FDSAN_ERROR_LEVEL_FATAL);
		} else if (strcmp(param_value, "warn") == 0) {
				return fdsan_set_error_level(FDSAN_ERROR_LEVEL_WARN_ALWAYS);
		} else if (strcmp(param_value, "warn_once") == 0) {
				return fdsan_set_error_level(FDSAN_ERROR_LEVEL_WARN_ONCE);
		} else {
				MUSL_LOGD("[fdsan] musl.debug.fdsan set to unknown value '%{public}s'", param_value);
		}
#endif
		return fdsan_set_error_level(default_level);
}

int close(int fd)
{
	int rc = fdsan_close_with_tag(fd, 0);
	if (rc == -1 && errno == EINTR) {
		return 0;
	}
	return rc;
}