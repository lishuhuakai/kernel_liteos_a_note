#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <pthread.h>
#include <regex>
#include <thread>
#include <threads.h>

using namespace testing::ext;

#define TS_PER_T 1000000000
constexpr int USLEEP_TIME = 1000;

class ThreadRwlockTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

class RwlockHandleControl {
public:
    explicit RwlockHandleControl(std::function<int(pthread_rwlock_t*)> mLock_function, bool mFlag = false,
        pid_t mTid = 0, std::function<int(pthread_rwlock_t*)> mTrylock_function = &pthread_rwlock_trywrlock)
    {
        wrLockTestFunc_ = mLock_function;
        timeoutFlag_ = mFlag;
        tid_ = mTid;
        tryWrLockTestFunc_ = mTrylock_function;
    };

    RwlockHandleControl(std::function<int(pthread_rwlock_t*, const timespec*)> mTimedLockFunction, clockid_t mClock,
        bool mFlag = true, pid_t mTid = 0,
        std::function<int(pthread_rwlock_t*)> mTrylock_function = &pthread_rwlock_trywrlock)
    {
        wrLockTestTimedFunc_ = mTimedLockFunction;
        clock_ = mClock;
        timeoutFlag_ = mFlag;
        tid_ = mTid;
        tryWrLockTestFunc_ = mTrylock_function;
    };

    ~RwlockHandleControl()
    {
        tryWrLockTestFunc_ = nullptr;
        wrLockTestFunc_ = nullptr;
        wrLockTestTimedFunc_ = nullptr;
    };

    static void PthreadRwlockWakeupHelper(RwlockHandleControl* arg)
    {
        ASSERT_NE(nullptr, arg);
        arg->tid_ = gettid();
        EXPECT_EQ(EBUSY, arg->tryWrLockTestFunc_(&arg->lock_));
        EXPECT_EQ(0, arg->wrLockTestFunc_(&arg->lock_));
        EXPECT_EQ(0, pthread_rwlock_unlock(&arg->lock_));
    };

    static void PthreadRwlockTimeoutHelper(RwlockHandleControl* arg)
    {
        ASSERT_NE(nullptr, arg);
        arg->tid_ = gettid();
        EXPECT_EQ(EBUSY, arg->tryWrLockTestFunc_(&arg->lock_));
        timespec times {};
        EXPECT_EQ(0, clock_gettime(arg->clock_, &times));
        EXPECT_EQ(ETIMEDOUT, arg->wrLockTestTimedFunc_(&arg->lock_, &times));
        times.tv_nsec = -1;
        EXPECT_EQ(EINVAL, arg->wrLockTestTimedFunc_(&arg->lock_, &times));
        times.tv_nsec = TS_PER_T;
        EXPECT_EQ(EINVAL, arg->wrLockTestTimedFunc_(&arg->lock_, &times));
        EXPECT_EQ(0, clock_gettime(arg->clock_, &times));
        times.tv_sec += 1;
        EXPECT_EQ(ETIMEDOUT, arg->wrLockTestTimedFunc_(&arg->lock_, &times));
    };

    static inline void WaitThreadSleep(pid_t& tid)
    {
        while (tid == 0) {
            usleep(USLEEP_TIME);
        }
        std::string filename = "/proc/" + std::to_string(tid) + "/stat";
        std::regex regex { "\\s+S\\s+" };
        while (true) {
            std::ifstream file(filename);
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            if (std::regex_search(content, regex)) {
                break;
            }
            usleep(USLEEP_TIME);
        }
    }

