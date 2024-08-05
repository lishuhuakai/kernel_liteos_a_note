/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <float.h>
#include "functionalext.h"


#define EXPECT_DOUBLE_EQ(a, b)                                             \
    do {                                                                   \
        if (!(a == b || (fabs(a - b) < DBL_EPSILON)))                      \
            t_error("%s failed: %f is not equal to %f\n", __func__, a, b); \
    } while (0)


#define LENGTH(x) (sizeof(x) / sizeof *(x))

static struct {
    wchar_t *s;
    double f;
} g_t[] = {
    {L"0", 0.0},
    {L"00.00", 0.0},
    {L"-.00000", -0.0},
    {L"1e+1000000", INFINITY},
    {L"1e-1000000", 0},
    // 2^-1074 * 0.5 - eps
    {L".2470328229206232720882843964341106861825299013071623822127928412503377536351043e-323", 0},
    // 2^-1074 * 0.5 + eps
    {L".2470328229206232720882843964341106861825299013071623822127928412503377536351044e-323", 0x1p-1074},
    // 2^-1074 * 1.5 - eps
    {L".7410984687618698162648531893023320585475897039214871466383785237510132609053131e-323", 0x1p-1074},
    // 2^-1074 * 1.5 + eps
    {L".7410984687618698162648531893023320585475897039214871466383785237510132609053132e-323", 0x1p-1073},
    // 2^-1022 + 2^-1075 - eps
    {L".2225073858507201630123055637955676152503612414573018013083228724049586647606759e-307", 0x1p-1022},
    // 2^-1022 + 2^-1075 + eps
    {L".2225073858507201630123055637955676152503612414573018013083228724049586647606760e-307", 0x1.0000000000001p-1022},
    // 2^1024 - 2^970 - eps
    {L"17976931348623158079372897140530341507993413271003782693617377898044"
    L"49682927647509466490179775872070963302864166928879109465555478519404"
    L"02630657488671505820681908902000708383676273854845817711531764475730"
    L"27006985557136695962284291481986083493647529271907416844436551070434"
    L"2711559699508093042880177904174497791.999999999999999999999999999999", 0x1.fffffffffffffp1023},
    // 2^1024 - 2^970
    {L"17976931348623158079372897140530341507993413271003782693617377898044"
    L"49682927647509466490179775872070963302864166928879109465555478519404"
    L"02630657488671505820681908902000708383676273854845817711531764475730"
    L"27006985557136695962284291481986083493647529271907416844436551070434"
    L"2711559699508093042880177904174497792", INFINITY},
    // some random numbers
    {L".5961860348131807091861002266453941950428e00", 0.59618603481318067}, // 0x1.313f4bc3b584cp-1
    {L"1.815013169218038729887460898733526957442e-1", 0.18150131692180388}, // 0x1.73b6f662e1712p-3
    {L"42.07082357534453600681618685682257590772e-2", 0.42070823575344535}, // 0x1.aece23c6e028dp-2
    {L"665.4686306516261456328973225579833470816e-3", 0.66546863065162609}, // 0x1.54b84dea53453p-1
    {L"6101.852922970868621786690495485449831753e-4", 0.61018529229708685}, // 0x1.386a34e5d516bp-1
    {L"76966.95208236968077849464348875471158549e-5", 0.76966952082369677}, // 0x1.8a121f9954dfap-1
    {L"250506.5322228682496132604807222923702304e-6", 0.25050653222286823}, // 0x1.0084c8cd538c2p-2
    {L"2740037.230228005325852424697698331177377e-7", 0.27400372302280052}, // 0x1.18946e9575ef4p-2
    {L"20723093.50049742645941529268715428324490e-8", 0.20723093500497428}, // 0x1.a868b14486e4dp-3
    {L"0.7900280238081604956226011047460238748912e1", 7.9002802380816046}, // 0x1.f99e3100f2eaep+2
    {L"0.9822860653737296848190558448760465863597e2", 98.228606537372968}, // 0x1.88ea17d506accp+6
    {L"0.7468949723190370809405570560160405324869e3", 746.89497231903704}, // 0x1.75728e73f48b7p+9
    {L"0.1630268320282728475980459844271031751665e4", 1630.2683202827284}, // 0x1.97912c28d5cbp+10
    {L"0.4637168629719170695109918769645492022088e5", 46371.686297191707}, // 0x1.6a475f6258737p+15
    {L"0.6537805944497711554209461686415872067523e6", 653780.59444977110}, // 0x1.3f3a9305bb86cp+19
    {L"0.2346324356502437045212230713960457676531e6", 234632.43565024371}, // 0x1.ca4437c3631eap+17
    {L"0.9709481716420048341897258980454298205278e8", 97094817.164200485}, // 0x1.7263284a8242cp+26
    {L"0.4996908522051874110779982354932499499602e9", 499690852.20518744}, // 0x1.dc8ad6434872ap+28
};

/**
 * @tc.name      : wcstod_l_0100
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for wcstod_l
 * @tc.level     : Level 0
 */
void wcstod_l_0100(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    int i;
    double x;
    wchar_t *p;

    for (i = 0; i < LENGTH(g_t); i++) {
        x = wcstod_l(g_t[i].s, &p, loc);
        EXPECT_DOUBLE_EQ(x, g_t[i].f);
    }
}


/**
 * @tc.name      : wcstod_l_0200
 * @tc.desc      : set locale as zh_CN, transfer string 123.45xxx to double, and check rest string
 * @tc.level     : Level 0
 */
