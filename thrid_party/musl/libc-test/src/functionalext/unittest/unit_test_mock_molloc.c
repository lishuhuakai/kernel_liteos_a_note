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

#include <stddef.h>

static char buf[80] = "abcde"; 

void *__libc_malloc(size_t n)
{
    return (void*)buf;
}

void __libc_free(void *p)
{}

void *__libc_calloc(size_t m, size_t n)
{
    return (void*)buf;
}

void *__libc_realloc(void* p, size_t n)
{
    buf[n+1] = '\0';
    return (void*)buf;
}