    static void TestPthreadRwlockWakeupWriter(RwlockHandleControl* arg)
    {
        ASSERT_NE(nullptr, arg);
        EXPECT_EQ(0, pthread_rwlock_init(&arg->lock_, nullptr));
        pthread_t thread;
        if (!arg->timeoutFlag_) {
            EXPECT_EQ(0, pthread_rwlock_rdlock(&arg->lock_));
            EXPECT_EQ(0,
                pthread_create(&thread, nullptr, reinterpret_cast<void* (*)(void*)>(PthreadRwlockWakeupHelper), arg));
            WaitThreadSleep(arg->tid_);
            EXPECT_EQ(0, pthread_rwlock_unlock(&arg->lock_));
            EXPECT_EQ(0, pthread_join(thread, nullptr));
        } else {
            EXPECT_EQ(0, pthread_rwlock_wrlock(&arg->lock_));
            EXPECT_EQ(0,
                pthread_create(&thread, nullptr, reinterpret_cast<void* (*)(void*)>(PthreadRwlockTimeoutHelper), arg));
            WaitThreadSleep(arg->tid_);
            EXPECT_EQ(0, pthread_join(thread, nullptr));
            EXPECT_EQ(0, pthread_rwlock_unlock(&arg->lock_));
        }
        EXPECT_EQ(0, pthread_rwlock_destroy(&arg->lock_));
    };

private:
    pthread_rwlock_t lock_;
    bool timeoutFlag_;
    clockid_t clock_;
    pid_t tid_;
    std::function<int(pthread_rwlock_t*)> tryWrLockTestFunc_;
    std::function<int(pthread_rwlock_t*)> wrLockTestFunc_;
    std::function<int(pthread_rwlock_t*, const timespec*)> wrLockTestTimedFunc_;
};

