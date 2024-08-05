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

#include "functionalext.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

const char *path = "/data/test.txt";

/**
 * @tc.name      : fwrite_0100
 * @tc.desc      : File pointer at the end of a file, the position to which the current file pointer points
 * @tc.level     : Level 0
 */
void fwrite_0100(void)
{
    FILE *fptr = fopen(path, "w+");
    EXPECT_PTRNE("fwrite_0100", fptr, NULL);

    char buf[] = "this is test";
    int result = fwrite(buf, sizeof(char), strlen(buf), fptr);
    EXPECT_TRUE("fwrite_0100", result == strlen(buf));

    fclose(fptr);
    remove(path);
}

/**
 * @tc.name      : fwrite_0200
 * @tc.desc      : File pointer at the beginning of a file, the location to which the current file pointer points
 * @tc.level     : Level 0
 */
void fwrite_0200(void)
{
    FILE *fptr = fopen(path, "w+");
    EXPECT_PTRNE("fwrite_0200", fptr, NULL);

    char buf[] = "this is test";
    int result = fwrite(buf, 0, strlen(buf), fptr);
    EXPECT_EQ("fwrite_0200", result, 0);

    fclose(fptr);
    remove(path);
}

/**
 * @tc.name      : fwrite_0300
 * @tc.desc      : Invalid argument. Cannot get the location of the current file pointer
 * @tc.level     : Level 2
 */
void fwrite_0300(void)
{
    FILE *fptr = fopen(path, "w+");
    EXPECT_PTRNE("fwrite_0300", fptr, NULL);

    char buf[] = "this is test";
    int result = fwrite(buf, sizeof(char), 0, fptr);
    EXPECT_EQ("fwrite_0300", result, 0);

    fclose(fptr);
    remove(path);
}

void fwrite_0400(void)
{
    pid_t childPid = 0;
    int fds[2] = {0};
    pipe(fds);
    int pipeRead = 0;
    int pipeWrite = 1;

    char buf[1024] = {0};

    childPid = fork();
    EXPECT_NE("fwrite_0400", childPid, -1);
    if (childPid == 0) {
        // childr
        dup2(fds[pipeWrite], STDOUT_FILENO);
        dup2(fds[pipeRead], STDIN_FILENO);

        close(fds[pipeWrite]);
        close(fds[pipeRead]);

        // exec
        execl("/bin/sh", "/bin/sh", "-c", "/system/bin/bm get -u", NULL);
        exit(0);
    } else {
        // parent
        close(fds[pipeWrite]);
        fcntl(fds[pipeRead], F_SETFD, F_DUPFD_CLOEXEC);
        int cn = read(fds[pipeRead], buf, sizeof(buf));
        EXPECT_MT("fwrite_0400", cn, 0);
    }
}

int main(int argc, char *argv[])
{
    fwrite_0100();
    fwrite_0200();
    fwrite_0300();
    return t_status;
}
