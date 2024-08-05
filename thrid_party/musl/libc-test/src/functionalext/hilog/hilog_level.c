/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#ifdef OHOS_ENABLE_PARAMETER
#undef _GNU_SOURCE
#include <hilog/hilog_adapter.c>
#define _GNU_SOURCE
#include <musl_log.h>
#include <test.h>
#endif
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define OVERWRITE 1
#define ZERO 0
#define INVALID_ENV (-1)
#define VALID_ENV 0
#define ENABLE_LOG "param set musl.log.enable true"
#define LOG_LEVEL_FATAL "param set musl.log.level FATAL"
#define LOG_LEVEL_WARN "param set musl.log.level WARN"
#define LOG_LEVEL_ERROR "param set musl.log.level ERROR"
#define LOG_LEVEL_DEBUG "param set musl.log.level DEBUG"
#define LOG_LEVEL_INFO "param set musl.log.level INFO"

int TestValidEnv(const char *str)
{
    FILE *res = popen(str, "r");
    if (res == NULL) {
        return INVALID_ENV;
    }
    char path[1035];
    while (fgets(path, sizeof(path), res) != NULL) {
        if (strstr(path, "fail")) {
            return INVALID_ENV;
        }
    }
    pclose(res);
    return VALID_ENV;
}

void TestFatalLevel()
{
    int result = TestValidEnv(LOG_LEVEL_FATAL);
    if (result == INVALID_ENV) {
        return;
    }
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_FATAL level LOG_FATAL print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_FATAL level LOG_ERROR print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_FATAL level LOG_WARN print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_FATAL level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_FATAL level LOG_DEBUG print failed \n");
    }
}

void TestErrorLevel()
{
    int result = TestValidEnv(LOG_LEVEL_ERROR);
    if (result == INVALID_ENV) {
        return;
    }
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_ERROR level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_ERROR level LOG_ERROR print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_ERROR level LOG_WARN print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_ERROR level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_ERROR level LOG_DEBUG print failed \n");
    }
}

void TestWarnLevel()
{
    int result = TestValidEnv(LOG_LEVEL_WARN);
    if (result == INVALID_ENV) {
        return;
    }
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_WARN level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_WARN level LOG_ERROR print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_WARN level LOG_WARN print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_WARN level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_WARN level LOG_DEBUG print failed \n");
    }
}

void TestInfoLevel()
{
    int result = TestValidEnv(LOG_LEVEL_INFO);
    if (result == INVALID_ENV) {
        return;
    }
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_INFO level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_INFO level LOG_ERROR print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_INFO level LOG_WARN print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_INFO level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_INFO level LOG_DEBUG print failed \n");
    }
}

void TestDebugLevel()
{
    int result = TestValidEnv(LOG_LEVEL_DEBUG);
    if (result == INVALID_ENV) {
        return;
    }
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_DEBUG level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_DEBUG level LOG_ERROR print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_DEBUG level LOG_WARN print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_DEBUG level LOG_INFO print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_DEBUG level LOG_DEBUG print failed \n");
    }
}

int main()
{
#ifdef OHOS_ENABLE_PARAMETER
    int result = TestValidEnv(ENABLE_LOG);
    if (result == INVALID_ENV) {
        return ZERO;
    }
    TestFatalLevel();
    TestErrorLevel();
    TestWarnLevel();
    TestInfoLevel();
    TestDebugLevel();
#endif
    return t_status;
}