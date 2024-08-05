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

#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
#define SLEEP_TIME 6
int time_out = 0;
void handler()
{
    time_out = 1;
}

/**
 * @tc.name      : alarm_0100
 * @tc.desc      : Set the alarm time for the process.
 * @tc.level     : Level 0
 */
void alarm_0100()
{
    signal(SIGALRM, handler);
    unsigned int result = alarm(SLEEP_TIME);
    EXPECT_EQ("alarm_0100", result, 0);
}

TEST_FUN G_Fun_Array[] = {
    alarm_0100,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}