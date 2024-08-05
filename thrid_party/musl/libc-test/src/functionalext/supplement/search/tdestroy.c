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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <time.h>
#include "test.h"

#define ARRAY_SIZE (10)
static void *root = NULL;
static int g_free_calls = 0;

void *malloc_node(unsigned n)
{
    void *p;
    p = malloc(n);
    if (p) {
        return p;
    }
    fprintf(stderr, "insufficient memory\n");
    exit(EXIT_FAILURE);
}

int cmp_node(const void *pa, const void *pb)
{
    if (*(int *)pa < *(int *)pb) {
        return -1;
    }
    if (*(int *)pa > *(int *)pb) {
        return 1;
    }
    return 0;
}

void node_free(void *p)
{
    free(p);
    g_free_calls++;
}

/**
 * @tc.name      : tdestroy_0100
 * @tc.desc      : Removes the whole tree pointed to by root
 * @tc.level     : Level 0
 */
void tdestroy_0100(void)
{
    int i, *ptr, *target;
    void *val, *result;
    int array[ARRAY_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (i = 0; i < ARRAY_SIZE; i++) {
        ptr = malloc_node(sizeof(int));
        *ptr = array[i];
        val = tsearch((void *)ptr, &root, cmp_node);
        if (val == NULL) {
            exit(EXIT_FAILURE);

        } else if ((*(int **)val) != ptr) {
            free(ptr);
        }
    }

    tdestroy(root, node_free);
    if (g_free_calls != ARRAY_SIZE) {
        t_error("%s tdestroy failed, g_free_calls is %d\n", __func__, g_free_calls);
    }
}

int main(int argc, char *argv[])
{
    tdestroy_0100();
    return t_status;
}