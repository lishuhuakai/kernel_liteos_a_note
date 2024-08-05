/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <hilog_adapter.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "musl_log.h"

#define PATH_MAX 256
#define DFX_LOG_LIB "libasan_logger.z.so"
#define DFX_LOG_INTERFACE "WriteGwpAsanLog"
static void (*g_dfxLogPtr)(char*, size_t);
static void* g_dfxLibHandler = NULL;
static pthread_mutex_t g_muslLogMutex = PTHREAD_MUTEX_INITIALIZER;

int ohos_dfx_log(const char *str)
{
    if (g_dfxLogPtr != NULL) {
        g_dfxLogPtr(str, strlen(str));
        return 0;
    }

    pthread_mutex_lock(&g_muslLogMutex);
    if (g_dfxLogPtr != NULL) {
        g_dfxLogPtr(str, strlen(str));
        pthread_mutex_unlock(&g_muslLogMutex);
        return 0;
    }
    if (g_dfxLibHandler == NULL) {
        g_dfxLibHandler = dlopen(DFX_LOG_LIB, RTLD_LAZY);
        if (g_dfxLibHandler == NULL) {
            MUSL_LOGE("[musl_log] dlopen %{public}s failed!\n", DFX_LOG_LIB);
            pthread_mutex_unlock(&g_muslLogMutex);
            return 0;
        }
    }
    if (g_dfxLogPtr == NULL) {
        *(void **)(&g_dfxLogPtr) = dlsym(g_dfxLibHandler, DFX_LOG_INTERFACE);
        if (g_dfxLogPtr != NULL) {
            g_dfxLogPtr(str, strlen(str));
        } else {
            MUSL_LOGE("[musl_log] dlsym %{public}s, failed!\n", DFX_LOG_INTERFACE);
        }
    }
    pthread_mutex_unlock(&g_muslLogMutex);

    return 0;
}

int musl_log(const char *fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = HiLogAdapterPrintArgs(MUSL_LOG_TYPE, LOG_INFO, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, fmt, ap);
    va_end(ap);
    return ret;
}
