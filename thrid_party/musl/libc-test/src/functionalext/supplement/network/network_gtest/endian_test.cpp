#include <arpa/inet.h>
#include <endian.h>
#include <gtest/gtest.h>
using namespace testing::ext;

class EnDianTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: htons_001
 * @tc.desc: Convert a 16 bit host byte order integer to a network byte order.
 * @tc.type: FUNC
 **/
HWTEST_F(EnDianTest, htons_001, TestSize.Level1)
{
    constexpr uint16_t le16 = 0x9876;
    constexpr uint16_t be16 = 0x7698;
    uint16_t result = htons(le16);
    EXPECT_EQ(be16, result);
}

/**
 * @tc.name: htonl_001
 * @tc.desc: Convert a 32 bit host byte order integer to a network byte order.
 * @tc.type: FUNC
 **/
HWTEST_F(EnDianTest, htonl_001, TestSize.Level1)
{
    constexpr uint32_t le32 = 0x98765432;
    constexpr uint32_t be32 = 0x32547698;
    uint32_t result = htonl(le32);
    EXPECT_EQ(be32, result);
}

/**
 * @tc.name: ntohs_001
 * @tc.desc: Convert a 16 bit integer from network byte order to host byte order.
 * @tc.type: FUNC
 **/
HWTEST_F(EnDianTest, ntohs_001, TestSize.Level1)
{
    constexpr uint16_t be16 = 0x7698;
    constexpr uint16_t le16 = 0x9876;
    uint16_t result = ntohs(be16);
    EXPECT_EQ(le16, result);
}

/**
 * @tc.name: ntohl_001
 * @tc.desc: Convert a 32 bit integer from network byte order to host byte order.
 * @tc.type: FUNC
 **/
HWTEST_F(EnDianTest, ntohl_001, TestSize.Level1)
{
    constexpr uint32_t be32 = 0x32547698;
    constexpr uint32_t le32 = 0x98765432;
    uint32_t result = ntohl(be32);
    EXPECT_EQ(le32, result);
}