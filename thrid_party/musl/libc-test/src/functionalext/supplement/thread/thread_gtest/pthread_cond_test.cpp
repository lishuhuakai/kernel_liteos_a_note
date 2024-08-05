#include <gtest/gtest.h>
#include <pthread.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

#define TS_PER_T 1000000000
constexpr int USLEEP_TIME = 5000;

class PthreadCondTest : public testing::Test {
protected:
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    bool sleepState_ = true;
    bool accessState_ = true;
    pthread_t thread_;
    timespec times_;

    void WaitingThread()
    {
        sleepState_ = true;
        pthread_create(&thread_, nullptr, reinterpret_cast<void* (*)(void*)>(WaitThreadSign), this);
        while (sleepState_) {
            usleep(USLEEP_TIME);
        }
        usleep(USLEEP_TIME);
    }

    static void WaitThreadSign(PthreadCondTest* arg)
    {
        pthread_mutex_init(&arg->mutex_, nullptr);
        ASSERT_NE(nullptr, arg);
        pthread_mutex_lock(&arg->mutex_);
        arg->sleepState_ = false;
        arg->accessState_ = true;
        while (arg->accessState_) {
            pthread_cond_wait(&arg->cond_, &arg->mutex_);
        }
        pthread_mutex_unlock(&arg->mutex_);
    }

    void SetUp() override {}
    void TearDown() override {}

    void InitMonotonic(bool monotonic = false)
    {
        pthread_mutex_init(&mutex_, nullptr);
        if (monotonic) {
            pthread_condattr_t condAttr;
            pthread_condattr_init(&condAttr);
            EXPECT_EQ(0, pthread_condattr_setclock(&condAttr, CLOCK_MONOTONIC));
            clockid_t clock;
            EXPECT_EQ(0, pthread_condattr_getclock(&condAttr, &clock));
            EXPECT_EQ(CLOCK_MONOTONIC, clock);
            EXPECT_EQ(0, pthread_cond_init(&cond_, &condAttr));
        } else {
            EXPECT_EQ(0, pthread_cond_init(&cond_, nullptr));
        }
    }

    void MakeTimeout(
        clockid_t clock, int (*testWaitFunc)(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* timeout))
    {
        pthread_mutex_lock(&mutex_);
        clock_gettime(clock, &times_);
        EXPECT_EQ(ETIMEDOUT, testWaitFunc(&cond_, &mutex_, &times_));
        times_.tv_nsec = -1;
        EXPECT_EQ(EINVAL, testWaitFunc(&cond_, &mutex_, &times_));
        pthread_mutex_unlock(&mutex_);
    }
};

