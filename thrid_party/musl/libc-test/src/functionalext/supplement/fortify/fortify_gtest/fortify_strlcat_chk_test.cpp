#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#define __FORTIFY_COMPILATION
#include <fortify/string.h>

#define MAX_SIZE 1024

using namespace testing::ext;

class FortifyStrlcatchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __strlcat_chk_001
 * @tc.desc: Verify if the __strlcat_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrlcatchkTest, __strlcat_chk_001, TestSize.Level1)
{
    char srcChar[MAX_SIZE] = "Source";
    const char* dstChar = "Destination";
    __strlcat_chk(srcChar, dstChar, MAX_SIZE, MAX_SIZE);
    EXPECT_STREQ("SourceDestination", srcChar);
}
