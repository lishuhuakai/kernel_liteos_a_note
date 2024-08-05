#include <ctype.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class CtypeToupperTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: toupper_001
 * @tc.desc: Validate the functionality of the toupper interface, including the conversion of non-letter characters and
 *           lowercase letters.
 * @tc.type: FUNC
 */
HWTEST_F(CtypeToupperTest, toupper_001, TestSize.Level1)
{
    EXPECT_TRUE(toupper('?') == '?');
    EXPECT_TRUE(toupper('b') == 'B');
    EXPECT_TRUE(toupper('C') == 'C');
}