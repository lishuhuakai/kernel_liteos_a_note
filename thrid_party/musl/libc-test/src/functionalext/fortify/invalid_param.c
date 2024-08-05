/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include "stdio.h"
#include <pthread.h>
#include <limits.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <dirent.h>
#include <utime.h>
#include "functionalext.h"

int compare(const void *p, const void *q)
{
    return (*(int *)p - *(int *)q);
}
/**
 * @tc.name      : clearerr_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void clearerr_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("clearerr_0100: Error forking process");
    } else if (pid == 0) {
        clearerr(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("clearerr_0200", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : feof_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void feof_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("feof_0100: Error forking process");
    } else if (pid == 0) {
        feof(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("feof_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : ferror_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void ferror_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("ferror_0100: Error forking process");
    } else if (pid == 0) {
        ferror(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("ferror_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fgetc_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fgetc_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fgetc_0100: Error forking process");
    } else if (pid == 0) {
        fgetc(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fgetc_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fgets_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fgets_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fgets_0100: Error forking process");
    } else if (pid == 0) {
        char buffer[32];
        fgets(buffer, sizeof(buffer), NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fgets_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fprintf_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fprintf_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fprintf_0100: Error forking process");
    } else if (pid == 0) {
        char str[] = "test";
        fprintf(NULL, "%s", str);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fprintf_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fputc_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fputc_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fputc_0100: Error forking process");
    } else if (pid == 0) {
        fputc('a', NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fputc_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fputs_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fputs_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fputs_0100: Error forking process");
    } else if (pid == 0) {
        fputs("", NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fputs_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fread_0600
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fread_0600(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fread_0600: Error forking process");
    } else if (pid == 0) {
        char buffer[1024];
        fread(buffer, sizeof(char), sizeof(buffer), NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fread_0600", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fscanf_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fscanf_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fscanf_0100: Error forking process");
    } else if (pid == 0) {
        char buffer[1024];
        fscanf(NULL, "%s", buffer);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fscanf_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fseek_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fseek_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fseek_0100: Error forking process");
    } else if (pid == 0) {
        char buffer[1024];
        fseek(NULL, 0L, SEEK_END);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fseek_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fseeko_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fseeko_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fseeko_0100: Error forking process");
    } else if (pid == 0) {
        fseeko(NULL, 0L, SEEK_END);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fseeko_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : ftell_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void ftell_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("ftell_0100: Error forking process");
    } else if (pid == 0) {
        ftell(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("ftell_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : fwrite_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void fwrite_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("fwrite_0100: Error forking process");
    } else if (pid == 0) {
        char buf[] = "this is test";
        fwrite(buf, sizeof(char), strlen(buf), NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("fwrite_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : getc_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void getc_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("getc_0100: Error forking process");
    } else if (pid == 0) {
        getc(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("getc_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pclose_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pclose_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pclose_0100: Error forking process");
    } else if (pid == 0) {
        pclose(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pclose_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : rewind_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void rewind_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("rewind_0100: Error forking process");
    } else if (pid == 0) {
        rewind(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("rewind_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : setvbuf_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void setvbuf_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("setvbuf_0100: Error forking process");
    } else if (pid == 0) {
        char buff[1024] = {0};
        setvbuf(NULL, buff, -1, 1024);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("setvbuf_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : wcsnrtombs_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void wcsnrtombs_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("wcsnrtombs_0100: Error forking process");
    } else if (pid == 0) {
        char buffer[32];
        mbstate_t mbs;
        size_t wn = 4;
        size_t n = 4;
        mbrlen(NULL, 0, &mbs);
        memset(buffer, 0, sizeof(buffer));
        wcsnrtombs(buffer, NULL, wn, n, &mbs);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("wcsnrtombs_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : unsetenv_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void unsetenv_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("unsetenv_0100: Error forking process");
    } else if (pid == 0) {
        unsetenv(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("unsetenv_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : readdir_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void readdir_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("readdir_0100: Error forking process");
    } else if (pid == 0) {
        readdir(NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("readdir_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pthread_join_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pthread_join_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pthread_join_0100: Error forking process");
    } else if (pid == 0) {
        pthread_t invalid_thread = 0;
        void *res;
        pthread_join(invalid_thread, &res);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pthread_join_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pthread_kill_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pthread_kill_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pthread_kill_0100: Error forking process");
    } else if (pid == 0) {
        pthread_t invalid_thread = 0;
        int valid_sig = SIGTERM;
        pthread_kill(invalid_thread, valid_sig);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pthread_kill_0100", SIGABRT, sig);
        }
    }
}

/**
 * @tc.name      : pthread_setschedparam_0100
 * @tc.desc      : Verify clearerr with invalid parameter
 * @tc.level     : Level 2
 */
void pthread_setschedparam_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("pthread_setschedparam_0100: Error forking process");
    } else if (pid == 0) {
        pthread_t invalid_thread = 0;
        struct sched_param param;
        param.sched_priority = 1;
        pthread_setschedparam(invalid_thread, SCHED_FIFO, &param);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("pthread_setschedparam_0100", SIGABRT, sig);
        }
    }
}

int main(int argc, char *argv[])
{
    clearerr_0100();
    feof_0100();
    ferror_0100();
    fgetc_0100();
    fgets_0100();
    fprintf_0100();
    fputc_0100();
    fputs_0100();
    fread_0600();
    fscanf_0100();
    fseek_0100();
    fseeko_0100();
    ftell_0100();
    fwrite_0100();
    getc_0100();
    pclose_0100();
    rewind_0100();
    setvbuf_0100();

    wcsnrtombs_0100();
    unsetenv_0100();
    readdir_0100();
    pthread_join_0100();
    pthread_kill_0100();
    pthread_setschedparam_0100();
    return t_status;
}