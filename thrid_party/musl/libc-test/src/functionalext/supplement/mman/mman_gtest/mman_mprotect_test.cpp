#include <errno.h>
#include <gtest/gtest.h>
#include <sys/mman.h>

using namespace testing::ext;

class MmanMprotectTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int SIZE = 8;

/**
 * @tc.name: mprotect_001
 * @tc.desc: This test verifies that the MProtect function can successfully write data after setting the memory page to
 *           writable permission, and can still successfully read data after setting the memory page to read-only
 *           permission.
 * @tc.type: FUNC
 */
HWTEST_F(MmanMprotectTest, mprotect_001, TestSize.Level1)
{
    size_t pageSize = getpagesize();
    void* memoryBuffer = memalign(pageSize, SIZE * pageSize);
    int protectResult = mprotect(memoryBuffer, pageSize, PROT_WRITE);
    EXPECT_EQ(0, protectResult);
    char* bufferPtr = static_cast<char*>(memoryBuffer);
    strcpy(bufferPtr, "test");
    protectResult = mprotect(memoryBuffer, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC);
    EXPECT_EQ(0, protectResult);
    free(memoryBuffer);
}