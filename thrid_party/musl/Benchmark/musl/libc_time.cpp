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

#include "ctime"
#if not defined __APPLE__
#include "sys/timerfd.h"
#endif
#include "sys/time.h"
#include "cstdlib"
#include "cstdio"
#include "csignal"
#include "unistd.h"
#include "util.h"

using namespace std;
constexpr int TEN = 10;
constexpr int HUNDRED = 100;
// Used to put the current thread to sleep for the specified time
static void Bm_function_Nanosleep_0ns(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(nanosleep(&req, &rem));
    }
}

static void Bm_function_Nanosleep_10ns(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(nanosleep(&req, &rem));
    }
}

static void Bm_function_Nanosleep_100ns(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = HUNDRED;
    for (auto _ : state) {
        benchmark::DoNotOptimize(nanosleep(&req, &rem));
    }
}

// Used to set information about the time zone
static void Bm_function_Tzset(benchmark::State &state)
{
    while (state.KeepRunning()) {
        tzset();
    }
}
static void Bm_function_Clock_nanosleep_realtime(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_REALTIME, 0, &req, &rem));
    }
}

static void Bm_function_Clock_nanosleep_realtime_raw(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_REALTIME_COARSE, 0, &req, &rem));
    }
}

static void Bm_function_Clock_nanosleep_realtime_coarse(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_REALTIME_COARSE, 0, &req, &rem));
    }
}

static void Bm_function_Clock_nanosleep_monotonic(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_MONOTONIC, 0, &req, &rem));
    }
}
static void Bm_function_Clock_nanosleep_monotonic_coarse(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_MONOTONIC_COARSE, 0, &req, &rem));
    }
}
static void Bm_function_Clock_nanosleep_monotonic_raw(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_MONOTONIC_RAW, 0, &req, &rem));
    }
}
static void Bm_function_Clock_nanosleep_boottime(benchmark::State &state)
{
    struct timespec req, rem;
    req.tv_nsec = TEN;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clock_nanosleep(CLOCK_BOOTTIME, 0, &req, &rem));
    }
}

constexpr int BUFFER_SIZE = 32;

static void Bm_function_Strftime(benchmark::State &state)
{
    time_t rawTime = time(nullptr);
    struct tm *localTime = localtime(&rawTime);
    char buf[BUFFER_SIZE];
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(strftime(buf, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", localTime));
    }
}

static void Bm_function_Mktime(benchmark::State &state)
{
    time_t rawTime = time(nullptr);
    struct tm *localTime = localtime(&rawTime);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(mktime(localTime));
    }
}

static void Bm_function_Gmtime(benchmark::State &state)
{
    time_t rawTime = time(nullptr);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(gmtime(&rawTime));
    }
}

static void Bm_function_Timerfd_settime(benchmark::State &state)
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd < 0) {
        perror("timerfd_create");
    }

    struct timespec now;
    struct itimerspec newValue;
    if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
        perror("clock_gettime");
    }

    newValue.it_value.tv_sec = now.tv_sec + 3;
    newValue.it_value.tv_nsec = now.tv_nsec;
    newValue.it_interval.tv_sec = 0;
    newValue.it_interval.tv_nsec = 0;

    while (state.KeepRunning()) {
        if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &newValue, nullptr) == -1) {
            perror("timerfd_settime");
        }
    }

    close(fd);
}

// Converts a timestamp of a time_t type to a readable string representation
static void Bm_function_Ctime_r(benchmark::State &state)
{
    time_t t = time(nullptr);
    for (auto _ : state) {
        char buf[100] = {0};
        benchmark::DoNotOptimize(ctime_r(&t, buf));
    }
}

void HandleTimer(int signum)
{
    switch (signum) {
        case SIGALRM:
            // do nothing
            break;
        case SIGVTALRM:
            // do nothing
            break;
        case SIGPROF:
            // do nothing
            break;
        default:
            break;
    }
}

// Used to set the timer
// ITIMER_REAL timing mode
static void Bm_function_Setitimer_realtime(benchmark::State &state)
{
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    for (auto _ : state) {
        signal(SIGALRM, HandleTimer);
        if (setitimer(ITIMER_REAL, &timer, nullptr) == -1) {
            perror("Set timer error!");
        }
        benchmark::DoNotOptimize(timer);
    }
}

// ITIMER_PROF timing mode
static void Bm_function_Setitimer_peoftime(benchmark::State &state)
{
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    for (auto _ : state) {
        signal(SIGPROF, HandleTimer);
        if (setitimer(ITIMER_PROF, &timer, nullptr) == -1) {
            perror("Set timer error!");
        }
        benchmark::DoNotOptimize(timer);
    }
}

