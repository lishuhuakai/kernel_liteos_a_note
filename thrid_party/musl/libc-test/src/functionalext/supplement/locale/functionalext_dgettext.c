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

#include <libintl.h>
#include <stdio.h>
#include <locale.h>
#include "functionalext.h"

#define PACKAGE "test_dcngettext"

/**
 * @tc.name      : dgettext_0200
 * @tc.desc      : Get string from dgettext() with mo file and specified domain
 *  This case needs to push the test_dgettext.mo file of the current path to the remote board.
 *  The remote path should be "${test_src_dir}/zh_CN/LC_MESSAGES/test_dgettext.mo"
 * @tc.level     : level 0
 */
void dgettext_0100()
{
    char *msgid = "hello";
    setlocale(LC_ALL, "zh_CN");
    bindtextdomain(PACKAGE, "/data/local/tmp/");
    char* result = dgettext(PACKAGE, msgid);
    if (!result) {
        EXPECT_PTRNE("dgettext_0200", result, NULL);
        return;
    }
    EXPECT_TRUE("dgettext_0200", strlen(result) > 0);
    EXPECT_STREQ("dgettext_0200", "nihao", result);
}

int main(void)
{
    dgettext_0100();
    return t_status;
}