/**
 * @tc.name: pthread_cond_signal_001
 * @tc.desc: Call the wait function to block the thread and determine if the individual thread wakes up successfully
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_cond_signal_001, TestSize.Level1)
{
    pthread_condattr_t condAttr;
    pthread_condattr_init(&condAttr);
    pthread_condattr_setclock(&condAttr, CLOCK_REALTIME);
    pthread_cond_init(&cond_, &condAttr);
    pthread_condattr_destroy(&condAttr);
    WaitingThread();
    accessState_ = false;
    EXPECT_EQ(0, pthread_cond_signal(&cond_));
    pthread_join(thread_, nullptr);
    pthread_cond_destroy(&cond_);
}

/**
 * @tc.name: pthread_cond_broadcast_001
 * @tc.desc: Set the clock property for the condition, set the waiting thread to determine if the function is
 *           successful, and then broadcast to wake up the waiting thread
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_cond_broadcast_001, TestSize.Level1)
{
    pthread_condattr_t condAttr;
    pthread_condattr_init(&condAttr);
    pthread_condattr_setclock(&condAttr, CLOCK_REALTIME);
    pthread_cond_init(&cond_, &condAttr);
    pthread_condattr_destroy(&condAttr);
    WaitingThread();
    accessState_ = false;
    EXPECT_EQ(0, pthread_cond_broadcast(&cond_));
    pthread_join(thread_, nullptr);
    pthread_cond_destroy(&cond_);
}

/**
 * @tc.name: pthread_cond_timedwait_001
 * @tc.desc: Set the negative timeout time of the CLOCK_REALTIME clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_REALTIME clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_REALTIME clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_cond_timedwait_001, TestSize.Level1)
{
    InitMonotonic();
    MakeTimeout(CLOCK_REALTIME, pthread_cond_timedwait);
    pthread_mutex_destroy(&mutex_);
}

/**
 * @tc.name: pthread_cond_timedwait_monotonic_np_001
 * @tc.desc: Set the negative timeout time of the CLOCK_MONOTONIC clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_MONOTONIC clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_MONOTONIC clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_cond_timedwait_monotonic_np_001, TestSize.Level1)
{
    InitMonotonic();
    MakeTimeout(CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
    InitMonotonic(true);
    MakeTimeout(CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
    pthread_mutex_destroy(&mutex_);
}

/**
 * @tc.name: pthread_cond_clockwait_001
 * @tc.desc: Set the negative timeout time of the CLOCK_MONOTONIC clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_MONOTONIC clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_MONOTONIC clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_cond_clockwait_001, TestSize.Level1)
{
    InitMonotonic();
    MakeTimeout(CLOCK_MONOTONIC, [](pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* timeout) {
        return pthread_cond_clockwait(cond, mutex, CLOCK_MONOTONIC, timeout);
    });
    InitMonotonic(true);
    MakeTimeout(CLOCK_MONOTONIC, [](pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* timeout) {
        return pthread_cond_clockwait(cond, mutex, CLOCK_MONOTONIC, timeout);
    });
    pthread_mutex_destroy(&mutex_);
}

/**
 * @tc.name: pthread_cond_clockwait_002
 * @tc.desc: Set the negative timeout time of the CLOCK_REALTIME clock as the function to determine whether the return
 *           is invalid, set the large timeout time of the CLOCK_REALTIME clock as the function to determine whether
 *           the return is invalid, and set the normal timeout time of the CLOCK_REALTIME clock as the function to
 *           determine whether the return is successful
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_cond_clockwait_002, TestSize.Level1)
{
    InitMonotonic();
    MakeTimeout(CLOCK_REALTIME, [](pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* timeout) {
        return pthread_cond_clockwait(cond, mutex, CLOCK_REALTIME, timeout);
    });
    InitMonotonic(true);
    MakeTimeout(CLOCK_REALTIME, [](pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* timeout) {
        return pthread_cond_clockwait(cond, mutex, CLOCK_REALTIME, timeout);
    });
    pthread_mutex_destroy(&mutex_);
}

/**
 * @tc.name: pthread_cond_clockwait_003
 * @tc.desc: Set invalid attribute and return corresponding error code
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_cond_clockwait_003, TestSize.Level1)
{
    pthread_cond_t pthreadCond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
    timespec times;
    EXPECT_EQ(EINVAL, pthread_cond_clockwait(&pthreadCond, &pthreadMutex, CLOCK_PROCESS_CPUTIME_ID, &times));
}

/**
 * @tc.name: pthread_condattr_init_001
 * @tc.desc: Determine whether the conditional variable sets/obtains clock attribute equality
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_condattr_init_001, TestSize.Level1)
{
    pthread_condattr_t condAttr;
    clockid_t clock;
    pthread_condattr_init(&condAttr);
    EXPECT_EQ(0, pthread_condattr_getclock(&condAttr, &clock));
    EXPECT_EQ(CLOCK_REALTIME, clock);
}

/**
 * @tc.name: pthread_condattr_setclock_001
 * @tc.desc: Determine whether the conditional variable sets/obtains clock attribute equality
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_condattr_setclock_001, TestSize.Level1)
{
    pthread_condattr_t condAttr;
    clockid_t clock;
    pthread_condattr_init(&condAttr);
    EXPECT_EQ(0, pthread_condattr_setclock(&condAttr, CLOCK_REALTIME));
    EXPECT_EQ(0, pthread_condattr_getclock(&condAttr, &clock));
    EXPECT_EQ(CLOCK_REALTIME, clock);
}

/**
 * @tc.name: pthread_condattr_setclock_002
 * @tc.desc: Determine whether the conditional variable sets/obtains clock attribute equality
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_condattr_setclock_002, TestSize.Level1)
{
    pthread_condattr_t condAttr;
    clockid_t clock;
    pthread_condattr_init(&condAttr);
    EXPECT_EQ(0, pthread_condattr_setclock(&condAttr, CLOCK_MONOTONIC));
    EXPECT_EQ(0, pthread_condattr_getclock(&condAttr, &clock));
    EXPECT_EQ(CLOCK_MONOTONIC, clock);
}

/**
 * @tc.name: pthread_condattr_setclock_003
 * @tc.desc: Determine whether the conditional variable sets/obtains clock attribute equality
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCondTest, pthread_condattr_setclock_003, TestSize.Level1)
{
    pthread_condattr_t condAttr;
    pthread_condattr_init(&condAttr);
    EXPECT_EQ(EINVAL, pthread_condattr_setclock(&condAttr, CLOCK_THREAD_CPUTIME_ID));
}