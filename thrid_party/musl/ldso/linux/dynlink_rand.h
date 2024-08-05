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

#ifndef DYNLINK_RAND_H
#define DYNLINK_RAND_H

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "libc.h"
#include "dynlink.h"
#include "namespace.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dso;

#define READ_ELF_LENGTH 896

// load library context
struct loadtask {
    // parameters
    const char *name;
    struct dso *needed_by;
    ns_t *namespace;
    bool check_inherited;
    bool isloaded;

    // variables for load library
    char buf[PATH_MAX + 1];
    const char *pathname;
    struct dso *p;
    int fd;
    uint64_t file_offset; /* Used to read an uncompress library from a zip file, file_offset is relative offset of start of zip file. */

    // variables for map library
    Ehdr ehdr_buf[(READ_ELF_LENGTH + sizeof(Ehdr)) / sizeof(Ehdr)];
    void *allocated_buf;
    size_t phsize;
    Ehdr *eh;
    Phdr *ph0;
    size_t dyn;
    size_t tls_image;

    // variables for segment header
    void *shdr_allocated_buf;
    size_t shsize;

    void *dyn_map;
    size_t dyn_map_len;
    size_t *dyn_addr;
    void *str_map;
    size_t str_map_len;
    char *str_addr;
    struct tls_module tls;
};

// dynamic array for loadtask
struct loadtasks {
    struct loadtask **array;
    size_t capacity;
    size_t length;
};

hidden void *add_handle_node(void *handle, struct dso *dso);
hidden struct dso *find_dso_by_handle(void *handle);
hidden void *find_handle_by_dso(struct dso *dso);
hidden void remove_handle_node(void *handle);
hidden void *assign_valid_handle(struct dso *p);

hidden struct loadtasks *create_loadtasks(void);
hidden bool append_loadtasks(struct loadtasks *tasks, struct loadtask *item);
hidden void free_task(struct loadtask *task);
hidden struct loadtask *get_loadtask(struct loadtasks *tasks, size_t index);
hidden void free_loadtasks(struct loadtasks *tasks);
hidden void shuffle_loadtasks(struct loadtasks *tasks);
hidden struct loadtask *create_loadtask(const char *name, struct dso *needed_by, ns_t *ns, bool check_inherited);

#ifdef __cplusplus
}
#endif

#endif