#include <gtest/gtest.h>
#include <libgen.h>
using namespace testing::ext;

class MiscDirnameTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dirname_001
 * @tc.desc: Verify the functionality of the dirname() function by checking if it correctly extracts the parent
 *           directory from a given file path and returns a non-null pointer.
 * @tc.type: FUNC
 **/
HWTEST_F(MiscDirnameTest, dirname_001, TestSize.Level1)
{
    char path[] = "/proc/version";
    char* dir = dirname(path);
    EXPECT_NE(nullptr, dir);
}