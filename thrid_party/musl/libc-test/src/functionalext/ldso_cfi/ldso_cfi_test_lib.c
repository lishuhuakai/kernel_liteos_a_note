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

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t g_count = 0;
static uint64_t g_type_id = 0;
static void *g_address = NULL;
static void *g_diag = NULL;

// Make sure the library crosses at least one kLibraryAlignment(=256KB) boundary.
char buf[1024 * 1024];

// Mock a CFI-enabled library without relying on the compiler.
__attribute__((aligned(4096))) void __cfi_check(uint64_t call_site_type_id,
                                                void *target_addr, void *diag) {
    ++g_count;
    g_type_id = call_site_type_id;
    g_address = target_addr;
    g_diag = diag;
}

size_t get_count() {
    return g_count;
}

uint64_t get_type_id() {
    return g_type_id;
}

void *get_address() {
    return g_address;
}

void *get_diag() {
    return g_diag;
}

void *get_global_address() {
    return &g_count;
}
