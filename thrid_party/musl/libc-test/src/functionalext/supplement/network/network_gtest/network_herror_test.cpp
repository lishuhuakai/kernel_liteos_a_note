#include <gtest/gtest.h>
#include <netdb.h>
using namespace testing::ext;

class NetworkHerrorTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: herror_001
 * @tc.desc: The herror function is called to print an error message
 * @tc.type: FUNC
 **/
HWTEST_F(NetworkHerrorTest, herror_001, TestSize.Level1)
{
    struct hostent* hostInfo;
    hostInfo = gethostbyname("nonexistenthostname");
    if (hostInfo == nullptr) {
        herror("gethostbyname failed");
    }
}