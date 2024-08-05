#ifdef OHOS_SOCKET_HOOK_ENABLE
#include "musl_socket.h"
#include <sys/socket.h>
#include "common_def.h"
#include "musl_socket_preinit_common.h"

int socket(int domain, int type, int protocol)
{
	volatile const struct SocketDispatchType* dispatch = get_socket_dispatch();
	if (__predict_false(dispatch != NULL)) {
		int ret = dispatch->socket(domain, type, protocol);
		return ret;
	}
	int result = MuslSocket(socket)(domain, type, protocol);
	return result;
}

#endif