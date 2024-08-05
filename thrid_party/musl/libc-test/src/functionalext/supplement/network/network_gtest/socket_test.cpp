#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <netinet/ip.h>
#include <string.h>
#include <sys/un.h>
using namespace testing::ext;

class SocketTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static void CloseOnExec(int fd, bool closeOnExec)
{
    int flags = fcntl(fd, F_GETFD);
    EXPECT_NE(flags, -1);
    if (closeOnExec) {
        flags |= FD_CLOEXEC;
    } else {
        flags &= ~FD_CLOEXEC;
    }
    int err = fcntl(fd, F_SETFD, flags);
    EXPECT_NE(err, -1);
}

struct DataLink {
    std::function<bool(int)> callbackFn;
    const char* sockPath;
};

static void* FnLink(void* data)
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
    int result1 = bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    int result2 = listen(fd, 1);
    EXPECT_NE(-1, result1);
    EXPECT_NE(-1, result2);

    returnVal.fd = fd;
    returnVal.addr = addr;
}

static void SelectFunc(struct ReturnVal returnVal)
{
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(returnVal.fd, &readSet);
    timeval tv = { .tv_sec = 5, .tv_usec = 0 };
    int result = select(returnVal.fd + 1, &readSet, nullptr, nullptr, &tv);
    EXPECT_GT(result, 0);
}

const char* g_recvMmsgs[] = {
    "musl_recvmmsg_001",
    "musl_recvmmsg_02",
    "musl_recvmmsg_3",
};

#define NUM_RECV_MSGS (sizeof(g_recvMmsgs) / sizeof(const char*))

static bool SendMMsgTest(int fd)
{
    for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
        if (send(fd, g_recvMmsgs[i], strlen(g_recvMmsgs[i]) + 1, 0) < 0) {
            return false;
        }
    }
    return true;
}

