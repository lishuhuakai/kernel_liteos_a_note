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

#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : execvp_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execvp_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvptest.txt", NULL};
        execvp("touch", argv);
    }
    sleep(1);
    int isExist = access("execvptest.txt", F_OK);
    EXPECT_EQ("execvp_0100", isExist, 0);
    if (isExist == 0) {
        remove("execvptest.txt");
    }
}

/**
 * @tc.name      : execvp_0200
 * @tc.desc      : Each parameter is valid, the PATH is empty, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execvp_0200(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        char *argv[] = {"touch", "execvptest.txt", NULL};
        char *buff = getenv("ls");
        setenv("ls", " ", 1);
        execvp("touch", argv);
        setenv("ls", buff, 1);
    }
    sleep(1);
    int isExist = access("execvptest.txt", F_OK);
    EXPECT_EQ("execvp_0200", isExist, 0);
    if (isExist == 0) {
        remove("execvptest.txt");
    }
}

/**
 * @tc.name      : execvp_0300
 * @tc.desc      : The content pointed to by the path parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execvp_0300(void)
{
    char *argv[] = {"touch", "execvptest.txt", NULL};
    int ret = execvp(" ", argv);
    EXPECT_EQ("execvp_0300", ret, -1);
}

/**
 * @tc.name      : execvp_0400
 * @tc.desc      : The length of path exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execvp_0400(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    char *argv[] = {"touch", "execvptest.txt", NULL};
    int ret = execvp(buff, argv);
    EXPECT_EQ("execvp_0400", ret, -1);
}

int main(int argc, char *argv[])
{
    execvp_0100();
    execvp_0200();
    execvp_0300();
    execvp_0400();
    return t_status;
}