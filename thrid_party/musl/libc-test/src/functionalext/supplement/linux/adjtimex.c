/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <sys/timex.h>
#include "functionalext.h"

extern int __adjtimex_time64(struct timex *);

const int32_t NUM_NEG = -1;

/**
 * @tc.name      : adjtimex_0100
 * @tc.desc      : Verify the time to tune the kernel.
 * @tc.level     : Level 0
 */
void adjtimex_0100(void)
{
    struct timex tx;
    tx.offset = ADJ_OFFSET;
    tx.tick = ADJ_TICK;
    tx.maxerror = ADJ_MAXERROR;
    tx.esterror = ADJ_ESTERROR;
    tx.constant = ADJ_TIMECONST;
    tx.freq = ADJ_FREQUENCY;
    tx.status = ADJ_STATUS;
    tx.precision = 1;
    tx.tolerance = 1;
    tx.ppsfreq = 1;
    tx.jitter = 1;
    tx.stabil = 1;
    tx.jitcnt = 1;
    tx.calcnt = 1;
    tx.errcnt = 1;
    tx.stbcnt = 1;
    int result = adjtimex(&tx);
    EXPECT_NE("adjtimex_0100", result, NUM_NEG);
}

/**
 * @tc.name      : adjtimex_time64_0100
 * @tc.desc      : Verify the time to tune the kernel.
 * @tc.level     : Level 0
 */
void adjtimex_time64_0100(void)
{
    struct timex tx;
    tx.offset = ADJ_OFFSET;
    tx.tick = ADJ_TICK;
    tx.maxerror = ADJ_MAXERROR;
    tx.esterror = ADJ_ESTERROR;
    tx.constant = ADJ_TIMECONST;
    tx.freq = ADJ_FREQUENCY;
    tx.status = ADJ_STATUS;
    tx.precision = 1;
    tx.tolerance = 1;
    tx.ppsfreq = 1;
    tx.jitter = 1;
    tx.stabil = 1;
    tx.jitcnt = 1;
    tx.calcnt = 1;
    tx.errcnt = 1;
    tx.stbcnt = 1;
    int result = __adjtimex_time64(&tx);
    EXPECT_NE("adjtimex_time64_0100", result, NUM_NEG);
}

int main(int argc, char *argv[])
{
    adjtimex_0100();
    adjtimex_time64_0100();

    return t_status;
}
