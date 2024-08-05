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

#include "cmath"
#include "util.h"

using namespace std;

constexpr double PI = 3.14159265;
constexpr double FLT_MIN = 1.175494351e-38F;

static const double DOUBLE_VALUES[] = { 0.1, 10.0, -100.0, 0.0001, 5.14e11, -0.0001, 10000000.0, -100000000.0 };
static const double COSSIN_VALUES[] = { -334.143, -2.0, -1.0, 0.0, 0.5, 1.0, 243.01, 3.14 };
static const long double DIVIDEND_VALUES[] = { 10.5L, -10.5L, 2.71L, -2.71L, 3.14159265358979323846L,
                                               -3.14159265358979323846L, 1.0 / 3.0L, -1.0 / 3.0L };
static const long double DIVISOR_VALUES[] = { 3.0L, -3.0L, 1.414L, -1.414L, 0.5L, -0.5L,
                                              2.7182818284590452354L, -2.7182818284590452354L };

static const double FLOAT_VALUES[] = { 0.1, 10.0, -100.0, 0.0001, 5.14e11, -0.0001, 10000000.0, -100000000.0 };
static const double ATANHF_FLOAT_VALUES[] = { -1.0, -0.0, 0.0, -0.5, 0.9, 1.0, -100, 1000000.0 };
static const double RINTF_FLOAT_VALUES[] = { -3.52467, 0.0, 2.0 / 0.0, 3.37562, 3.76542 };
static const float LOGF_VALUES[] = { 0.0, static_cast<float>(0.0 / 0.0), 20.0, static_cast<float>(9.6 / 0.0), 0.0001, 5.14e11, -1.0, -100 };
// The function generates a value that has the size of the parameter x and the symbol of the parameter y
static void Bm_function_Copysignl_Allpositive(benchmark::State &state)
{
    long double x = 2417851639229258349412352.000000;
    long double y = 6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

static void Bm_function_Copysignl_Allnegative(benchmark::State &state)
{
    long double x = -2417851639229258349412352.000000;
    long double y = -6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

static void Bm_function_Copysignl_Np(benchmark::State &state)
{
    long double x = -2417851639229258349412352.000000;
    long double y = 6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

static void Bm_function_Copysignl_Pn(benchmark::State &state)
{
    long double x = 2417851639229258349412352.000000;
    long double y = -6051711999279104.000000;
    for (auto _ : state) {
        benchmark::DoNotOptimize(copysignl(x, y));
    }
}

// remainder(long doubel)
static void Bm_function_Fmodl(benchmark::State &state)
{
    long double x = DIVIDEND_VALUES[state.range(0)];
    long double y = DIVISOR_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(fmodl(x, y));
    }
}

static void Bm_function_Fmodf(benchmark::State &state)
{
    float x = DIVIDEND_VALUES[state.range(0)];
    float y = DIVISOR_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(fmodf(x, y));
    }
}
// The 3.14th power of e
static void Bm_function_Exp(benchmark::State &state)
{
    double x = DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(exp(x));
    }
}

// The logarithm of base e and x
static void Bm_function_Log(benchmark::State &state)
{
    double x = DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(log(x));
    }
}

// Returns the cosine of the radian angle x
static void Bm_function_Cos(benchmark::State &state)
{
    double x = COSSIN_VALUES[state.range(0)];
    double val = PI / 180.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(cos(x * val));
    }
}

// Break floating point number into mantissa and exponent
static void Bm_function_Frexpl(benchmark::State &state)
{
    long double x = DIVIDEND_VALUES[state.range(0)];
    int e;
    for (auto _ : state) {
        benchmark::DoNotOptimize(frexpl(x, &e));
    }
}

