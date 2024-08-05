#include <dirent.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class DirentAlphasort64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: alphasort64_001
 * @tc.desc: Test alphasort is passed as a callback function parameter to scandir to test the effect of alphasort
 * @tc.type: FUNC
 **/
HWTEST_F(DirentAlphasort64Test, alphasort64_001, TestSize.Level1)
{
    struct dirent** nameList;
    int n = scandir(".", &nameList, nullptr, alphasort64);

    EXPECT_GE(n, 0);
    if (n < 0) {
        perror("scandir");
    } else {
        while (n--) {
            free(nameList[n]);
        }
        free(nameList);
    }
}