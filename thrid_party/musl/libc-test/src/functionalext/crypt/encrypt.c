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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include "functionalext.h"

#define BITS_COUNT_OF_ONE_BYTE 8
#define LENGTH 8

// 验证加解密之后是否还与原文相同
static void test_encrypt_and_decrypt(void)
{
    char keytext[] = "ABCDEFGH";
    char key[65];
    char plaintext[] = "HELLO123";
    char buf[65];
    char block_cipher[9];
    char block_decrypt[9];

    // 处理密钥
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < BITS_COUNT_OF_ONE_BYTE; j++) {
            key[i * BITS_COUNT_OF_ONE_BYTE + j] = (keytext[i] >> j) & 1;
        }
    }

    // 将明文处理为二进制
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < BITS_COUNT_OF_ONE_BYTE; j++) {
            buf[i * BITS_COUNT_OF_ONE_BYTE + j] = (plaintext[i] >> j) & 1;
        }
        setkey(key);
    }
    printf("Before encrypting: %s\n", plaintext);

    // 加密并将二进制数据处理为字符串
    encrypt(buf, 0);
    for (int i = 0; i < LENGTH; i++) {
        block_cipher[i] = '\0';
        for (int j = 0; j < BITS_COUNT_OF_ONE_BYTE; j++) {
            block_cipher[i] |= buf[i * BITS_COUNT_OF_ONE_BYTE + j] << j;
        }
        block_cipher[LENGTH] = '\0';
    }
    printf("After encrypting:  %s\n", block_cipher);

    // 解密并将二进制数据处理为字符串
    encrypt(buf, 1);
    for (int i = 0; i < LENGTH; i++) {
        block_decrypt[i] = '\0';
        for (int j = 0; j < BITS_COUNT_OF_ONE_BYTE; j++) {
            block_decrypt[i] |= buf[i * BITS_COUNT_OF_ONE_BYTE + j] << j;
        }
        block_decrypt[LENGTH] = '\0';
    }
    printf("After decrypting:  %s\n", block_decrypt);
    EXPECT_STREQ("test_encrypt_and_decrypt", plaintext, block_decrypt);
}

int main(void)
{
    test_encrypt_and_decrypt();
    return t_status;
}