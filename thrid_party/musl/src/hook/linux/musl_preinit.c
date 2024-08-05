/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

/*
The hook mode has 3 kinds which can be set by command "param set":
(1) param set libc.hook_mode "startup:\"prog1 \""
(2) param set libc.hook_mode startup:program1
(3) param set libc.hook_mode step
(4) param set libc.hook_mode direct

Case 1 represents "startup" mode, and the hooked process is "prog1 ",
which loads hooking shared library when the program starts up.
The path is added with two quotation marks because a quotation mark is a special charcter,
which need be escaped.
(2) is similar with (1), but no escaped character, so quotation marks doesn't need.
(3) represents "step" mode, which loads hooking shared library by some steps.
(4) represetnt  "direct" mode, which loads hooking shared library by a step.
*/

#ifdef HOOK_ENABLE
#include <unistd.h>
#include <signal.h>
#include "musl_malloc_dispatch_table.h"
#include "musl_malloc.h"
#include "musl_preinit_common.h"
#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdatomic.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include "musl_log.h"

void* ohos_malloc_hook_init_function(size_t bytes);
void default_memtrace(void* addr, size_t size, const char* tag, bool is_using) {}

static struct MallocDispatchType __ohos_malloc_hook_init_dispatch = {
	.malloc = ohos_malloc_hook_init_function,
	.free = MuslFunc(free),
	.mmap = MuslMalloc(mmap),
	.munmap = MuslMalloc(munmap),
	.calloc = MuslFunc(calloc),
	.realloc = MuslFunc(realloc),
	.prctl = MuslMalloc(prctl),
	.malloc_usable_size = MuslMalloc(malloc_usable_size),
	.memtrace = default_memtrace,
	.aligned_alloc = MuslMalloc(aligned_alloc),
};
#define MAX_SYM_NAME_SIZE 1000
#define MAX_PROC_NAME_SIZE 256
#define ADDR_NATIVE_ENABLE (1<<4)
#define ADDR_NATIVE_DISABLE (1<<5)
#define ADDR_NATIVE_SAVE (1<<6)
#define ADDR_NATIVE_CLEAR (1<<7)
static char *__malloc_hook_shared_lib = "libnative_hook.z.so";
static char *__malloc_hook_function_prefix = "ohos_malloc_hook";
volatile atomic_llong ohos_malloc_hook_shared_library;
static char *kMemTrackSharedLib = "libmemleak_tracker.so";
static char *kMemTrackPrefix = "track";
static char *kMemTrackPropertyEnable = "const.hiview.memleak_tracker.enable";
static char *kMemTrackSign = "true";
bool checkLoadMallocMemTrack = false;
unsigned int memLeakTypeContent = 0;
volatile atomic_llong memleak_ever_shared_library_handle;
volatile atomic_llong ohos_malloc_ever_shared_library_handle;
void* function_of_shared_lib[LAST_FUNCTION];
void* function_of_ohos_malloc_shared_lib[LAST_FUNCTION];
void* function_of_memleak_shared_lib[LAST_FUNCTION];
static enum EnumHookMode __hook_mode = STEP_HOOK_MODE;
static void __uninstal_malloc_hook();

static void get_native_hook_param(char *buf, unsigned int buf_len)
{
#ifdef OHOS_ENABLE_PARAMETER
	CachedHandle handle = CachedParameterCreate(MUSL_HOOK_PARAM_NAME,  "");
	const char *value = CachedParameterGet(handle);
	if (value != NULL) {
		size_t size = strlen(value);
		if (size > 0 && size < buf_len) {
			strcpy(buf, value);
		}
	}
	CachedParameterDestroy(handle);
#else
	return;
#endif
}

static void get_memleak_hook_param()
{
#ifdef OHOS_ENABLE_PARAMETER
	CachedHandle handle = CachedParameterCreate(kMemTrackPropertyEnable,  "false");
	const char *value = CachedParameterGet(handle);
	if (value != NULL && strncmp(value, kMemTrackSign, strlen(kMemTrackSign)) == 0) {
		checkLoadMallocMemTrack = true;
	}
	CachedParameterDestroy(handle);
#else
	return;
#endif
}

