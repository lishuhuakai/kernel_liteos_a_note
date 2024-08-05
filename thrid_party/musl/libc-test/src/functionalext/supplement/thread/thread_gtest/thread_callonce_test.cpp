#include <gtest/gtest.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

class ThreadCallonceTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static void CallThrdSleep()
{
    timespec shrotTime = { .tv_sec = 1 };
    thrd_sleep(&shrotTime, nullptr);
}

/**
 * @tc.name: call_once_001
 * @tc.desc: Call the function twice in a single thread, ensuring that the function is only called once
 *           Call the function twice in a row under multiple threads, ensuring that the function is only called once
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadCallonceTest, call_once_001, TestSize.Level1)
{
    timespec oTs;
    EXPECT_EQ(TIME_UTC, timespec_get(&oTs, TIME_UTC));
    int onceCall = ONCE_FLAG_INIT;
    call_once(&onceCall, CallThrdSleep);
    call_once(&onceCall, CallThrdSleep);

    std::thread([&onceCall] { call_once(&onceCall, CallThrdSleep); }).join();
    timespec dTs;
    EXPECT_EQ(TIME_UTC, timespec_get(&dTs, TIME_UTC));
    EXPECT_GE(1, dTs.tv_sec - oTs.tv_sec);
}

/**
 * @tc.name: pthread_once_001
 * @tc.desc: Call the function twice in a single thread, ensuring that the function is only called once
 *           Call the function twice in a row under multiple threads, ensuring that the function is only called once
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadCallonceTest, pthread_once_001, TestSize.Level1)
{
    timespec oTs;
    EXPECT_EQ(TIME_UTC, timespec_get(&oTs, TIME_UTC));
    pthread_once_t onceCall = PTHREAD_ONCE_INIT;
    EXPECT_EQ(0, pthread_once(&onceCall, CallThrdSleep));
    EXPECT_EQ(0, pthread_once(&onceCall, CallThrdSleep));

    std::thread([&onceCall] { pthread_once(&onceCall, CallThrdSleep); }).join();
    timespec dTs;
    EXPECT_EQ(TIME_UTC, timespec_get(&dTs, TIME_UTC));
    EXPECT_GE(1, dTs.tv_sec - oTs.tv_sec);
}