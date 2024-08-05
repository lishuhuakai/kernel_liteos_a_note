#include <gtest/gtest.h>
#include <stdio.h>
using namespace testing::ext;

class StdioFgetpos64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fgetpos64_001
 * @tc.desc: Ensure that the fgetpos64() function correctly retrieves the current position in the file.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFgetpos64Test, fgetpos64_001, TestSize.Level1)
{
    FILE* file = fopen("/proc/version", "r");
    ASSERT_NE(nullptr, file);
    fpos64_t position;
    int result = fgetpos64(file, &position);
    EXPECT_EQ(0, result);
    fclose(file);
}