#include <fcntl.h>
#include <gtest/gtest.h>
#include <locale.h>
#include <nl_types.h>
using namespace testing::ext;

class LocaleCatopenTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: catopen_001
 * @tc.desc: The test check whether the catopen function successfully opens a message catalog named "my_messages"
 *           using the NL_CAT_LOCALE category.
 * @tc.type: FUNC
 **/
HWTEST_F(LocaleCatopenTest, catopen_001, TestSize.Level1)
{
    nl_catd result = catopen("not_exist", NL_CAT_LOCALE);
    EXPECT_EQ(reinterpret_cast<nl_catd>(-1), result);
}