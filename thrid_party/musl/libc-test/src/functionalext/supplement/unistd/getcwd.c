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

#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "functionalext.h"

// Manual test result
char *manual = "/data/tests/libc-test/src/";
// hdc shell test result
char *hdc_auto = "/";

/*
 * @tc.name      : getcwd_0100
 * @tc.desc      : Verify that the absolute path of the current working directory can be obtained
 *                 (parameters are valid)
 * @tc.level     : Level 0
 */
void getcwd_0100(void)
{
    char *result;
    char buf[200] = {0};
    result = getcwd(buf, sizeof(buf));

    if (!(strcmp(result, manual) ^ strcmp(result, hdc_auto))) {
        t_error("%s getcwd failed, except result is %s when manual or %s when hdc test\n", __func__, manual, hdc_auto);
    }
}

/*
 * @tc.name      : getcwd_0200
 * @tc.desc      : Verify that the absolute path of the current working directory can be obtained (parameter is NULL)
 * @tc.level     : Level 1
 */
void getcwd_0200(void)
{
    char *result;
    result = getcwd(NULL, 200);
    if (!(strcmp(result, manual) ^ strcmp(result, hdc_auto))) {
        t_error("%s getcwd failed, except result is %s when manual or %s when hdc test\n", __func__, manual, hdc_auto);
    }
}

/*
 * @tc.name      : getcwd_0300
 * @tc.desc      : Verify that the absolute path of the current working directory cannot be obtained (parameter is 0)
 * @tc.level     : Level 2
 */
void getcwd_0300(void)
{
    char *result;
    char buf[200] = {0};
    result = getcwd(buf, 0);
    if (result) {
        t_error("%s getcwd should failed, the result should be null\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    getcwd_0100();
    getcwd_0200();
    getcwd_0300();
    return t_status;
}