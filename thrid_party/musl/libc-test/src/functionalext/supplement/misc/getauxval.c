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

#include <sys/auxv.h>
#include "functionalext.h"

/*
 * @tc.name      : getauxval_0100
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SECURE)
 * @tc.level     : Level 0
 */
void getauxval_0100(void)
{
    unsigned long int result;
    result = getauxval(AT_SECURE);
    EXPECT_EQ("getauxval_0100", result, 0);
}

/*
 * @tc.name      : getauxval_0200
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SYSINFO)
 * @tc.level     : Level 0
 */
void getauxval_0200(void)
{
    unsigned long int result;
    result = getauxval(AT_SYSINFO);
    EXPECT_EQ("getauxval_0200", result, 0);
}

/*
 * @tc.name      : getauxval_0300
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SYSINFO_EHDR)
 * @tc.level     : Level 0
 */
void getauxval_0300(void)
{
    unsigned long int result;
    result = getauxval(AT_SYSINFO_EHDR);
    EXPECT_TRUE("getauxval_0300", result != 0);
}

int main(int argc, char *argv[])
{
    getauxval_0100();
    getauxval_0200();
    getauxval_0300();
    return t_status;
}