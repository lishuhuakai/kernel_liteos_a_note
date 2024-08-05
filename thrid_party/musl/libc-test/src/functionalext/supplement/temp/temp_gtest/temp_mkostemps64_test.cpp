#include <fcntl.h>
#include <gtest/gtest.h>
using namespace testing::ext;

class TempMkostemps64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: mkostemps64_001
 * @tc.desc: Assist in verifying and validating software that involves working with temporary files
 * @tc.type: FUNC
 **/
HWTEST_F(TempMkostemps64Test, mkostemps64_001, TestSize.Level1)
{
    char templateName[] = "/tmp/mytempXXXXXX";
    int suffixlen = 0;
    int flags = O_CREAT | O_RDWR;

    int fd = mkostemps64(templateName, suffixlen, flags);
    EXPECT_NE(-1, fd);
    close(fd);
    remove(templateName);
}