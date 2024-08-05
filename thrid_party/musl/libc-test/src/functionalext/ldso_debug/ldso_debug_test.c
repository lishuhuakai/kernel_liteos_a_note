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

#include "functionalext.h"

#define LIB_PATH_1 "/data/tests/libc-test/src/libldso_debug_test_lib_1.so"
#define LIB_PATH_2 "/data/tests/libc-test/src/libldso_debug_test_lib_2.so"
#define LIB_PATH_3 "/data/tests/libc-test/src/libldso_debug_test_lib_3.so"
#define LIB_PATH_4 "/data/tests/libc-test/src/libldso_debug_test_lib_4.so"
#define LIB_PATH_5 "/data/tests/libc-test/src/libldso_debug_test_lib_5.so"
#define LIB_PATH_6 "/data/tests/libc-test/src/libldso_debug_test_lib_6.so"
#define LIB_PATH_BAK "/data/tests/libc-test/src/libldso_debug_test_lib_bak.so"

#define TEST_LIB_LOADING_TIME 20

typedef void(*TEST_FUN)(void);

/**
 * @tc.name      : ldso_debug_test_0001
 * @tc.desc      : Loading three dsos and unload them not in order.
 * @tc.level     : Level 0
 */
void ldso_debug_test_0001(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);

    void* handle_lib1 = dlopen(LIB_PATH_1, RTLD_NOW);
    EXPECT_PTRNE("ldso_debug_test_0001", handle_lib1, NULL);
    void* handle_lib2 = dlopen(LIB_PATH_2, RTLD_NOW);
    EXPECT_PTRNE("ldso_debug_test_0001", handle_lib2, NULL);
    void* handle_lib3 = dlopen(LIB_PATH_3, RTLD_NOW);
    EXPECT_PTRNE("ldso_debug_test_0001", handle_lib3, NULL);

    EXPECT_EQ("ldso_debug_test_0001", dlclose(handle_lib1), 0);
    EXPECT_EQ("ldso_debug_test_0001", dlclose(handle_lib3), 0);
    EXPECT_EQ("ldso_debug_test_0001", dlclose(handle_lib2), 0);

    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : ldso_debug_test_0002
 * @tc.desc      : Loading one dso and reload it for 20 times.
 * @tc.level     : Level 0
 */
void ldso_debug_test_0002(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);

    void* handle_lib1 = dlopen(LIB_PATH_1, RTLD_NOW);
    EXPECT_PTRNE("ldso_debug_test_0002", handle_lib1, NULL);

    for (int i = 0; i < TEST_LIB_LOADING_TIME; i++) {
        handle_lib1 = dlopen(LIB_PATH_1, RTLD_NOW);
        EXPECT_PTRNE("ldso_debug_test_0002", handle_lib1, NULL);
    }

    EXPECT_EQ("ldso_debug_test_0002", dlclose(handle_lib1), 0);

    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : ldso_debug_test_0003
 * @tc.desc      : Loading a dso which depends on non-existing dso.
 * @tc.level     : Level 0
 */
void ldso_debug_test_0003(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);

    EXPECT_EQ("ldso_debug_test_0003", rename(LIB_PATH_5, LIB_PATH_BAK), 0);

    void* handle_lib4 = dlopen(LIB_PATH_4, RTLD_NOW);
    EXPECT_PTREQ("ldso_debug_test_0003", handle_lib4, NULL);

    EXPECT_EQ("ldso_debug_test_0003", rename(LIB_PATH_BAK, LIB_PATH_5), 0);

    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

/**
 * @tc.name      : ldso_debug_test_0004
 * @tc.desc      : Loading a dso which is not exist.
 * @tc.level     : Level 0
 */
void ldso_debug_test_0004(void)
{
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);

    void* handle_lib6 = dlopen(LIB_PATH_6, RTLD_NOW);
    EXPECT_PTREQ("ldso_debug_test_0004", handle_lib6, NULL);

    printf("["__FILE__"][Line: %d][%s]: end\n", __LINE__, __func__);
}

TEST_FUN G_Fun_Array[] = {
    ldso_debug_test_0001,
    ldso_debug_test_0002,
    ldso_debug_test_0003,
    ldso_debug_test_0004,
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