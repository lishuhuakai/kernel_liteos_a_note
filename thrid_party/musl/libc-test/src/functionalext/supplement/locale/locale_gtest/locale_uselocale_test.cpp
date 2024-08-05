#include <gtest/gtest.h>
#include <locale.h>

using namespace testing::ext;

class LocaleUselocaleTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: uselocale_001
 * @tc.desc: This test verifies the behavior of the uselocal function on switching and obtaining the current local.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleUselocaleTest, uselocale_001, TestSize.Level1)
{
    locale_t initialLocale = uselocale(nullptr);
    ASSERT_NE(initialLocale, nullptr);
    locale_t newLocale = newlocale(LC_ALL, "C", nullptr);
    ASSERT_NE(newLocale, nullptr);
    ASSERT_NE(newLocale, initialLocale);
    locale_t previousLocale = uselocale(newLocale);
    ASSERT_NE(previousLocale, nullptr);
    ASSERT_EQ(previousLocale, initialLocale);
    ASSERT_EQ(newLocale, uselocale(nullptr));
}