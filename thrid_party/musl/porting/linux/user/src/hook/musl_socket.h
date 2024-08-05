#ifndef _MUSL_SOCKET_H
#define _MUSL_SOCKET_H

#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OHOS_SOCKET_HOOK_ENABLE
#define MuslSocket(func) __libc_ ## func
int __libc_socket(int, int, int);
#else
#define MuslSocket(func) func
#endif

#ifdef __cplusplus
}
#endif

#endif