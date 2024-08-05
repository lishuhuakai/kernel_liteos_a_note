/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/wait.h>
#include "functionalext.h"

#define LIB_PATH "/data/tests/libc-test/src/libldso_cfi_test_lib.so"

struct dso {
    char *mock;
    unsigned char *map;
    size_t map_len;
};

extern int init_cfi_shadow(struct dso *dso_list, struct dso *ldso);
extern int map_dso_to_cfi_shadow(struct dso *dso);
extern void unmap_dso_from_cfi_shadow(struct dso *dso);
extern void __cfi_slowpath(uint64_t CallSiteTypeId, void *Ptr);
extern void __cfi_slowpath_diag(uint64_t CallSiteTypeId, void *Ptr, void *DiagData);

typedef void(*TEST_FUN)(void);
typedef size_t (*GET_COUNT)();
typedef uint64_t (*GET_TP_ID)();
typedef void* (*GET_LAST_ADDR)();
typedef void* (*GET_LAST_DIAG)();
typedef void* (*GET_GL_ADDR)();
typedef void (*CFI_CHECK)();
typedef char* (*BUF_CHECK)();

GET_COUNT get_count = NULL;
GET_TP_ID get_type_id = NULL;
GET_LAST_ADDR get_address = NULL;
GET_LAST_DIAG get_diag = NULL;
GET_GL_ADDR get_global_address = NULL;
CFI_CHECK cfi_check = NULL;
BUF_CHECK buf_check = NULL;

static void test_func() {}

/**
 * @tc.name      : cfi_init_test_0001
 * @tc.desc      : If no dso while initializing the CFI shadow, do nothing and return true.
 * @tc.level     : Level 1
 */
void cfi_init_test_0001(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    EXPECT_EQ("cfi_init_test_0001", init_cfi_shadow(NULL, NULL), 0);
}

/**
 * @tc.name      : cfi_init_test_0002
 * @tc.desc      : If dso is NULL while mapping to the CFI shadow, do nothing and return true.
 * @tc.level     : Level 2
 */
void cfi_init_test_0002(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    EXPECT_EQ("cfi_init_test_0002", map_dso_to_cfi_shadow(NULL), 0);
}

/**
 * @tc.name      : cfi_init_test_0003
 * @tc.desc      : If dso is NULL while unmapping from the CFI shadow, do nothing.
 * @tc.level     : Level 2
 */
void cfi_init_test_0003(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    unmap_dso_from_cfi_shadow(NULL);
}

/**
 * @tc.name      : cfi_slowpath_function_test_0002
 * @tc.desc      : Loading a dso that contains __cfi_check() symbol, call the __cfi_slowpath() function with 
 *                 address inside the DSO, the __cfi_check() function is called.
 * @tc.level     : Level 1
 */
