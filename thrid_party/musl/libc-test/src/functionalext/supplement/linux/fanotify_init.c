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
#include <fcntl.h>
#include <sys/fanotify.h>
#include "test.h"

/**
 * @tc.name      : fanotify_init_0100
 * @tc.desc      : Initializes a new fanotify group and returns a file descriptor for the event queue associated with
 *                 the group.
 * @tc.level     : Level 0
 */
void fanotify_init_0100(void)
{
    errno = 0;

    int fd = fanotify_init(0, 0);
    if (fd < 0) {
        switch (errno) {
            case ENOSYS:
                t_error("%s SKIP: missing support for fanotify (check CONFIG_FANOTIFY=y)\n", __func__);
            case EPERM:
                t_error("%s SKIP: missing proper permissions for runtime test\n", __func__);
        }

        t_error("%s fanotify_init failed\n", __func__);
    }

    int result = fanotify_mark(fd,
        FAN_MARK_ADD | FAN_MARK_MOUNT,
        FAN_ACCESS | FAN_MODIFY | FAN_OPEN | FAN_CLOSE | FAN_ONDIR | FAN_EVENT_ON_CHILD,
        AT_FDCWD,
        ".");

    if (result) {
        t_error("%s fanotify_mark (...) failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    // fanotify_init_0100();
    return t_status;
}
