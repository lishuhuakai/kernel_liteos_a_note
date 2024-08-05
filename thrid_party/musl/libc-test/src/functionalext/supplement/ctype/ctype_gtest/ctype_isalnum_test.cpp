#include <ctype.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class CtypeIsalnumTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: isalnum_001
 * @tc.desc: Validate the functionality of the isalnum interface in identifying numbers and letters, and correctly
 *           recognizing characters that are not numbers or letters.
 * @tc.type: FUNC
 */
HWTEST_F(CtypeIsalnumTest, isalnum_001, TestSize.Level1)
{
    EXPECT_TRUE(isalnum('1'));
    EXPECT_TRUE(isalnum('B'));
    EXPECT_TRUE(isalnum('b'));
    EXPECT_FALSE(isalnum('@'));
}