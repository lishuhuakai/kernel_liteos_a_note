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

#include <arpa/inet.h>
#include "functionalext.h"

static const uint16_t LE = 0x1234;
static const uint16_t BE = 0x3412;

/**
 * @tc.name      : htons_0100
 * @tc.desc      : Verify htons process success.
 * @tc.level     : Level 0
 */
void htons_0100(void)
{
    EXPECT_EQ("htonls_0100", BE, htons(LE));
}

int main(void)
{
    htons_0100();
    return t_status;
}
