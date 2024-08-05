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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : shutdown_0100
 * @tc.desc      : The test stops the socket transmission through the shutdown function
 * @tc.level     : Level 0
 */
void shutdown_0100(void)
{
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9898);
    server_addr.sin_family = AF_INET;
    bind(sd, (struct sockaddr *)(&server_addr), sizeof(server_addr));
    listen(sd, 5);
    int result = shutdown(sd, 0);
    if (result != 0) {
        t_error("%s shutdown get result is %d are not 0", result);
    }
    close(sd);
}

int main(int argc, char *argv[])
{
    shutdown_0100();
    return t_status;
}