/**
 * @tc.name: accept4_001
 * @tc.desc: Test the functionality of the "accept4" function by creating a Unix domain socket, setting up a
 *           connection, and accepting incoming connections using accept4 with the SOCK_CLOEXEC flag.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, accept4_001, TestSize.Level1)
{
    const char* sockPath = "test_accept4";
    struct ReturnVal returnVal;

    SockInit(sockPath, returnVal);
    DataLink connData;
    connData.callbackFn = nullptr;
    connData.sockPath = sockPath;

    struct sockaddr_un* addr = &returnVal.addr;
    pthread_t thread;
    int cresult = pthread_create(&thread, nullptr, FnLink, &connData);
    EXPECT_EQ(0, cresult);
    SelectFunc(returnVal);

    socklen_t len = sizeof(*addr);
    struct sockaddr* sockaddrPtr = reinterpret_cast<struct sockaddr*>(addr);
    int acceptFd = accept4(returnVal.fd, sockaddrPtr, &len, SOCK_CLOEXEC);
    EXPECT_NE(acceptFd, -1);
    CloseOnExec(acceptFd, true);
    close(acceptFd);

    void* retVal;
    int jresult = pthread_join(thread, &retVal);
    EXPECT_EQ(0, jresult);
    EXPECT_EQ(nullptr, retVal);
    close(returnVal.fd);
}

/**
 * @tc.name: accept4_002
 * @tc.desc: Test the functionality of the "accept4" function by passing an invalid file descriptor (-1) and
 *           expecting it to return -1
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, accept4_002, TestSize.Level1)
{
    int err = accept4(-1, nullptr, nullptr, 0);
    EXPECT_EQ(-1, err);
}

/**
 * @tc.name: recvmmsg_001
 * @tc.desc: Test the functionality of the "recvmmsg" function by creating a Unix domain socket, setting up a
 *           connection, and receiving multiple messages using "recvmmsg".
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, recvmmsg_001, TestSize.Level1)
{
    const char* sockPath = "test_revmmsg";
    struct ReturnVal returnVal;

    SockInit(sockPath, returnVal);
    DataLink connData;
    connData.callbackFn = SendMMsgTest;
    connData.sockPath = sockPath;

    struct sockaddr_un* addr = &returnVal.addr;
    pthread_t thread;
    int cresult = pthread_create(&thread, nullptr, FnLink, &connData);
    EXPECT_EQ(0, cresult);
    SelectFunc(returnVal);

    socklen_t len = sizeof(*addr);
    int acceptFd = accept(returnVal.fd, reinterpret_cast<struct sockaddr*>(addr), &len);
    EXPECT_NE(acceptFd, -1);

    std::array<struct mmsghdr, NUM_RECV_MSGS> msgs;
    std::array<struct iovec, NUM_RECV_MSGS> io;
    std::array<char[BUFSIZ], NUM_RECV_MSGS> bufs;
    for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
        io[i].iov_base = bufs[i];
        io[i].iov_len = strlen(g_recvMmsgs[i]) + 1;

        msgs[i].msg_hdr.msg_iov = &io[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
        msgs[i].msg_len = sizeof(struct msghdr);
    }
    struct timespec ts {
        5, 0
    };
    EXPECT_EQ(NUM_RECV_MSGS, static_cast<size_t>(recvmmsg(acceptFd, msgs.data(), NUM_RECV_MSGS, 0, &ts)));
    for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
        EXPECT_STREQ(g_recvMmsgs[i], bufs[i]);
    }

    void* retVal;
    int jresult = pthread_join(thread, &retVal);
    EXPECT_EQ(0, jresult);
    EXPECT_EQ(nullptr, retVal);
    close(returnVal.fd);
}

/**
 * @tc.name: recvmmsg_002
 * @tc.desc: Test the error handling functionality of the "recvmmsg" function by passing an invalid file
 *           descriptor (-1) and expecting it to return -1
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, recvmmsg_002, TestSize.Level1)
{
    EXPECT_EQ(-1, recvmmsg(-1, nullptr, 0, 0, nullptr));
    EXPECT_EQ(EBADF, errno);
}

const char* g_sendMMsg[] = {
    "musl_sendmmsg_001",
    "musl_sendmmsg_02",
    "musl_sendmmsg_3",
};
#define SEND_MSGS_NUM (sizeof(g_sendMMsg) / sizeof(const char*))

static bool SendMMsg(int fd)
{
    struct mmsghdr msgs[SEND_MSGS_NUM];
    memset(&msgs, 0, sizeof(msgs));
    struct iovec io[SEND_MSGS_NUM];
    memset(&io, 0, sizeof(io));
    for (size_t i = 0; i < SEND_MSGS_NUM; i++) {
        io[i].iov_base = const_cast<char*>(g_sendMMsg[i]);
        io[i].iov_len = strlen(g_sendMMsg[i]) + 1;
        msgs[i].msg_hdr.msg_iov = &io[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
        msgs[i].msg_len = sizeof(msghdr);
    }
    if (sendmmsg(fd, msgs, SEND_MSGS_NUM, 0) < 0) {
        return false;
    }
    return true;
}

/**
 * @tc.name: sendmmsg_001
 * @tc.desc: Test the functionality of the "sendmmsg" function by creating a Unix domain socket, setting up a
 *           connection, and sending multiple messages using "sendmmsg".
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, sendmmsg_001, TestSize.Level1)
{
    const char* sockPath = "test_sendmmsg";
    struct ReturnVal returnVal;
    SockInit(sockPath, returnVal);
    DataLink connData;
    connData.callbackFn = SendMMsg;
    connData.sockPath = sockPath;

    struct sockaddr_un* addr = &returnVal.addr;
    pthread_t thread;
    int cresult = pthread_create(&thread, nullptr, FnLink, &connData);
    EXPECT_EQ(0, cresult);
    SelectFunc(returnVal);

    socklen_t len = sizeof(*addr);
    int acceptFd = accept(returnVal.fd, reinterpret_cast<struct sockaddr*>(addr), &len);
    EXPECT_NE(acceptFd, -1) << strerror(errno);
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(acceptFd, &readSet);
    for (size_t i = 0; i < SEND_MSGS_NUM; i++) {
        timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        EXPECT_LT(0, select(acceptFd + 1, &readSet, nullptr, nullptr, &tv));
        char buf[BUFSIZ];
        int len1 = sizeof(buf);
        ssize_t bytesReceived = recv(acceptFd, buf, len1, 0);
        EXPECT_EQ(strlen(g_sendMMsg[i]) + 1, static_cast<size_t>(bytesReceived));
        EXPECT_STREQ(g_sendMMsg[i], buf);
    }
    close(acceptFd);

    void* retVal;
    int jresult = pthread_join(thread, &retVal);
    EXPECT_EQ(0, jresult);
    EXPECT_EQ(nullptr, retVal);
    close(returnVal.fd);
}

/**
 * @tc.name: sendmmsg_002
 * @tc.desc: Test the error handling functionality of the "sendmmsg" function by passing an invalid file descriptor
 *           (-1) and expecting it to return -1
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, sendmmsg_002, TestSize.Level1)
{
    EXPECT_GE(0, sendmmsg(-1, nullptr, 0, 0));
    EXPECT_EQ(EBADF, errno);
}

/**
 * @tc.name: bind_listen_001
 * @tc.desc: Ensure that the binding and listening operations on the Unix domain socket are functioning as expected.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, bind_listen_001, TestSize.Level1)
{
    auto fd = std::make_unique<int>(socket(PF_UNIX, SOCK_SEQPACKET, 0));
    EXPECT_NE(*fd, -1);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    const char* sockPath = "bind_listen_test1";
    strcpy(addr.sun_path + 1, sockPath);
    int result1 = bind(*fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    int result2 = listen(*fd, 1);
    EXPECT_NE(-1, result1);
    EXPECT_NE(-1, result2);
}

/**
 * @tc.name: accept_001
 * @tc.desc: This test case helps ensure that the socket can successfully accept incoming connections and that the
 *           connection handling thread completes without errors.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, accept_001, TestSize.Level1)
{
    const char* sockPath = "accept_test";
    struct ReturnVal returnVal;
    SockInit(sockPath, returnVal);
    DataLink connData;
    connData.callbackFn = SendMMsg;
    connData.sockPath = sockPath;

    struct sockaddr_un* addr = &returnVal.addr;
    pthread_t thread;
    int cresult = pthread_create(&thread, nullptr, FnLink, &connData);
    EXPECT_EQ(0, cresult);
    SelectFunc(returnVal);

    socklen_t len = sizeof(*addr);
    int acceptFd = accept(returnVal.fd, reinterpret_cast<struct sockaddr*>(addr), &len);
    EXPECT_NE(-1, acceptFd);

    void* retVal;
    int jresult = pthread_join(thread, &retVal);
    EXPECT_EQ(0, jresult);
    EXPECT_EQ(nullptr, retVal);
    close(acceptFd);
    close(returnVal.fd);
}

const char* g_message = "hello";
static bool SendTest(int fd)
{
    if (send(fd, g_message, strlen(g_message) + 1, 0) < 0) {
        return false;
    }
    return true;
}

/**
 * @tc.name: recv_001
 * @tc.desc: This test case helps ensure that the recv function can successfully receive data from a connected
 *           socket and that the received data matches the expected content.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, recv_001, TestSize.Level1)
{
    const char* sockPath = "test_recv";
    struct ReturnVal returnVal;
    SockInit(sockPath, returnVal);
    DataLink connData;
    connData.callbackFn = SendTest;
    connData.sockPath = sockPath;

    struct sockaddr_un* addr = &returnVal.addr;
    pthread_t thread;
    int cresult = pthread_create(&thread, nullptr, FnLink, &connData);
    EXPECT_EQ(0, cresult);
    SelectFunc(returnVal);

    socklen_t len = sizeof(*addr);
    int acceptFd = accept(returnVal.fd, reinterpret_cast<struct sockaddr*>(addr), &len);
    EXPECT_NE(acceptFd, -1) << strerror(errno);
    char buf[BUFSIZ];
    memset(buf, 0, sizeof(buf));
    EXPECT_EQ(strlen(g_message) + 1, static_cast<size_t>(recv(acceptFd, buf, sizeof(buf), 0)));
    EXPECT_STREQ(g_message, buf);
    close(acceptFd);

    void* retVal;
    int jresult = pthread_join(thread, &retVal);
    EXPECT_EQ(0, jresult);
    EXPECT_EQ(nullptr, retVal);
    close(returnVal.fd);
}

/**
 * @tc.name: recv_002
 * @tc.desc: It checks if the function correctly handles the error condition and sets the appropriate errno value.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, recv_002, TestSize.Level1)
{
    int err = recv(-1, nullptr, 0, 0);
    EXPECT_EQ(-1, err);
    EXPECT_EQ(EBADF, errno);
}

/**
 * @tc.name: send_001
 * @tc.desc: This test case helps ensure that the send function can successfully send data to a connected socket and
 *           that the received data matches the expected content.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, send_001, TestSize.Level1)
{
    const char* sockPath = "test_send";
    struct ReturnVal returnVal;
    SockInit(sockPath, returnVal);
    DataLink connData;
    connData.callbackFn = SendTest;
    connData.sockPath = sockPath;

    struct sockaddr_un* addr = &returnVal.addr;
    pthread_t thread;
    int cresult = pthread_create(&thread, nullptr, FnLink, &connData);
    EXPECT_EQ(0, cresult);
    SelectFunc(returnVal);

    socklen_t len = sizeof(*addr);
    int acceptFd = accept(returnVal.fd, reinterpret_cast<struct sockaddr*>(addr), &len);
    EXPECT_NE(acceptFd, -1);
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(acceptFd, &readSet);
    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    EXPECT_LT(0, select(acceptFd + 1, &readSet, nullptr, nullptr, &tv));
    char buf[BUFSIZ];
    size_t len1 = sizeof(buf);
    EXPECT_EQ(strlen(g_message) + 1, static_cast<size_t>(recv(acceptFd, buf, len1, 0)));
    EXPECT_STREQ(g_message, buf);
    close(acceptFd);

    void* retVal;
    int jresult = pthread_join(thread, &retVal);
    EXPECT_EQ(0, jresult);
    EXPECT_EQ(nullptr, retVal);
    close(returnVal.fd);
}

/**
 * @tc.name: send_002
 * @tc.desc: It checks if the function correctly handles the error condition and sets the appropriate errno value.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, send_002, TestSize.Level1)
{
    int err = send(-1, nullptr, 0, 0);
    EXPECT_EQ(-1, err);
    EXPECT_EQ(EBADF, errno);
}

/**
 * @tc.name: socket_001
 * @tc.desc: The purpose of this test case is to verify that the socket function can successfully create a socket of
 *           the specified type (TCP, IPv4) and return a valid file descriptor.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, socket_001, TestSize.Level1)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_GT(fd, 0);
}

/**
 * @tc.name: socketpair_001
 * @tc.desc: A pair of sockets created by testing socketpair can be used for inter process communication
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, socketpair_001, TestSize.Level1)
{
    int sv[2];
    char buf[BUFSIZ];
    int fd = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    EXPECT_NE(-1, fd);
    pid_t pid = fork();
    EXPECT_NE(-1, pid);
    if (pid == 0) {
        close(sv[0]);
        const char* message = "hello";
        write(sv[1], message, strlen(message) + 1);
        _exit(0);
    } else {
        close(sv[1]);
        read(sv[0], buf, sizeof(buf));
        EXPECT_STREQ("hello", buf);
    }
    close(fd);
    close(sv[0]);
    close(sv[1]);
}

/**
 * @tc.name: shutdown_001
 * @tc.desc: Verify that the shutdown function successfully shuts down the sending side of a Unix domain socket,
 *           and that the function returns 0 to indicate successful operation.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, shutdown_001, TestSize.Level1)
{
    int sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
    int result = shutdown(sockfd, SHUT_WR);
    EXPECT_EQ(0, result);
    close(sockfd);
}

/**
 * @tc.name: recvmsg_001
 * @tc.desc: Verify that the recvmsg function can successfully receive data from a connected socket and that the
 *           received data matches the expected content.
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, recvmsg_001, TestSize.Level1)
{
    int ret;
    int socks[2];
    struct msghdr msg;
    struct iovec iov[1];
    char sendBuf[BUFSIZ] = "it is a test";
    struct msghdr msgr;
    struct iovec iovr[1];
    char recvBuf[BUFSIZ];
    ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);

    bzero(&msg, sizeof(msg));
    msg.msg_name = nullptr;
    msg.msg_namelen = 0;
    iov[0].iov_base = sendBuf;
    iov[0].iov_len = sizeof(sendBuf);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    ret = sendmsg(socks[1], &msg, 0);
    if (ret == -1) {
        close(socks[0]);
        close(socks[1]);
    }

    bzero(&msgr, sizeof(msgr));
    msgr.msg_name = nullptr;
    msgr.msg_namelen = 0;
    iovr[0].iov_base = &recvBuf;
    iovr[0].iov_len = sizeof(recvBuf);
    msgr.msg_iov = iovr;
    msgr.msg_iovlen = 1;
    ret = recvmsg(socks[0], &msgr, 0);
    EXPECT_NE(-1, ret);
    close(socks[0]);
    close(socks[1]);
}

/**
 * @tc.name: sendmsg_001
 * @tc.desc: Testing that sendmsg can correctly sendmsg buf data
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, sendmsg_001, TestSize.Level1)
{
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    EXPECT_NE(-1, sockfd);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(1234);
    int result = connect(sockfd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    EXPECT_NE(-1, result);
    struct msghdr msg;

    int retval = sendmsg(sockfd, &msg, 2);
    EXPECT_NE(-1, retval);
    close(sockfd);
}

/**
 * @tc.name: sendto_001
 * @tc.desc: Testing the sendto() function to send data on a local socket
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, sendto_001, TestSize.Level1)
{
    int socks[2];
    char sendBuf[BUFSIZ] = "it is a test";
    int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
    ret = sendto(socks[1], sendBuf, sizeof(sendBuf), 0, nullptr, 0);
    EXPECT_NE(-1, ret);
    close(socks[0]);
    close(socks[1]);
}

/**
 * @tc.name: setsockopt_001
 * @tc.desc: Testing that the setsockopt function can correctly set socket options
 * @tc.type: FUNC
 **/
HWTEST_F(SocketTest, setsockopt_001, TestSize.Level1)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_NE(-1, sockfd);
    int option = 1;
    int result = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    EXPECT_NE(-1, result);
    close(sockfd);
}
