#include <errno.h>
#include <gtest/gtest.h>
#include <netdb.h>
#include <string.h>
#include <sys/un.h>
#include <vector>
using namespace testing::ext;

class FortifyRecvchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

struct DataLink {
    std::function<bool(int)> callbackFn;
    const char* sockPath;
};

static void* ConnectFunc(void* data)
{
    DataLink* p = reinterpret_cast<DataLink*>(data);
    std::function<bool(int)> callbackFn = p->callbackFn;
    void* returnValue = nullptr;

    int fd = socket(PF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    if (fd < 0) {
        return reinterpret_cast<void*>(-1);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strcpy(addr.sun_path + 1, p->sockPath);

    if (connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        returnValue = reinterpret_cast<void*>(-1);
    } else if (callbackFn && !callbackFn(fd)) {
        returnValue = reinterpret_cast<void*>(-1);
    }

    close(fd);
    return returnValue;
}

struct ReturnVal {
    int fd;
    struct sockaddr_un addr;
};

static void SockInit(const char* sockPath, struct ReturnVal& returnVal)
{
    int fd = socket(PF_UNIX, SOCK_SEQPACKET, 0);
    EXPECT_NE(fd, -1);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strcpy(addr.sun_path + 1, sockPath);
    EXPECT_NE(-1, bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)));
    EXPECT_NE(-1, listen(fd, 1));

    returnVal.fd = fd;
    returnVal.addr = addr;
}

static void CallSelectFunc(struct ReturnVal returnVal)
{
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(returnVal.fd, &readSet);
    timeval tv = { .tv_sec = 5, .tv_usec = 0 };
    EXPECT_GT(select(returnVal.fd + 1, &readSet, nullptr, nullptr, &tv), 0);
}

const char* g_testRecv = "hello";
static bool Send(int fd)
{
    if (send(fd, g_testRecv, strlen(g_testRecv) + 1, 0) < 0) {
        return false;
    }
    return true;
}

/**
 * @tc.name: __recv_chk_001
 * @tc.desc: This test case helps ensure that the recv_chk function can successfully receive data from a connected
 *           socket and that the received data matches the expected content.
 * @tc.type: FUNC
 **/
HWTEST_F(FortifyRecvchkTest, __recv_chk_001, TestSize.Level1)
{
    const char* sockPath = "test_recv_chk";
    struct ReturnVal returnVal;
    SockInit(sockPath, returnVal);
    DataLink ConnectData;
    ConnectData.callbackFn = Send;
    ConnectData.sockPath = sockPath;

    struct sockaddr_un* addr = &returnVal.addr;
    pthread_t thread;
    int ret = pthread_create(&thread, nullptr, ConnectFunc, &ConnectData);
    EXPECT_EQ(0, ret);
    CallSelectFunc(returnVal);

    socklen_t len = sizeof(*addr);
    int acceptFd = accept(returnVal.fd, reinterpret_cast<struct sockaddr*>(addr), &len);
    EXPECT_NE(acceptFd, -1) << strerror(errno);
    char buf[BUFSIZ];
    memset(buf, 0, sizeof(buf));
    size_t len1 = sizeof(buf);
    size_t result = static_cast<size_t>(__recv_chk(acceptFd, buf, len1, len1, 0));
    EXPECT_EQ(strlen(g_testRecv) + 1, result);
    EXPECT_STREQ(g_testRecv, buf);
    close(acceptFd);

    void* retVal;
    EXPECT_EQ(0, pthread_join(thread, &retVal));
    EXPECT_EQ(nullptr, retVal);
    close(returnVal.fd);
}
