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

#include "ld_log.h"

static bool ld_log_enable = false;
static bool ld_dlclose_debug = false;

#ifdef OHOS_ENABLE_PARAMETER
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "sys_param.h"

#define SYSPARAM_LENGTH 32
#define PROCESS_NAME_LEN 1024

static char *get_app_name(char *buf, size_t length)
{
    char *app = NULL;
    int fd = open("/proc/self/cmdline", O_RDONLY);
    if (fd != -1) {
        ssize_t ret = read(fd, buf, length - 1);
        if (ret != -1) {
            buf[ret] = 0;
            app = strrchr(buf, '/');
            if (app) {
                app++;
            } else {
                app = buf;
            }
            char *app_end = strchr(app, ':');
            if (app_end) {
                *app_end = 0;
            }
        }
        close(fd);
    }
    return app;
}

static bool get_ld_log_value()
{
    char buf[PROCESS_NAME_LEN];
    char *path = get_app_name(buf, PROCESS_NAME_LEN);
    if (!path) {
        return false;
    }

    char app_param_name[PROCESS_NAME_LEN] = "musl.log.ld.app.";
    strcat(app_param_name, path);
    static CachedHandle app_param_handle = NULL;
    static CachedHandle all_param_handle = NULL;
    if (app_param_handle == NULL) {
        app_param_handle = CachedParameterCreate(app_param_name, "false");
    }
    if (all_param_handle == NULL) {
        all_param_handle = CachedParameterCreate("musl.log.ld.all", "false");
    }
    return (get_bool_sysparam(app_param_handle) || get_bool_sysparam(all_param_handle));
}

static bool get_ld_debug_dlclose_value()
{
    static CachedHandle param_handle = NULL;
    if (param_handle == NULL) {
        param_handle = CachedParameterCreate("musl.ld.debug.dlclose", "false");
    }
    return get_bool_sysparam(param_handle);
}
#endif

void ld_log_reset()
{
#if (defined(OHOS_ENABLE_PARAMETER))
    ld_dlclose_debug = get_ld_debug_dlclose_value();
    if (!is_musl_log_enable()) {
        ld_log_enable = false;
        return;
    }
    ld_log_enable = get_ld_log_value();
#else
    ld_log_enable = is_musl_log_enable();
#endif
}

bool get_ld_log_enable()
{
    return ld_log_enable;
}

bool is_dlclose_debug_enable()
{
    return ld_dlclose_debug;
}
