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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <wchar.h>
#include "test.h"
#include <sys/wait.h>
#include "functionalext.h"

void deal_aberrant(int code)
{
    if (code != SIGSEGV) {
        t_error("putw_0200 code is %d are not SIGSEGV", __func__, code);
    }
    exit(t_status);
}

/**
 * @tc.name      : putw_0100
 * @tc.desc      : Test the putw method to write integers to the file
 * @tc.level     : Level 0
 */
void putw_0100(void)
{
    FILE *fp;
    char *file = "putw_test.txt";
    fp = fopen(file, "w+");
    if (fp == NULL) {
        t_error("%s fopen failed\n", __func__);
        return;
    }
    int result = putw(10, fp);
    if (result != 0) {
        t_error("%s putw error get result is %d are not want 0\n", __func__, result);
    }
    fclose(fp);
    remove(file);
}

/**
 * @tc.name      : putw_0200
 * @tc.desc      : Test the result of putw when the incoming file pointer is empty
 * @tc.level     : Level 2
 */
void putw_0200(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("putw_0200: Error forking process");
    } else if (pid == 0) {
        FILE *fp = NULL;
        putw(10, fp);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("putw_0200", SIGABRT, sig);
        }
    }
}

int main(int argc, char *argv[])
{
    putw_0100();
    putw_0200();
    return t_status;
}