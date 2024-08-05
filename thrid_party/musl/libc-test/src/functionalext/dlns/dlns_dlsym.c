/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "dlns_test.h"
#include "functionalext.h"

const char *LIB_NAME_A = "libdlns_dlsym_dep_a.so";
const char *LIB_NAME_B = "libdlns_dlsym_dep_b.so";
const char *LIB_NAME_C = "libdlns_dlsym_dep_c.so";

static inline void dlns_dlsym_0100_init_ns(Dl_namespace *dlnsA, Dl_namespace *dlnsB, Dl_namespace *dlnsC)
{
    dlns_init(dlnsA, "dlns_dlsym_0100_A");
    dlns_init(dlnsB, "dlns_dlsym_0100_B");
    dlns_init(dlnsC, "dlns_dlsym_0100_C");
    EXPECT_EQ(__FUNCTION__, dlns_create(dlnsA, pathA), EOK);
    EXPECT_EQ(__FUNCTION__, dlns_create(dlnsB, pathB), EOK);
    EXPECT_EQ(__FUNCTION__, dlns_create(dlnsC, pathC), EOK);
    EXPECT_EQ(__FUNCTION__, dlns_inherit(dlnsB, dlnsC, NULL), EOK);
    EXPECT_EQ(__FUNCTION__, dlns_inherit(dlnsA, dlnsB, NULL), EOK);
}

static inline void close_so(void* handle)
{
    if (handle) {
        dlclose(handle);
    }
}

/**
 * @tc.name      : dlns_dlsym_0100
 * @tc.desc      : Call the dlsym interface to load libA,libB,libC dynamic library.
 * @tc.level     : Level1
 */
static void dlns_dlsym_0100(void)
{
    Dl_namespace dlnsA, dlnsB, dlnsC;
    dlns_dlsym_0100_init_ns(&dlnsA, &dlnsB, &dlnsC);

    void* handle_c = dlopen_ns(&dlnsC, LIB_NAME_C, RTLD_NOW);
    EXPECT_PTRNE(__FUNCTION__, handle_c, NULL);
    if (handle_c == NULL) {
        return;
    }

    void* handle_b = dlopen_ns(&dlnsB, LIB_NAME_B, RTLD_NOW);
    EXPECT_PTRNE(__FUNCTION__, handle_b, NULL);
    if (handle_b == NULL) {
        close_so(handle_c);
        return;
    }

    void* handle_a = dlopen_ns(&dlnsA, LIB_NAME_A, RTLD_NOW);
    EXPECT_PTRNE(__FUNCTION__, handle_a, NULL);
    if (handle_a == NULL) {
        close_so(handle_b);
        close_so(handle_c);
        return;
    }

    int(* test_a)(void) = dlsym(handle_a, "test");
    EXPECT_PTRNE(__FUNCTION__, test_a, NULL);
    if (test_a) {
        EXPECT_EQ(__FUNCTION__, test_result_1, test_a());
    }

    int(* test_b)(void) = dlsym(handle_a, "test_b");
    EXPECT_PTRNE(__FUNCTION__, test_b, NULL);
    if (test_b) {
        EXPECT_EQ(__FUNCTION__, test_result_2, test_b());
    }

    int(* test_c)(void) = dlsym(handle_c, "test");
    EXPECT_PTRNE(__FUNCTION__, test_c, NULL);
    if (test_c) {
        EXPECT_EQ(__FUNCTION__, test_result_3, test_c());
    }

    int(* test_d)(void) = dlsym(handle_c, "test_c");
    EXPECT_PTRNE(__FUNCTION__, test_d, NULL);
    if (test_d) {
        EXPECT_EQ(__FUNCTION__, test_result_4, test_d());
    }

    int(* test_e)(void) = dlsym(handle_a, "test_c");
    EXPECT_PTREQ(__FUNCTION__, test_e, NULL);

    close_so(handle_c);
    close_so(handle_b);
    close_so(handle_a);
}

TEST_FUN G_Fun_Array[] = {
    dlns_dlsym_0100,
};

int main(void)
{
    int num = sizeof(G_Fun_Array)/sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}