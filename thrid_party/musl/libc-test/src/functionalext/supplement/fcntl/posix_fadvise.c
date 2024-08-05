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

#include <fcntl.h>
#include "functionalext.h"

#define TEST_LEN 10
#define TEST_FLAG_SIZE 6
/**
 * @tc.name      : posix_fadvise_0100
 * @tc.desc      : Correct parameters, set file preload size
 * @tc.level     : Level 0
 */
void posix_fadvise_0100(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("posix_fadvise_0100", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    EXPECT_TRUE("posix_fadvise_0100", fd != -1);
    if (fd == -1) {
        fclose(fp);
        return;
    }

    int flags[TEST_FLAG_SIZE] = {POSIX_FADV_NORMAL,
        POSIX_FADV_SEQUENTIAL,
        POSIX_FADV_RANDOM,
        POSIX_FADV_NOREUSE,
        POSIX_FADV_WILLNEED,
        POSIX_FADV_DONTNEED};
    int i;
    for (i = 0; i < TEST_FLAG_SIZE; i++) {
        int ret = posix_fadvise(fd, 0, TEST_LEN, flags[i]);
        EXPECT_EQ("posix_fadvise_0100", ret, 0);
    }
    int rev = fclose(fp);
    EXPECT_EQ("posix_fadvise_0100", rev, 0);
}

/**
 * @tc.name      : posix_fadvise_0200
 * @tc.desc      : Set file preload size to -1
 * @tc.level     : Level 2
 */
void posix_fadvise_0200(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("posix_fadvise_0200", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    EXPECT_TRUE("posix_fadvise_0200", fd != -1);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int ret = posix_fadvise(fd, 0, -1, POSIX_FADV_NORMAL);
    EXPECT_EQ("posix_fadvise_0200", ret, EINVAL);
    ret = fclose(fp);
    EXPECT_EQ("posix_fadvise_0200", ret, 0);
}

/**
 * @tc.name      : posix_fadvise_0300
 * @tc.desc      : Set file preload offset to -1
 * @tc.level     : Level 0
 */
void posix_fadvise_0300(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("posix_fadvise_0300", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    EXPECT_TRUE("posix_fadvise_0300", fd != -1);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int ret = posix_fadvise(fd, -1, TEST_LEN, POSIX_FADV_NORMAL);
    EXPECT_EQ("posix_fadvise_0300", ret, 0);
    ret = fclose(fp);
    EXPECT_EQ("posix_fadvise_0300", ret, 0);
}

/**
 * @tc.name      : posix_fadvise_0400
 * @tc.desc      : set wrong file descriptor
 * @tc.level     : Level 2
 */
void posix_fadvise_0400(void)
{
    int ret = posix_fadvise(-1, 0, TEST_LEN, POSIX_FADV_NORMAL);
    EXPECT_EQ("posix_fadvise_0400", ret, EBADF);
}

/**
 * @tc.name      : posix_fadvise_0500
 * @tc.desc      : set wrong flag
 * @tc.level     : Level 2
 */
void posix_fadvise_0500(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("posix_fadvise_0500", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    EXPECT_TRUE("posix_fadvise_0500", fd != -1);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int ret = posix_fadvise(fd, 0, TEST_LEN, -1);
    EXPECT_EQ("posix_fadvise_0500", ret, EINVAL);
    ret = fclose(fp);
    EXPECT_EQ("posix_fadvise_0500", ret, 0);
}

int main(void)
{
    posix_fadvise_0100();
    posix_fadvise_0200();
    posix_fadvise_0300();
    posix_fadvise_0400();
    posix_fadvise_0500();
    return t_status;
}