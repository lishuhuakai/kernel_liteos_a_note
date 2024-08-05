#include <gtest/gtest.h>
#include <locale.h>

using namespace testing::ext;

class LocaleNewlocaleTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" locale_t __newlocale(int mask, const char* name, locale_t loc);

/**
 * @tc.name: __newlocale_001
 * @tc.desc: This test verifies that when the current region is set to "en_US.utf8", the call__newlocale function
 *           successfully sets the region to "fr-FR. utf8". The expected outcome is__newlocale function returns a value
 *           of 0, indicating a failure to set the region.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleNewlocaleTest, __newlocale_001, TestSize.Level1)
{
    setlocale(LC_ALL, "en_US.utf8");
    locale_t newLoc = __newlocale(LC_ALL, "fr_FR.utf8", nullptr);
    EXPECT_TRUE(newLoc == nullptr);
    if (newLoc != nullptr) {
        freelocale(newLoc);
    }
}