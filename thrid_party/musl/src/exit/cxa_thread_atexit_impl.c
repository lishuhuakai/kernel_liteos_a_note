/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pthread_impl.h"

__attribute__((__weak__)) extern void add_dso_handle_node(void *dso_handle) ;
__attribute__((__weak__)) extern void remove_dso_handle_node(void *dso_handle);

/*
 * There are two ways to implement cxa_thread_atexit_impl:
 * - CXA_THREAD_USE_TSD(default): use pthread_key_xxx to implement cxa_thread_atexit_impl.
 * - CXA_THREAD_USE_TLS: put dtors in pthread to implement cxa_thread_atexit_impl.
 */
#ifdef CXA_THREAD_USE_TSD
struct dtor_list {
    void (*dtor) (void*);
    void *arg;
    void *dso_handle;
    struct dtor_list* next;
};

// A list for current thread local dtors.
__thread struct dtor_list* thread_local_dtors = NULL;
// Whether the current thread local dtors have not been executed or registered.
__thread bool thread_local_dtors_alive = false;
static pthread_key_t dtors_key;

void run_cur_thread_dtors(void *)
{
    while (thread_local_dtors != NULL) {
        struct dtor_list* cur = thread_local_dtors;
        thread_local_dtors = cur->next;
        cur->dtor(cur->arg);
        if (remove_dso_handle_node) {
            remove_dso_handle_node(cur->dso_handle);
        }
        __libc_free(cur);
    }
    thread_local_dtors_alive = false;
    return;
} 

__attribute__((constructor())) void cxa_thread_init()
{
    if (pthread_key_create(&dtors_key, run_cur_thread_dtors) != 0) {
        abort();
    }
    return;
}

/*
 * Used for the thread calls exit(include main thread).
 * We can't register a destructor of libc for run_cur_thread_dtors because of deadlock problem:
 *   exit -> __libc_exit_fini[acquire init_fini_lock] -> run_cur_thread_dtors ->
 *   remove_dso_handle_node-> do_dlclose ->dlclose_impl[try to get init_fini_lock] -> deadlock.
 * So we call __cxa_thread_finalize actively at exit. 
 */
void __cxa_thread_finalize()
{
    run_cur_thread_dtors(NULL);
    return;
}

int __cxa_thread_atexit_impl(void (*func)(void*), void *arg, void *dso_handle)
{
    if (!thread_local_dtors_alive) {
        // Bind dtors_key to current thread, so that `run_cur_thread_dtors` can be executed when thread exits.
        if (pthread_setspecific(dtors_key, &dtors_key) != 0) {
            return -1;
        }
        thread_local_dtors_alive = true;
    }
    struct dtor_list* dtor = __libc_malloc(sizeof(*dtor));
    if (!dtor) {
        return -1;
    }
    dtor->dtor = func;
    dtor->arg = arg;
    dtor->dso_handle = dso_handle;
    dtor->next = thread_local_dtors;
    thread_local_dtors = dtor;
    if (add_dso_handle_node != NULL) {
        add_dso_handle_node(dso_handle);
    }

    return 0;
}
#endif

#ifdef CXA_THREAD_USE_TLS

int __cxa_thread_atexit_impl(void (*func)(void*), void *arg, void *dso_handle)
{
    struct thread_local_dtor* dtor = __libc_malloc(sizeof(*dtor));
    if (!dtor) {
        return -1;
    }
    dtor->func = func;
    dtor->arg = arg;
    dtor->dso_handle = dso_handle;
    pthread_t thr = __pthread_self();
    dtor->next = thr->thread_local_dtors;
    thr->thread_local_dtors = dtor;
    if (add_dso_handle_node != NULL) {
        add_dso_handle_node(dso_handle);
    }
    return 0;
}

void __cxa_thread_finalize()
{
    pthread_t thr = __pthread_self();
    while (thr->thread_local_dtors != NULL) {
        struct thread_local_dtor* cur = thr->thread_local_dtors;
        thr->thread_local_dtors= cur->next;
        cur->func(cur->arg);
        if (remove_dso_handle_node) {
            remove_dso_handle_node(cur->dso_handle);
        }
    }
    return;
}

#endif

