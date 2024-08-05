#include <gtest/gtest.h>
#include <resolv.h>
using namespace testing::ext;

class ResolvTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: res_init_001
 * @tc.desc: Verify that the "res_init" function successfully initializes the resolver state without encountering any
 *           errors or exceptions.
 * @tc.type: FUNC
 **/
HWTEST_F(ResolvTest, res_init_001, TestSize.Level1)
{
    EXPECT_EQ(0, res_init());
}