static int parse_hook_variable(enum EnumHookMode* mode, char* path, int size)
{
	if (!mode || !path || size <= 0) {
		return -1;
	}
	char hook_param_value[OHOS_PARAM_MAX_SIZE + 1] = {0};
	unsigned int len = OHOS_PARAM_MAX_SIZE;
	get_native_hook_param(hook_param_value, len);
	if (hook_param_value[0] == '\0') {
		*mode = STEP_HOOK_MODE;
		path[0] = '\0';
	} else {
		char* ptr = hook_param_value;
		while (*ptr && *ptr != ':') {
			++ptr;
		}

		if (*ptr == ':') {
			*ptr = '\0';
			++ptr;
		}

		if (strcmp(hook_param_value, "startup") == 0) {
			*mode = STARTUP_HOOK_MODE;
		} else if (strcmp(hook_param_value, "direct") == 0) {
			*mode = DIRECT_HOOK_MODE;
		} else if (strcmp(hook_param_value, "step") == 0) {
			*mode = STEP_HOOK_MODE;
		} else {
			*mode = STEP_HOOK_MODE;
		}
		if (*mode == STARTUP_HOOK_MODE) {
			if (*ptr == '\"') {
				++ptr;
				int idx = 0;
				while (idx < size - 1 && *ptr && *ptr != '\"') {
					path[idx++] = *ptr++;
				}
				path[idx] = '\0';
			} else {
				int idx = 0;
				while (idx < size - 1 && *ptr) {
					path[idx++] = *ptr++;
				}
				path[idx] = '\0';
			}
		}
	}
	return 0;
}

static bool get_proc_name(pid_t pid, char *buf, unsigned int buf_len)
{
	if (pid <= 0) {
		return false;
	}
	char target_file[FILE_NAME_MAX_SIZE] = {0};
	(void)snprintf(target_file, sizeof(target_file), "/proc/%d/cmdline", pid);
	FILE *f = fopen(target_file, "r");
	if (f == NULL) {
		return false;
	}
	if (fgets(buf, buf_len, f) == NULL) {
		(void)fclose(f);
		return false;
	}
	(void)fclose(f);
	return true;
}

