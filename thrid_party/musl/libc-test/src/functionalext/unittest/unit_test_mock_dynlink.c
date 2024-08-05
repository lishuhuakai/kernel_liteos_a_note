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
#include "dynlink_rand.h"
#include <sys/mman.h>
#include <unistd.h>

#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif

#ifdef UNIT_TEST_STATIC
void free_task(struct loadtask *task)
{
    if (task == NULL) {
        return;
    }
    if (task->name) {
        free((void *)task->name);
        task->name = NULL;
    }
    if (task->allocated_buf) {
        free(task->allocated_buf);
        task->allocated_buf = NULL;
    }
    if (task->dyn_map_len) {
        munmap(task->dyn_map, task->dyn_map_len);
        task->dyn_map = NULL;
        task->dyn_map_len = 0;
    }
    if (task->str_map_len) {
        munmap(task->str_map, task->str_map_len);
        task->str_map = NULL;
        task->str_map_len = 0;
    }
    if (task->fd != -1 && task->fd) {
        close(task->fd);
        task->fd = -1;
    }
    free(task);
}

struct loadtask *get_loadtask(struct loadtasks *tasks, size_t index)
{
    if (tasks && tasks->array && (index < tasks->length)) {
        return tasks->array[index];
    } else {
        return NULL;
    }
}

void free_loadtasks(struct loadtasks *tasks)
{
    if (tasks) {
        if (tasks->length) {
            for (size_t i = 0; i < tasks->length; i++) {
                free_task(get_loadtask(tasks, i));
            }
            tasks->length = 0;
        }
        if (tasks->array) {
            free(tasks->array);
            tasks->array = NULL;
        }
        tasks->capacity = 0;
        free(tasks);
    }
}
#endif

bool get_ld_debug_dlclose_value()
{
#ifdef OHOS_ENABLE_PARAMETER
    static CachedHandle param_handle = NULL;
    if (param_handle == NULL) {
        param_handle = CachedParameterCreate("musl.ld.debug.dlclose", "false");
    }

    const char *param_value = CachedParameterGet(param_handle);
    if (param_value != NULL) {
        if (strcmp(param_value, "true") == 0) {
            return true;
        }
    }
#endif
    return false;
}