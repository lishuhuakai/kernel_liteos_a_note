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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include "test.h"

void vmsplice_0100(void)
{
    int pipe_size = 2;
    int iov_length = 6;
    int pipe_fds[pipe_size];
    if (pipe(pipe_fds) != 0) {
        t_error("%s create pipe error\n", __func__);
    }
    struct iovec v[pipe_size];
    char *str_hello = "hello ";
    char *str_world = "world\n";
    v[0].iov_base = str_hello;
    v[0].iov_len = iov_length;
    v[1].iov_base = str_world;
    v[1].iov_len = iov_length;
    size_t result = vmsplice(pipe_fds[1], v, sizeof(v) / sizeof(struct iovec), 0);
    if (result != v[0].iov_len + v[1].iov_len) {
        t_error("%s vmsplice error get result is %d are not want %d\n", __func__, result, v[0].iov_len + v[1].iov_len);
    }
    close(pipe_fds[1]);
    char buf[BUFSIZ];
    FILE *fp = fdopen(pipe_fds[0], "r");
    if (!fp) {
        t_error("%s fdopen get ptr is NULL\n", __func__);
    }
    if (!fgets(buf, sizeof(buf), fp)) {
        t_error("%s fgets get ptr is NULL\n", __func__);
    }
    fclose(fp);
    if (strcmp(buf, "hello world\n") != 0) {
        t_error("%s fgets get str is '%s' are not want 'hello world'\n", __func__, buf);
    }
}

int main(int argc, char *argv[])
{
    vmsplice_0100();
    return t_status;
}