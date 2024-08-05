#include <gtest/gtest.h>
#include <pthread.h>
#include <semaphore.h>
#include <threads.h>

using namespace testing::ext;

#define TS_PER_T 1000000000

class ThreadSemTest : public testing::Test {
protected:
    sem_t sem_;
    timespec ts_;

    static void* ThreadFn(void* arg)
    {
        if (arg == nullptr) {
            return nullptr;
        }
        sem_t& sem = *reinterpret_cast<sem_t*>(arg);
        EXPECT_EQ(0, sem_wait(&sem));
        return nullptr;
    }

    void SetUp() override
    {
        EXPECT_EQ(0, sem_init(&sem_, 0, 0));
    }

    void TearDown() override
    {
        EXPECT_EQ(0, sem_destroy(&sem_));
    }
};

/**
 * @tc.name: sem_wait_001
 * @tc.desc: Verify whether the behavior of using semaphores for thread synchronization is correct in a multithreaded
 *           environment.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadSemTest, sem_wait_001, TestSize.Level1)
{
    pthread_t thread1, thread2;
    EXPECT_EQ(0, pthread_create(&thread1, nullptr, ThreadFn, &sem_));
    EXPECT_EQ(0, pthread_create(&thread2, nullptr, ThreadFn, &sem_));

    EXPECT_EQ(0, sem_post(&sem_));
    EXPECT_EQ(0, sem_post(&sem_));

    EXPECT_EQ(0, pthread_join(thread1, nullptr));
    EXPECT_EQ(0, pthread_join(thread2, nullptr));
}

/**
 * @tc.name: sem_timedwait_001
 * @tc.desc: Illegal waiting time has been set, such as changing Set tv_nsec to -1, set tv_nsec set to exceed
 *           TS_PER_T value, to verify is the sem_timedwait function handling illegal parameters correctly.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadSemTest, sem_timedwait_001, TestSize.Level1)
{
    EXPECT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts_));
    EXPECT_EQ(-1, sem_timedwait(&sem_, &ts_));
    ts_.tv_nsec = -1;
    EXPECT_EQ(-1, sem_timedwait(&sem_, &ts_));
    ts_.tv_nsec = TS_PER_T - 1;
    ts_.tv_sec = -1;
    EXPECT_EQ(-1, sem_timedwait(&sem_, &ts_));
    EXPECT_EQ(0, sem_destroy(&sem_));
}