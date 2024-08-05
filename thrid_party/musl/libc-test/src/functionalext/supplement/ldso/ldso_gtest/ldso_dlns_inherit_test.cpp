#include <dlfcn.h>
#include <gtest/gtest.h>
#include <stdlib.h>

using namespace std;
using namespace testing::ext;

class LdsoDlnsInheritTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static const char* g_dllName = "libdlsym_get_answer.so";
static const char* g_sharedLib = "libempty.so";

/**
 * @tc.name: dlns_inherit_001
 * @tc.desc: Test dlns_inherit when ns is null or inherit_ns is null.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlnsInheritTest, dlns_inherit_001, TestSize.Level1)
{
    Dl_namespace dlns1;
    dlns_init(&dlns1, "ns_ori");
    dlns_create(&dlns1, "/data/local/");

    Dl_namespace dlns2;
    dlns_init(&dlns2, "ns_inherit");
    dlns_create2(&dlns2, "src/test", 0);

    ASSERT_NE(0, dlns_inherit(nullptr, nullptr, nullptr));

    ASSERT_NE(0, dlns_inherit(&dlns1, nullptr, nullptr));

    ASSERT_NE(0, dlns_inherit(nullptr, &dlns2, nullptr));

    ASSERT_EQ(0, dlns_inherit(&dlns1, &dlns2, nullptr));
}

/**
 * @tc.name: dlns_inherit_002
 * @tc.desc: check inherit all so file, all so file can find by inherited.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlnsInheritTest, dlns_inherit_002, TestSize.Level1)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "namespaceA");
    dlns_init(&dlnsB, "namespaceB");

    dlns_create(&dlnsA, "/data/tmp/libcgtest");
    dlns_create(&dlnsB, "/data/tmp/libcgtest/libs");

    ASSERT_EQ(dlns_inherit(&dlnsA, &dlnsB, nullptr), 0);

    void* handle = dlopen_ns(&dlnsA, g_dllName, RTLD_LAZY);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);

    void* handle1 = dlopen_ns(&dlnsA, g_sharedLib, RTLD_LAZY);
    ASSERT_NE(handle1, nullptr);
    dlclose(handle1);
}
