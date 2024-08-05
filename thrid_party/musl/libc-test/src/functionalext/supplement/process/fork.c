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

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "functionalext.h"

/**
 * @tc.name      : fork_0100
 * @tc.desc      : The function call is successful and the process can be created
 * @tc.level     : Level 0
 */
void fork_0100()
{
    pid_t fpid;
    int count = 0;
    int options = 0;
    const char *sign_r = "1";
    const char *sign_f = "0";
    char list1[2];
    char list2[2];

    fpid = fork();
    if (fpid < 0) {
        t_error("%s error in fork!", __func__);
    } else if (fpid == 0) {
        FILE* fp = fopen("test1.txt", "w+");
        if (!fp) {
            t_error("failed to open test1.txt for writing");
        }
        fwrite(sign_r, sizeof(char), strlen(sign_r), fp);
        fclose(fp);
        _exit(EXIT_SUCCESS);
    } else {
        FILE* fp = fopen("test2.txt", "w+");
        if (!fp) {
            t_error("failed to open test2.txt for writing");
        }
        fwrite(sign_r, sizeof(char), strlen(sign_r), fp);
        fclose(fp);
        waitpid(fpid, NULL, options);
	
        FILE *fp1 = fopen("test1.txt", "r");
        if (!fp1) {
            t_error("failed to open test1.txt for reading");
        }
        FILE *fp2 = fopen("test2.txt", "r");
        if (!fp2) {
            t_error("failed to open test2.txt for reading");
        }
        fread(list1, sizeof(list1), 1, fp1);
        fread(list2, sizeof(list2), 1, fp2);
        EXPECT_EQ("fork_0100", list1[0], '1');
        EXPECT_EQ("fork_0100", list2[0], '1');

        fclose(fp1);
        fclose(fp2);
        remove("test1.txt");
        remove("test2.txt");
    }
}

int main(int argc, char *argv[])
{
    fork_0100();
    return t_status;
}
