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

#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include "test.h"

/**
 * @tc.name      : sendmsg_0100
 * @tc.desc      : Test sendmsg to send messages through socket
 * @tc.level     : Level 0
 */
void sendmsg_0100(void)
{
    int sockfd;
    struct sockaddr_in addr;
    int retval;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        t_error("sendmsg_0100 socket error");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(1234);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        t_error("sendmsg_0100 connect error");
        exit(EXIT_FAILURE);
    }
    struct msghdr msg;
    bzero(&msg, sizeof(struct msghdr));

    retval = sendmsg(sockfd, &msg, 2);
    if (retval == -1)
        t_error("sendmsg_0100 sendmsg error");
}

int main(int argc, char *argv[])
{
    sendmsg_0100();
    return t_status;
}