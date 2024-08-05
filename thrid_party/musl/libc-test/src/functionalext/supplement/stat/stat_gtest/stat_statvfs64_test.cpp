#include <gtest/gtest.h>
#include <sys/statvfs.h>
using namespace testing::ext;

class StatStatvfs64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: statvfs64_001
 * @tc.desc: Verify whether the statvfs64() function successfully retrieves the filesystem
 *           statistics for the root directory ("/") without any errors.
 * @tc.type: FUNC
 **/
HWTEST_F(StatStatvfs64Test, statvfs64_001, TestSize.Level1)
{
    const char* path = "/";
    struct statvfs64 buf;
    EXPECT_EQ(0, statvfs64(path, &buf));
}