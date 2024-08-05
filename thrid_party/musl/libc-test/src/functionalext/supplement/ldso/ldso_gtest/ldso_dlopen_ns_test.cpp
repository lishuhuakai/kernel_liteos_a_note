#include <cstdlib>
#include <dlfcn.h>
#include <gtest/gtest.h>
#include <string.h>
#include <sys/mman.h>
#include <thread>

#include "libs/ldso_gtest_util.h"

using namespace testing::ext;

typedef int (*FuncTypeRetInt)(void);

static std::string g_ldsoTestRootPath = "/data/tmp/libcgtest/libs";
static std::string g_ldsoTestNameSpaceOnePath = "/namespace_one_libs";
static std::string g_ldsoTestNameSpaceTwoPath = "/namespace_two_libs";
static std::string g_ldsoTestNameSpaceTwoImplPath = "/namespace_two_impl_libs";

static const char* g_nsOneSo = "libldso_ns_one.so";

class LdsoDlopenNsTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dlopen_ns_001
 * @tc.desc: Test the ability of using namespaces to load link libraries normally.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenNsTest, dlopen_ns_001, TestSize.Level1)
{
    static const char* rootSo = "libldso_ns_root.so";
    const std::string searchPath =
        g_ldsoTestRootPath + g_ldsoTestNameSpaceOnePath + ":" + g_ldsoTestRootPath + g_ldsoTestNameSpaceTwoPath;

    Dl_namespace dlns;
    dlns_init(&dlns, "name_one");
    dlns_create(&dlns, searchPath.c_str());

    void* handle = dlopen_ns(&dlns, rootSo, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn1 = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetNsOneImplNum"));
    ASSERT_NE(fn1, nullptr);
    EXPECT_EQ(TEST_NUM_10, fn1());

    FuncTypeRetInt fn2 = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetNsImplNum"));
    ASSERT_NE(fn2, nullptr);
    EXPECT_EQ(TEST_NUM_10, fn2());

    dlclose(handle);
}

/**
 * @tc.name: dlopen_ns_002
 * @tc.desc: Test the inheritance ability of namespaces.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenNsTest, dlopen_ns_002, TestSize.Level1)
{
    static const char* rootSo = "libldso_ns_root.so";

    const std::string searchPathOne = g_ldsoTestRootPath + g_ldsoTestNameSpaceOnePath;
    const std::string searchPathTwo = g_ldsoTestRootPath + g_ldsoTestNameSpaceTwoPath;

    Dl_namespace dlnsOne;
    dlns_init(&dlnsOne, "name_one");
    dlns_create(&dlnsOne, searchPathOne.c_str());

    Dl_namespace dlnsTwo;
    dlns_init(&dlnsTwo, "name_two");
    dlns_create(&dlnsTwo, searchPathTwo.c_str());
    dlns_inherit(&dlnsTwo, &dlnsOne, g_nsOneSo);

    void* handle = dlopen_ns(&dlnsTwo, rootSo, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn1 = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetNsOneImplNum"));
    ASSERT_NE(fn1, nullptr);
    EXPECT_EQ(0, fn1());

    FuncTypeRetInt fn2 = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetNsImplNum"));
    EXPECT_TRUE(fn2 == nullptr);

    dlclose(handle);
}

/**
 * @tc.name: dlopen_ns_003
 * @tc.desc: Test the ability to uninstall libraries inherited through namespaces.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenNsTest, dlopen_ns_003, TestSize.Level1)
{
    static const char* rootSo = "libldso_ns_root.so";

    const std::string searchPathOne = g_ldsoTestRootPath + g_ldsoTestNameSpaceOnePath;
    const std::string searchPathTwo = g_ldsoTestRootPath + g_ldsoTestNameSpaceTwoPath;

    Dl_namespace dlnsOne;
    dlns_init(&dlnsOne, "name_one");
    dlns_create(&dlnsOne, searchPathOne.c_str());

    Dl_namespace dlnsTwo;
    dlns_init(&dlnsTwo, "name_two");
    dlns_create(&dlnsTwo, searchPathTwo.c_str());
    dlns_inherit(&dlnsTwo, &dlnsOne, g_nsOneSo);

    void* handle = dlopen_ns(&dlnsTwo, rootSo, RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);

    handle = dlopen_ns(&dlnsTwo, rootSo, RTLD_NOW | RTLD_NOLOAD);
    EXPECT_TRUE(handle == nullptr);

    handle = dlopen_ns(&dlnsOne, g_nsOneSo, RTLD_NOW | RTLD_NOLOAD);
    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_ns_004
 * @tc.desc: Test that when namespaces are inherited from each other,
 *           there will be no exceptions during the loading process.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenNsTest, dlopen_ns_004, TestSize.Level1)
{
    static const char* sharedSo = "libnoexist.so";
    const std::string searchPath = g_ldsoTestRootPath + g_ldsoTestNameSpaceOnePath;

    Dl_namespace dlnsOne;
    dlns_init(&dlnsOne, "name_one");
    dlns_create(&dlnsOne, searchPath.c_str());

    Dl_namespace dlnsTwo;
    dlns_init(&dlnsTwo, "name_two");
    dlns_create(&dlnsTwo, searchPath.c_str());
    dlns_inherit(&dlnsTwo, &dlnsOne, sharedSo);
    dlns_inherit(&dlnsOne, &dlnsTwo, sharedSo);

    void* handle = dlopen_ns(&dlnsOne, sharedSo, RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);
}

#if defined(__arm__) || defined(__aarch64__)
/**
 * @tc.name: dlopen_ns_005
 * @tc.desc: Open an uncompressed link library in an aligned zip file by setting search path.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenNsTest, dlopen_ns_005, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_zip_test.so", RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);

    Dl_namespace dlns;
    dlns_init(&dlns, "lspath");
    dlns_create(&dlns, (g_ldsoTestRootPath + "/libzipalign_lspath.zip" + "!/libso").c_str());

    handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    int* value = reinterpret_cast<int*>(dlsym(handle, "g_testNumber"));
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, TEST_NUM_1000);

    dlclose(handle);
}
#endif