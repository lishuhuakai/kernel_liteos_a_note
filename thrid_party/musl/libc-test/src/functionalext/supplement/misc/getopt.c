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

#include <stdio.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : getopt_0100
 * @tc.desc      : Each parameter is valid, argv is an existing command line parameter, and the analysis of the
 *                 command line parameter is successful
 * @tc.level     : Level 0
 */
void getopt_0100(void)
{
    int argc = 2;
    char *argv[] = {"./getopt", "-b"};
    char *optstring = "a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != 'b') {
        t_error("%s getopt get result is '%c' are not want 'b'\n", __func__, result);
    }
}

/**
 * @tc.name      : getopt_0200
 * @tc.desc      : Each parameter is valid, argv is an existing command line parameter with a colon, and the analysis
 *                 of the command line parameter is failed
 * @tc.level     : Level 0
 */
void getopt_0200(void)
{
    int argc = 2;
    char *argv[] = {"./getopt", "-a12345"};
    char *optstring = "a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != -1) {
        t_error("%s getopt get result is %d are not want -1\n", __func__, result);
    }
}

/**
 * @tc.name      : getopt_0300
 * @tc.desc      : The argv parameter is invalid without a colon, and the parsing of the command line parameters failed
 * @tc.level     : Level 2
 */
void getopt_0300(void)
{
    int argc = 2;
    char *argv[] = {"./getopt", "-a"};
    char *optstring = "a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != -1) {
        t_error("%s getopt get result is %d are not want -1\n", __func__, result);
    }
}

/**
 * @tc.name      : getopt_0400
 * @tc.desc      : The argv parameter is invalid, the command line parameter does not exist, and the parsing of the
 *                 command line parameter failed
 * @tc.level     : Level 2
 */
void getopt_0400(void)
{
    int argc = 2;
    char *argv[] = {"./getopt", "-f"};
    char *optstring = "a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != -1) {
        t_error("%s getopt get result is %d are not want -1\n", __func__, result);
    }
}

/**
 * @tc.name      : getopt_0500
 * @tc.desc      : The argv argument is invalid, argv is NULL, and parsing command line arguments failed
 * @tc.level     : Level 2
 */
void getopt_0500(void)
{
    int argc = 2;
    char *argv[] = {"./getopt", NULL};
    char *optstring = "a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != -1) {
        t_error("%s getopt get result is %d are not want -1\n", __func__, result);
    }
}

/**
 * @tc.name      : getopt_0600
 * @tc.desc      : Invalid argv argument, argv is -, failed to parse command line arguments
 * @tc.level     : Level 2
 */
void getopt_0600(void)
{
    int argc = 2;
    char *argv[] = {"./getopt", "-"};
    char *optstring = "a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != -1) {
        t_error("%s getopt get result is %d are not want -1\n", __func__, result);
    }
}

/**
 * @tc.name      : getopt_0700
 * @tc.desc      : argv argument is valid, optstring starts with -, parsing command line arguments fails
 * @tc.level     : Level 2
 */
void getopt_0700(void)
{
    int argc = 2;
    char *argv[] = {"./getopt", "b"};
    char *optstring = "-a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != -1) {
        t_error("%s getopt get result is %d are not want -1\n", __func__, result);
    }
}

/**
 * @tc.name      : getopt_0800
 * @tc.desc      : Invalid argc argument, failed to parse command line arguments
 * @tc.level     : Level 2
 */
void getopt_0800(void)
{
    int argc = 0;
    char *argv[] = {"./getopt", "-b"};
    char *optstring = "a:bcde";
    int result = getopt(argc, argv, optstring);
    if (result != -1) {
        t_error("%s getopt get result is %d are not want -1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    getopt_0100();
    getopt_0200();
    getopt_0300();
    getopt_0400();
    getopt_0500();
    getopt_0600();
    getopt_0700();
    getopt_0800();
    return t_status;
}