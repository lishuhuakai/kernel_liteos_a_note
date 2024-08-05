#include <gtest/gtest.h>
#include <setjmp.h>
using namespace testing::ext;

class SetJmpSetJmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: setjmp_001
 * @tc.desc: Verify that the setjmp function successfully stores the calling environment in env, and returns 0
 *           when directly invoked.
 * @tc.type: FUNC
 **/
HWTEST_F(SetJmpSetJmpTest, setjmp_001, TestSize.Level1)
{
    jmp_buf env;
    EXPECT_EQ(setjmp(env), 0);
}