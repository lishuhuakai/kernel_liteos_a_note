#include <ctype.h>
#include <gtest/gtest.h>
#include <locale.h>

using namespace testing::ext;

class CtypeTowctranslTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: towctrans_l_001
 * @tc.desc: Verify whether the towctrans_l() function correctly performs the character mapping within the
 *           specified locale by comparing the expected result with the actual output.
 * @tc.type: FUNC
 **/
HWTEST_F(CtypeTowctranslTest, towctrans_l_001, TestSize.Level1)
{
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    wctrans_t trans = wctrans_l("toupper", loc);
    wchar_t wc = L'a';
    wchar_t converted = towctrans_l(wc, trans, loc);
    EXPECT_EQ(65, static_cast<int>(converted));
    freelocale(loc);
}