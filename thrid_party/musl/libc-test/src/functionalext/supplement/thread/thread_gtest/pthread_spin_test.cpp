#include <gtest/gtest.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

class PthreadSpinTest : public testing::Test {
protected:
    pthread_spinlock_t spinLock;

    void SetUp() override
    {
        EXPECT_EQ(0, pthread_spin_init(&spinLock, 0));
    }
    void TearDown() override
    {
        EXPECT_EQ(0, pthread_spin_destroy(&spinLock));
    }
};

/**
 * @tc.name: pthread_spinlock_001
 * @tc.desc: Verify that the trylock sucessful.
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadSpinTest, pthread_spinlock_001, TestSize.Level1)
{
    EXPECT_EQ(0, pthread_spin_trylock(&spinLock));
    EXPECT_EQ(0, pthread_spin_unlock(&spinLock));
}

/**
 * @tc.name: pthread_spinlock_002
 * @tc.desc: Verify that the trylock interface cannot lock a lock that has already been locked again.
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadSpinTest, pthread_spinlock_002, TestSize.Level1)
{
    EXPECT_EQ(0, pthread_spin_lock(&spinLock));
    EXPECT_EQ(EBUSY, pthread_spin_trylock(&spinLock));
    EXPECT_EQ(0, pthread_spin_unlock(&spinLock));
}