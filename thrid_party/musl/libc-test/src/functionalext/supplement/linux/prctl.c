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

#include <sys/prctl.h>
#include "functionalext.h"

/*
 * @tc.name      : prctl_0100
 * @tc.desc      : Verify that the thread can be given a name and get the name
 * @tc.level     : Level 0
 */
void prctl_0100(void)
{
    char buf[BUFSIZ];
    memset(buf, 0, BUFSIZ);

    int result = prctl(PR_SET_NAME, (unsigned long)"TestThread");
    EXPECT_EQ("prctl_0100", result, 0);

    result = prctl(PR_GET_NAME, (unsigned long)buf);
    EXPECT_EQ("prctl_0100", result, 0);
    EXPECT_STREQ("prctl_0100", buf, "TestThread");
}

int main(int argc, char *argv[])
{
    prctl_0100();
    return t_status;
}