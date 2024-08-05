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

#include <stdio_ext.h>
#include "functionalext.h"

/**
 * @tc.name      : __flbf_0100
 * @tc.desc      : Line buffer test
 * @tc.level     : Level 0
 */
void __flbf_0100(void)
{
    char buf[BUFSIZ];

    FILE *fp = fopen("/proc/version", "r");
    EXPECT_PTRNE("__flbf_0100", fp, NULL);

    int ret = setvbuf(fp, buf, _IOLBF, sizeof buf);
    EXPECT_EQ("__flbf_0100", ret, 0);

    int result = __flbf(fp);
    EXPECT_TRUE("__flbf_0100", result);
    fclose(fp);
}

/**
 * @tc.name      : __flbf_0200
 * @tc.desc      : Full buffer test
 * @tc.level     : Level 2
 */
void __flbf_0200(void)
{
    char buf[BUFSIZ];

    FILE *fp = fopen("/proc/version", "r");
    EXPECT_PTRNE("__flbf_0200", fp, NULL);

    int ret = setvbuf(fp, buf, _IOFBF, sizeof buf);
    EXPECT_EQ("__flbf_0200", ret, 0);

    int result = __flbf(fp);
    EXPECT_FALSE("__flbf_0200", result);
    fclose(fp);
}

/**
 * @tc.name      : __flbf_0300
 * @tc.desc      : Unbuffered test
 * @tc.level     : Level 2
 */
void __flbf_0300(void)
{
    char buf[BUFSIZ];

    FILE *fp = fopen("/proc/version", "r");
    EXPECT_PTRNE("__flbf_0300", fp, NULL);

    int ret = setvbuf(fp, buf, _IONBF, sizeof buf);
    EXPECT_EQ("__flbf_0300", ret, 0);

    int result = __flbf(fp);
    EXPECT_FALSE("__flbf_0300", result);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    __flbf_0100();
    __flbf_0200();
    __flbf_0300();
    return t_status;
}