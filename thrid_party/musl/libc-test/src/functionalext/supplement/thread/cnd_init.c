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

#include <threads.h>
#include "functionalext.h"

/**
 * @tc.name      : cnd_init_0100
 * @tc.desc      : Verify that a new condition variable can be initialized
 * @tc.level     : Level 0
 */
void cnd_init_0100(void)
{
    cnd_t buf;
    int result = cnd_init(&buf);
    EXPECT_EQ("cnd_init_0100", result, thrd_success);
}

int main(int argc, char *argv[])
{
    cnd_init_0100();
    return t_status;
}