static bool init_malloc_function(void* malloc_shared_library_handler, MallocMallocType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "malloc");
	*func = (MallocMallocType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_free_function(void* malloc_shared_library_handler, MallocFreeType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "free");
	*func = (MallocFreeType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_mmap_function(void* malloc_shared_library_handler, MallocMmapType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "mmap");
	*func = (MallocMmapType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_munmap_function(void* malloc_shared_library_handler, MallocMunmapType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "munmap");
	*func = (MallocMunmapType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_memtrace_function(void* malloc_shared_library_handler, MemTrace* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "memtrace");
	*func = (MemTrace)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_calloc_function(void* malloc_shared_library_handler, MallocCallocType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "calloc");
	*func = (MallocCallocType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_realloc_function(void* malloc_shared_library_handler, MallocReallocType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "realloc");
	*func = (MallocReallocType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_malloc_usable_size_function(void* malloc_shared_library_handler, MallocMallocUsableSizeType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "malloc_usable_size");
	*func = (MallocMallocUsableSizeType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_prctl_function(void* malloc_shared_library_handler, MallocPrctlType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "prctl");
	*func = (MallocPrctlType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static bool init_aligned_alloc_function(void* malloc_shared_library_handler, MallocAlignedAllocType* func, const char* prefix)
{
	char symbol[MAX_SYM_NAME_SIZE];
	snprintf(symbol, sizeof(symbol), "%s_%s", prefix, "aligned_alloc");
	*func = (MallocAlignedAllocType)(dlsym(malloc_shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

#ifdef USE_JEMALLOC_RECYCLE_FUNC
extern int je_reclaim_cache();
static void handle_recycle_cache()
{
	int ret;
	ret = je_reclaim_cache();
	MUSL_LOGD("je_reclaim_cache result=%{public}d", ret);
}

static void init_jemalloc_recycle_handler()
{
	struct sigaction action = {};
	action.sa_handler = NULL;
	action.sa_sigaction = handle_recycle_cache;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, MUSL_SIGNAL_RECYCLE_JEMALLOC);
	action.sa_flags = SA_SIGINFO | SA_RESTART;
	action.sa_restorer = NULL;
	sigaction(MUSL_SIGNAL_RECYCLE_JEMALLOC, &action, NULL);
}
#endif

static bool init_hook_functions(void* shared_library_handler, struct MallocDispatchType* table, const char* prefix)
{
	if (!init_malloc_function(shared_library_handler, &table->malloc, prefix)) {
		return false;
	}
	if (!init_free_function(shared_library_handler, &table->free, prefix)) {
		return false;
	}
	if (!init_mmap_function(shared_library_handler, &table->mmap, prefix)) {
		return false;
	}
	if (!init_munmap_function(shared_library_handler, &table->munmap, prefix)) {
		return false;
	}
	if (!init_calloc_function(shared_library_handler, &table->calloc, prefix)) {
		return false;
	}
	if (!init_realloc_function(shared_library_handler, &table->realloc, prefix)) {
		return false;
	}
	if (!init_memtrace_function(shared_library_handler, &table->memtrace, prefix)) {
		return false;
	}
	if (!init_malloc_usable_size_function(shared_library_handler, &table->malloc_usable_size, prefix)) {
		return false;
	}
	if (!init_prctl_function(shared_library_handler, &table->prctl, prefix)) {
		return false;
	}
	if (!init_aligned_alloc_function(shared_library_handler, &table->aligned_alloc, prefix)) {
		return false;
	}
	return true;
}

static void clear_function_table()
{
	for (size_t i = 0; i < LAST_FUNCTION; i++) {
		function_of_shared_lib[i] = NULL;
		if (__get_memleak_hook_flag()) {
			function_of_memleak_shared_lib[i] = NULL;
		} else if (__get_global_hook_flag()) {
			function_of_ohos_malloc_shared_lib[i] = NULL;
		}
	}
}

bool init_malloc_hook_shared_library(void* shared_library_handle, const char* shared_lib, const char* prefix, struct MallocDispatchType* dispatch_table)
{
	static const char* names[] = {
		"initialize",
		"finalize",
		"get_hook_flag",
		"set_hook_flag",
		"on_start",
		"on_end",
		"send_hook_misc_data",
		"get_hook_config",
	};

	for (int i = 0; i < LAST_FUNCTION; i++) {
		char symbol[MAX_SYM_NAME_SIZE];
		snprintf(symbol, sizeof(symbol), "%s_%s", prefix, names[i]);
		function_of_shared_lib[i] = dlsym(shared_library_handle, symbol);
		if (__get_memleak_hook_flag()) {
			function_of_memleak_shared_lib[i] = function_of_shared_lib[i];
		} else if (__get_global_hook_flag()) {
			function_of_ohos_malloc_shared_lib[i] = function_of_shared_lib[i];
		}
		if (function_of_shared_lib[i] == NULL) {
			// __musl_log(__MUSL_LOG_ERROR, "%s: %s routine not found in %s\n", getprogname(), symbol, shared_lib);
			clear_function_table();
			return false;
		}
	}

	if (!init_hook_functions(shared_library_handle, dispatch_table, prefix)) {
		clear_function_table();
		return false;
	}

	return true;
}

void* load_malloc_hook_shared_library(const char* shared_lib, const char* prefix, struct MallocDispatchType* dispatch_table)
{
	void* shared_library_handle = NULL;

	shared_library_handle = dlopen(shared_lib, RTLD_NOW | RTLD_LOCAL);

	if (shared_library_handle == NULL) {
		MUSL_LOGI("HiProfiler, Unable to open shared library %{public}s: %{public}s.", shared_lib, dlerror());
		return NULL;
	}

	if (!init_malloc_hook_shared_library(shared_library_handle, shared_lib, prefix, dispatch_table)) {
		dlclose(shared_library_handle);
		shared_library_handle = NULL;
	}
	MUSL_LOGI("HiProfiler, load_malloc_hook_shared_library success.");
	return shared_library_handle;
}

typedef void (*finalize_func_t)();
typedef bool (*init_func_t)(const struct MallocDispatchType*, bool*, const char*);
typedef bool (*on_start_func_t)();
typedef bool (*on_end_func_t)();

static void malloc_finalize()
{
	__set_hook_flag(false);
	((finalize_func_t)function_of_shared_lib[FINALIZE_FUNCTION])();

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}

bool finish_install_ohos_malloc_hooks(struct musl_libc_globals* globals, const char* options, const char* prefix, void* shared_library_handle)
{
	init_func_t init_func = (init_func_t)(function_of_shared_lib[INITIALIZE_FUNCTION]);
	if (!init_func(&__libc_malloc_default_dispatch, NULL, options)) {
		// __musl_log(__MUSL_LOG_ERROR, "%s: failed to enable malloc %s\n", getprogname(), prefix);
		clear_function_table();
		return false;
	}
	on_start_func_t start_func = (on_start_func_t)(function_of_shared_lib[ON_START_FUNCTION]);
	if (__get_global_hook_flag()) {
		if (!start_func(__uninstal_malloc_hook)) {
			// __musl_log(__MUSL_LOG_ERROR, "%s: failed to start %s\n", getprogname(), prefix);
			clear_function_table();
			return false;
		}
		atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)shared_library_handle, memory_order_seq_cst);
		atomic_store_explicit(&__musl_libc_globals.so_dispatch_table, (volatile long long)&globals->malloc_dispatch_table, memory_order_seq_cst);
		atomic_store_explicit(&__musl_libc_globals.current_dispatch_table, (volatile long long)&globals->malloc_dispatch_table, memory_order_seq_cst);
	}
	if (__get_memleak_hook_flag() && checkLoadMallocMemTrack) {
		if (!start_func(memLeakTypeContent)) {
			clear_function_table();
			return false;
		}
		atomic_store_explicit(&__musl_libc_globals.memleak_tracker_so_dispatch_table, (volatile long long)&globals->memleak_tracker_malloc_dispatch_table, memory_order_seq_cst);
		atomic_store_explicit(&__musl_libc_globals.current_dispatch_table, (volatile long long)&globals->memleak_tracker_malloc_dispatch_table, memory_order_seq_cst);
	}
	int ret_value = atexit(malloc_finalize);
	if (ret_value != 0) {
		// __musl_log(__MUSL_LOG_ERROR, "failed to set atexit cleanup function: %d\n", ret_value);
	}
	return true;
}

static bool is_empty_string(const char* str)
{
	while (*str) {
		if (!isspace((unsigned char)(*str))) {
			return false;
		}
	}
	return true;
}

static void install_ohos_malloc_hook(struct musl_libc_globals* globals, const char* shared_lib, const char* prefix)
{
	volatile void* shared_library_handle = (volatile void *)atomic_load_explicit(&ohos_malloc_hook_shared_library, memory_order_acquire);
	assert(shared_library_handle == NULL || shared_library_handle == (volatile void*)-1);
	if (__get_memleak_hook_flag()) {
		shared_library_handle = (volatile void*)load_malloc_hook_shared_library(shared_lib, prefix, &globals->memleak_tracker_malloc_dispatch_table);
	} else if (__get_global_hook_flag()) {
		shared_library_handle = (volatile void*)load_malloc_hook_shared_library(shared_lib, prefix, &globals->malloc_dispatch_table);
	}
	if (shared_library_handle == NULL) {
		// __musl_log(__MUSL_LOG_ERROR, "Can't load shared library '%s'\n", __malloc_hook_shared_lib);
		return;
	}

	if (finish_install_ohos_malloc_hooks(globals, NULL, prefix, shared_library_handle)) {
		if (strncmp(__malloc_hook_function_prefix, prefix, strlen(prefix)) == 0) {
			atomic_store_explicit(&ohos_malloc_ever_shared_library_handle, (volatile long long)shared_library_handle, memory_order_seq_cst);
		} else {
			atomic_store_explicit(&memleak_ever_shared_library_handle, (volatile long long)shared_library_handle, memory_order_seq_cst);
		}
	} else {
		// __musl_log(__MUSL_LOG_ERROR, "finish_install_ohos_malloc_hooks failed\n");
		dlclose((void *)shared_library_handle);
		atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)0, memory_order_seq_cst);
	}
}

static void* init_ohos_malloc_hook()
{
	if (__get_memleak_hook_flag()) {
		get_memleak_hook_param();
		if (checkLoadMallocMemTrack) {
			install_ohos_malloc_hook(&__musl_libc_globals, kMemTrackSharedLib, kMemTrackPrefix);
		}
	}
	if (__get_global_hook_flag()) {
		install_ohos_malloc_hook(&__musl_libc_globals, __malloc_hook_shared_lib, __malloc_hook_function_prefix);
	}
	return NULL;
}

void* ohos_malloc_hook_init_function(size_t bytes)
{
	if (atomic_exchange(&__musl_libc_globals.current_dispatch_table, (volatile const long long)NULL)) {
		pthread_t thread_id;
		MUSL_LOGI("HiProfiler, ohos_malloc_hook_init_function, pthread_create.");
		if (pthread_create(&thread_id, NULL, init_ohos_malloc_hook, NULL) != 0) {
			// __musl_log(__MUSL_LOG_ERROR, "%s: ohos_malloc_hook: failed to pthread_create\n", getprogname());
		} else if (pthread_detach(thread_id) != 0) {
			// __musl_log(__MUSL_LOG_ERROR, "%s: ohos_malloc_hook: failed to pthread_detach\n", getprogname());
		}
	}
	void*ptr = MuslFunc(malloc)(bytes);
	return ptr;

}

static void __set_default_malloc()
{
	atomic_store_explicit(&__musl_libc_globals.current_dispatch_table, (volatile const long long)NULL, memory_order_seq_cst);
}

static void __restore_hook_function_table()
{
	for (size_t i = 0; i < LAST_FUNCTION; i++) {
		if (__get_memleak_hook_flag()) {
			function_of_shared_lib[i] = function_of_memleak_shared_lib[i];
		} else if (__get_global_hook_flag()) {
			function_of_shared_lib[i] = function_of_ohos_malloc_shared_lib[i];
		}
	}
}

static void __install_malloc_hook()
{
	if (__get_memleak_hook_flag()) {
		return;
	}
	atomic_store_explicit(&__hook_enable_hook_flag, (volatile bool)true, memory_order_seq_cst);
	volatile void* ohos_malloc_ever_handle = (volatile void*)atomic_load_explicit(&ohos_malloc_ever_shared_library_handle, memory_order_acquire);
	if (ohos_malloc_ever_handle != NULL) {
		atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)ohos_malloc_ever_handle, memory_order_seq_cst);
	} else {
		atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)0, memory_order_seq_cst);
	}
	volatile void* shared_library_handle = (volatile void*)atomic_load_explicit(&ohos_malloc_hook_shared_library, memory_order_acquire);
	if (shared_library_handle == NULL) {
		MUSL_LOGI("HiProfiler, __install_malloc_hook __hook_mode %{public}d", __hook_mode);
		if (__hook_mode == STEP_HOOK_MODE) {
			atomic_store_explicit(&__musl_libc_globals.current_dispatch_table, (volatile const long long)&__ohos_malloc_hook_init_dispatch, memory_order_seq_cst);
			atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)-1, memory_order_seq_cst);
		} else {
			init_ohos_malloc_hook();
		}
	} else if (shared_library_handle != (void*)-1) {
		__restore_hook_function_table();
		volatile const struct MallocDispatchType* so_dispatch_value = (volatile const struct MallocDispatchType*)atomic_load_explicit(&__musl_libc_globals.so_dispatch_table, memory_order_acquire);
		atomic_store_explicit(&__musl_libc_globals.current_dispatch_table, (volatile long long)so_dispatch_value, memory_order_seq_cst);
		on_start_func_t start_func = (on_start_func_t)(function_of_shared_lib[ON_START_FUNCTION]);
		if (start_func && !start_func(__uninstal_malloc_hook)) {
			// __musl_log(__MUSL_LOG_ERROR, "%s: failed to enable malloc\n", getprogname());
		}

	}
}

