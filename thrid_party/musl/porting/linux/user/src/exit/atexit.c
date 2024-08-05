#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "libc.h"
#include "lock.h"
#include "fork_impl.h"
#include "dynlink.h"
#include "musl_log.h"

#define malloc __libc_malloc
#define calloc __libc_calloc
#define realloc undef
#define free __libc_free

/* Ensure that at least 32 atexit handlers can be registered without malloc */
#define COUNT 32

struct node {
    void (*func)(void *);
    void *arg;
    void *dso;
    struct dso *internal_dso; // the internal dso weekptr, used for dlclose
    struct node *prev;
    struct node *next;
};

static size_t g_len;                  // the number of nodes currently in use
static size_t g_capacity;             // the number of available nodes
static struct node builtin[COUNT];  // 32 builtin nodes without malloc
static struct node *tail;           // point to the last node, or NULL
static struct node *head;           // point to the first node

static volatile int lock[1];
volatile int *const __atexit_lockptr = lock;

static int grow()
{
    struct node *nodes;

    if (g_capacity == 0) {
        nodes = builtin;
        head = nodes;
    } else {
        nodes = malloc(sizeof(struct node) * COUNT);
        if (nodes == NULL) {
            return -1;
        }
    }

    for (size_t i = 0; i < COUNT - 1; i++) {
        nodes[i].next = nodes + (i + 1);
    }
    nodes[COUNT - 1].next = NULL;

    // link new nodes after tail
    if (tail) {
        tail->next = nodes;
    }

    g_capacity += COUNT;
    return 0;
}

static void append_node(void (*func)(void *), void *arg, void *dso, struct dso *internal_dso)
{
    struct node *new_tail;
    if (tail == NULL) {
        new_tail = head;
    } else {
        new_tail = tail->next;
    }

    new_tail->func = func;
    new_tail->arg = arg;
    new_tail->dso = dso;
    new_tail->internal_dso = internal_dso;

    new_tail->prev = tail;
    tail = new_tail;

    g_len++;
}

static struct node* RemoveNode(struct node *node)
{
    struct node *prev = node->prev;
    if (tail == node) {
        // move back
        tail = prev;
        if (tail == NULL) {
            head = node;
        }
    } else {
        // remove node
        struct node *next = node->next;
        if (next) {
            next->prev = prev;
        }
        if (prev) {
            prev->next = next;
        }

        // insert node after tail
        struct node *tail_next = tail->next;
        node->prev = tail;
        node->next = tail_next;
        tail->next = node;
        if (tail_next) {
            tail_next->prev = node;
        }
    }

    g_len--;
    return prev;
}

void __funcs_on_exit()
{
    void (*func)(void *), *arg;

    LOCK(lock);
    for (; tail; tail = tail->prev) {
        func = tail->func;
        if (func != NULL) {
            arg = tail->arg;
            UNLOCK(lock);
            func(arg);
            LOCK(lock);
        }
    }
    UNLOCK(lock);
}

void __cxa_finalize(void *dso)
{
    void (*func)(void *), *arg;
    struct node *node;

    LOCK(lock);
    for (node = tail; node; ) {
        if (dso == node->dso) {
            func = node->func;
            if (func != NULL) {
                arg = node->arg;
                UNLOCK(lock);
                func(arg);
                LOCK(lock);
            }

            node = RemoveNode(node);
            continue;
        }

        node = node->prev;
    }

    UNLOCK(lock);
}

static void call(void *p);
__attribute__ ((__weak__)) extern void *addr2dso(size_t a);

int __cxa_atexit(void (*func)(void *), void *arg, void *dso)
{
    struct dso *p = NULL;
    LOCK(lock);

#if (defined(FEATURE_ATEXIT_CB_PROTECT))
	if ((func == (void *)call) && (dso == NULL)) {
		if (addr2dso != NULL) {
			p = addr2dso((size_t)arg);
			if (p == NULL) {
				UNLOCK(lock);
				MUSL_LOGE("call atexit with invalid callback ptr=%{public}p", arg);
				return -1;
			}
		}
	}
#endif

    if (g_len >= g_capacity) {
        if (grow()) {
            UNLOCK(lock);
            return -1;
        }
    }

    append_node(func, arg, dso, p);

    UNLOCK(lock);
    return 0;
}

static void call(void *p)
{
    if (p != NULL)
        ((void (*)(void))(uintptr_t)p)();
}

int atexit(void (*func)(void))
{
    return __cxa_atexit(call, (void *)(uintptr_t)func, 0);
}

int invalidate_exit_funcs(struct dso *p)
{
    struct node *node;

    LOCK(lock);
    for (node = tail; node; node = node->prev) {
        // if found exit callback relative to this dso, and
        if (p == node->internal_dso) {
            if ((node->dso == NULL) && node->func == (void *)call) {
                MUSL_LOGD("invalidate callback ptr=%{public}p when uninstall %{public}s", node->arg, p->name);
                node->arg = NULL;
            }
        }
    }
    UNLOCK(lock);

    return 0;
}