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
#include <sys/xattr.h>
#include "functionalext.h"

const char *path = "file.txt";
const char *name = "user.txt";
const char *value = "dat";
const int size = 3;

/**
 * @tc.name      : lgetxattr_0100
 * @tc.desc      : Verify lgetxattr process success.
 * @tc.level     : Level 0
 */
void lgetxattr_0100(void)
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

    int ret = lsetxattr(path, name, value, strlen(value), XATTR_CREATE);
    if (ret != 0) {
        remove(path);
        t_error("%s failed: ret = %d\n", __func__, ret);
        return;
    }

    char buf[BUFSIZ] = {0};
    ret = lgetxattr(path, name, buf, sizeof(buf));
    EXPECT_EQ("lgetxattr_0100", ret, size);
    EXPECT_STREQ("lgetxattr_0100", buf, str);
    remove(path);
}

/**
 * @tc.name      : lgetxattr_0200
 * @tc.desc      : Verify lgetxattr process fail bacause param is null.
 * @tc.level     : Level 2
 */
void lgetxattr_0200(void)
{
    int ret = lgetxattr(NULL, NULL, NULL, -1);
    EXPECT_EQ("lgetxattr_0200", ret, -1);
}

int main(void)
{
    lgetxattr_0100();
    lgetxattr_0200();
    return t_status;
}
