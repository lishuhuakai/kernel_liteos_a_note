#include <ctype.h>
#include <gtest/gtest.h>
#include <locale.h>
using namespace testing::ext;

class CtypeTolowerlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tolower_l_001
 * @tc.desc: Verify whether the tolower_l() function correctly transforms characters to lowercase within the
 *           specified locale by comparing the expected result with the actual output.
 * @tc.type: FUNC
 **/
HWTEST_F(CtypeTolowerlTest, tolower_l_001, TestSize.Level1)
{
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    int lowerCase1 = tolower_l('?', loc);
    int lowerCase2 = tolower_l('A', loc);
    int lowerCase3 = tolower_l('a', loc);
    EXPECT_EQ('?', lowerCase1);
    EXPECT_EQ('a', lowerCase2);
    EXPECT_EQ('a', lowerCase3);
    freelocale(loc);
}