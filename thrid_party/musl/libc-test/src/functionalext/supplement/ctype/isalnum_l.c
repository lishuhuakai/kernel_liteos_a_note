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

#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

const int COUNT = 62;
const int SIZE = 128;

/**
 * @tc.name      : isalnum_l_0100
 * @tc.desc      : Verify isalnum_l process success when using the en-US.UTF-8 character set.
 *                 The parameter c is an English letter,
 *                 and it is judged that the input character is a letter.
 * @tc.level     : Level 0
 */
void isalnum_l_0100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0100 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_0100", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_0100 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_0100", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0200
 * @tc.desc      : Verify isalnum_l process success when using the en-US.UTF-8 character set.
 *                 The parameter c is an English number,
 *                 and it is judged that the input character is a number.
 * @tc.level     : Level 0
 */
void isalnum_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *str = "0123456789";
    const char *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0200 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_0200", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_0200 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_0200", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0300
 * @tc.desc      : Verify isalnum_l process fail when using the en-US.UTF-8 character set.
 *                 The parameter c is an special character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *str = "@#$%^&";
    const char *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0300 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_0300", 1, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (ret) {
            t_printf("Test case isalnum_l_0300 failed while testing character %c\n", *p);
        }
        EXPECT_EQ("isalnum_l_0300", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0400
 * @tc.desc      : Verify isalnum_l process fail when using the en-US.UTF-8 character set.
 *                 Determine the number of letters and numbers in the ascii code table.
 * @tc.level     : Level 1
 */
void isalnum_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0400 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_0400", 1, 0);
        return ;
    }

    for (int i = 0; i < SIZE; i++) {
        int ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ("isalnum_l_0400", total, COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0500
 * @tc.desc      : Verify isalnum_l process success when using the en-US.UTF-8 character set.
 *                 The parameter c is an wide English character,
 *                 and it is judged that the input wide character is a letter.
 * @tc.level     : Level 0
 */
void isalnum_l_0500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const wchar_t *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0500 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_0500", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_0500 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_0500", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0600
 * @tc.desc      : Verify isalnum_l process success when using the en-US.UTF-8 character set.
 *                 The parameter c is an wide English character,
 *                 and it is judged that the input wide character is a number.
 * @tc.level     : Level 0
 */
void isalnum_l_0600(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"0123456789";
    const wchar_t *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0600 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_0600", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_0600 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_0600", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0700
 * @tc.desc      : Verify isalnum_l process fail when using the en-US.UTF-8 character set.
 *                 The parameter c is an wide English character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_l_0700(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"@#$%^&";
    const wchar_t *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0700 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_0700", 1, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (ret) {
            t_printf("Test case isalnum_l_0700 failed while testing character %c\n", *p);
        }
        EXPECT_EQ("isalnum_l_0700", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0800
 * @tc.desc      : Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *                 The parameter c is an English letter,
 *                 and it is judged that the input character is a letter.
 * @tc.level     : Level 0
 */
void isalnum_l_0800(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0800 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_0800", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_0800 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_0800", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_0900
 * @tc.desc      : Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *                 The parameter c is an English number,
 *                 and it is judged that the input character is a number.
 * @tc.level     : Level 0
 */
void isalnum_l_0900(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const char *str = "0123456789";
    const char *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_0900 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_0900", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_0900 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_0900", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1000
 * @tc.desc      : Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *                 The parameter c is an special character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_l_1000(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const char *str = "@#$%^&";
    const char *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_1000 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_1000", 1, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (ret) {
            t_printf("Test case isalnum_l_1000 failed while testing character %c\n", *p);
        }
        EXPECT_EQ("isalnum_l_1000", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1100
 * @tc.desc      : Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *                 Determine the number of letters and numbers in the ascii code table.
 * @tc.level     : Level 1
 */
void isalnum_l_1100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    int total = 0;

    if (!m_locale) {
        t_printf("Test case isalnum_l_1100 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_1100", 1, 0);
        return ;
    }

    for (int i = 0; i < SIZE; i++) {
        int ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ("isalnum_l_1100", total, COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1200
 * @tc.desc      : Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *                 The parameter c is an wide English character,
 *                 and it is judged that the input wide character is a letter.
 * @tc.level     : Level 0
 */
void isalnum_l_1200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const wchar_t *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_1200 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_1200", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_1200 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_1200", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1300
 * @tc.desc      : Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *                 The parameter c is an wide English character,
 *                 and it is judged that the input wide character is a number.
 * @tc.level     : Level 0
 */
void isalnum_l_1300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const wchar_t *str = L"0123456789";
    const wchar_t *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_1300 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_1300", 0, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_1300 failed while testing character %c\n", *p);
        }
        EXPECT_NE("isalnum_l_1300", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1400
 * @tc.desc      : Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *                 The parameter c is an wide English character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_l_1400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    const wchar_t *str = L"@#$%^&";
    const wchar_t *p = str;

    if (!m_locale) {
        t_printf("Test case isalnum_l_1400 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_1400", 1, 0);
        return ;
    }

    while (p && *p != '\0') {
        int ret = isalnum_l(*p, m_locale);
        if (ret) {
            t_printf("Test case isalnum_l_1400 failed while testing character %c\n", *p);
        }
        EXPECT_EQ("isalnum_l_1400", ret, 0);
        p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1500
 * @tc.desc      : Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *                 The characters are not in the en-US.UTF-8 and zh_CN.UTF-8 character sets,
 *                 and it is judged that the input character is a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_l_1500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);

    if (!m_locale) {
        t_printf("Test case isalnum_l_1500 failed to call function newlocale\n");
        EXPECT_NE("isalnum_l_1500", 0, 0);
        return ;
    }

    //Arabic characters(U+0600 - U+06FF)
    for (int i = 0x0660; i <= 0x0669; i++) {
        int ret = isalnum_l(i, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_1500 failed while testing unicode %04x\n", i);
        }
        EXPECT_NE("isalnum_l_1500", ret, 0);
    }

    //GREEK characters(U+0370 - U+03FF)
    //GREEK SMALL LETTER
    for (int i = 0x03AC; i <= 0x03CE; i++) {
        int ret = isalnum_l(i, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_1500 failed while testing unicode %04x\n", i);
        }
        EXPECT_NE("isalnum_l_1500", ret, 0);
    }

    //ARMENIAN characters(U+1D360 - U+1D37F)
    //ARMENIAN CAPITAL LETTER
    for (int i = 0x0531; i <= 0x0556; i++) {
        int ret = isalnum_l(i, m_locale);
        if (!ret) {
            t_printf("Test case isalnum_l_1500 failed while testing unicode %04x\n", i);
        }
        EXPECT_NE("isalnum_l_1500", ret, 0);
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1600
 * @tc.desc      : Verify isalnum_l process fail when using the zh_CN.UTF-8 character set.
 *                 The characters are not in the en-US.UTF-8 and zh_CN.UTF-8 character sets,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_l_1600(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);

    if (!m_locale) {
        t_printf("Test case isalnum_l_1600 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_1600", 1, 0);
        return ;
    }

    //GREEK TONOS
    //GREEK DIALYTIKA TONOS
    for (int i = 0x0384; i <= 0x0385; i++) {
        int ret = isalnum_l(i, m_locale);
        if (ret) {
            t_printf("Test case isalnum_l_1600 failed while testing unicode %04x\n", i);
        }
        EXPECT_EQ("isalnum_l_1600", ret, 0);
    }

    //ARMENIAN APOSTROPHE
    //ARMENIAN EMPHASIS MARK
    //ARMENIAN EXCLAMATION MARK
    //ARMENIAN COMMA
    //ARMENIAN QUESTION MARK
    //ARMENIAN ABBREVIATION MARK
    for (int i = 0x055A; i <= 0x055F; i++) {
        int ret = isalnum_l(i, m_locale);
        if (ret) {
            t_printf("Test case isalnum_l_1600 failed while testing unicode %04x\n", i);
        }
        EXPECT_EQ("isalnum_l_1600", ret, 0);
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isalnum_l_1700
 * @tc.desc      : Verify isalnum_l process success when using the zh_CN.UTF-8 character set.
 *                 BOPOMOFO LETTER in the unicode is letter.
 * @tc.level     : Level 1
 */
void isalnum_l_1700(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    int total = 0;

    if (!m_locale) {
        t_printf("Test case isalnum_l_1700 failed to call function newlocale\n");
        EXPECT_EQ("isalnum_l_1700", 1, 0);
        return ;
    }

    //0x3105(BOPOMOFO LETTER B) ~ 0x312f(BOPOMOFO LETTER NN)
    for (int i = 0x3105; i <= 0x312f; i++) {
        int ret = isalnum_l(i, m_locale);
        if (ret) {
            total++;
        } else {
            t_printf("Test case isalnum_l_1700 failed i = 0x%x\n", i);
        }
    }

    EXPECT_EQ("isalnum_l_1700", total, 43);
    freelocale(m_locale);
    m_locale = NULL;
}

int main(void)
{
    isalnum_l_0100();
    isalnum_l_0200();
    isalnum_l_0300();
    isalnum_l_0400();
    isalnum_l_0500();
    isalnum_l_0600();
    isalnum_l_0700();
    isalnum_l_0800();
    isalnum_l_0900();
    isalnum_l_1000();
    isalnum_l_1100();
    isalnum_l_1200();
    isalnum_l_1300();
    isalnum_l_1400();
    isalnum_l_1500();
    isalnum_l_1600();
    isalnum_l_1700();
    return t_status;
}