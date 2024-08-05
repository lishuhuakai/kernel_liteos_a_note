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

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"

/**
 * @tc.name      : socketpair_0100
 * @tc.desc      : Use the socketpair function to create a pair of unnamed, interconnected sockets.
 * @tc.level     : Level 0
 */
void socketpair_0100(void)
{
    int fb[2];
    char buf[128] = {0};
    char *str = "mgn info is hello world";
    int result = socketpair(AF_UNIX, SOCK_STREAM, 0, fb);
    if (result == -1) {
        t_error("%s socketpair get result error is -1\n", __func__);
    }
    int size = write(fb[0], str, strlen(str));
    read(fb[1], buf, size);
    if (strcmp(buf, str) != 0) {
        t_error("%s socketpair get buf is %s are not %s\n", __func__, buf, str);
    }
}

/**
 * @tc.name      : socketpair_0200
 * @tc.desc      : Pass in AF_INET when testing the socketpair function call
 * @tc.level     : Level 2
 */
void socketpair_0200(void)
{
    int fb[2];
    int result = socketpair(AF_INET, SOCK_STREAM, 0, fb);
    if (result != -1) {
        t_error("%s socketpair get result error is %d not -1\n", __func__, result);
    }
}

/**
 * @tc.name      : socketpair_0300
 * @tc.desc      : Test that the length of the array passed two
 * @tc.level     : Level 1
 */
void socketpair_0300(void)
{
    int fb[3];
    char buf[128] = {0};
    char *str = "mgn info is hello world";
    int result = socketpair(AF_UNIX, SOCK_STREAM, 0, fb);
    if (result == -1) {
        t_error("%s socketpair get result error is %d not -1\n", __func__, result);
    }
    int size = write(fb[0], str, strlen(str));
    read(fb[1], buf, size);
    if (strcmp(buf, str) != 0) {
        t_error("%s socketpair get buf is %s are not %s\n", __func__, buf, str);
    }
}

int main(int argc, char *argv[])
{
    socketpair_0100();
    socketpair_0200();
    socketpair_0300();
    return t_status;
}