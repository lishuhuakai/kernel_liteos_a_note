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

#include <errno.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include "functionalext.h"

#define TEST_BUFFER_SIZE 128
#define TEST_DATA_COUNT 10

struct inotify_test {
    int fd;
    int wd;
    uint32_t mask;
};

static int inotify_init_test(void)
{
    errno = 0;
    return inotify_init();
}

static int inotify_add_watch_test(int fd, uint32_t mask)
{
    errno = 0;
    return inotify_add_watch(fd, "/data", mask);
}

static int inotify_rm_watch_test(int fd, int wd)
{
    errno = 0;
    return inotify_rm_watch(fd, wd);
}

static void inotify_test(const char *msg)
{
    int fd = inotify_init_test();
    EXPECT_NE(msg, fd, ERREXPECT);
    if (fd == -1) {
        return;
    }

    int i, j;
    struct inotify_test data[TEST_DATA_COUNT] = {{.fd = fd, .wd = -1, .mask = IN_ACCESS},
        {.fd = fd, .wd = -1, .mask = IN_MODIFY},
        {.fd = fd, .wd = -1, .mask = IN_ATTRIB},
        {.fd = fd, .wd = -1, .mask = IN_CLOSE_WRITE},
        {.fd = fd, .wd = -1, .mask = IN_CLOSE_NOWRITE},
        {.fd = fd, .wd = -1, .mask = IN_OPEN},
        {.fd = fd, .wd = -1, .mask = IN_MOVED_FROM},
        {.fd = fd, .wd = -1, .mask = IN_MOVED_TO},
        {.fd = fd, .wd = -1, .mask = IN_CREATE},
        {.fd = fd, .wd = -1, .mask = IN_DELETE}};

    for (i = 0; i < TEST_DATA_COUNT; i++) {
        data[i].wd = inotify_add_watch_test(fd, data[i].mask);
        EXPECT_NE(msg, data[i].wd, ERREXPECT);
    }

    for (i = 0; i < TEST_DATA_COUNT; i++) {
        for (j = i; j >= 0; j--) {
            if (data[j].wd != data[i].wd) {
                int ret = inotify_rm_watch_test(fd, data[i].wd);
                EXPECT_EQ(msg, ret, CMPFLAG);
                break;
            }
        }
    }
    close(fd);
}

/**
 * @tc.name      : inotify_init_0100
 * @tc.desc      : initialize an inotify instance
 * @tc.level     : Level 0
 */
void inotify_init_0100(void)
{
    int fd = inotify_init_test();
    EXPECT_NE("inotify_init_0100", fd, ERREXPECT);
    EXPECT_EQ("inotify_init_0100", errno, CMPFLAG);
    close(fd);
}

/**
 * @tc.name      : inotify_add_watch_0100
 * @tc.desc      : add some watches to an initialized inotify instance
 * @tc.level     : Level 0
 */
void inotify_add_watch_0100(void)
{
    inotify_test("inotify_add_watch_0100");
}

/**
 * @tc.name      : inotify_add_watch_0200
 * @tc.desc      : Using the wrong fd, add some monitoring events to the inotify instance
 * @tc.level     : Level 2
 */
void inotify_add_watch_0200(void)
{
    int i;
    struct inotify_test data[TEST_DATA_COUNT] = {{.fd = -1, .wd = -1, .mask = IN_ACCESS},
        {.fd = -1, .wd = -1, .mask = IN_MODIFY},
        {.fd = -1, .wd = -1, .mask = IN_ATTRIB},
        {.fd = -1, .wd = -1, .mask = IN_CLOSE_WRITE},
        {.fd = -1, .wd = -1, .mask = IN_CLOSE_NOWRITE},
        {.fd = -1, .wd = -1, .mask = IN_OPEN},
        {.fd = -1, .wd = -1, .mask = IN_MOVED_FROM},
        {.fd = -1, .wd = -1, .mask = IN_MOVED_TO},
        {.fd = -1, .wd = -1, .mask = IN_CREATE},
        {.fd = -1, .wd = -1, .mask = IN_DELETE}};

    for (i = 0; i < TEST_DATA_COUNT; i++) {
        data[i].wd = inotify_add_watch_test(data[i].fd, data[i].mask);
        EXPECT_EQ("inotify_add_watch_0100", data[i].wd, ERREXPECT);
    }
}

/**
 * @tc.name      : inotify_rm_watch_0100
 * @tc.desc      : Remove some monitoring events from the inotify instance
 * @tc.level     : Level 0
 */
void inotify_rm_watch_0100(void)
{
    inotify_test("inotify_rm_watch_0100");
}

/**
 * @tc.name      : inotify_rm_watch_0200
 * @tc.desc      : Provide exception parameter to remove monitoring event from inotify instance
 * @tc.level     : Level 2
 */
void inotify_rm_watch_0200(void)
{
    int fd = inotify_init_test();
    if (fd == -1) {
        return;
    }

    int ret = inotify_rm_watch(fd, -1);
    EXPECT_EQ("inotify_rm_watch_0200", ret, ERREXPECT);
    close(fd);

    ret = inotify_rm_watch(-1, -1);
    EXPECT_EQ("inotify_rm_watch_0200", ret, ERREXPECT);
}

int main(void)
{
    inotify_init_0100();

    inotify_add_watch_0100();
    inotify_add_watch_0200();

    inotify_rm_watch_0100();
    inotify_rm_watch_0200();
    return t_status;
}