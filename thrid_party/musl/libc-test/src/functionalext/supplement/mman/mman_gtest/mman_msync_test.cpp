#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/mman.h>

using namespace testing::ext;

class MmanMsyncTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: msync_001
 * @tc.desc: This test verifies whether the msync function can successfully flush data from shared memory to a temporary
 *           file on disk and return the expected success result (0).
 * @tc.type: FUNC
 */
HWTEST_F(MmanMsyncTest, msync_001, TestSize.Level1)
{
    FILE* fp = tmpfile();
    ASSERT_NE(fp, nullptr);
    const char testData[] = "Good Time!";
    size_t testDataSize = strlen(testData);
    fwrite(testData, sizeof(char), testDataSize, fp);
    int fd = fileno(fp);
    ASSERT_NE(fd, -1);
    void* map = mmap(nullptr, testDataSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ASSERT_NE(map, MAP_FAILED);
    int rev = msync(map, testDataSize, MS_ASYNC);
    EXPECT_EQ(rev, 0);
    munmap(map, testDataSize);
    fclose(fp);
}