// ITIMER_VIRTUAL timing mode
static void Bm_function_Setitimer_virtualtime(benchmark::State &state)
{
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    for (auto _ : state) {
        signal(SIGVTALRM, HandleTimer);
        if (setitimer(ITIMER_VIRTUAL, &timer, nullptr) == -1) {
            perror("Set timer error!");
        }
        benchmark::DoNotOptimize(timer);
    }
}

static void Handler(int signum)
{
    // do nothing
}

// Use the timer function to create a timer
static void Bm_function_Timer(benchmark::State &state)
{
    timer_t timerid;
    struct sigevent evp;
    struct itimerspec itVal;
    int ret;

    evp.sigev_value.sival_ptr = &timerid;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGUSR1;

    itVal.it_value.tv_sec = 1;
    itVal.it_value.tv_nsec = 0;
    itVal.it_interval.tv_sec = 1;
    itVal.it_interval.tv_nsec = 0;

    for (auto _ : state) {
        ret = timer_create(CLOCK_REALTIME, &evp, &timerid);
        if (ret) {
            perror("timer_create");
        }

        ret = timer_settime(timerid, 0, &itVal, nullptr);
        if (ret) {
            perror("timer_settime");
        }

        signal(SIGUSR1, Handler);

        ret = timer_delete(timerid);
        if (ret) {
            perror("timer_delete");
        }
    }
}

static void Bm_function_Asctime(benchmark::State &state)
{
    struct tm tmTime = {
        .tm_year = 90,
        .tm_mon = 1,
        .tm_mday = 12,
        .tm_hour = 20,
        .tm_min = 0,
        .tm_sec = 04,
        .tm_isdst = -1,
    };

    for (auto _ : state) {
        benchmark::DoNotOptimize(asctime(&tmTime));
    }
}

static void Bm_function_Gmtime_r(benchmark::State &state)
{
    time_t timestamp = time(nullptr);
    struct tm result;
    for (auto _ : state) {
        benchmark::DoNotOptimize(gmtime_r(&timestamp, &result));
    }
}

static void Bm_function_Timegm(benchmark::State &state)
{
    struct tm tmTime = {
        .tm_year = 90,
        .tm_mon = 1,
        .tm_mday = 12,
        .tm_hour = 20,
        .tm_min = 0,
        .tm_sec = 04,
        .tm_isdst = -1,
    };

    for (auto _ : state) {
        benchmark::DoNotOptimize(timegm(&tmTime));
    }
}

// Converts a string to a time based on a different time zone
static void Bm_function_Strptime(benchmark::State &state)
{
    const char *formatsrc[] = { "%c", "%c %Z%z", "%D", "%F", "%g", "%k", "%l", "%s", "%u",
                                "%v", "%G", "%j", "%P", "%U", "%w", "%V", "%R", "%r", "%T",
                                "%x", "%X", "%%" };
    const char *format = formatsrc[state.range(0)];
    time_t rawTime = time(nullptr);
    struct tm *localTime = localtime(&rawTime);
    char buf[BUFFER_SIZE];
    strftime(buf, BUFFER_SIZE, format, localTime);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(strptime(buf, format, localTime));
    }
}

MUSL_BENCHMARK(Bm_function_Nanosleep_0ns);
MUSL_BENCHMARK(Bm_function_Nanosleep_10ns);
MUSL_BENCHMARK(Bm_function_Nanosleep_100ns);
MUSL_BENCHMARK(Bm_function_Tzset);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_realtime);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_realtime_raw);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_realtime_coarse);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_monotonic);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_monotonic_raw);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_monotonic_coarse);
MUSL_BENCHMARK(Bm_function_Clock_nanosleep_boottime);
MUSL_BENCHMARK(Bm_function_Strftime);
MUSL_BENCHMARK(Bm_function_Mktime);
MUSL_BENCHMARK(Bm_function_Gmtime);
MUSL_BENCHMARK(Bm_function_Timerfd_settime);
MUSL_BENCHMARK(Bm_function_Ctime_r);
MUSL_BENCHMARK(Bm_function_Setitimer_realtime);
MUSL_BENCHMARK(Bm_function_Setitimer_peoftime);
MUSL_BENCHMARK(Bm_function_Setitimer_virtualtime);
MUSL_BENCHMARK(Bm_function_Timer);
MUSL_BENCHMARK(Bm_function_Asctime);
MUSL_BENCHMARK(Bm_function_Gmtime_r);
MUSL_BENCHMARK(Bm_function_Timegm);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strptime, "BENCHMARK_22");

