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

#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "functionalext.h"

const int SUCCESS = 0;

extern int __gettimeofday_time64(struct timeval *__restrict, void *__restrict);

/**
 * @tc.name      : gettimeofday_0100
 * @tc.desc      : Each parameter is valid, and the current system time can be obtained.
 * @tc.level     : Level 0
 */
void gettimeofday_0100(void)
{
    struct timeval tv;
    char str[11];
    system("date +%s > ./time.txt");
    FILE *fptr = fopen("time.txt", "r");
    fflush(fptr);
    fread(str, sizeof(str), 1, fptr);
    str[10] = '\0';
    
    int sec = atoi(str);
    int returnflag = gettimeofday(&tv, NULL);
    remove("time.txt");
    EXPECT_EQ("gettimeofday_0100", returnflag, SUCCESS);
    EXPECT_EQ("gettimeofday_0100",(sec == tv.tv_sec || (sec+1) == tv.tv_sec),true);
}

/**
 * @tc.name      : gettimeofday_0200
 * @tc.desc      : Each parameter is invalid, and the current system time can be obtained.
 * @tc.level     : Level 2
 */
void gettimeofday_0200(void)
{
    int returnflag = gettimeofday(NULL, NULL);
    EXPECT_EQ("gettimeofday_0200", returnflag, SUCCESS);
}

/**
 * @tc.name      : gettimeofday_time64_0100
 * @tc.desc      : Each parameter is valid, and the current system time can be obtained.
 * @tc.level     : Level 0
 */
void gettimeofday_time64_0100(void)
{
    struct timeval tv;
    char str[11];
    system("date +%s > ./time.txt");
    FILE *fptr = fopen("time.txt", "r");
    fflush(fptr);
    fread(str, sizeof(str), 1, fptr);
    str[10] = '\0';
    
    int sec = atoi(str);
    int returnflag = __gettimeofday_time64(&tv, NULL);
    remove("time.txt");
    EXPECT_EQ("gettimeofday_time64_0100", returnflag, SUCCESS);
    EXPECT_EQ("gettimeofday_time64_0100", (sec == tv.tv_sec || (sec+1) == tv.tv_sec),true);
}

int main(int argc, char *argv[])
{
    gettimeofday_0100();
    gettimeofday_0200();
    gettimeofday_time64_0100();
    return t_status;
}