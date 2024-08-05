#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdAccessTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: access_001
 * @tc.desc: Ensure that the basic operations of the file can function properly.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdAccessTest, access_001, TestSize.Level1)
{
    const char* ptr = "accesstest.txt";
    FILE* fptr = fopen(ptr, "w");
    EXPECT_TRUE(fptr);
    EXPECT_EQ(0, access(ptr, F_OK));
    fclose(fptr);
    remove(ptr);
    fptr = nullptr;
    ptr = nullptr;
}