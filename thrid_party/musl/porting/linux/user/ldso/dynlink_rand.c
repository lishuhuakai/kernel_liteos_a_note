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

#include <stdlib.h>
#include <sys/mman.h>
#include <sys/random.h>
#include <unistd.h>

#define HANDLE_INCREASE 2
#define TASK_BASE_CAPACITY 8

// linked list node for handle randomization
struct handle_node {
    struct handle_node *next;
    void *handle;
    struct dso *dso;
};

// linked list for handle randomization
static struct handle_node *handle_map_list = NULL;

static uintptr_t saved_handle = 0;

void *add_handle_node(void *handle, struct dso *dso)
{
    struct handle_node *node = __libc_malloc(sizeof(*node));
    if (!node) {
        return NULL;
    }
    node->handle = handle;
    node->dso = dso;
    node->next = handle_map_list;
    handle_map_list = node;
    return node;
}

struct dso *find_dso_by_handle(void *handle)
{
    struct handle_node *node = handle_map_list;
    while (node) {
        if (node->handle == handle) {
            return node->dso;
        }
        node = node->next;
    }
    return NULL;
}

void *find_handle_by_dso(struct dso *dso)
{
    struct handle_node *node = handle_map_list;
    while (node) {
        if (node->dso == dso) {
            return node->handle;
        }
        node = node->next;
    }
    return 0;
}

void remove_handle_node(void *handle)
{
    struct handle_node *node = handle_map_list;
    struct handle_node *pre_node = NULL;
    while (node) {
        if (node->handle == handle) {
            if (pre_node) {
                pre_node->next = node->next;
            } else {
                handle_map_list = node->next;
            }
            __libc_free(node);
            return;
        } else {
            pre_node = node;
            node = node->next;
        }
    }
}

static void *gen_handle(void)
{
    uintptr_t handle = saved_handle;
    do {
        if (getrandom(&handle, sizeof handle, GRND_RANDOM | GRND_NONBLOCK) == -1) {
            handle += HANDLE_INCREASE;
            saved_handle = handle;
        }
    } while (find_dso_by_handle((void *)handle) || handle == 0);
    return (void *)handle;
}

void *assign_valid_handle(struct dso *p)
{
    void *handle = find_handle_by_dso(p);
    if (handle == 0) {
        handle = gen_handle();
        if (!add_handle_node(handle, p)) {
            handle = 0;
        }
    }
    return handle;
}

struct loadtasks *create_loadtasks(void)
{
    struct loadtasks *tasks = __libc_malloc(sizeof(struct loadtasks));
    if (tasks) {
        tasks->array = NULL;
        tasks->capacity = 0;
        tasks->length = 0;
        return tasks;
    }
    return NULL;
}

bool append_loadtasks(struct loadtasks *tasks, struct loadtask *item)
{
    if (tasks->length + 1 > tasks->capacity) {
        size_t new_cap = 0;
        new_cap = tasks->capacity + TASK_BASE_CAPACITY;
        void *realloced = NULL;
        if (tasks->array) {
            realloced = __libc_realloc(tasks->array, new_cap * sizeof(struct loadtask *));
        } else {
            realloced = __libc_malloc(TASK_BASE_CAPACITY * sizeof(struct loadtask *));
        }
        if (realloced) {
            tasks->array = realloced;
            tasks->capacity = new_cap;
        } else {
            return false;
        }
    }
    tasks->array[tasks->length] = item;
    tasks->length += 1;
    return true;
}

void free_task(struct loadtask *task)
{
    if (task == NULL) {
        return;
    }
    if (task->name) {
        __libc_free(task->name);
        task->name = NULL;
    }
    if (task->allocated_buf) {
        __libc_free(task->allocated_buf);
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
    __libc_free(task);
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
            for (int i = 0; i < tasks->length; i++) {
                free_task(get_loadtask(tasks, i));
            }
            tasks->length = 0;
        }
        if (tasks->array) {
            __libc_free(tasks->array);
            tasks->array = NULL;
        }
        tasks->capacity = 0;
        __libc_free(tasks);
    }
}

void shuffle_loadtasks(struct loadtasks *tasks)
{
    size_t index = 0;
    struct loadtask *task = NULL;
    for (size_t i = 0; i < tasks->length; i++) {
        if (getrandom(&index, sizeof index, GRND_RANDOM | GRND_NONBLOCK) == -1) {
            return;
        } else {
            index %= tasks->length;
            task = tasks->array[i];
            tasks->array[i] = tasks->array[index];
            tasks->array[index] = task;
        }
    }
}

struct loadtask *create_loadtask(const char *name, struct dso *needed_by, ns_t *ns, bool check_inherited)
{
    size_t name_len = strlen(name);
    char *name_buf = (char *)__libc_malloc(name_len + 1);
    if (!name_buf) {
        return NULL;
    }
    struct loadtask *task = __libc_calloc(1, sizeof(struct loadtask));
    if (!task) {
        return NULL;
    }
    strcpy(name_buf, name);
    task->name = name_buf;
    task->needed_by = needed_by;
    task->namespace = ns;
    task->check_inherited = check_inherited;
    task->shdr_allocated_buf = MAP_FAILED;
    return task;
}
