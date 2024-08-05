#include <gtest/gtest.h>
#include <stdio.h>

using namespace testing::ext;

class StdioPerrorTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: perror_001
 * @tc.desc: Verify if its output meets expectations.
 * @tc.type: FUNC
 * */
HWTEST_F(StdioPerrorTest, perror_001, TestSize.Level1)
{
    FILE* file = fopen("failedOpenFile.txt", "r");
    EXPECT_EQ(file, nullptr);
    testing::internal::CaptureStderr();
    perror("test perror success");
    std::string out = testing::internal::GetCapturedStderr();
    const char* perr = "test perror success: No such file or directory\n";
    EXPECT_STREQ(perr, out.c_str());
}