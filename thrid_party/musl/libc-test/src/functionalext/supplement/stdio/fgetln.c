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

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name       : fgetln_0100
 * @tc.desc       : Returns a pointer to the next line from the stream referenced by stream
 * @tc.level      : level 0
 */
void fgetln_0100(void)
{
    size_t plen;

    FILE *f = fopen("/data/test.txt", "w+");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
    }

    fputs("HelloWorld", f);
    fseek(f, 0, SEEK_SET);
    char *result = fgetln(f, &plen);
    if (!result) {
        t_error("%s fgetln failed\n", __func__);
    }
    if (strcmp(result, "HelloWorld")) {
        t_error("%s result is %s, not HelloWorld\n", __func__, result);
    }
    if (plen != strlen(result)) {
        t_error("%s failed, plen is %zu\n", __func__, plen);
    }

    fclose(f);
    remove("/data/test.txt");
}

/**
 * @tc.name       : fgetln_0200
 * @tc.desc       :
 * @tc.level      : level 2
 */
void fgetln_0200(void)
{
    size_t plen;

    FILE *f = fopen("/data/test.txt", "w+");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
    }

    char *result = fgetln(f, &plen);
    if (result) {
        t_error("%s fgetln should be failed\n", __func__);
    }

    fclose(f);
    remove("/data/test.txt");
}

int main(int argc, char *argv[])
{
    fgetln_0100();
    fgetln_0200();
    return t_status;
}