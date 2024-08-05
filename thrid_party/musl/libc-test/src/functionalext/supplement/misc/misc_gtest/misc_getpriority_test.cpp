#include <errno.h>
#include <gtest/gtest.h>
#include <sys/resource.h>

using namespace testing::ext;

class MiscGetpriorityTest : public testing::Test {
    int priorityNum = -1, currentErrno = 0;
    void SetUp() override
    {
        priorityNum = getpriority(PRIO_USER, getuid());
        if (priorityNum == -1) {
            currentErrno = errno;
        }
        errno = 0;
    }
    void TearDown() override
    {
        if (currentErrno == 0) {
            setpriority(PRIO_USER, getuid(), priorityNum);
        }
    }
};

constexpr int PRIORITY = 10;

/**
 * @tc.name: getpriority_001
 * @tc.desc: The testing viewpoint of this test case is to verify that the getpriority function can successfully obtain
 *           the priority PRIO of the current PRIO_PROCESS and ensure that the return value matches the expected
 *           priority.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetpriorityTest, getpriority_001, TestSize.Level1)
{
    int result1 = setpriority(PRIO_PROCESS, getpid(), PRIORITY);
    EXPECT_EQ(result1, 0);
    int result2 = getpriority(PRIO_PROCESS, getpid());
    EXPECT_EQ(result2, PRIORITY);
}

/**
 * @tc.name: getpriority_002
 * @tc.desc: The testing viewpoint of this test case is to verify that the getpriority function can successfully obtain
 *           the priority PRIO of the current PRIO_PGRP and ensure that the return value matches the expected
 *           priority.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetpriorityTest, getpriority_002, TestSize.Level1)
{
    int result1 = setpriority(PRIO_PGRP, getpgid(getpid()), PRIORITY);
    EXPECT_EQ(result1, 0);
    int result2 = getpriority(PRIO_PGRP, getpgid(getpid()));
    EXPECT_EQ(result2, PRIORITY);
}

/**
 * @tc.name: getpriority_003
 * @tc.desc: The testing viewpoint of this test case is to verify that the getpriority function can successfully obtain
 *           the priority PRIO of the current PRIO_USER and ensure that the return value matches the expected
 *           priority.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetpriorityTest, getpriority_003, TestSize.Level1)
{
    int result1 = setpriority(PRIO_USER, getuid(), PRIORITY);
    EXPECT_EQ(result1, 0);
    int result2 = getpriority(PRIO_USER, getuid());
    EXPECT_EQ(result2, PRIORITY);
}