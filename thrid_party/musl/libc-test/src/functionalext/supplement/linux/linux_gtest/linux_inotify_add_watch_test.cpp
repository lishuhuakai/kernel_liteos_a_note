#include <gtest/gtest.h>
#include <sys/inotify.h>

using namespace testing::ext;

class LinuxInotifyaddwatchTest : public ::testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int TEST_DATA_COUNT = 2;

struct InotifyTestData {
    int fd;
    int wd;
    uint32_t mask;
};

/**
 * @tc.name: inotify_add_watch_001
 * @tc.desc: This test verifies the effectiveness of inotify_add_watch make multiple calls to the test function and
 *           verify that its return value matches expectations.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxInotifyaddwatchTest, inotify_add_watch_001, TestSize.Level1)
{
    InotifyTestData data[TEST_DATA_COUNT] = {
        { .fd = -1, .wd = -1, .mask = IN_ACCESS },
        { .fd = -1, .wd = -1, .mask = IN_MODIFY },
    };
    for (int i = 0; i < TEST_DATA_COUNT; i++) {
        data[i].wd = inotify_add_watch(data[i].fd, "/path/to/file", data[i].mask);
        EXPECT_EQ(-1, data[i].wd);
    }
}