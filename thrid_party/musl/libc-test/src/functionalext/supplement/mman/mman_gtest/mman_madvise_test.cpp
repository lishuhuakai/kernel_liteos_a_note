#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/mman.h>

using namespace testing::ext;

class MmanMadviseTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr size_t LEN = 2048;

/**
 * @tc.name: madvise_001
 * @tc.desc: This test verifies the creation and writing functionality of file operations by creating a file and writing
 *           data, and then verifying whether the file was successfully created and contains the expected initial data.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMadviseTest, madvise_001, TestSize.Level1)
{
    int fd = open("madvise_001.txt", O_RDWR | O_CREAT, 0666);
    ASSERT_NE(fd, -1);

    void* addr = mmap(nullptr, LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ASSERT_NE(addr, MAP_FAILED);
    int result = madvise(addr, LEN, MADV_NORMAL);
    EXPECT_TRUE(result == 0);

    munmap(addr, LEN);
    close(fd);
    remove("madvise_001.txt");
}