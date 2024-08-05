#include <errno.h>
#include <gtest/gtest.h>
#include <utmp.h>

using namespace testing::ext;

class LegacyUtmpnameTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: utmpname_001
 * @tc.desc: Check whether the utmpname function behaves as expected when provided with an empty string as
 *           an argument and correctly sets errno in case of failure.
 * @tc.type: FUNC
 **/
HWTEST_F(LegacyUtmpnameTest, utmpname_001, TestSize.Level1)
{
    errno = 0;
    EXPECT_EQ(-1, utmpname(" "));
    EXPECT_EQ(ENOTSUP, errno);
}