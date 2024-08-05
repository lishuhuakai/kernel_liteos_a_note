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

#include <ctype.h>
#include "functionalext.h"

/**
 * @tc.name      : ispunct_0100
 * @tc.desc      : Determine whether the input character is a punctuation mark or a special symbol.
 * @tc.level     : Level 0
 */
void ispunct_0100(void)
{
    int var1 = 44;
    int ret = ispunct(var1);
    EXPECT_NE("ispunct_0100", ret, 0);
}

/**
 * @tc.name      : ispunct_0200
 * @tc.desc      : Determine whether the input character is a punctuation mark or a special symbol.
 * @tc.level     : Level 0
 */
void ispunct_0200(void)
{
    int var1 = 64;
    int ret = ispunct(var1);
    EXPECT_NE("ispunct_0200", ret, 0);
}

/**
 * @tc.name      : ispunct_0300
 * @tc.desc      : Determine whether the input character is a punctuation mark or a special symbol.
 * @tc.level     : Level 0
 */
void ispunct_0300(void)
{
    int var1 = 94;
    int ret = ispunct(var1);
    EXPECT_NE("ispunct_0300", ret, 0);
}

/**
 * @tc.name      : ispunct_0400
 * @tc.desc      : Determine if the input character is not a punctuation mark or a special symbol.
 * @tc.level     : Level 2
 */
void ispunct_0400(void)
{
    int var1 = 65;
    int ret = ispunct(var1);
    EXPECT_EQ("ispunct_0400", ret, 0);
}

/**
 * @tc.name      : ispunct_0500
 * @tc.desc      : Determine the number of punctuation marks or special symbols in the ascii code table.
 * @tc.level     : Level 1
 */
void ispunct_0500(void)
{
    int cout = 0;
    for (int i = 0; i < 128; i++) {
        if (ispunct(i)) {
            cout++;
        }
    }
    EXPECT_EQ("ispunct_0500", cout, 32);
}

int main(int argc, char *argv[])
{
    ispunct_0100();
    ispunct_0200();
    ispunct_0300();
    ispunct_0400();
    ispunct_0500();
    return t_status;
}