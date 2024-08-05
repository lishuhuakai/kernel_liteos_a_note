#include <gtest/gtest.h>
#include <stdlib.h>

using namespace testing::ext;

class PrngRandTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: rand_001
 * @tc.desc: This test verifies whether the random number returned by consecutive calls to the rand function after
 *           setting the seed value to 0x01020304 using the srand function meets the expected value.
 * @tc.type: FUNC
 */
HWTEST_F(PrngRandTest, rand_001, TestSize.Level1)
{
    srand(0x010203);
    EXPECT_EQ(610060228, rand());
    EXPECT_EQ(1081226626, rand());
    EXPECT_EQ(612507619, rand());
    EXPECT_EQ(1272242898, rand());
}