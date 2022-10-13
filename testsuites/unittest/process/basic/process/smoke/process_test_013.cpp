/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "it_test_process.h"
#include "sched.h"

static int TestCase()
{
#define PROCESS_TEST_PRI1 (currProcessPri + 1)
    struct sched_param param = { 0 };
    int ret = OS_ERROR;
    int val, currPolicy;
    int currProcessPri = getpriority(PRIO_PROCESS, getpid());
    ICUNIT_GOTO_WITHIN_EQUAL(currProcessPri, 0, 31, currProcessPri, ERROR_OUT); // 31, assert currProcessPri equal to this.

    currPolicy = sched_getscheduler(getpid());
    ICUNIT_GOTO_EQUAL(currPolicy, SCHED_RR, currPolicy, ERROR_OUT);

    val = getpriority(PRIO_PROCESS, 0);
    ICUNIT_GOTO_EQUAL(val, currProcessPri, val, ERROR_OUT);

    ret = sched_getparam(0, &param);
    ICUNIT_GOTO_EQUAL(ret, 0, ret, ERROR_OUT);
    ICUNIT_GOTO_EQUAL(param.sched_priority, currProcessPri, param.sched_priority, ERROR_OUT);

    val = sched_getscheduler(0);
    ICUNIT_GOTO_EQUAL(val, currPolicy, val, ERROR_OUT);

    ret = setpriority(PRIO_PROCESS, 0, PROCESS_TEST_PRI1);
    ICUNIT_GOTO_EQUAL(ret, 0, ret, ERROR_OUT);

    ret = getpriority(PRIO_PROCESS, 0);
    ICUNIT_GOTO_EQUAL(ret, PROCESS_TEST_PRI1, ret, ERROR_OUT);

    param.sched_priority = currProcessPri;
    ret = sched_setparam(0, &param);
    ICUNIT_GOTO_EQUAL(ret, 0, ret, ERROR_OUT);

    ret = getpriority(PRIO_PROCESS, getpid());
    ICUNIT_GOTO_EQUAL(ret, currProcessPri, ret, ERROR_OUT);

    ret = sched_setscheduler(0, SCHED_FIFO, &param);
    ICUNIT_GOTO_EQUAL(ret, -1, ret, ERROR_OUT);
    ret = errno;
    ICUNIT_GOTO_EQUAL(ret, EINVAL, ret, ERROR_OUT);

    ret = sched_setscheduler(0, SCHED_RR, &param);
    ICUNIT_GOTO_EQUAL(ret, 0, ret, ERROR_OUT);

    ret = sched_setscheduler(1, SCHED_FIFO, &param);
    ICUNIT_GOTO_EQUAL(ret, -1, ret, ERROR_OUT);
    ret = errno;
    ICUNIT_GOTO_EQUAL(ret, EPERM, ret, ERROR_OUT);

    ret = sched_setscheduler(2, SCHED_FIFO, &param); // 2, input the pid.
    ICUNIT_GOTO_EQUAL(ret, -1, ret, ERROR_OUT);
    ret = errno;
    ICUNIT_GOTO_EQUAL(ret, EPERM, ret, ERROR_OUT);

    ret = sched_setparam(1, &param);
    ICUNIT_GOTO_EQUAL(ret, -1, ret, ERROR_OUT);
    ret = errno;
    ICUNIT_GOTO_EQUAL(ret, EPERM, ret, ERROR_OUT);

    ret = sched_setparam(2, &param); // 2, set the param.
    ICUNIT_GOTO_EQUAL(ret, -1, ret, ERROR_OUT);
    ret = errno;
    ICUNIT_GOTO_EQUAL(ret, EPERM, ret, ERROR_OUT);

    ret = setpriority(PRIO_PROCESS, 1, PROCESS_TEST_PRI1);
    ICUNIT_GOTO_EQUAL(ret, -1, ret, ERROR_OUT);
    ret = errno;
    ICUNIT_GOTO_EQUAL(ret, EPERM, ret, ERROR_OUT);

    ret = setpriority(PRIO_PROCESS, 2, PROCESS_TEST_PRI1); // 2, Used to calculate priorities.
    ICUNIT_GOTO_EQUAL(ret, -1, ret, ERROR_OUT);
    ret = errno;
    ICUNIT_GOTO_EQUAL(ret, EPERM, ret, ERROR_OUT);

    return 0;

ERROR_OUT:
    return ret;
}

void ItTestProcess013(void)
{
    TEST_ADD_CASE("IT_POSIX_PROCESS_013", TestCase, TEST_POSIX, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}
