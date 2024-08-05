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
#include "test.h"
#include "ld_log.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

/**
 * @tc.name      : ld_log_test_0010
 * @tc.desc      : test ld_log_reset normal condition.
 * @tc.level     : Level 0
 */
static void ld_log_test_0010(void)
{
    ld_log_reset();
    EXPECT_TRUE(__FUNCTION__, true);
}

TEST_FUN G_Fun_Array[] = {
    ld_log_test_0010,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}