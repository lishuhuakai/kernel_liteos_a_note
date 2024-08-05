#include <errno.h>
#include <gtest/gtest.h>
#include <malloc.h>
#include <stdint.h>

using namespace testing::ext;

class MallocCallocTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr size_t LEN = 200;
constexpr size_t SIZE = 100;

/**
 * @tc.name: calloc_001
 * @tc.desc: The testing viewpoint is to verify whether the calloc function can correctly handle errors and return
 *           nullptr when passing illegal parameters, and whether errno is set to the appropriate error code.
 * @tc.type: FUNC
 */
HWTEST_F(MallocCallocTest, calloc_001, TestSize.Level1)
{
    void* memoryBlock = calloc(-1, LEN);
    EXPECT_EQ(memoryBlock, nullptr);
    EXPECT_EQ(errno, ENOMEM);
}

/**
 * @tc.name: calloc_002
 * @tc.desc: This test verifies that the calloc function can successfully allocate a memory block of the specified size,
 *           and the allocated memory block size meets the expected value.
 * @tc.type: FUNC
 */
HWTEST_F(MallocCallocTest, calloc_002, TestSize.Level1)
{
    void* block = calloc(1, SIZE);
    ASSERT_NE(block, nullptr);
    EXPECT_LE(SIZE, malloc_usable_size(block));
    free(block);
}

/**
 * @tc.name: calloc_003
 * @tc.desc: This test verifies that when calling the calloc function, passing 0 as the number and size of elements, the
 *           function can successfully allocate memory and return a non null pointer.
 * @tc.type: FUNC
 */
HWTEST_F(MallocCallocTest, calloc_003, TestSize.Level1)
{
    void* memoryBlock = calloc(0, 0);
    ASSERT_NE(memoryBlock, nullptr);
    free(memoryBlock);
}

/**
 * @tc.name: calloc_004
 * @tc.desc: This test verifies that when calling the calloc function, passing 0 as the number of elements and non-zero
 *           size, the function can successfully allocate memory and return a non-null pointer.
 * @tc.type: FUNC
 */
HWTEST_F(MallocCallocTest, calloc_004, TestSize.Level1)
{
    void* memoryBlock = calloc(0, 1);
    ASSERT_NE(memoryBlock, nullptr);
    free(memoryBlock);
}

/**
 * @tc.name: calloc_005
 * @tc.desc: This test verifies that when calling the calloc function, the function can successfully allocate memory and
 *           return a non null pointer when passing in a non-zero number of elements and a zero size.
 * @tc.type: FUNC
 */
HWTEST_F(MallocCallocTest, calloc_005, TestSize.Level1)
{
    void* memoryBlock = calloc(1, 0);
    ASSERT_NE(memoryBlock, nullptr);
    free(memoryBlock);
}