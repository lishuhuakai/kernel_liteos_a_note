#include <gtest/gtest.h>
#include <sys/sysinfo.h>

using namespace testing::ext;

class ConfGetNprocsConfTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: get_procs_001
 * @tc.desc: Verify whether the get_nprocs_conf interface returns a configuration processor core count greater than or
 *           equal to 0.
 * @tc.type: FUNC
 */
HWTEST_F(ConfGetNprocsConfTest, get_nprocs_conf_001, TestSize.Level1)
{
    int numProcsConf = get_nprocs_conf();
    EXPECT_GE(numProcsConf, 0);
}