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

#ifndef _NAMESPACE_H
#define _NAMESPACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "strops.h"

#define NS_DEFAULT_NAME "default"
struct dso;
/* define dso list */
typedef struct _dso_list_ {
    uint16_t num;
    uint16_t size;
    struct dso **dsos;    
} dsolist;
/* define namespace struct */
struct _ns_inherit_list_;
typedef struct _namespace_t_ {
    char *ns_name;            /* namespace name */
    char *env_paths;          /* value of LD_LIBRARY_PATH. splited by ':'. */
    char *lib_paths;          /* library search paths splited by ':'. */

    char *asan_lib_paths;          /* when asan is enable, library search paths splited by ':'. */
    strlist *permitted_paths;    /* when separated, permitted search paths splited by ':', including sub dirs. */
    strlist *asan_permitted_paths;    /* when asan is enable and separated,the same as above.  */

    bool separated;           /* if separated */
    strlist *allowed_libs;       /* when separated, allowed library names splited by ':'. */
    dsolist *ns_dsos;         /* dso list in this namespace */
    struct _ns_inherit_list_ *ns_inherits;   /* inherit list in this namespace */
    int flag;
} ns_t;
/* define namespace list */
typedef struct _namespaces_list_ {
    uint16_t num;
    uint16_t size;
    ns_t **nss;
} nslist;
/* define namespace inherit */
typedef struct _namespace_inherit_ {
    ns_t *inherited_ns;       /* inherited namespace */
    strlist *shared_libs;        /* when inherited, shared library names splited by ':'. */
} ns_inherit;
/* define namespace inherit list */
typedef struct _ns_inherit_list_ {
    uint16_t num;
    uint16_t size;
    ns_inherit **inherits;
} ns_inherit_list;

/* init g_ns_list */
nslist *nslist_init();

/* namespace funcs */
ns_t *ns_alloc();
void ns_free(ns_t *ns);
void ns_set_name(ns_t *ns, const char *name);
void ns_set_env_paths(ns_t *ns, const char *env_paths);
void ns_set_lib_paths(ns_t *ns, const char *lib_paths);
void ns_set_asan_lib_paths(ns_t *ns, const char *asan_lib_paths);
void ns_set_permitted_paths(ns_t *ns, const char *permitted_paths);
void ns_set_asan_permitted_paths(ns_t *ns, const char *asan_permitted_paths);
void ns_set_separated(ns_t *ns, bool separated);
void ns_set_allowed_libs(ns_t *ns, const char *allowed_libs);
void ns_add_dso(ns_t *ns, struct dso *dso);
void nslist_add_ns(ns_t *ns);
void ns_add_inherit(ns_t *ns,ns_t *inherited, const char *shared_libs);
void ns_set_flag(ns_t *ns, int flag);

/* get default namespace */
ns_t *get_default_ns();

/* check if library pathname is accessible in the namespace */
bool is_accessible(ns_t *ns, const char *lib_pathname, bool is_asan, bool check_inherited);

/* check if asan_lib_paths or asan_permitted_paths pathname is accessible in the namespace */
bool check_asan_path(ns_t *ns, const char *lib_pathname);

/* check if library is sharable in the inherited namespace */
bool is_sharable(ns_inherit *inherit, const char *lib_name);

/* find namespace by name */
ns_t *find_ns_by_name(const char *ns_name);

#ifdef __cplusplus
}
#endif

#endif