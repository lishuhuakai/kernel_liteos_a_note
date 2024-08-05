/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
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

#define _BSD_SOURCE
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"

int __fstat(int fd, struct stat *st)
{
    if (fd < 0) {
        return __syscall_ret(-EBADF);
    }
    return fstatat(fd, "", st, AT_EMPTY_PATH);
}
weak_alias(__fstat, fstat);
#if !_REDIR_TIME64
weak_alias(__fstat, fstat64);
#endif
