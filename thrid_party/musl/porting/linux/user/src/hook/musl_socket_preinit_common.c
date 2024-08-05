#ifdef OHOS_SOCKET_HOOK_ENABLE
#include <sys/socket.h>
#include "musl_socket.h"
#include "musl_socket_preinit_common.h"

struct SocketDispatchType __musl_libc_socket_dispatch;

long long __current_dispatch;

struct SocketDispatchType __libc_socket_default_dispatch = {
	.socket = MuslSocket(socket),
};

bool __socket_hook_begin_flag;

#endif