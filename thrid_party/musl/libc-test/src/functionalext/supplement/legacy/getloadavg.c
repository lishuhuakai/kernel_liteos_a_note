/*
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

#include <limits.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : getloadavg_0100
 * @tc.desc      : Verify the average number of processes in the system running queue in different time periods
 * @tc.level     : Level 0
 */
void getloadavg_0100(void)
{
    int result;
    double load[3];

    result = getloadavg(load, -1);
    EXPECT_EQ("getloadavg_0100", result, -1);
    result = getloadavg(load, INT_MIN);
    EXPECT_EQ("getloadavg_0100", result, -1);

    result = getloadavg(load, 0);
    EXPECT_EQ("getloadavg_0100", result, 0);

    result = getloadavg(load, 1);
    EXPECT_EQ("getloadavg_0100", result, 1);
    result = getloadavg(load, 2);
    EXPECT_EQ("getloadavg_0100", result, 2);
    result = getloadavg(load, 3);
    EXPECT_EQ("getloadavg_0100", result, 3);
    result = getloadavg(load, 4);
    EXPECT_EQ("getloadavg_0100", result, 3);
    result = getloadavg(load, INT_MAX);
    EXPECT_EQ("getloadavg_0100", result, 3);
}

int main(int argc, char *argv[])
{
    getloadavg_0100();
    return t_status;
}