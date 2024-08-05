#include <gtest/gtest.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

#define tls_mod_off_t size_t

class ThreadTlsgetaddrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" void* __tls_get_addr(tls_mod_off_t* v);

void ThreadFunc()
{
    tls_mod_off_t v;
    EXPECT_EQ(nullptr, __tls_get_addr(&v));
}

/**
 * @tc.name: __tls_get_addr_001
 * @tc.desc: Invalid parameter passed in, return value is empty.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadTlsgetaddrTest, __tls_get_addr_001, TestSize.Level1)
{
    std::thread t(ThreadFunc);
    t.join();
}