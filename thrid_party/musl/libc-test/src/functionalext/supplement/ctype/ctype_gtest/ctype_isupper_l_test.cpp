#include <ctype.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class CtypeIspperlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: isupper_l_001
 * @tc.desc: Validate the functionality of the isupper_l interface in identifying uppercase letters under the specified
 *           global locale, correctly recognizing uppercase letters and distinguishing lowercase letters.
 * @tc.type: FUNC
 */
HWTEST_F(CtypeIspperlTest, isupper_l_001, TestSize.Level1)
{
    EXPECT_TRUE(isupper_l('B', LC_GLOBAL_LOCALE));
    EXPECT_FALSE(isupper_l('b', LC_GLOBAL_LOCALE));
}