void wcstod_l_0200(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"123.45xxx";
    const double target = 123.45;
    wchar_t str1[] = L"xxx";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_0300
 * @tc.desc      : set locale as zh_CN, transfer string +123.45xxx to double, and check rest string
 * @tc.level     : Level 0
 */
void wcstod_l_0300(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"+123.45xxx";
    const double target = 123.45;
    wchar_t str1[] = L"xxx";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_0400
 * @tc.desc      : set locale as zh_CN, transfer string -123.45xxx to double, and check rest string
 * @tc.level     : Level 0
 */
void wcstod_l_0400(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"-123.45xxx";
    const double target = -123.45;
    wchar_t str1[] = L"xxx";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_0500
 * @tc.desc      : set locale as zh_CN, transfer string   123.45xxx to double, and check rest string
 * @tc.level     : Level 0
 */
void wcstod_l_0500(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"  123.45xxx";
    const double target = 123.45;
    wchar_t str1[] = L"xxx";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_0600
 * @tc.desc      : set locale as zh_CN, transfer string   123.45xxx to double, and check rest string
 * @tc.level     : Level 0
 */
void wcstod_l_0600(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"xxx123.45";
    const double target = 0;
    wchar_t str1[] = L"xxx123.45";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_0700
 * @tc.desc      : set locale as zh_CN, transfer string +inf to double
 * @tc.level     : Level 0
 */
void wcstod_l_0700(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"+inf";
    const double target = INFINITY;
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
}

/**
 * @tc.name      : wcstod_l_0800
 * @tc.desc      : set locale as zh_CN, transfer string infinity to double
 * @tc.level     : Level 0
 */
void wcstod_l_0800(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"infinity";
    const double target = INFINITY;
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
}

/**
 * @tc.name      : wcstod_l_0900
 * @tc.desc      : set locale as zh_CN, transfer string infinixxx to double, and check rest string
 * @tc.level     : Level 0
 */
void wcstod_l_0900(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"infinixxx";
    const double target = INFINITY;
    wchar_t str1[] = L"inixxx";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1000
 * @tc.desc      : set locale as zh_CN, transfer string -inf to double
 * @tc.level     : Level 0
 */
void wcstod_l_1000(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"-inf";
    const double target = -INFINITY;
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
}

/**
 * @tc.name      : wcstod_l_1100
 * @tc.desc      : set locale as zh_CN, transfer string +nan to double
 * @tc.level     : Level 0
 */
void wcstod_l_1100(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"+nan";
    const double target = NAN;
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    if (!isnan(num)) {
        t_error("%s failed: res should be nan\n", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1200
 * @tc.desc      : set locale as zh_CN, transfer string -nan to double
 * @tc.level     : Level 0
 */
void wcstod_l_1200(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"-nan";
    const double target = NAN;
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    if (!isnan(num)) {
        t_error("%s failed: res should be nan\n", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1300
 * @tc.desc      : set locale as zh_CN, transfer string 0X1.BC to double, and check rest string
 * tips: wcstod_l does not support hexadecimal number, should return 0
 * @tc.level     : Level 0
 */
void wcstod_l_1300(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"0X1.BC";
    const double target = 0;
    wchar_t str1[] = L"X1.BC";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1400
 * @tc.desc      : set locale as zh_CN, transfer empty string, the res should be zero
 * @tc.level     : Level 0
 */
void wcstod_l_1400(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"";
    const double target = 0;
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
}


/**
 * @tc.name      : wcstod_l_1500
 * @tc.desc      : set locale as zh_CN, transfer string    xxx to num, the res should be zero
 * @tc.level     : Level 0
 */
void wcstod_l_1500(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"   xxx";
    const double target = 0;
    wchar_t str1[] = L"   xxx";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1600
 * @tc.desc      : set locale as zh_CN, transfer string 123.45你好 to num, the res should be zero
 * @tc.level     : Level 0
 */
void wcstod_l_1600(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wchar_t* str = L"123.45你好";
    const double target = 123.45;
    wchar_t str1[] = L"你好";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1700
 * @tc.desc      : set locale as de_DE whcih is not supported by newlocale, transfer string 1234.56 to num,
 the string should be processed by original wcstod
 * @tc.level     : Level 0
 */
void wcstod_l_1700(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "de_DE", NULL);
    wchar_t* str = L"1234.56";
    const double target = 1234.56;
    wchar_t str1[] = L"";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1800
 * @tc.desc      : set locale LC_CTYPE_MASK as zh_CN, transfer blank string to num
 * @tc.level     : Level 0
 */
void wcstod_l_1800(void)
{
    locale_t loc = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    wchar_t* str = L"     ";
    const double target = 0;
    wchar_t str1[] = L"     ";
    wchar_t* end;
    double num = wcstod_l(str, &end, loc);
    EXPECT_DOUBLE_EQ(num, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_l_1900
 * @tc.desc      : set locale as zh_CN.UTF-8, set endptr as NULL
 * @tc.level     : Level 0
 */
void wcstod_l_1900(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    wchar_t* str = L"123.45";
    const double target = 123.45;
    double num = wcstod_l(str, NULL, loc);
    EXPECT_DOUBLE_EQ(num, target);
}

int main(void)
{
    wcstod_l_0100();
    wcstod_l_0200();
    wcstod_l_0300();
    wcstod_l_0400();
    wcstod_l_0500();
    wcstod_l_0600();
    wcstod_l_0700();
    wcstod_l_0800();
    wcstod_l_0900();
    wcstod_l_1000();
    wcstod_l_1100();
    wcstod_l_1200();
    wcstod_l_1300();
    wcstod_l_1400();
    wcstod_l_1500();
    wcstod_l_1600();
    wcstod_l_1700();
    wcstod_l_1800();
    wcstod_l_1900();

    return t_status;
}