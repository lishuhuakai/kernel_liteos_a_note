#include <clocale>
#include <cwctype>
#include <gtest/gtest.h>

using namespace testing::ext;

class CtypeTowupperTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: towupper_001
 * @tc.desc: Validate the functionality of the "towupper" function by testing its behavior with specific wide
 *           character inputs and comparing the results with expected outputs.
 * @tc.type: FUNC
 **/
HWTEST_F(CtypeTowupperTest, towupper_001, TestSize.Level1)
{
    wint_t lowerCase1 = towupper(L'?');
    wint_t lowerCase2 = towupper(L'A');
    wint_t lowerCase3 = towupper(L'a');

    EXPECT_EQ('?', lowerCase1);
    EXPECT_EQ('A', lowerCase2);
    EXPECT_EQ('A', lowerCase3);
}