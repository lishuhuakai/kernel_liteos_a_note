#include <dlfcn.h>
#include <gtest/gtest.h>
#include <libgen.h>
#include <sys/auxv.h>
#include <unistd.h>

#include "libs/ldso_gtest_util.h"

using namespace testing::ext;

#define INIT_ORDER 321
#define FINALIZE_ORDER 123

typedef int (*FuncTypeRetInt)(void);
typedef void (*FuncTypeBool)(bool*);
static std::string g_ldsoTestRootPath = "/data/tmp/libcgtest/libs";

class LdsoDlopenTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dlopen_001
 * @tc.desc: Test the loading order of dynamic link libraries.
 *           libdlopen_relocation.so(RelocationTest, calls RelocationTestOrder)
 *            |-----1.so(RelocationTestOrder)->expect value
 *            |-----2.so(RelocationTestOrder)
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_001, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_relocation.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "RelocationTest"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_15, fn());

    dlclose(handle);
}

/**
 * @tc.name: dlopen_002
 * @tc.desc: Test the loading order of dynamic link libraries.
 *           libdlopen_order_02.so(LoadOrderTest01, calls LoadOrderTest)
 *            |-----1_2.so
 *            |-----|-----1.so(LoadOrderTest)
 *            |     |-----2.so(LoadOrderTest, LoadOrderTest02)->expect value2
 *            |-----3.so(LoadOrderTest)->expect value1
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_002, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_order_02.so", RTLD_NOW | RTLD_GLOBAL);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(RTLD_DEFAULT, "LoadOrderTest01"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_3, fn());
    fn = reinterpret_cast<FuncTypeRetInt>(dlsym(RTLD_DEFAULT, "LoadOrderTest02"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_12, fn());

    dlclose(handle);
}

/**
 * @tc.name: dlopen_003
 * @tc.desc: Test the loading order of dynamic link libraries.
 *           libdlopen_order_003.so
 *            |-----003_1.so
 *            |-----|-----003_1_1.so(DlopenOrderTestImpl003)->expect value
 *            |     |-----003_1_2.so(DlopenOrderTestImpl003)
 *            |-----003_2.so
 *            |-----|-----003_2_1.so(DlopenOrderTestImpl003)
 *            |     |-----003_2_2.so(DlopenOrderTest003, calls DlopenOrderTestImpl003)
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_003, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_order_003.so", RTLD_NOW | RTLD_LOCAL);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlopenOrderTest003"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_311, fn());

    dlclose(handle);
}

/**
 * @tc.name: dlopen_004
 * @tc.desc: Test the loading order of dynamic link libraries.
 *           libdlopen_005.so
 *            |-----005_1.so
 *            |-----|-----005_1_1.so(DlopenTestImpl005)->expect value
 *            |     |-----005_1_2.so(DlopenTestImpl005)
 *            |-----005_2.so(DlopenTest005, calls DlopenTestImpl005)
 *            |-----|-----005_2_1.so(DlopenTestImpl005)
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_004, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_005.so", RTLD_NOW | RTLD_LOCAL);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlopenTest005"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_511, fn());

    dlclose(handle);
}

/**
 * @tc.name: dlopen_005
 * @tc.desc: Test the loading order of dynamic link libraries.
 *            libdlopen_test_dependency.so(DlopenTestDependencyImpl)
 *            |-----libdlopen_test_dependency_1.so(DlopenTestDependency, calls DlopenTestDependencyImpl)
 *            |-----|-----libdlopen_test_dependency_1_1_invalid.so->soname libdlopen_test_dependency_1_1.so
 *            |-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|----libdlopen_test_dependency_1_1_1.so
 *            |-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|----|----libdlopen_test_dependency_1.so
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_005, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_test_dependency.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlopenTestDependency"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(1, fn());
    dlclose(handle);

    handle = dlopen("libdlopen_test_dependency.so", RTLD_NOW | RTLD_NOLOAD);
    EXPECT_TRUE(handle == nullptr);

    handle = dlopen("libdlopen_test_dependency_1.so", RTLD_NOW | RTLD_NOLOAD);
    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_006
 * @tc.desc: If the "so" has been loaded, when using the RTLD_NOLOAD to dlopen the "so",
 *          the same handle of the "so" can still be returned.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_006, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_common.so", RTLD_NOLOAD);
    EXPECT_TRUE(handle == nullptr);
    handle = dlopen("libdlopen_common.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    void* handle01 = dlopen("libdlopen_common.so", RTLD_NOLOAD);
    ASSERT_NE(handle01, nullptr);
    EXPECT_EQ(handle, handle01);

    dlclose(handle);
    dlclose(handle01);
}

/**
 * @tc.name: dlopen_007
 * @tc.desc: Test preloads a dependent "003_1.so" before loading "order_003.so".
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_007, TestSize.Level1)
{
    void* handle01 = dlopen("libdlopen_order_003_1.so", RTLD_NOLOAD);
    EXPECT_TRUE(handle01 == nullptr);
    void* handle = dlopen("libdlopen_order_003.so", RTLD_NOLOAD);
    EXPECT_TRUE(handle == nullptr);

    handle01 = dlopen("libdlopen_order_003_1.so", RTLD_NOW);
    ASSERT_NE(handle01, nullptr);
    handle = dlopen("libdlopen_order_003.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlopenOrderTest003"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_311, fn());

    dlclose(handle01);
    dlclose(handle);
}

/**
 * @tc.name: dlopen_008
 * @tc.desc: Test the default mode of dlopen is RTLD_LOCAL.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_008, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_relocation.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "RelocationTest"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_15, fn());
    dlclose(handle);

    handle = dlopen("libdlopen_relocation.so", RTLD_NOW | RTLD_LOCAL);
    ASSERT_NE(handle, nullptr);
    fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "RelocationTest"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_15, fn());
    dlclose(handle);
}

/**
 * @tc.name: dlopen_009
 * @tc.desc: Test when the mode of dlopen is RTLD_GLOBAL, dlsym can use RTLD_DEFAULT to find symbols.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_009, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_relocation.so", RTLD_NOW | RTLD_GLOBAL);
    ASSERT_NE(handle, nullptr);
    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(RTLD_DEFAULT, "RelocationTest"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_15, fn());

    void* handle01 = dlopen(nullptr, RTLD_NOW);
    ASSERT_NE(handle01, nullptr);
    fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle01, "RelocationTest"));
    ASSERT_NE(fn, nullptr);

    dlclose(handle);
    dlclose(handle01);
}

/**
 * @tc.name: dlopen_010
 * @tc.desc: Test libdlopen_load_so.so depends on libdlopen_load_so_1.so that calls dlopen.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_010, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_load_so.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);
}

/**
 * @tc.name: dlopen_011
 * @tc.desc: After using RTLD_NODELETE to load library with dlopen,
 *          during dlclose, the library is not unloaded.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_011, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_nodelete.so", RTLD_NOW | RTLD_NODELETE);
    ASSERT_NE(handle, nullptr);
    FuncTypeBool fn = reinterpret_cast<FuncTypeBool>(dlsym(handle, "DlopenNodeleteSetIsClosedPtr"));
    ASSERT_NE(fn, nullptr);
    bool isClosed = false;
    fn(&isClosed);

    int32_t* value = reinterpret_cast<int32_t*>(dlsym(handle, "g_testValue"));
    ASSERT_NE(value, nullptr);
    *value = TEST_NUM_20;
    dlclose(handle);

    value = reinterpret_cast<int32_t*>(dlsym(handle, "g_testValue"));
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, TEST_NUM_20);
    EXPECT_FALSE(isClosed);
}

/**
 * @tc.name: dlopen_012
 * @tc.desc: Test invalid library.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_012, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_invalid.so", RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);
}

static int32_t g_finalizeOrder;

static void FinalizeCallback(int32_t order)
{
    g_finalizeOrder = g_finalizeOrder * TEST_NUM_10 + order;
}

/**
 * @tc.name: dlopen_013
 * @tc.desc: Test the construction and deconstruction order of libraries.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_013, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_init_finalize.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    int32_t* initOrder = reinterpret_cast<int32_t*>(dlsym(handle, "g_initOrder"));
    ASSERT_NE(initOrder, nullptr);
    EXPECT_EQ(INIT_ORDER, *initOrder);

    void (*finalizeCb)(void (*callback)(int32_t order));
    finalizeCb = reinterpret_cast<void (*)(void (*callback)(int32_t order))>(dlsym(handle, "SetFinalizeCallback"));
    ASSERT_NE(finalizeCb, nullptr);
    finalizeCb(FinalizeCallback);
    dlclose(handle);
    EXPECT_EQ(FINALIZE_ORDER, g_finalizeOrder);
}

/**
 * @tc.name: dlopen_014
 * @tc.desc: Test that library with global flags can be dlopened.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_014, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_global.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);
}

/**
 * @tc.name: dlopen_015
 * @tc.desc: Test that library with rpath can be dlopened.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_015, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_rpath.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "Dlopen005");
    ASSERT_NE(fn, nullptr);
    dlclose(handle);
}

/**
 * @tc.name: dlopen_016
 * @tc.desc: Test that dlopen successfully by using rpath's absolute path.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_016, TestSize.Level1)
{
    void* handle = dlopen((g_ldsoTestRootPath + "/libdlopen_rpath.so").c_str(), RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlopenTestImpl005"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(), EXPECT_RETURN_VALUE_511);

    dlclose(handle);
}

/**
 * @tc.name: dlopen_017
 * @tc.desc: Test that dlopen successfully by using absolute path.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_017, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_common.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "DlopenCommon");
    ASSERT_NE(fn, nullptr);
    Dl_info dlInfo;
    int ret = dladdr(fn, &dlInfo);
    EXPECT_TRUE(ret);
    const char* fileName = "dlopen_common";
    const char* result = strstr(dlInfo.dli_fname, fileName);
    EXPECT_TRUE(result != nullptr);

    void* handle01 = dlopen(dlInfo.dli_fname, RTLD_NOW);
    ASSERT_NE(handle01, nullptr);
    EXPECT_TRUE(handle == handle01);

    dlclose(handle);
    dlclose(handle01);
}

/**
 * @tc.name: dlopen_018
 * @tc.desc: Test the ability to use symbolic link dlopen libraries.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_018, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_common.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "DlopenCommon");
    ASSERT_NE(fn, nullptr);
    Dl_info dlInfo;
    int ret = dladdr(fn, &dlInfo);
    EXPECT_TRUE(ret);
    const char* fileName = "dlopen_common";
    const char* result = strstr(dlInfo.dli_fname, fileName);
    EXPECT_TRUE(result != nullptr);

    // create symlink
    std::string path = dlInfo.dli_fname;
    char* dir = dirname(const_cast<char*>(path.c_str()));
    std::string dirStr = dir;
    std::string linkPath = dirStr + "/" + "libctest_dlopen_common_link" + ".so";
    EXPECT_TRUE(!symlink(dlInfo.dli_fname, linkPath.c_str()));

    // dlopen symlink
    std::string linkFile = "libctest_dlopen_common_link.so";
    void* handle01 = dlopen(linkFile.c_str(), RTLD_NOW);
    ASSERT_NE(handle01, nullptr);
    EXPECT_TRUE(handle == handle01);

    EXPECT_TRUE(!unlink(linkPath.c_str()));
    dlclose(handle);
    dlclose(handle01);
}

/**
 * @tc.name: dlopen_019
 * @tc.desc: Test the ability to load virtual dynamic shared libraries.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_019, TestSize.Level1)
{
    unsigned long int result = getauxval(AT_SYSINFO_EHDR);
    if (!result) {
        return;
    }
    void* handle = dlopen("linux-gate.so.1", RTLD_NOW);
    bool isloaded = (handle != nullptr);

    handle = dlopen("linux-vdso.so.1", RTLD_NOW);
    isloaded |= (handle != nullptr);

    ASSERT_TRUE(isloaded);
    dlclose(handle);
}

/**
 * @tc.name: dlopen_020
 * @tc.desc: Test RTLD_DEFAULT searches for symbols in the default shared target search order.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenTest, dlopen_020, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_relocation.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(RTLD_DEFAULT, "RelocationTest"));
    EXPECT_TRUE(fn == nullptr);

    fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "RelocationTest"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(EXPECT_RETURN_VALUE_15, fn());

    dlclose(handle);
}
