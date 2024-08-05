#include <gtest/gtest.h>
#include <sys/mman.h>

using namespace testing::ext;

class HookMunmapTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

#define MMAP_SIZE 2048

/**
 * @tc.name: munmap_001
 * @tc.desc: Verify the use of the munmap function to release memory, and expect a return value of 0 to indicate
 *           successful release of the specified memory block.
 * @tc.type: FUNC
 */
HWTEST_F(HookMunmapTest, munmap_001, TestSize.Level1)
{
    void* ptr = mmap(nullptr, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    ASSERT_NE(MAP_FAILED, ptr);
    EXPECT_TRUE(munmap(ptr, MMAP_SIZE) == 0);
}

/**
 * @tc.name: munmap_002
 * @tc.desc: Verify the use of the munmap function to release memory, and expect a return value of 0 to indicate
 *           successful release of the specified memory block.
 * @tc.type: FUNC
 */
HWTEST_F(HookMunmapTest, munmap_002, TestSize.Level1)
{
    void* ptr = mmap64(nullptr, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    ASSERT_NE(MAP_FAILED, ptr);
    EXPECT_TRUE(munmap(ptr, MMAP_SIZE) == 0);
}