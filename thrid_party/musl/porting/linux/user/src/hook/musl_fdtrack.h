#ifndef _MUSL_FDTRACK_H
#define _MUSL_FDTRACK_H

#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum fdtrack_type {
	FDTRACK_EVENT_TYPE_CREATE,
};

struct fdtrack_event {
	uint32_t type;
};

typedef void (*fdtrack_hook)(struct fdtrack_event*);
void set_fdtrack_enabled(bool newValue);
bool fdtrack_cas_hook(fdtrack_hook* expected, fdtrack_hook value);

#ifdef __cplusplus
}
#endif
#endif