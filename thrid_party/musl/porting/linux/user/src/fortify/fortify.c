/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#define _GNU_SOURCE
#define _BSD_SOURCE

#include <fortify/fortify.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FILE_MODE_ALL (0777)

void __fortify_error(const char* info, ...)
{
    va_list ap;
    va_start(ap, info);
    fprintf(stderr, FORTIFY_RUNTIME_ERROR_PREFIX);
    vfprintf(stderr, info, ap);
    va_end(ap);
    abort();
}

static inline int __force_O_LARGEFILE(int flags)
{
#if defined(__LP64__)
    return flags; // No need, and aarch64's strace gets confused.
#else
    return flags | O_LARGEFILE;
#endif
}

static inline bool __needs_mode(int flags)
{
    return ((flags & O_CREAT) == O_CREAT) || ((flags & O_TMPFILE) == O_TMPFILE);
}

int __open_chk(const char* pathname, int flags)
{
    if (__needs_mode(flags)) {
        __fortify_error("open: " OPEN_TOO_FEW_ARGS_ERROR);
    }
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(open)(pathname, __force_O_LARGEFILE(flags), 0);
}

int __openat_chk(int fd, const char* pathname, int flags)
{
    if (__needs_mode(flags)) {
        __fortify_error("openat: " OPEN_TOO_FEW_ARGS_ERROR);
    }
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(openat)(fd, pathname, __force_O_LARGEFILE(flags), 0);
}

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
int __open64_chk(const char* pathname, int flags)
{
    if (__needs_mode(flags)) {
        __fortify_error("open64: " OPEN_TOO_FEW_ARGS_ERROR);
    }
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(open64)(pathname, __force_O_LARGEFILE(flags), 0);
}

int __openat64_chk(int fd, const char* pathname, int flags)
{
    if (__needs_mode(flags)) {
        __fortify_error("openat64: " OPEN_TOO_FEW_ARGS_ERROR);
    }
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(openat64)(fd, pathname, __force_O_LARGEFILE(flags), 0);
}
#endif

static inline void __diagnose_pollfd_array(const char* fn, size_t fds_size, nfds_t fd_count)
{
    size_t pollfd_array_length = fds_size / sizeof(struct pollfd);
    if (__DIAGNOSE_PREDICT_FALSE(pollfd_array_length < fd_count)) {
        __fortify_error("%s: %zu-element pollfd array too small for %u fds\n",
        fn, pollfd_array_length, fd_count);
    }
}

int __poll_chk(struct pollfd* fds, nfds_t fd_count, int timeout, size_t fds_size)
{
    __diagnose_pollfd_array("poll", fds_size, fd_count);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(poll)(fds, fd_count, timeout);
}

#ifdef _GNU_SOURCE
int __ppoll_chk(struct pollfd* fds, nfds_t fd_count, const struct timespec* timeout,
    const sigset_t* mask, size_t fds_size)
{
    __diagnose_pollfd_array("ppoll", fds_size, fd_count);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(ppoll)(fds, fd_count, timeout, mask);
}
#endif

static inline void __diagnose_buffer_access(const char* fn, const char* action,
    size_t claim, size_t actual)
{
    if (__DIAGNOSE_PREDICT_FALSE(claim > actual)) {
        __fortify_error("%s: avoid %zu-byte %s %zu-byte buffer\n", fn, claim, action, actual);
    }
}

