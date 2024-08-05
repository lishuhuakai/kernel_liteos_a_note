#include <gtest/gtest.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

class ThreadThrdTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static int ExitArg(void* arg)
{
    thrd_exit(static_cast<int>(reinterpret_cast<uintptr_t>(arg)));
}

static int ReturnArg(void* arg)
{
    return static_cast<int>(reinterpret_cast<uintptr_t>(arg));
}

/**
 * @tc.name: thrd_create_001
 * @tc.desc: Determine if the thread function creation was successful, and use thrd_join The join function saves the
 *           thread return value to a variable, and finally determines that the variable is equal to the expected value.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadThrdTest, thrd_create_001, TestSize.Level1)
{
    thrd_t thrd;
    int realResult;
    EXPECT_EQ(thrd_success, thrd_create(&thrd, ReturnArg, reinterpret_cast<void*>(1)));
    EXPECT_EQ(thrd_success, thrd_join(thrd, &realResult));
    EXPECT_EQ(1, realResult);
}

/**
 * @tc.name: thrd_create_002
 * @tc.desc: It determines that the thread function was successfully created, and it determines that the thrd_join
 *           The join function successfully saved the return value of the thread function to a null pointer
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadThrdTest, thrd_create_002, TestSize.Level1)
{
    thrd_t thrd;
    EXPECT_EQ(thrd_success, thrd_create(&thrd, ReturnArg, reinterpret_cast<void*>(1)));
    EXPECT_EQ(thrd_success, thrd_join(thrd, nullptr));
}

/**
 * @tc.name: thrd_create_003
 * @tc.desc: Determine whether the thread creation function call was successful, and determine thrd_ The detach function
 *           call was successful
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadThrdTest, thrd_create_003, TestSize.Level1)
{
    thrd_t thread;

    EXPECT_EQ(thrd_success, thrd_create(&thread, ExitArg, reinterpret_cast<void*>(1)));
    EXPECT_EQ(thrd_success, thrd_detach(thread));
}

/**
 * @tc.name: thrd_current_001
 * @tc.desc: In multithreading, determine if the current identifiers of the child thread and the main thread are not
 *           equal.
 * 2. Determine if the identifiers of the same child thread are equal
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadThrdTest, thrd_current_001, TestSize.Level1)
{
    thrd_t thread1 = thrd_current();
    thrd_t thread2 = thrd_current();
    EXPECT_TRUE(thrd_equal(thread1, thread2));
}

/**
 * @tc.name: thrd_equal_001
 * @tc.desc: Determine if the identifiers of the same child thread are equal
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadThrdTest, thrd_equal_001, TestSize.Level1)
{
    thrd_t thread1 = thrd_current();
    std::thread([&thread1] {
        thrd_t thread2 = thrd_current();
        EXPECT_FALSE(thrd_equal(thread1, thread2));
        thrd_t thread3 = thrd_current();
        EXPECT_TRUE(thrd_equal(thread2, thread3));
    }).join();
}

/**
 * @tc.name: thrd_exit_001
 * @tc.desc: Call thrd in the main thread_ When using the exit function,
 *           the program can exit normally after the last thread terminates, and the exit status code is EXIT_ SUCCESS
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadThrdTest, thrd_exit_001, TestSize.Level1)
{
    EXPECT_EXIT(thrd_exit(12), ::testing::ExitedWithCode(EXIT_SUCCESS), "");
}

/**
 * @tc.name: thrd_sleep_001
 * @tc.desc: Set a negative sleep time to determine if the return is invalidTime
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadThrdTest, thrd_sleep_001, TestSize.Level1)
{
    timespec invalidTime = { .tv_sec = -1 };
    EXPECT_EQ(-2, thrd_sleep(&invalidTime, nullptr));
}