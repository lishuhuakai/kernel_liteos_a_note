/*
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

#include <fcntl.h>
#include <stdio.h>
#include <sys/vfs.h>
#include "test.h"

/**
 * @tc.name      : fstatfs_0100
 * @tc.desc      : Validation failed to get file status (parameter invalid file does not exist)
 * @tc.level     : Level 0
 */
void fstatfs_0100(void)
{
    struct statfs st;
    int fd = open("/proc", O_RDONLY);
    if (fd < 0) {
        t_error("%s open failed\n", __func__);
    }

    int result = fstatfs(fd, &st);
    if (result != 0) {
        t_error("%s fstatfs failed, result is %d\n", __func__, result);
    }

    close(fd);

    if ((int)st.f_bsize != 4096) {
        t_error("%s st.f_bsize invalid\n", __func__);
    }

    if (st.f_bfree != 0U) {
        t_error("%s st.f_bfree invalid\n"), __func__;
    }

    if (st.f_ffree != 0U) {
        t_error("%s st.f_ffree invalid\n"), __func__;
    }

    if (st.f_fsid.__val[0] != 0) {
        t_error("%s st.f_fsid.__val[0] invalid\n"), __func__;
    }

    if (st.f_fsid.__val[1] != 0) {
        t_error("%s st.f_fsid.__val[1] invalid\n"), __func__;
    }

    if ((int)st.f_namelen != 255) {
        t_error("%s st.f_namelen invalid\n"), __func__;
    }
}

/**
 * @tc.name      : fstatfs_0200
 * @tc.desc      : Validation failed to get file status (parameter invalid file does not exist)
 * @tc.level     : Level 2
 */
void fstatfs_0200(void)
{
    struct statfs st;
    int result = fstatfs(-1, &st);
    if (result != -1) {
        t_error("%s fstatfs should be failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    fstatfs_0100();
    fstatfs_0200();
    return t_status;
}