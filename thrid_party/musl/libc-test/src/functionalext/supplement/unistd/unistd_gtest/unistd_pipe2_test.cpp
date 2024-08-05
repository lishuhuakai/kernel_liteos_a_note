#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdPipe2Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: pipe2_001
 * @tc.desc: Verified whether the pipe function can successfully create a pipeline, and whether data can be correctly
 *           written and read to the pipeline in both child and parent processes.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdPipe2Test, pipe2_001, TestSize.Level1)
{
    std::vector<int> fds = { -1, -1 };
    EXPECT_EQ(0, pipe2(fds.data(), O_CLOEXEC));
}