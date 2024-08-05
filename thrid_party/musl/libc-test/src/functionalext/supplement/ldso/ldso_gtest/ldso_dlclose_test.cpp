#include <dlfcn.h>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include <thread>

#include "libs/ldso_gtest_util.h"

using namespace testing::ext;

typedef void (*FuncTypeBool)(bool*);
typedef void (*FuncTypeVoid)(void);

class LdsoDlcloseTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dlclose_001
 * @tc.desc: Test that after dlclose, symbols in the library cannot be accessed.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlcloseTest, dlclose_001, TestSize.Level1)
{
    void* handle = dlopen("libdlopen_common_close.so", RTLD_NOW | RTLD_GLOBAL);
    ASSERT_NE(handle, nullptr);

    int* testValue = reinterpret_cast<int*>(dlsym(handle, "g_testNumber"));
    ASSERT_NE(testValue, nullptr);
    EXPECT_EQ(TEST_NUM_1000, *testValue);
    void* fn = dlsym(handle, "DlopenCommon");
    ASSERT_NE(fn, nullptr);
    dlclose(handle);

    testValue = reinterpret_cast<int*>(dlsym(RTLD_DEFAULT, "g_testNumber"));
    EXPECT_TRUE(testValue == nullptr);
    fn = dlsym(RTLD_DEFAULT, "DlopenCommon");
    EXPECT_TRUE(fn == nullptr);
}

static void CloseTestFunc(void* handle, bool* isFinalizeFlag, const char* symbolName)
{
    FuncTypeBool fn = reinterpret_cast<FuncTypeBool>(dlsym(handle, symbolName));
    if (!fn) {
        return;
    }
    fn(isFinalizeFlag);
    EXPECT_FALSE(*isFinalizeFlag);
}

/**
 * @tc.name: dlclose_002
 * @tc.desc: Wait for the thread to end and then execute dlcose.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlcloseTest, dlclose_002, TestSize.Level1)
{
    void* handle = dlopen("libldso_thread_test.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    bool isFinalizeFlag = false;
    std::thread threadObj(CloseTestFunc, handle, &isFinalizeFlag, "SetThreadLocalValue");
    threadObj.join();

    ASSERT_TRUE(isFinalizeFlag);
    dlclose(handle);

    handle = dlopen("libldso_thread_test.so", RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlclose_003
 * @tc.desc: Wait for the thread to end and then execute dlcose.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlcloseTest, dlclose_003, TestSize.Level1)
{
    void* handle = dlopen("libldso_thread_test_2.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    bool isFinalizeFlag = false;
    std::thread threadObj(CloseTestFunc, handle, &isFinalizeFlag, "SetThreadLocalValue01");
    threadObj.join();

    ASSERT_TRUE(isFinalizeFlag);
    dlclose(handle);

    handle = dlopen("libldso_thread_test_2.so", RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlclose_004
 * @tc.desc: If the subsequent so depends on the current so, the current so can also be unloaded at last.
 * @tc.type: FUNC
 * libldso_dlclose_test_a.so
 *     -> libldso_dlclose_test_b.so
 *     -> libldso_dlclose_test_c.so
 * libldso_dlclose_test_c.so
 *     -> libldso_dlclose_test_b.so
 */
HWTEST_F(LdsoDlcloseTest, dlclose_004, TestSize.Level1)
{
    FuncTypeVoid fn = nullptr;
    void* handle = dlopen("libldso_dlclose_test_a.so", RTLD_GLOBAL);
    ASSERT_NE(handle, nullptr);

    fn = reinterpret_cast<FuncTypeVoid>(dlsym(RTLD_DEFAULT, "dlclose_test_a"));
    ASSERT_NE(fn, nullptr);
    fn();

    fn = reinterpret_cast<FuncTypeVoid>(dlsym(RTLD_DEFAULT, "dlclose_test_b"));
    ASSERT_NE(fn, nullptr);
    fn();

    fn = reinterpret_cast<FuncTypeVoid>(dlsym(RTLD_DEFAULT, "dlclose_test_c"));
    ASSERT_NE(fn, nullptr);
    fn();

    dlclose(handle);
    // test if unload libldso_dlclose_test_a.so succeed.
    fn = reinterpret_cast<FuncTypeVoid>(dlsym(RTLD_DEFAULT, "dlclose_test_a"));
    ASSERT_EQ(fn, nullptr);

    // test if unload libldso_dlclose_test_b.so succeed.
    fn = reinterpret_cast<FuncTypeVoid>(dlsym(RTLD_DEFAULT, "dlclose_test_b"));
    ASSERT_EQ(fn, nullptr);

    // test if unload libldso_dlclose_test_c.so succeed.
    fn = reinterpret_cast<FuncTypeVoid>(dlsym(RTLD_DEFAULT, "dlclose_test_c"));
    ASSERT_EQ(fn, nullptr);
}
