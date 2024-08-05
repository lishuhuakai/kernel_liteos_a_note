#include <gtest/gtest.h>
#include <sys/sysinfo.h>

using namespace testing::ext;

class ConfGetNprocsTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: get_nprocs_001
 * @tc.desc: Verify whether the get_nprocs interface returns a processor core count greater than 0.
 * @tc.type: FUNC
 */
HWTEST_F(ConfGetNprocsTest, get_nprocs_001, TestSize.Level1)
{
    int numProcs = get_nprocs();
    EXPECT_GT(numProcs, 0);
}