#include <gtest/gtest.h>
#include <malloc.h>

using namespace testing::ext;

class MallocMallocusablesizeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr size_t INITIALSIZE = 100;
constexpr int SIZE = 2;

/**
 * @tc.name: malloc_usable_size_001
 * @tc.desc: This test verifies malloc_usable_size function can correctly return the available space size after
 *           reallocating memory, and verify that the realloc function can successfully reallocate memory while
 *           retaining the original data.
 * @tc.type: FUNC
 */
HWTEST_F(MallocMallocusablesizeTest, malloc_usable_size_001, TestSize.Level1)
{
    void* p = malloc(INITIALSIZE);
    ASSERT_NE(p, nullptr);
    p = realloc(p, INITIALSIZE * SIZE);
    ASSERT_NE(p, nullptr);
    size_t ret = malloc_usable_size(p);
    EXPECT_GE(ret, INITIALSIZE * SIZE);
    free(p);
}