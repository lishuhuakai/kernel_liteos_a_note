#include <fcntl.h>
#include <gtest/gtest.h>

using namespace testing::ext;

constexpr int BUF_SIZE = 128;

class TempMkstemp64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: mkstemp64_001
 * @tc.desc: Verify mkstemp process success. Provide the correct template and no fixed suffix, create a
 *                 temporary file.
 * @tc.type: FUNC
 * */
HWTEST_F(TempMkstemp64Test, mkstemp64_001, TestSize.Level1)
{
    char tmpFile[] = "/data/mkstemp64_XXXXXX";
    int fd = mkstemp64(tmpFile);
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