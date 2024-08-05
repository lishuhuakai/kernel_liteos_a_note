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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gwp_asan.h"
#include "test.h"

#define TEST_NAME "gwp_asan_unwind_test"

__attribute__((noinline)) int func1(size_t *frame_buf, size_t max_record_stack, size_t *nums_frame)
{
    int res = 1;
    res += res;
    *nums_frame = libc_gwp_asan_unwind_fast(frame_buf, max_record_stack, NULL);
    return res;
}

__attribute__((noinline)) int func2(size_t *frame_buf, size_t max_record_stack, size_t *nums_frame)
{
    int res = 2;
    res += func1(frame_buf, max_record_stack, nums_frame);
    return res;
}

__attribute__((noinline)) int func3(size_t *frame_buf, size_t max_record_stack, size_t *nums_frame)
{
    int res = 3;
    res += func2(frame_buf, max_record_stack, nums_frame);
    return res;
}

int main()
{
    size_t frame_buf[256];
    size_t max_record_stack = 10;
    size_t nums_frames = 0;
    size_t expect_ret = 7;
    size_t min_stack_num = 3;
    int res = func3((size_t *)frame_buf, max_record_stack, &nums_frames);
    if (res != expect_ret || nums_frames < min_stack_num) {
        t_error("FAIL:gwp_asan unwind test return is incorrect nums_frames:%d res:%d!\n",
                nums_frames, res);
    }
    for (size_t i = 0; i < max_record_stack; i++) {
        if (frame_buf[i]) {
            Dl_info info;
            if (dladdr((void *)frame_buf[i], &info)) {
                size_t offset = frame_buf[i] - (uintptr_t)info.dli_fbase;
                if (i < min_stack_num && !strstr(info.dli_fname, TEST_NAME)) {
                    t_error("FAIL:gwp_asan unwind file name is incorrect expect %s but get %s!\n",
                            TEST_NAME, info.dli_fname);
                }
            } else {
                t_error("FAIL:gwp_asan unwind failed, dladdr %p return 0!\n", (void*)frame_buf[i]);
            }
        }
    }
}
