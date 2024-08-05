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

#include <stdio.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : freeifaddrs_0100
 * @tc.desc      : Verifies that the specified memory can be freed (parameter valid)
 * @tc.level     : Level 0
 */
void freeifaddrs_0100(void)
{
    struct ifaddrs *addrs = NULL;
    struct ifaddrs *lo_inet4 = NULL;
    struct ifaddrs *lo_inet6 = NULL;
    struct ifaddrs *lo_packet = NULL;

    int result = getifaddrs(&addrs);
    if (result != 0) {
        t_error("%s getifaddrs failed\n", __func__);
    }
    if (!addrs) {
        t_error("%s addrs is NULL\n", __func__);
    }

    for (struct ifaddrs *addr = addrs; addr != NULL; addr = addr->ifa_next) {
        if (addr->ifa_name && strcmp(addr->ifa_name, "lo") == 0) {
            if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_INET) {
                lo_inet4 = addr;
            } else if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_INET6) {
                lo_inet6 = addr;
            } else if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_PACKET) {
                lo_packet = addr;
            }
        }
    }

    if (lo_inet4 == NULL) {
        t_error("%s lo_inet4 is NULL\n", __func__);
    }
    if (lo_inet6 == NULL) {
        t_error("%s lo_inet4 is NULL\n", __func__);
    }
    if (lo_packet == NULL) {
        t_error("%s lo_inet4 is NULL\n", __func__);
    }

    freeifaddrs(addrs);
}

int main(int argc, char *argv[])
{
    freeifaddrs_0100();
    return t_status;
}