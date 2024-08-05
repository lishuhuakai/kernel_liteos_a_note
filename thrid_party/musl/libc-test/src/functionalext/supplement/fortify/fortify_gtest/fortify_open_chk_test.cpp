#include <fcntl.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class FortifyOpenChkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __open_chk_001
 * @tc.desc: Verify that the file can be opened and closed properly when using the __open_chk function to open the file.
 * @tc.type: FUNC
 */
HWTEST_F(FortifyOpenChkTest, __open_chk_001, TestSize.Level1)
{
    int file = open("test.txt", O_RDONLY | O_CREAT, 0644);
    close(file);
    int fd = __open_chk("test.txt", O_RDONLY);
    ASSERT_NE(fd, -1);
    close(fd);
    EXPECT_EQ(0, remove("test.txt"));
}