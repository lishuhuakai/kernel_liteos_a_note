#include <dlfcn_ext.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <thread>

#include "libs/ldso_gtest_util.h"

using namespace testing::ext;

typedef int (*FuncTypeRetInt)(void);
#define RELRO_FILE_PATH "./TemporaryFile-XXXXXX"
#define RELRO_FILE_PATH_1 "./TemporaryFile_1-XXXXXX"

class LdsoDlopenExtTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dlopen_ext_001
 * @tc.desc: Test that set dl_extinfo to null, and call dlopen_ext.It is equivalent to dlopen.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_001, TestSize.Level1)
{
    void* handle = dlopen_ext("libdlopen_common_relro.so", RTLD_NOW, nullptr);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "DlopenCommon");
    ASSERT_NE(fn, nullptr);
    dlclose(handle);
}

/**
 * @tc.name: dlopen_ext_002
 * @tc.desc: Test that set dl_extinfo's flag to 0, and call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_002, TestSize.Level1)
{
    dl_extinfo extInfo;
    extInfo.flag = 0;
    void* handle = dlopen_ext("libdlopen_common_relro.so", RTLD_NOW, &extInfo);
    ASSERT_NE(handle, nullptr);

    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "DlopenCommon"));
    ASSERT_NE(fn, nullptr);

    int* testValue = reinterpret_cast<int*>(dlsym(handle, "g_testNumber"));
    ASSERT_NE(testValue, nullptr);
    EXPECT_EQ(fn(), *testValue);
    dlclose(handle);
}

/**
 * @tc.name: dlopen_ext_003
 * @tc.desc: Test that set dl_extinfo's flag to DL_EXT_RESERVED_ADDRESS, and call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_003, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_1024_SQUARE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_1024_SQUARE;
    void* handle = dlopen_ext("libdlopen_common_relro.so", RTLD_NOW, &extInfo);
    ASSERT_NE(handle, nullptr);
    void* testValue = dlsym(handle, "g_testNumber");
    ASSERT_NE(testValue, nullptr);
    int* num = reinterpret_cast<int*>(testValue);
    EXPECT_EQ(TEST_NUM_1000, *num);
    EXPECT_GE(testValue, addr);
    EXPECT_LT(testValue, reinterpret_cast<char*>(addr) + SIZE_1024_SQUARE);

    dlclose(handle);
    munmap(addr, SIZE_1024_SQUARE);
}

/**
 * @tc.name: dlopen_ext_004
 * @tc.desc: Test that set dl_extinfo's flag to DL_EXT_RESERVED_ADDRESS,
 *           and set reserved_size to a small size, then call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_004, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_128, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_128;
    void* handle = dlopen_ext("libdlopen_common_relro.so", RTLD_NOW, &extInfo);
    EXPECT_TRUE(handle == nullptr);
    munmap(addr, SIZE_128);
}

/**
 * @tc.name: dlopen_ext_005
 * @tc.desc: Use empty relro_fd to call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_005, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_1024_SQUARE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_1024_SQUARE;
    extInfo.relro_fd = INVALID_VALUE;

    void* handle = dlopen_ext("libldso_relro_test.so", RTLD_NOW, &extInfo);
    ASSERT_NE(handle, nullptr);
    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetRelroValue"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(TEST_NUM_50, fn());

    int* testValue = reinterpret_cast<int*>(dlsym(handle, "g_testNumber"));
    ASSERT_NE(testValue, nullptr);
    EXPECT_EQ(TEST_NUM_1000, *testValue);

    dlclose(handle);
    munmap(addr, SIZE_1024_SQUARE);
}

/**
 * @tc.name: dlopen_ext_006
 * @tc.desc: Test that set dl_extinfo's flag to DL_EXT_RESERVED_ADDRESS_RECURSIVE,
 *           and then call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_006, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_1024_SQUARE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS | DL_EXT_RESERVED_ADDRESS_RECURSIVE;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_1024_SQUARE;
    void* handle = dlopen_ext("libldso_relro_recursive_test.so", RTLD_NOW, &extInfo);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "GetRelroValue");
    ASSERT_NE(fn, nullptr);
    void* fn1 = dlsym(handle, "GetRelroRecursiveValue");
    ASSERT_NE(fn1, nullptr);
    void* testValue = dlsym(handle, "g_testNumber");
    ASSERT_NE(testValue, nullptr);
    EXPECT_GE(fn, addr);
    EXPECT_LT(fn, reinterpret_cast<char*>(addr) + SIZE_1024_SQUARE);
    EXPECT_GE(fn1, addr);
    EXPECT_LT(fn1, reinterpret_cast<char*>(addr) + SIZE_1024_SQUARE);

    FuncTypeRetInt func = reinterpret_cast<FuncTypeRetInt>(fn);
    FuncTypeRetInt func1 = reinterpret_cast<FuncTypeRetInt>(fn1);
    EXPECT_EQ(TEST_NUM_50, func());
    EXPECT_EQ(TEST_NUM_100, func1());
    int* num = reinterpret_cast<int*>(testValue);
    EXPECT_EQ(TEST_NUM_1000, *num);

    dlclose(handle);
    munmap(addr, SIZE_1024_SQUARE);
}

/**
 * @tc.name: dlopen_ext_007
 * @tc.desc: Test that set dl_extinfo's flag to DL_EXT_RESERVED_ADDRESS_RECURSIVE,
 *           and set reserved_size to a small size, then call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_007, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_128, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS | DL_EXT_RESERVED_ADDRESS_RECURSIVE;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_128;
    void* handle = dlopen_ext("libldso_relro_recursive_test.so", RTLD_NOW, &extInfo);
    EXPECT_TRUE(handle == nullptr);

    munmap(addr, SIZE_128);
}

static int CreateTempRelroFile(char* path)
{
    int fd = mkstemp(path);
    if (fd != INVALID_VALUE) {
        close(fd);
    }
    return fd;
}

static void WriteRelro(dl_extinfo* extInfo, const char* libFileName, int fd, bool isRecursive)
{
    if (!extInfo) {
        return;
    }
    if (isRecursive) {
        extInfo->flag |= DL_EXT_RESERVED_ADDRESS_RECURSIVE;
    }

    pid_t pid = fork();
    if (pid == 0) {
        extInfo->flag |= DL_EXT_WRITE_RELRO;
        extInfo->relro_fd = fd;
        void* handle = dlopen_ext(libFileName, RTLD_NOW, extInfo);
        if (handle == nullptr) {
            printf("dlopen_ext fail in child process");
            exit(INVALID_VALUE);
        }
        FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetRelroValue"));
        if (fn != nullptr) {
            EXPECT_EQ(TEST_NUM_50, fn());
        } else {
            printf("dlsym(GetRelroValue) fail in child process");
            exit(INVALID_VALUE);
        }

        if (isRecursive) {
            fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetRelroRecursiveValue"));
            if (fn != nullptr) {
                EXPECT_EQ(TEST_NUM_100, fn());
            } else {
                printf("dlsym(GetRelroRecursiveValue) fail in child process");
                exit(INVALID_VALUE);
            }
        }

        int* testValue = reinterpret_cast<int*>(dlsym(handle, "g_testNumber"));
        if (testValue != nullptr) {
            EXPECT_EQ(TEST_NUM_1000, *testValue);
            exit(0);
        } else {
            printf("dlsym(g_testNumber) fail in child process");
            exit(INVALID_VALUE);
        }
    }
    ASSERT_NE(pid, INVALID_VALUE);
    int status = 0;
    int waitPid = waitpid(pid, &status, 0);
    ASSERT_EQ(pid, waitPid);
}

static void UseRelro(dl_extinfo* extInfo, const char* libFileName, int fd, bool isRecursive)
{
    extInfo->flag |= DL_EXT_USE_RELRO;
    extInfo->relro_fd = fd;

    void* handle = dlopen_ext(libFileName, RTLD_NOW, extInfo);
    ASSERT_NE(handle, nullptr);
    FuncTypeRetInt fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetRelroValue"));
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(TEST_NUM_50, fn());

    if (isRecursive) {
        fn = reinterpret_cast<FuncTypeRetInt>(dlsym(handle, "GetRelroRecursiveValue"));
        ASSERT_NE(fn, nullptr);
        EXPECT_EQ(TEST_NUM_100, fn());
    }

    int* testValue = reinterpret_cast<int*>(dlsym(handle, "g_testNumber"));
    ASSERT_NE(testValue, nullptr);
    EXPECT_EQ(TEST_NUM_1000, *testValue);

    dlclose(handle);
}

/**
 * @tc.name: dlopen_ext_008
 * @tc.desc: Test that set dl_extinfo's flag to DL_EXT_WRITE_RELRO | DL_EXT_USE_RELRO,
 *           then call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_008, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_1024_SQUARE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_1024_SQUARE;
    extInfo.relro_fd = INVALID_VALUE;

    char relroFile[] = RELRO_FILE_PATH;
    if (CreateTempRelroFile(relroFile) < 0) {
        return;
    }

    int fd = open(relroFile, O_RDWR | O_TRUNC | O_CLOEXEC);
    ASSERT_NE(fd, INVALID_VALUE);
    WriteRelro(&extInfo, "libldso_relro_test.so", fd, false);
    close(fd);

    fd = open(relroFile, O_RDONLY | O_CLOEXEC);
    ASSERT_NE(fd, INVALID_VALUE);
    UseRelro(&extInfo, "libldso_relro_test.so", fd, false);
    close(fd);

    unlink(relroFile);
    munmap(addr, SIZE_1024_SQUARE);
}

/**
 * @tc.name: dlopen_ext_009
 * @tc.desc: Test that set dl_extinfo's flag to DL_EXT_RESERVED_ADDRESS_RECURSIVE |
 *           DL_EXT_WRITE_RELRO | DL_EXT_USE_RELRO, then call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_009, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_1024_SQUARE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_1024_SQUARE;
    extInfo.relro_fd = INVALID_VALUE;

    char relroFile[] = RELRO_FILE_PATH;
    if (CreateTempRelroFile(relroFile) < 0) {
        return;
    }

    int fd = open(relroFile, O_RDWR | O_TRUNC | O_CLOEXEC);
    ASSERT_NE(fd, INVALID_VALUE);
    WriteRelro(&extInfo, "libldso_relro_recursive_test.so", fd, true);
    close(fd);

    fd = open(relroFile, O_RDONLY | O_CLOEXEC);
    ASSERT_NE(fd, INVALID_VALUE);
    UseRelro(&extInfo, "libldso_relro_recursive_test.so", fd, true);
    close(fd);

    unlink(relroFile);
    munmap(addr, SIZE_1024_SQUARE);
}

/**
 * @tc.name: dlopen_ext_010
 * @tc.desc: Test that set the library to norelro, and then call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_010, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_1024_SQUARE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_1024_SQUARE;
    extInfo.relro_fd = INVALID_VALUE;

    char relroFile[] = RELRO_FILE_PATH;
    if (CreateTempRelroFile(relroFile) < 0) {
        return;
    }

    int fd = open(relroFile, O_RDWR | O_TRUNC | O_CLOEXEC);
    ASSERT_NE(fd, INVALID_VALUE);
    WriteRelro(&extInfo, "libldso_norelro_test.so", fd, false);
    close(fd);

    fd = open(relroFile, O_RDONLY | O_CLOEXEC);
    ASSERT_NE(fd, INVALID_VALUE);
    UseRelro(&extInfo, "libldso_norelro_test.so", fd, false);
    close(fd);

    unlink(relroFile);
    munmap(addr, SIZE_1024_SQUARE);
}

/**
 * @tc.name: dlopen_ext_011
 * @tc.desc: Test that set dl_extinfo's flag to DL_EXT_RESERVED_ADDRESS_HINT,
 *           and set reserved_size to a small size, then call dlopen_ext.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlopenExtTest, dlopen_ext_011, TestSize.Level1)
{
    void* addr = mmap(nullptr, SIZE_128, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, INVALID_VALUE, 0);
    ASSERT_NE(addr, MAP_FAILED);

    dl_extinfo extInfo;
    extInfo.flag = DL_EXT_RESERVED_ADDRESS_HINT;
    extInfo.reserved_addr = addr;
    extInfo.reserved_size = SIZE_128;
    void* handle = dlopen_ext("libldso_relro_recursive_test.so", RTLD_NOW, &extInfo);
    ASSERT_NE(handle, nullptr);

    void* fn = dlsym(handle, "GetRelroValue");
    ASSERT_NE(fn, nullptr);
    EXPECT_TRUE((fn < addr) || (fn >= (reinterpret_cast<char*>(addr) + SIZE_128)));

    FuncTypeRetInt func = reinterpret_cast<FuncTypeRetInt>(fn);
    EXPECT_EQ(TEST_NUM_50, func());

    dlclose(handle);
    munmap(addr, SIZE_128);
}
