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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "test.h"

const char *text = "Hello";

/**
 * @tc.name      : tcflow_0100
 * @tc.desc      : Resume blocked terminal and read data
 * @tc.level     : Level 0
 */
void tcflow_0100(void)
{
    char data[BUFSIZ];
    int result;

    int fdm = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    if (fdm < 0) {
        t_error("%s open master failed", __func__);
        return;
    }
    result = grantpt(fdm);
    if (result < 0) {
        t_error("%s grantpt failed", __func__);
        return;
    }

    result = unlockpt(fdm);
    if (result < 0) {
        t_error("%s unlockpt failed", __func__);
        return;
    }

    char *slave = ptsname(fdm);
    int fds = open(slave, O_RDWR | O_NOCTTY);
    if (fds < 0) {
        t_error("%s open slave failed", __func__);
        return;
    }

    if (write(fds, text, strlen(text) + 1) == -1) {
        t_error("%s write failed", __func__);
        return;
    }

    if (tcflow(fds, TCOON) != 0) {
        t_error("%s tcflow failed", __func__);
    }

    if (read(fdm, data, sizeof(data)) == -1) {
        t_error("%s read resume failed", __func__);
    }
    if (strcmp(data, text)) {
        t_error("%s read data is %s", __func__, data);
    }

    close(fdm);
    close(fds);
}

/**
 * @tc.name      : tcflow_0200
 * @tc.desc      : Fildes is not a valid open file descriptor
 * @tc.level     : Level 2
 */
void tcflow_0200(void)
{
    int result = tcflow(-1, TCOON);
    if (result != -1) {
        t_error("%s result is %d", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    tcflow_0100();
    tcflow_0200();
    return t_status;
}