static void __uninstal_malloc_hook()
{
	if (__get_memleak_hook_flag()) {
		return;
	}
	if (!atomic_load_explicit(&__hook_enable_hook_flag, memory_order_acquire)) {
		return;
	}
	bool expected = true;
	if (atomic_compare_exchange_strong_explicit(&__hook_enable_hook_flag, &expected, false, memory_order_release, memory_order_relaxed)) {
		bool flag = __set_hook_flag(false);
		__set_default_malloc();
		on_end_func_t end_func = (on_end_func_t)(function_of_shared_lib[ON_END_FUNCTION]);
		if (end_func) {
			end_func();
		}
	}
}

static void __install_memleak_tracker_hook(int32_t sigNum, siginfo_t *info, void *ptr)
{
	if (__get_global_hook_flag()) {
		return;
	}
	atomic_store_explicit(&__memleak_hook_flag, (volatile bool)true, memory_order_seq_cst);
	volatile void* memleak_ever_handle = (volatile void*)atomic_load_explicit(&memleak_ever_shared_library_handle, memory_order_acquire);
	if (memleak_ever_handle != NULL) {
		atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)memleak_ever_handle, memory_order_seq_cst);
	} else {
		atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)0, memory_order_seq_cst);
	}
	volatile void* shared_library_handle = (volatile void*)atomic_load_explicit(&ohos_malloc_hook_shared_library, memory_order_acquire);
	memLeakTypeContent = (unsigned int)(siginfo_t *)((info)->si_addr);
	if (shared_library_handle == NULL) {
		if (__hook_mode == STEP_HOOK_MODE) {
			if (memLeakTypeContent & ADDR_NATIVE_ENABLE) {
				atomic_store_explicit(&__musl_libc_globals.current_dispatch_table, (volatile const long long)&__ohos_malloc_hook_init_dispatch, memory_order_seq_cst);
				atomic_store_explicit(&ohos_malloc_hook_shared_library, (volatile long long)-1, memory_order_seq_cst);
			}
		} else {
			init_ohos_malloc_hook();
		}
	} else if (shared_library_handle != (void*)-1) {
		if (checkLoadMallocMemTrack) {
			__restore_hook_function_table();
			on_start_func_t start_func = (on_start_func_t)(function_of_shared_lib[ON_START_FUNCTION]);
			if (memLeakTypeContent & ADDR_NATIVE_ENABLE) {
				volatile const struct MallocDispatchType* memleak_tracker_so_dispatch_value = (volatile const struct MallocDispatchType*)atomic_load_explicit(&__musl_libc_globals.memleak_tracker_so_dispatch_table, memory_order_acquire);
				atomic_store_explicit(&__musl_libc_globals.current_dispatch_table, (volatile long long)memleak_tracker_so_dispatch_value, memory_order_seq_cst);
			}
			if (start_func && !start_func(memLeakTypeContent)) {
				// __musl_log(__MUSL_LOG_ERROR, "%s: failed to enable malloc\n", getprogname());
				clear_function_table();
			}
			if (memLeakTypeContent & ADDR_NATIVE_CLEAR) {
				atomic_store_explicit(&__memleak_hook_flag, (volatile bool)false, memory_order_seq_cst);
				bool flag = __set_hook_flag(false);
				__set_default_malloc();
				on_end_func_t end_func = (on_end_func_t)(function_of_shared_lib[ON_END_FUNCTION]);
				if (end_func) {
					end_func();
				}
				memLeakTypeContent = 0;
			}
		}
	}
}


