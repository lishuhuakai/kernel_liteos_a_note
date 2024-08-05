/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <trace/trace_marker.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test.h>
#include <unistd.h>

#define BUFFER_LEN 10240
#define READ_BUFFER_SIZE 4096
#define OUTFILE "/data/local/tmp/musl.trace"
#define EXPECT_TRUE(c)                \
    do                                \
    {                                 \
        if (!(c))                     \
            t_error("[%s] failed\n"); \
    } while (0)
#define EXPECT_FALSE(c)                \
    do                                 \
    {                                  \
        if ((c))                       \
            t_error("[%s] failed \n"); \
    } while (0)

#ifndef TRACE_TEMP_FAILURE_RETRY
#define TRACE_TEMP_FAILURE_RETRY(exp)          \
    ({                                         \
    long int _rc;                              \
    do {                                       \
        _rc = (long int)(exp);                 \
    } while ((_rc == -1) && (errno == EINTR)); \
    _rc;                                       \
    })
#endif
#define SET_TRACE(str) \
    do                                 \
    {                                  \
        system("echo "#str" > /sys/kernel/debug/tracing/tracing_on"); \
    } while (0)


typedef void (*TEST_FUN)(void);
static const int WAIT_TIME = 1;
static const int count = 100;

static void clear_trace()
{
    if (access("/sys/kernel/tracing/trace", F_OK) == 0) {
        system("echo > /sys/kernel/tracing/trace");
    }

    if (access("/sys/kernel/debug/tracing/trace", F_OK) == 0) {
        system("echo > /sys/kernel/tracing/trace");
    }

    return;
}

static void dump_trace(int trace_fd)
{
    char buffer[READ_BUFFER_SIZE];
    int nwrite;
    int nread;
    int out_fd = open(OUTFILE, O_WRONLY | O_CREAT, TEST_MODE);
    if (out_fd == -1) {
        return;
    }
    do {
        nread = TRACE_TEMP_FAILURE_RETRY(read(trace_fd, buffer, READ_BUFFER_SIZE));
        if ((nread == 0) || (nread == -1)) {
            break;
        }
        nwrite = TRACE_TEMP_FAILURE_RETRY(write(out_fd, buffer, nread));
    } while (nwrite > 0);
    close(out_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_begin and trace_marker_end.
 * @tc.level     : Level 0
 */
static void trace_marker_0010(void)
{
    clear_trace();
    SET_TRACE(1);
    trace_marker_begin(HITRACE_TAG_MUSL, "Musl_Trace_Marker_0010", "");
    trace_marker_end(HITRACE_TAG_MUSL);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Musl_Trace_Marker_0010");
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker_async
 * @tc.desc      : Test trace_marker_async_begin and trace_marker_async_end.
 * @tc.level     : Level 0
 */
static void trace_marker_0020(void)
{
    clear_trace();
    SET_TRACE(1);
    trace_marker_async_begin(HITRACE_TAG_MUSL, "async_begin_0200", "trace_async",1);
    trace_marker_async_end(HITRACE_TAG_MUSL, "async_end_0200", "trace_async",1);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_async_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "async_begin_0200", "trace_async" , 1);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "async_end_0200", "trace_async" , 1);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_begin and trace_marker_end.
 * @tc.level     : Level 0
 */
static void trace_marker_0030(void)
{
    clear_trace();
    SET_TRACE(1);
    int traceCount = 5;
    trace_marker_count(HITRACE_TAG_MUSL, "traceCount", traceCount);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_count_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount", traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test the multiple processes of trace_marker.
 * @tc.level     : Level 0
 */
static void trace_marker_0040(void)
{
    clear_trace();
    bool trace_sucess = false;
    char buffer_fir[BUFFER_LEN] = {0};
    char buffer_sec[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork! \n");
    } else if (fpid == 0) {
        int pidChild = getpid();
        SET_TRACE(1);
        trace_marker_begin(HITRACE_TAG_MUSL, "Trace_Marker0400_Forkfir", "");
        trace_marker_end(HITRACE_TAG_MUSL);
        SET_TRACE(0);

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker0400_Forkfir");
        if (buf_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
        if (buf_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_fir, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_fir, buf_begin) != NULL && strstr(buffer_fir, buf_end) != NULL) {
                trace_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_sucess);
        close(trace_fd);
        exit(pidChild);
    } else {
        SET_TRACE(1);
        trace_marker_begin(HITRACE_TAG_MUSL, "Trace_Marker0400_Forksec", "");
        trace_marker_end(HITRACE_TAG_MUSL);
        SET_TRACE(0);

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker0400_Forksec");
        if (buf_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
        if (buf_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_sec, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_sec, buf_begin) != NULL && strstr(buffer_sec, buf_end) != NULL) {
                trace_sucess = true;
                break;
            }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
    }
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test the multiple processes of trace_marker.
 * @tc.level     : Level 0
 */
static void trace_marker_0050(void)
{
    clear_trace();
    bool trace_async_sucess = false;
    char buffer_forkFir[BUFFER_LEN] = {0};
    char buffer_forkSec[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};

    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork! \n");
    } else if (fpid == 0) {
        int pidChild = getpid();
        SET_TRACE(1);
        trace_marker_async_begin(HITRACE_TAG_MUSL, "async0500_Forkfir", "begin_fir", 2);
        trace_marker_async_end(HITRACE_TAG_MUSL, "async0500_Forkfir", "end_fir", 2);
        SET_TRACE(0);

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "async0500_Forkfir", "begin_fir", 2);
        if (buf_async_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "async0500_Forkfir", "end_fir", 2);
        if (buf_async_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkFir, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_forkFir, buf_async_begin) != NULL && strstr(buffer_forkFir, buf_async_end) != NULL) {
                trace_async_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_async_sucess);
        close(trace_fd);
        exit(pidChild);
    } else {
        SET_TRACE(1);
        trace_marker_async_begin(HITRACE_TAG_MUSL, "async0500_Forksec", "begin_sec", 3);
        trace_marker_async_end(HITRACE_TAG_MUSL, "async0500_Forksec", "end_sec", 3);
        SET_TRACE(0);

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "async0500_Forksec", "begin_sec", 3);
        if (buf_async_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "async0500_Forksec", "end_sec", 3);
        if (buf_async_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkSec, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_forkSec, buf_async_begin) != NULL && strstr(buffer_forkSec, buf_async_end) != NULL) {
                trace_async_sucess = true;
                break;
            }
        }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
    }
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test the multiple processes of trace_marker.
 * @tc.level     : Level 0
 */
