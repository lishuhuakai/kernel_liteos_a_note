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

#include <sys/mman.h>
#include <sys/stat.h>
#include "functionalext.h"

/*
 * @tc.name      : munmap_0100
 * @tc.desc      : Verify that the parameters are valid to unmap the memory
 * @tc.level     : Level 0
 */
void munmap_0100(void)
{
    const char *ptr = "/data/test.txt";
    const char str[] = "this is a sample!";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("munmap_0100", fptr, NULL);

    struct stat statbuff;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    char *p_map = mmap(NULL, sizeof(char) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(fptr), 0);
    fclose(fptr);
    int data = munmap(p_map, sizeof(char) * 10);
    EXPECT_EQ("munmap_0100", data, 0);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    munmap_0100();
    return t_status;
}