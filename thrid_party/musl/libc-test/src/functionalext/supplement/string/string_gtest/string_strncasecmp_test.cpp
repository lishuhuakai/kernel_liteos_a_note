#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE_ONE = 3;
constexpr int STR_SIZE_TWO = 4;
constexpr int STR_SIZE_THREE = 5;
constexpr int STR_SIZE_FOUR = 6;

using namespace testing::ext;

class StringStrncasecmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strncasecmp_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrncasecmpTest, strncasecmp_001, TestSize.Level1)
{
    EXPECT_EQ(0, strncasecmp("TEST", "test", STR_SIZE_ONE));
    EXPECT_EQ(0, strncasecmp("TESTSTR", "TESTOTHER", STR_SIZE_TWO));
    EXPECT_EQ(0, strncasecmp("TEST", "test", STR_SIZE_THREE));
    EXPECT_EQ(-1, strncasecmp("TEST_1", "test_2", STR_SIZE_FOUR));
}