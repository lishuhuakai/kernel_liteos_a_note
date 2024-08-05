#include <gtest/gtest.h>
#include <pthread.h>
#include <thread>
#include <threads.h>

constexpr size_t TEST_SIZE = 65536;
constexpr size_t ENAVAIL_TIME = -100;
constexpr size_t USED_TIME = 2;

using namespace testing::ext;

class ThreadMutexTest : public testing::Test {
protected:
    pthread_mutex_t mutex_;
    mtx_t mtx_;
    timespec timeSpec_;

    void InitMutex(int mutexType, int protocol)
    {
        pthread_mutexattr_t mutexAttr;
        EXPECT_EQ(0, pthread_mutexattr_init(&mutexAttr));
        EXPECT_EQ(0, pthread_mutexattr_settype(&mutexAttr, mutexType));
        EXPECT_EQ(0, pthread_mutexattr_setprotocol(&mutexAttr, protocol));
        EXPECT_EQ(0, pthread_mutex_init(&mutex_, &mutexAttr));
        EXPECT_EQ(0, pthread_mutexattr_destroy(&mutexAttr));
    }

    void SetUp() override
    {
        EXPECT_EQ(0, pthread_mutex_init(&mutex_, nullptr));
    }

    void TearDown() override
    {
        EXPECT_EQ(0, pthread_mutex_destroy(&mutex_));
    }
};

