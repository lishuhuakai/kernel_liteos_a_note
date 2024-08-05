#include <gtest/gtest.h>
#include <pthread.h>

using namespace testing::ext;
constexpr int USLEEP_TIME = 1000;

class PthreadCleanupTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

void CleanupFunc(void* arg)
{
    if (arg == nullptr) {
        return;
    }
    EXPECT_EQ(1, *((int*)(arg)));
}

void* ThreadFn(void* arg)
{
    if (arg == nullptr) {
        return nullptr;
    }
    int cleanupArg = 1;
    struct __ptcb cleanup;
    cleanup.__f = &CleanupFunc;
    cleanup.__x = reinterpret_cast<void*>(&cleanupArg);
    cleanup.__next = nullptr;
    _pthread_cleanup_push(&cleanup, cleanup.__f, cleanup.__x);
    usleep(USLEEP_TIME);

    _pthread_cleanup_pop(&cleanup, 1);
    return arg;
}

/**
 * @tc.name: pthread_cleanup_001
 * @tc.desc: Verify whether the function can perform the clear operation correctly in a multithreaded environment.
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadCleanupTest, pthread_cleanup_001, TestSize.Level1)
{
    pthread_t pt;
    EXPECT_EQ(0, pthread_create(&pt, nullptr, ThreadFn, nullptr));
    EXPECT_EQ(0, pthread_join(pt, nullptr));
}