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
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include "test.h"

/**
 * @tc.name      : tcdrain_0100
 * @tc.desc      : Wait until output has been transmitted
 * @tc.level     : Level 0
 */
void tcdrain_0100(void)
{
    char Master[] = "/dev/ptmx";
    char text[] = "text to be written to tty";
    char data[80];
    int fd_master, fd_slave, result;
    time_t T;

    fd_master = open(Master, O_RDWR | O_NONBLOCK);
    if (fd_master < 0) {
        t_error("%s open master failed", __func__);
    }

    result = grantpt(fd_master);
    if (result < 0) {
        t_error("%s grantpt failed", __func__);
        return;
    }

    result = unlockpt(fd_master);
    if (result < 0) {
        t_error("%s unlockpt failed", __func__);
        return;
    }

    char *Slave = ptsname(fd_master);
    fd_slave = open(Slave, O_RDWR | O_NONBLOCK);
    if (fd_slave < 0) {
        t_error("%s open slave failed", __func__);
    }

    if (fork() == 0) {
        if (write(fd_slave, text, strlen(text) + 1) == -1) {
            t_error("%s write failed", __func__);
        }
        time(&T);
        printf("child has written to tty, tcdrain() started at %s", ctime(&T));

        if (tcdrain(fd_slave) != 0) {
            t_error("%s tcdrain failed", __func__);
        }

        time(&T);
        printf("tcdrain() returned at %s", ctime(&T));
        exit(0);
    }

    time(&T);
    printf("parent is starting nap at %s", ctime(&T));
    sleep(2);
    time(&T);
    printf("parent is done with nap at %s", ctime(&T));
    if (read(fd_master, data, sizeof(data)) == -1) {
        t_error("%s read failed", __func__);
    } else {
        printf("read '%s' from the tty\n", data);
    }
    sleep(2);
    close(fd_slave);
    close(fd_master);
}

int main(int argc, char *argv[])
{
    tcdrain_0100();

    return t_status;
}