static void trace_marker_0060(void)
{
    clear_trace();
    int traceCount = 5;
    bool trace_count_sucess = false;
    char buffer_forkFir[BUFFER_LEN] = {0};
    char buffer_forkSec[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork! \n");
    } else if (fpid == 0) {
        int pidChild = getpid();

        SET_TRACE(1);
        trace_marker_count(HITRACE_TAG_MUSL, "traceCount_forkfir", traceCount);
        SET_TRACE(0);

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_count_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_forkfir", traceCount);
        if (buf_count_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkFir, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buf_count, buffer_forkFir) != NULL) {
                trace_count_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_count_sucess);
        close(trace_fd);
        exit(pidChild);
    } else {
        SET_TRACE(1);
        trace_marker_count(HITRACE_TAG_MUSL, "traceCount_forksec", traceCount);
        SET_TRACE(0);

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_count_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_forksec", traceCount);
        if (buf_count_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkSec, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buf_count, buffer_forkSec) != NULL) {
                trace_count_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_count_sucess);
        close(trace_fd);
    }
}

static void *ThreadTraceMarkerFir(void *arg)
{
    trace_marker_begin(HITRACE_TAG_MUSL, "Trace_Marker_Threadfir", "pthreadfir");
    trace_marker_end(HITRACE_TAG_MUSL);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker_Threadfir");
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerFir Exit");
}

static void *ThreadTraceMarkerSec(void *arg)
{
    trace_marker_begin(HITRACE_TAG_MUSL, "Trace_Marker_Threadsec", "pthreadsec");
    trace_marker_end(HITRACE_TAG_MUSL);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker_Threadsec");
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerSec Exit");
}

/**
 * @tc.number: trace_marker_0070
 * @tc.name: trace_marker
 * @tc.desc: Test the multithreading of trace_marker.
 */
