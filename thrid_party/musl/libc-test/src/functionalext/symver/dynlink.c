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

#include "dso_easy_symver.h"
#include "dso_hard_symver.h"
#include "dso_no_symver.h"
#include "dso_symver.h"

/**
 * @tc.name      : dynlink_0100
 * @tc.desc      : invoke a symbol without versions directly
 * @tc.level     : Level 0
 */
void dynlink_0100(void)
{
    symver_log("start");

    const char *result = dso_no_symver();
    symver_streq(result, dso_no_symver_symbol);

    symver_log("end");
}

/**
 * @tc.name      : dynlink_0200
 * @tc.desc      : invoke a symbol with versions directly
 * @tc.level     : Level 1
 */
void dynlink_0200(void)
{
    symver_log("start");

    const char *result = dso_easy_symver();
    symver_streq(result, dso_easy_symver_symbol_stable);

    symver_log("end");
}

/**
 * @tc.name      : dynlink_0300
 * @tc.desc      : invoke a symbol with versions directly
 * @tc.level     : Level 1
 */
void dynlink_0300(void)
{
    symver_log("start");

    const char *result = dso_hard_symver_ld();
    symver_streq(result, dso_hard_symver_ld_symbol_stable);

    symver_log("end");
}

int main(int argc, char *argv[])
{
    symver_log("start");

    dynlink_0100();
    dynlink_0200();
    dynlink_0300();

    symver_log("t_status = %d", t_status);
    symver_log("end");

    return t_status;
}
