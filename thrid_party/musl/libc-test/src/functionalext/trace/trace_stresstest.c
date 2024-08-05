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

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <test.h>
#include <unistd.h>

#define BUFFER_LEN 1000000
static const int count = 10;

/**
 * @tc.name      : trace_marker
 * @tc.desc      : The stress test of trace_marker.
 * @tc.level     : Level 0
 */
static void trace_marker_stresstest_0010(void)
{
    int pidChild = 0;
    int pidCParent = 0;
    int pidCChild = 0;

    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork! \n");
    } else if (fpid == 0) {
        int pidChild = getpid();
        int traceCount = 0;
        bool trace_async_sucess = false;
        char buf[BUFFER_LEN] = {0};

        while (traceCount <= 5000) {
            snprintf(buf, BUFFER_LEN, "%d", traceCount);
            system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
            trace_marker_async_begin(HITRACE_TAG_MUSL, "Trace_Marker_Async_Begin", buf, 1);
            trace_marker_async_end(HITRACE_TAG_MUSL, "Trace_Marker_Async_End", buf, 1);
            system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");
            printf("trace_marker_async_begin has been running times is:%d\n", traceCount);
            traceCount++;
        }
        exit(pidChild);
    } else {
        pid_t pidCChild;
        int traceCount = 0;
        char buf[BUFFER_LEN] = {0};

        // start process again
        pidCChild = fork();
        if (pidCChild < 0) {
            t_printf("error in fork!");
        } else if (pidCChild == 0) {
            while (traceCount <= 5000) {
                snprintf(buf, BUFFER_LEN, "%d", traceCount);
                system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
                trace_marker_begin(HITRACE_TAG_MUSL, "Trace_Marker", buf);
                trace_marker_end(HITRACE_TAG_MUSL);
                system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");
                printf("trace_marker_begin has been running times is:%d\n", traceCount);  
                traceCount++;
            }
            exit(pidCChild);
        } else {
            pidCParent = getpid();
            int traceCount = 0;

            while (traceCount <= 5000) {
                system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
                trace_marker_count(HITRACE_TAG_MUSL, "traceCount", traceCount);
                system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");
                printf("trace_marker_count has been running times is:%d\n", traceCount);
                traceCount++;
            }
            exit(pidCParent);
        }
    }
}

int main(void)
{
    trace_marker_stresstest_0010();

    return t_status;
}
