/*
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
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <resolv.h>
#include <arpa/nameser.h>
#include <poll.h>
#include <pthread.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
unsigned char TEST_IOCTL_ACCESS_TOKEN_ID = 'A';
unsigned int GET_TOKEN_ID = 1;
unsigned int SET_TOKEN_ID = 2;
const char device_tokenid[] = "/dev/access_token_id";
#define TEST_TOKENID_GET_TOKENID _IOR(TEST_IOCTL_ACCESS_TOKEN_ID, GET_TOKEN_ID, unsigned long long)
#define TEST_TOKENID_SET_TOKENID _IOW(TEST_IOCTL_ACCESS_TOKEN_ID, SET_TOKEN_ID, unsigned long long)
static unsigned long long GetRand64(void)
{
    unsigned long long randValue = 0;
    int randFd = open("/dev/random", O_RDONLY);
    if (randFd > 0) {
        read(randFd, &randValue, sizeof(unsigned long long));
    }
    close(randFd);
    return randValue;
}

/**
 * @tc.name      : ioctl_0100
 * @tc.desc      : Verify ioctl management of character device.
 * @tc.level     : Level 0
 */
void ioctl_0100(void)
{
    unsigned long long tokenGet;
    unsigned long long token = GetRand64();
    int fd = open(device_tokenid, O_RDWR);
    if (fd < 0) {
        printf("open %s failed\n", device_tokenid);
        EXPECT_TRUE(ioctl_0100, false);
        return;
    }
    int ret = ioctl(fd, TEST_TOKENID_GET_TOKENID, &token);
    EXPECT_EQ("ioctl_0100", 0, ret);
    ret = ioctl(fd, TEST_TOKENID_GET_TOKENID, &tokenGet);
    EXPECT_EQ("ioctl_0100", 0, ret);
    EXPECT_EQ("ioctl_0100", 0, ret);
}

/**
 * @tc.name      : ioctl_0200
 * @tc.desc      : Verify ioctl management of network device.
 * @tc.level     : Level 0
 */
void ioctl_0200(void)
{
    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0) {
        printf("socket failed\n");
        EXPECT_TRUE(ioctl_0200, false);
        close(sockFd);
        return;
    }
    EXPECT_NE("ioctl_0200", -1, sockFd);
    struct ifreq ifr[3];
    struct ifconf ifc = {0};
    ifc.ifc_len = 3 * sizeof(struct ifreq);
    ifc.ifc_buf = (char *)ifr;
    int ret = ioctl(sockFd, SIOCGIFCONF, &ifc);
    EXPECT_EQ("ioctl_0200", 0, ret);
    int ifrCount = ifc.ifc_len / sizeof(struct ifreq);
    for (int i = 0; i < ifrCount; i++) {
        printf("interface name: %s\n", ifr[i].ifr_name);
        printf("address: %s\n", inet_ntoa(((struct sockaddr_in *)&(ifr[i].ifr_addr))->sin_addr));
    }
    if (ifrCount == 0) {
        close(sockFd);
        return;
    }
    char interName[128];
    strcpy(interName, ifr[0].ifr_name);
    char interAddr[128];
    strcpy(interAddr, inet_ntoa(((struct sockaddr_in *)&(ifr[0].ifr_addr))->sin_addr));
    struct ifreq ifrAddr = {0};
    strcpy(ifrAddr.ifr_name, interName);
    ret = ioctl(sockFd, SIOCGIFADDR, &ifrAddr);
    EXPECT_EQ("ioctl_0200", 0, ret);
    struct sockaddr_in *srvAddr = (struct sockaddr_in *)&ifrAddr.ifr_addr;
    EXPECT_STREQ("ioctl_0200", interAddr, inet_ntoa(srvAddr->sin_addr));
    close(sockFd);
}

TEST_FUN G_Fun_Array[] = {
    ioctl_0100,
    ioctl_0200,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
