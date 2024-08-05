#include <errno.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class MiscGetentropyTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int BUFFER_SIZE = 1024;
constexpr int SIZE = 32;

/**
 * @tc.name: getentropy_001
 * @tc.desc: This test verifies that when calling the getentropy function, the correct buffer and size parameters are
 *           passed in to ensure that the function can successfully generate random data and return a value of 0.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetentropyTest, getentropy_001, TestSize.Level1)
{
    char buffer[SIZE];
    int ret = getentropy(buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);
}

/**
 * @tc.name: getentropy_002
 * @tc.desc: The testing viewpoint of this code is to verify whether the getentropy function can correctly return an
 *           error code and set errno to EFAULT when its buf parameter is nullptr.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetentropyTest, getentropy_002, TestSize.Level1)
{
    errno = 0;
    int ret = getentropy(nullptr, 1);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(EFAULT == errno);
}

/**
 * @tc.name: getentropy_003
 * @tc.desc: The testing viewpoint of this code is to verify whether the getentropy function can correctly return an
 *           error code and set errno to EIO when it cannot generate enough entropy.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetentropyTest, getentropy_003, TestSize.Level1)
{
    char buffer[BUFFER_SIZE];
    errno = 0;
    int ret = getentropy(buffer, sizeof(buffer));
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(EIO == errno);
}