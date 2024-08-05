#include <errno.h>
#include <gtest/gtest.h>
#include <unistd.h>

using namespace testing::ext;

class ProcessUnsetenvTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: unsetenv_001
 * @tc.desc: If the variable has been successfully removed by retrieving its value using getenv,
 *           and expects the value to be nullptr.
 * @tc.type: FUNC
 **/
HWTEST_F(ProcessUnsetenvTest, unsetenv_001, TestSize.Level1)
{
    setenv("MY_VAR", "Hello", 1);
    unsetenv("MY_VAR");
    char* value = getenv("MY_VAR");
    EXPECT_EQ(nullptr, value);
}

/**
 * @tc.name: unsetenv_002
 * @tc.desc: Check how the unsetenv function handles invalid input parameters and whether it sets the errno
 *           appropriately in such cases.
 * @tc.type: FUNC
 **/
HWTEST_F(ProcessUnsetenvTest, unsetenv_002, TestSize.Level1)
{
    errno = 0;
    int result = unsetenv("");
    EXPECT_EQ(-1, result);
    EXPECT_EQ(EINVAL, errno);
    result = unsetenv("a = b");
    EXPECT_EQ(-1, result);
    EXPECT_EQ(EINVAL, errno);
}