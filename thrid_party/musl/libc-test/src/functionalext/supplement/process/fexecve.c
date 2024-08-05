/*
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

#include <fcntl.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : fexecve_0100
 * @tc.desc      : Each parameter is valid, the fexecve function can execute the specified file.
 * @tc.level     : Level 0
 */
void fexecve_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"./fexecverely", NULL};
        char *environ[] = {0, NULL};
        int fd = open("fexecverely", O_RDONLY);
        int ret = fexecve(fd, argv, environ);
        EXPECT_NE("fexecve_0100", ret, -1);
    }
}

/**
 * @tc.name      : fexecve_0200
 * @tc.desc      : The fd parameter is invalid (NULL), the fexecve function cannot execute the specified file.
 * @tc.level     : Level 2
 */
void fexecve_0200(void)
{
    char *my_env[] = {NULL};
    char *argv[] = {"touch", "fexecvetest.txt", NULL};
    int ret = fexecve(-1, argv, my_env);
    EXPECT_EQ("fexecve_0200", ret, -1);
}

int main(int argc, char *argv[])
{
    fexecve_0100();
    fexecve_0200();
    return t_status;
}