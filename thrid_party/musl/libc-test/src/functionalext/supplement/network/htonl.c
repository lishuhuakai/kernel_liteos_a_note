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

const uint32_t IP = 0x7f000001;

/**
 * @tc.name      : htonl_0100
 * @tc.desc      : Verify htonl process success
 * @tc.level     : Level 0
 */
void htonl_0100(void)
{
    EXPECT_EQ("htonl_0100", htonl(IP), inet_addr("127.0.0.1"));
}

int main(void)
{
    htonl_0100();
    return t_status;
}
