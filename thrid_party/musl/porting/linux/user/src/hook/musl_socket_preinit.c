#ifdef OHOS_SOCKET_HOOK_ENABLE
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <dlfcn.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "musl_socket_preinit_common.h"
#include "musl_log.h"

static char *__socket_hook_shared_lib = "libfwmark_client.z.so";
static char *__socket_hook_function_prefix = "ohos_socket_hook";
void* shared_lib_func[LAST_FUNC];
long long __ohos_socket_hook_shared_library;
typedef bool (*init_func_type)(const struct SocketDispatchType*, bool*, const char*);
typedef void (*finalize_func_type)();
#define MAX_SYMBOL_SIZE 1000

static bool init_socket_function(void* shared_library_handler, SocketSocketType* func)
{
	char symbol[MAX_SYMBOL_SIZE];
	(void)snprintf(symbol, sizeof(symbol), "%s_%s", __socket_hook_function_prefix, "socket");
	*func = (SocketSocketType)(dlsym(shared_library_handler, symbol));
	if (*func == NULL) {
		return false;
	}
	return true;
}

static void clear_socket_function()
{
	memset(shared_lib_func, 0, sizeof(shared_lib_func));
}

static void socket_finalize()
{
	((finalize_func_type)shared_lib_func[FINALIZE_FUNC])();
	__current_dispatch = 0;
	__socket_hook_begin_flag = false;
	// Don't dlclose because hidumper crash
}

static bool finish_install_ohos_socket_hooks(const char* options)
{
	init_func_type init_func = (init_func_type)(shared_lib_func[INITIALIZE_FUNC]);
	if (!init_func(&__libc_socket_default_dispatch, NULL, options)) {
		MUSL_LOGI("Netsys, init_func failed.");
		clear_socket_function();
		return false;
	}

	int ret_value = atexit(socket_finalize);
	if (ret_value != 0) {
		MUSL_LOGI("Netsys, set atexit failed.");
	}
	return true;
}

static bool init_socket_hook_shared_library(void* shared_library_handle)
{
	static const char* names[] = {
		"initialize",
		"finalize",
		"get_hook_flag",
		"set_hook_flag",
	};

	for (int i = 0; i < LAST_FUNC; i++) {
		char symbol[MAX_SYMBOL_SIZE];
		(void)snprintf(symbol, sizeof(symbol), "%s_%s", __socket_hook_function_prefix, names[i]);
		shared_lib_func[i] = dlsym(shared_library_handle, symbol);
		if (shared_lib_func[i] == NULL) {
			clear_socket_function();
			return false;
		}
	}

	if (!init_socket_function(shared_library_handle, &(__musl_libc_socket_dispatch.socket))) {
		MUSL_LOGI("Netsys, set socket function failed.");
		clear_socket_function();
		return false;
	}

	return true;
}

static void* load_socket_hook_shared_library()
{
	void* shared_library_handle = NULL;

	shared_library_handle = dlopen(__socket_hook_shared_lib, RTLD_NOW | RTLD_LOCAL);

	if (shared_library_handle == NULL) {
		MUSL_LOGI("Netsys, Unable to open shared library %s: %s.\n", __socket_hook_shared_lib, dlerror());
		return NULL;
	}

	if (!init_socket_hook_shared_library(shared_library_handle)) {
		dlclose(shared_library_handle);
		shared_library_handle = NULL;
	}
	return shared_library_handle;
}

static void install_ohos_socket_hook()
{
	void* shared_library_handle = (void *)__ohos_socket_hook_shared_library;
	if (shared_library_handle != NULL && shared_library_handle != (void*)-1) {
		MUSL_LOGI("Netsys, ohos_socket_hook_shared_library has had.");
		return;
	}

	__current_dispatch = 0;
	shared_library_handle = load_socket_hook_shared_library();
	if (shared_library_handle == NULL) {
		MUSL_LOGI("Netsys, load_socket_hook_shared_library failed.");
		return;
	}
	MUSL_LOGI("Netsys, load_socket_hook_shared_library success.");

	if (finish_install_ohos_socket_hooks(NULL)) {
		MUSL_LOGI("Netsys, finish_install_ohos_socket_hooks success.");
		__ohos_socket_hook_shared_library = (long long)shared_library_handle;
		__current_dispatch = (long long)(&__musl_libc_socket_dispatch);
	} else {
		MUSL_LOGI("Netsys, finish_install_ohos_socket_hooks failed.");
		__ohos_socket_hook_shared_library = 0;
		dlclose((void *)shared_library_handle);
	}
}

static void init_ohos_socket_hook()
{
	install_ohos_socket_hook();
}

__attribute__((constructor())) static void __musl_socket_initialize()
{
	bool begin_flag = __get_socket_hook_begin_flag();
	MUSL_LOGI("Netsys, %d begin musl_socket_initialize, flag %d.\n", getpid(), begin_flag);
	if (!begin_flag) {
		__socket_hook_begin_flag = true;
		init_ohos_socket_hook();
	}
}
#endif