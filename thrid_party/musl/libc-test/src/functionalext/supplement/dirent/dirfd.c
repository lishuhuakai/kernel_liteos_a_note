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

#include <dirent.h>
#include "test.h"

/*
 * @tc.name      : dirfd_0100
 * @tc.desc      : Get directory stream file descriptor
 * @tc.level     : Level 0
 */
void dirfd_0100(void)
{
    DIR *dir = opendir(".");
    if (!dir) {
        t_error("%s opendir failed\n", __func__);
    }

    int fd = dirfd(dir);
    if (fd < 0) {
        t_error("%s dirfd failed\n", __func__);
    }

    int result = close(fd);
    if (result < 0) {
        t_error("%s close fd failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    dirfd_0100();
    return t_status;
}