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
// #define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include "test.h"

const char *pathname = "/proc/self/mountinfo";

/**
 * @tc.name      : name_to_handle_at_0100
 * @tc.desc      : Verify that the name_TO_HANDLE_AT function was successfully called (all parameters are valid)
 * @tc.level     : Level 0
 */
void name_to_handle_at_0100(void)
{
    struct file_handle *fhp;
    char buf[1000];
    int mount_id, fhsize, flags, dirfd;

    fhsize = sizeof(*fhp);
    fhp = malloc(fhsize);
    if (fhp == NULL) {
        t_error("%s malloc failed\n", __func__);
    }

    dirfd = AT_FDCWD;
    flags = 0;
    fhp->handle_bytes = 0;

    if (name_to_handle_at(dirfd, pathname, fhp, &mount_id, flags) != -1 || errno != EOVERFLOW) {
        t_error("%s Unexpected name_to_handle_at\n", __func__);
    }

    fhsize = sizeof(*fhp) + fhp->handle_bytes;
    fhp = realloc(fhp, fhsize);
    if (fhp == NULL) {
        t_error("%s realloc failed\n", __func__);
    }

    int result = name_to_handle_at(dirfd, pathname, fhp, &mount_id, flags);
    if (result == -1) {
        t_error("%s name_to_handle_at failed\n");
    }

    int fd = open_by_handle_at(mount_id, fhp, O_RDONLY);
    if (fd == -1) {
        t_error("%s open_by_handle_at failed\n", __func__);
    }

    int nread = read(fd, buf, sizeof(buf));
    if (nread == -1) {
        t_error("%s read failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    // name_to_handle_at_0100();
    return t_status;
}
