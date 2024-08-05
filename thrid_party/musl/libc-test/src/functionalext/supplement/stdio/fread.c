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

#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t NUM_ZERO = 0;
const int32_t NUM_TEN = 10;
const int32_t NUM_TWENTY = 20;

/**
 * @tc.name      : fread_0100
 * @tc.desc      : Verify that the number of data read is less than the number of data in the file
 * @tc.level     : Level 0
 */
void fread_0100(void)
{
    char abc[100] = {0};
    const char *wrstring = "starttowritehelloworld";
    const char *ptr = "/data/Freadtest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0100", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_SET);

    size_t rsize = fread(abc, 1, 10, fptr);
    EXPECT_EQ("fread_0100", rsize, 10);
    EXPECT_STREQ("fread_0100", abc, "starttowri");

    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fread_0200
 * @tc.desc      : Verify that the number of data read is greater than the number of data in the file
 * @tc.level     : Level 1
 */
void fread_0200(void)
{
    char abc[100] = {0};
    const char *wrstring = "startwritehelloworld";
    const char *ptr = "/data/Freadtest.txt";

    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0200", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_SET);

    size_t rsize = fread(abc, 1, 25, fptr);
    EXPECT_EQ("fread_0200", rsize, 20);
    EXPECT_STREQ("fread_0200", abc, wrstring);

    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fread_0300
 * @tc.desc      : Verify that the number of data read is greater than the number of data in the file
 *                 (the file pointer is at the end)
 * @tc.level     : Level 2
 */
void fread_0300(void)
{
    char abc[100] = {0};
    const char *wrstring = "startwritehelloworld";
    const char *ptr = "/data/Freadtest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0200", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_END);
    size_t rsize = fread(abc, 1, 10, fptr);
    EXPECT_EQ("fread_0300", rsize, 0);

    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fread_0400
 * @tc.desc      : Verify that the size parameter is invalid and cannot read the specified file content
 * @tc.level     : Level 2
 */
void fread_0400(void)
{
    char abc[100] = {0};
    const char *wrstring = "startwritehelloworld";
    const char *ptr = "/data/Freadtest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0200", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_END);

    size_t rsize = fread(abc, 0, 10, fptr);
    EXPECT_EQ("fread_0400", rsize, 0);

    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fread_0500
 * @tc.desc      : Verify that the return value of syscall have been processed correctly
 * @tc.level     : Level 2
 */
#define FREAD_0500_BUFSZ (4097)
void fread_0500(void)
{
    pid_t pid = fork();
    if (pid == -1) {
    	perror("fread_0500 fork:");
    	exit(-1);
    }
    
    /* child */
    if (pid == 0) {
        /* make sure parent opening the status file */
    	sleep(1);
    	exit(-1);
    }
    
    char buf[FREAD_0500_BUFSZ] = {0};
    sprintf(buf, "/proc/%d/status", pid);
    FILE *fStatus = fopen(buf, "rb");
    EXPECT_PTRNE("fread_0500", fStatus, NULL);
    
    /* wait child exit, and status file of child will disappear */
    int status = 0;
    pid_t w = wait(&status);

    /* read >4K data from file, check if return correctly */
    size_t rsize = fread(buf, 1, FREAD_0500_BUFSZ, fStatus);
    EXPECT_EQ("fread_0500", rsize, 0);
    fclose(fStatus);
}

#define ARRAY_SIZE_1026 (1026)
#define BUFFER_SIZE_1026 (1026)
#define READ_LENGTH (20)
#define SEEK_OFFSET (1006)
#define BUFFER_SIZE_1024 (1024)
#define ZERO (0)

void fread_0600()
{
    char myArray[ARRAY_SIZE_1026] = {};
    memset(myArray, '3', ARRAY_SIZE_1026);
    myArray[ARRAY_SIZE_1026 - 2] = '1';
    myArray[ARRAY_SIZE_1026 - 1] = '2';

    // 使用fmemopen获取FILE*指针
    FILE *fp = fmemopen(myArray, sizeof(myArray), "r");
    if (fp == NULL) {
        t_error("fmemopen failed");
    }

    // 使用fread读取FILE*指针
    char *buffer = (char *)malloc((BUFFER_SIZE_1026 + 1) * sizeof(char));
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    char result1[READ_LENGTH + 1] = {};
    memset(result1, '3', READ_LENGTH);
    result1[READ_LENGTH] = '\0';

    char result2[READ_LENGTH + 1] = {};
    memset(result2, '3', READ_LENGTH);
    result2[READ_LENGTH - 2] = '1';
    result2[READ_LENGTH - 1] = '2';
    result2[READ_LENGTH] = '\0';

    char result3[BUFFER_SIZE_1024 + 1] = {};
    memset(result3, '3', BUFFER_SIZE_1024 * sizeof(char));
    result3[BUFFER_SIZE_1024] = '\0';

    // read 20 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    size_t bytesRead = fread(buffer, 1, READ_LENGTH, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0600 point 1", buffer, result1);

    // read 20 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, READ_LENGTH, 1, fp);
    buffer[READ_LENGTH] = '\0';
    EXPECT_STREQ("fread_0600 point 2", buffer, result1);

    // read 20 bytes from fp, start from 1006
    fseek(fp, SEEK_OFFSET, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, READ_LENGTH, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0600 point 3 from 1006", buffer, result2);

    // read 1024 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, BUFFER_SIZE_1024, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0600 point 4 read 1024", buffer, result3);

    // read 1026 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, BUFFER_SIZE_1026, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0600 point 5 read 1026", buffer, myArray);

    free(buffer);
    fclose(fp);
}

void fread_0700()
{
    char myArray[ARRAY_SIZE_1026] = {};
    memset(myArray, '3', ARRAY_SIZE_1026);
    myArray[ARRAY_SIZE_1026 - 2] = '1';
    myArray[ARRAY_SIZE_1026 - 1] = '2';

    // 使用fmemopen获取FILE*指针
    FILE *fp = fmemopen(myArray, sizeof(myArray), "r");
    if (fp == NULL) {
        t_error("fmemopen failed");
    }

    char *fpBuffer = (char *)malloc((BUFFER_SIZE_1026 + 1) * sizeof(char));
    int result = setvbuf(fp, fpBuffer, _IOFBF, BUFFER_SIZE_1026);
    if (result != ZERO) {
        t_error("fread_0700 servbuf failed errno=", errno);
    }

    // 使用fread读取FILE*指针
    char *buffer = (char *)malloc((BUFFER_SIZE_1026 + 1) * sizeof(char));
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    char result1[READ_LENGTH + 1] = {};
    memset(result1, '3', READ_LENGTH);
    result1[READ_LENGTH] = '\0';

    char result2[READ_LENGTH + 1] = {};
    memset(result2, '3', READ_LENGTH);
    result2[READ_LENGTH - 2] = '1';
    result2[READ_LENGTH - 1] = '2';
    result2[READ_LENGTH] = '\0';

    char result3[BUFFER_SIZE_1024 + 1] = {};
    memset(result3, '3', BUFFER_SIZE_1024 * sizeof(char));
    result3[BUFFER_SIZE_1024] = '\0';

    // read 20 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    size_t bytesRead = fread(buffer, 1, READ_LENGTH, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 1", buffer, result1);

    // read 20 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, READ_LENGTH, 1, fp);
    buffer[READ_LENGTH] = '\0';
    EXPECT_STREQ("fread_0700 point 2", buffer, result1);

    // read 20 bytes from fp, start from 1006
    fseek(fp, SEEK_OFFSET, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, READ_LENGTH, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 3 from 1006", buffer, result2);

    // read 1024 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, BUFFER_SIZE_1024, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 4 read 1024", buffer, result3);

    // read 1026 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, BUFFER_SIZE_1026, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 5 read 1026", buffer, myArray);

    free(buffer);
    fclose(fp);
    free(fpBuffer);
}

void fread_0800()
{
    char myArray[ARRAY_SIZE_1026] = {};
    memset(myArray, '3', ARRAY_SIZE_1026);
    myArray[ARRAY_SIZE_1026 - 2] = '1';
    myArray[ARRAY_SIZE_1026 - 1] = '2';

    // 使用fmemopen获取FILE*指针
    FILE *fp = fmemopen(myArray, sizeof(myArray), "r");
    if (fp == NULL) {
        t_error("fmemopen failed");
    }

    int result = setvbuf(fp, NULL, _IONBF, 0);
    if (result != ZERO) {
        t_error("fread_0700 servbuf failed errno=", errno);
    }

    // 使用fread读取FILE*指针
    char *buffer = (char *)malloc((BUFFER_SIZE_1026 + 1) * sizeof(char));
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    char result1[READ_LENGTH + 1] = {};
    memset(result1, '3', READ_LENGTH);
    result1[READ_LENGTH] = '\0';

    char result2[READ_LENGTH + 1] = {};
    memset(result2, '3', READ_LENGTH);
    result2[READ_LENGTH - 2] = '1';
    result2[READ_LENGTH - 1] = '2';
    result2[READ_LENGTH] = '\0';

    char result3[BUFFER_SIZE_1024 + 1] = {};
    memset(result3, '3', BUFFER_SIZE_1024 * sizeof(char));
    result3[BUFFER_SIZE_1024] = '\0';

    // read 20 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    size_t bytesRead = fread(buffer, 1, READ_LENGTH, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 1", buffer, result1);

    // read 20 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, READ_LENGTH, 1, fp);
    buffer[READ_LENGTH] = '\0';
    EXPECT_STREQ("fread_0700 point 2", buffer, result1);

    // read 20 bytes from fp, start from 1006
    fseek(fp, SEEK_OFFSET, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, READ_LENGTH, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 3 from 1006", buffer, result2);

    // read 1024 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, BUFFER_SIZE_1024, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 4 read 1024", buffer, result3);

    // read 1026 bytes from fp
    fseek(fp, ZERO, SEEK_SET);
    memset(buffer, ZERO, BUFFER_SIZE_1026 * sizeof(char));
    bytesRead = fread(buffer, 1, BUFFER_SIZE_1026, fp);
    buffer[bytesRead] = '\0';
    EXPECT_STREQ("fread_0700 point 5 read 1026", buffer, myArray);

    free(buffer);
    fclose(fp);
}

TEST_FUN G_Fun_Array[] = {
    fread_0100,
    fread_0200,
    fread_0300,
    fread_0400,
    fread_0500,
    fread_0600,
    fread_0700,
    fread_0800,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
