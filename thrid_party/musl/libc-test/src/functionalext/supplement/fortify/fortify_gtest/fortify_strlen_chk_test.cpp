#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE = 13;

using namespace testing::ext;

class FortifyStrlenchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __strlen_chk_001
 * @tc.desc: Verify if the function is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrlenchkTest, __strlen_chk_001, TestSize.Level1)
{
    char str[] = "Hello, world!";
    int result = __strlen_chk(str, sizeof(str));
    EXPECT_EQ(STR_SIZE, result);
}