#include <ctype.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class CtypeTolowerTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tolower_001
 * @tc.desc: Validate the functionality of the tolower interface, including the conversion of non-letter characters and
 *           uppercase letters.
 * @tc.type: FUNC
 */
HWTEST_F(CtypeTolowerTest, tolower_001, TestSize.Level1)
{
    EXPECT_TRUE(tolower('?') == '?');
    EXPECT_TRUE(tolower('B') == 'b');
    EXPECT_TRUE(tolower('c') == 'c');
}