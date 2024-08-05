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
 * @tc.name      : __fbufsize_0100
 * @tc.desc      : Returns the size of the buffer currently used by the given stream
 * @tc.level     : Level 0
 */
void __fbufsize_0100(void)
{
    char buf[1024];
    FILE *fp = fopen("/proc/version", "r");
    EXPECT_PTRNE("__fbufsize_0100", fp, NULL);

    int ret = setvbuf(fp, buf, _IOFBF, sizeof buf);
    EXPECT_NE("__fbufsize_0100", ret, EOF);

    size_t result = __fbufsize(fp);
    EXPECT_TRUE("__fbufsize_0100", result <= sizeof buf);

    fclose(fp);
}

/**
 * @tc.name      : __fbufsize_0200
 * @tc.desc      : Returns the size of the buffer currently used by the given stream when the file size is 0
 * @tc.level     : Level 0
 */
void __fbufsize_0200(void)
{
    FILE *fp = fopen("/proc/version", "r");
    EXPECT_PTRNE("__fbufsize_0200", fp, NULL);

    int ret = setvbuf(fp, NULL, _IONBF, 0);
    EXPECT_NE("__fbufsize_0100", ret, EOF);

    size_t result = __fbufsize(fp);
    EXPECT_EQ("__fbufsize_0200", result, 0U);

    fclose(fp);
}

int main(int argc, char *argv[])
{
    __fbufsize_0100();
    __fbufsize_0200();
    return t_status;
}