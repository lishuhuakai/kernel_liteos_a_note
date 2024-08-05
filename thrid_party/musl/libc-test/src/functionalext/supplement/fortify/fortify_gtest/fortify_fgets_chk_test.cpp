#include <gtest/gtest.h>
#include <string.h>
#define __FORTIFY_COMPILATION
#include <fortify/stdio.h>

constexpr int STR_SIZE = 100;

using namespace testing::ext;

class FortifyFgetschkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __fgets_chk_001
 * @tc.desc: Verify if the __fgets_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyFgetschkTest, __fgets_chk_001, TestSize.Level1)
{
    char str[STR_SIZE];
    const char* ptr = "fgetstest.txt";
    const char* wrstring = "this is a test\n";
    FILE* fptr = fopen(ptr, "wr+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fflush(fptr);
    fseek(fptr, 0L, SEEK_SET);
    char* content = __fgets_chk(str, STR_SIZE, fptr, STR_SIZE);
    int result = strcmp(content, "this is a test\n");
    EXPECT_EQ(0, result);

    fclose(fptr);
    remove(ptr);
    fptr = nullptr;
    ptr = nullptr;
    wrstring = nullptr;
}