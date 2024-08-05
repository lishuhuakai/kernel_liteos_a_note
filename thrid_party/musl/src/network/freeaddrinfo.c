#include <stdlib.h>
#include <stddef.h>
#include <netdb.h>
#include "lookup.h"
#include "lock.h"

void freeaddrinfo(struct addrinfo *p)
{
	if (!p) {
		return;
	}
	size_t cnt;
	struct aibuf *b = (void *)((char *)p - offsetof(struct aibuf, ai));
	for (cnt=1; p->ai_next; cnt++, p=p->ai_next);
	LOCK(b->lock);
	if (!(b->ref -= cnt)) free(b);
	else UNLOCK(b->lock);
}
