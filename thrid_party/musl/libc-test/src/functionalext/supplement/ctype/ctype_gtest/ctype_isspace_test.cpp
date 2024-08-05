#include <ctype.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class CtypeIsspaceTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: isspace_001
 * @tc.desc: Verify the correctness of the isspace interface, testing whether tab, carriage return, and space are
 *           recognized as whitespace characters, and whether a non-whitespace character is correctly identified.
 * @tc.type: FUNC
 */
HWTEST_F(CtypeIsspaceTest, isspace_001, TestSize.Level1)
{
    EXPECT_TRUE(isspace('\t'));
    EXPECT_TRUE(isspace('\r'));
    EXPECT_TRUE(isspace(' '));
    EXPECT_FALSE(isspace('a'));
}