#include <errno.h>
#include <gtest/gtest.h>
#include <locale.h>

using namespace testing::ext;

class MultibyteMbrtowcTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int SIZE = 8;
constexpr int BUFFERSIZE = 4;

/**
 * @tc.name: mbrtowc_001
 * @tc.desc: This test verifies that in the C.UTF-8 character encoding environment, when using the mbrtowc function to
 *           convert an invalid multi byte sequence to a wide character, the function should return -1 and set errno to
 *           EILSEQ (invalid byte sequence).
 * @tc.type: FUNC
 */
HWTEST_F(MultibyteMbrtowcTest, mbrtowc_001, TestSize.Level1)
{
    const char* localeStr = setlocale(LC_CTYPE, "C.UTF-8");
    EXPECT_EQ(std::string("C.UTF-8"), std::string(localeStr));
    uselocale(LC_GLOBAL_LOCALE);
    wchar_t out[SIZE] = {};
    errno = 0;
    size_t ret = mbrtowc(out, "\xf5\x80\x80\x80", BUFFERSIZE, nullptr);
    EXPECT_TRUE(static_cast<size_t>(-1) == ret);
    EXPECT_TRUE(EILSEQ == errno);
}