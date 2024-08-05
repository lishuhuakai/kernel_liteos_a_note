#include <gtest/gtest.h>
#include <string.h>
#define __FORTIFY_COMPILATION
#include <fortify/stdio.h>

using namespace testing::ext;

class FortifyFwritechkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __fwrite_chk_001
 * @tc.desc: Verify if the function is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyFwritechkTest, __fwrite_chk_001, TestSize.Level1)
{
    FILE* fptr = fopen("/data/fwritetest.txt", "w+");
    EXPECT_TRUE(fptr);

    char buf[] = "fwritetest";
    int result = __fwrite_chk(buf, sizeof(char), strlen(buf), fptr, sizeof(buf));
    EXPECT_EQ(result, strlen(buf));

    fclose(fptr);
    remove("/data/fwritetest.txt");
}