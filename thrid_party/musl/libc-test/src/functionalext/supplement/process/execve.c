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

#include "functionalext.h"

char *my_env[] = {0, NULL};

/**
 * @tc.name      : execve_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execve_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvetest.txt", NULL};
        execve("/bin/touch", argv, my_env);
    }
    sleep(1);
    int isExist = access("execvetest.txt", F_OK);
    EXPECT_EQ("execve_0100", isExist, 0);
    remove("execvetest.txt");
}

/**
 * @tc.name      : execve_0200
 * @tc.desc      : The content pointed to by the path parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execve_0200(void)
{
    char *argv[] = {"touch", "execvetest.txt", NULL};
    int ret = execve(" ", argv, my_env);
    EXPECT_EQ("execve_0200", ret, -1);
}

/**
 * @tc.name      : execve_0300
 * @tc.desc      : The length of path exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execve_0300(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    char *argv[] = {"touch", "execvetest.txt", NULL};
    int ret = execve(buff, argv, my_env);
    EXPECT_EQ("execve_0300", ret, -1);
}

int main(int argc, char *argv[])
{
    execve_0100();
    execve_0200();
    execve_0300();
    return t_status;
}