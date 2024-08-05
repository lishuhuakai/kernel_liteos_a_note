#include <gtest/gtest.h>

using namespace testing::ext;

class EnvGetenvTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getenv_001
 * @tc.desc: Verify whether the getenv function can correctly obtain the value of environmental variables, and
 *           verified it by setting and deleting environmental variables, and verified whether the obtained value meets
 *           expectations through assertions.
 * @tc.type: FUNC
 */
HWTEST_F(EnvGetenvTest, getenv_001, TestSize.Level1)
{
    EXPECT_TRUE(setenv("PATH", "/usr/bin:/usr/local/bin", 1) == 0);
    EXPECT_TRUE(strcmp("/usr/bin:/usr/local/bin", getenv("PATH")) == 0);
    EXPECT_TRUE(unsetenv("PATH") == 0);
    EXPECT_EQ(getenv("PATH"), nullptr);
}