/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef LIBC_TEST_SRC_FUNCTIONALEXT_DLNS_DLNS_TEST_H
#define LIBC_TEST_SRC_FUNCTIONALEXT_DLNS_DLNS_TEST_H

static const char* path = "/data/tests/libc-test/src";
static const char* dllName = "libdlopen_ns_dso.so";
static const char* dllNamePath = "/data/tests/libc-test/src/libdlopen_ns_dso.so";
static const char* errdllNamePath = "/etc/test/libdlopen_ns_dso.so";
static const char* dllName2 = "libdlopen_dso.so";
static const char* dllNamePath2 = "/data/tests/libc-test/src/libdlopen_dso.so";
static const char* errPath_ns = "src/test";

static const char* sharedLib = "sharedlibtest.so";
static const char* pathA = "/data/tests/libc-test/src/A";
static const char* pathB = "/data/tests/libc-test/src/B";
static const char* pathC = "/data/tests/libc-test/src/C";
static const char* pathD = "/data/tests/libc-test/src/D";

static const char* libB = "libB.so";
static const char* libC = "libC.so";
static const char* libD = "libD.so";

static const char* dllName_sep_009 = "separated_0900.so";
static const char* dllName_set_002 = "set_lib_path_0200.so";
static const char* dllName_inh_003 = "inherit_0300.so";
static const char* dllName_inh_007 = "inherit_0700.so";
static const char* dllName_inh_008 = "inherit_0800.so";
static const char* dllName_inh_011 = "inherit_1100.so";

#if defined(MUSL_ARM)
static const char* dllAcePath = "/system/lib/platformsdk/libace.z.so";
static const char* dllDylibPath = "/system/lib/libstd.dylib.so";
#else
static const char* dllAcePath = "/system/lib64/platformsdk/libace.z.so";
static const char* dllDylibPath = "/system/lib64/libstd.dylib.so";
#endif

static const char* dllHashsysvPath = "/data/tests/libc-test/src/libdlopen_hash_sysv.so";
static const char* dllHashsysv = "libdlopen_hash_sysv.so";
static const char* dllFillRandom = "/data/tests/libc-test/src/libdlopen_fill_random.so";

typedef void(*TEST_FUN)(void);

static const int EOK = 0;

static const int test_result_1 = 1;
static const int test_result_2 = 2;
static const int test_result_3 = 3;
static const int test_result_4 = 4;
#endif