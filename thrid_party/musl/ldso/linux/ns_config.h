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

#ifndef _NS_CONFIG_H
#define _NS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "strops.h"
/*
 * The following #include is necessary on many Unixes but not Linux.
 * It is not needed for Windows platforms.
 * Uncomment it if needed.
 */
/* #include <unistd.h> */

typedef struct _kv_list_ {
    size_t num;     /** Number of entries in list */
    size_t size;  /** Storage size */
    char **key;   /** List of string keys */
    char **val;   /** List of string values */
} kvlist;

typedef struct _section_list_ {
  size_t num;
  size_t size;
  char **names;
  kvlist **kvs;
} section_list;

typedef struct _ns_configor_ {
  char *file_path;
  char *exe_path;
  char *config_sys_path;
  char *config_asan_sys_path;
  section_list *sections;
  kvlist *kvs;

  void (*set_error_callback)(int (*errback)(const char *, ...));
  int (*parse)(const char *file_path, const char *exe_path);
  strlist *(*get_namespaces)(void);
  char *(*get_lib_paths)(const char *ns_name);
  char *(*get_asan_lib_paths)(const char *ns_name);
  char *(*get_permitted_paths)(const char *ns_name);
  char *(*get_asan_permitted_paths)(const char *ns_name);
  bool (*get_separated)(const char *ns_name);
  strlist *(*get_inherits)(const char *ns_name);
  char *(*get_allowed_libs)(const char *ns_name);
  char *(*get_inherit_shared_libs)(const char *ns_name, const char *inherited_ns_name);
  char *(*get_sys_paths)(void);
  char *(*get_asan_sys_paths)(void);
} ns_configor;

ns_configor *configor_init();
void configor_free();


#ifdef __cplusplus
}
#endif

#endif
