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
#include "functionalext.h"

/**
 * @tc.name      : pthread_attr_setscope_0100
 * @tc.desc      : Set contention scope attribute in thread attributes object with PTHREAD_SCOPE_SYSTEM
 * @tc.level     : Level 0
 */
void pthread_attr_setscope_0100(void)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int ret = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    EXPECT_EQ("pthread_attr_setscope_0100", ret, CMPFLAG);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_setscope_0200
 * @tc.desc      : Set contention scope attribute in thread attributes object with PTHREAD_SCOPE_PROCESS
 * @tc.level     : Level 2
 */
void pthread_attr_setscope_0200(void)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int ret = pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
    EXPECT_EQ("pthread_attr_setscope_0200", ret, ENOTSUP);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_setscope_0300
 * @tc.desc      : Set contention scope attribute in thread attributes object with -1
 * @tc.level     : Level 2
 */
void pthread_attr_setscope_0300(void)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int ret = pthread_attr_setscope(&attr, -1);
    EXPECT_EQ("pthread_attr_setscope_0300", ret, EINVAL);
    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_attr_setscope_0100();
    pthread_attr_setscope_0200();
    pthread_attr_setscope_0300();
    return t_status;
}