#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
using namespace testing::ext;

class StatUmaskTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: stat_001
 * @tc.desc: Verifiy that the umask() function can successfully set and retrieve the file mode creation mask for
 *           the current process
 * @tc.type: FUNC
 **/
HWTEST_F(StatUmaskTest, umask_001, TestSize.Level1)
{
    mode_t umaskValue = 0022;
    umask(umaskValue);
    mode_t currentUmask = umask(0);
    EXPECT_EQ(currentUmask, umaskValue);
}
