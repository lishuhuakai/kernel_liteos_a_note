/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pthread.h"
#include "semaphore.h"
#include "csignal"
#if not defined __APPLE__
#include "threads.h"
#endif
#include "unistd.h"
#if not defined __APPLE__
#include "sys/tgkill.h"
#endif
#include "ctime"
#include "util.h"

static void Bm_function_pthread_mutexattr_settype(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL));
    }
    pthread_mutexattr_destroy(&mutexAttr);
}

static void Bm_function_pthread_mutex_trylock_fast(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_errchk(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_rec(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_pi_fast(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL);
    pthread_mutexattr_setprotocol(&mutexAttr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_pi_errorcheck(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setprotocol(&mutexAttr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_pi_rec(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_setprotocol(&mutexAttr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_fast(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_errchk(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_rec(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_pi_fast(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL);
    pthread_mutexattr_setprotocol(&mutexAttr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_pi_errchk(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setprotocol(&mutexAttr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_pi_rec(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_setprotocol(&mutexAttr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_rwlock_tryrdlock(benchmark::State &state)
{
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, nullptr);
    while (state.KeepRunning()) {
        pthread_rwlock_tryrdlock(&rwlock);
        pthread_rwlock_unlock(&rwlock);
    }
    pthread_rwlock_destroy(&rwlock);
}

static void Bm_function_pthread_mutex_init(benchmark::State &state)
{
    pthread_mutex_t mutex;

    for (auto _ : state) {
        pthread_mutex_init(&mutex, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_mutex_init_destroy(benchmark::State &state)
{
    pthread_mutex_t mutex;

    for (auto _ : state) {
        pthread_mutex_init(&mutex, nullptr);
        pthread_mutex_destroy(&mutex);
    }
}

static void Bm_function_pthread_cond_init(benchmark::State &state)
{
    pthread_cond_t cond;

    for (auto _ : state) {
        pthread_cond_init(&cond, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_cond_init_destroy(benchmark::State &state)
{
    pthread_cond_t cond;

    for (auto _ : state) {
        pthread_cond_init(&cond, nullptr);
        pthread_cond_destroy(&cond);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_rwlock_init(benchmark::State &state)
{
    pthread_rwlock_t rwlock;

    for (auto _ : state) {
        pthread_rwlock_init(&rwlock, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_rwlock_init_destroy(benchmark::State &state)
{
    pthread_rwlock_t rwlock;

    for (auto _ : state) {
        pthread_rwlock_init(&rwlock, nullptr);
        pthread_rwlock_destroy(&rwlock);
    }
    state.SetBytesProcessed(state.iterations());
}

static void BM_pthread_rwlock_tryread(benchmark::State& state)
{
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_tryrdlock(&lock);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void BM_pthread_rwlock_trywrite(benchmark::State& state)
{
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_trywrlock(&lock);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_tss_get(benchmark::State &state)
{
    tss_t key;
    tss_create(&key, nullptr);

    while (state.KeepRunning()) {
        tss_get(key);
    }

    tss_delete(key);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_rwlock_timedrdlock(benchmark::State &state)
{
    struct timespec tout;
    time(&tout.tv_sec);
    tout.tv_nsec = 0;
    tout.tv_sec += 1;
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_timedrdlock(&lock, &tout);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_rwlock_timedwrlock(benchmark::State &state)
{
    struct timespec tout;
    time(&tout.tv_sec);
    tout.tv_nsec = 0;
    tout.tv_sec += 1;
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_timedwrlock(&lock, &tout);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_cond_timedwait(benchmark::State &state)
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 1;
    pthread_mutex_lock(&mutex);
    while (state.KeepRunning()) {
        pthread_cond_timedwait(&cond, &mutex, &ts);
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    state.SetBytesProcessed(state.iterations());
}

pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_mutex1 = PTHREAD_MUTEX_INITIALIZER;
void* ThreadTaskOne(void* arg)
{
    pthread_mutex_lock(&g_mutex1);
    pthread_cond_wait(&g_cond, &g_mutex1);
    sleep(1);
    pthread_mutex_unlock(&g_mutex1);
    return nullptr;
}

void* ThreadTaskTwo(void* arg)
{
    pthread_mutex_lock(&g_mutex1);
    pthread_cond_wait(&g_cond, &g_mutex1);
    sleep(1);
    pthread_mutex_unlock(&g_mutex1);
    return nullptr;
}

void* BroadcastNotifyMutex(void* arg)
{
    benchmark::State *statePtr = (benchmark::State *)arg;
    statePtr->ResumeTiming();
    benchmark::DoNotOptimize(pthread_cond_broadcast(&g_cond));
    statePtr->PauseTiming();
    return nullptr;
}

static void Bm_function_pthread_cond_broadcast(benchmark::State &state)
{
    while (state.KeepRunning()) {
        state.PauseTiming();
        pthread_t threadOne;
        pthread_t threadTwo;
        pthread_t threadThree;
        pthread_create(&threadOne, nullptr, ThreadTaskOne, nullptr);
        pthread_create(&threadTwo, nullptr, ThreadTaskTwo, nullptr);
        sleep(3);
        pthread_create(&threadThree, nullptr, BroadcastNotifyMutex, &state);
        pthread_join(threadOne, nullptr);
        pthread_join(threadTwo, nullptr);
        pthread_join(threadThree, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sem_timewait(benchmark::State &state)
{
    sem_t semp;
    sem_init(&semp, 0, 1);
    struct timespec spec;
    spec.tv_sec = 0;
    spec.tv_nsec = 5;
    while (state.KeepRunning()) {
        sem_timedwait(&semp, &spec);
    }
    sem_destroy(&semp);
}

static void Bm_function_Sem_post_wait(benchmark::State &state)
{
    sem_t semp;
    sem_init(&semp, 0, 0);
    struct timespec spec;
    spec.tv_sec = 0;
    spec.tv_nsec = 5;
    while (state.KeepRunning()) {
        sem_post(&semp);
        sem_wait(&semp);
    }
    sem_destroy(&semp);
}

static void Bm_function_pthread_cond_signal(benchmark::State &state)
{
    pthread_cond_t cond;
    pthread_cond_init(&cond, nullptr);
    while (state.KeepRunning())
    {
        pthread_cond_signal(&cond);
    }
    pthread_cond_destroy(&cond);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_cond_signal_wait(benchmark::State &state)
{
    while (state.KeepRunning()) {
        state.PauseTiming();
        pthread_t threadOne;
        pthread_create(&threadOne, nullptr, ThreadTaskOne, nullptr);
        sleep(1);
        state.ResumeTiming();
        pthread_cond_signal(&g_cond);
        pthread_join(threadOne, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sigaddset(benchmark::State &state)
{
    sigset_t set;
    sigemptyset(&set);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sigaddset(&set, SIGUSR1));
    }
}

static void Bm_function_pthread_setcancelstate(benchmark::State &state)
{
    while (state.KeepRunning()) {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

void *GetThreadId(void *arg)
{
    pthread_t tid = pthread_self();
    if (tid == 0) {
        perror("thread create fail");
    }
    return nullptr;
}

// Check that the two threads are equal
static void Bm_function_pthread_equal(benchmark::State &state)
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, nullptr, GetThreadId, nullptr);
    pthread_create(&thread2, nullptr, GetThreadId, nullptr);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(pthread_equal(thread1, thread2));
    }
    state.SetBytesProcessed(state.iterations());
}

// Initialize and destroy thread properties
static void Bm_function_pthread_attr_init_destroy(benchmark::State &state)
{
    pthread_attr_t attr;
    int ret;
    pthread_t thread1;
    pthread_t thread2;
    while (state.KeepRunning()) {
        ret = pthread_attr_init(&attr);
        benchmark::DoNotOptimize(ret);
        state.PauseTiming();
        pthread_create(&thread1, &attr, GetThreadId, nullptr);
        pthread_create(&thread2, &attr, GetThreadId, nullptr);
        pthread_join(thread1, nullptr);
        pthread_join(thread2, nullptr);
        state.ResumeTiming();
        benchmark::DoNotOptimize(pthread_attr_destroy(&attr));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_sigmask(benchmark::State &state)
{
    sigset_t set;
    sigset_t oset;
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    sigaddset(&set, SIGUSR1);

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(pthread_sigmask(SIG_BLOCK, &set, &oset));
        benchmark::DoNotOptimize(pthread_sigmask(SIG_SETMASK, &oset, nullptr));
    }
    state.SetBytesProcessed(state.iterations());
}

static pthread_spinlock_t g_spinLock;
void* Func(void* arg)
{
    benchmark::State *statePtr = (benchmark::State *)arg;
    statePtr->ResumeTiming();
    benchmark::DoNotOptimize(pthread_spin_lock(&g_spinLock));
    pthread_spin_unlock(&g_spinLock);
    statePtr->PauseTiming();
    return nullptr;
}

// Requests a lock spin lock
static void Bm_function_pthread_spin_lock_and_spin_unlock(benchmark::State &state)
{
    pthread_t tid;
    pthread_spin_init(&g_spinLock, PTHREAD_PROCESS_PRIVATE);
    while (state.KeepRunning()) {
        state.PauseTiming();
        pthread_create(&tid, nullptr, Func, &state);
        pthread_join(tid, nullptr);
        state.ResumeTiming();
    }
    pthread_spin_destroy(&g_spinLock);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_mutexattr_init_and_mutexattr_destroy(benchmark::State &state)
{
    pthread_mutexattr_t mutexAttr;
    while (state.KeepRunning()) {
        pthread_mutexattr_init(&mutexAttr);
        pthread_mutexattr_destroy(&mutexAttr);
    }
    state.SetBytesProcessed(state.iterations());
}

void* ThreadFunc(void* arg)
{
    return nullptr;
}

static void Bm_function_pthread_detach(benchmark::State &state)
{
    while (state.KeepRunning()) {
        state.PauseTiming();
        pthread_t tid;
        pthread_create(&tid, nullptr, ThreadFunc, nullptr);
        state.ResumeTiming();
        pthread_detach(tid);
    }
    state.SetBytesProcessed(state.iterations());
}

void* ThreadFuncWait(void* arg)
{
    sleep(1);
    return nullptr;
}

// Set a unique name for the thread which is limited to 16 characters in length
// including terminating null bytes
constexpr int NAME_LEN = 16;
static void Bm_function_pthread_setname_np(benchmark::State &state)
{
    pthread_t tid;
    char threadName[NAME_LEN] = "THREADFOO";
    if (pthread_create(&tid, nullptr, ThreadFuncWait, nullptr) != 0) {
        perror("pthread_create pthread_setname_np");
    }

    while (state.KeepRunning()) {
        if (pthread_setname_np(tid, threadName) != 0) {
            perror("pthread_setname_np proc");
        }
    }
    pthread_join(tid, nullptr);

    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_attr_setschedpolicy(benchmark::State &state)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int setpolicy = 1;
    while (state.KeepRunning()) {
        if (pthread_attr_setschedpolicy(&attr, setpolicy) != 0) {
            perror("pthread_attr_setschedpolicy proc");
        }
    }
    pthread_attr_destroy(&attr);
}

// Get the scheduling parameter param of the thread attribute ATTR
static void Bm_function_pthread_attr_getschedparam(benchmark::State &state)
{
    pthread_attr_t attr;
    struct sched_param setparam;
    struct sched_param getparam;
    setparam.sched_priority = 1;
    pthread_attr_init(&attr);
    if (pthread_attr_setschedparam(&attr, &setparam) != 0) {
        perror("pthread_attr_setschedparam pthread_attr_getschedparam");
    }
    getparam.sched_priority = 0;
    while (state.KeepRunning()) {
        if (pthread_attr_getschedparam(&attr, &getparam) != 0) {
            perror("pthread_attr_getschedparam proc");
        }
    }
    pthread_attr_destroy(&attr);
}

static void Bm_function_pthread_condattr_setclock(benchmark::State &state)
{
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    while (state.KeepRunning()) {
        if (pthread_condattr_setclock(&attr,  CLOCK_MONOTONIC_RAW) != 0) {
            perror("pthread_condattr_setclock proc");
        }
    }
    pthread_condattr_destroy(&attr);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Tgkill(benchmark::State &state)
{
    pid_t pid = getpid();
    pid_t pgid = getpgid(pid);
    for (auto _ : state) {
        if (tgkill(pgid, pid, SIGCONT) == -1) {
            perror("tgkill proc");
        }
    }
}

// Set the scheduling parameter param of the thread attribute ATTR
static void Bm_function_pthread_attr_setschedparam(benchmark::State &state)
{
    pthread_attr_t attr;
    struct sched_param setparam;
    setparam.sched_priority = 1;
    pthread_attr_init(&attr);
    while (state.KeepRunning()) {
        if (pthread_attr_setschedparam(&attr, &setparam) != 0) {
            perror("pthread_attr_setschedparam pthread_attr_getschedparam");
        }
    }
    pthread_attr_destroy(&attr);
}
void PthreadCleanup(void* arg)
{
    // empty
}

void* CleanupPushAndPop(void* arg)
{
    benchmark::State *statePtr = (benchmark::State *)arg;
    statePtr->ResumeTiming();
    pthread_cleanup_push(PthreadCleanup, NULL);
    pthread_cleanup_pop(0);
    statePtr->PauseTiming();
    return nullptr;
}

static void Bm_function_pthread_clean_push_and_pop(benchmark::State &state)
{
    pthread_t tid;
    while (state.KeepRunning()) {
        state.PauseTiming();
        pthread_create(&tid, nullptr, CleanupPushAndPop, &state);
        pthread_join(tid, nullptr);
        state.ResumeTiming();
    }
}

MUSL_BENCHMARK(Bm_function_Sigaddset);
MUSL_BENCHMARK(Bm_function_pthread_cond_signal);
MUSL_BENCHMARK(Bm_function_pthread_cond_signal_wait);
MUSL_BENCHMARK(Bm_function_pthread_mutexattr_settype);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_errchk);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_rec);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_pi_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_pi_errorcheck);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_pi_rec);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_errchk);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_rec);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_pi_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_pi_errchk);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_pi_rec);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_tryrdlock);
MUSL_BENCHMARK(Bm_function_pthread_mutex_init);
MUSL_BENCHMARK(Bm_function_pthread_mutex_init_destroy);
MUSL_BENCHMARK(Bm_function_pthread_cond_init);
MUSL_BENCHMARK(Bm_function_pthread_cond_init_destroy);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_init);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_init_destroy);
MUSL_BENCHMARK(BM_pthread_rwlock_tryread);
MUSL_BENCHMARK(BM_pthread_rwlock_trywrite);
MUSL_BENCHMARK(Bm_function_tss_get);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_timedrdlock);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_timedwrlock);
MUSL_BENCHMARK(Bm_function_pthread_cond_timedwait);
MUSL_BENCHMARK(Bm_function_pthread_cond_broadcast);
MUSL_BENCHMARK(Bm_function_Sem_timewait);
MUSL_BENCHMARK(Bm_function_Sem_post_wait);
MUSL_BENCHMARK(Bm_function_pthread_setcancelstate);
MUSL_BENCHMARK(Bm_function_pthread_equal);
MUSL_BENCHMARK(Bm_function_pthread_attr_init_destroy);
MUSL_BENCHMARK(Bm_function_pthread_sigmask);
MUSL_BENCHMARK(Bm_function_pthread_spin_lock_and_spin_unlock);
MUSL_BENCHMARK(Bm_function_pthread_mutexattr_init_and_mutexattr_destroy);
MUSL_BENCHMARK(Bm_function_pthread_detach);
MUSL_BENCHMARK(Bm_function_pthread_setname_np);
MUSL_BENCHMARK(Bm_function_pthread_attr_setschedpolicy);
MUSL_BENCHMARK(Bm_function_pthread_attr_getschedparam);
MUSL_BENCHMARK(Bm_function_pthread_condattr_setclock);
MUSL_BENCHMARK(Bm_function_Tgkill);
MUSL_BENCHMARK(Bm_function_pthread_attr_setschedparam);
MUSL_BENCHMARK(Bm_function_pthread_clean_push_and_pop);
