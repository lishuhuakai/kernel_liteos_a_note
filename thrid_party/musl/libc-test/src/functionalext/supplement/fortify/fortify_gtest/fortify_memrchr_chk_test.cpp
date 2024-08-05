#include <gtest/gtest.h>
#include <string.h>
#define __FORTIFY_COMPILATION
#include <fortify/string.h>

using namespace testing::ext;

class FortifyMempcpychkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __memrchr_chk_001
 * @tc.desc: Verify if the __memrchr_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyMempcpychkTest, __memrchr_chk_001, TestSize.Level1)
{
    const char* src = "memrchr test.";
    const char* result = reinterpret_cast<const char*>(__memrchr_chk(src, 'r', strlen(src), strlen(src)));
    EXPECT_STREQ("r test.", result);

    EXPECT_FALSE(__memrchr_chk(src, 'w', strlen(src), strlen(src)));
}