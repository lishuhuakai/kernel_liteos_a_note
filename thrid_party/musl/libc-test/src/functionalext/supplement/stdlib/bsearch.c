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
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include "functionalext.h"

#define NUM 8
int compare(const void *p, const void *q)
{
    return (*(int *)p - *(int *)q);
}

/**
 * @tc.name      : bsearch_0100
 * @tc.desc      : Verify that binary search can be achieved (each parameter is valid)
 * @tc.level     : Level 0
 */
void bsearch_0100(void)
{
    int array[NUM] = {9, 2, 7, 11, 3, 87, 34, 6};
    int key = 3;
    int *p;
    qsort(array, NUM, sizeof(int), compare);
    p = (int *)bsearch(&key, array, NUM, sizeof(int), compare);
    EXPECT_EQ("bsearch_0100", (*p), key);
}

/**
 * @tc.name      : bsearch_0200
 * @tc.desc      : Binary lookup cannot be implemented in validation (invalid parameters)
 * @tc.level     : Level 2
 */
void bsearch_0200(void)
{
    int array[NUM] = {9, 2, 7, 11, 3, 87, 34, 6};
    int key = 3;
    int *p;
    qsort(array, NUM, sizeof(int), compare);
    p = (int *)bsearch(&key, array, 0, sizeof(int), compare);
    EXPECT_TRUE("bsearch_0200", p == NULL);
}

int main(int argc, char *argv[])
{
    bsearch_0100();
    bsearch_0200();
    return t_status;
}