static void trace_marker_0070(void)
{
    SET_TRACE(1);
    clear_trace();
    int res;
    const char msgThread1[1024] = {"msgThread1"};
    const char msgThread2[1024] = {"msgThread2"};
    pthread_t fatalMessageThread1, fatalMessageThread2;
    res = pthread_create(&fatalMessageThread1, NULL, ThreadTraceMarkerFir, (void *)msgThread1);
    if (res != 0) {
        t_printf("pthread_create1 error.");
    }
    res = pthread_create(&fatalMessageThread2, NULL, ThreadTraceMarkerSec, (void *)msgThread2);
    if (res != 0) {
        t_printf("pthread_create2 error.");
    }
    pthread_join(fatalMessageThread1, NULL);
    pthread_join(fatalMessageThread2, NULL);
    SET_TRACE(0);
}

static void *ThreadTraceMarkerAsyncFir(void *arg)
{
    trace_marker_async_begin(HITRACE_TAG_MUSL, "Async_Threadfir", "begin_threadfir",4);
    trace_marker_async_end(HITRACE_TAG_MUSL, "Async_Threadfir", "end_threadfir", 4);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_async_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "Async_Threadfir", "begin_threadfir", 4);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "Async_Threadfir", "end_threadfir", 4);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerAsyncFir Exit");
}

static void *ThreadTraceMarkerAsyncSec(void *arg)
{
    trace_marker_async_begin(HITRACE_TAG_MUSL, "Async_Threadsec", "begin_threadsec",5);
    trace_marker_async_end(HITRACE_TAG_MUSL, "Async_Threadsec", "end_threadsec",5);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_async_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "Async_Threadsec", "begin_threadsec", 5);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "Async_Threadsec", "end_threadsec", 5);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerAsyncSec Exit");
}

/**
 * @tc.number: trace_marker_0080
 * @tc.name: trace_marker
 * @tc.desc: Test the multithreading of trace_marker.
 */
static void trace_marker_0080(void)
{
    SET_TRACE(1);
    clear_trace();
    int res;
    const char msgThread1[1024] = {"msgThread3"};
    const char msgThread2[1024] = {"msgThread4"};
    pthread_t fatalMessageThread1, fatalMessageThread2;
    res = pthread_create(&fatalMessageThread1, NULL, ThreadTraceMarkerAsyncFir, (void *)msgThread1);
    if (res != 0) {
        t_printf("pthread_create3 error.");
    }
    res = pthread_create(&fatalMessageThread2, NULL, ThreadTraceMarkerAsyncSec, (void *)msgThread2);
    if (res != 0) {
        t_printf("pthread_create4 error.");
    }
    pthread_join(fatalMessageThread1, NULL);
    pthread_join(fatalMessageThread2, NULL);
    SET_TRACE(0);
}

static void *ThreadTraceMarkerCountFir(void *arg)
{
    int traceCount = 5;
    trace_marker_count(HITRACE_TAG_MUSL, "traceCount_Threadfir", traceCount);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_count_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_Threadfir", traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerCountFir Exit");
}

static void *ThreadTraceMarkerCountSec(void *arg)
{
    int traceCount = 5;
    trace_marker_count(HITRACE_TAG_MUSL, "traceCount_Threadsec", traceCount);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_count_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_Threadsec", traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerCountSec Exit");
}
/**
 * @tc.number: trace_marker_0090
 * @tc.name: trace_marker
 * @tc.desc: Test the multithreading of trace_marker.
 */
