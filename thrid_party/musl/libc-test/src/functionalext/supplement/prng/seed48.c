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

#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

unsigned short xsp0 = 0x330e;
unsigned short xsp1 = 0x5432;
unsigned short xsp2 = 0x9876;

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : seed48_0100
 * @tc.desc      : generate uniformly distributed pseudo-random numbers
 * @tc.level     : Level 0
 */
void seed48_0100(void)
{
    long seed = 0x98765432;
    srand48(seed);

    unsigned short xs[] = {0x0001, 0x0012, 0x0123};
    unsigned short *xsp = seed48(xs);
    if (xsp[0] != xsp0 || xsp[1] != xsp1 || xsp[2] != xsp2) {
        t_error("%s failed: xsp[0] = %x, xsp[1] = %x, xsp[2] = %x\n", __func__, xsp[0], xsp[1], xsp[2]);
    }

    memcpy(xs, xsp, sizeof(xs));
    seed48(xs);
}

/**
 * @tc.name      : seed48_0200
 * @tc.desc      : generate uniformly distributed pseudo-random numbers with NULL
 * @tc.level     : Level 2
 */
void seed48_0200(void)
{
    signal(SIGSEGV, handler);

    seed48(NULL);
}

int main(int argc, char *argv[])
{
    seed48_0100();
    seed48_0200();

    return t_status;
}
