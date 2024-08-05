#include <gtest/gtest.h>
#include <pthread.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

class ThreadTssTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static int g_callCount = 0;

static void TssThreadFunc(void* ptr)
{
    if (ptr == nullptr) {
        return;
    }
    ++g_callCount;
    free(ptr);
}

/**
 * @tc.name: tss_create_001
 * @tc.desc: Successfully created key
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadTssTest, tss_create_001, TestSize.Level1)
{
    tss_t tssKey;
    EXPECT_EQ(thrd_success, tss_create(&tssKey, nullptr));
    tss_delete(tssKey);
}

/**
 * @tc.name: tss_create_002
 * @tc.desc: 1. Set the function for the key and call tss once in the main thread and sub thread respectively tss_set to
 *           determine if the function in the key will be called in the sub thread, but not in the main thread
 *           2. Create and obtain key verification equality in the main thread, and verify equality in the sub thread.
 *           The sub thread key and the main thread key are not equal,
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadTssTest, tss_create_002, TestSize.Level1)
{
    tss_dtor_t dtor = TssThreadFunc;
    tss_t tssKey;
    const char* testChar = "0";
    testChar = (char*)malloc(sizeof(testChar) + 1);
    EXPECT_EQ(thrd_success, tss_create(&tssKey, dtor));
    EXPECT_EQ(thrd_success, tss_set(tssKey, const_cast<char*>("1")));
    EXPECT_STREQ("1", reinterpret_cast<char*>(tss_get(tssKey)));
    EXPECT_EQ(0, g_callCount);

    std::thread([&tssKey, testChar] {
        EXPECT_EQ(nullptr, tss_get(tssKey));
        tss_set(tssKey, const_cast<char*>(testChar));
    }).join();
    EXPECT_EQ(1, g_callCount);
    EXPECT_STREQ("1", reinterpret_cast<char*>(tss_get(tssKey)));

    g_callCount = 0;
    tss_delete(tssKey);
    EXPECT_EQ(0, g_callCount);
}