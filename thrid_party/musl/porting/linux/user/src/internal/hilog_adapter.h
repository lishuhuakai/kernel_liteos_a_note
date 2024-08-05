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

#ifndef MUSL_HILOG_ADAPTER_H
#define MUSL_HILOG_ADAPTER_H

#include <features.h>
#include <stdarg.h>
#include <stdbool.h>

#ifndef hidden
#define hidden __attribute__((visibility("hidden")))
#endif

// Log type
typedef enum {
    LOG_TYPE_MIN = 0,
    LOG_APP = 0,
    // Log to kmsg, only used by init phase.
    LOG_INIT = 1,
    // Used by core service, framework.
    LOG_CORE = 3,
    LOG_KMSG = 4,
    LOG_TYPE_MAX
} LogType;

// Log level
typedef enum {
    LOG_LEVEL_MIN = 0,
    LOG_DEBUG = 3,
    LOG_INFO = 4,
    LOG_WARN = 5,
    LOG_ERROR = 6,
    LOG_FATAL = 7,
    LOG_LEVEL_MAX = 8,
} LogLevel;

hidden int HiLogAdapterPrint(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt, ...)
    __attribute__((__format__(os_log, 5, 6)));

hidden int HiLogAdapterPrintArgs(const LogType type, const LogLevel level, const unsigned int domain, const char *tag,
                                 const char *fmt, va_list ap);
hidden int HiLogAdapterVaList(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt,
                              va_list ap);

hidden bool HiLogAdapterIsLoggable(unsigned int domain, const char *tag, LogLevel level);

#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
hidden bool get_bool_sysparam(CachedHandle cachedhandle);
#endif

hidden bool is_musl_log_enable();
hidden void musl_log_reset();

#endif  // MUSL_HILOG_ADAPTER_H
