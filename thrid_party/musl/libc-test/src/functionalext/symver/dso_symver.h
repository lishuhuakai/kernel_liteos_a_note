/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>

#include "test.h"

extern const char *dso_symbol_invalid;
extern const char *dso_version_invalid;

extern const char *dso_no_symver_name;
extern const char *dso_no_symver_symbol;

extern const char *dso_easy_symver_name;
extern const char *dso_easy_symver_symbol;
extern const char *dso_easy_symver_symbol_old;
extern const char *dso_easy_symver_symbol_stable;
extern const char *dso_easy_symver_symbol_dev;
extern const char *dso_easy_symver_version_old;
extern const char *dso_easy_symver_version_stable;
extern const char *dso_easy_symver_version_dev;

extern const char *dso_hard_symver_name;
extern const char *dso_hard_symver_if_symbol;
extern const char *dso_hard_symver_if_symbol_old;
extern const char *dso_hard_symver_if_symbol_stable;
extern const char *dso_hard_symver_if_symbol_dev;
extern const char *dso_hard_symver_ld_symbol;
extern const char *dso_hard_symver_ld_symbol_old;
extern const char *dso_hard_symver_ld_symbol_stable;
extern const char *dso_hard_symver_ld_symbol_dev;
extern const char *dso_hard_symver_version_old;
extern const char *dso_hard_symver_version_stable;
extern const char *dso_hard_symver_version_dev;

typedef char *(*functype)(void);

#define SYMVER_DEBUG
#ifdef SYMVER_DEBUG

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define symver_log(fmt, ...)                                                                  \
    do {                                                                                      \
        printf("[%s:%4d][%s]" fmt "\n", __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

#else  // SYMVER_DEBUG

#define symver_log(fmt, ...)

#endif  // SYMVER_DEBUG

#define symver_error(fmt, ...)                                        \
    do {                                                              \
        t_error("%s failed: " fmt "\n", __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

#define symver_streq(s1, s2)                                                \
    do {                                                                    \
        if (strcmp(s1, s2)) {                                               \
            t_error("%s failed: s1 = %s, s2 = %s\n", __FUNCTION__, s1, s2); \
        }                                                                   \
    } while (0)
