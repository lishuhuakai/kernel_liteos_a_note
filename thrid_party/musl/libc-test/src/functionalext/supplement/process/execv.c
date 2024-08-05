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

/**
 * @tc.name      : execv_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execv_0100(void)
{
    pid_t fpid;
    fpid = fork();
    FILE *fp;
    if (fpid == 0) {
        char *argv[] = {"touch", "touch", "execvtest.txt", NULL};
        int ret = execv("/bin/touch", argv);
        EXPECT_NE("execv_0100", ret, -1);
    }
    sleep(1);
    int isExist = access("execvtest.txt", F_OK);
    EXPECT_EQ("execv_0100", isExist, 0);
    remove("execvtest.txt");
}

/**
 * @tc.name      : execv_0200
 * @tc.desc      : The content pointed to by the path parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execv_0200(void)
{
    char *argv[] = {"touch", "touch", "execvtest.txt", NULL};
    int ret = execv(" ", argv);
    EXPECT_TRUE("execv_0200", ret == -1);
}

/**
 * @tc.name      : execv_0300
 * @tc.desc      : The length of path exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execv_0300(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    char *argv[] = {"touch", "touch", "execvtest.txt", NULL};
    int ret = execv(buff, argv);
    EXPECT_TRUE("execv_0300", ret == -1);
}

int main(int argc, char *argv[])
{
    execv_0100();
    execv_0200();
    execv_0300();
    return t_status;
}