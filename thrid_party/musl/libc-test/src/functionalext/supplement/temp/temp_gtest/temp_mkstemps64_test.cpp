#include <fcntl.h>
#include <gtest/gtest.h>

using namespace testing::ext;

constexpr int BUF_SIZE = 128;

class TempMkostemps64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: mkstemps64_001
 * @tc.desc: Provide the correct template, create a temporary file.
 * @tc.type: FUNC
 * */
HWTEST_F(TempMkostemps64Test, mkstemps64_001, TestSize.Level1)
{
    char tmpFile[] = "/data/mkstemps64_XXXXXX.dat";
    int fd = mkstemps64(tmpFile, strlen(".dat"));
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