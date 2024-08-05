#include <errno.h>
#include <gtest/gtest.h>
#include <netdb.h>
#include <string.h>
#include <vector>
using namespace testing::ext;

class NetdbTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getaddrinfo_001
 * @tc.desc: Verify that the "getaddrinfo" function correctly translates the "localhost" host name into socket
 *           addresses
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getaddrinfo_001, TestSize.Level1)
{
    const char* host = "localhost";
    const char* serv = nullptr;
    addrinfo hints;
    addrinfo* addr = nullptr;
    int result = getaddrinfo(host, serv, &hints, &addr);
    EXPECT_EQ(0, result);
    ASSERT_NE(addr, nullptr);
    freeaddrinfo(addr);
}

/**
 * @tc.name: getaddrinfo_002
 * @tc.desc: Verify that the "getaddrinfo" function correctly translates the "localhost" host name and "9999"
 *           service name into socket addresses for both TCP and UDP protocols
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getaddrinfo_002, TestSize.Level1)
{
    const char* host = "localhost";
    const char* serv = "9999";
    addrinfo hints;
    addrinfo* addr = nullptr;
    EXPECT_EQ(0, getaddrinfo(host, serv, &hints, &addr));

    bool tcp = false;
    bool udp = false;

    for (auto p = addr; p != nullptr; p = p->ai_next) {
        EXPECT_TRUE(p->ai_family == AF_INET || p->ai_family == AF_INET6);
        if (p->ai_socktype != SOCK_STREAM && p->ai_socktype != SOCK_DGRAM) {
            continue;
        } else if (p->ai_socktype == SOCK_STREAM && IPPROTO_TCP == p->ai_protocol) {
            tcp = true;
        } else if (p->ai_socktype == SOCK_DGRAM && IPPROTO_UDP == p->ai_protocol) {
            udp = true;
        }
    }

    EXPECT_TRUE(tcp);
    EXPECT_TRUE(udp);

    freeaddrinfo(addr);
}

/**
 * @tc.name: getaddrinfo_003
 * @tc.desc: Verify the correct behavior of the getaddrinfo function in resolving network addresses for
 *           a given host and service.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getaddrinfo_003, TestSize.Level1)
{
    const char* host = "localhost";
    const char* service = "9999";
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    addrinfo* addr = nullptr;
    int result = getaddrinfo(host, service, &hints, &addr);
    EXPECT_EQ(0, result);
    ASSERT_NE(addr, nullptr);
    for (addrinfo* cur = addr; cur != nullptr; cur = cur->ai_next) {
        EXPECT_EQ(AF_INET, cur->ai_family);
        EXPECT_EQ(SOCK_STREAM, cur->ai_socktype);
        EXPECT_EQ(IPPROTO_TCP, cur->ai_protocol);
    }
    freeaddrinfo(addr);
}

/**
 * @tc.name: getaddrinfo_004
 * @tc.desc: Verify the correct behavior of the getaddrinfo function when resolving IPv6 addresses for a given host.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getaddrinfo_004, TestSize.Level1)
{
    const char* host = "ip6-localhost";
    const char* service = nullptr;
    addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;
    addrinfo* addr;
    int ret = getaddrinfo(host, service, &hints, &addr);
    EXPECT_EQ(ret, 0);
    ASSERT_NE(addr, nullptr);

    struct sockaddr_in6* addr6 = reinterpret_cast<struct sockaddr_in6*>(addr->ai_addr);
    EXPECT_EQ(addr6->sin6_family, AF_INET6);
    EXPECT_EQ(memcmp(&(addr6->sin6_addr), &in6addr_loopback, sizeof(struct in6_addr)), 0);

    freeaddrinfo(addr);
}

/**
 * @tc.name: getnameinfo_001
 * @tc.desc: Verify the basic functionality of getnameinfo for retrieving the textual representation of an IPv4 address.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_001, TestSize.Level1)
{
    sockaddr_storage addr;
    memset(&addr, 0, sizeof(addr));
    sockaddr* sa = reinterpret_cast<sockaddr*>(&addr);
    char node[BUFSIZ];
    memset(&node, 0, sizeof(node));
    addr.ss_family = AF_INET;
    socklen_t plumb = sizeof(sockaddr_in);
    int result = getnameinfo(sa, plumb, node, sizeof(node), nullptr, 0, NI_NUMERICHOST);
    EXPECT_EQ(0, result);
    EXPECT_STREQ("0.0.0.0", node);
}

/**
 * @tc.name: getnameinfo_002
 * @tc.desc: Verify that the getnameinfo() function properly converts an IPv4 address to its numeric form and stores
 *           it in the "node" buffer. In this case, the expected result is "0.0.0.0".
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_002, TestSize.Level1)
{
    sockaddr_storage addr;
    memset(&addr, 0, sizeof(addr));
    sockaddr* sa = reinterpret_cast<sockaddr*>(&addr);
    char node[BUFSIZ];
    memset(&node, 0, sizeof(node));
    addr.ss_family = AF_INET;
    socklen_t more = sizeof(addr);
    int result = getnameinfo(sa, more, node, sizeof(node), nullptr, 0, NI_NUMERICHOST);
    EXPECT_EQ(0, result);
    EXPECT_STREQ("0.0.0.0", node);
}

/**
 * @tc.name: getnameinfo_003
 * @tc.desc: Verify that the getnameinfo() function returns the expected error code (EAI_FAMILY) when the length
 *           argument is smaller than the size of the sockaddr structure.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_003, TestSize.Level1)
{
    sockaddr_storage addr;
    memset(&addr, 0, sizeof(addr));
    sockaddr* sa = reinterpret_cast<sockaddr*>(&addr);
    char node[BUFSIZ];
    memset(&node, 0, sizeof(node));
    addr.ss_family = AF_INET;
    socklen_t less = sizeof(sockaddr_in) - 1;
    int result = getnameinfo(sa, less, node, sizeof(node), nullptr, 0, NI_NUMERICHOST);
    EXPECT_EQ(EAI_FAMILY, result);
}

/**
 * @tc.name: getnameinfo_004
 * @tc.desc: Verify that the getnameinfo() function properly converts an IPv6 address to its numeric form and stores
 *           it in the "node" buffer. In this case, the expected result is "::".
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_004, TestSize.Level1)
{
    sockaddr_storage addr;
    memset(&addr, 0, sizeof(addr));
    sockaddr* sa = reinterpret_cast<sockaddr*>(&addr);
    char node[BUFSIZ];
    memset(&node, 0, sizeof(node));
    addr.ss_family = AF_INET6;
    socklen_t plumb = sizeof(sockaddr_in6);
    int result = getnameinfo(sa, plumb, node, sizeof(node), nullptr, 0, NI_NUMERICHOST);
    EXPECT_EQ(0, result);
    EXPECT_STREQ("::", node);
}

/**
 * @tc.name: getnameinfo_005
 * @tc.desc: Verify that the getnameinfo() function properly converts an IPv6 address to its numeric form and stores
 *           it in the "node" buffer. In this case, the expected result is "::".
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_005, TestSize.Level1)
{
    sockaddr_storage addr;
    memset(&addr, 0, sizeof(addr));
    sockaddr* sa = reinterpret_cast<sockaddr*>(&addr);
    char node[BUFSIZ];
    memset(&node, 0, sizeof(node));
    addr.ss_family = AF_INET6;
    socklen_t more = sizeof(sockaddr_in6) + 1;
    int result = getnameinfo(sa, more, node, sizeof(node), nullptr, 0, NI_NUMERICHOST);
    EXPECT_EQ(0, result);
    EXPECT_STREQ("::", node);
}

/**
 * @tc.name: getnameinfo_006
 * @tc.desc: Verify that the getnameinfo() function correctly handles the scenario where the length argument
 *           provided is smaller than the actual size of the sockaddr structure.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_006, TestSize.Level1)
{
    sockaddr_storage addr;
    memset(&addr, 0, sizeof(addr));
    sockaddr* sa = reinterpret_cast<sockaddr*>(&addr);
    char node[BUFSIZ];
    memset(&node, 0, sizeof(node));
    addr.ss_family = AF_INET6;
    socklen_t less = sizeof(sockaddr_in6) - 1;
    int result = getnameinfo(sa, less, node, sizeof(node), nullptr, 0, NI_NUMERICHOST);
    EXPECT_EQ(EAI_FAMILY, result);
}

/**
 * @tc.name: getnameinfo_007
 * @tc.desc: Verify the getnameinfo function's ability to retrieve the hostname associated with a
 *           specific IPv4 address.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_007, TestSize.Level1)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(0x7f000001);

    char host[NI_MAXHOST];
    memset(&host, 0, sizeof(host));
    int result = getnameinfo(reinterpret_cast<sockaddr*>(&addr), sizeof(addr), host, sizeof(host), nullptr, 0, 0);
    EXPECT_EQ(0, result);
    EXPECT_STREQ(host, "localhost");
}

/**
 * @tc.name: getnameinfo_008
 * @tc.desc: Verify the getnameinfo function's ability to retrieve the hostname associated with
 *           the IPv6 loopback address.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnameinfo_008, TestSize.Level1)
{
    sockaddr_in6 addr = { .sin6_family = AF_INET6, .sin6_addr = in6addr_loopback };
    char host[NI_MAXHOST];
    memset(&host, 0, sizeof(host));
    int result = getnameinfo(reinterpret_cast<sockaddr*>(&addr), sizeof(addr), host, sizeof(host), nullptr, 0, 0);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: gethostbyname_001
 * @tc.desc: Verify the gethostbyname function's ability to retrieve host information for the "localhost" hostname.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyname_001, TestSize.Level1)
{
    hostent* hptr = gethostbyname("localhost");
    ASSERT_NE(hptr, nullptr);
    EXPECT_EQ(hptr->h_addrtype, AF_INET);

    in_addr hostAddr;
    hostAddr.s_addr = htonl(INADDR_LOOPBACK);
    EXPECT_TRUE(memcmp(hptr->h_addr, &hostAddr, sizeof(in_addr)) == 0);
}

/**
 * @tc.name: gethostbyname_r_001
 * @tc.desc: Verify the thread-safe version of the gethostbyname function's ability to retrieve
 *           host information for the "localhost" hostname.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyname_r_001, TestSize.Level1)
{
    hostent ht;
    hostent* hptr1 = nullptr;
    char buf[BUFSIZ];
    memset(&buf, 0, sizeof(buf));
    int err;
    int result = gethostbyname_r("localhost", &ht, buf, sizeof(buf), &hptr1, &err);
    EXPECT_EQ(0, err);
    EXPECT_EQ(0, result);
    ASSERT_NE(hptr1, nullptr);
    EXPECT_EQ(hptr1->h_addrtype, AF_INET);

    in_addr hostAddr1;
    hostAddr1.s_addr = htonl(INADDR_LOOPBACK);
    EXPECT_TRUE(memcmp(hptr1->h_addr, &hostAddr1, sizeof(in_addr)) == 0);
    hptr1->h_addr_list[0][0] = 2;

    hostent ht2;
    hostent* hptr2 = nullptr;
    char buf2[BUFSIZ];
    memset(&buf2, 0, sizeof(buf2));
    result = gethostbyname_r("localhost", &ht2, buf2, sizeof(buf2), &hptr2, &err);
    EXPECT_EQ(0, err);
    EXPECT_EQ(0, result);
    ASSERT_NE(hptr2, nullptr);
    EXPECT_EQ(hptr2->h_addrtype, AF_INET);

    in_addr hostAddr2;
    hostAddr2.s_addr = htonl(INADDR_LOOPBACK);
    EXPECT_TRUE(memcmp(hptr2->h_addr, &hostAddr2, sizeof(in_addr)) == 0);
    EXPECT_EQ(2, hptr1->h_addr_list[0][0]);
}

/**
 * @tc.name: gethostbyname_r_002
 * @tc.desc: Verify the thread-safe version of the gethostbyname function's ability to handle non-existing hostnames.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyname_r_002, TestSize.Level1)
{
    hostent ht;
    char buf[BUFSIZ];
    memset(&buf, 0, sizeof(buf));
    hostent* hptr1 = nullptr;
    size_t buflen = sizeof(buf);

    int err;
    int result = gethostbyname_r("musl.not.exist", &ht, buf, buflen, &hptr1, &err);
    EXPECT_EQ(HOST_NOT_FOUND, err);
    EXPECT_NE(0, result);
    EXPECT_EQ(nullptr, hptr1);
}

/**
 * @tc.name: gethostbyname2_001
 * @tc.desc: Verify the gethostbyname2 function's ability to retrieve host information for the "localhost"
 *           hostname and AF_INET address family.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyname2_001, TestSize.Level1)
{
    hostent* hptr = gethostbyname2("localhost", AF_INET);
    ASSERT_NE(hptr, nullptr);
    EXPECT_EQ(hptr->h_addrtype, AF_INET);

    in_addr hostAddr;
    hostAddr.s_addr = htonl(INADDR_LOOPBACK);
    EXPECT_TRUE(memcmp(hptr->h_addr, &hostAddr, sizeof(in_addr)) == 0);
}

/**
 * @tc.name: gethostbyname2_r_001
 * @tc.desc: Verify the thread-safe version of the gethostbyname2 function's ability to handle insufficient
 *           buffer sizes when retrieving host information.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyname2_r_001, TestSize.Level1)
{
    hostent ht;
    hostent* hptr = nullptr;
    char buf[BUFSIZ];
    memset(&buf, 0, sizeof(buf));
    size_t buflen = sizeof(buf);
    int err;
    int result = gethostbyname2_r("localhost", AF_INET, &ht, buf, buflen, &hptr, &err);
    EXPECT_EQ(0, result);
    EXPECT_NE(nullptr, hptr);
    EXPECT_EQ(0, err);
}

/**
 * @tc.name: gethostbyname2_r_002
 * @tc.desc: Verify the thread-safe version of the gethostbyname2 function's ability to handle non-existing hostnames.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyname2_r_002, TestSize.Level1)
{
    hostent ht;
    hostent* hptr = nullptr;
    char buf[BUFSIZ];
    memset(&buf, 0, sizeof(buf));
    size_t buflen = sizeof(buf);
    int err;
    int result = gethostbyname2_r("musl.not.exist", AF_INET, &ht, buf, buflen, &hptr, &err);
    EXPECT_EQ(HOST_NOT_FOUND, err);
    EXPECT_NE(0, result);
    EXPECT_EQ(nullptr, hptr);
}

/**
 * @tc.name: gethostbyaddr_001
 * @tc.desc: Verify the ability of the gethostbyaddr function to retrieve host information based on an IPv4 address.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyaddr_001, TestSize.Level1)
{
    in_addr addr = { htonl(0x7f000001) };
    hostent* hptr = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    ASSERT_NE(hptr, nullptr);
    EXPECT_EQ(hptr->h_addrtype, AF_INET);

    in_addr hostAddr;
    hostAddr.s_addr = htonl(INADDR_LOOPBACK);
    EXPECT_TRUE(memcmp(hptr->h_addr, &hostAddr, sizeof(in_addr)) == 0);
}

/**
 * @tc.name: gethostbyaddr_r_001
 * @tc.desc: The purpose of this test case is to validate the functionality of the gethostbyaddr_r function in
 *           retrieving host information based on an IP address, and to verify its thread safety
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, gethostbyaddr_r_001, TestSize.Level1)
{
    in_addr addr = { htonl(0x7f000001) };
    hostent ht;
    hostent* hptr1 = nullptr;
    char buf[BUFSIZ];
    memset(&buf, 0, sizeof(buf));
    int err;
    int result = gethostbyaddr_r(&addr, sizeof(addr), AF_INET, &ht, buf, sizeof(buf), &hptr1, &err);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, err);
    ASSERT_NE(hptr1, nullptr);
    EXPECT_EQ(hptr1->h_addrtype, AF_INET);

    in_addr hostAddr1;
    hostAddr1.s_addr = htonl(INADDR_LOOPBACK);
    EXPECT_TRUE(memcmp(hptr1->h_addr, &hostAddr1, sizeof(in_addr)) == 0);
    hptr1->h_addr_list[0][0] = 2;

    hostent ht2;
    hostent* hptr2 = nullptr;
    char buf2[BUFSIZ];
    memset(&buf2, 0, sizeof(buf2));
    result = gethostbyaddr_r(&addr, sizeof(addr), AF_INET, &ht2, buf2, sizeof(buf2), &hptr2, &err);
    EXPECT_EQ(0, err);
    EXPECT_EQ(0, result);
    ASSERT_NE(hptr2, nullptr);
    EXPECT_EQ(hptr2->h_addrtype, AF_INET);

    in_addr hostAddr2;
    hostAddr2.s_addr = htonl(INADDR_LOOPBACK);
    EXPECT_TRUE(memcmp(hptr2->h_addr, &hostAddr2, sizeof(in_addr)) == 0);
    EXPECT_EQ(2, hptr1->h_addr_list[0][0]);
}

/**
 * @tc.name: getnetbyaddr_001
 * @tc.desc: Verify that the "getnetbyaddr" function handles the case of an invalid or non-existent IP address
 *           gracefully and returns nullptr when no network information is available.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnetbyaddr_001, TestSize.Level1)
{
    EXPECT_EQ(nullptr, getnetbyaddr(0, 0));
}

/**
 * @tc.name: getnetbyname_001
 * @tc.desc: Verify that the "getnetbyname" function handles the case of an invalid or non-existent network name
 *           gracefully and returns nullptr when no network information is available.
 * @tc.type: FUNC
 **/
HWTEST_F(NetdbTest, getnetbyname_001, TestSize.Level1)
{
    EXPECT_EQ(nullptr, getnetbyname("z"));
}