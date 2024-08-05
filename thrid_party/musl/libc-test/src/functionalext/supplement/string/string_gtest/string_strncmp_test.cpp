#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE_ONE = 3;
constexpr int STR_SIZE_TWO = 4;
constexpr int STR_SIZE_THREE = 5;
constexpr int STR_SIZE_FOUR = 6;
constexpr int STR_SIZE_FIVE = 8;

using namespace testing::ext;

class StringStrncmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strncmp_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrncmpTest, strncmp_001, TestSize.Level1)
{
    EXPECT_EQ(-1, strncmp("TEST", "test", STR_SIZE_ONE));
    EXPECT_EQ(-1, strncmp("TEST", "test", STR_SIZE_THREE));
    EXPECT_EQ(-1, strncmp("TEST_1", "test_2", STR_SIZE_FOUR));
    EXPECT_EQ(0, strncmp("TESTSTR", "TESTOTHER", STR_SIZE_TWO));
    EXPECT_EQ(1, strncmp("EFG", "ABC", STR_SIZE_ONE));
    EXPECT_EQ(1, strncmp("TESTSTR", "TESTOTHER", STR_SIZE_FIVE));
}