/**
 * @tc.name: mtx_init_001
 * @tc.desc: Determine mtx_plain Is the initial session successful in plain mode.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_init_001, TestSize.Level1)
{
    EXPECT_EQ(thrd_success, mtx_init(&mtx_, mtx_plain));
    mtx_destroy(&mtx_);
}

/**
 * @tc.name: mtx_init_002
 * @tc.desc: Determine mtx_timed Is initialization successful in timed mode.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_init_002, TestSize.Level1)
{
    EXPECT_EQ(thrd_success, mtx_init(&mtx_, mtx_timed));
    mtx_destroy(&mtx_);
}

/**
 * @tc.name: mtx_init_003
 * @tc.desc: Determine mtx_plain | mtx_recursive initialization successful in recursive mode.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_init_003, TestSize.Level1)
{
    EXPECT_EQ(thrd_success, mtx_init(&mtx_, mtx_plain | mtx_recursive));
    mtx_destroy(&mtx_);
}

/**
 * @tc.name: mtx_init_004
 * @tc.desc: Determine mtx_timed | mtx_recursive initialization was successful in recursive mode.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_init_004, TestSize.Level1)
{
    EXPECT_EQ(thrd_success, mtx_init(&mtx_, mtx_timed | mtx_recursive));
    mtx_destroy(&mtx_);
}

/**
 * @tc.name: mtx_trylock_001
 * @tc.desc: Mtx_plain initialize the lock in recursive mode and lock it. Use this function to return thrd_success.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_trylock_001, TestSize.Level1)
{
    EXPECT_EQ(thrd_success, mtx_init(&mtx_, mtx_plain));
    EXPECT_EQ(thrd_success, mtx_trylock(&mtx_));
    EXPECT_EQ(thrd_busy, mtx_trylock(&mtx_));
    EXPECT_EQ(thrd_success, mtx_unlock(&mtx_));
    EXPECT_EQ(thrd_success, mtx_lock(&mtx_));
    EXPECT_EQ(thrd_busy, mtx_trylock(&mtx_));
    EXPECT_EQ(thrd_success, mtx_unlock(&mtx_));
    mtx_destroy(&mtx_);
}

/**
 * @tc.name: mtx_trylock_002
 * @tc.desc: Mtx_plain | mtx_recursive the lock in recursive mode and lock it. Use this function to return thrd_ Success
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_trylock_002, TestSize.Level1)
{
    EXPECT_EQ(thrd_success, mtx_init(&mtx_, mtx_plain | mtx_recursive));
    EXPECT_EQ(thrd_success, mtx_lock(&mtx_));
    EXPECT_EQ(thrd_success, mtx_trylock(&mtx_));
    EXPECT_EQ(thrd_success, mtx_unlock(&mtx_));
    EXPECT_EQ(thrd_success, mtx_unlock(&mtx_));
    mtx_destroy(&mtx_);
}

/**
 * @tc.name: mtx_timedlock_001
 * @tc.desc: 1. Do not set an interval to determine whether the returned joinResult is successful.
 *           2. Set an extremely short time to determine its timeout
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_timedlock_001, TestSize.Level1)
{
    mtx_t mtxTime;
    EXPECT_EQ(thrd_success, mtx_init(&mtxTime, mtx_timed));
    EXPECT_EQ(thrd_success, mtx_timedlock(&mtxTime, &timeSpec_));

    std::thread([&mtxTime] {
        EXPECT_EQ(thrd_success, mtx_init(&mtxTime, mtx_timed));
        timespec timeSpec = {};
        EXPECT_EQ(thrd_success, mtx_timedlock(&mtxTime, &timeSpec));

        timeSpec = { .tv_nsec = 1000000 };
        EXPECT_EQ(thrd_timedout, mtx_timedlock(&mtxTime, &timeSpec));
    }).join();

    EXPECT_EQ(thrd_success, mtx_unlock(&mtxTime));
    mtx_destroy(&mtxTime);
}

/**
 * @tc.name: mtx_unlock_001
 * @tc.desc: Judge whether the unlocking is successful under normal operation and whether it can be locked again after
 *           unlocking
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, mtx_unlock_001, TestSize.Level1)
{
    mtx_t mtxUnlock;
    EXPECT_EQ(thrd_success, mtx_init(&mtxUnlock, mtx_plain));
    EXPECT_EQ(thrd_success, mtx_lock(&mtxUnlock));
    std::thread([&mtxUnlock] { EXPECT_EQ(thrd_busy, mtx_trylock(&mtxUnlock)); }).join();
    EXPECT_EQ(thrd_success, mtx_unlock(&mtxUnlock));
    mtx_destroy(&mtxUnlock);
}

/**
 * @tc.name: pthread_mutex_001
 * @tc.desc: Successfully created and used a large number of different pthread keys
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_001, TestSize.Level1)
{
    std::vector<pthread_mutex_t> mutexes(TEST_SIZE);
    for (auto& mutex : mutexes) {
        EXPECT_EQ(0, pthread_mutex_init(&mutex, nullptr));
        EXPECT_EQ(0, pthread_mutex_lock(&mutex));
        EXPECT_EQ(0, pthread_mutex_unlock(&mutex));
        EXPECT_EQ(0, pthread_mutex_destroy(&mutex));
    }
    mutexes.clear();
}

/**
 * @tc.name: pthread_mutex_timedlock_001
 * @tc.desc: 1. Set the minimum timeout waiting time to obtain the lock return timeout.
 *           2. Set the einval timeout waiting time to obtain the lock return einval.
 *           3. Set the normal timeout waiting time to obtain the return lock normal.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_timedlock_001, TestSize.Level1)
{
    pthread_mutex_lock(&mutex_);
    clock_gettime(CLOCK_REALTIME, &timeSpec_);
    EXPECT_EQ(ETIMEDOUT, pthread_mutex_timedlock(&mutex_, &timeSpec_));
    timeSpec_.tv_nsec = ENAVAIL_TIME;
    EXPECT_EQ(EINVAL, pthread_mutex_timedlock(&mutex_, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
    clock_gettime(CLOCK_REALTIME, &timeSpec_);
    timeSpec_.tv_sec += USED_TIME;
    EXPECT_EQ(0, pthread_mutex_timedlock(&mutex_, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
}

/**
 * @tc.name: pthread_mutex_timedlock_002
 * @tc.desc: Verify the correctness of the pthread_mutex_timedlock, especially whether the ETIMEOUT error can be
 *           returned correctly when the waiting time expires.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_timedlock_002, TestSize.Level1)
{
    InitMutex(PTHREAD_MUTEX_NORMAL, PTHREAD_PRIO_INHERIT);
    clock_gettime(CLOCK_REALTIME, &timeSpec_);
    timeSpec_.tv_sec += 1;
    EXPECT_EQ(0, pthread_mutex_timedlock(&mutex_, &timeSpec_));
    auto timeLockThread = [](void* arg) -> void* {
        if (arg == nullptr) {
            return nullptr;
        }
        auto mutexArg = static_cast<pthread_mutex_t*>(arg);
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        intptr_t joinResult = pthread_mutex_timedlock(mutexArg, &ts);
        return reinterpret_cast<void*>(joinResult);
    };
    pthread_t thread;
    pthread_create(&thread, nullptr, timeLockThread, &mutex_);
    void* joinResult;
    pthread_join(thread, &joinResult);
    EXPECT_EQ(ETIMEDOUT, reinterpret_cast<intptr_t>(joinResult));
}

/**
 * @tc.name: pthread_mutex_timedlock_monotonic_np_001
 * @tc.desc: 1. Set the minimum timeout waiting time to obtain the lock return timeout.
 *           2. Set the einval timeout waiting time to obtain the lock return einval.
 *           3. Set the normal timeout waiting time to obtain the return lock normal.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_timedlock_monotonic_np_001, TestSize.Level1)
{
    pthread_mutex_lock(&mutex_);
    clock_gettime(CLOCK_MONOTONIC, &timeSpec_);
    EXPECT_EQ(ETIMEDOUT, pthread_mutex_timedlock_monotonic_np(&mutex_, &timeSpec_));
    timeSpec_.tv_nsec = ENAVAIL_TIME;
    EXPECT_EQ(EINVAL, pthread_mutex_timedlock_monotonic_np(&mutex_, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
    clock_gettime(CLOCK_MONOTONIC, &timeSpec_);
    timeSpec_.tv_sec += USED_TIME;
    EXPECT_EQ(0, pthread_mutex_timedlock_monotonic_np(&mutex_, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
}

/**
 * @tc.name: pthread_mutex_timedlock_monotonic_np_002
 * @tc.desc: Verify the correctness of the pthread_mutex_timedlock_monotonic_np, especially whether the ETIMEOUT error
 *           can be returned correctly when the waiting time expires.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_timedlock_monotonic_np_002, TestSize.Level1)
{
    InitMutex(PTHREAD_MUTEX_NORMAL, PTHREAD_PRIO_INHERIT);
    clock_gettime(CLOCK_MONOTONIC, &timeSpec_);
    timeSpec_.tv_sec += 1;
    EXPECT_EQ(0, pthread_mutex_timedlock_monotonic_np(&mutex_, &timeSpec_));
    auto timeLockThread = [](void* arg) -> void* {
        if (arg == nullptr) {
            return nullptr;
        }
        auto mutexArg = static_cast<pthread_mutex_t*>(arg);
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        ts.tv_sec += 1;
        intptr_t joinResult = pthread_mutex_timedlock_monotonic_np(mutexArg, &ts);
        return reinterpret_cast<void*>(joinResult);
    };
    pthread_t thread;
    pthread_create(&thread, nullptr, timeLockThread, &mutex_);
    void* joinResult;
    pthread_join(thread, &joinResult);
    EXPECT_EQ(ETIMEDOUT, reinterpret_cast<intptr_t>(joinResult));
}

/**
 * @tc.name: pthread_mutex_clocklock_001
 * @tc.desc: Under the same clock
 *           1. Set the minimum timeout waiting time to obtain the lock return timeout.
 *           2. Set the einval timeout waiting time to obtain the lock return einval.
 *           3. Set the normal timeout waiting time to obtain the return lock normal.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_clocklock_001, TestSize.Level1)
{
    pthread_mutex_lock(&mutex_);
    clock_gettime(CLOCK_MONOTONIC, &timeSpec_);
    EXPECT_EQ(ETIMEDOUT, pthread_mutex_clocklock(&mutex_, CLOCK_MONOTONIC, &timeSpec_));
    timeSpec_.tv_nsec = ENAVAIL_TIME;
    EXPECT_EQ(EINVAL, pthread_mutex_clocklock(&mutex_, CLOCK_MONOTONIC, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
    clock_gettime(CLOCK_MONOTONIC, &timeSpec_);
    timeSpec_.tv_sec += USED_TIME;
    EXPECT_EQ(0, pthread_mutex_clocklock(&mutex_, CLOCK_MONOTONIC, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
}

/**
 * @tc.name: pthread_mutex_clocklock_002
 * @tc.desc: Under the same clock
 *           1. Set the minimum timeout waiting time to obtain the lock return timeout.
 *           2. Set the einval timeout waiting time to obtain the lock return einval.
 *           3. Set the normal timeout waiting time to obtain the return lock normal
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_clocklock_002, TestSize.Level1)
{
    pthread_mutex_lock(&mutex_);
    clock_gettime(CLOCK_REALTIME, &timeSpec_);
    EXPECT_EQ(ETIMEDOUT, pthread_mutex_clocklock(&mutex_, CLOCK_REALTIME, &timeSpec_));
    timeSpec_.tv_nsec = ENAVAIL_TIME;
    EXPECT_EQ(EINVAL, pthread_mutex_clocklock(&mutex_, CLOCK_REALTIME, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
    clock_gettime(CLOCK_REALTIME, &timeSpec_);
    timeSpec_.tv_sec += USED_TIME;
    EXPECT_EQ(0, pthread_mutex_clocklock(&mutex_, CLOCK_REALTIME, &timeSpec_));
    pthread_mutex_unlock(&mutex_);
}

/**
 * @tc.name: pthread_mutex_clocklock_003
 * @tc.desc: Verify the correctness of the pthread_mutex_clocklock, especially whether the ETIMEOUT error can be
 *           returned correctly when the waiting time expires.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_clocklock_003, TestSize.Level1)
{
    InitMutex(PTHREAD_MUTEX_NORMAL, PTHREAD_PRIO_INHERIT);
    clock_gettime(CLOCK_MONOTONIC, &timeSpec_);
    timeSpec_.tv_sec += 1;
    EXPECT_EQ(0, pthread_mutex_clocklock(&mutex_, CLOCK_MONOTONIC, &timeSpec_));
    auto timeLockThread = [](void* arg) -> void* {
        if (arg == nullptr) {
            return nullptr;
        }
        auto mutexArg = static_cast<pthread_mutex_t*>(arg);
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        ts.tv_sec += 1;
        intptr_t joinResult = pthread_mutex_clocklock(mutexArg, CLOCK_MONOTONIC, &ts);
        return reinterpret_cast<void*>(joinResult);
    };
    pthread_t thread;
    pthread_create(&thread, nullptr, timeLockThread, &mutex_);
    void* joinResult;
    pthread_join(thread, &joinResult);
    EXPECT_EQ(ETIMEDOUT, reinterpret_cast<intptr_t>(joinResult));
}

/**
 * @tc.name: pthread_mutex_clocklock_004
 * @tc.desc: Verify the correctness of the pthread_mutex_clocklock, especially whether the ETIMEOUT error can be
 *           returned correctly when the waiting time expires.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_clocklock_004, TestSize.Level1)
{
    InitMutex(PTHREAD_MUTEX_NORMAL, PTHREAD_PRIO_INHERIT);
    clock_gettime(CLOCK_REALTIME, &timeSpec_);
    timeSpec_.tv_sec += 1;
    EXPECT_EQ(0, pthread_mutex_clocklock(&mutex_, CLOCK_REALTIME, &timeSpec_));
    auto timeLockThread = [](void* arg) -> void* {
        if (arg == nullptr) {
            return nullptr;
        }
        auto mutexArg = static_cast<pthread_mutex_t*>(arg);
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        intptr_t joinResult = pthread_mutex_clocklock(mutexArg, CLOCK_REALTIME, &ts);
        return reinterpret_cast<void*>(joinResult);
    };
    pthread_t thread;
    pthread_create(&thread, nullptr, timeLockThread, &mutex_);
    void* joinResult;
    pthread_join(thread, &joinResult);
    EXPECT_EQ(ETIMEDOUT, reinterpret_cast<intptr_t>(joinResult));
}

/**
 * @tc.name: pthread_mutex_clocklock_005
 * @tc.desc: Setting the wait time of thread lock with unassigned ts returns invalid
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexTest, pthread_mutex_clocklock_005, TestSize.Level1)
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    EXPECT_EQ(EINVAL, pthread_mutex_clocklock(&mutex, CLOCK_PROCESS_CPUTIME_ID, &timeSpec_));
}