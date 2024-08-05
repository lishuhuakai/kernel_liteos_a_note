#include <arpa/inet.h>
#include <gtest/gtest.h>
using namespace testing::ext;

class ArpaInetTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: inet_pton_001
 * @tc.desc: Test inet_pton function replaces addr dotted decimal string with an IPv4 address string
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_pton_001, TestSize.Level1)
{
    struct sockaddr_in addr;
    int result = inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
    EXPECT_EQ(1, result);
    char buf[INET_ADDRSTRLEN];
    memset(&buf, 0, sizeof(buf));
    inet_ntop(AF_INET, &(addr.sin_addr), buf, INET_ADDRSTRLEN);
    EXPECT_STREQ("127.0.0.1", buf);
}

/**
 * @tc.name: inet_ntoa_001
 * @tc.desc: Verify that the inet_ntoa function correctly converts a valid IPv4 address in binary format to its
 *           string representation.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_ntoa_001, TestSize.Level1)
{
    in_addr addr = { (htonl)(0x7f000001) };
    char* result = inet_ntoa(addr);
    EXPECT_STREQ("127.0.0.1", result);
}

/**
 * @tc.name: inet_aton_001
 * @tc.desc: Verify that the inet_aton function correctly converts a valid IPv4 address to binary format and stores
 *           it in the s_addr field of the in_addr struct.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_001, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("127.9.8.7", &addr);
    EXPECT_EQ(1, result);
    EXPECT_EQ((htonl)(0x7f090807), addr.s_addr);
}

/**
 * @tc.name: inet_aton_002
 * @tc.desc: Verify that the inet_aton function correctly converts a partial IPv4 address to binary format and stores
 *           it in the s_addr field of the in_addr struct.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_002, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("127.9.8", &addr);
    EXPECT_EQ(1, result);
    EXPECT_EQ((htonl)(0x7f090008), addr.s_addr);
}

/**
 * @tc.name: inet_aton_003
 * @tc.desc: Verify that the inet_aton function correctly converts a partial IPv4 address to binary format and stores
 *           it in the s_addr field of the in_addr struct.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_003, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("127.9", &addr);
    EXPECT_EQ(1, result);
    EXPECT_EQ((htonl)(0x7f000009), addr.s_addr);
}

/**
 * @tc.name: inet_aton_004
 * @tc.desc: Verify that the inet_aton function correctly converts a hexadecimal IPv4 address to binary format and
 *           stores it in the s_addr field of the in_addr struct.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_004, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("0xFf.0.0.9", &addr);
    EXPECT_EQ(1, result);
    EXPECT_EQ((htonl)(0xff000009), addr.s_addr);
}

/**
 * @tc.name: inet_aton_005
 * @tc.desc: Verify that the inet_aton function correctly converts a hexadecimal IPv4 address to binary format and
 *           stores it in the s_addr field of the in_addr struct.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_005, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("0XfF.0.0.9", &addr);
    EXPECT_EQ(1, result);
    EXPECT_EQ((htonl)(0xff000009), addr.s_addr);
}

/**
 * @tc.name: inet_aton_006
 * @tc.desc: Verify that the inet_aton function correctly converts an octal IPv4 address to binary format and stores
 *           it in the s_addr field of the in_addr struct.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_006, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("0177.0.0.9", &addr);
    EXPECT_EQ(1, result);
    EXPECT_EQ((htonl)(0x7f000009), addr.s_addr);
}

/**
 * @tc.name: inet_aton_007
 * @tc.desc: Verify that the inet_aton function correctly converts a decimal integer IPv4 address to binary format
 *           and stores it in the s_addr field of the in_addr struct.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_007, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("369", &addr);
    EXPECT_EQ(1, result);
    EXPECT_EQ((htonl)(369u), addr.s_addr);
}

/**
 * @tc.name: inet_aton_008
 * @tc.desc: Verify that the inet_aton function returns the expected values when given empty or valid IPv4 addresses,
 *           indicating the correct behavior of converting the addresses from text to binary format.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_008, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result1 = inet_aton("", nullptr);
    EXPECT_EQ(0, result1);
    int result2 = inet_aton("127.0.0.1", &addr);
    EXPECT_EQ(1, result2);
}

/**
 * @tc.name: inet_aton_009
 * @tc.desc: Verify that the inet_aton function returns 0 when given invalid IPv4 addresses with invalid characters,
 *           spaces, or leading zeros, indicating a failure to convert the addresses from text to binary format.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_009, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result1 = inet_aton(" ", nullptr);
    int result2 = inet_aton("a", &addr);
    int result3 = inet_aton("0ax.0.0.9", &addr);
    int result4 = inet_aton("127.0.0.1a", &addr);
    int result5 = inet_aton("09.0.0.9", &addr);
    EXPECT_EQ(0, result1);
    EXPECT_EQ(0, result2);
    EXPECT_EQ(0, result3);
    EXPECT_EQ(0, result4);
    EXPECT_EQ(0, result5);
}

/**
 * @tc.name: inet_aton_010
 * @tc.desc: Verify that the inet_aton function returns 0 when given invalid IPv4 addresses with too many octets or
 *           trailing dots, indicating a failure to convert the addresses from text to binary format.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_010, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result1 = inet_aton("9.8.7.6.5", &addr);
    int result2 = inet_aton("9.8.7.6.", &addr);
    EXPECT_EQ(0, result1);
    EXPECT_EQ(0, result2);
}

/**
 * @tc.name: inet_aton_011
 * @tc.desc: Verify that the inet_aton function returns 0 when given invalid IPv4 addresses with octets greater than
 *           255, indicating a failure to convert the addresses from text to binary format.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_011, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result1 = inet_aton("333.2.0.9", &addr);
    int result2 = inet_aton("0.333.0.9", &addr);
    int result3 = inet_aton("0.2.333.9", &addr);
    int result4 = inet_aton("0.2.0.333", &addr);
    EXPECT_EQ(0, result1);
    EXPECT_EQ(0, result2);
    EXPECT_EQ(0, result3);
    EXPECT_EQ(0, result4);
}

/**
 * @tc.name: inet_aton_012
 * @tc.desc: Verify that the inet_aton function returns 0 when given invalid IPv4 addresses, indicating a failure
 *           to convert the addresses from text to binary format.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_012, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result1 = inet_aton("260.0.0", &addr);
    int result2 = inet_aton("0.260.0", &addr);
    int result3 = inet_aton("0.0.0x10000", &addr);
    EXPECT_EQ(0, result1);
    EXPECT_EQ(0, result2);
    EXPECT_EQ(0, result3);
}

/**
 * @tc.name: inet_aton_013
 * @tc.desc: Verify that the inet_aton function returns 0 when given invalid IPv4 addresses, indicating a failure
 *           to convert the addresses from text to binary format.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_013, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result1 = inet_aton("260.0", &addr);
    int result2 = inet_aton("0.0x1000000", &addr);
    EXPECT_EQ(0, result1);
    EXPECT_EQ(0, result2);
}

/**
 * @tc.name: inet_aton_014
 * @tc.desc: Verify that the inet_aton function returns 0 when given an invalid IPv4 address, indicating a failure
 *           to convert the address from text to binary format.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_aton_014, TestSize.Level1)
{
    in_addr addr;
    addr.s_addr = 0;
    int result = inet_aton("0900.0.0.9", &addr);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: inet_addr_001
 * @tc.desc: Ensure that the "inet_addr" function behaves correctly for converting addr valid IPv4 address string into
 *its binary representation
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_addr_001, TestSize.Level1)
{
    in_addr_t result = inet_addr("127.0.0.1");
    EXPECT_EQ((htonl)(0x7f000001), result);
}

/**
 * @tc.name: inet_ntop_001
 * @tc.desc: Verify that the inet_pton function correctly converts a string representation of an IPv4 address to its
 *           binary format and that the inet_ntop function correctly converts a binary format IPv4 address to its
 *           string representation.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_ntop_001, TestSize.Level1)
{
    sockaddr_storage ss0;
    EXPECT_EQ(1, inet_pton(AF_INET, "127.0.0.1", &ss0));
    char s0[INET_ADDRSTRLEN];
    memset(&s0, 0, sizeof(s0));
    const char* result1 = inet_ntop(AF_INET, &ss0, s0, INET_ADDRSTRLEN);
    const char* result2 = inet_ntop(AF_INET, &ss0, s0, 2 * INET_ADDRSTRLEN);
    EXPECT_STREQ("127.0.0.1", result1);
    EXPECT_STREQ("127.0.0.1", result2);
}

/**
 * @tc.name: inet_ntop_002
 * @tc.desc: Verify the correctness and robustness of the inet_ntop function when dealing with IPv6 addresses.
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_ntop_002, TestSize.Level1)
{
    sockaddr_storage ss1;
    EXPECT_EQ(1, inet_pton(AF_INET6, "::1", &ss1));
    char s1[INET6_ADDRSTRLEN];
    memset(&s1, 0, sizeof(s1));
    const char* result1 = inet_ntop(AF_INET6, &ss1, s1, INET_ADDRSTRLEN);
    const char* result2 = inet_ntop(AF_INET6, &ss1, s1, INET6_ADDRSTRLEN);
    const char* result3 = inet_ntop(AF_INET6, &ss1, s1, 2 * INET6_ADDRSTRLEN);
    EXPECT_STREQ("::1", result1);
    EXPECT_STREQ("::1", result2);
    EXPECT_STREQ("::1", result3);
}

/**
 * @tc.name: inet_network_001
 * @tc.desc: Ensure that the "inet_network" function behaves correctly for converting IPv4 addresses in string format
 *           to their corresponding binary form
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_network_001, TestSize.Level1)
{
    in_addr_t result = inet_network("");
    EXPECT_EQ(~0U, result);
}
/**
 * @tc.name: inet_network_002
 * @tc.desc: Verify that the inet_network function correctly converts a string representation of
 *           an IPv4 address to its binary format, and that the converted address matches the expected binary
 *           representation of the IPv4 address "127.0.0.1".
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_network_002, TestSize.Level1)
{
    in_addr_t result = inet_network("127.0.0.1");
    EXPECT_EQ(0x7f000001U, result);
}
/**
 * @tc.name: inet_network_003
 * @tc.desc: Verifiy that the inet_network function correctly converts a string representation of an
 *           IPv4 address in hexadecimal format to its binary format, and that the converted address matches the
 *           expected binary representation of the hexadecimal address "0x9f".
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_network_003, TestSize.Level1)
{
    in_addr_t result = inet_network("0x9f");
    EXPECT_EQ(0x9fU, result);
}

/**
 * @tc.name: inet_lnaof_001
 * @tc.desc: Ensure that the "inet_lnaof" function behaves correctly for extracting the host portion from
 *           an IPv4 address
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_lnaof_001, TestSize.Level1)
{
    in_addr addr = { htonl(0x98765432) };
    in_addr_t result = inet_lnaof(addr);
    EXPECT_EQ(0x5432, result);
}

/**
 * @tc.name: inet_netof_001
 * @tc.desc: Ensure that the "inet_netof" function behaves correctly for extracting the network portion from an
 *           IPv4 address
 * @tc.type: FUNC
 **/
HWTEST_F(ArpaInetTest, inet_netof_001, TestSize.Level1)
{
    in_addr addr = { htonl(0x98765432) };
    in_addr_t result = inet_netof(addr);
    EXPECT_EQ(0x9876, result);
}