ssize_t __recvfrom_chk(int socket, void* buf, size_t len, size_t buf_size,
    int flags, struct sockaddr* src_addr, socklen_t* addrlen)
{
    __diagnose_buffer_access("recvfrom", "write into", len, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(recvfrom)(socket, buf, len, flags, src_addr, addrlen);
}

ssize_t __sendto_chk(int socket, const void* buf, size_t len, size_t buflen,
    int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
{
    __diagnose_buffer_access("sendto", "read from", len, buflen);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(sendto)(socket, buf, len, flags, dest_addr, addrlen);
}

ssize_t __recv_chk(int socket, void* buf, size_t len, size_t buf_size, int flags)
{
    __diagnose_buffer_access("recv", "write into", len, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(recv)(socket, buf, len, flags);
}

ssize_t __send_chk(int socket, const void* buf, size_t len, size_t buflen, int flags)
{
    __diagnose_buffer_access("send", "read from", len, buflen);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(send)(socket, buf, len, flags);
}

mode_t __umask_chk(mode_t mode)
{
    if (__DIAGNOSE_PREDICT_FALSE((mode & FILE_MODE_ALL) != mode)) {
        __fortify_error("umask: called with invalid mask %o\n", mode);
    }
    return __umask_real(mode);
}

// Runtime implementation of the string-dependent interface.
// Used directly by the compiler, not in a header file.
static inline void __diagnose_count(const char *fn, const char *identifier, size_t value)
{
    if (__DIAGNOSE_PREDICT_FALSE(value > LONG_MAX)) {
        __fortify_error("%s: %s %zu > SSIZE_MAX\n", fn, identifier, value);
    }
}

size_t __strlen_chk(const char* s, size_t s_len)
{
    size_t ret = __DIAGNOSE_CALL_BYPASSING_FORTIFY(strlen)(s);
    if (__DIAGNOSE_PREDICT_FALSE(ret >= s_len)) {
        __fortify_error("strlen: diagnose read exceed end of buffer\n");
    }
    return ret;
}

char* __strncat_chk(char* dest, const char* src, size_t len, size_t dst_buf_size)
{
    size_t src_len = strlen(src) + strlen(dest);
    __diagnose_buffer_access("strncat", "write into", src_len, dst_buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strncat)(dest, src, len);
}

char* __strcat_chk(char* dest, const char* src, size_t dst_buf_size)
{
    size_t src_len = strlen(src) + strlen(dest);
    __diagnose_buffer_access("strcat", "write into", src_len, dst_buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strcat)(dest, src);
}

char* __strcpy_chk(char* dest, const char* src, size_t dst_len)
{
    size_t src_len = strlen(src) + 1;
    __diagnose_buffer_access("strcpy", "write into", src_len, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strcpy)(dest, src);
}

void* __memmove_chk(void* dest, const void* src, size_t len, size_t dst_len)
{
    __diagnose_buffer_access("memmove", "write into", len, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(memmove)(dest, src, len);
}

void* __memcpy_chk(void* dest, const void* src, size_t count, size_t dst_len)
{
    __diagnose_count("memcpy", "count", count);
    __diagnose_buffer_access("memcpy", "write into", count, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(memcpy)(dest, src, count);
}

#ifdef _GNU_SOURCE
void* __mempcpy_chk(void* dest, const void* src, size_t count, size_t dst_len)
{
    __diagnose_count("mempcpy", "count", count);
    __diagnose_buffer_access("mempcpy", "write into", count, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(mempcpy)(dest, src, count);
}
#endif

char* __stpcpy_chk(char* dest, const char* src, size_t dst_len)
{
    size_t src_len = strlen(src);
    __diagnose_buffer_access("stpcpy", "write into", src_len, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(stpcpy)(dest, src);
}

void* __memchr_chk(const void* s, int c, size_t n, size_t actual_size)
{
    __diagnose_buffer_access("memchr", "read from", n, actual_size);
    void* const_cast_s = s;
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(memchr)(const_cast_s, c, n);
}

char* __stpncpy_chk(char* dest, const char* src, size_t len, size_t dst_len)
{
    __diagnose_buffer_access("stpncpy", "write into", len, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(stpncpy)(dest, src, len);
}

char *__strncpy_chk(char *dest, const char *src, size_t len, size_t dst_len)
{
    __diagnose_buffer_access("strncpy", "write into", len, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strncpy)(dest, src, len);
}

void *__memset_chk(void *dest, int byte, size_t count, size_t dst_len)
{
    __diagnose_count("memset", "count", count);
    __diagnose_buffer_access("memset", "write into", count, dst_len);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(memset)(dest, byte, count);
}

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
size_t __strlcpy_chk(char *dest, const char *src,
    size_t supplied_size, size_t dst_len_from_compiler)
{
    __diagnose_buffer_access("strlcpy", "write into", supplied_size, dst_len_from_compiler);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strlcpy)(dest, src, supplied_size);
}

size_t __strlcat_chk(char *dest, const char *src,
    size_t supplied_size, size_t dst_len_from_compiler)
{
    __diagnose_buffer_access("strlcat", "write into", supplied_size, dst_len_from_compiler);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strlcat)(dest, src, supplied_size);
}
#endif

char* __strchr_chk(const char *s, int c, size_t s_len)
{
    if (s_len == 0) {
        __fortify_error("strchr: avoid read exceed end of buffer\n");
    }
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strchr)(s, c);
}

char *__strrchr_chk(const char *s, int c, size_t s_len)
{
    if (s_len == 0) {
        __fortify_error("strrchr: avoid read exceed end of buffer\n");
    }
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(strrchr)(s, c);
}

#ifdef _GNU_SOURCE
void* __memrchr_chk(const void *s, int c, size_t n, size_t actual_size)
{
    __diagnose_buffer_access("memrchr", "read from", n, actual_size);
    void *const_cast_s = s;
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(memrchr)(const_cast_s, c, n);
}
#endif

char* __getcwd_chk(char* buf, size_t len, size_t actual_size)
{
    __diagnose_buffer_access("getcwd", "write into", len, actual_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(getcwd)(buf, len);
}

ssize_t __pread_chk(int fd, void* buf, size_t count, off_t offset, size_t buf_size)
{
    __diagnose_count("pread", "count", count);
    __diagnose_buffer_access("pread", "write into", count, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(pread)(fd, buf, count, offset);
}

ssize_t __pwrite_chk(int fd, const void* buf, size_t count, off_t offset, size_t buf_size)
{
    __diagnose_count("pwrite", "count", count);
    __diagnose_buffer_access("pwrite", "read from", count, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(pwrite)(fd, buf, count, offset);
}

ssize_t __read_chk(int fd, void* buf, size_t count, size_t buf_size)
{
    __diagnose_count("read", "count", count);
    __diagnose_buffer_access("read", "write into", count, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(read)(fd, buf, count);
}

ssize_t __write_chk(int fd, const void* buf, size_t count, size_t buf_size)
{
    __diagnose_count("write", "count", count);
    __diagnose_buffer_access("write", "read from", count, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(write)(fd, buf, count);
}

ssize_t __readlink_chk(const char* path, char* buf, size_t size, size_t buf_size)
{
    __diagnose_count("readlink", "size", size);
    __diagnose_buffer_access("readlink", "write into", size, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(readlink)(path, buf, size);
}

ssize_t __readlinkat_chk(int dirfd, const char* path, char* buf, size_t size, size_t buf_size)
{
    __diagnose_count("readlinkat", "size", size);
    __diagnose_buffer_access("readlinkat", "write into", size, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(readlinkat)(dirfd, path, buf, size);
}

size_t __fread_chk(void* buf, size_t size, size_t count, FILE* stream, size_t buf_size)
{
    size_t total;
    if (__DIAGNOSE_PREDICT_FALSE(__DIAGNOSE__SIZE_MUL_OVERFLOW(size, count, &total))) {
        // overflow: trigger the error path in fread
        return fread(buf, size, count, stream);
    }
    __diagnose_buffer_access("fread", "write into", total, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(fread)(buf, size, count, stream);
}

size_t __fwrite_chk(const void* buf, size_t size, size_t count, FILE* stream, size_t buf_size)
{
    size_t total;
    if (__DIAGNOSE_PREDICT_FALSE(__DIAGNOSE__SIZE_MUL_OVERFLOW(size, count, &total))) {
        // overflow: trigger the error path in fwrite
        return fwrite(buf, size, count, stream);
    }
    __diagnose_buffer_access("fwrite", "read from", total, buf_size);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(fwrite)(buf, size, count, stream);
}

char* __fgets_chk(char* dest, int supplied_size, FILE* stream, size_t dst_len_from_compiler)
{
    __diagnose_buffer_access("fgets", "write into", supplied_size, dst_len_from_compiler);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(fgets)(dest, supplied_size, stream);
}

int __vsnprintf_chk(char* dest, size_t supplied_size, int flags,
    size_t dst_len_from_compiler, const char* format, va_list va)
{
    __diagnose_buffer_access("vsnprintf", "write into", supplied_size, dst_len_from_compiler);
    return __DIAGNOSE_CALL_BYPASSING_FORTIFY(vsnprintf)(dest, supplied_size, format, va);
}

int __vsprintf_chk(char* dest, int flags,
    size_t dst_len_from_compiler, const char* format, va_list va)
{
    // The compiler has SIZE_MAX, But vsnprintf cannot use such a large size.
    int result = __DIAGNOSE_CALL_BYPASSING_FORTIFY(vsnprintf)(dest,
        dst_len_from_compiler == SIZE_MAX ? SSIZE_MAX : dst_len_from_compiler,
        format, va);

    // Attempts to find out after the fact fail.
    __diagnose_buffer_access("vsprintf", "write into", result + 1, dst_len_from_compiler);
    return result;
}
#undef SSIZE_MAX
#undef SIZE_MAX

int __snprintf_chk(char* dest, size_t supplied_size, int flags,
    size_t dst_len_from_compiler, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int result = __vsnprintf_chk(dest, supplied_size, flags, dst_len_from_compiler, format, va);
    va_end(va);
    return result;
}

int __sprintf_chk(char* dest, int flags, size_t dst_len_from_compiler, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int result = __vsprintf_chk(dest, flags, dst_len_from_compiler, format, va);
    va_end(va);
    return result;
}
