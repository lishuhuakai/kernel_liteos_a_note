#include <gtest/gtest.h>
#include <locale.h>

using namespace testing::ext;

class LocaleStrtoflTest : public ::testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strtof_l_001
 * @tc.desc: This test is validated by using strtof_ Verify that the input string "100.1" can be correctly converted to
 *           the corresponding floating-point value 100.1 using the l function and specific localization environment.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleStrtoflTest, strtof_l_001, TestSize.Level1)
{
    const char* inputValue = "100.1";
    char* lastptr;
    locale_t locale = newlocale(LC_ALL_MASK, "A", nullptr);
    float endresult = strtof_l(inputValue, &lastptr, locale);
    EXPECT_FLOAT_EQ(endresult, 100.1f);
    freelocale(locale);
}