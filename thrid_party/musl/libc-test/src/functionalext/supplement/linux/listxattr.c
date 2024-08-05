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
#include <stdlib.h>
#include <string.h>
#include <sys/xattr.h>
#include "functionalext.h"

const char *path = "file.txt";
const char *name = "user.txt";
const char *value = "dat";

/**
 * @tc.name      : listxattr_0100
 * @tc.desc      : Verify listxattr process success.
 * @tc.level     : Level 0
 */
void listxattr_0100(void)
{
    if (access(path, F_OK) == 0) {
        remove(path);
    }
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int fd = open(path, O_RDWR | O_CREAT, perms);
    if (fd == -1) {
        t_error("%s failed: fd = %d\n", __func__, fd);
        return;
    }

    char str[] = "dat";
    write(fd, str, sizeof(str));
    close(fd);

    system("chmod 777 file.txt");
    int ret = setxattr(path, name, value, strlen(value), XATTR_CREATE);
    if (ret != 0) {
        t_error("%s failed: ret = %d\n", __func__, ret);
        remove(path);
        return;
    }

    char list[BUFSIZ] = {0};
    ret = listxattr(path, list, sizeof(list));
    EXPECT_TRUE("listxattr_0100", ret > 0);
    if (strcasecmp(list, name)) {
        if (strcasecmp(list, "security.selinux")) {
            t_error("%s failed: list error\n", __func__);
        }
    }
    ret = remove(path);
    EXPECT_EQ("listxattr_0100", ret, 0);
}

/**
 * @tc.name      : listxattr_0200
 * @tc.desc      : Verify listxattr process fail bacause param is null.
 * @tc.level     : Level 2
 */
void listxattr_0200(void)
{
    int ret = listxattr(NULL, NULL, -1);
    EXPECT_EQ("listxattr_0200", ret, -1);
}

int main(void)
{
    listxattr_0100();
    listxattr_0200();
    return t_status;
}