/**
 * @tc.name: pthread_rwlock_wrlock_001
 * @tc.desc: Set up a sleep thread to test the normal use of this interface in the current scenario
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_wrlock_001, TestSize.Level1)
{
    RwlockHandleControl* rhc = new RwlockHandleControl(&pthread_rwlock_wrlock);
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_timedwrlock_001
 * @tc.desc: Set the waiting time based on the CLOCK_REALTIME clock for this interface to determine its normal operation
 *           in the sleep thread
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_timedwrlock_001, TestSize.Level1)
{
    timespec times;
    EXPECT_EQ(0, clock_gettime(CLOCK_REALTIME, &times));
    times.tv_sec += 1;
    RwlockHandleControl* rhc =
        new RwlockHandleControl([&](pthread_rwlock_t* rwlock) { return pthread_rwlock_timedwrlock(rwlock, &times); });
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_timedwrlock_monotonic_np_001
 * @tc.desc: Set the negative timeout time of the CLOCK_MONOTONIC clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_MONOTONIC clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_MONOTONIC clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_timedwrlock_monotonic_np_001, TestSize.Level1)
{
    RwlockHandleControl* rhc = new RwlockHandleControl(&pthread_rwlock_timedwrlock_monotonic_np, CLOCK_MONOTONIC);
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_timedwrlock_monotonic_np_002
 * @tc.desc: Set the waiting time based on the CLOCK_MONOTONIC clock for this interface to determine its normal
 *           operation in the sleep thread
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_timedwrlock_monotonic_np_002, TestSize.Level1)
{
    timespec times;
    EXPECT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &times));
    times.tv_sec += 1;
    RwlockHandleControl* rhc = new RwlockHandleControl(
        [&](pthread_rwlock_t* rwlock) { return pthread_rwlock_timedwrlock_monotonic_np(rwlock, &times); });
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_clockwrlock_001
 * @tc.desc: 1.Set the waiting time based on the CLOCK_MONOTONIC clock for this interface to determine its normal
 *           operation in the sleep thread.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_clockwrlock_001, TestSize.Level1)
{
    timespec times;
    EXPECT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &times));
    times.tv_sec += 1;
    RwlockHandleControl* rhcMonotonic = new RwlockHandleControl(
        [&](pthread_rwlock_t* rwlock) { return pthread_rwlock_clockwrlock(rwlock, CLOCK_MONOTONIC, &times); });
    delete (rhcMonotonic);
    rhcMonotonic = nullptr;
}

/**
 * @tc.name: pthread_rwlock_clockwrlock_002
 * @tc.desc: Set the waiting time based on the CLOCK_REALTIME clock for this interface to determine its normal
 *           operation in the sleep thread.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_clockwrlock_002, TestSize.Level1)
{
    timespec times;
    EXPECT_EQ(0, clock_gettime(CLOCK_REALTIME, &times));
    times.tv_sec += 1;
    RwlockHandleControl* rhcRealtime = new RwlockHandleControl(
        [&](pthread_rwlock_t* rwlock) { return pthread_rwlock_clockwrlock(rwlock, CLOCK_REALTIME, &times); });
    delete (rhcRealtime);
    rhcRealtime = nullptr;
}

/**
 * @tc.name: pthread_rwlock_clockwrlock_003
 * @tc.desc: Testing the function with invalid locks returns invalid
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_clockwrlock_003, TestSize.Level1)
{
    pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
    timespec times;
    EXPECT_EQ(EINVAL, pthread_rwlock_clockwrlock(&rwlock, CLOCK_THREAD_CPUTIME_ID, &times));
}

/**
 * @tc.name: pthread_rwlock_timedrdlock_001
 * @tc.desc: Set the negative timeout time of the CLOCK_REALTIME clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_REALTIME clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_REALTIME clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_timedrdlock_001, TestSize.Level1)
{
    RwlockHandleControl* rhc = new RwlockHandleControl(&pthread_rwlock_timedrdlock, CLOCK_REALTIME);
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_timedrdlock_monotonic_np_001
 * @tc.desc: Set the negative timeout time of the CLOCK_MONOTONIC clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_MONOTONIC clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_MONOTONIC clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_timedrdlock_monotonic_np_001, TestSize.Level1)
{
    RwlockHandleControl* rhc = new RwlockHandleControl(&pthread_rwlock_timedrdlock_monotonic_np, CLOCK_MONOTONIC);
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_timedrdlock_monotonic_np_002
 * @tc.desc: Set the waiting time based on the CLOCK_MONOTONIC clock for this interface to determine its normal
 *           operation in the sleep thread
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_timedrdlock_monotonic_np_002, TestSize.Level1)
{
    timespec times;
    EXPECT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &times));
    times.tv_sec += 1;
    RwlockHandleControl* rhcMonotonic = new RwlockHandleControl(
        [&](pthread_rwlock_t* rwlock) { return pthread_rwlock_timedrdlock_monotonic_np(rwlock, &times); });
    delete (rhcMonotonic);
    rhcMonotonic = nullptr;
}

/**
 * @tc.name: pthread_rwlock_clockrdlock_001
 * @tc.desc: Set the negative timeout time of the CLOCK_MONOTONIC clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_MONOTONIC clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_MONOTONIC clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_clockrdlock_001, TestSize.Level1)
{
    RwlockHandleControl* rhc = new RwlockHandleControl(
        [&](pthread_rwlock_t* __rwlock, const timespec* __timeout) {
            return pthread_rwlock_clockrdlock(__rwlock, CLOCK_MONOTONIC, __timeout);
        },
        CLOCK_MONOTONIC);
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_clockrdlock_002
 * @tc.desc: Set the negative timeout time of the CLOCK_REALTIME clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_REALTIME clock as the function to determine whether the
 *           return is invalid, and set the normal timeout time of the CLOCK_REALTIME clock as the function to determine
 *           whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_clockrdlock_002, TestSize.Level1)
{
    RwlockHandleControl* rhc = new RwlockHandleControl(
        [&](pthread_rwlock_t* __rwlock, const timespec* __timeout) {
            return pthread_rwlock_clockrdlock(__rwlock, CLOCK_REALTIME, __timeout);
        },
        CLOCK_REALTIME);
    RwlockHandleControl::TestPthreadRwlockWakeupWriter(rhc);
    delete (rhc);
    rhc = nullptr;
}

/**
 * @tc.name: pthread_rwlock_clockrdlock_003
 * @tc.desc: Testing the function with invalid locks returns invalid
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_clockrdlock_003, TestSize.Level1)
{
    pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
    timespec times;
    EXPECT_EQ(EINVAL, pthread_rwlock_clockrdlock(&rwlock, CLOCK_THREAD_CPUTIME_ID, &times));
}

/**
 * @tc.name: pthread_rwlock_001
 * @tc.desc: Single read rwlock: obtaining read rwlock and unlocking.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_001, TestSize.Level1)
{
    pthread_rwlock_t wrLock;
    EXPECT_EQ(0, pthread_rwlock_init(&wrLock, nullptr));
    EXPECT_EQ(0, pthread_rwlock_rdlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_destroy(&wrLock));
}

/**
 * @tc.name: pthread_rwlock_002
 * @tc.desc: Multiple read locks: Continuously acquire multiple read locks and unlock them.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_002, TestSize.Level1)
{
    pthread_rwlock_t wrLock;
    EXPECT_EQ(0, pthread_rwlock_init(&wrLock, nullptr));
    EXPECT_EQ(0, pthread_rwlock_rdlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_rdlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_destroy(&wrLock));
}

/**
 * @tc.name: pthread_rwlock_003
 * @tc.desc: Write rwlock: Obtain the write rwlock and unlock it.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_003, TestSize.Level1)
{
    pthread_rwlock_t wrLock;
    EXPECT_EQ(0, pthread_rwlock_init(&wrLock, nullptr));
    EXPECT_EQ(0, pthread_rwlock_wrlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_destroy(&wrLock));
}

/**
 * @tc.name: pthread_rwlock_004
 * @tc.desc: Attempt to write rwlock: Attempt to obtain the write rwlock. The first successful attempt, the second
 *           attempt, and the attempt to read the rwlock should all return EBUSY.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_004, TestSize.Level1)
{
    pthread_rwlock_t wrLock;
    EXPECT_EQ(0, pthread_rwlock_init(&wrLock, nullptr));
    EXPECT_EQ(0, pthread_rwlock_trywrlock(&wrLock));
    EXPECT_EQ(EBUSY, pthread_rwlock_trywrlock(&wrLock));
    EXPECT_EQ(EBUSY, pthread_rwlock_tryrdlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_destroy(&wrLock));
}

/**
 * @tc.name: pthread_rwlock_005
 * @tc.desc: Attempt to read rwlock: Attempt to obtain read rwlock, successful first and second attempts, attempt to
 *           write rwlock should return EBUSY, and then unlock twice.
 *           Obtain write rwlock: After unlocking, obtain the write rwlock again, which should be successful and
 *           immediately unlocked.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlock_005, TestSize.Level1)
{
    pthread_rwlock_t wrLock;
    EXPECT_EQ(0, pthread_rwlock_init(&wrLock, nullptr));
    EXPECT_EQ(0, pthread_rwlock_tryrdlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_tryrdlock(&wrLock));
    EXPECT_EQ(EBUSY, pthread_rwlock_trywrlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_wrlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_unlock(&wrLock));
    EXPECT_EQ(0, pthread_rwlock_destroy(&wrLock));
}

/**
 * @tc.name: pthread_rwlockattr_001
 * @tc.desc: Determine whether the set shared attribute is equal to the obtained shared attribute
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadRwlockTest, pthread_rwlockattr_001, TestSize.Level1)
{
    pthread_rwlockattr_t rwlockAttr;
    int pthreadShared;
    EXPECT_EQ(0, pthread_rwlockattr_init(&rwlockAttr));
    EXPECT_EQ(0, pthread_rwlockattr_setpshared(&rwlockAttr, PTHREAD_PROCESS_PRIVATE));
    EXPECT_EQ(0, pthread_rwlockattr_getpshared(&rwlockAttr, &pthreadShared));
    EXPECT_EQ(PTHREAD_PROCESS_PRIVATE, pthreadShared);
    EXPECT_EQ(0, pthread_rwlockattr_setpshared(&rwlockAttr, PTHREAD_PROCESS_SHARED));
    EXPECT_EQ(0, pthread_rwlockattr_getpshared(&rwlockAttr, &pthreadShared));
    EXPECT_EQ(PTHREAD_PROCESS_SHARED, pthreadShared);
    EXPECT_EQ(0, pthread_rwlockattr_destroy(&rwlockAttr));
}