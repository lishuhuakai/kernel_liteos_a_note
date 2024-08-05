#include <gtest/gtest.h>
#include <sys/resource.h>

#define NUM_INVALID_RESOURCE 100

using namespace testing::ext;

class MiscGetrlimit64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getrlimit64_001
 * @tc.desc: This test verifies that the getrlimit64 function can successfully obtain a valid resource limit value and
 *           return an error when an invalid resource type is passed in.
 * @tc.type: FUNC
 */
HWTEST_F(MiscGetrlimit64Test, getrlimit64_001, TestSize.Level1)
{
    struct rlimit resourceLimit;
    int result = getrlimit64(RLIMIT_CPU, &resourceLimit);
    int getResult = getrlimit64(NUM_INVALID_RESOURCE, &resourceLimit);
    EXPECT_EQ(result, 0);
    EXPECT_TRUE(getResult != 0);
}