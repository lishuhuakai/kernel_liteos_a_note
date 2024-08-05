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

#include <stdlib.h>
#include <threads.h>
#include "functionalext.h"

static char list[100];
static char buf[12] = "called once";
void do_once(void)
{
    FILE *fp = fopen("call_once.txt", "a+");
    fwrite(buf, sizeof(char), strlen(buf), fp);
    fclose(fp);
    FILE *fp2 = fopen("call_once.txt", "r");
    fread(list, sizeof(list), 1, fp2);
    fclose(fp2);
}
static once_flag flag = ONCE_FLAG_INIT;
void func(void *data)
{
    call_once(&flag, do_once);
}

/**
 * @tc.name      : call_once_0100
 * @tc.desc      : Each parameter is valid, and the function func is called only once.
 * @tc.level     : Level 0
 */
void call_once_0100(void)
{
    EXPECT_EQ("call_once_0100", strcmp(list, buf), 0);
}

int main(void)
{
    thrd_t t1, t2, t3, t4;
    thrd_create(&t1, (thrd_start_t)func, NULL);
    thrd_create(&t2, (thrd_start_t)func, NULL);
    thrd_create(&t3, (thrd_start_t)func, NULL);
    thrd_create(&t4, (thrd_start_t)func, NULL);
    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
    thrd_join(t3, NULL);
    thrd_join(t4, NULL);
    call_once_0100();
    remove("call_once.txt");
    memset(list, 0x00, sizeof(list));
    return t_status;
}