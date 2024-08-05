#include <gtest/gtest.h>

using namespace testing::ext;

constexpr size_t MEMORY_SIZE = 100;

extern "C" void memtrace(void* addr, size_t size, const char* tag, bool is_using);

class HookMemtraceTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: memtrace_001
 * @tc.desc: Verify that after calling the memtrace function, the memory address memoryAddr is not successfully
 *           updated.
 * @tc.type: FUNC
 */
HWTEST_F(HookMemtraceTest, memtrace_001, TestSize.Level1)
{
    void* memoryAddr = nullptr;
    const char* tag = "data";
    bool enableTracing = true;
    memtrace(memoryAddr, MEMORY_SIZE, tag, enableTracing);
    EXPECT_FALSE(memoryAddr);
}