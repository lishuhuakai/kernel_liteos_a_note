#include <ctype.h>
#include <gtest/gtest.h>
#include <locale.h>

using namespace testing::ext;

class CtypeToupperlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: toupper_l_001
 * @tc.desc: Verify whether the toupper_l() function correctly transforms characters to uppercase within the
 *           specified locale by comparing the expected results with the actual outputs.
 * @tc.type: FUNC
 **/
HWTEST_F(CtypeToupperlTest, toupper_l_001, TestSize.Level1)
{
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    int lowerCase1 = toupper_l('?', loc);
    int lowerCase2 = toupper_l('A', loc);
    int lowerCase3 = toupper_l('a', loc);
    EXPECT_EQ('?', lowerCase1);
    EXPECT_EQ('A', lowerCase2);
    EXPECT_EQ('A', lowerCase3);
    freelocale(loc);
}