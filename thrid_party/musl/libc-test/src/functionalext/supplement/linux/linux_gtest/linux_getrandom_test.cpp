#include <errno.h>
#include <gtest/gtest.h>
#include <sys/random.h>

using namespace testing::ext;

class LinuxGetrandomTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getrandom_001
 * @tc.desc: This test verifies whether the getrandom function in Linux can successfully generate a non blocking random
 *           number.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxGetrandomTest, getrandom_001, TestSize.Level1)
{
    unsigned int randomValue;
    int32_t result = getrandom(&randomValue, sizeof(unsigned int), GRND_NONBLOCK);
    ASSERT_NE(result, -1);
}