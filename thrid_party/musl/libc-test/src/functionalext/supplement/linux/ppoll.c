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

#include <sys/stat.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>
#include "functionalext.h"

/**
 * @tc.name      : ppoll_0100
 * @tc.desc      : Monitor the readable attribute time of the specified file
 * @tc.level     : Level 0
 */
void ppoll_0100(void)
{
    int fd = open("ppoll_function_file", O_RDWR | O_CREAT, S_IRWXU);
    if (fd == -1) {
        t_error("create file failed\n");
        return;
    }
    struct pollfd pollfds[] = {{.fd = fd, .events = POLLIN, .revents = 0}};
    struct timespec timeout;
    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;

    int ret = ppoll(pollfds, 1, &timeout, NULL);
    EXPECT_GT("ppoll_0100", ret, CMPFLAG);
    EXPECT_EQ("ppoll_0100", pollfds[0].revents, POLLIN);
    close(fd);
    ret = access("ppoll_function_file", F_OK);
    if (ret != -1) {
        ret = remove("ppoll_function_file");
        EXPECT_EQ("ppoll_0100", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : ppoll_0200
 * @tc.desc      : Listen to non-specified file time (timeout returns)
 * @tc.level     : Level 2
 */
void ppoll_0200(void)
{
    struct pollfd pollfds[] = {{.fd = -1, .events = POLLIN, .revents = 0}};
    struct timespec timeout;
    timeout.tv_sec = 3;
    timeout.tv_nsec = 0;

    int ret = ppoll(pollfds, 1, &timeout, NULL);
    EXPECT_EQ("ppoll_0200", ret, CMPFLAG);
}

/**
 * @tc.name      : ppoll_0300
 * @tc.desc      : Listen for readable file event (illegal parameter)
 * @tc.level     : Level 2
 */
void ppoll_0300(void)
{
    struct timespec timeout;
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    int ret = ppoll(NULL, 1, &timeout, NULL);
    EXPECT_EQ("ppoll_0300", ret, ERREXPECT);
}

int main(void)
{
    ppoll_0100();
    ppoll_0200();
    ppoll_0300();
    return t_status;
}