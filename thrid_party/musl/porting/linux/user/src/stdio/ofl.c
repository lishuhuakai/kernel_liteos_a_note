#include "stdio_impl.h"
#include "lock.h"
#include "fork_impl.h"
#include <stdlib.h>

#define DEFAULT_ALLOC_FILE (8)

static FILE *ofl_head = NULL;
static FILE *ofl_free = NULL;

static volatile int ofl_lock[1];
volatile int *const __stdio_ofl_lockptr = ofl_lock;

FILE **__ofl_lock()
{
	LOCK(ofl_lock);
	return &FILE_LIST_HEAD(ofl_head);
}

void __ofl_unlock()
{
	UNLOCK(ofl_lock);
}

FILE *__ofl_alloc()
{
	FILE *fsb = NULL;
	size_t cnt = 0;
	FILE *f = NULL;

	LOCK(ofl_lock);
	if (!FILE_LIST_EMPTY(ofl_free)) {
		f = FILE_LIST_HEAD(ofl_free);
		FILE_LIST_REMOVE(ofl_free);
		UNLOCK(ofl_lock);

		return f;
	}
	UNLOCK(ofl_lock);

	/* alloc new FILEs(8) */
	fsb = (FILE *)malloc(DEFAULT_ALLOC_FILE * sizeof(FILE));
	if (!fsb) {
		return NULL;
	}

	LOCK(ofl_lock);

	for (cnt = 0; cnt < DEFAULT_ALLOC_FILE; cnt++) {
		FILE_LIST_INSERT_HEAD(ofl_free, &fsb[cnt]);
	}

	/* retrieve fist and move to next free FILE */
	f = FILE_LIST_HEAD(ofl_free);
	FILE_LIST_REMOVE(ofl_free);

	UNLOCK(ofl_lock);
	return f;
}

void __ofl_free(FILE *f)
{
	LOCK(ofl_lock);
	if (!f) {
		UNLOCK(ofl_lock);
		return;
	}

	/* remove from head list */
	FILE_LIST_REMOVE(f);

	/* push to free list */
	FILE_LIST_INSERT_HEAD(ofl_free, f);

	UNLOCK(ofl_lock);
}
