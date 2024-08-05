#include <gtest/gtest.h>
#include <stdint.h>

using namespace testing::ext;

constexpr int SIZE = 128;
constexpr int ILLEGAL_SIZE = -128;

class HookReallocTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: realloc_001
 * @tc.desc: Validate the behavior of the realloc function when given a null pointer and a size of 0, ensuring that
 *           realloc behaves correctly in boundary cases and returns a valid pointer.
 * @tc.type: FUNC
 */
HWTEST_F(HookReallocTest, realloc_001, TestSize.Level1)
{
    void* ptr = realloc(nullptr, 0);
    ASSERT_NE(ptr, nullptr);
    free(ptr);
}
/**
 * @tc.name: realloc_002
 * @tc.desc: Validate the behavior of the realloc function when given a null pointer and a non-zero size, ensuring that
 *           realloc can allocate memory correctly and return a valid pointer.
 * @tc.type: FUNC
 */
HWTEST_F(HookReallocTest, realloc_002, TestSize.Level1)
{
    void* ptr = realloc(nullptr, SIZE);
    ASSERT_NE(ptr, nullptr);
    free(ptr);
}
/**
 * @tc.name: realloc_003
 * @tc.desc: Validate the behavior of the realloc function when attempting to allocate memory with an illegal size,
 *           ensuring that realloc correctly handles illegal size scenarios and returns a null pointer.
 * @tc.type: FUNC
 */
HWTEST_F(HookReallocTest, realloc_003, TestSize.Level1)
{
    void* ptr = realloc(nullptr, ILLEGAL_SIZE);
    EXPECT_EQ(ptr, nullptr);
}
/**
 * @tc.name: realloc_004
 * @tc.desc: Validate the behavior of the realloc function when reallocation is performed on an already allocated memory
 *           block, ensuring that realloc correctly handles the situation and returns a valid pointer.
 * @tc.type: FUNC
 */
HWTEST_F(HookReallocTest, realloc_004, TestSize.Level1)
{
    void* ptr = malloc(SIZE);
    ptr = realloc(ptr, SIZE);
    ASSERT_NE(ptr, nullptr);
    free(ptr);
}

/**
 * @tc.name: realloc_005
 * @tc.desc: Verify that the realloc function correctly returns a non-null pointer when expanding the size of the
 *           original memory block to twice its original size, ensuring its proper functionality.
 * @tc.type: FUNC
 */
HWTEST_F(HookReallocTest, realloc_005, TestSize.Level1)
{
    void* ptr = malloc(SIZE);
    ptr = realloc(ptr, SIZE * 2);
    ASSERT_NE(ptr, nullptr);
    free(ptr);
}

/**
 * @tc.name: realloc_006
 * @tc.desc: Verify that the realloc function correctly returns a non-null pointer when reducing the size of the
 *           original memory block by half, ensuring its proper functionality.
 * @tc.type: FUNC
 */
HWTEST_F(HookReallocTest, realloc_006, TestSize.Level1)
{
    void* ptr = malloc(SIZE);
    ptr = realloc(ptr, SIZE / 2);
    ASSERT_NE(ptr, nullptr);
    free(ptr);
}
