#include <errno.h>
#include <gtest/gtest.h>
#include <utmp.h>

using namespace testing::ext;

class LegacyEndutentTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: endutent_001
 * @tc.desc: Ensure that the endutent() function is correctly closing the user accounting database after the iteration
 *           is complete and that the ut_user field of each user entry is not empty
 * @tc.type: FUNC
 **/
HWTEST_F(LegacyEndutentTest, endutent_001, TestSize.Level1)
{
    setutent();
    struct utmp* user;
    errno = 0;
    while ((user = getutent()) != nullptr) {
        EXPECT_NE(" ", user->ut_user);
    }
    endutent();
    EXPECT_EQ(0, errno);
}