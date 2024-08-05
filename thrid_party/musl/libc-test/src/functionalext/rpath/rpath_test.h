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

#ifndef LIBC_TEST_SRC_FUNCTIONALEXT_RPATH_TEST_H
#define LIBC_TEST_SRC_FUNCTIONALEXT_RPATH_TEST_H

static const char* g_absoluteLib = "libprimary_absolute.so";
static const char* g_originLib = "libprimary_origin.so";
static const char* g_multipleLib = "libprimary_multiple.so";

static const char* g_dlName = "dlns_rpath_test";
static const char* g_dlpath = "/data/tests/libc-test/src/rpath_lib:/system/lib:/system/lib64";
static const char* g_path = "/data/tests/libc-test/src";
static const char* g_errPath = "/data/tests/libc-test/src/rpath_lib/rpath_support_B";

typedef void(*TEST_FUN)(void);
#endif