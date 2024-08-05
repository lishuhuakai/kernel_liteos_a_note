/**
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

#include <pthread.h>
#include <stdlib.h>
#include "functionalext.h"

#define TEST_STACK_SIZE 4096

/**
 * @tc.name      : pthread_attr_getscope_0100
 * @tc.desc      : Get thread scope
 * @tc.level     : Level 0
 */
void pthread_attr_getscope_0100(void)
{
    int scope = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int rev = pthread_attr_getscope(&attr, &scope);
    EXPECT_EQ("pthread_attr_getscope_0100", rev, 0);
    EXPECT_EQ("pthread_attr_getscope_0100", scope, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_getguardsize_0100
 * @tc.desc      : Get the guard value of the thread property
 * @tc.level     : Level 0
 */
void pthread_attr_getguardsize_0100(void)
{
    size_t size = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int rev = pthread_attr_getguardsize(&attr, &size);
    EXPECT_EQ("pthread_attr_getguardsize_0100", rev, 0);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_getinheritsched_0100
 * @tc.desc      : Verify that the preset properties can be obtained through get inheritsched in
 *                 a single-threaded environment
 * @tc.level     : Level 0
 */
void pthread_attr_getinheritsched_0100(void)
{
    pthread_attr_t attr;
    int result = -1;

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_getinheritsched_0100", ret, CMPFLAG);
    ret = pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
    EXPECT_EQ("pthread_attr_getinheritsched_0100", ret, CMPFLAG);

    ret = pthread_attr_getinheritsched(&attr, &result);
    EXPECT_EQ("pthread_attr_getinheritsched_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_getinheritsched_0100", result, PTHREAD_INHERIT_SCHED);

    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    EXPECT_EQ("pthread_attr_getinheritsched_0100", ret, CMPFLAG);

    ret = pthread_attr_getinheritsched(&attr, &result);
    EXPECT_EQ("pthread_attr_getinheritsched_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_getinheritsched_0100", result, PTHREAD_EXPLICIT_SCHED);

    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_getinheritsched_0200
 * @tc.desc      : Verify that the return value is EINVAL when setting an exception value in a single-threaded
 * environment
 * @tc.level     : Level 0
 */
void pthread_attr_getinheritsched_0200(void)
{
    pthread_attr_t attr;
    int inheritsched = 2;
    int result = -1;

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_getinheritsched_0200", ret, CMPFLAG);
    ret = pthread_attr_setinheritsched(&attr, inheritsched);
    EXPECT_EQ("pthread_attr_getinheritsched_0200", ret, EINVAL);

    ret = pthread_attr_getinheritsched(&attr, &result);
    EXPECT_EQ("pthread_attr_getinheritsched_0200", ret, CMPFLAG);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_barrierattr_getpshared_0100
 * @tc.desc      : Correct parameter setting barrier variable property value
 * @tc.level     : Level 0
 */
void pthread_barrierattr_getpshared_0100(void)
{
    pthread_barrierattr_t attr;
    int result = -1;
    int share = 0;

    int ret = pthread_barrierattr_init(&attr);
    EXPECT_EQ("pthread_barrierattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_barrierattr_setpshared(&attr, share);
    EXPECT_EQ("pthread_barrierattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_barrierattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_barrierattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_barrierattr_getpshared_0100", result, share);

    share = 1;
    ret = pthread_barrierattr_setpshared(&attr, share);

    ret = pthread_barrierattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_barrierattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_barrierattr_getpshared_0100", result, share);

    pthread_barrierattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_condattr_getclock_0100
 * @tc.desc      : Correct parameter setting condition variable property value
 * @tc.level     : Level 0
 */
void pthread_condattr_getclock_0100(void)
{
    pthread_condattr_t attr;
    clockid_t result = -1;

    int ret = pthread_condattr_init(&attr);
    EXPECT_EQ("pthread_condattr_getclock_0100", ret, CMPFLAG);

    ret = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC_RAW);
    EXPECT_EQ("pthread_condattr_getclock_0100", ret, CMPFLAG);

    ret = pthread_condattr_getclock(&attr, &result);
    EXPECT_EQ("pthread_condattr_getclock_0200", ret, CMPFLAG);
    EXPECT_EQ("pthread_condattr_getclock_0100", result, CLOCK_MONOTONIC_RAW);

    pthread_condattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_condattr_getpshared_0100
 * @tc.desc      : Correct parameter setting condition variable property value
 * @tc.level     : Level 0
 */
void pthread_condattr_getpshared_0100(void)
{
    pthread_condattr_t attr;
    int result = -1;
    int share = 0;
    int ret = pthread_condattr_init(&attr);

    EXPECT_EQ("pthread_condattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_condattr_setpshared(&attr, share);
    EXPECT_EQ("pthread_condattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_condattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_condattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_condattr_getpshared_0100", result, share);

    share = 1;
    ret = pthread_condattr_setpshared(&attr, share);

    ret = pthread_condattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_condattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_condattr_getpshared_0100", result, share);

    pthread_condattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_mutexattr_getprotocol_0100
 * @tc.desc      : Correct parameter setting condition variable property value
 * @tc.level     : Level 0
 */
void pthread_mutexattr_getprotocol_0100(void)
{
    pthread_mutexattr_t attr;
    int result = -1;

    int ret = pthread_mutexattr_init(&attr);
    EXPECT_EQ("pthread_mutexattr_getprotocol_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_NONE);
    EXPECT_EQ("pthread_mutexattr_getprotocol_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_getprotocol(&attr, &result);
    EXPECT_EQ("pthread_mutexattr_getprotocol_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_mutexattr_getprotocol_0100", result, PTHREAD_PRIO_NONE);

    pthread_mutexattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_mutexattr_gettype_0100
 * @tc.desc      : Correct parameter setting condition variable property value
 * @tc.level     : Level 0
 */
void pthread_mutexattr_gettype_0100(void)
{
    pthread_mutexattr_t attr;
    int result = -1;

    int ret = pthread_mutexattr_init(&attr);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_gettype(&attr, &result);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", result, PTHREAD_MUTEX_NORMAL);

    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_gettype(&attr, &result);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", result, PTHREAD_MUTEX_ERRORCHECK);

    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_gettype(&attr, &result);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", result, PTHREAD_MUTEX_RECURSIVE);

    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_DEFAULT);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_gettype(&attr, &result);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_mutexattr_gettype_0100", result, PTHREAD_MUTEX_DEFAULT);

    pthread_mutexattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_mutexattr_getpshared_0100
 * @tc.desc      : Correct parameter setting mutex variable property value
 * @tc.level     : Level 0
 */
void pthread_mutexattr_getpshared_0100(void)
{
    pthread_mutexattr_t attr;
    int result = -1;
    int share = 0;
    int ret = pthread_mutexattr_init(&attr);

    EXPECT_EQ("pthread_mutexattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_setpshared(&attr, share);
    EXPECT_EQ("pthread_mutexattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_mutexattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_mutexattr_getpshared_0100", result, share);

    share = 1;
    ret = pthread_mutexattr_setpshared(&attr, share);

    ret = pthread_mutexattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_mutexattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_mutexattr_getpshared_0100", result, share);

    pthread_mutexattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_rwlockattr_getpshared_0100
 * @tc.desc      : Correct parameter setting rwlock variable property value
 * @tc.level     : Level 0
 */
void pthread_rwlockattr_getpshared_0100(void)
{
    pthread_rwlockattr_t attr;
    int result = -1;
    int share = 0;
    int ret = pthread_rwlockattr_init(&attr);

    EXPECT_EQ("pthread_rwlockattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_rwlockattr_setpshared(&attr, share);
    EXPECT_EQ("pthread_rwlockattr_getpshared_0100", ret, CMPFLAG);

    ret = pthread_rwlockattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_rwlockattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_rwlockattr_getpshared_0100", result, share);

    share = 1;
    ret = pthread_rwlockattr_setpshared(&attr, share);

    ret = pthread_rwlockattr_getpshared(&attr, &result);
    EXPECT_EQ("pthread_rwlockattr_getpshared_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_rwlockattr_getpshared_0100", result, share);

    pthread_rwlockattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_getdetachstate_0100
 * @tc.desc      : Get detached state from thread attr
 * @tc.level     : Level 0
 */
void pthread_attr_getdetachstate_0100(void)
{
    pthread_attr_t attr;
    int state = 0;
    pthread_attr_init(&attr);
    int ret = pthread_attr_getdetachstate(&attr, &state);
    EXPECT_EQ("pthread_attr_getdetachstate_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_getdetachstate_0100", state, CMPFLAG);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_getschedpolicy_0100
 * @tc.desc      : Get detached state from thread attr
 * @tc.level     : Level 0
 */
void pthread_attr_getschedpolicy_0100(void)
{
    pthread_attr_t attr;
    int setpolicy = 1;
    int getpolicy = 0;
    pthread_attr_init(&attr);
    int ret = pthread_attr_setschedpolicy(&attr, setpolicy);
    EXPECT_EQ("pthread_attr_getschedpolicy_0100", ret, CMPFLAG);

    ret = pthread_attr_getschedpolicy(&attr, &getpolicy);
    EXPECT_EQ("pthread_attr_getschedpolicy_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_getschedpolicy_0100", getpolicy, setpolicy);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_getschedparam_0100
 * @tc.desc      : Get scheduling parameter attributes in thread attributes object
 * @tc.level     : Level 0
 */
void pthread_attr_getschedparam_0100(void)
{
    pthread_attr_t attr;
    struct sched_param setparam;
    struct sched_param getparam;

    memset(&setparam, 0x0, sizeof(setparam));
    memset(&getparam, 0x0, sizeof(getparam));
    setparam.sched_priority = 1;

    pthread_attr_init(&attr);
    int ret = pthread_attr_setschedparam(&attr, &setparam);
    EXPECT_EQ("pthread_attr_getschedparam_0100", ret, CMPFLAG);

    ret = pthread_attr_getschedparam(&attr, &getparam);
    EXPECT_EQ("pthread_attr_getschedparam_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_getschedparam_0100", getparam.sched_priority, 1);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_getstacksize_0100
 * @tc.desc      : Get stack size attribute in thread attributes object
 * @tc.level     : Level 0
 */
void pthread_attr_getstacksize_0100(void)
{
    size_t size;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int ret = pthread_attr_setstacksize(&attr, TEST_STACK_SIZE);
    EXPECT_EQ("pthread_attr_getstacksize_0100", ret, CMPFLAG);

    ret = pthread_attr_getstacksize(&attr, &size);
    EXPECT_EQ("pthread_attr_getstacksize_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_getstacksize_0100", size, TEST_STACK_SIZE);
    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_attr_getscope_0100();
    pthread_attr_getguardsize_0100();
    pthread_attr_getinheritsched_0100();
    pthread_attr_getinheritsched_0200();
    pthread_barrierattr_getpshared_0100();
    pthread_condattr_getclock_0100();
    pthread_condattr_getpshared_0100();
    pthread_mutexattr_getprotocol_0100();
    pthread_mutexattr_gettype_0100();
    pthread_mutexattr_getpshared_0100();
    pthread_rwlockattr_getpshared_0100();
    pthread_attr_getdetachstate_0100();
    pthread_attr_getschedpolicy_0100();
    pthread_attr_getschedparam_0100();
    pthread_attr_getstacksize_0100();
    return t_status;
}