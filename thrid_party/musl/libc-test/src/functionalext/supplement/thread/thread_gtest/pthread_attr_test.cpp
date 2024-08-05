#include <gtest/gtest.h>
#include <pthread.h>
#include <syscall.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

#define PTHREAD_CREATE_EINVAL (-1)
#define REAL_SIZE 4096

constexpr size_t TEST_MULTIPLE_ONE = 4;
constexpr size_t TEST_MULTIPLE_TWO = 64;
constexpr size_t TEST_MULTIPLE_THREE = 68;
constexpr size_t TEST_BASE = 1024;

class ThreadAttrTest : public testing::Test {
protected:
    pthread_attr_t pthreadAttrMusl;
    int attrStateMusl;

    void SetUp() override
    {
        EXPECT_EQ(0, pthread_attr_init(&pthreadAttrMusl));
    }
    void TearDown() override {}
};

/**
 * @tc.name: pthread_attr_setdetachstate_001
 * @tc.desc: Setting the PTHREAD_CREATE_DETACHED property for the thread returns success.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setdetachstate_001, TestSize.Level1)
{
    EXPECT_EQ(0, pthread_attr_setdetachstate(&pthreadAttrMusl, PTHREAD_CREATE_DETACHED));
    EXPECT_EQ(0, pthread_attr_getdetachstate(&pthreadAttrMusl, &attrStateMusl));
    EXPECT_EQ(PTHREAD_CREATE_DETACHED, attrStateMusl);
}

/**
 * @tc.name: pthread_attr_setdetachstate_002
 * @tc.desc: Setting the PTHREAD_CREATE_JOINABLE property for the thread returns success.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setdetachstate_002, TestSize.Level1)
{
    EXPECT_EQ(0, pthread_attr_setdetachstate(&pthreadAttrMusl, PTHREAD_CREATE_JOINABLE));
    EXPECT_EQ(0, pthread_attr_getdetachstate(&pthreadAttrMusl, &attrStateMusl));
    EXPECT_EQ(PTHREAD_CREATE_JOINABLE, attrStateMusl);
}

/**
 * @tc.name: pthread_attr_getdetachstate_001
 * @tc.desc: Setting the invalid property for the thread returns invalidity and obtains its status verification
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_getdetachstate_001, TestSize.Level1)
{
    EXPECT_EQ(EINVAL, pthread_attr_setdetachstate(&pthreadAttrMusl, PTHREAD_CREATE_EINVAL));
    EXPECT_EQ(0, pthread_attr_getdetachstate(&pthreadAttrMusl, &attrStateMusl));
    EXPECT_EQ(PTHREAD_CREATE_JOINABLE, attrStateMusl);
}

/**
 * @tc.name: pthread_attr_setinheritsched_001
 * @tc.desc: Set an invalid scheduling policy and inheritance mode to ensure that the thread can run normally
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setinheritsched_001, TestSize.Level1)
{
    sched_param schedParam = { .sched_priority = sched_get_priority_max(SCHED_FIFO) + 1 };
    int inHeritSched;
    pthread_attr_setschedparam(&pthreadAttrMusl, &schedParam);
    pthread_attr_setschedpolicy(&pthreadAttrMusl, SCHED_FIFO);
    EXPECT_EQ(0, pthread_attr_setinheritsched(&pthreadAttrMusl, PTHREAD_INHERIT_SCHED));
    pthread_attr_getinheritsched(&pthreadAttrMusl, &inHeritSched);
    EXPECT_EQ(PTHREAD_INHERIT_SCHED, inHeritSched);
    auto returnFunc = [](void* threadArgMusl) -> void* { return threadArgMusl; };
    pthread_t threadMusl;
    EXPECT_EQ(0, pthread_create(&threadMusl, &pthreadAttrMusl, returnFunc, nullptr));
    EXPECT_EQ(0, pthread_join(threadMusl, nullptr));
}

static void* GetAttrRealGuardSizeThread(void* threadArgMusl)
{
    pthread_attr_t guardAttributes;
    pthread_getattr_np(pthread_self(), &guardAttributes);
    pthread_attr_getguardsize(&guardAttributes, reinterpret_cast<size_t*>(threadArgMusl));
    return nullptr;
}

static size_t GetAttrRealGuardSize(const pthread_attr_t& pthreadAttrMusl)
{
    size_t attrGuardSizeResult;
    pthread_t threadMusl;
    pthread_create(&threadMusl, &pthreadAttrMusl, GetAttrRealGuardSizeThread, &attrGuardSizeResult);
    pthread_join(threadMusl, nullptr);
    return attrGuardSizeResult;
}

static void* GetAttrRealStackSizeThread(void* threadArgMusl)
{
    pthread_attr_t stackAttributes;
    pthread_getattr_np(pthread_self(), &stackAttributes);
    pthread_attr_getstacksize(&stackAttributes, reinterpret_cast<size_t*>(threadArgMusl));
    return nullptr;
}

static size_t GetAttrRealStackSize(const pthread_attr_t& pthreadAttrMusl)
{
    size_t attrStackSizeResult;
    pthread_t threadMusl;
    pthread_create(&threadMusl, &pthreadAttrMusl, GetAttrRealStackSizeThread, &attrStackSizeResult);
    pthread_join(threadMusl, nullptr);
    return attrStackSizeResult;
}

/**
 * @tc.name: pthread_attr_setguardsize_001
 * @tc.desc: Set/obtain smaller protection pthreadAttrMusl and verify equality, obtain unset true protection
 *           pthreadAttrMusl and verify correctness
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setguardsize_001, TestSize.Level1)
{
    size_t attrGuardSizeMusl;
    EXPECT_EQ(0, pthread_attr_init(&pthreadAttrMusl));
    EXPECT_EQ(0, pthread_attr_setguardsize(&pthreadAttrMusl, TEST_MULTIPLE_ONE * TEST_MULTIPLE_TWO));
    EXPECT_EQ(0, pthread_attr_getguardsize(&pthreadAttrMusl, &attrGuardSizeMusl));
    EXPECT_EQ(TEST_MULTIPLE_ONE * TEST_MULTIPLE_TWO, attrGuardSizeMusl);
    EXPECT_EQ(REAL_SIZE, GetAttrRealGuardSize(pthreadAttrMusl));
}

/**
 * @tc.name: pthread_attr_setguardsize_002
 * @tc.desc: Set/obtain the true protection attribute size and verify equality, observe if the true size changes after
 *           setting
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setguardsize_002, TestSize.Level1)
{
    size_t attrGuardSizeMusl;
    EXPECT_EQ(0, pthread_attr_setguardsize(&pthreadAttrMusl, TEST_MULTIPLE_TWO * TEST_BASE));
    EXPECT_EQ(0, pthread_attr_getguardsize(&pthreadAttrMusl, &attrGuardSizeMusl));
    EXPECT_EQ(TEST_MULTIPLE_TWO * TEST_BASE, attrGuardSizeMusl);
    EXPECT_EQ(TEST_MULTIPLE_TWO * TEST_BASE, GetAttrRealGuardSize(pthreadAttrMusl));
}

/**
 * @tc.name: pthread_attr_setguardsize_003
 * @tc.desc: Set/obtain a protection attribute that is slightly larger than the actual one
 *           and verify its equality. Observe whether the actual size changes after setting
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setguardsize_003, TestSize.Level1)
{
    size_t attrGuardSizeMusl;
    EXPECT_EQ(0, pthread_attr_setguardsize(&pthreadAttrMusl, TEST_MULTIPLE_TWO * TEST_BASE + 1));
    EXPECT_EQ(0, pthread_attr_getguardsize(&pthreadAttrMusl, &attrGuardSizeMusl));
    EXPECT_EQ(TEST_MULTIPLE_TWO * TEST_BASE + 1, attrGuardSizeMusl);
    EXPECT_EQ(TEST_MULTIPLE_THREE * TEST_BASE, GetAttrRealGuardSize(pthreadAttrMusl));
}

/**
 * @tc.name: pthread_attr_setguardsize_004
 * @tc.desc: Set/obtain protection pthreadAttrMusl that are very large compared to the real ones
 *           and verify their equality. Observe if the real size changes after setting
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setguardsize_004, TestSize.Level1)
{
    size_t attrGuardSizeMusl;
    EXPECT_EQ(0, pthread_attr_setguardsize(&pthreadAttrMusl, TEST_MULTIPLE_TWO * TEST_BASE * TEST_BASE));
    EXPECT_EQ(0, pthread_attr_getguardsize(&pthreadAttrMusl, &attrGuardSizeMusl));
    EXPECT_EQ(TEST_MULTIPLE_TWO * TEST_BASE * TEST_BASE, attrGuardSizeMusl);
    EXPECT_EQ(TEST_MULTIPLE_TWO * TEST_BASE * TEST_BASE, GetAttrRealGuardSize(pthreadAttrMusl));
}

/**
 * @tc.name: pthread_attr_setstacksize_001
 * @tc.desc: Get the default stack size and set too small stack.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setstacksize_001, TestSize.Level1)
{
    size_t einvalStatckSize;
    size_t attrStackSizeMusl;
    EXPECT_EQ(0, pthread_attr_getstacksize(&pthreadAttrMusl, &einvalStatckSize));
    EXPECT_EQ(EINVAL, pthread_attr_setstacksize(&pthreadAttrMusl, TEST_MULTIPLE_ONE * TEST_MULTIPLE_TWO));
    EXPECT_EQ(0, pthread_attr_getstacksize(&pthreadAttrMusl, &attrStackSizeMusl));
    EXPECT_EQ(einvalStatckSize, attrStackSizeMusl);
    EXPECT_GE(GetAttrRealStackSize(pthreadAttrMusl), einvalStatckSize);
}

/**
 * @tc.name: pthread_attr_setstacksize_002
 * @tc.desc: Set a larger size to be equal to the obtained one.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setstacksize_002, TestSize.Level1)
{
    size_t attrStackSizeMusl;
    EXPECT_EQ(0, pthread_attr_setstacksize(&pthreadAttrMusl, TEST_MULTIPLE_TWO * TEST_BASE));
    EXPECT_EQ(0, pthread_attr_getstacksize(&pthreadAttrMusl, &attrStackSizeMusl));
    EXPECT_EQ(TEST_MULTIPLE_TWO * TEST_BASE, attrStackSizeMusl);
    EXPECT_GE(GetAttrRealStackSize(pthreadAttrMusl), TEST_MULTIPLE_TWO * TEST_BASE);
}

/**
 * @tc.name: pthread_attr_setstacksize_003
 * @tc.desc: Setting a larger size but not aligning to determine if the system will align.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_setstacksize_003, TestSize.Level1)
{
    size_t attrStackSizeMusl;
    EXPECT_EQ(0, pthread_attr_setstacksize(&pthreadAttrMusl, TEST_MULTIPLE_TWO * TEST_BASE + 1));
    EXPECT_EQ(0, pthread_attr_getstacksize(&pthreadAttrMusl, &attrStackSizeMusl));
    EXPECT_EQ(TEST_MULTIPLE_TWO * TEST_BASE + 1, attrStackSizeMusl);
}

/**
 * @tc.name: pthread_attr_getscope_001
 * @tc.desc: Judgment of successful acquisition
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadAttrTest, pthread_attr_getscope_001, TestSize.Level1)
{
    int attrScopeMusl;
    EXPECT_EQ(0, pthread_attr_getscope(&pthreadAttrMusl, &attrScopeMusl));
    EXPECT_EQ(PTHREAD_SCOPE_SYSTEM, attrScopeMusl);
}