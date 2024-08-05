#include <dlfcn.h>
#include <gtest/gtest.h>
#include <stdlib.h>

#include "libs/ldso_gtest_util.h"

using namespace std;
using namespace testing::ext;

typedef int (*FuncTypeRetInt)(void);
typedef bool (*FuncTypeRetbool)(void);

class LdsoDlsymTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" int DlsymGetNumBySelf()
{
    return 1;
}
extern "C" int DlsymLowFunc()
{
    return 0;
}

extern "C" int DlsymLowFuncYet()
{
    return 0;
}

/**
 * @tc.name: dlsym_001
 * @tc.desc: Test dlsym can find and use sofile symbol.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_001, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_get_answer.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetAnswer"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(), EXPECT_RETURN_VALUE_23);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_002
 * @tc.desc: Test dlsym find and use a not found symbol.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_002, TestSize.Level1)
{
    void* handle = dlopen("libempty.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    EXPECT_EQ(dlsym(handle, ""), nullptr);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_003
 * @tc.desc: Test dlsym failures.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_003, TestSize.Level1)
{
    void* handle = dlopen("libempty.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    EXPECT_EQ(dlsym(handle, "NotFoundSymbol"), nullptr);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_004
 * @tc.desc: Test dlsym can open symbol from dependence.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_004, TestSize.Level1)
{
    void* handle = dlopen("libldso_dlsym_dependent.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    int (*func)() = reinterpret_cast<int (*)()>(dlsym(handle, "GetNum"));
    ASSERT_NE(func, nullptr);
    EXPECT_EQ(TEST_NUM_10, func());

    dlclose(handle);
}

/**
 * @tc.name: dlsym_005
 * @tc.desc: Test dlsym find and use symbol from dependent sofile with preload.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_005, TestSize.Level1)
{
    void* prehandle = dlopen("libdlsym_get_symbol_impl.so", RTLD_NOW | RTLD_LOCAL);
    void* handle = dlopen("libdlsym_get_symbol.so", RTLD_NOW | RTLD_LOCAL);

    ASSERT_NE(prehandle, nullptr);
    ASSERT_NE(handle, nullptr);

    EXPECT_EQ(nullptr, dlsym(RTLD_DEFAULT, "g_num"));

    FuncTypeRetInt func = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlsymGetTheSymbol"));
    ASSERT_NE(func, nullptr);
    EXPECT_EQ(TEST_NUM_10, func());

    dlclose(handle);
    dlclose(prehandle);
}

/**
 * @tc.name: dlsym_006
 * @tc.desc: Test dlsym can not find global symbol by sofile handle.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_006, TestSize.Level1)
{
    void* handle = dlopen("libempty.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    void* handle01 = dlopen("libdlsym_get_answer.so", RTLD_NOW | RTLD_GLOBAL);
    ASSERT_NE(handle01, nullptr);

    void* fn = dlsym(handle, "GetAnswer");
    EXPECT_EQ(fn, nullptr);

    fn = dlsym(handle, "DlsymGetNumBySelf");
    EXPECT_EQ(fn, nullptr);

    dlclose(handle);
    dlclose(handle01);
}

/**
 * @tc.name: dlsym_007
 * @tc.desc: Test dlsym find and use a defined symbol by RTLD_NEXT.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_007, TestSize.Level1)
{
    void* fn = dlsym(RTLD_NEXT, "printf");
    EXPECT_TRUE(fn != nullptr);
}

/**
 * @tc.name: dlsym_008
 * @tc.desc: Test dlsym find and use a undefined symbol by RTLD_DEFAULT.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_008, TestSize.Level1)
{
    void* fn = dlsym(RTLD_DEFAULT, "UNDEFINED_SYMBOL");
    EXPECT_TRUE(fn == nullptr);
}

/**
 * @tc.name: dlsym_009
 * @tc.desc: Test dlsym find and use a defined symbol by RTLD_DEFAULT.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_009, TestSize.Level1)
{
    void* fn = dlsym(RTLD_DEFAULT, "printf");
    EXPECT_TRUE(fn != nullptr);
}

/**
 * @tc.name: dlsym_010
 * @tc.desc: Test dlsym find and use a undefined symbol by RTLD_NEXT.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_010, TestSize.Level1)
{
    void* fn = dlsym(RTLD_NEXT, "UNDEFINED_SYMBOL");
    EXPECT_TRUE(fn == nullptr);
}

/**
 * @tc.name: dlsym_011
 * @tc.desc: Test dlsym can not find and use a undefined weak symbol.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_011, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_weak_func_undefined.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlsymWeakUndefined"));
    EXPECT_EQ(fn, nullptr);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_012
 * @tc.desc: Test dlsym find and use a weak symbol.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_012, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_weak_func.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "WeakFunc"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(), EXPECT_RETURN_VALUE_66);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_013
 * @tc.desc: Test dlsym flag RTLD_NEXT can use in dlopened library.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_013, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_lib_use_rtld_next.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    void* (*fn)() = reinterpret_cast<void* (*)()>(dlsym(handle, "RtldNextFunc"));
    ASSERT_NE(fn, nullptr);

    void* fn1 = dlsym(RTLD_DEFAULT, "printf");
    ASSERT_NE(fn1, nullptr);
    EXPECT_EQ(fn1, fn());

    dlclose(handle);
}

/**
 * @tc.name: dlsym_014
 * @tc.desc: Test the linker to find the correct symbol through the symbol version(only one version).
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_014, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_symbol_impl_v1.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt sym = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetDlsymVersion"));
    ASSERT_NE(sym, nullptr);
    EXPECT_EQ(sym(), 1);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_015
 * @tc.desc: Test the linker to find the correct symbol through the symbol version(Two versions).
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_015, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_symbol_impl_v2.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt sym = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetDlsymVersion"));
    ASSERT_NE(sym, nullptr);
    EXPECT_EQ(sym(), EXPECT_RETURN_VALUE_2);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_016
 * @tc.desc: Test the linker to find the correct symbol through the symbol version(Version 2 has two implementations).
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_016, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_symbol_v2_relro.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt sym = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetDlsymVersion"));
    ASSERT_NE(sym, nullptr);
    EXPECT_EQ(sym(), EXPECT_RETURN_VALUE_22);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_017
 * @tc.desc: Test the linker to find the correct symbol through the symbol version(Three versions).
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_017, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_symbol_v3_relro.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt sym = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetDlsymVersion"));
    ASSERT_NE(sym, nullptr);
    EXPECT_EQ(sym(), EXPECT_RETURN_VALUE_3);

    dlclose(handle);
}

/**
 * @tc.name: dlsym_018
 * @tc.desc: Test the linker to find the correct symbol through the symbol version.
 *           Directly obtain symbols with multiple versions and confirm obtaining the latest version.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlsym_018, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_symbol_version_3.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt sym = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlsymVersion"));
    ASSERT_NE(sym, nullptr);
    ASSERT_EQ(sym(), EXPECT_RETURN_VALUE_3);

    dlclose(handle);
}

/**
 * @tc.name: dlvsym_001
 * @tc.desc: Test that dlvsym can obtain the specified version of symbols.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlsymTest, dlvsym_001, TestSize.Level1)
{
    void* handle = dlopen("libdlsym_symbol_version_3.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlvsym(handle, "DlsymVersion", "NullVersion"));
    EXPECT_TRUE(fn == nullptr);
    fn = reinterpret_cast<FuncTypeRetInt>(dlvsym(handle, "DlsymVersion", "DlsymVersion_2"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(), EXPECT_RETURN_VALUE_2);

    dlclose(handle);
}
