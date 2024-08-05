#include <gtest/gtest.h>
#include <libgen.h>
using namespace testing::ext;

class MiscBasenameTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: basename_001
 * @tc.desc: Verify the functionality of the basename() function by checking if it correctly extracts the fileName
 *           from a given file path and returns a non-null pointer.
 * @tc.type: FUNC
 **/
HWTEST_F(MiscBasenameTest, basename_001, TestSize.Level1)
{
    char path[] = "/proc/version";
    char* fileName = basename(path);

    EXPECT_NE(nullptr, path);
    EXPECT_NE(nullptr, fileName);
}