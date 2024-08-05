#include <errno.h>
#include <gtest/gtest.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <string>
using namespace testing::ext;

class NetIfTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: if_nametoindex_001
 * @tc.desc: Validate the functionality of the if_nametoindex function in retrieving the index of a network interface
 *           based on its name
 * @tc.type: FUNC
 **/
HWTEST_F(NetIfTest, if_nametoindex_001, TestSize.Level1)
{
    struct ifaddrs* ifAddr;
    ASSERT_NE(-1, getifaddrs(&ifAddr));
    unsigned index = if_nametoindex(ifAddr->ifa_name);
    EXPECT_NE(index, 0U);
    freeifaddrs(ifAddr);
}

/**
 * @tc.name: if_indextoname_001
 * @tc.desc: Validate the functionality of the if_indextoname function in retrieving the name of a network interface
 *           based on its index
 * @tc.type: FUNC
 **/
HWTEST_F(NetIfTest, if_indextoname_001, TestSize.Level1)
{
    struct ifaddrs* ifAddr;
    ASSERT_NE(-1, getifaddrs(&ifAddr));
    unsigned index = if_nametoindex(ifAddr->ifa_name);
    EXPECT_NE(index, 0U);
    char s[IF_NAMESIZE] = {};
    memset(&s, 0, sizeof(s));
    char* name = if_indextoname(index, s);
    EXPECT_STREQ(ifAddr->ifa_name, name);
    freeifaddrs(ifAddr);
}

/**
 * @tc.name: if_nameindex_001
 * @tc.desc: Ensure that the if_nameindex function successfully retrieves a list of network interface names and indices
 * @tc.type: FUNC
 **/
HWTEST_F(NetIfTest, if_nameindex_001, TestSize.Level1)
{
    auto list = std::unique_ptr<struct if_nameindex, decltype(&if_freenameindex)>(if_nameindex(), &if_freenameindex);
    ASSERT_NE(list, nullptr);

    char buf[IF_NAMESIZE] = {};
    memset(&buf, 0, sizeof(buf));

    for (struct if_nameindex* it = list.get(); it->if_index != 0; ++it) {
        fprintf(stderr, "\t%d\t%s\n", it->if_index, it->if_name);
        unsigned int result1 = if_nametoindex(it->if_name);
        char* result2 = if_indextoname(it->if_index, buf);
        EXPECT_EQ(it->if_index, result1);
        EXPECT_STREQ(it->if_name, result2);
    }
}