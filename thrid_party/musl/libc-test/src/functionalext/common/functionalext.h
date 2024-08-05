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

#ifndef __FUNCTIONALEXT_H__
#define __FUNCTIONALEXT_H__

#include <dlfcn.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

#define EPS (0.00001)
#define CMPFLAG 0
#define ERREXPECT (-1)
#define ONREXPECT 1

#define EXPECT_TRUE(fun, c)                \
    do {                                   \
        if (!(c))                          \
            t_error("[%s] failed\n", fun); \
    } while (0)

#define EXPECT_FALSE(fun, c)                \
    do {                                    \
        if ((c))                            \
            t_error("[%s] failed \n", fun); \
    } while (0)

#define EXPECT_EQ(fun, a, b)                               \
    do {                                                   \
        if ((a) != (b))                                    \
            t_error("[%s] failed %d != %d \n", fun, a, b); \
    } while (0)

#define EXPECT_LT(fun, a, b)                                                              \
    do {                                                                                  \
        if ((a) >= (b))                                                                   \
            t_error("[%s] failed (errno: %s)  %d >= %d \n", #fun, strerror(errno), a, b); \
    } while (0)

#define EXPECT_MT(fun, a, b)                                                              \
    do {                                                                                  \
        if ((a) <= (b))                                                                   \
            t_error("[%s] failed (errno: %s)  %d >= %d \n", #fun, strerror(errno), a, b); \
    } while (0)

#define EXPECT_NE(fun, a, b)                                   \
    do {                                                       \
        if ((int)(a) == (int)(b))                              \
            t_error("[%s] failed %d == %d \n", fun, (a), (b)); \
    } while (0)

/* char*, char[] comparison */
#define EXPECT_STREQ(fun, a, b)                                \
    do {                                                       \
        if (strlen(a) != strlen(b) || strcmp((a), (b)) != 0)   \
            t_error("[%s] failed %s != %s \n", fun, (a), (b)); \
    } while (0)

#define EXPECT_STRLT(fun, a, b)                                                           \
    do {                                                                                  \
        if ((a) >= (b))                                                                   \
            t_error("[%s] failed (errno: %s)  %d >= %d \n", #fun, strerror(errno), a, b); \
    } while (0)

#define EXPECT_STRMT(fun, a, b)                                                           \
    do {                                                                                  \
        if ((a) <= (b))                                                                   \
            t_error("[%s] failed (errno: %s)  %d >= %d \n", #fun, strerror(errno), a, b); \
    } while (0)

/* floating point comparison */
#define FLOAT_EQUAL(fun, a, b)                                   \
    do {                                                         \
        if (!(fabs((a) - (b)) <= EPS))                           \
            t_error("[%s] failed %fd != %fd \n", fun, (a), (b)); \
    } while (0)

#define EXPECT_PTREQ(fun, a, b)                            \
    do {                                                   \
        if ((a) != (b)) {                                  \
            t_error("[%s] failed %p != %p \n", fun, a, b); \
        }                                                  \
    } while (0)

#define EXPECT_PTRNE(fun, a, b)                            \
    do {                                                   \
        if ((a) == (b)) {                                  \
            t_error("[%s] failed %p == %p \n", fun, a, b); \
        }                                                  \
    } while (0)

#define EXPECT_STRNE(fun, a, b)                                \
    do {                                                       \
        if (strcmp((a), (b)) == 0)                             \
            t_error("[%s] failed %s == %s \n", fun, (a), (b)); \
    } while (0)

#define EXPECT_LONGEQ(fun, a, b)                             \
    do {                                                     \
        if ((long)(a) != (long)(b))                          \
            t_error("[%s] failed %ld != %ld \n", fun, a, b); \
    } while (0)

#define EXPECT_LONGLONGEQ(fun, a, b)                           \
    do {                                                       \
        if ((long)(a) != (long)(b))                            \
            t_error("[%s] failed %lld != %lld \n", fun, a, b); \
    } while (0)

#define EXPECT_GT(fun, a, b)                               \
    do {                                                   \
        if ((a) <= (b)) {                                  \
            t_error("[%s] failed %d <= %d \n", fun, a, b); \
        }                                                  \
    } while (0)

#define EXPECT_GTE(fun, a, b)                             \
    do {                                                  \
        if ((a) < (b)) {                                  \
            t_error("[%s] failed %d < %d \n", fun, a, b); \
        }                                                 \
    } while (0)

#endif
