#include <dirent.h>
#include <errno.h>
#include <gtest/gtest.h>
#include <set>

using namespace testing::ext;

class DirentReaddir64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: readdir64_001
 * @tc.desc: Verify that when using the readdir64 function to read the file entries in the directory one by one, the
 *file names can be correctly obtained and the expected set of file names can be obtained.
 * @tc.type: FUNC
 */
HWTEST_F(DirentReaddir64Test, readdir64_001, TestSize.Level1)
{
    DIR* dir = opendir("/data");
    ASSERT_NE(dir, nullptr);
    std::set<std::string> fileNames;
    errno = 0;
    dirent64* entry;
    while ((entry = readdir64(dir)) != nullptr) {
        fileNames.insert(entry->d_name);
    }
    EXPECT_EQ(0, errno);
    EXPECT_EQ(closedir(dir), 0);
    ASSERT_NE(fileNames.find("."), fileNames.end());
    ASSERT_NE(fileNames.find(".."), fileNames.end());
}