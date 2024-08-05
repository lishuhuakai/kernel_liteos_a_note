#include <fcntl.h>
#include <gtest/gtest.h>
#include <unistd.h>
using namespace testing::ext;

class ConfConfstrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: confstr_001
 * @tc.desc: Test the usage of confstr to determine the required size for a specific configuration option,
 *           in this case, the maximum length of a pathname.
 * @tc.type: FUNC
 **/
HWTEST_F(ConfConfstrTest, confstr_001, TestSize.Level1)
{
    long pathMax;
    pathMax = confstr(_CS_PATH, nullptr, 0);
    EXPECT_LE(0, pathMax);
}