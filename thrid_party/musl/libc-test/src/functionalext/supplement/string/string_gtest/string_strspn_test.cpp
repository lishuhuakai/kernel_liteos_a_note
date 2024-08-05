#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE_ONE = 6;

using namespace testing::ext;

class StringStrspnTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strspn_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrspnTest, strspn_001, TestSize.Level1)
{
    const char* srcChar = "ABCDEF123456";
    const char* dstChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    EXPECT_EQ(STR_SIZE_ONE, strspn(srcChar, dstChar));

    srcChar = nullptr;
    dstChar = nullptr;
}