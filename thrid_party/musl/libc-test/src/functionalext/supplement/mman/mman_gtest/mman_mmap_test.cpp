#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace testing::ext;

class MmanMmapTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

const char* g_path;
FILE* g_filePtr;
constexpr int SIZE = 2048;

void CreateFileAndWrite()
{
    g_path = "mmap.txt";
    g_filePtr = fopen(g_path, "w+");
    static char content[] = "musl sample!";
    fwrite(content, sizeof(char), strlen(content), g_filePtr);
    fseek(g_filePtr, 0L, SEEK_SET);
    fclose(g_filePtr);
}

void CloseFileAndRemove(const char* path, FILE* filePtr, int fd)
{
    int result = remove(g_path);
    EXPECT_TRUE(result == 0);
    g_filePtr = nullptr;
    g_path = nullptr;
    close(fd);
}

/**
 * @tc.name: mmap_001
 * @tc.desc: This test verifies whether anonymous mapping can be successfully created using the mmap function and checks
 *           whether the returned pointer is legal; At the same time, it was also verified whether using the munmap
 *           function can correctly unload the map and release related resources after successfully creating the map.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMmapTest, mmap_001, TestSize.Level1)
{
    void* mapping = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    ASSERT_NE(MAP_FAILED, mapping);
    int munmapResult = munmap(mapping, SIZE);
    EXPECT_TRUE(munmapResult == 0);
}

/**
 * @tc.name: mmap_002
 * @tc.desc: This test verifies that when using the mmap function, when the file size is 0, the returned mapping address
 *           should be MAP_FAILED.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMmapTest, mmap_002, TestSize.Level1)
{
    CreateFileAndWrite();
    int fd = open("mmap.txt", O_RDWR | O_CREAT, 0777);
    void* mmapping = mmap(nullptr, 0, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_EQ(mmapping, MAP_FAILED);
    CloseFileAndRemove(g_path, g_filePtr, fd);
}

/**
 * @tc.name: mmap_003
 * @tc.desc: This test verifies that when the requested memory size exceeds the maximum value of the pointer type, the
 *           mmap function will return MAP_FAILED.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMmapTest, mmap_003, TestSize.Level1)
{
    CreateFileAndWrite();
    int fd = open("mmap.txt", O_RDWR | O_CREAT, 0777);
    void* mmapping = mmap(nullptr, static_cast<size_t>(PTRDIFF_MAX) + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_EQ(mmapping, MAP_FAILED);
    CloseFileAndRemove(g_path, g_filePtr, fd);
}

/**
 * @tc.name: mmap_004
 * @tc.desc: This test verifies that obtaining file information through the stat function and creating a file with PROT
 *           using the mmap function private mapping area protected by NONE, while verifying whether the starting
 *           address of this mapping is MAP_FAILED.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMmapTest, mmap_004, TestSize.Level1)
{
    CreateFileAndWrite();
    int fd = open("mmap.txt", O_RDWR | O_CREAT, 0777);
    struct stat sb;
    int status = stat(g_path, &sb);
    void* mmapping = mmap(nullptr, sb.st_size, PROT_NONE, MAP_PRIVATE | MAP_FIXED, -1, 0);
    EXPECT_EQ(status, 0);
    EXPECT_EQ(mmapping, MAP_FAILED);
    CloseFileAndRemove(g_path, g_filePtr, fd);
}

/**
 * @tc.name: mmap_005
 * @tc.desc: This test verifies that the file information is obtained through the stat function and the file pointer is
 *           closed, then, the mmap function is used to create a shared mapping area with read and write permissions,
 *           and the starting address of this mapping is verified to be not MAP_FAILED.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMmapTest, mmap_005, TestSize.Level1)
{
    CreateFileAndWrite();
    int fd = open("mmap.txt", O_RDWR | O_CREAT, 0777);
    struct stat sb;
    int status = stat(g_path, &sb);
    void* mmapping = mmap(nullptr, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_EQ(status, 0);
    ASSERT_NE(mmapping, MAP_FAILED);
    CloseFileAndRemove(g_path, g_filePtr, fd);
    int unmapResult = munmap(mmapping, sb.st_size);
    EXPECT_EQ(unmapResult, 0);
}

/**
 * @tc.name: mmap_006
 * @tc.desc: This test verifies that the file information is obtained through the stat function and the file pointer is
 *           closed, then, the mmap function is used to create a mapping area with executable permissions, private
 *           mapping, and locking flags, and to verify whether the starting address of this mapping is not MAP_FAILED.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMmapTest, mmap_006, TestSize.Level1)
{
    CreateFileAndWrite();
    int fd = open("mmap.txt", O_RDWR | O_CREAT, 0777);
    struct stat sb;
    int status = stat(g_path, &sb);
    void* mmapping = mmap(nullptr, sb.st_size, PROT_EXEC, MAP_PRIVATE | MAP_LOCKED, fd, 0);
    EXPECT_EQ(status, 0);
    ASSERT_NE(mmapping, MAP_FAILED);
    CloseFileAndRemove(g_path, g_filePtr, fd);
    int unmapResult = munmap(mmapping, sb.st_size);
    EXPECT_EQ(unmapResult, 0);
}

/**
 * @tc.name: mmap_007
 * @tc.desc: This test verifies that when attempting to use the mmap function to map beyond PTRDIFF when the memory
 *           size is MAX, it will return MAP_Failed, which means the mapping has failed.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMmapTest, mmap_007, TestSize.Level1)
{
    EXPECT_EQ(MAP_FAILED, mmap(nullptr, (PTRDIFF_MAX) + 1, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}