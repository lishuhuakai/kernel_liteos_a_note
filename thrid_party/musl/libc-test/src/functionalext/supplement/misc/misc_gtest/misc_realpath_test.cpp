#include <errno.h>
#include <gtest/gtest.h>
#include <limits.h>

using namespace testing::ext;

class MiscRealpathTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: realpath_001
 * @tc.desc: This test case aims to test whether the realpath () function correctly sets errno and returns nullptr when
 *           passing invalid parameters.
 * @tc.type: FUNC
 */
HWTEST_F(MiscRealpathTest, realpath_001, TestSize.Level1)
{
    errno = 0;
    const char* testPath = nullptr;
    char* ptr = realpath(testPath, nullptr);
    ASSERT_TRUE(ptr == nullptr);
    EXPECT_EQ(errno, EINVAL);
}

/**
 * @tc.name: realpath_002
 * @tc.desc: This test case aims to test whether the realpath () function correctly sets errno and returns nullptr when
 *           passing an empty path.
 * @tc.type: FUNC
 */
HWTEST_F(MiscRealpathTest, realpath_002, TestSize.Level1)
{
    errno = 0;
    char* ptr = realpath("", nullptr);
    ASSERT_TRUE(ptr == nullptr);
    EXPECT_EQ(errno, ENOENT);
}

/**
 * @tc.name: realpath_003
 * @tc.desc: This test case aims to test whether the realpath () function correctly sets errno and returns nullptr when
 *           passing non-existent paths.
 * @tc.type: FUNC
 */
HWTEST_F(MiscRealpathTest, realpath_003, TestSize.Level1)
{
    errno = 0;
    char* ptr = realpath("/musl/test", nullptr);
    ASSERT_TRUE(ptr == nullptr);
    EXPECT_EQ(errno, ENOENT);
}

/**
 * @tc.name: realpath_005
 * @tc.desc: This test case aims to test whether the realpath () function correctly sets errno and returns nullptr when
 *           passing non directory paths.
 * @tc.type: FUNC
 */
HWTEST_F(MiscRealpathTest, realpath_005, TestSize.Level1)
{
    char existingPath[] = "/tmp";
    errno = 0;
    char* ptr = realpath(existingPath, nullptr);
    ASSERT_NE(ptr, nullptr);
    free(ptr);
}

/**
 * @tc.name: realpath_006
 * @tc.desc: The purpose of this test case is to test that the realpath () function can correctly ignore the dot symbols
 *           in the path when parsing it.
 * @tc.type: FUNC
 */
HWTEST_F(MiscRealpathTest, realpath_006, TestSize.Level1)
{
    char* resolvedPath = realpath("/proc/./version", nullptr);
    EXPECT_EQ(strcmp("/proc/version", resolvedPath), 0);
    free(resolvedPath);
}

/**
 * @tc.name: realpath_007
 * @tc.desc: The purpose of this test case is to test that the realpath () function can correctly handle double dot
 *           symbols in the path when parsing it.
 * @tc.type: FUNC
 */
HWTEST_F(MiscRealpathTest, realpath_007, TestSize.Level1)
{
    char* resolvedPath = realpath("/dev/../proc/version", nullptr);
    EXPECT_EQ(strcmp("/proc/version", resolvedPath), 0);
    free(resolvedPath);
}