#include <gtest/gtest.h>
#include <unistd.h>

using namespace testing::ext;

class LegacyGetpagesizeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getpagesize_001
 * @tc.desc: Verify that the page size returned by the getpagesize function is equal to the natural page size of the
 *           hardware platform.
 * @tc.type: FUNC
 */
HWTEST_F(LegacyGetpagesizeTest, getpagesize_001, TestSize.Level1)
{
    long naturalPageSize = getpagesize();
    long pageSize = sysconf(_SC_PAGESIZE);
    EXPECT_EQ(pageSize, naturalPageSize);
}