#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringMemrchrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: memrchr_001
 * @tc.desc: Validate the behavior of the memrchr function when the specified character is not found in the given
 *           string.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemrchrTest, memrchr_001, TestSize.Level1)
{
    const char str[] = "hello, world";
    const char* result = static_cast<const char*>(memrchr(str, 'z', strlen(str)));
    EXPECT_EQ(result, nullptr);
}

/**
 * @tc.name: memrchr_002
 * @tc.desc: Validate the behavior of the memrchr function when given a null pointer and a length of 0.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemrchrTest, memrchr_002, TestSize.Level1)
{
    const char* str = nullptr;
    const char* result = static_cast<const char*>(memrchr(str, 'o', 0));
    EXPECT_EQ(result, nullptr);
}