void cfi_slowpath_function_test_0002(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t expected_call_site_type_id = 20;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_function_test_0002", handle, NULL);

    *(void **)(&get_count) = dlsym(handle, "get_count");
    *(void **)(&get_type_id) = dlsym(handle, "get_type_id");
    *(void **)(&get_address) = dlsym(handle, "get_address");
    *(void **)(&get_diag) = dlsym(handle, "get_diag");
    *(void **)(&get_global_address) = dlsym(handle, "get_global_address");

    size_t count = (*get_count)();

    __cfi_slowpath(expected_call_site_type_id, (*get_global_address)());
    EXPECT_EQ("cfi_slowpath_function_test_0002", expected_call_site_type_id, (*get_type_id)());
    EXPECT_EQ("cfi_slowpath_function_test_0002", (*get_global_address)(), (*get_address)());
    EXPECT_EQ("cfi_slowpath_function_test_0002", NULL, (*get_diag)());
    EXPECT_EQ("cfi_slowpath_function_test_0002", ++count, (*get_count)());

    dlclose(handle);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_slowpath_function_test_0003
 * @tc.desc      : Loading a dso that contains __cfi_check() symbol, call the __cfi_slowpath() function with 
 *                 address belongs to other dso that does not enable Cross-DSO, the __cfi_check() function is 
 *                 not called.
 * @tc.level     : Level 1
 */
void cfi_slowpath_function_test_0003(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t expected_call_site_type_id = 30;
    uint64_t unexpected_call_site_type_id = 40;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_function_test_0003", handle, NULL);

    *(void **)(&get_count) = dlsym(handle, "get_count");
    *(void **)(&get_type_id) = dlsym(handle, "get_type_id");
    *(void **)(&get_address) = dlsym(handle, "get_address");
    *(void **)(&get_diag) = dlsym(handle, "get_diag");
    *(void **)(&get_global_address) = dlsym(handle, "get_global_address");

    __cfi_slowpath(expected_call_site_type_id, (*get_global_address)());
    size_t count = (*get_count)();
    __cfi_slowpath(unexpected_call_site_type_id, (void*)(&test_func));
    EXPECT_EQ("cfi_slowpath_function_test_0003", expected_call_site_type_id, (*get_type_id)());
    EXPECT_EQ("cfi_slowpath_function_test_0003", (*get_global_address)(), (*get_address)());
    EXPECT_EQ("cfi_slowpath_function_test_0003", NULL, (*get_diag)());
    EXPECT_EQ("cfi_slowpath_function_test_0003", count, (*get_count)());

    dlclose(handle);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_slowpath_function_test_0004
 * @tc.desc      : Calling __cfi_slowpath() with target_addr = NULL
 * @tc.level     : Level 2
 */
void cfi_slowpath_function_test_0004(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t call_site_type_id = 30;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_function_test_0004", handle, NULL);

    *(void **)(&get_count) = dlsym(handle, "get_count");

    size_t count = (*get_count)();
    __cfi_slowpath(call_site_type_id, NULL);
    EXPECT_EQ("cfi_slowpath_function_test_0004", count, (*get_count)());
    
    dlclose(handle);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_slowpath_function_test_0005
 * @tc.desc      : Loading a dso that contains __cfi_check() symbol, call the __cfi_slowpath() function with 
 *                 invalid address, coredump happened.
 * @tc.level     : Level 2
 */
void cfi_slowpath_function_test_0005(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t call_site_type_id = 30;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_function_test_0005", handle, NULL);

    *(void **)(&get_count) = dlsym(handle, "get_count");
    *(void **)(&get_type_id) = dlsym(handle, "get_type_id");
    *(void **)(&get_address) = dlsym(handle, "get_address");
    *(void **)(&get_diag) = dlsym(handle, "get_diag");
    *(void **)(&get_global_address) = dlsym(handle, "get_global_address");

    size_t count = (*get_count)();
    int status;
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        printf("["__FILE__"][Line: %d][%s]: parent process pid = %d\n", __LINE__, __func__, getppid());
        wait(&status);
    } else if (pid == 0) {
        printf("["__FILE__"][Line: %d][%s]: child pid = %d\n", __LINE__, __func__, pid);
        __cfi_slowpath(call_site_type_id, (void*)&count);
    } else {
        printf("["__FILE__"][Line: %d][%s]: fork failed!\n", __LINE__, __func__);
    }
    dlclose(handle);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_slowpath_function_test_0006
 * @tc.desc      : Loading a dso which size is larger than 1 LIBRARY_ALIGNMENT(256KB), call the __cfi_slowpath() 
 *                 function with address in different LIBRARY_ALIGNMENT range and make sure __cfi_check() is called.
 * @tc.level     : Level 1
 */
void cfi_slowpath_function_test_0006(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t call_site_type_id = 50;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_function_test_0006", handle, NULL);

    *(void **)(&get_count) = dlsym(handle, "get_count");
    *(void **)(&buf_check) = dlsym(handle, "buf");

    size_t count = (*get_count)();
    const size_t bss_size = 1024 * 1024;

    for (size_t i = 0; i < bss_size; ++i) {
        __cfi_slowpath(call_site_type_id, (char*)buf_check + i);
        EXPECT_EQ("cfi_slowpath_function_test_0006", ++count, (*get_count)());
    }

    dlclose(handle);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_slowpath_function_test_0007
 * @tc.desc      : Loading a dso 2 times
 * @tc.level     : Level 1
 */
void cfi_slowpath_function_test_0007(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t call_site_type_id1 = 10;
    uint64_t call_site_type_id2 = 20;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_function_test_0007", handle, NULL);

    *(void **)(&get_count) = dlsym(handle, "get_count");
    *(void **)(&get_global_address) = dlsym(handle, "get_global_address");
    __cfi_slowpath(call_site_type_id1, (*get_global_address)());

    void* handle2 = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTREQ("cfi_slowpath_function_test_0007", handle, handle2);
    *(void **)(&get_count) = dlsym(handle2, "get_count");
    *(void **)(&get_type_id) = dlsym(handle2, "get_type_id");
    *(void **)(&get_address) = dlsym(handle2, "get_address");
    *(void **)(&get_diag) = dlsym(handle2, "get_diag");
    *(void **)(&get_global_address) = dlsym(handle2, "get_global_address");

    size_t count = (*get_count)();
    __cfi_slowpath(call_site_type_id2, (*get_global_address)());
    EXPECT_EQ("cfi_slowpath_function_test_0007", call_site_type_id2, (*get_type_id)());
    EXPECT_EQ("cfi_slowpath_function_test_0007", (*get_global_address)(), (*get_address)());
    EXPECT_EQ("cfi_slowpath_function_test_0007", ++count, (*get_count)());

    dlclose(handle);
    dlclose(handle2);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_slowpath_function_test_0008
 * @tc.desc      : Calling dso's function after unloading the dso.
 * @tc.level     : Level 1
 */
void cfi_slowpath_function_test_0008(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t call_site_type_id = 30;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_function_test_0008", handle, NULL);
    *(void **)(&get_global_address) = dlsym(handle, "get_global_address");
    dlclose(handle);

    int status;
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        printf("["__FILE__"][Line: %d][%s]: parent process pid = %d\n", __LINE__, __func__, getppid());
        wait(&status);
    } else if (pid == 0) {
        printf("["__FILE__"][Line: %d][%s]: child pid = %d\n", __LINE__, __func__, pid);
        __cfi_slowpath(call_site_type_id, (*get_global_address)());
    } else {
        printf("["__FILE__"][Line: %d][%s]: fork failed!\n", __LINE__, __func__);
    }
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_slowpath_diag_function_test_0001
 * @tc.desc      : Loading a dso that contains __cfi_check() symbol, call the __cfi_slowpath_diag() function with 
 *                 address inside the DSO, the __cfi_check() function is called.
 * @tc.level     : Level 1
 */
void cfi_slowpath_diag_function_test_0001(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    uint64_t call_site_type_id = 10;
    uint64_t diag_info = 0;
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    EXPECT_PTRNE("cfi_slowpath_diag_function_test_0001", handle, NULL);

    *(void **)(&get_count) = dlsym(handle, "get_count");
    *(void **)(&get_type_id) = dlsym(handle, "get_type_id");
    *(void **)(&get_diag) = dlsym(handle, "get_diag");
    *(void **)(&get_global_address) = dlsym(handle, "get_global_address");

    size_t count = (*get_count)();

    __cfi_slowpath_diag(call_site_type_id, NULL, &diag_info);
    EXPECT_EQ("cfi_slowpath_diag_function_test_0001", count, (*get_count)());

    __cfi_slowpath_diag(call_site_type_id, (*get_global_address)(), &diag_info);
    EXPECT_EQ("cfi_slowpath_diag_function_test_0001", ++count, (*get_count)());
    EXPECT_EQ("cfi_slowpath_diag_function_test_0001", call_site_type_id, (*get_type_id)());
    EXPECT_EQ("cfi_slowpath_diag_function_test_0001", (*get_diag)(), &diag_info);

    dlclose(handle);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_unmap_dso_from_cfi_shadow_001
 * @tc.desc      : If dso map is NULL while unmapping from the CFI shadow, do nothing.
 * @tc.level     : Level 2
 */
void cfi_unmap_dso_from_cfi_shadow_001(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    struct dso test_dso = {};
    test_dso.map = 0;
    test_dso.map_len = 1;
    unmap_dso_from_cfi_shadow(&test_dso);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : cfi_unmap_dso_from_cfi_shadow_002
 * @tc.desc      : If dso map_len is NULL while unmapping from the CFI shadow, do nothing.
 * @tc.level     : Level 2
 */
void cfi_unmap_dso_from_cfi_shadow_002(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    struct dso test_dso = {};
    unsigned char a = 'S';
    test_dso.map = &a;
    test_dso.map_len = 0;
    unmap_dso_from_cfi_shadow(&test_dso);
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

TEST_FUN G_Fun_Array[] = {
    cfi_init_test_0001,
    cfi_init_test_0002,
    cfi_init_test_0003,
    cfi_slowpath_function_test_0002,
    cfi_slowpath_function_test_0003,
    cfi_slowpath_function_test_0004,
    cfi_slowpath_function_test_0005,
    cfi_slowpath_function_test_0006,
    cfi_slowpath_function_test_0007,
    cfi_slowpath_function_test_0008,
    cfi_slowpath_diag_function_test_0001,
    cfi_unmap_dso_from_cfi_shadow_001,
    cfi_unmap_dso_from_cfi_shadow_002,
};

int main(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
    return 0;
}
