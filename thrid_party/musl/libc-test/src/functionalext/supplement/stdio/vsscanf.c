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
#include <stdarg.h>
#include <stdlib.h>
#include "test.h"

int get_return_vaule(char *s, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int result = vsscanf(s, fmt, ap);
    va_end(ap);
    return result;
}

/**
 * @tc.name      : vsscanf_0100
 * @tc.desc      : Call vsscanf to convert and format the data from the data according to format
 * @tc.level     : Level 0
 */
void vsscanf_0100(void)
{
    int val = 0;
    char buffer[100];
    int result = get_return_vaule("value 123 info", "%s %d", &buffer, &val);
    if (result < 0) {
        t_error("%s vsscanf get result is %d are less 0\n", __func__, result);
    }
    if (strcmp(buffer, "value") != 0) {
        t_error("%s vsscanf get is '%s' are not 'value'\n", __func__, buffer);
    }
    if (val != 123) {
        t_error("%s vsscanf get is %d are not 123\n", __func__, val);
    }
}

/**
 * @tc.name      : vsscanf_0200
 * @tc.desc      : Test conversion of different formats
 * @tc.level     : Level 1
 */
void vsscanf_0200(void)
{
    char buffer[100];
    char get_val[100];
    int result = get_return_vaule("value 123 info", "%s %s", &buffer, &get_val);
    if (result < 0) {
        t_error("%s vsscanf get result is %d are less 0\n", __func__, result);
    }
    if (strcmp(buffer, "value") != 0) {
        t_error("%s vsscanf get is '%s' are not 'value'\n", __func__, buffer);
    }
    if (strcmp(get_val, "123") != 0) {
        t_error("%s vsscanf get is %s are not 123\n", __func__, get_val);
    }
}

/**
 * @tc.name      : vsscanf_0300
 * @tc.desc      : The test corresponding position does not match the format
 * @tc.level     : Level 2
 */
void vsscanf_0300(void)
{
    int val = 0;
    int val_temp = 0;
    int result = get_return_vaule("value 123 info", "%d %d", &val, &val_temp);
    if (result > 0) {
        t_error("%s vsscanf get result is %d are more than the 0\n", __func__, result);
    }
    if (val != 0) {
        t_error("%s vsscanf get is %d are not 0\n", __func__, val);
    }
    if (val_temp != 0) {
        t_error("%s vsscanf get is %d are not 0\n", __func__, val_temp);
    }
}

int main(int argc, char *argv[])
{
    vsscanf_0100();
    vsscanf_0200();
    vsscanf_0300();
    return t_status;
}