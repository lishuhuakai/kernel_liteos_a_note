/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include <assert.h>
#include <info/fatal_message.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#undef assert
#define assert(x) ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__),0)))

#define FILE_ABORT "abort.txt"
#define FILE_IGNORE "ignore.txt"
#define FILE_RETRY "retry.txt"
#define ASSERT_ABORT_INFO "ASSERT_ABORT"
#define ASSERT_IGNORE_INFO "ASSERT_IGNORE"
#define ASSERT_RETRY_INFO "ASSERT_RETRY"
#define MAX_FILE_SIZE 1024

void WriteToFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s\n", content);
        fclose(file);
    } else {
        perror("fopen");
    }
}

char* ReadFile(const char *filename)
{
    static char buffer[MAX_FILE_SIZE];
    FILE *file;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return NULL;
    }
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        perror("fgets");
        fclose(file);
        return NULL;
    }

    fclose(file);
    return buffer;
}

Assert_Status CallbackFunctionAbort(AssertFailureInfo assert_fail)
{
    char content[MAX_FILE_SIZE] = ASSERT_ABORT_INFO;
    WriteToFile(FILE_ABORT, content);
    Assert_Status res = ASSERT_ABORT;
    return res;
}

Assert_Status CallbackFunctionIgnore(AssertFailureInfo assert_fail)
{
    char content[MAX_FILE_SIZE] = ASSERT_IGNORE_INFO;
    WriteToFile(FILE_IGNORE, content);
    Assert_Status res = ASSERT_IGNORE;
    return res;
}

Assert_Status CallbackFunctionRetry(AssertFailureInfo assert_fail)
{
    char content[MAX_FILE_SIZE] = ASSERT_RETRY_INFO;
    WriteToFile(FILE_RETRY, content);
    Assert_Status res = ASSERT_RETRY;
    return res;
}

void ProcessAbort()
{
    set_assert_callback(CallbackFunctionAbort);
    assert(0);
}

void ProcessIgnore()
{
    set_assert_callback(CallbackFunctionIgnore);
    assert(0);
}

void ProcessRetry()
{
    set_assert_callback(CallbackFunctionRetry);
    assert(0);
}

int main(void)
{
    signal(SIGUSR1, SIG_DFL);
    pid_t pid1, pid2, pid3;
    int status;

    // Create the first child process
    pid1 = fork();
    if (pid1 < 0) {
        // Failed to create the process
        fprintf(stderr, "Fork failed for Process One\n");
        return 1;
    } else if (pid1 == 0) {
        // Child process one
        ProcessAbort(); // Specific operations for Child Process One can be added here
        return 0;
    }

    // Create the second child process
    pid2 = fork();
    if (pid2 < 0) {
        // Failed to create the process
        fprintf(stderr, "Fork failed for Process Two\n");
        return 1;
    } else if (pid2 == 0) {
        // Child process two
        ProcessIgnore(); // Specific operations for Child Process Two can be added here
        return 0;
    }

    // Create the third child process
    pid3 = fork();
    if (pid3 < 0) {
        // Failed to create the process
        fprintf(stderr, "Fork failed for Process Three\n");
        return 1;
    } else if (pid3 == 0) {
        // Child process three
        ProcessRetry(); // Specific operations for Child Process Three can be added here
        return 0;
    }

    // Wait for all child processes to finish
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    waitpid(pid3, &status, 0);

    char *BufferAbort = strdup(ReadFile(FILE_ABORT));
    char *BufferIgnore = strdup(ReadFile(FILE_IGNORE));
    char *BufferRetry = strdup(ReadFile(FILE_RETRY));

    if (strcmp(BufferAbort, ASSERT_ABORT_INFO) == 0 && strcmp(BufferIgnore, ASSERT_IGNORE_INFO) == 0
    && strcmp(BufferRetry, ASSERT_RETRY_INFO) == 0) {
        printf("All processes finished.\n");
    }

    free(BufferAbort);
    free(BufferIgnore);
    free(BufferRetry);

    return 0;
}