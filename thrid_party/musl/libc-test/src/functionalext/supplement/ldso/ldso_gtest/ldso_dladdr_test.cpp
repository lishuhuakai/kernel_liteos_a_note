#include <dlfcn.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class LdsoDladdrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dladdr_001
 * @tc.desc: Test that dladdr can execute correctly.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDladdrTest, dladdr_001, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_common.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "DlopenCommon");
    ASSERT_NE(fn, nullptr);
    Dl_info dlInfo;
    int ret = dladdr(fn, &dlInfo);
    EXPECT_TRUE(ret);
    // Specify the symbol name closest to the specified address.
    EXPECT_STREQ(dlInfo.dli_sname, "DlopenCommon");
    // The module name of the loading module containing address.
    const char* fileName = "dlopen_common";
    const char* result = strstr(dlInfo.dli_fname, fileName);
    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(dlInfo.dli_saddr, fn);

    dlclose(handle);
}

/**
 * @tc.name: dladdr_002
 * @tc.desc: Test that ability to open and use library with gnu style of symbol hash.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDladdrTest, dladdr_002, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_gnu_hash.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "DlopenCommon");
    ASSERT_NE(fn, nullptr);
    Dl_info dlInfo;
    int ret = dladdr(fn, &dlInfo);
    EXPECT_TRUE(ret);
    // Specify the symbol name closest to the specified address.
    EXPECT_STREQ(dlInfo.dli_sname, "DlopenCommon");
    // The module name of the loading module containing address.
    const char* fileName = "dlopen_gnu_hash";
    const char* result = strstr(dlInfo.dli_fname, fileName);
    EXPECT_TRUE(result != nullptr);
    // The actual address closest to the symbol
    EXPECT_EQ(dlInfo.dli_saddr, fn);

    dlclose(handle);
}

/**
 * @tc.name: dladdr_003
 * @tc.desc: Test that ability to open and use library with sysv style of symbol hash.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDladdrTest, dladdr_003, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_sysv_hash.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "DlopenCommon");
    ASSERT_NE(fn, nullptr);
    Dl_info dlInfo;
    int ret = dladdr(fn, &dlInfo);
    EXPECT_TRUE(ret);
    // Specify the symbol name closest to the specified address.
    EXPECT_STREQ(dlInfo.dli_sname, "DlopenCommon");
    // The module name of the loading module containing address.
    const char* fileName = "dlopen_sysv_hash";
    const char* result = strstr(dlInfo.dli_fname, fileName);
    EXPECT_TRUE(result != nullptr);
    // The actual address closest to the symbol
    EXPECT_EQ(dlInfo.dli_saddr, fn);

    dlclose(handle);
}

#if defined(__arm__) || defined(__aarch64__)
/**
 * @tc.name: dladdr_004
 * @tc.desc: Test the ability to correctly parse methods in libc.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDladdrTest, dladdr_004, TestSize.Level1)
{
    Dl_info dlInfo;
    int ret = dladdr((void*)(&printf), &dlInfo);
    EXPECT_TRUE(ret);
    // Specify the symbol name closest to the specified address.
    EXPECT_STREQ(dlInfo.dli_sname, "printf");
// The library name of the loading library containing address.
#ifdef __arm__
    const char* fileName = "ld-musl-arm.so";
#else
    const char* fileName = "ld-musl-aarch64.so";
#endif

    const char* result = strstr(dlInfo.dli_fname, fileName);
    EXPECT_TRUE(result != nullptr);
    // The actual address closest to the symbol
    EXPECT_EQ(dlInfo.dli_saddr, (void*)(&printf));
}
#endif

/**
 * @tc.name: dladdr_005
 * @tc.desc: Test invalid parameter.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDladdrTest, dladdr_005, TestSize.Level1)
{
    Dl_info dlInfo;
    int ret = dladdr(nullptr, &dlInfo);
    EXPECT_TRUE(!ret);

    int addr = 123;
    ret = dladdr(&addr, &dlInfo);
    EXPECT_TRUE(!ret);
}
