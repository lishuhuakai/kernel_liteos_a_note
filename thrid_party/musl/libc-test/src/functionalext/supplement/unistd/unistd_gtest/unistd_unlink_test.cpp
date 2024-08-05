#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdUnlinkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: unlink_001
 * @tc.desc: Test that the function is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdUnlinkTest, unlink_001, TestSize.Level1)
{
    const char* filename = "absent_test";

    EXPECT_EQ(-1, unlink(filename));
}