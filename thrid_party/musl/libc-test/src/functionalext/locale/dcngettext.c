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
#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

#define PACKAGE "test_dcngettext"

/**
 * @tc.name      : dcngettext_0100
 * @tc.desc      : Get string from dcngettext() with mo file
 *  This case need push the test_dcngettext.mo file of the current path to remote board
 *  The remote path should be "${test_src_dir}/zh_CN/LC_MESSAGES/test_dcngettext.mo"
 * @tc.level     : Level 0
 */
static void dcngettext_0100(void)
{
    char *msgid = "hello";
    setlocale(LC_ALL, "zh_CN");
    bindtextdomain(PACKAGE, "/data/local/tmp/");
    textdomain(PACKAGE);
    char *result = dcngettext(PACKAGE, msgid, 0, 1, LC_MESSAGES);
    if (!result) {
        EXPECT_PTRNE("dcngettext_0100", result, NULL);
        return;
    }

    EXPECT_TRUE("dcngettext_0100", strlen(result) > 0);
    EXPECT_STREQ("dcngettext_0100", "nihao", result);
}

int main(void)
{
    dcngettext_0100();
    return t_status;
}