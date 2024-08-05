/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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
#include <stdlib.h>
#include "functionalext.h"

#define BUFFER_SIZE 1024

void sprintf_specifier_tests(void)
{
    char str[BUFFER_SIZE];
    int ret = 0;

    ret = sprintf(str, "test");
    EXPECT_EQ("sprintf_specifier_null", ret, 4);
    EXPECT_STREQ("sprintf_specifier_null", str, "test");

    ret = sprintf(str, "%d,%i", 1, 2);
    EXPECT_EQ("sprintf_specifier_integer", ret, 3);
    EXPECT_STREQ("sprintf_specifier_integer", str, "1,2");

    ret = sprintf(str, "%u,%u", 1, -1);
    EXPECT_EQ("sprintf_specifier_Unsigned_integer", ret, 12);
    EXPECT_STREQ("sprintf_specifier_Unsigned_integer", str, "1,4294967295");

    ret = sprintf(str, "%o", 9);
    EXPECT_EQ("sprintf_specifier_octal_integer", ret, 2);
    EXPECT_STREQ("sprintf_specifier_octal_integer", str, "11");

    ret = sprintf(str, "%x,%X", 15, 15);
    EXPECT_EQ("sprintf_specifier_hexadecimal_integer", ret, 3);
    EXPECT_STREQ("sprintf_specifier_hexadecimal_integer", str, "f,F");

    ret = sprintf(str, "%f,%F", 1.23, 1.23);
    EXPECT_EQ("sprintf_specifier_float", ret, 17);
    EXPECT_STREQ("sprintf_specifier_float", str, "1.230000,1.230000");

    ret = sprintf(str, "%e,%E", 1.23, 1.23);
    EXPECT_EQ("sprintf_specifier_scientific_notation", ret, 25);
    EXPECT_STREQ("sprintf_specifier_scientific_notation", str, "1.230000e+00,1.230000E+00");

    ret = sprintf(str, "%c", '1');
    EXPECT_EQ("sprintf_specifier_char", ret, 1);
    EXPECT_STREQ("sprintf_specifier_char", str, "1");

    ret = sprintf(str, "%s,%s", "123", "456");
    EXPECT_EQ("sprintf_specifier_str", ret, 7);
    EXPECT_STREQ("sprintf_specifier_str", str, "123,456");

    ret = sprintf(str, "%p", 6618688);
    EXPECT_EQ("sprintf_specifier_pointer_address", ret, 8);
    EXPECT_STREQ("sprintf_specifier_pointer_address", str, "0x64fe40");

    ret = sprintf(str, "%%test");
    EXPECT_EQ("sprintf_specifier_double_percent_sign", ret, 5);
    EXPECT_STREQ("sprintf_specifier_double_percent_sign", str, "%test");

    ret = sprintf(str, "%3$d,%2$s,%1$c", 'a', "bc", 123);
    EXPECT_EQ("sprintf_$_symbol", ret, 8);
    EXPECT_STREQ("sprintf_$_symbol", str, "123,bc,a");
}

void sprintf_flag_tests(void)
{
    char str[BUFFER_SIZE];
    int ret = 0;

    ret = sprintf(str, "%+d %+d %d %d", -1, 1, -1, 1);
    EXPECT_EQ("sprintf_flag_+_and_space", ret, 10);
    EXPECT_STREQ("sprintf_flag_+_and_space", str, "-1 +1 -1 1");

    ret = sprintf(str, "%#o,%#x,%#X", 9, 9, 9);
    EXPECT_EQ("sprintf_flag_#", ret, 11);
    EXPECT_STREQ("sprintf_flag_#", str, "011,0x9,0X9");

    ret = sprintf(str, "%5d,%05d,%0-5d", 123, 123, 123);
    EXPECT_EQ("sprintf_flag_-_and_0", ret, 17);
    EXPECT_STREQ("sprintf_flag_-_and_0", str, "  123,00123,123  ");

}

void sprintf_width_tests(void)
{
    char str[BUFFER_SIZE];
    int ret = 0;
    ret = sprintf(str, "%d,%3d,%10d", 12345, 12345, 12345);
    EXPECT_EQ("sprintf_width_num", ret, 22);
    EXPECT_STREQ("sprintf_width_num", str, "12345,12345,     12345");

    ret = sprintf(str, "%*d", 10, 12345);
    EXPECT_EQ("sprintf_width_*", ret, 10);
    EXPECT_STREQ("sprintf_width_*", str, "     12345");
}

void sprintf_precision_tests(void)
{
    char str[BUFFER_SIZE];
    int ret = 0;
    ret = sprintf(str, "%f,%.3f", 1.2345, 1.2345);
    EXPECT_EQ("sprintf_precision_num", ret, 14);
    EXPECT_STREQ("sprintf_precision_num", str, "1.234500,1.234");

    ret = sprintf(str, "%.*f", 3, 1.2345);
    EXPECT_EQ("sprintf_precision_*", ret, 5);
    EXPECT_STREQ("sprintf_precision_*", str, "1.234");
}


int main(void)
{
     // %[flags][width][.precision][length]specifier
    sprintf_specifier_tests();
    sprintf_flag_tests();
    sprintf_width_tests();
    sprintf_precision_tests();
    return t_status;
}