static void __install_malloc_hook_signal_handler()
{
	struct sigaction actionInstallHook = {};
	actionInstallHook.sa_handler = __install_malloc_hook;
	sigemptyset(&actionInstallHook.sa_mask);
	sigaddset(&actionInstallHook.sa_mask, MUSL_SIGNAL_UNHOOK);
	sigaction(MUSL_SIGNAL_HOOK, &actionInstallHook, NULL);

	struct sigaction actionDef = {};
	actionDef.sa_handler = __uninstal_malloc_hook;
	sigemptyset(&actionDef.sa_mask);
	sigaddset(&actionDef.sa_mask, MUSL_SIGNAL_HOOK);
	sigaction(MUSL_SIGNAL_UNHOOK, &actionDef, NULL);

	struct sigaction actionInstallMemleakHook = {};
	actionInstallMemleakHook.sa_handler = NULL;
	actionInstallMemleakHook.sa_sigaction = __install_memleak_tracker_hook;
	sigemptyset(&actionInstallMemleakHook.sa_mask);
	sigaddset(&actionInstallMemleakHook.sa_mask, MUSL_SIGNAL_MEMCHECK);
	actionInstallMemleakHook.sa_flags = SA_SIGINFO;
	actionInstallMemleakHook.sa_restorer = NULL;
	sigaction(MUSL_SIGNAL_MEMCHECK, &actionInstallMemleakHook, NULL);
}

