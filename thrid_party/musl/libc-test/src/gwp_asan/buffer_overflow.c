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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "gwp_asan.h"
#include "test.h"

void buffer_overflow_handler()
{
    find_and_check_file(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG, "Buffer Overflow");
    clear_log(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG);
    _exit(0);
}

static void install_sigv_handler()
{
    struct sigaction sigv = {
        .sa_handler = buffer_overflow_handler,
        .sa_flags = 0,
    };
    sigaction(SIGSEGV, &sigv, NULL);
}

/*
 *
 *|--guard area--|--malloc area--|--guard area--|
 *                                ^
 */
int main()
{
    clear_log(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG);
    install_sigv_handler();
    char *ptr = (char *)malloc(sysconf(_SC_PAGE_SIZE));
    if (!ptr) {
        return 0;
    }
    char c = *(ptr + sysconf(_SC_PAGE_SIZE) + 1);
    printf("c:%c", c);
    return 0;
}
