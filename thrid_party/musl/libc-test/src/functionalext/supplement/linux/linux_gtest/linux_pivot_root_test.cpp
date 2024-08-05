#include <gtest/gtest.h>

using namespace testing::ext;

class LinuxPivotrootTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" int pivot_root(const char*, const char*);

/**
 * @tc.name: pivot_root_001
 * @tc.desc: Returns an exception when verifying that the parameter path is empty.
 * @tc.type: FUNC
 * */
HWTEST_F(LinuxPivotrootTest, pivot_root_001, TestSize.Level1)
{
    int result = pivot_root(nullptr, nullptr);
    EXPECT_EQ(-1, result);
}