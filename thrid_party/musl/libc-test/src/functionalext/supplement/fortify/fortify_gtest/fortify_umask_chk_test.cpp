#include <gtest/gtest.h>
#include <fortify/stat.h>

using namespace testing::ext;

class FortifyUmaskchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __umask_chk_001
 * @tc.desc: Verify if the __umask_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyUmaskchkTest, __umask_chk_001, TestSize.Level1)
{
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    mode_t result = __umask_chk(mode);
    result = __umask_chk(result);
    EXPECT_EQ(mode, result);
}