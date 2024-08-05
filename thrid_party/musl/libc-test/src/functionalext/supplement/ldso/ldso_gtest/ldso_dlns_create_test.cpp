#include <dlfcn.h>
#include <dlfcn_ext.h>
#include <gtest/gtest.h>

using namespace std;
using namespace testing::ext;

class LdsoDlnsCreateTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dlns_create2_001
 * @tc.desc: Check dlns_create2 flag run success without exception.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlnsCreateTest, dlns_create2_001, TestSize.Level1)
{
    const char* rootSo = "libldso_ns_root.so";
    void* handle = dlopen(rootSo, RTLD_LAZY);
    ASSERT_EQ(handle, nullptr);

    const std::string searchPath =
        "/data/tmp/libcgtest/libs/namespace_one_libs:/data/tmp/libcgtest/libs/namespace_two_libs";
    Dl_namespace dlns;
    dlns_init(&dlns, "create2_ns");
    dlns_create2(&dlns, searchPath.c_str(), CREATE_INHERIT_DEFAULT);
    handle = dlopen_ns(&dlns, rootSo, RTLD_LAZY);
    ASSERT_NE(handle, nullptr);

    dlclose(handle);
}
