#include <gtest/gtest.h>
#include <pthread.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

class ThreadMutexattrTest : public testing::Test {
protected:
    pthread_mutexattr_t mutexAttr_;

    void SetUp() override
    {
        EXPECT_EQ(0, pthread_mutexattr_init(&mutexAttr_));
    }
    void TearDown() override
    {
        EXPECT_EQ(0, pthread_mutexattr_destroy(&mutexAttr_));
    }
};

/**
 * @tc.name: pthread_mutexattr_gettype_001
 * @tc.desc: Set PTHREAD_MUTEX_NORMAL for mutex, call this function to obtain the properties, and then verify that it
 *           is equal to expected
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexattrTest, pthread_mutexattr_gettype_001, TestSize.Level1)
{
    int pthreadAttrType;

    EXPECT_EQ(0, pthread_mutexattr_settype(&mutexAttr_, PTHREAD_MUTEX_NORMAL));
    EXPECT_EQ(0, pthread_mutexattr_gettype(&mutexAttr_, &pthreadAttrType));
    EXPECT_EQ(PTHREAD_MUTEX_NORMAL, pthreadAttrType);
}

/**
 * @tc.name: pthread_mutexattr_gettype_002
 * @tc.desc: Set PTHREAD_MUTEX_ERRORCHECK for mutex, call this function to obtain the properties, and then verify that
 *           it is equal to expected
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexattrTest, pthread_mutexattr_gettype_002, TestSize.Level1)
{
    int pthreadAttrType;

    EXPECT_EQ(0, pthread_mutexattr_settype(&mutexAttr_, PTHREAD_MUTEX_ERRORCHECK));
    EXPECT_EQ(0, pthread_mutexattr_gettype(&mutexAttr_, &pthreadAttrType));
    EXPECT_EQ(PTHREAD_MUTEX_ERRORCHECK, pthreadAttrType);
}

/**
 * @tc.name: pthread_mutexattr_gettype_003
 * @tc.desc: Set PTHREAD_MUTEX_RECURSIVE for mutex, call this function to obtain the properties, and then verify that it
 *           is equal to expected
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexattrTest, pthread_mutexattr_gettype_003, TestSize.Level1)
{
    int pthreadAttrType;

    EXPECT_EQ(0, pthread_mutexattr_settype(&mutexAttr_, PTHREAD_MUTEX_RECURSIVE));
    EXPECT_EQ(0, pthread_mutexattr_gettype(&mutexAttr_, &pthreadAttrType));
    EXPECT_EQ(PTHREAD_MUTEX_RECURSIVE, pthreadAttrType);
}

/**
 * @tc.name: pthread_mutexattr_gettype_004
 * @tc.desc: Set PTHREAD_MUTEX_DEFAULT for mutex, call this function to obtain the properties, and then verify that it
 *           is equal to expected
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexattrTest, pthread_mutexattr_gettype_004, TestSize.Level1)
{
    int pthreadAttrType;

    EXPECT_EQ(0, pthread_mutexattr_settype(&mutexAttr_, PTHREAD_MUTEX_DEFAULT));
    EXPECT_EQ(0, pthread_mutexattr_gettype(&mutexAttr_, &pthreadAttrType));
    EXPECT_EQ(PTHREAD_MUTEX_DEFAULT, pthreadAttrType);
}

/**
 * @tc.name: pthread_mutexattr_setprotocol_001
 * @tc.desc: Verify pthread_mutexattr_t setting and obtaining the Protocol attribute in the t structure works normally
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadMutexattrTest, pthread_mutexattr_setprotocol_001, TestSize.Level1)
{
    int destProtocol;
    EXPECT_EQ(0, pthread_mutexattr_getprotocol(&mutexAttr_, &destProtocol));
    EXPECT_EQ(PTHREAD_PRIO_NONE, destProtocol);

    EXPECT_EQ(0, pthread_mutexattr_setprotocol(&mutexAttr_, PTHREAD_PRIO_NONE));
    EXPECT_EQ(0, pthread_mutexattr_getprotocol(&mutexAttr_, &destProtocol));
    EXPECT_EQ(destProtocol, PTHREAD_PRIO_NONE);

    EXPECT_EQ(0, pthread_mutexattr_setprotocol(&mutexAttr_, PTHREAD_PRIO_INHERIT));
    EXPECT_EQ(0, pthread_mutexattr_getprotocol(&mutexAttr_, &destProtocol));
    EXPECT_EQ(destProtocol, PTHREAD_PRIO_INHERIT);
}