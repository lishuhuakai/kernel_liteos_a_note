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
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

const char *path1 = "/data/polltest1.txt";
const char *str1 = "HelloWorld";
const char *path2 = "/data/polltest2.txt";
const char *str2 = "TestforPoll";

/*
 * @tc.name      : poll_0100
 * @tc.desc      : Waits for one of a set of file descriptors to become ready to perform I/O.
 * @tc.level     : Level 0
 */
void poll_0100(void)
{
    char buf[BUFSIZ];
    struct pollfd fds[2];
    int time_delay = 2000;

    int fd1 = open(path1, O_WRONLY | O_CREAT, TEST_MODE);
    if (fd1 < 0) {
        t_error("%s open path1 failed\n", __func__);
    }
    write(fd1, str1, strlen(str1) + 1);
    close(fd1);

    int fd2 = open(path2, O_WRONLY | O_CREAT, TEST_MODE);
    if (fd2 < 0) {
        t_error("%s open path2 failed\n", __func__);
    }
    write(fd2, str2, strlen(str2) + 1);
    close(fd2);

    fds[0].fd = open(path1, O_RDONLY | O_NONBLOCK);
    if (fds[0].fd < 0) {
        t_error("%s open failed\n", __func__);
    }
    fds[0].events = POLLIN;

    fds[1].fd = open(path2, O_RDONLY | O_NONBLOCK);
    if (fds[1].fd < 0) {
        t_error("%s open failed\n", __func__);
    }
    fds[1].events = POLLIN;

    while (fds[0].events || fds[1].events) {
        int ret = poll(fds, 2, time_delay);
        if (ret <= 0) {
            t_error("%s poll failed\n", __func__);
        }
        for (int i = 0; i < 2; i++) {
            if (fds[i].revents) {
                memset(buf, 0, BUFSIZ);
                int result = read(fds[i].fd, buf, BUFSIZ);
                if (result < 0) {
                    t_error("%s read failed\n", __func__);
                } else if (result == 0) {
                    close(fds[i].fd);
                    fds[i].events = 0;
                } else {
                    buf[result] = '\0';
                    if (i == 0) {
                        if (strcmp(buf, str1)) {
                            t_error("%s read buf is %s, not HelloWorld\n", __func__, buf);
                        }
                    } else {
                        if (strcmp(buf, str2)) {
                            t_error("%s read buf is %s, not TestforPoll\n", __func__, buf);
                        }
                    }
                }
            }
        }
    }

    remove(path1);
    remove(path2);
}

/*
 * @tc.name      : poll_0200
 * @tc.desc      : When the parameter __fds is invalid, test the return value of the function
 * @tc.level     : Level 2
 */
void poll_0200(void)
{
    int result = poll(NULL, 1, 0);
    if (result != -1) {
        t_error("%s poll should be failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    poll_0100();
    poll_0200();
    return t_status;
}