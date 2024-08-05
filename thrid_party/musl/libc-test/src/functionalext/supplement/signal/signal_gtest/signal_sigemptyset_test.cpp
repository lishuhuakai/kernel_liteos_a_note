#include <errno.h>
#include <gtest/gtest.h>
#include <signal.h>
using namespace testing::ext;

class SignalSigemptySetTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: sigemptyset_001
 * @tc.desc: Verify whether the sigemptyset() and sigaddset() functions successfully initialize an empty
 *           signal set and add two signals to the set without any errors.
 * @tc.type: FUNC
 **/
HWTEST_F(SignalSigemptySetTest, sigemptyset_001, TestSize.Level1)
{
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);

    EXPECT_EQ(0, sigemptyset(&set));
}