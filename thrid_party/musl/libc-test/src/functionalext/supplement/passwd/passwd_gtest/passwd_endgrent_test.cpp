#include <errno.h>
#include <grp.h>
#include <gtest/gtest.h>
using namespace testing::ext;

class PasswdEndgrentTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: endgrent_001
 * @tc.desc: The endgrent function is called to close the group database and release any associated resources.
 * @tc.type: FUNC
 **/
HWTEST_F(PasswdEndgrentTest, endgrent_001, TestSize.Level1)
{
    errno = 0;
    setgrent();
    endgrent();
    EXPECT_EQ(0, errno);
}