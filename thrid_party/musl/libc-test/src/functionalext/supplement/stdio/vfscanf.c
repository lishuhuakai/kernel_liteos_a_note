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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "filepath_util.h"

int readFile(FILE *stream, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int result = vfscanf(stream, fmt, ap);
    va_end(ap);
    return result;
}

/**
 * @tc.name      : vfscanf_0100
 * @tc.desc      : Reads data from the stream and stores them according to the parameter format.
 * @tc.level     : Level 0
 */
void vfscanf_0100(void)
{
    FILE *fp = NULL;
    int val = 0;
    char buffer[BUFSIZ];
    char file[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_VFSCANF_TXT, file);
    fp = fopen(file, "w+");
    if (fp == NULL) {
        t_error("%s fopen failed\n", __func__);
        return;
    }
    if (fprintf(fp, "%s %d", "vfscanftest", 123) < 0) {
        t_error("%s fprintf failed\n", __func__);
        return;
    }
    rewind(fp);

    int result = readFile(fp, "%s %d", buffer, &val);
    if (result < 0) {
        t_error("%s vfscanf get result is %d are less 0\n", __func__, result);
    }
    if (strcmp(buffer, "vfscanftest") != 0) {
        t_error("%s vfscanf get is '%s' are not 'vfscanftest'\n", __func__, buffer);
    }
    if (val != 123) {
        t_error("%s vfscanf get is %d are not 123\n", __func__, val);
    }
    fclose(fp);
    remove(file);
}

/**
 * @tc.name      : vfscanf_0200
 * @tc.desc      : Test conversion of different formats
 * @tc.level     : Level 1
 */
void vfscanf_0200(void)
{
    FILE *fp = NULL;
    char val[BUFSIZ];
    char buffer[BUFSIZ];
    char file[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_VFSCANF_TXT, file);
    fp = fopen(file, "w+");
    if (fp == NULL) {
        t_error("%s fopen failed\n", __func__);
        return;
    }
    if (fprintf(fp, "%s %d", "vfscanftest", 123) < 0) {
        t_error("%s fprintf failed\n", __func__);
        return;
    }
    rewind(fp);

    int result = readFile(fp, "%s %s", buffer, val);
    if (result < 0) {
        t_error("%s vfscanf get result is %d are less 0\n", __func__, result);
    }
    if (strcmp(buffer, "vfscanftest") != 0) {
        t_error("%s vfscanf get is '%s' are not 'vfscanftest'\n", __func__, buffer);
    }
    if (strcmp(val, "123") != 0) {
        t_error("%s vfscanf get is '%s' are not '123'\n", __func__, val);
    }
    fclose(fp);
    remove(file);
}

/**
 * @tc.name      : vfscanf_0300
 * @tc.desc      : The test corresponding position does not match the format
 * @tc.level     : Level 2
 */
void vfscanf_0300(void)
{
    FILE *fp = NULL;
    int val1 = 0;
    int val2 = 0;
    char file[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_VFSCANF_TXT, file);
    fp = fopen(file, "w+");
    if (fp == NULL) {
        t_error("%s fopen failed", __func__);
        return;
    }
    if (fprintf(fp, "%s %d", "vfscanftest", 123) < 0) {
        t_error("%s fprintf failed", __func__);
        return;
    }
    rewind(fp);

    int result = readFile(fp, "%d %d", val1, val2);
    if (result > 0) {
        t_error("%s vfscanf get result is %d are more than the 0\n", __func__, result);
    }
    if (val1 != 0) {
        t_error("%s vfscanf get is %d are not 0\n", __func__, val1);
    }
    if (val2 != 0) {
        t_error("%s vfscanf get is %d are not 0\n", __func__, val2);
    }
    fclose(fp);
    remove(file);
}

int main(int argc, char *argv[])
{
    vfscanf_0100();
    vfscanf_0200();
    vfscanf_0300();
    return t_status;
}