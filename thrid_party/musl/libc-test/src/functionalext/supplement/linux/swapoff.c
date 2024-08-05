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
#include <stdio.h>
#include <stdlib.h>
#include <sys/swap.h>

#include "filepath_util.h"

/**
 * @tc.name      : swapoff_0100
 * @tc.desc      : Stop swapping to file/device
 * @tc.level     : Level 0
 */
void swapoff_0100(void)
{
    char dir_path[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(dir_path);
    errno = 0;
    char cmd[PATH_MAX] = {0};
    snprintf(cmd, sizeof(cmd), "cd %s; dd if=/dev/zero of=swapfile count=1 bs=1k; mkswap swapfile", dir_path);
    system(cmd);

    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_SWAP, path);
    int result = swapon(path, SWAP_FLAG_PREFER);
    if (result == -1) {
        t_error("%s swapon failed\n", __func__);
        remove(path);
        return;
    }
    if (errno == ENOSYS) {
        t_error("%s errno is %d\n", __func__, errno);
        return;
    }

    result = swapoff(path);
    if (result == -1) {
        t_error("%s swapoff failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    // swapoff_0100();
    return t_status;
}
