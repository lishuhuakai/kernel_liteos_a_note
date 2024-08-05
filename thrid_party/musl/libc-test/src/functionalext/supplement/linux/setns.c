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

#include <errno.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "test.h"

/**
 * @tc.name      : setns_0100
 * @tc.desc      : reassociate thread with a namespace
 * @tc.level     : Level 0
 */
void setns_0100(void)
{
    pid_t pid = getpid();
    if (pid < 0) {
        t_error("%s failed: pid = %d\n", __func__, pid);
    }

    int fd = syscall(SYS_pidfd_open, pid, 0);
    if (fd < 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
    }

    errno = 0;
    int result = setns(fd, 0);
    if (result != -1 || errno != EINVAL) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : setns_0200
 * @tc.desc      : reassociate an invliad fd with a namespace
 * @tc.level     : Level 2
 */
void setns_0200(void)
{
    int fd = -1;
    int nstype = 0;

    errno = 0;
    int result = setns(fd, nstype);
    if (result == 0 || errno == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    setns_0100();
    setns_0200();

    return t_status;
}
