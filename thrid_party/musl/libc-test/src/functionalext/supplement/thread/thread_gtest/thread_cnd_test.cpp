#include <gtest/gtest.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

class ThreadCndTest : public testing::Test {
protected:
    mtx_t threadMtx;
    cnd_t threadCnd;

    void SetUp() override
    {
        EXPECT_EQ(thrd_success, mtx_init(&threadMtx, mtx_plain));
        EXPECT_EQ(thrd_success, cnd_init(&threadCnd));
    }
    void TearDown() override
    {
        mtx_destroy(&threadMtx);
        cnd_destroy(&threadCnd);
    }
};

class WaitThread {
public:
    WaitThread(mtx_t* threadMtx, cnd_t* threadCnd, int* threadIndex)
    {
        mtx_ = threadMtx;
        cnd_ = threadCnd;
        index_ = threadIndex;
    };

    ~WaitThread() {};

    void WaitrBroadCast()
    {
        EXPECT_EQ(thrd_success, mtx_lock(this->mtx_));
        while (*index_ != 1) {
            EXPECT_EQ(thrd_success, cnd_wait(this->cnd_, this->mtx_));
        }
        EXPECT_EQ(thrd_success, mtx_unlock(this->mtx_));
    };

    void WaitrSignal()
    {
        EXPECT_EQ(thrd_success, mtx_lock(this->mtx_));
        EXPECT_EQ(thrd_success, cnd_wait(this->cnd_, this->mtx_));
        EXPECT_EQ(thrd_success, mtx_unlock(this->mtx_));
        (*index_)++;
    };

    void ChangeInit()
    {
        EXPECT_EQ(thrd_success, mtx_lock(mtx_));
        (*index_)++;
        EXPECT_EQ(thrd_success, mtx_unlock(mtx_));
    };

private:
    mtx_t* mtx_;
    cnd_t* cnd_;
    int* index_;
};

/**
 * @tc.name: cnd_broadcast_001
 * @tc.desc: Testing whether calling cnd_broadcast to wake up all blocked threads can succeed in multithreading
 *           situations
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadCndTest, cnd_broadcast_001, TestSize.Level1)
{
    int threadIndex = 0;

    WaitThread wt(&threadMtx, &threadCnd, &threadIndex);

    std::thread thread1([&]() { wt.WaitrBroadCast(); });
    std::thread thread2([&]() { wt.WaitrBroadCast(); });

    wt.ChangeInit();

    EXPECT_EQ(thrd_success, cnd_broadcast(&threadCnd));
    thread1.join();
    thread2.join();
}

/**
 * @tc.name: cnd_signal_001
 * @tc.desc: Testing whether calling cnd_signal to wake up all blocked threads can succeed in multithreading situations
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadCndTest, cnd_signal_001, TestSize.Level1)
{
    int count = 0;
    WaitThread wt(&threadMtx, &threadCnd, &count);

    std::thread thread1([&]() { wt.WaitrSignal(); });
    std::thread thread2([&]() { wt.WaitrSignal(); });

    usleep(10000);
    EXPECT_EQ(thrd_success, cnd_signal(&threadCnd));
    while (count == 0) {
    }
    usleep(10000);
    EXPECT_EQ(thrd_success, cnd_signal(&threadCnd));
    while (count == 1) {
    }
    thread1.join();
    thread2.join();
}

/**
 * @tc.name: cnd_timedwait_001
 * @tc.desc: Test whether calling a function returns a timeout without setting a conditional wait time
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadCndTest, cnd_timedwait_001, TestSize.Level1)
{
    cnd_t threadCnd;
    timespec ts = {};
    mtx_lock(&threadMtx);
    cnd_init(&threadCnd);
    EXPECT_EQ(thrd_timedout, cnd_timedwait(&threadCnd, &threadMtx, &ts));
}