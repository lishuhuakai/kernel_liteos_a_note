#include <gtest/gtest.h>
#include <stdio.h>
using namespace testing::ext;

class StdioFerrorunlockedTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: ferror_unlocked_001
 * @tc.desc: Verify that ferror_unlocked correctly detects errors during file reading and returns true when an
 *           error occurs.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFerrorunlockedTest, ferror_unlocked_001, TestSize.Level1)
{
    const char* dirName = "test_ferror_unlocked.txt";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int ret = mkdir(dirName, mode);
    ASSERT_NE(-1, ret);

    FILE* file = fopen("test_ferror_unlocked.txt", "r");
    ASSERT_NE(nullptr, file);
    bool result = false;
    char buffer[BUFSIZ];
    if (fgets(buffer, BUFSIZ, file) == nullptr) {
        EXPECT_TRUE(ferror_unlocked(file));
        result = true;
    }
    EXPECT_TRUE(result);
    fclose(file);
    remove("test_ferror_unlocked.txt");
}