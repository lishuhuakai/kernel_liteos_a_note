#include <arpa/inet.h>
#include <endian.h>
#include <gtest/gtest.h>
using namespace testing::ext;

class FortifyTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __send_chk_001
 * @tc.desc: Testing that the __send_chk function can be able to transmit data correctly
 * @tc.type: FUNC
 **/
HWTEST_F(FortifyTest, __send_chk_001, TestSize.Level1)
{
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    EXPECT_NE(-1, sockfd);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(1234);
    EXPECT_NE(-1, connect(sockfd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)));
    const void* buf[BUFSIZ];

    int retval = __send_chk(sockfd, buf, 0, sizeof(addr), 0);
    EXPECT_NE(-1, retval);
    close(sockfd);
}

/**
 * @tc.name: __send_chk_002
 * @tc.desc: Return -1 when testing for incorrect parameters passed in
 * @tc.type: FUNC
 **/
HWTEST_F(FortifyTest, __send_chk_002, TestSize.Level1)
{
    int result = __send_chk(-1, nullptr, 0, 0, 0);
    EXPECT_EQ(-1, result);
}