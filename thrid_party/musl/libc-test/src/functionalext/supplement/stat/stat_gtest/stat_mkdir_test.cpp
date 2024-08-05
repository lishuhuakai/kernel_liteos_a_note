#include <gtest/gtest.h>
#include <sys/stat.h>
using namespace testing::ext;

class StatMkdirTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: mkdir_001
 * @tc.desc: Verify the ability to create a directory with specific permissions and to ensure that it can be
 *           subsequently removed without errors.
 * @tc.type: FUNC
 **/
HWTEST_F(StatMkdirTest, mkdir_001, TestSize.Level1)
{
    const char* dirname = "test_mkdir";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_NE(-1, mkdir(dirname, mode));
    remove("test_mkdir");
}

/**
 * @tc.name: mkdir_002
 * @tc.desc: Verify the error handling of the mkdir() function when attempting to create a directory that already
 *           exists, and it checks if the appropriate error code (-1) is returned.
 * @tc.type: FUNC
 **/
HWTEST_F(StatMkdirTest, mkdir_002, TestSize.Level1)
{
    const char* dirname = "test_mkdir";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    mkdir(dirname, mode);
    EXPECT_EQ(-1, mkdir(dirname, mode));
    remove("test_mkdir");
}