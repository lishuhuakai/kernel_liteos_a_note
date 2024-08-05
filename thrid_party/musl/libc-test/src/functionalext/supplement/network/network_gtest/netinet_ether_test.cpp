#include <gtest/gtest.h>
#include <netdb.h>
#include <netinet/ether.h>
using namespace testing::ext;

class NetinetEtherTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: ether_aton_001
 * @tc.desc: Verify that the ether_aton function can correctly convert Ethernet MAC addresses from string to binary
 *           representations without errors and that the resulting binary representation matches the expected values.
 * @tc.type: FUNC
 **/
HWTEST_F(NetinetEtherTest, ether_aton_001, TestSize.Level1)
{
    ether_addr* a = ether_aton("ef:cd:ab:89:67:45");
    ASSERT_NE(nullptr, a);
    EXPECT_EQ(0xef, a->ether_addr_octet[0]);
    EXPECT_EQ(0xcd, a->ether_addr_octet[1]);
    EXPECT_EQ(0xab, a->ether_addr_octet[2]);
    EXPECT_EQ(0x89, a->ether_addr_octet[3]);
    EXPECT_EQ(0x67, a->ether_addr_octet[4]);
    EXPECT_EQ(0x45, a->ether_addr_octet[5]);
}

/**
 * @tc.name: ether_ntoa_001
 * @tc.desc: Verify that the ether_ntoa and ether_aton functions can correctly convert Ethernet MAC addresses between
 *           string and binary representations without errors.
 * @tc.type: FUNC
 **/
HWTEST_F(NetinetEtherTest, ether_ntoa_001, TestSize.Level1)
{
    ether_addr* a = ether_aton("ef:cd:ab:89:67:45");
    ASSERT_NE(nullptr, a);
    EXPECT_STREQ("EF:CD:AB:89:67:45", ether_ntoa(a));
}

/**
 * @tc.name: ether_aton_r_001
 * @tc.desc: Verify that the ether_aton_r function can correctly convert Ethernet MAC addresses from string to binary
 *           representations without errors and that the resulting binary representation matches the expected values.
 * @tc.type: FUNC
 **/
HWTEST_F(NetinetEtherTest, ether_aton_r_001, TestSize.Level1)
{
    ether_addr addr;
    memset(&addr, 0, sizeof(addr));
    ether_addr* a = ether_aton_r("ef:cd:ab:89:67:45", &addr);
    EXPECT_EQ(&addr, a);
    EXPECT_EQ(0xef, addr.ether_addr_octet[0]);
    EXPECT_EQ(0xcd, addr.ether_addr_octet[1]);
    EXPECT_EQ(0xab, addr.ether_addr_octet[2]);
    EXPECT_EQ(0x89, addr.ether_addr_octet[3]);
    EXPECT_EQ(0x67, addr.ether_addr_octet[4]);
    EXPECT_EQ(0x45, addr.ether_addr_octet[5]);
}

/**
 * @tc.name: ether_ntoa_r_001
 * @tc.desc: Verify that the ether_ntoa_r and ether_aton_r functions can correctly convert Ethernet MAC addresses
 *           between string and binary representations without errors.
 * @tc.type: FUNC
 **/
HWTEST_F(NetinetEtherTest, ether_ntoa_r_001, TestSize.Level1)
{
    ether_addr pa;
    memset(&pa, 0, sizeof(pa));
    ether_addr* a = ether_aton_r("ef:cd:ab:89:67:45", &pa);
    EXPECT_EQ(&pa, a);

    char x[BUFSIZ];

    memset(&x, 0, sizeof(x));
    char* p = ether_ntoa_r(&pa, x);
    EXPECT_EQ(x, p);
    EXPECT_STREQ("EF:CD:AB:89:67:45", x);
}