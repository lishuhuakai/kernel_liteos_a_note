#include <gtest/gtest.h>
#include <pthread.h>
#include <threads.h>

using namespace testing::ext;
class ThreadBarrierTest : public testing::Test {
protected:
    pthread_barrierattr_t barrierAttrMusl;
    int pthreadBarrierShareAttr;

    void SetUp() override
    {
        EXPECT_EQ(0, pthread_barrierattr_init(&barrierAttrMusl));
    }
    void TearDown() override
    {
        EXPECT_EQ(0, pthread_barrierattr_destroy(&barrierAttrMusl));
    }
};

/**
 * @tc.name: pthread_barrierattr_setpshared_001
 * @tc.desc: Obtain correct initial attribute judgment, set PTHREAD_PROCESS_SHARED attribute to obtain equal judgment,
 *           and destroy attribute successfully
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadBarrierTest, pthread_barrierattr_setpshared_001, TestSize.Level1)
{
    EXPECT_EQ(0, pthread_barrierattr_setpshared(&barrierAttrMusl, PTHREAD_PROCESS_SHARED));
    EXPECT_EQ(0, pthread_barrierattr_getpshared(&barrierAttrMusl, &pthreadBarrierShareAttr));
    EXPECT_EQ(PTHREAD_PROCESS_SHARED, pthreadBarrierShareAttr);
    EXPECT_EQ(EINVAL, pthread_barrierattr_setpshared(&barrierAttrMusl, -1));
    EXPECT_EQ(0, pthread_barrierattr_getpshared(&barrierAttrMusl, &pthreadBarrierShareAttr));
    EXPECT_EQ(PTHREAD_PROCESS_SHARED, pthreadBarrierShareAttr);
}

/**
 * @tc.name: pthread_barrierattr_getpshared_001
 * @tc.desc: Obtain correct initial attribute judgment, set PTHREAD_PROCESS_SHARED attribute to obtain equal judgment,
 *           and destroy attribute successfully
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadBarrierTest, pthread_barrierattr_getpshared_001, TestSize.Level1)
{
    EXPECT_EQ(0, pthread_barrierattr_getpshared(&barrierAttrMusl, &pthreadBarrierShareAttr));
    EXPECT_EQ(PTHREAD_PROCESS_PRIVATE, pthreadBarrierShareAttr);
}