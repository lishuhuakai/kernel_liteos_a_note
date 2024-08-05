#include <clocale>
#include <cwctype>
#include <gtest/gtest.h>

using namespace testing::ext;

class CtypeTowupperlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: towupper_l_001
 * @tc.desc: Validation of the "towupper_l" function, providing confidence that it accurately reflects
 *           locale-specific behavior for character case conversion.
 * @tc.type: FUNC
 **/
HWTEST_F(CtypeTowupperlTest, towupper_l_001, TestSize.Level1)
{
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    wint_t lowerCase1 = towupper_l(L'?', loc);
    wint_t lowerCase2 = towupper_l(L'A', loc);
    wint_t lowerCase3 = towupper_l(L'a', loc);
    EXPECT_EQ('?', lowerCase1);
    EXPECT_EQ('A', lowerCase2);
    EXPECT_EQ('A', lowerCase3);
    freelocale(loc);
}