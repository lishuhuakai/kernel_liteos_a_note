#include <fcntl.h>
#include <gtest/gtest.h>

using namespace testing::ext;

constexpr int BUF_SIZE = 128;

class TempMkostemp64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: mkostemp64_001
 * @tc.desc: Verify mkostemp process success. Provide the correct template and no fixed suffix,
 *           specified in flags: O_APPEND, create a temporary file.
 * @tc.type: FUNC
 * */
HWTEST_F(TempMkostemp64Test, mkostemp64_001, TestSize.Level1)
{
    char tmpFile[] = "/data/mkostemp64_XXXXXX";
    int fd = mkostemp64(tmpFile, O_APPEND);
    EXPECT_NE(-1, fd);
    if (fd != -1) {
        int cnt = write(fd, tmpFile, strlen(tmpFile));
        EXPECT_TRUE(cnt == strlen(tmpFile));
        close(fd);
        char rmFile[BUF_SIZE];
        int len = sprintf(rmFile, "rm %s", tmpFile);
        if (len > 0) {
            system(rmFile);
        }
    }
}