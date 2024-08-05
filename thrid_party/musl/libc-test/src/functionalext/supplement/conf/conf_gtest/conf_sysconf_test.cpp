#include <gtest/gtest.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>

using namespace testing::ext;

class ConfSysconfTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: sysconf_001
 * @tc.desc: Validate whether the sysconf interface correctly reports that the maximum number of user trace events is
 *           not available by checking if the value returned by sysconf(_SC_TRACE_USER_EVENT_MAX) is equal to -1.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_001, TestSize.Level1)
{
    int result = sysconf(_SC_TRACE_USER_EVENT_MAX);
    EXPECT_EQ(-1, result);
}

/**
 * @tc.name: sysconf_002
 * @tc.desc: Validate whether the sysconf interface correctly returns the maximum number of thread-specific data keys by
 *           comparing the value returned by sysconf(_SC_THREAD_KEYS_MAX) with the predefined value PTHREAD_KEYS_MAX.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_002, TestSize.Level1)
{
    int pthreadKeyMax = sysconf(_SC_THREAD_KEYS_MAX);
    EXPECT_EQ(pthreadKeyMax, PTHREAD_KEYS_MAX);
}

/**
 * @tc.name: sysconf_003
 * @tc.desc: Validate whether the sysconf interface correctly returns the clock ticks per second by comparing the value
 *           returned by sysconf(_SC_CLK_TCK) with the predefined value 100.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_003, TestSize.Level1)
{
    int result = sysconf(_SC_CLK_TCK);
    EXPECT_EQ(100, result);
}

/**
 * @tc.name: sysconf_004
 * @tc.desc: Validate whether the sysconf interface correctly returns the maximum length of time zone names by comparing
 *           the value returned by sysconf(_SC_TZNAME_MAX) with the predefined value TZNAME_MAX.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_004, TestSize.Level1)
{
    int result = sysconf(_SC_TZNAME_MAX);
    EXPECT_EQ(result, TZNAME_MAX);
}

/**
 * @tc.name: sysconf_005
 * @tc.desc: Check whether the system supports priority scheduling by verifying that the value returned by
 *           sysconf(_SC_PRIORITY_SCHEDULING) is -1.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_005, TestSize.Level1)
{
    int result = sysconf(_SC_PRIORITY_SCHEDULING);
    EXPECT_EQ(-1, result);
}

/**
 * @tc.name: sysconf_006
 * @tc.desc: Validate whether the sysconf interface correctly returns the maximum number of real-time signals by
 *           comparing the value returned by sysconf(_SC_RTSIG_MAX) with the calculated result of _NSIG - 1 - 31 - 3.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_006, TestSize.Level1)
{
    int result = sysconf(_SC_SIGQUEUE_MAX);
    EXPECT_EQ(-1, result);
}

/**
 * @tc.name: sysconf_007
 * @tc.desc: Validate whether the sysconf interface correctly returns the page size by comparing the value returned by
 *           sysconf(_SC_PAGE_SIZE) with the page size returned by the getpagesize() function.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_007, TestSize.Level1)
{
    int result = sysconf(_SC_PAGE_SIZE);
    EXPECT_EQ(result, getpagesize());
}

/**
 * @tc.name: sysconf_008
 * @tc.desc: Validate whether the sysconf interface correctly returns the maximum number of semaphores per semaphore
 *           identifier by comparing the value returned by sysconf(_SC_SEM_NSEMS_MAX) with the predefined value
 *           SEM_NSEMS_MAX.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_008, TestSize.Level1)
{
    int result = sysconf(_SC_SEM_NSEMS_MAX);
    EXPECT_EQ(result, SEM_NSEMS_MAX);
}

/**
 * @tc.name: sysconf_009
 * @tc.desc: Validate whether the sysconf interface correctly returns the maximum number of weights available for
 *           sorting rules by comparing the value returned by sysconf(_SC_COLL_WEIGHTS_MAX) with the predefined value
 *           COLL_WEIGHTS_MAX.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_009, TestSize.Level1)
{
    int result = sysconf(_SC_COLL_WEIGHTS_MAX);
    EXPECT_EQ(result, COLL_WEIGHTS_MAX);
}

/**
 * @tc.name: sysconf_010
 * @tc.desc: Validate whether the sysconf interface correctly returns the number of configured processors by comparing
 *           the value returned by sysconf(_SC_NPROCESSORS_CONF) with the number of processors returned by the
 *           get_nprocs function.
 * @tc.type: FUNC
 */
HWTEST_F(ConfSysconfTest, sysconf_010, TestSize.Level1)
{
    int result = sysconf(_SC_NPROCESSORS_CONF);
    EXPECT_EQ(result, get_nprocs());
}