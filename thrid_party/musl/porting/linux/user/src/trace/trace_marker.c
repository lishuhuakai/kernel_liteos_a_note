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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "musl_log.h"
#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif

#ifndef MUSL_TEMP_FAILURE_RETRY
#define MUSL_TEMP_FAILURE_RETRY(exp)           \
    ({                                         \
    long int _rc;                              \
    do {                                       \
        _rc = (long int)(exp);                 \
    } while ((_rc == -1) && (errno == EINTR)); \
    _rc;                                       \
    })
#endif

#define LIKELY(exp) (__builtin_expect(!!(exp), 1))

#ifdef OHOS_ENABLE_PARAMETER
#define TRACE_PROPERTY_FLAG "debug.hitrace.tags.enableflags"
static uint64_t g_trace_switch_status = 0;

uint64_t get_uint64_sysparam(CachedHandle cachedhandle)
{
    char *param_value = CachedParameterGet(cachedhandle);
    if (param_value != NULL) {
        return strtoull(param_value, NULL, 0);
    }
    return 0;
}
#endif

void trace_marker_reset(void)
{
#ifdef OHOS_ENABLE_PARAMETER
    static CachedHandle trace_switch_handle = NULL;
    if (trace_switch_handle == NULL) {
        trace_switch_handle = CachedParameterCreate(TRACE_PROPERTY_FLAG, "0");
    }
    g_trace_switch_status = get_uint64_sysparam(trace_switch_handle);
#else
    return;
#endif
}

// Check whether the user space trace function is enabled
static inline bool is_enable_trace(uint64_t label)
{
#ifdef OHOS_ENABLE_PARAMETER
    return (((g_trace_switch_status & label) != 0) || ((g_trace_switch_status & HITRACE_TAG_ALWAYS) != 0));
#else
    return false;
#endif
}

// Get the fd of trace_marker
static inline int get_trace_marker_fd(void)
{
    int trace_marker_fd = MUSL_TEMP_FAILURE_RETRY(open("/sys/kernel/tracing/trace_marker", O_CLOEXEC | O_WRONLY | O_APPEND));
    if (trace_marker_fd == -1) {
        trace_marker_fd = MUSL_TEMP_FAILURE_RETRY(open("/sys/kernel/debug/tracing/trace_marker", O_CLOEXEC | O_WRONLY | O_APPEND));
    }
    return trace_marker_fd;
}

/* Write the function call information to the trace_marker node in kernel space,
used on the same thread as trace_marker_end(),with the symbol "B". */
void trace_marker_begin(uint64_t label, const char *message, const char *value)
{
    if (LIKELY((!is_enable_trace(label) || message == NULL))) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = 0;
    if (value == NULL) {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "B|%d|%s", getpid(), message);
    } else {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "B|%d|%s %s", getpid(), message, value);
    }
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write B. %d", ret);
        }
    } else {
        MUSL_LOGE("B length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

/* Write the terminator to the trace_marker node of the kernel space,
used on the same thread as trace_marker_begin(),with the symbol "E". */
void trace_marker_end(uint64_t label)
{
    if (LIKELY(!is_enable_trace(label))) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "E|%d", getpid());
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write E. %d", ret);
        }
    } else {
        MUSL_LOGE("E length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

/* Write the function call information to the trace_marker node in kernel space,
used in a different thread than trace_marker_async_end(),with the symbol "S". */
void trace_marker_async_begin(uint64_t label, const char *message, const char *value, int taskId)
{
    if (LIKELY((!is_enable_trace(label) || message == NULL))) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = 0;
    if (value == NULL) {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "S|%d|%s %d", getpid(), message, taskId);
    } else {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "S|%d|%s|%s %d", getpid(), message, value, taskId);
    }
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write S. %d", ret);
        }
    } else {
        MUSL_LOGE("S length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

/* Write the terminator to the trace_marker node in kernel space,
used in a different thread than trace_marker_async_begin(),with the symbol "F". */
void trace_marker_async_end(uint64_t label, const char *message, const char *value, int taskId)
{
    if (LIKELY((!is_enable_trace(label) || message == NULL))) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = 0;
    if (value == NULL) {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "F|%d|%s %d", getpid(), message, taskId);
    } else {
        len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "F|%d|%s|%s %d", getpid(), message, value, taskId);
    }
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write F. %d", ret);
        }
    } else {
        MUSL_LOGE("F length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}

// A numeric variable used to mark a pre trace, with the symbol "C".
void trace_marker_count(uint64_t label, const char *message, int value)
{
    if (LIKELY((!is_enable_trace(label) || message == NULL))) {
        return;
    }

    int trace_marker_fd = get_trace_marker_fd();
    if (trace_marker_fd == -1) {
        return;
    }

    char buf[TRACE_MARKER_MESSAGE_LEN] = {0};
    int len = snprintf(buf, TRACE_MARKER_MESSAGE_LEN, "C|%d|%s %d", getpid(), message, value);
    if (len > 0 && len < sizeof(buf)) {
        int ret = MUSL_TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
        if (ret == -1) {
            MUSL_LOGE("fail to write C. %d", ret);
        }
    } else {
        MUSL_LOGE("C length error. %d", len);
    }

    // close file descriptor of trace_marker
    close(trace_marker_fd);
}
