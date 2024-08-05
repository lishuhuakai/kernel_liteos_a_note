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
#ifndef __LITEOS__
    return &FILE_LIST_HEAD(ofl_head);
#else
        return &ofl_head;
#endif
}

void __ofl_unlock()
{
        UNLOCK(ofl_lock);
}

FILE *__ofl_alloc()
{
#ifndef __LITEOS__
        FILE *fsb = NULL;
#else
        unsigned char *fsb = NULL;
#endif
        size_t cnt = 0;
        FILE *f = NULL;

        LOCK(ofl_lock);
#ifndef __LITEOS__
        if (!FILE_LIST_EMPTY(ofl_free)) {
                f = FILE_LIST_HEAD(ofl_free);
                FILE_LIST_REMOVE(ofl_free);
                UNLOCK(ofl_lock);

                return f;
        }
#else
        if (ofl_free) {
                f = ofl_free;
                ofl_free = ofl_free->next;
                f->next = NULL;
                f->prev = NULL;
                UNLOCK(ofl_lock);

                return f;
        }
#endif
        UNLOCK(ofl_lock);

        /* alloc new FILEs(8) */
#ifndef __LITEOS__
        fsb = (FILE *)malloc(DEFAULT_ALLOC_FILE * sizeof(FILE));
#else
        fsb = (unsigned char *)malloc(DEFAULT_ALLOC_FILE * sizeof(FILE));
#endif
        if (!fsb) {
                return NULL;
        }

        LOCK(ofl_lock);
#ifndef __LITEOS__
        for (cnt = 0; cnt < DEFAULT_ALLOC_FILE; cnt++) {
                FILE_LIST_INSERT_HEAD(ofl_free, &fsb[cnt]);
        }

        /* retrieve fist and move to next free FILE */
        f = FILE_LIST_HEAD(ofl_free);
        FILE_LIST_REMOVE(ofl_free);
#else
        ofl_free = (FILE*)fsb;
        ofl_free->prev = NULL;
        f = ofl_free;

        for (cnt = 1; cnt < DEFAULT_ALLOC_FILE; cnt++) {
                FILE *tmp = (FILE*)(fsb + cnt * sizeof(FILE));
                tmp->next = NULL;
                f->next = tmp;
                tmp->prev = f;
                f = f->next;
        }

        /* reset and move to next free FILE */
        f = ofl_free;
        ofl_free = ofl_free->next;
        if (ofl_free) {
                ofl_free->prev = NULL;
        }
        f->next = NULL;
        f->prev = NULL;
#endif

        UNLOCK(ofl_lock);
        return f;
}

void __ofl_free(FILE *f)
{
        LOCK(ofl_lock);
        if (!f) {
#ifndef __LITEOS__
                UNLOCK(ofl_lock);
#endif
                return;
        }

#ifndef __LITEOS__
        /* remove from head list */
        FILE_LIST_REMOVE(f);

        /* push to free list */
        FILE_LIST_INSERT_HEAD(ofl_free, f);
#else
        /* remove from head list */
        if (f->prev) {
                f->prev->next = f->next;
        }
        if (f->next) {
                f->next->prev = f->prev;
        }
        if (f == ofl_head) {
                ofl_head = f->next;
        }

        /* push to free list */
        f->next = ofl_free;
        if (ofl_free) {
                ofl_free->prev = f;
        }
        ofl_free = f;
#endif

        UNLOCK(ofl_lock);
}