// x * 2 ^ n
static void Bm_function_Scalbn1(benchmark::State &state)
{
    double x = 3.8;
    int n = 1024;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbn2(benchmark::State &state)
{
    double x = 10.9;
    int n = -1023;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbn3(benchmark::State &state)
{
    double x = -100.9;
    int n = -100;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbn4(benchmark::State &state)
{
    double x = -100.9;
    int n = 100;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbn(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl1(benchmark::State &state)
{
    long double x = static_cast<long double>(2e-10);
    int n = -16384;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl2(benchmark::State &state)
{
    long double x = static_cast<long double>(2e-10);
    int n = 16384;

    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl3(benchmark::State &state)
{
    long double x = -PI;
    int n = 1000;

    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// x * 2 ^ n
static void Bm_function_Scalbnl4(benchmark::State &state)
{
    long double x = -PI;
    int n = -1000;

    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnl(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Scalbnf1(benchmark::State &state)
{
    float x = 3.8;
    int n = 1024;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Scalbnf2(benchmark::State &state)
{
    float x = 10.9;
    int n = -1023;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Scalbnf3(benchmark::State &state)
{
    float x = -3.8;
    int n = 1024;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Scalbnf4(benchmark::State &state)
{
    float x = -10.9;
    int n = -1023;
    for (auto _ : state) {
        benchmark::DoNotOptimize(scalbnf(x, n));
    }
    state.SetItemsProcessed(state.iterations());
}

// remainder
static void Bm_function_Fmod(benchmark::State &state)
{
    double x = (double)DIVIDEND_VALUES[state.range(0)];
    double y = (double)DIVISOR_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(fmod(x, y));
    }
}

// Returns the sine of the radian angle x
static void Bm_function_Sin(benchmark::State &state)
{
    double x = COSSIN_VALUES[state.range(0)];
    double val = PI / 180.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(sin(x*val));
    }
}

static void Bm_function_Atanf(benchmark::State &state)
{
    float x = ATANHF_FLOAT_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(atanf(x));
    }
}

static void Bm_function_Atan2f(benchmark::State &state)
{
    float x = DIVIDEND_VALUES[state.range(0)];
    float y = DIVISOR_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(atan2f(x, y));
    }
}

// FP_INFINITE  The specified value is positive or negative infinity
static void Bm_function_fpclassifyl(benchmark::State &state)
{
    long double x = log(0.0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// FP_SUBNORMAL  The specified value is a positive or negative normalization value
static void Bm_function_fpclassifyl1(benchmark::State &state)
{
    long double x = (FLT_MIN / 2.0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// FP_NAN  The specified value is not a number
static void Bm_function_fpclassifyl2(benchmark::State &state)
{
    long double x = sqrt(-1.0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// FP_ZERO  Specify a value of zero
static void Bm_function_fpclassifyl3(benchmark::State &state)
{
    long double x = -0.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fpclassify(x));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Expm1(benchmark::State &state)
{
    double x = DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(expm1(x));
    }
    state.SetItemsProcessed(state.iterations());
}

// Rounds up one floating-point number
static void Bm_function_Ceilf(benchmark::State &state)
{
    float a = (float)DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(ceilf(a));
    }
}

// Returns the largest integer value less than or equal to the input value
static void Bm_function_Floor(benchmark::State &state)
{
    double a = DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(floor(a));
    }
}

// The logarithmic value used to calculate the gamma function
static void Bm_function_Lgammaf(benchmark::State &state)
{
    float a = (float)DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(lgammaf(a));
    }
}

static void Bm_function_Erfcf(benchmark::State &state)
{
    float a = (float)DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(erfcf(a));
    }
}

static void Bm_function_Ilogbf(benchmark::State &state)
{
    float a = (float)DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(ilogbf(a));
    }
}

// Used to perform multiplication and addition operations on floating-point numbers
// a * b + c
static void Bm_function_Fmaf(benchmark::State &state)
{
    srand(time(nullptr));
    for (int i = 0; i <= state.range(0); i++) {
        for (auto _ : state) {
            int eight = 8;
            float a = (float)DOUBLE_VALUES[rand() % eight];
            float b = (float)DOUBLE_VALUES[(rand()+1) % eight];
            float c = (float)DOUBLE_VALUES[(rand()+2) % eight];
            benchmark::DoNotOptimize(fmaf(a, b, c));
        }
    }
}

// create a floating-point number that represents a non-numeric value
static void Bm_function_Nan(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(nan("5.14e11"));
    }
}

static void Bm_function_Nan_null(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(nan(""));
    }
}

static void Bm_function_Acoshf(benchmark::State &state)
{
    float a = COSSIN_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(acoshf(a));
    }
}

static void Bm_function_Asinhf(benchmark::State &state)
{
    float a = COSSIN_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(asinhf(a));
    }
}

static void Bm_function_Modff(benchmark::State &state)
{
    float value = (float)COSSIN_VALUES[state.range(0)];
    float ipart;
    for (auto _ : state) {
        benchmark::DoNotOptimize(modff(value, &ipart));
    }
}

static void Bm_function_Llroundf(benchmark::State &state)
{
    float x = (float)DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(llroundf(x));
    }
}

static void Bm_function_Logbf(benchmark::State &state)
{
    float x = LOGF_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(logbf(x));
    }
}

static void Bm_function_Log10f(benchmark::State &state)
{
    float x = LOGF_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(log10f(x));
    }
}

static void Bm_function_Expm1f(benchmark::State &state)
{
    float x = FLOAT_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(expm1f(x));
    }
}

static void Bm_function_Atanhf(benchmark::State &state)
{
    float x = ATANHF_FLOAT_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(atanhf(x));
    }
}

static void Bm_function_Rintf(benchmark::State &state)
{
    float x = RINTF_FLOAT_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(rintf(x));
    }
}

static void Bm_function_Log1pf(benchmark::State &state)
{
    float a = (float)DOUBLE_VALUES[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(log1pf(a));
    }
}

MUSL_BENCHMARK(Bm_function_Copysignl_Allpositive);
MUSL_BENCHMARK(Bm_function_Copysignl_Allnegative);
MUSL_BENCHMARK(Bm_function_Copysignl_Np);
MUSL_BENCHMARK(Bm_function_Copysignl_Pn);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Fmodl, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Fmodf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Log, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Cos, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Frexpl, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Scalbn1);
MUSL_BENCHMARK(Bm_function_Scalbn2);
MUSL_BENCHMARK(Bm_function_Scalbn3);
MUSL_BENCHMARK(Bm_function_Scalbn4);
MUSL_BENCHMARK(Bm_function_Scalbnl1);
MUSL_BENCHMARK(Bm_function_Scalbnl2);
MUSL_BENCHMARK(Bm_function_Scalbnl3);
MUSL_BENCHMARK(Bm_function_Scalbnl4);
MUSL_BENCHMARK(Bm_function_Scalbnf1);
MUSL_BENCHMARK(Bm_function_Scalbnf2);
MUSL_BENCHMARK(Bm_function_Scalbnf3);
MUSL_BENCHMARK(Bm_function_Scalbnf4);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Fmod, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Sin, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Atanf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Atan2f, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_fpclassifyl);
MUSL_BENCHMARK(Bm_function_fpclassifyl1);
MUSL_BENCHMARK(Bm_function_fpclassifyl2);
MUSL_BENCHMARK(Bm_function_fpclassifyl3);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Expm1, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Exp, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Ceilf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Floor, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Lgammaf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Erfcf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Ilogbf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Fmaf, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Nan);
MUSL_BENCHMARK(Bm_function_Nan_null);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Acoshf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Asinhf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Modff, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Llroundf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Logbf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Log10f, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Expm1f, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Atanhf, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Rintf, "BENCHMARK_5");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Log1pf, "BENCHMARK_8");
