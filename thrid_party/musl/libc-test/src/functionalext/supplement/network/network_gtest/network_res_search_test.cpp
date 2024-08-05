#include <arpa/nameser.h>
#include <gtest/gtest.h>
#include <netdb.h>
#include <resolv.h>
using namespace testing::ext;

class NetworkRessearchTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: res_search_001
 * @tc.desc: Verify that the "res_search" function can perform a DNS query for the specified domain and
 *           return the result effectively.
 * @tc.type: FUNC
 **/
HWTEST_F(NetworkRessearchTest, res_search_001, TestSize.Level1)
{
    const char* domain = " ";
    unsigned char answer[NS_MAXDNAME];
    memset(&answer, 0, sizeof(answer));
    int anslen = sizeof(answer);
    int result = res_search(domain, C_IN, T_A, answer, anslen);
    EXPECT_EQ(-1, result);
}
