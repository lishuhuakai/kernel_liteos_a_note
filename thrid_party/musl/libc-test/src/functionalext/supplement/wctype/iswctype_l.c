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

#include <locale.h>
#include <stdlib.h>
#include <wctype.h>
#include "functionalext.h"

const int COUNT = 62;

/**
 * @tc.name      : iswctype_l_0100
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_ALNUM,
 *                 and it is judged that the input character is a WCTYPE_ALNUM.
 * @tc.level     : Level 0
 */
void iswctype_l_0100(void)
{
    wchar_t c = L'0';
    wctype_t wt = wctype("alnum");
    EXPECT_NE("iswctype_l_0100", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0100", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0200
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_ALPHA,
 *                 and it is judged that the input character is a WCTYPE_ALPHA.
 * @tc.level     : Level 0
 */
void iswctype_l_0200(void)
{
    wchar_t c = L'a';
    wctype_t wt = wctype("alpha");
    EXPECT_NE("iswctype_l_0200", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0200", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0300
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_BLANK,
 *                 and it is judged that the input character is a WCTYPE_BLANK.
 * @tc.level     : Level 0
 */
void iswctype_l_0300(void)
{
    wchar_t c = L' ';
    wctype_t wt = wctype("blank");
    EXPECT_NE("iswctype_l_0300", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0300", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0400
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_CNTRL,
 *                 and it is judged that the input character is a WCTYPE_CNTRL.
 * @tc.level     : Level 0
 */
void iswctype_l_0400(void)
{
    wchar_t c = L'\n';
    wctype_t wt = wctype("cntrl");
    EXPECT_NE("iswctype_l_0400", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0400", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0500
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_DIGIT,
 *                 and it is judged that the input character is a WCTYPE_DIGIT.
 * @tc.level     : Level 0
 */
void iswctype_l_0500(void)
{
    wchar_t c = L'1';
    wctype_t wt = wctype("digit");
    EXPECT_NE("iswctype_l_0500", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0500", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0600
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_GRAPH,
 *                 and it is judged that the input character is a WCTYPE_GRAPH.
 * @tc.level     : Level 0
 */
void iswctype_l_0600(void)
{
    wchar_t c = L'!';
    wctype_t wt = wctype("graph");
    EXPECT_NE("iswctype_l_0600", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0600", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0700
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_LOWER,
 *                 and it is judged that the input character is a WCTYPE_LOWER.
 * @tc.level     : Level 0
 */
void iswctype_l_0700(void)
{
    wchar_t c = L'z';
    wctype_t wt = wctype("lower");
    EXPECT_NE("iswctype_l_0700", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0700", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0800
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_PRINT,
 *                 and it is judged that the input character is a WCTYPE_PRINT.
 * @tc.level     : Level 0
 */
void iswctype_l_0800(void)
{
    wchar_t c = L'p';
    wctype_t wt = wctype("print");
    EXPECT_NE("iswctype_l_0800", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0800", ret, 0);
}

/**
 * @tc.name      : iswctype_l_0900
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_PUNCT,
 *                 and it is judged that the input character is a WCTYPE_PUNCT.
 * @tc.level     : Level 0
 */
void iswctype_l_0900(void)
{
    wchar_t c = L'/';
    wctype_t wt = wctype("punct");
    EXPECT_NE("iswctype_l_0900", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_0900", ret, 0);
}

/**
 * @tc.name      : iswctype_l_1000
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_SPACE,
 *                 and it is judged that the input character is a WCTYPE_SPACE.
 * @tc.level     : Level 0
 */
void iswctype_l_1000(void)
{
    wchar_t c = L'\n';
    wctype_t wt = wctype("space");
    EXPECT_NE("iswctype_l_1000", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_1000", ret, 0);
}

/**
 * @tc.name      : iswctype_l_1100
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_UPPER,
 *                 and it is judged that the input character is a WCTYPE_UPPER.
 * @tc.level     : Level 0
 */
void iswctype_l_1100(void)
{
    wchar_t c = L'A';
    wctype_t wt = wctype("upper");
    EXPECT_NE("iswctype_l_1100", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_1100", ret, 0);
}

/**
 * @tc.name      : iswctype_l_1200
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a WCTYPE_XDIGIT,
 *                 and it is judged that the input character is a WCTYPE_XDIGIT.
 * @tc.level     : Level 0
 */
void iswctype_l_1200(void)
{
    wchar_t c = L'd';
    wctype_t wt = wctype("xdigit");
    EXPECT_NE("iswctype_l_1200", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_NE("iswctype_l_1200", ret, 0);
}

/**
 * @tc.name      : iswctype_l_1300
 * @tc.desc      : Verify iswctype_l process success. The parameter c is a error type,
 *                 and it is judged that the input character is a error type.
 * @tc.level     : Level 2
 */
void iswctype_l_1300(void)
{
    wchar_t c = L'【';
    wctype_t wt = wctype("xdigit");
    EXPECT_NE("iswctype_l_1300", wt, -1);
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswctype_l(c, wt, m_locale);
    EXPECT_EQ("iswctype_l_1300", ret, 0);
}

int main(void)
{
    iswctype_l_0100();
    iswctype_l_0200();
    iswctype_l_0300();
    iswctype_l_0400();
    iswctype_l_0500();
    iswctype_l_0600();
    iswctype_l_0700();
    iswctype_l_0800();
    iswctype_l_0900();
    iswctype_l_1000();
    iswctype_l_1100();
    iswctype_l_1200();
    iswctype_l_1300();
    return t_status;
}