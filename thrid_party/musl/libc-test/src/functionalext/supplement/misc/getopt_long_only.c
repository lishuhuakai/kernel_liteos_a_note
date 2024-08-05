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

#include <getopt.h>
#include "functionalext.h"

const int32_t FAILED = -1;

/**
 * @tc.name      : getopt_long_only_0100
 * @tc.desc      : Each parameter is valid, and the short option command is first parsed with the long parameter.
 * @tc.level     : Level 0
 */
void getopt_long_only_0100(int a)
{
    int c = a;
    EXPECT_NE("getopt_long_only_0100", c, FAILED);
}

/**
 * @tc.name      : getopt_long_only_0200
 * @tc.desc      : Invalid argument, command line parsing failed.
 * @tc.level     : Level 2
 */
void getopt_long_only_0200(int b)
{
    int c = b;
    EXPECT_EQ("getopt_long_only_0200", c, FAILED);
}

int main(int argc, char *argv[])
{
    argc = 2;
    argv[1] = "-n";
    char *l_opt_arg;
    char *const short_options = "nbl:";
    struct option long_options[] = {
        {"name", 0, NULL, 'n'},
        {"bf_name", 0, NULL, 'b'},
        {"love", 1, NULL, 'l'},
        {0, 0, 0, 0},
    };
    int c;
    c = getopt_long_only(argc, argv, short_options, long_options, NULL);
    getopt_long_only_0100(c);
    c = getopt_long_only(0, 0, 0, 0, NULL);
    getopt_long_only_0200(c);

    return t_status;
}