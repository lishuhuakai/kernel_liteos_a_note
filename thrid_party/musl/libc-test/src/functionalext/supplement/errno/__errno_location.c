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

#include <stdio.h>
#include <errno.h>
#include "test.h"

/**
 * @tc.name      : __errno_location_0100
 * @tc.desc      : Test the __errno_location method to get the errno address
 * @tc.level     : Level 0
 */
void __errno_location_0100(void)
{
    int result = *__errno_location();
    if (result != 0) {
        t_error("%s __errno_location get errno code is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : __errno_location_0200
 * @tc.desc      : The result returned by __errno_location when the value of errno has just been changed
 * @tc.level     : Level 1
 */
void __errno_location_0200(void)
{
    errno = 10;
    int result = *__errno_location();
    if (result != errno) {
        t_error("%s __errno_location get errno code is %d are not want %d\n", __func__, result, errno);
    }
    errno = 0;
}

/**
 * @tc.name      : __errno_location_0300
 * @tc.desc      : Test the address returned by __errno_location and compare it with the errno address
 * @tc.level     : Level 1
 */
void __errno_location_0300(void)
{
    int *result = __errno_location();
    if (result != &errno) {
        t_error("%s __errno_location get ptr is %p are not want %p\n", __func__, result, &errno);
    }
}

int main(int argc, char *argv[])
{
    __errno_location_0100();
    __errno_location_0200();
    __errno_location_0300();
    return t_status;
}