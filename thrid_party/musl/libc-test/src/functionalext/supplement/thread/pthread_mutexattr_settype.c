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
#include "functionalext.h"

static pthread_mutexattr_t gmutexattr;
const int INVALIDTYPE = 4;

/**
 * @tc.name:      pthread_mutexattr_settype_0100
 * @tc.desc:      Verify process pthread_mutexattr_settype success
 * @tc.level:     level 0
 */
void pthread_mutexattr_settype_0100(void)
{
    pthread_mutexattr_init(&gmutexattr);
    int ret = pthread_mutexattr_settype(&gmutexattr, PTHREAD_MUTEX_NORMAL);
    EXPECT_EQ("pthread_mutexattr_settype", ret, 0);
    pthread_mutexattr_destroy(&gmutexattr);
}

/**
 * @tc.name:      pthread_mutexattr_settype_0200
 * @tc.desc:      Verify process pthread_mutexattr_settype fail. Because type is invalid
 * @tc.level:     level 2
 */
void pthread_mutexattr_settype_0200(void)
{
    pthread_mutexattr_init(&gmutexattr);
    int ret = pthread_mutexattr_settype(&gmutexattr, INVALIDTYPE);
    EXPECT_EQ("pthread_mutexattr_settype", ret, EINVAL);
    pthread_mutexattr_destroy(&gmutexattr);
}

int main(void)
{
    pthread_mutexattr_settype_0100();
    pthread_mutexattr_settype_0200();
    return t_status;
}