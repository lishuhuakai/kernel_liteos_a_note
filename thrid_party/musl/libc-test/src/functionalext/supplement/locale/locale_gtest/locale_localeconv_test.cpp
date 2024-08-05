#include <errno.h>
#include <gtest/gtest.h>
#include <langinfo.h>
#include <limits.h>
#include <locale.h>

using namespace testing::ext;

class LocaleLocaleconvTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: localeconv_001
 * @tc.desc: The viewpoint of this test is to verify whether the fields in the struct lconv structure returned by
 *           localeconv match the expected empty string.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleLocaleconvTest, localeconv_001, TestSize.Level1)
{
    struct lconv* pconv = localeconv();
    EXPECT_TRUE(strcmp(".", pconv->decimal_point) == 0);
    EXPECT_TRUE(strcmp("", pconv->thousands_sep) == 0);
    EXPECT_TRUE(strcmp("", pconv->grouping) == 0);
    EXPECT_TRUE(strcmp("", pconv->int_curr_symbol) == 0);
    EXPECT_TRUE(strcmp("", pconv->currency_symbol) == 0);
    EXPECT_TRUE(strcmp("", pconv->mon_decimal_point) == 0);
    EXPECT_TRUE(strcmp("", pconv->mon_thousands_sep) == 0);
    EXPECT_TRUE(strcmp("", pconv->mon_grouping) == 0);
    EXPECT_TRUE(strcmp("", pconv->positive_sign) == 0);
    EXPECT_TRUE(strcmp("", pconv->negative_sign) == 0);
}
/**
 * @tc.name: localeconv_002
 * @tc.desc: This test verifies whether the fields in the struct lconv structure returned by localeconv are equal to
 *           CHAR_MAX.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleLocaleconvTest, localeconv_002, TestSize.Level1)
{
    struct lconv* pconv = localeconv();
    EXPECT_TRUE(pconv->int_frac_digits == CHAR_MAX);
    EXPECT_TRUE(pconv->frac_digits == CHAR_MAX);
    EXPECT_TRUE(pconv->p_cs_precedes == CHAR_MAX);
    EXPECT_TRUE(pconv->p_sep_by_space == CHAR_MAX);
    EXPECT_TRUE(pconv->n_cs_precedes == CHAR_MAX);
    EXPECT_TRUE(pconv->n_sep_by_space == CHAR_MAX);
    EXPECT_TRUE(pconv->p_sign_posn == CHAR_MAX);
    EXPECT_TRUE(pconv->n_sign_posn == CHAR_MAX);
}

/**
 * @tc.name: localeconv_003
 * @tc.desc: This test verifies the use of nl_langinfo() function consistent with the decimal character returned by the
 *           localeconv() function to ensure the correctness of the decimal character under the local setting.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleLocaleconvTest, localeconv_003, TestSize.Level1)
{
    char* radixChar = nl_langinfo(RADIXCHAR);
    ASSERT_NE(radixChar, nullptr);
    EXPECT_TRUE(strcmp(localeconv()->decimal_point, radixChar) == 0);
}

/**
 * @tc.name: localeconv_004
 * @tc.desc: The testing point of this test (in English) is to verify whether the thousands separator obtained using the
 *           nl_langinfo() function matches the thousands separator returned by the localeconv() function, in order to
 *           ensure the correctness of the thousands separator in the locale setting.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleLocaleconvTest, localeconv_004, TestSize.Level1)
{
    char* thousandsSep = nl_langinfo(THOUSEP);
    ASSERT_NE(thousandsSep, nullptr);
    EXPECT_TRUE(strcmp(localeconv()->thousands_sep, thousandsSep) == 0);
}