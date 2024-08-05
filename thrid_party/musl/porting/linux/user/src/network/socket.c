#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stddef.h>
#include "syscall.h"

#if OHOS_PERMISSION_INTERNET
typedef uint8_t (*AllowFunc)(void);
static const char *LIB_NETSYS_CLIENT_NAME = "libnetsys_client.z.so";
static const char *ALLOW_SOCKET_FUNC_NAME = "IsAllowInternet";

/*
 * Read a flag from netsys_client, there is only one place to set this flag, is the
 * founction named DoStartup in startup_appspawn.
 * */
uint8_t is_allow_internet(void)
{
	static uint8_t first_time = 1;
	static uint8_t allow = 1;

	if (!first_time) {
		return allow;
	}

	void *handler = dlopen(LIB_NETSYS_CLIENT_NAME, RTLD_LAZY);
	if (handler != NULL) {
		AllowFunc func = (AllowFunc)dlsym(handler, ALLOW_SOCKET_FUNC_NAME);
		if (func != NULL && func() == 0) {
			allow = 0;
		}
		dlclose(handler);
	}
	first_time = 0;
	return allow;
}
#endif

#ifdef OHOS_SOCKET_HOOK_ENABLE
int __libc_socket(int domain, int type, int protocol)
#else
int socket(int domain, int type, int protocol)
#endif
{
#if OHOS_PERMISSION_INTERNET
	if ((domain == AF_INET || domain == AF_INET6) && is_allow_internet() == 0) {
		errno = EPERM;
		return -1;
	}
#endif
	int s = __socketcall(socket, domain, type, protocol, 0, 0, 0);
	if ((s==-EINVAL || s==-EPROTONOSUPPORT)
		&& (type & (SOCK_CLOEXEC | SOCK_NONBLOCK))) {
		s = __socketcall(socket, domain,
			type & ~(SOCK_CLOEXEC | SOCK_NONBLOCK),
			protocol, 0, 0, 0);
		if (s < 0) {
			return __syscall_ret(s);
		}
		if (type & SOCK_CLOEXEC) {
			__syscall(SYS_fcntl, s, F_SETFD, FD_CLOEXEC);
		}
		if (type & SOCK_NONBLOCK) {
			__syscall(SYS_fcntl, s, F_SETFL, O_NONBLOCK);
		}
	}
	return __syscall_ret(s);
}