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
#include <iostream>
#if not defined __APPLE__
#include <stdio_ext.h>
#endif
#include <unistd.h>
#include <thread>
#include "cstring"
#include "cwchar"
#include "err.h"
#include "cerrno"
#include "clocale"
#include "util.h"
using namespace std;

static const std::vector<int> bufferSizes {
    8,
    16,
    32,
    64,
    512,
    1 * K,
    8 * K,
    16 * K,
    32 * K,
    64 * K,
    128 * K,
};

static const std::vector<int> limitSizes {
    1,
    8,
    64,
    1 * K,
    16 * K,
    64 * K,
    128 * K,
};

static void StringtestArgs(benchmark::internal::Benchmark* b)
{
    for (auto l : bufferSizes) {
        for (auto f : limitSizes) {
            if (f > l) {
                b->Args({l, l, 0});
            } else {
                b->Args({l, f, 0});
            }
        }
    }
}

// Searches for the first occurrence of the character x in the first n bytes of the selected string
static void Bm_function_Memchr(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t bmmemchrAlignment = state.range(2);
    vector<char> bmmemchr;
    char *bmmemchrAligned = GetAlignedPtrFilled(&bmmemchr, bmmemchrAlignment, nbytes, 'n');
    bmmemchrAligned[nbytes - 1] = '\0';
    while (state.KeepRunning()) {
        if (memchr(bmmemchrAligned, 'x', limitsize) != nullptr) {
            errx(1, "ERROR: memchr found a chr where it should have failed.");
        }
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Finds the last occurrence of the specified character in a string and returns a pointer to that position
static void Bm_function_Strrchr(benchmark::State &state)
{
    const char *strrchrtestsrc[] = { "com.ohos.launcher", "/system/lib/libfilemgmt_libhilog.z.so",
                                     "/system/lib/libstatic_subscriber_extension.z.so",
                                     "../../base/startup/init/services/param/base/param_base.c",
                                     "/system/lib/libwallpapermanager.z.so",
                                     "/system/lib/libwallpaperextension.z.so",
                                     "/system/lib/module/libaccessibility.z.so",
                                     "/../base/startup/init/services/param/base/param_trie.c" };
    const char strrchrtesttag[] = { 'm', 'l', 's', 'o', 'z', 't', 'i', 'c', '\0' };
    const char *test = strrchrtestsrc[state.range(0)];
    const char ch = strrchrtesttag[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(strrchr(test, ch));
    }
    state.SetBytesProcessed(state.iterations());
}

// The selected range calculates the length
static void Bm_function_Strnlen(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t bmstrnlenAlignment = state.range(2);
    vector<char> bmstrnlen;
    char *bmstrnlenAligned = GetAlignedPtrFilled(&bmstrnlen, bmstrnlenAlignment, nbytes + 1, 'n');
    bmstrnlenAligned[nbytes - 1] = '\0';

    volatile int c __attribute__((unused)) = 0;
    while (state.KeepRunning()) {
        c += strnlen(bmstrnlenAligned, limitsize);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
#if not defined __APPLE__
extern "C" size_t __strlen_chk(const char* s, size_t s_len);

static void Bm_function_Strlen_chk(benchmark::State& state)
{
    const size_t nbytes = bufferSizes[state.range(0)];
    vector<char> bmstrlen;
    char *bmstrlenAligned = GetAlignedPtrFilled(&bmstrlen, 0, nbytes + 1, 'n');
    bmstrlenAligned[nbytes - 1] = '\0';

    volatile int c __attribute__((unused)) = 0;
    while (state.KeepRunning()) {
        c += __strlen_chk(bmstrlenAligned, __builtin_object_size(bmstrlenAligned, 0));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
#endif

// Specifies the maximum number of copies to replicate
static void Bm_function_Stpncpy(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t srcAlignment = state.range(2);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'z');
    char *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        stpncpy(dstAligned, srcAligned, limitsize);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Used to copy one string to another, you can limit the maximum length of copying
static void Bm_function_Strncpy(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t srcAlignment = state.range(2);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'z');
    char *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        strncpy(dstAligned, srcAligned, limitsize);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Comparing whether two binary blocks of data are equal is functionally similar to MEMCMP
static void Bm_function_Bcmp(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'x');
    char *dstAligned = GetAlignedPtrFilled(&dst, dstAlignment, nbytes, 'x');

    volatile int c __attribute__((unused)) = 0;
    while (state.KeepRunning()) {
        c += bcmp(dstAligned, srcAligned, nbytes);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Find the first character in a given string that matches any character in another specified string
static void Bm_function_Strpbrk(benchmark::State &state)
{
    const char *strpbrktestsrc[] = { "method", "setTimeout", "open.harmony",
                                     "libfilemgmt_libhilog", "libwallpaperextension",
                                     "startup", "libwallpapermanager", "param_trie" };
    const char *strpbrktesttag[] = { "th", "me", "enh", "lo", "en", "tu", "ag", "pa" };
    const char *src = strpbrktestsrc[state.range(0)];
    const char *tag = strpbrktesttag[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(strpbrk(src, tag));
    }
}

// Set the first n characters in the wide string to another wide character
static void Bm_function_Wmemset(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t alignment = state.range(1);

    vector<wchar_t> buf;
    wchar_t *bufAligned = GetAlignedPtr(&buf, alignment, nbytes + 1);

    while (state.KeepRunning()) {
        wmemset(bufAligned, L'n', nbytes);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// The first n characters in the source memory region are copied to the destination memory region
static void Bm_function_Wmemcpy(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<wchar_t> src;
    vector<wchar_t> dst;
    wchar_t *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, L'z');
    wchar_t *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    while (state.KeepRunning()) {
        wmemcpy(dstAligned, srcAligned, nbytes);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Returns the index value of the first successful match
static void Bm_function_Strcspn(benchmark::State &state)
{
    const char *strcspnsrc[] = { "system/lib64", "system/lib64/chipset-pub-sdk", "vendor/lib64/chipsetsdk",
                                 "system/lib64/ndk", "system/lib64/platformsdk", "system/lib64/priv-platformsdk",
                                 "system/lib64/module/data", "tem/lib64/module/security" };
    const char *strcspntag[] = { "vendor/lib64", "/system/lib64/chipset-sdk", "/system/lib64/ndk",
                                 "lib64/chipset-pub-sdk", "priv-platformsdk", "/system/lib64/priv-module",
                                 "/system/lib64/module/multimedia", "/system/lib" };
    const char *src = strcspnsrc[state.range(0)];
    const char *tag = strcspntag[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcspn(src, tag));
    }
}

#if not defined __APPLE__
static void Bm_function_Strchrnul_exist(benchmark::State &state)
{
    const char *str = "musl.ld.debug.dlclose";
    int c = 46;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strchrnul(str, c));
    }
}

static void Bm_function_Strchrnul_noexist(benchmark::State &state)
{
    const char *str = "all";
    int c = 46;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strchrnul(str, c));
    }
}

static void Bm_function_Strchrnul(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    std::vector<char> haystack;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    haystackAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        strchrnul(haystackAligned, '.');
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
#endif

static void Bm_function_Strcasecmp_capital_equal(benchmark::State &state)
{
    const char *l = "ABCDEF";
    const char *r = "ABCDEF";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp_small_equal(benchmark::State &state)
{
    const char *l = "abcdef";
    const char *r = "abcdef";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp_equal(benchmark::State &state)
{
    const char *l = "aBcDeD";
    const char *r = "ABCdEd";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp_not_equal(benchmark::State &state)
{
    const char *l = "bbcdef";
    const char *r = "bBCdEd";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    const size_t needleAlignment = state.range(2);

    std::vector<char> haystack;
    std::vector<char> needle;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    char *needleAligned = GetAlignedPtrFilled(&needle, needleAlignment, nbytes, 'x');

    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(haystackAligned, needleAligned));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strncasecmp(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    const size_t needleAlignment = state.range(2);

    std::vector<char> haystack;
    std::vector<char> needle;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    char *needleAligned = GetAlignedPtrFilled(&needle, needleAlignment, nbytes, 'x');
    for (auto _ : state) {
        benchmark::DoNotOptimize(strncasecmp(haystackAligned, needleAligned, nbytes));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strdup(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    std::vector<char> haystack;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    haystackAligned[nbytes - 1] = '\0';
    char* ptr = nullptr;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(ptr = strdup(haystackAligned));
        free(ptr);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strncat(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    std::vector<char> haystack;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    haystackAligned[nbytes - 1] = '\0';
    std::vector<char> dstStack;
    char *dst = GetAlignedPtrFilled(&dstStack, haystackAlignment, nbytes, '0');
    while (state.KeepRunning()) {
        dst[0] = 0;
        benchmark::DoNotOptimize(strncat(dst, haystackAligned, nbytes));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Compare two wide strings according to the current local environment
static void BM_function_Wcscoll(benchmark::State& state)
{
    setlocale(LC_ALL, "");
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<wchar_t> src;
    vector<wchar_t> dst;
    wchar_t* srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, L'n');
    wchar_t* dstAligned = GetAlignedPtrFilled(&dst, dstAlignment, nbytes, L'z');

    volatile int c __attribute__((unused)) = 0;
    while (state.KeepRunning()) {
        c += wcscoll(dstAligned, srcAligned);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Similar to wcscoll the main difference is its support for localization
static void BM_function_Wcscoll_l(benchmark::State& state)
{
    setlocale(LC_ALL, "");
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<wchar_t> src;
    vector<wchar_t> dst;
    wchar_t* srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, L'n');
    wchar_t* dstAligned = GetAlignedPtrFilled(&dst, dstAlignment, nbytes, L'z');

    volatile int c __attribute__((unused)) = 0;
    while (state.KeepRunning()) {
        c += wcscoll_l(dstAligned, srcAligned, loc);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Converts wide characters to single bytes
static void Bm_function_Wctob(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    const wchar_t test [] = { L'Y', L'd', L'M', L'O', L'K', L'J', L'L', L's', L'\0' };
    const wchar_t a = test[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(wctob(a));
    }
}

// Converts single-byte characters to wide characters
static void Bm_function_Btowc(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    const char c [] = { 't', 'h', 'i', 's', 'a', 'm', 'z', 'g', '\0'};
    const char a = c[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(btowc(a));
    }
}

// According to the program's current regional options the character set converts
// the first n characters of the string src and places them in the string dest
static void Bm_function_Strxfrm(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'z');
    char *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        strxfrm(dstAligned, srcAligned, nbytes);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Similar to strxfrm the main difference is its support for localization
static void Bm_function_Strxfrm_l(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'z');
    char *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        strxfrm_l(dstAligned, srcAligned, nbytes, loc);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Converts a wide-character string into a sequenced string
// according to specified localization rules
static void Bm_function_Wcsxfrm(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<wchar_t> src;
    vector<wchar_t> dst;
    wchar_t *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, L'A');
    wchar_t *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        wcsxfrm(dstAligned, srcAligned, nbytes);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Similar to wcsxfrm the main difference is its support for localization
static void Bm_function_Wcsxfrm_l(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<wchar_t> src;
    vector<wchar_t> dst;
    wchar_t *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, L'A');
    wchar_t *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        wcsxfrm_l(dstAligned, srcAligned, nbytes, loc);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// compare two strings according to localized language rules
static void BM_function_Strcoll(benchmark::State& state)
{
    setlocale(LC_ALL, "");
    const size_t nbytes = state.range(0);
    const size_t s1Alignment = state.range(1);
    const size_t s2Alignment = state.range(2);

    vector<char> s1;
    vector<char> s2;
    char* s1ALigned = GetAlignedPtrFilled(&s1, s1Alignment, nbytes, 'x');
    char* s2ALigned = GetAlignedPtrFilled(&s2, s2Alignment, nbytes, 'x');
    s1ALigned[nbytes - 1] = '\0';
    s2ALigned[nbytes - 1] = '\0';

    volatile int c __attribute__((unused));
    while (state.KeepRunning()) {
        c = strcoll(s1ALigned, s2ALigned);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Similar to strcoll the main difference is its support for localization
static void BM_function_Strcoll_l(benchmark::State& state)
{
    setlocale(LC_ALL, "");
    locale_t loc = newlocale(LC_ALL_MASK, "", nullptr);
    const size_t nbytes = state.range(0);
    const size_t s1Alignment = state.range(1);
    const size_t s2Alignment = state.range(2);

    vector<char> s1;
    vector<char> s2;
    char* s1ALigned = GetAlignedPtrFilled(&s1, s1Alignment, nbytes, 'x');
    char* s2ALigned = GetAlignedPtrFilled(&s2, s2Alignment, nbytes, 'x');
    s1ALigned[nbytes - 1] = '\0';
    s2ALigned[nbytes - 1] = '\0';

    volatile int c __attribute__((unused));
    while (state.KeepRunning()) {
        c = strcoll_l(s1ALigned, s2ALigned, loc);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Wmemcmp(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<wchar_t> src;
    vector<wchar_t> dst;
    wchar_t *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'x');
    wchar_t *dstAligned = GetAlignedPtrFilled(&dst, dstAlignment, nbytes, 'x');

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(wmemcmp(dstAligned, srcAligned, nbytes));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

constexpr int BYTE_SIZE = 4;

static void BM_function_Wcsstr(benchmark::State& state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    const size_t needleAlignment = state.range(2);

    std::vector<wchar_t> haystack;
    std::vector<wchar_t> needle;
    wchar_t* haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, L'x');
    wchar_t* needleAligned = GetAlignedPtrFilled(&needle, needleAlignment, std::min(nbytes, static_cast<size_t>(5)),
        L'x');
    
    if (nbytes / BYTE_SIZE > 2) {
        for (size_t i = 0; nbytes / BYTE_SIZE >= 2 && i < nbytes / BYTE_SIZE - 2; i++) {
        haystackAligned[BYTE_SIZE * i + 3] = L'y';
        }
    }
    haystackAligned[nbytes - 1] = L'\0';
    needleAligned[needle.size() - 1] = L'\0';

    while (state.KeepRunning()) {
        if (wcsstr(haystackAligned, needleAligned) == nullptr) {
        errx(1, "ERROR: strstr failed to find valid substring.");
        }
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void BM_function_Strcasestr(benchmark::State& state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    const size_t needleAlignment = state.range(2);

    std::vector<char> haystack;
    std::vector<char> needle;
    char* haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    char* needleAligned = GetAlignedPtrFilled(&needle, needleAlignment, std::min(nbytes, static_cast<size_t>(5)), 'X');
    if (nbytes / BYTE_SIZE > 2) {
        for (size_t i = 0; nbytes / BYTE_SIZE >= 2 && i < nbytes / BYTE_SIZE - 2; i++) {
        haystackAligned[BYTE_SIZE * i + 3] = 'y';
        }
    }
    haystackAligned[nbytes - 1] = '\0';
    needleAligned[needle.size() - 1] = '\0';

    while (state.KeepRunning()) {
        if (strcasestr(haystackAligned, needleAligned) == nullptr) {
            errx(1, "ERROR: strcasestr failed to find valid substring.");
        }
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void BM_function_Strlcat(benchmark::State& state)
{
    const size_t nbytes = state.range(0);
    const size_t needleAlignment = state.range(1);

    std::vector<char> haystack(nbytes);
    std::vector<char> needle;
    char* dstBuf = haystack.data();
    char* srcBuf = GetAlignedPtrFilled(&needle, needleAlignment, nbytes, 'x');
    srcBuf[needle.size() - 1] = '\0';

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(strlcat(dstBuf, srcBuf, nbytes));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void BM_function_Getdelim(benchmark::State& state)
{
    FILE* fp = fopen(DATA_ROOT"/data/getdlim.txt", "w+");
    if (fp == nullptr) {
        errx(1, "ERROR: fp is nullptr\n");
    }
    const char* buf = "123 4567 78901 ";
    const int bufLen = strlen(buf);
    //增大测试文件，确保在进行benchmark循环测试时，不会读到文件尾而进入fseek函数影响数据准确性
    for (int i = 0; i < 1024 * 1024; ++i) {
        fwrite(buf, bufLen, 1, fp);
    }

    fflush(fp);
    fseek(fp, 0, SEEK_SET);

    size_t maxReadLen = 1024;
    char* readBuf = (char*)malloc(maxReadLen);
    if (readBuf == nullptr) {
        errx(1, "ERROR: readBuf is nullptr\n");
    }

    while (state.KeepRunning()) {
        ssize_t n = getdelim(&readBuf, &maxReadLen, ' ', fp);
        if (n == -1) {
            fseek(fp, 0, SEEK_SET);
        }
    }
    free(readBuf);
    fclose(fp);
}
MUSL_BENCHMARK(BM_function_Getdelim);
MUSL_BENCHMARK_WITH_ARG(BM_function_Strlcat, "ALIGNED_ONEBUF");
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Memchr, StringtestArgs);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Strnlen, StringtestArgs);
#if not defined __APPLE__
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strlen_chk, "BENCHMARK_8");
#endif
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Stpncpy, StringtestArgs);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Strncpy, StringtestArgs);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strcspn, "BENCHMARK_8");
#if not defined __APPLE__
MUSL_BENCHMARK(Bm_function_Strchrnul_exist);
MUSL_BENCHMARK(Bm_function_Strchrnul_noexist);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strchrnul, "ALIGNED_ONEBUF");
#endif
MUSL_BENCHMARK(Bm_function_Strcasecmp_capital_equal);
MUSL_BENCHMARK(Bm_function_Strcasecmp_small_equal);
MUSL_BENCHMARK(Bm_function_Strcasecmp_equal);
MUSL_BENCHMARK(Bm_function_Strcasecmp_not_equal);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strcasecmp, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strncasecmp, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strrchr, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Bcmp, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strpbrk, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wmemset, "ALIGNED_ONEBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wmemcpy, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strdup, "ALIGNED_ONEBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strncat, "ALIGNED_ONEBUF");
MUSL_BENCHMARK_WITH_ARG(BM_function_Wcscoll, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(BM_function_Wcscoll_l, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wctob, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Btowc, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strxfrm, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strxfrm_l, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wcsxfrm, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wcsxfrm_l, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(BM_function_Strcoll, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(BM_function_Strcoll_l, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wmemcmp, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(BM_function_Wcsstr, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(BM_function_Strcasestr, "ALIGNED_TWOBUF");
