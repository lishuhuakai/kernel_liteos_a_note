#ifndef _MUSL_SOCKET_DISPATCH_H
#define _MUSL_SOCKET_DISPATCH_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*SocketSocketType)(int, int, int);

typedef bool (*SocketGetHookFlagType)();
typedef bool (*SocketSetHookFlagType)(bool);

struct SocketDispatchType
{
    SocketSocketType socket;
    SocketGetHookFlagType get_hook_flag;
    SocketSetHookFlagType set_hook_flag;
};

#ifdef __cplusplus
}
#endif

#endif