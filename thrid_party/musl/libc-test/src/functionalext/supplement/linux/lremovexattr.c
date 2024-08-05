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
#include <sys/xattr.h>
#include "functionalext.h"

const char *path = "file.txt";
const char *name = "user.txt";
const char *value = "dat";
const int size = 3;

/**
 * @tc.name      : lremovexattr_0100
 * @tc.desc      : Verify lremovexattr process success.
 * @tc.level     : Level 0
 */
void lremovexattr_0100(void)
{
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd < 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
        return;
    }

    char str[] = "dat";
    write(fd, str, sizeof(str));
    close(fd);

    int ret = lsetxattr(path, name, value, strlen(value), XATTR_CREATE);
    if (ret != 0) {
        t_error("%s failed: ret = %d\n", __func__, ret);
        return;
    }

    char buf[BUFSIZ] = {0};

    ret = lgetxattr(path, name, buf, sizeof(buf));
    EXPECT_EQ("lgetxattr_0100", ret, size);
    EXPECT_STREQ("lgetxattr_0100", buf, str);

    ret = lremovexattr(path, name);
    EXPECT_EQ("lremovexattr_0100", ret, 0);

    ret = lgetxattr(path, name, buf, sizeof(buf));
    EXPECT_EQ("lgetxattr_0100", ret, -1);

    ret = remove(path);
    EXPECT_EQ("lgetxattr_0100", ret, 0);
}

/**
 * @tc.name      : lremovexattr_0200
 * @tc.desc      : Verify lremovexattr process fail bacause param is null.
 * @tc.level     : Level 0
 */
void lremovexattr_0200(void)
{
    int ret = lremovexattr(NULL, NULL);
    EXPECT_EQ("lremovexattr_0200", ret, -1);
}

int main(void)
{
    lremovexattr_0100();
    lremovexattr_0200();
    return t_status;
}