static void trace_marker_0090(void)
{
    SET_TRACE(1);
    clear_trace();
    int res;
    const char msgThread1[1024] = {"msgThread5"};
    const char msgThread2[1024] = {"msgThread6"};
    pthread_t fatalMessageThread1, fatalMessageThread2;
    res = pthread_create(&fatalMessageThread1, NULL, ThreadTraceMarkerCountFir, (void *)msgThread1);
    if (res != 0) {
        t_printf("pthread_create5 error.");
    }
    res = pthread_create(&fatalMessageThread2, NULL, ThreadTraceMarkerCountSec, (void *)msgThread2);
    if (res != 0) {
        t_printf("pthread_create6 error.");
    }
    pthread_join(fatalMessageThread1, NULL);
    pthread_join(fatalMessageThread2, NULL);
    SET_TRACE(0);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_begin and trace_marker_end, value is null.
 * @tc.level     : Level 2
 */
static void trace_marker_0100(void)
{
    clear_trace();
    SET_TRACE(1);
    trace_marker_begin(HITRACE_TAG_MUSL, "Musl_Trace_Marker_0100", NULL);
    trace_marker_end(HITRACE_TAG_MUSL);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Musl_Trace_Marker_0100");
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

     int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
       
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_begin and trace_marker_end, the size of message is 1026.
 * @tc.level     : Level 2
 */
static void trace_marker_0110(void)
{
    clear_trace();
    char message[1026]= {0};
    memset(message, 1, 1025);
    message[1025] = '\0';

    SET_TRACE(1);
    trace_marker_begin(HITRACE_TAG_MUSL, message, "");
    trace_marker_end(HITRACE_TAG_MUSL);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }
    bool trace_sucess = true;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), message);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

     int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = false;
            break;
        }
       
    }

    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_begin and trace_marker_end, message is null.
 * @tc.level     : Level 2
 */
static void trace_marker_0120(void)
{
    clear_trace();
    SET_TRACE(1);
    trace_marker_begin(HITRACE_TAG_MUSL, NULL, "");
    trace_marker_end(HITRACE_TAG_MUSL);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd); 
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_async_begin and trace_marker_async_end, the value is null. 
 * @tc.level     : Level 2
 */
static void trace_marker_0140(void)
{
    clear_trace();
    SET_TRACE(1);
    trace_marker_async_begin(HITRACE_TAG_MUSL, "async_begin_0200", NULL,1);
    trace_marker_async_end(HITRACE_TAG_MUSL, "async_end_0200", NULL,1);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_async_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};

    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s %d", getpid(), "async_begin_0200",1);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s %d", getpid(), "async_end_0200" ,1);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_async_begin and trace_marker_async_end, the size of message 
 *                 is 1026.
 * @tc.level     : Level 2
 */
static void trace_marker_0150(void)
{
    clear_trace();
    char message[1026]= {0};
    memset(message, 1, 1025);
    message[1025] = '\0';

    SET_TRACE(1);
    trace_marker_async_begin(HITRACE_TAG_MUSL, message, "trace_async",1);
    trace_marker_async_end(HITRACE_TAG_MUSL, message, "trace_async",1);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_async_sucess = true;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), message, "trace_async" , 1);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), message, "trace_async" , 1);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = false;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_async_begin and trace_marker_async_end, message is null.
 * @tc.level     : Level 2
 */
static void trace_marker_0160(void)
{
    clear_trace();
    SET_TRACE(1);
    trace_marker_async_begin(HITRACE_TAG_MUSL, NULL, "trace_async",1);
    trace_marker_async_end(HITRACE_TAG_MUSL, NULL, "trace_async",1);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_async_sucess = true;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s %d", getpid(), "trace_async" , 1);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s %d", getpid(), "trace_async" , 1);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = false;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_count, the size of messge is 1026.
 * @tc.level     : Level 2
 */
static void trace_marker_0180(void)
{
    clear_trace();
    SET_TRACE(1);
    int traceCount = 5;
    char message[1026]= {0};
    memset(message, 1, 1025);
    message[1025] = '\0';

    trace_marker_count(HITRACE_TAG_MUSL, message, traceCount);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_count_sucess = true;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), message, traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = false;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_count, message is null.
 * @tc.level     : Level 2
 */
static void trace_marker_0190(void)
{
    clear_trace();
    SET_TRACE(1);
    int traceCount = 5;
    trace_marker_count(HITRACE_TAG_MUSL, NULL, traceCount);
    SET_TRACE(0);

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_count_sucess = true;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};
    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d %d", getpid(), traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = false;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
}

TEST_FUN G_Fun_Array[] = {
    trace_marker_0010,
    trace_marker_0020,
    trace_marker_0030,
    trace_marker_0040,
    trace_marker_0050,
    trace_marker_0060,
    trace_marker_0070,
    trace_marker_0080,
    trace_marker_0090,
    trace_marker_0100,
    trace_marker_0110,
    trace_marker_0120,
    trace_marker_0140,
    trace_marker_0150,
    trace_marker_0160,
    trace_marker_0180,
    trace_marker_0190,
};

int main(void)
{
    trace_marker_reset();
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}