#include <gtest/gtest.h>
#include <stdlib.h>
using namespace testing::ext;

class StdlibDivTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int NUMONE = 17;
constexpr int NUMTWO = 5;
constexpr int EXPONE = 3;
constexpr int EXPTWO = 2;
/**
 * @tc.name: div_001
 * @tc.desc: Verify the functionality of the div() function by checking if it correctly computes the quotient and
 *           remainder of division between two given integers.
 * @tc.type: FUNC
 **/
HWTEST_F(StdlibDivTest, div_001, TestSize.Level1)
{
    div_t result = div(NUMONE, NUMTWO);
    EXPECT_EQ(EXPONE, result.quot);
    EXPECT_EQ(EXPTWO, result.rem);
}