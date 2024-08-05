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

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

int32_t COUNT = 0;
const int32_t INIT_LEN = 0;
const int32_t INCREASE_LEN = 1;
const int32_t PTHREADSTACK = 0x4001;
const char *URL = "http://c.test.com";

void *threadfuncA(void *arg)
{
    COUNT++;
    return arg;
}

void *threadfunc(void *arg)
{
    return arg;
}

/**
 * @tc.name:      pthread_create_0100
 * @tc.desc:      Verify pthread create process sucess when attrp and arg is NULL.
 * @tc.level:     level 0.
 */
void pthread_create_0100(void)
{
    pthread_t ph;
    int32_t ret = pthread_create(&ph, NULL, threadfuncA, NULL);
    pthread_join(ph, NULL);
    EXPECT_EQ("pthread_create_0100", ret, INIT_LEN);
    EXPECT_EQ("pthread_create_0100", COUNT, INCREASE_LEN);
}

/**
 * @tc.name:      pthread_create_0200
 * @tc.desc:      Verify pthread create process sucess when attrp is NULL.
 * @tc.level:     level 0.
 */
void pthread_create_0200(void)
{
    pthread_t ph;
    int32_t ret = pthread_create(&ph, NULL, threadfunc, (char *)(URL));
    pthread_join(ph, NULL);
    EXPECT_EQ("pthread_create_0200", ret, INIT_LEN);
}

/**
 * @tc.name:      pthread_create_0300
 * @tc.desc:      Verify pthread create process sucess when attrp is NULL.
 * @tc.level:     level 1.
 */
void pthread_create_0300(void)
{
    pthread_t ph;
    pthread_attr_t atrr;

    pthread_attr_init(&atrr);
    int32_t ret = pthread_create(&ph, &atrr, threadfunc, (char *)(URL));
    pthread_join(ph, NULL);
    EXPECT_EQ("pthread_create_0300", ret, INIT_LEN);
}

/**
 * @tc.name:      pthread_create_0400
 * @tc.desc:      Verify pthread create process sucess when stackAddr is null.
 * @tc.level:     level 1
 */
void pthread_create_0400(void)
{
    int32_t ret;
    pthread_t ph;
    void *stackAddr = NULL;
    pthread_attr_t attr;
    int32_t pageSize = getpagesize();
    pthread_attr_init(&attr);
    ret = posix_memalign(&stackAddr, pageSize, PTHREADSTACK);
    if (ret) {
        EXPECT_NE("pthread_create_0400", ret, 0);
        return;
    }
    ret = pthread_attr_setstack(&attr, stackAddr, PTHREADSTACK);
    if (ret) {
        EXPECT_NE("pthread_create_0400", ret, 0);
        return;
    }
    ret = pthread_create(&ph, &attr, threadfunc, (char *)(URL));
    pthread_join(ph, NULL);
    pthread_attr_destroy(&attr);
    EXPECT_EQ("pthread_create_0400", ret, INIT_LEN);
}

/**
 * @tc.name:      pthread_create_0500
 * @tc.desc:      Verify pthread_create success when pthread is detached.
 * @tc.level:     levlel 1
 */
void pthread_create_0500(void)
{
    pthread_t ph;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int32_t ret = pthread_create(&ph, &attr, threadfunc, (char *)(URL));
    EXPECT_EQ("pthread_create_0500", ret, INIT_LEN);
}

int main(void)
{
    pthread_create_0100();
    pthread_create_0200();
    pthread_create_0300();
    pthread_create_0400();
    pthread_create_0500();
    return t_status;
}
