#include <cxxabi.h>
#include <gtest/gtest.h>
#include <stdint.h>
#include <stdlib.h>

using namespace testing::ext;

class ExitCxaAtexitTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" int __cxa_atexit(void (*func)(void*), void* arg, void* dso);
extern "C" void __cxa_finalize(void* dso);

/**
 * @tc.name: __cxa_atexit_001
 * @tc.desc: Verify the functionality of the __cxa_atexit and __cxa_finalize functions, ensuring that registered exit
 *           handlers are properly executed when the program exits.
 * @tc.type: FUNC
 */
HWTEST_F(ExitCxaAtexitTest, __cxa_atexit_001, TestSize.Level1)
{
    int exitCount = 0;
    __cxa_atexit([](void* arg) { ++*static_cast<int*>(arg); }, &exitCount, &exitCount);
    __cxa_finalize(&exitCount);
    EXPECT_EQ(exitCount, 1);
    __cxa_finalize(&exitCount);
    EXPECT_EQ(exitCount, 1);
}