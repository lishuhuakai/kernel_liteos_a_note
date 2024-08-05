#include <gtest/gtest.h>

using namespace testing::ext;

class PrngRandrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr unsigned int SEED1 = 100;
constexpr unsigned int SEED2 = 200;

/**
 * @tc.name: rand_r_001
 * @tc.desc: This test verifies using different seeds to call rand_r function should produce different random
 *           number results.
 * @tc.type: FUNC
 */
HWTEST_F(PrngRandrTest, rand_r_001, TestSize.Level1)
{
    unsigned int seed1 = SEED1;
    unsigned int seed2 = SEED2;
    int result1 = rand_r(&seed1);
    int result2 = rand_r(&seed2);
    EXPECT_NE(result1, result2);
}