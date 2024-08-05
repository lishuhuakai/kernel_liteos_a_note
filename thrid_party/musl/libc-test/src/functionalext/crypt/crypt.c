/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <crypt.h>
#include "functionalext.h"

static void test_crypt(void)
{
    char *key = "password";
    char *salt;

    salt = "$1$";
    char *md5_result = crypt(key, salt);
    char *md5_expect = "5f4dcc3b5aa765d61d8327deb882cf99";
    EXPECT_EQ(test_crypt, md5_result, md5_expect);

    salt = "$2$";
    char *blowfish_result_first = crypt(key, salt);
    char *blowfish_result_second = crypt(key, salt);
    EXPECT_EQ(test_crypt, blowfish_result_first, blowfish_result_second);

    salt = "$5$";
    char *sha256_result = crypt(key, salt);
    char *sha256_expect =
        "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8";
    EXPECT_EQ(test_crypt, sha256_result, sha256_expect);

    salt = "$6$";
    char *sha512_result = crypt(key, salt);
    char *sha512_expect =
        "b109f3bbbc244eb82441917ed06d618b9008dd09b3befd1b5e07394c706a8bb980b1d778"
        "5e5976ec049b46df5f1326af5a2ea6d103fd07c95385ffab0cacbc86";
    EXPECT_EQ(test_crypt, sha512_result, sha512_expect);

    salt = "";
    char *des_result_first = crypt(key, salt);
    char *des_result_second = crypt(key, salt);
    EXPECT_EQ(test_crypt, des_result_first, des_result_second);
}

int main(void)
{
    test_crypt();
    return t_status;
}