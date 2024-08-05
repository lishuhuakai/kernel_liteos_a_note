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

#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : tmpnam_0100
 * @tc.desc      : When buffer is provided, the pointer points to the custom buffer
 * @tc.level     : Level 0
 */
void tmpnam_0100(void)
{
    char buffer[L_tmpnam];
    char *p = tmpnam(buffer);
    if (p == NULL) {
        t_error("%s tmpnam failed", __func__);
    }
    if (strlen(buffer) == 0) {
        t_error("%s creat temporary name failed", __func__);
    }
}

/**
 * @tc.name      : tmpnam_0200
 * @tc.desc      : When str is a null pointer, returns a pointer to the internal buffer
 * @tc.level     : Level 1
 */
void tmpnam_0200(void)
{
    char *p = tmpnam(NULL);
    if (p == NULL) {
        t_error("%s tmpnam failed", __func__);
    }
    if (strlen(p) == 0) {
        t_error("%s creat temporary name failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    // tmpnam_0100();
    // tmpnam_0200();
    return t_status;
}
