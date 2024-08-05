#include <gtest/gtest.h>

using namespace testing::ext;

extern "C" int init_module(void* a, unsigned long b, const char* c);

class LinuxInitmoduleTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: init_module_001
 * @tc.desc: Verify if the init_module operation is normal.
 * @tc.type: FUNC
 * */
HWTEST_F(LinuxInitmoduleTest, init_module_001, TestSize.Level1)
{
    int result1 = init_module(reinterpret_cast<void*>(const_cast<char*>("example_module")), 0, nullptr);
    int result2 = init_module(nullptr, 0, nullptr);
    EXPECT_EQ(-1, result1);
    EXPECT_EQ(-1, result2);
}