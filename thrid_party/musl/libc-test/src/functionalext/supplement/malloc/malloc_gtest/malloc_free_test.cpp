#include <gtest/gtest.h>
#include <malloc.h>

using namespace testing::ext;

class MallocFreeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int VALUE = 5;

/**
 * @tc.name: free_001
 * @tc.desc: This test verifies that after using the malloc function for dynamic memory allocation, when calling the
 *           free function to release memory blocks, it can correctly release memory and make the memory situation meet
 *           expectations.
 * @tc.type: FUNC
 */
HWTEST_F(MallocFreeTest, free_001, TestSize.Level1)
{
    int* memoryBlock = static_cast<int*>(malloc(sizeof(int)));
    *memoryBlock = VALUE;
    ASSERT_NE(memoryBlock, nullptr);
    EXPECT_TRUE(*memoryBlock == VALUE);
    free(memoryBlock);
}