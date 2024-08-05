#include <fnmatch.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
using namespace testing::ext;

class RegexFnmatchTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fnmatch_001
 * @tc.desc: Ensure that the fnmatch() function correctly matches the given string against the specified pattern.
 * @tc.type: FUNC
 **/
HWTEST_F(RegexFnmatchTest, fnmatch_001, TestSize.Level1)
{
    const char* dirname = "test_fnmatch.txt";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    ASSERT_NE(-1, mkdir(dirname, mode));

    const char* pattern = "*.txt";
    const char* string = "/test_fnmatch.txt";
    int result = fnmatch(pattern, string, 0);
    EXPECT_EQ(0, result);

    remove("test_fnmatch.txt");
}

/**
 * @tc.name: fnmatch_002
 * @tc.desc: Confirm that the fnmatch function correctly identifies that the given pattern does not match the
 *           specified input string.
 * @tc.type: FUNC
 **/
HWTEST_F(RegexFnmatchTest, fnmatch_002, TestSize.Level1)
{
    const char* pattern = "*.txt";
    const char* string = " ";
    int result = fnmatch(pattern, string, 0);
    EXPECT_EQ(FNM_NOMATCH, result);
}