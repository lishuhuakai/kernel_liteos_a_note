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

#include <netdb.h>
#include "functionalext.h"

#define TEST_MSG_COUNT 5

/**
 * @tc.name      : hstrerror_0100
 * @tc.desc      : Get error information from error code
 * @tc.level     : Level 0
 */
void hstrerror_0100(void)
{
    const char *msg[TEST_MSG_COUNT] = {
        "Unknown error", "Host not found", "Try again", "Non-recoverable error", "Address not available"};

    int i;
    for (i = 0; i < TEST_MSG_COUNT; i++) {
        const char *ret = hstrerror(i);
        EXPECT_PTRNE("hstrerror_0100", ret, NULL);
        if (ret) {
            EXPECT_STREQ("hstrerror_0100", ret, msg[i]);
        }
    }
}

int main(void)
{
    hstrerror_0100();
    return t_status;
}