static void __initialize_malloc()
{
	__install_malloc_hook_signal_handler();
#ifdef USE_JEMALLOC_RECYCLE_FUNC
	init_jemalloc_recycle_handler();
#endif
}

__attribute__((constructor(1))) static void __musl_initialize()
{
	atomic_store_explicit(&__hook_enable_hook_flag, (volatile bool)false, memory_order_seq_cst);
	atomic_store_explicit(&__memleak_hook_flag, (volatile bool)false, memory_order_seq_cst);
	__set_default_malloc();
	char hook_process_path[MAX_PROC_NAME_SIZE + 1] = {0};
	parse_hook_variable(&__hook_mode, hook_process_path, sizeof(hook_process_path) - 1);
	MUSL_LOGI("HiProfiler, __musl_initialize %{public}d", __hook_mode);
	if (__hook_mode == STARTUP_HOOK_MODE) {
		char proc_name[MAX_PROC_NAME_SIZE + 1] = {0};
		if (get_proc_name(getpid(), proc_name, sizeof(proc_name) - 1)) {
			const char *pos = strrchr(proc_name, '/');
			const char* file_name;
			if (pos != NULL) {
				file_name = pos + 1;
			} else {
				file_name = proc_name;
			}
			MUSL_LOGI("HiProfiler, current proc %{public}s, , target proc %{public}s", file_name, hook_process_path);
			if (strncmp(file_name, hook_process_path, strlen(hook_process_path)) == 0) {
				atomic_store_explicit(&__hook_enable_hook_flag, (volatile bool)true, memory_order_seq_cst);
				init_ohos_malloc_hook();
			} else {
				__hook_mode = STEP_HOOK_MODE;
			}
		} else {
			__hook_mode = STEP_HOOK_MODE;
		}
	}
	__initialize_malloc();
	errno = 0;
}
#endif
