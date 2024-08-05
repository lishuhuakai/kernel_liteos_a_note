#include <gtest/gtest.h>
#include <utmp.h>

using namespace testing::ext;

class LegacyGetutentTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getutent_001
 * @tc.desc: Verify whether the behavior of old Unix/Linux library functions such as getutent, setutent, endutent, and
 *           pututline under specific conditions meets expectations.
 * @tc.type: FUNC
 * */
HWTEST_F(LegacyGetutentTest, getutent_001, TestSize.Level1)
{
    EXPECT_EQ(-1, utmpname("test"));
    setutent();
    EXPECT_EQ(nullptr, getutent());
}