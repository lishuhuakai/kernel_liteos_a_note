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

const int32_t ERROR = 2U;

/**
 * @tc.name      : pthread_attr_setdetachstate_0100
 * @tc.desc      : Verify pthread_attr_setdetachstate process success
 * @tc.level     : Level 1
 */
void pthread_attr_setdetachstate_0100(void)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    struct sched_param sched = {0, 1};
    int32_t ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    EXPECT_EQ("pthread_attr_setdetachstate_0100", ret, 0);
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    EXPECT_EQ("pthread_attr_setdetachstate_0100", ret, 0);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_setdetachstate_0200
 * @tc.desc      : Verify pthread_attr_setdetachstate process fail.Because second param is error
 * @tc.level     : Level 1
 */
void pthread_attr_setdetachstate_0200(void)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int32_t ret = pthread_attr_setdetachstate(&attr, ERROR);
    EXPECT_EQ("pthread_attr_setdetachstate_0200", ret, EINVAL);
    pthread_attr_destroy(&attr);
}

int main(int argc, char *argv[])
{
    pthread_attr_setdetachstate_0100();
    pthread_attr_setdetachstate_0200();
    return t_status;
}
