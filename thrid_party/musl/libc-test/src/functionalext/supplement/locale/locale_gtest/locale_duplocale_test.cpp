#include <gtest/gtest.h>
#include <locale.h>
using namespace testing::ext;

class LocaleDuplocaleTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

bool SameLocale(locale_t loc1, locale_t loc2)
{
    wchar_t buf[] = { L'A', L'B', L'Z', L'Å', L'Ä', L'Ö', L'\0' };

    for (int i = 0; buf[i] != L'\0'; ++i) {
        wchar_t c = buf[i];

        wchar_t lower1 = towlower_l(c, loc1);
        wchar_t lower2 = towlower_l(c, loc2);
        if (lower1 != lower2) {
            return false;
        }
    }

    return true;
}

extern "C" locale_t __duplocale(locale_t old);
/**
 * @tc.name: __duplocale_001
 * @tc.desc: Verify that the new object copied by __duplocale is the same as the original object
 * @tc.type: FUNC
 **/
HWTEST_F(LocaleDuplocaleTest, __duplocale_001, TestSize.Level1)
{
    locale_t loc1 = newlocale(LC_ALL_MASK, "en_US.UTF-8", nullptr);
    locale_t loc2 = __duplocale(loc1);
    EXPECT_TRUE(SameLocale(loc1, loc2));

    freelocale(loc1);
    freelocale(loc2);
}