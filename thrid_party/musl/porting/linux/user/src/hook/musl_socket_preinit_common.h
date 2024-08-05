#ifndef _MUSL_SOCKET_PREINIT_COMMON_H
#define _MUSL_SOCKET_PREINIT_COMMON_H

#include <stdio.h>
#include "musl_socket_dispatch.h"
#include "common_def.h"

extern struct SocketDispatchType __musl_libc_socket_dispatch;
extern struct SocketDispatchType __libc_socket_default_dispatch;

enum SocketFuncEnum {
	INITIALIZE_FUNC,
	FINALIZE_FUNC,
	GET_HOOK_FLAG_FUNC,
	SET_HOOK_FLAG_FUNC,
	LAST_FUNC,
};

#ifdef OHOS_SOCKET_HOOK_ENABLE
extern long long __current_dispatch;
extern bool __socket_hook_begin_flag;
extern long long __ohos_socket_hook_shared_library;
extern void* shared_lib_func[LAST_FUNC];
#endif

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((always_inline))
inline bool __get_socket_hook_begin_flag()
{
#ifdef OHOS_SOCKET_HOOK_ENABLE
	return __socket_hook_begin_flag;
#else
	return false;
#endif
}

__attribute__((always_inline))
inline bool __get_socket_hook_flag()
{
#ifdef OHOS_SOCKET_HOOK_ENABLE
	void* handle = (void *)__ohos_socket_hook_shared_library;
	if (handle == NULL) {
		return false;
	} else if (handle == (void *)-1) {
		return true;
	} else {
		SocketGetHookFlagType get_hook_func_ptr = (SocketGetHookFlagType)(shared_lib_func[GET_HOOK_FLAG_FUNC]);
		bool flag = get_hook_func_ptr();
		return flag;
	}
#else
	return false;
#endif
}

__attribute__((always_inline))
inline volatile const struct SocketDispatchType* get_socket_dispatch()
{
#ifdef OHOS_SOCKET_HOOK_ENABLE
	volatile const struct SocketDispatchType* ret = (struct SocketDispatchType *)__current_dispatch;
	if (ret != NULL) {
		if (!__get_socket_hook_begin_flag()) {
			ret = NULL;
		} else if (!__get_socket_hook_flag()) {
			ret = NULL;
		} else {
			return ret;
		}
	}
	return ret;
#else
	return NULL;
#endif
}

#ifdef __cplusplus
}
#endif

#endif