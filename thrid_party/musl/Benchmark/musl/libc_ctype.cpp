/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
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

#include "cctype"
#include "cwctype"
#include "clocale"
#include "util.h"
#if defined __APPLE__
#include <xlocale.h>
#endif

using namespace std;

constexpr int TOTAL_COMMON_CHARACTERS = 127;

static void Bm_function_Tolower_a(benchmark::State &state)
{
    int c = 97;
    for (auto _ : state) {
        benchmark::DoNotOptimize(tolower(c));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Tolower_A(benchmark::State &state)
{
    int c = 65;
    for (auto _ : state) {
        benchmark::DoNotOptimize(tolower(c));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Tolower_all_ascii(benchmark::State &state)
{
    for (auto _ : state) {
        for (int i = 0; i < TOTAL_COMMON_CHARACTERS; i++) {
            benchmark::DoNotOptimize(tolower(i));
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isspace_space(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isspace(' '));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isspace_all_ascii(benchmark::State &state)
{
    for (auto _ : state) {
        for (int i = 0; i < TOTAL_COMMON_CHARACTERS; i++) {
            benchmark::DoNotOptimize(isspace(i));
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_a(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isalnum('a'));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_A(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isalnum('A'));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_0(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(isalnum('0'));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isalnum_all_ascii(benchmark::State &state)
{
    for (auto _ : state) {
        for (int i = 0; i < TOTAL_COMMON_CHARACTERS; i++) {
            benchmark::DoNotOptimize(isalnum(i));
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// Check if it is a base 16 number
static void Bm_function_Isxdigit(benchmark::State &state)
{
    const char a[] = { 'A', 'M', '1', '2', '3', '\0' };
    const char b = a[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(isxdigit(b));
    }
}

static void Bm_function_Isxdigit_l(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    const char a[] = { 'P', 'M', 'N', 'Z', '3', '\0' };
    const char b = a[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(isxdigit_l(b, loc));
    }
}

// Determines whether a given wide character is an alphabetic character
static void Bm_function_Iswalpha_l(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    const wchar_t test [] = { L'1', L's', L'6', L'g', L't', L'7', L'l', L'2', L'\0' };
    const wchar_t a = test[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(iswalpha_l(a, loc));
    }
}

static void Bm_function_Towupper_a(benchmark::State &state)
{
    wint_t ch = L'a';
    for (auto _ : state) {
        benchmark::DoNotOptimize(towupper(ch));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Towupper_A(benchmark::State &state)
{
    wint_t ch = L'A';
    for (auto _ : state) {
        benchmark::DoNotOptimize(towupper(ch));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Towupper_all_ascii(benchmark::State &state)
{
    for (auto _ : state) {
        for (int i = 0; i < TOTAL_COMMON_CHARACTERS; i++) {
            benchmark::DoNotOptimize(towupper(i));
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Towupper_unicode(benchmark::State &state)
{
    const wint_t test[] = {L'à', L'ζ', L'в', L'գ', L'კ', L'ａ', L'文', L'♬'};
    wint_t wc = test[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(towupper(wc));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isascii_yes(benchmark::State &state)
{
    int ch = 65;
    for (auto _ : state) {
        benchmark::DoNotOptimize(isascii(ch));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Isascii_no(benchmark::State &state)
{
    int ch = 128;
    for (auto _ : state) {
        benchmark::DoNotOptimize(isascii(ch));
    }
    state.SetItemsProcessed(state.iterations());
}

static locale_t g_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", nullptr);

// Checks whether the characters in the string are numeric
// have numeric characters
static void Bm_function_Isdigit_l_numericCharacters(benchmark::State &state)
{
    const char *str = "0123456789";
    const char *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            isdigit_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// no numeric characters
static void Bm_function_Isdigit_l_nonNumericCharacters(benchmark::State &state)
{
    const char *str = "!@hHiIjJZz";
    const char *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            isdigit_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// Checks whether the characters in the string are low-order characters
// have lowercharacter
static void Bm_function_Islower_l_lowerCharacters(benchmark::State &state)
{
    const char *str = "abcdegfhijklmnopqrstuvwxyz";
    const char *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            islower_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// no lowercharacter
static void Bm_function_Islower_l_nonLowerCharacter(benchmark::State &state)
{
    const char *str = "23!@ABCHIJZ";
    const char *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            islower_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// Check whether the characters in the string are uppercase
// have uppercase characters
static void Bm_function_Isupper_l_upperCharacters(benchmark::State &state)
{
    const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            isupper_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// no uppercase characters
static void Bm_function_Isupper_l_nonUpperCharacters(benchmark::State &state)
{
    const char *str = "23!@abcdefg";
    const char *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            isupper_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// Checks characters in wide strings for low-order characters
// have lowercharacter
static void Bm_function_Iswlower_l_lowerCharacter(benchmark::State &state)
{
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyz";
    const wchar_t *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            iswlower_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// no lowercharacter
static void Bm_function_Iswlower_l_nonLowerCharacter(benchmark::State &state)
{
    const wchar_t *str = L"12ABC!@#";
    const wchar_t *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            iswlower_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// Checks characters in wide strings for whitespace
// have blankcharacter
static void Bm_function_Iswblank_l_blankCharacter(benchmark::State &state)
{
    const wchar_t *str = L" \t";
    const wchar_t *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            iswblank_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

// no blankcharacter
static void Bm_function_Iswblank_l_nonBlankCharacter(benchmark::State &state)
{
    const wchar_t *str = L"2!~*3Ad";
    const wchar_t *p = nullptr;
    for (auto _ : state) {
        p = str;
        while (*p++ && *p != '\0') {
            iswblank_l(*p, g_locale);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Tolower_a);
MUSL_BENCHMARK(Bm_function_Tolower_A);
MUSL_BENCHMARK(Bm_function_Tolower_all_ascii);
MUSL_BENCHMARK(Bm_function_Isspace_space);
MUSL_BENCHMARK(Bm_function_Isspace_all_ascii);
MUSL_BENCHMARK(Bm_function_Isalnum_a);
MUSL_BENCHMARK(Bm_function_Isalnum_A);
MUSL_BENCHMARK(Bm_function_Isalnum_0);
MUSL_BENCHMARK(Bm_function_Isalnum_all_ascii);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Isxdigit, "BENCHMARK_5");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Isxdigit_l, "BENCHMARK_5");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Iswalpha_l, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Towupper_a);
MUSL_BENCHMARK(Bm_function_Towupper_A);
MUSL_BENCHMARK(Bm_function_Towupper_all_ascii);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Towupper_unicode, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Isascii_yes);
MUSL_BENCHMARK(Bm_function_Isascii_no);
MUSL_BENCHMARK(Bm_function_Isdigit_l_numericCharacters);
MUSL_BENCHMARK(Bm_function_Isdigit_l_nonNumericCharacters);
MUSL_BENCHMARK(Bm_function_Islower_l_lowerCharacters);
MUSL_BENCHMARK(Bm_function_Islower_l_nonLowerCharacter);
MUSL_BENCHMARK(Bm_function_Isupper_l_upperCharacters);
MUSL_BENCHMARK(Bm_function_Isupper_l_nonUpperCharacters);
MUSL_BENCHMARK(Bm_function_Iswlower_l_lowerCharacter);
MUSL_BENCHMARK(Bm_function_Iswlower_l_nonLowerCharacter);
MUSL_BENCHMARK(Bm_function_Iswblank_l_blankCharacter);
MUSL_BENCHMARK(Bm_function_Iswblank_l_nonBlankCharacter);
