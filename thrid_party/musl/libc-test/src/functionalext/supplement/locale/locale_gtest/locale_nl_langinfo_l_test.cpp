#include <gtest/gtest.h>
#include <langinfo.h>
#include <locale.h>

using namespace testing::ext;

class LocaleNllanginfolTest : public ::testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: nl_langinfo_l_001
 * @tc.desc: This test verifies that after setting a specific locale, nl is called langinfo_l function obtains the
 *           decimal separator and ensures that valid decimal separator information can be successfully obtained.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleNllanginfolTest, nl_langinfo_l_001, TestSize.Level1)
{
    setlocale(LC_ALL, "A");
    locale_t testLocale = newlocale(LC_ALL, "A", nullptr);
    const char* decimalSeparator = nl_langinfo_l(RADIXCHAR, testLocale);
    ASSERT_NE(decimalSeparator, nullptr);
    if (decimalSeparator != nullptr) {
        EXPECT_TRUE(strlen(decimalSeparator) > 0);
    } else {
        FAIL() << "Failed to retrieve decimal point information";
    }
    freelocale(testLocale);
}