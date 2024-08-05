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

#include "namespace.h"

#include "ld_log.h"
#include "strops.h"

static ns_t g_ns_default;
static nslist g_ns_list;

#ifndef NSLIST_DEFAULT_SIZE
#define NSLIST_DEFAULT_SIZE 16
#endif
#ifndef DSOLIST_DEFAULT_SIZE
#define DSOLIST_DEFAULT_SIZE 16
#endif
#ifndef INHERIT_DEFAULT_SIZE
#define INHERIT_DEFAULT_SIZE 16
#endif

#ifdef UNIT_TEST_STATIC
    #define UT_STATIC
#else
    #define UT_STATIC static
#endif

#define ALLOW_ALL_SHARED_LIBS "allow_all_shared_libs"

static ns_inherit_list *nsinherits_alloc()
{
    ns_inherit_list *nsinl;
    nsinl = (ns_inherit_list *)__libc_calloc(1, sizeof *nsinl);

    if (nsinl) {
        nsinl->size = INHERIT_DEFAULT_SIZE;
        nsinl->inherits = (ns_inherit **)__libc_calloc(INHERIT_DEFAULT_SIZE, sizeof *nsinl->inherits);
        if (!nsinl->inherits) {
            LD_LOGE("nsinherits_alloc failed,return NULL!");
            __libc_free(nsinl);
            nsinl = NULL;
        }
    }
    return nsinl;
}

static void nsinherits_free(ns_inherit_list *nsinl)
{
    if (!nsinl) {
        return;
    }
    for (size_t i = 0; i < nsinl->num; i++) {
        strlist_free(nsinl->inherits[i]->shared_libs);
        __libc_free(nsinl->inherits[i]);
    }
    __libc_free(nsinl->inherits);
    __libc_free(nsinl);
}

UT_STATIC void nsinherits_realloc(ns_inherit_list *nsinl)
{
    if (!nsinl) {
        return;
    }
    size_t size = 2 * nsinl->size;
    if (size) {
        ns_inherit **inherits;
        inherits = (ns_inherit **)__libc_realloc(nsinl->inherits, size * (sizeof *nsinl->inherits));
        if (!inherits) {
            LD_LOGE("nsinherits_realloc failed!");
            return;
        }
        nsinl->size = size;
        nsinl->inherits = inherits;
    }
    return;
}

static dsolist *dsolist_alloc()
{
    dsolist *dsol;
    dsol = (dsolist *)__libc_calloc(1, sizeof *dsol);

    if (dsol) {
        dsol->size = DSOLIST_DEFAULT_SIZE;
        dsol->dsos = (struct dso **)__libc_calloc(DSOLIST_DEFAULT_SIZE, sizeof *dsol->dsos);
        if (!dsol->dsos) {
            LD_LOGE("dsolist_alloc failed,return NULL!");
            __libc_free(dsol);
            dsol = NULL;
        }
    }
    return dsol;
}

static void dsolist_realloc(dsolist *dsol)
{
    if (!dsol) {
        return;
    }
    size_t size = 2 * dsol->size;
    if (size) {
        struct dso **ds;
        ds = (struct dso **)__libc_realloc(dsol->dsos, size * (sizeof *dsol->dsos));
        if (!ds) {
            LD_LOGE("dsolist_realloc failed!");
            return;
        }
        dsol->size = size;
        dsol->dsos = ds;
    }
    return;
}

ns_t *ns_alloc()
{
    ns_t *nst = (ns_t *)__libc_calloc(1, sizeof *nst);
    nst->ns_dsos = dsolist_alloc();
    if (!nst->ns_dsos) {
        LD_LOGE("ns_alloc failed,return NULL!");
        __libc_free(nst);
        nst = NULL;
    }
    return nst;
}

void ns_free(ns_t *ns)
{
    if (!ns) {
        return;
    }
    if (ns->ns_dsos) {
        __libc_free(ns->ns_dsos);
        ns->ns_dsos = NULL;
    }
    if (ns->ns_name) {
        __libc_free(ns->ns_name);
        ns->ns_name = NULL;
    }
    if (ns->env_paths) {
        __libc_free(ns->env_paths);
        ns->env_paths = NULL;
    }
    if (ns->lib_paths) {
        __libc_free(ns->lib_paths);
        ns->lib_paths = NULL;
    }
    if (ns->asan_lib_paths) {
        __libc_free(ns->asan_lib_paths);
        ns->asan_lib_paths = NULL;
    }
    strlist_free(ns->permitted_paths);
    strlist_free(ns->asan_permitted_paths);
    strlist_free(ns->allowed_libs);
    nsinherits_free(ns->ns_inherits);
    __libc_free(ns);
}

void ns_add_dso(ns_t *ns, struct dso *dso)
{
    if (!ns || !dso) {
        return;
    }
    if (!ns->ns_dsos) {
        ns->ns_dsos = dsolist_alloc();
    }
    if (!ns->ns_dsos) {
        return;
    }
    if (ns->ns_dsos->num == ns->ns_dsos->size) {
        /* if list is full, realloc size to double*/
        dsolist_realloc(ns->ns_dsos);
    }
    if (ns->ns_dsos->num < ns->ns_dsos->size) {
        /* realloc succ */
        ns->ns_dsos->dsos[ns->ns_dsos->num] = dso;
        ns->ns_dsos->num++;
    }
    return;
}

nslist *nslist_init()
{
    g_ns_list.size = NSLIST_DEFAULT_SIZE;
    g_ns_list.num = 0;
    g_ns_list.nss = (ns_t **)__libc_calloc(NSLIST_DEFAULT_SIZE, sizeof *g_ns_list.nss);
    if (!g_ns_list.nss) {
        LD_LOGE("nslist_init failed!");
        return NULL;
    }
    return &g_ns_list;
}

static void nslist_realloc()
{
    size_t size = 2 * g_ns_list.size;
    if (size) {
        ns_t **nss;
        nss = (ns_t **)__libc_realloc(g_ns_list.nss, size * (sizeof *g_ns_list.nss));
        if (!nss) {
            LD_LOGE("nslist_realloc failed!");
            return;
        }
        g_ns_list.size = size;
        g_ns_list.nss = nss;
    }
    return;
}

void nslist_add_ns(ns_t *ns)
{
    if (!ns) {
        return;
    }

    if (g_ns_list.num == g_ns_list.size) {
        /* if list is full, realloc size to double*/
        nslist_realloc();
    }
    if (g_ns_list.num < g_ns_list.size) {
        /* realloc succ */
        g_ns_list.nss[g_ns_list.num] = ns;
        g_ns_list.num++;
    }
    return;
}

ns_t *get_default_ns()
{
    return &g_ns_default;
}

/* set namespace attributes*/
void ns_set_name(ns_t *ns, const char *name)
{
    if (!ns || !name) {
        return;
    }
    if (ns->ns_name) __libc_free(ns->ns_name);
    ns->ns_name = ld_strdup(name);
    strtrim(ns->ns_name);
    LD_LOGD("ns_set_name ns_name:%{public}s.", ns->ns_name);
}

void ns_set_env_paths(ns_t *ns, const char *env_paths)
{
    if (!ns) {
        return;
    }
    if (ns->env_paths) __libc_free(ns->env_paths);
    if (env_paths) {
        ns->env_paths = ld_strdup(env_paths);
        strtrim(ns->env_paths);
    } else {
        ns->env_paths = NULL;
    }
    LD_LOGD("ns_set_env_paths ns[%{public}s] env_paths:%{public}s.", ns->ns_name, ns->env_paths);
}

void ns_set_lib_paths(ns_t *ns, const char *lib_paths)
{
    if (!ns) {
        return;
    }
    if (ns->lib_paths) __libc_free(ns->lib_paths);
    if (lib_paths) {
        ns->lib_paths = ld_strdup(lib_paths);
        strtrim(ns->lib_paths);
    } else {
        ns->lib_paths = NULL;
    }
    LD_LOGD("ns_set_lib_paths ns[%{public}s] lib_paths:%{public}s.", ns->ns_name, ns->lib_paths);
}

void ns_set_asan_lib_paths(ns_t *ns, const char *asan_lib_paths)
{
    if (!ns) {
        return;
    }
    if (ns->asan_lib_paths) {
        __libc_free(ns->asan_lib_paths);
    }
    if (asan_lib_paths) {
        ns->asan_lib_paths = ld_strdup(asan_lib_paths);
        strtrim(ns->asan_lib_paths);
    } else {
        ns->asan_lib_paths = NULL;
    }
    LD_LOGD("ns_set_asan_lib_paths ns[%{public}s] asan_lib_paths:%{public}s.", ns->ns_name, ns->asan_lib_paths);
}

void ns_set_permitted_paths(ns_t *ns, const char *permitted_paths)
{
    if (!ns) {
        return;
    }
    if (ns->permitted_paths) strlist_free(ns->permitted_paths);
    ns->permitted_paths = strsplit(permitted_paths, ":");
    LD_LOGD("ns_set_permitted_paths ns[%{public}s] permitted_paths:%{public}s.", ns->ns_name, permitted_paths);
}

void ns_set_asan_permitted_paths(ns_t *ns, const char *asan_permitted_paths)
{
    if (!ns) {
        return;
    }
    if (ns->asan_permitted_paths) {
        strlist_free(ns->asan_permitted_paths);
    }
    ns->asan_permitted_paths = strsplit(asan_permitted_paths, ":");
    LD_LOGD("ns_set_asan_permitted_paths ns[%{public}s] asan_permitted_paths:%{public}s.",
        ns->ns_name,
        asan_permitted_paths);
}

void ns_set_separated(ns_t *ns, bool separated)
{
    if (!ns) {
        return;
    }
    ns->separated = separated;
    LD_LOGD("ns_set_separated ns[%{public}s] separated:%{public}d.", ns->ns_name, ns->separated);
}

void ns_set_allowed_libs(ns_t *ns, const char *allowed_libs)
{
    if (!ns) {
        return;
    }

    if (ns->allowed_libs) strlist_free(ns->allowed_libs);
    ns->allowed_libs = NULL;
    if (allowed_libs) {
        /* if setted and not empty, split to list. */
        char *a_libs = ld_strdup(allowed_libs);
        if (strtrim(a_libs) > 0) ns->allowed_libs = strsplit(a_libs, ":");
        __libc_free(a_libs);
    }
    LD_LOGD("ns_set_allowed_libs ns[%{public}s] allowed_libs:%{public}s.", ns->ns_name, allowed_libs);
}

ns_t *find_ns_by_name(const char *ns_name)
{
    if (!ns_name) {
        return NULL;
    }
    if (strcmp(NS_DEFAULT_NAME, ns_name) == 0) {
        LD_LOGD("find_ns_by_name return default namespace!");
        return get_default_ns();
    }
    for (size_t i = 0; i < g_ns_list.num; i++) {
        if (strcmp(g_ns_list.nss[i]->ns_name, ns_name) == 0) {
            return g_ns_list.nss[i];
        }
    }
    LD_LOGD("find_ns_by_name ns_name[%{public}s] failed,return NULL!", ns_name);
    return NULL;
}

static ns_inherit *find_ns_inherit(ns_t *ns, ns_t *inherited)
{
    if (!ns || !inherited) {
        return NULL;
    }
    if (ns->ns_inherits) {
        for (size_t i = 0; i < ns->ns_inherits->num; i++) {
            if (ns->ns_inherits->inherits[i]->inherited_ns == inherited) return ns->ns_inherits->inherits[i];
        }
    }
    LD_LOGD(
        "find_ns_inherit ns[%{public}s] ns_inherited[%{public}s] failed,return NULL!", ns->ns_name, inherited->ns_name);
    return NULL;
}

void ns_add_inherit(ns_t *ns, ns_t *ns_inherited, const char *shared_libs)
{
    bool need_add = false;
    if (!ns || !ns_inherited) {
        return;
    }

    ns_inherit *inherit = find_ns_inherit(ns, ns_inherited);
    if (!inherit) {
        inherit = __libc_calloc(1, sizeof *inherit);
        if (!inherit) {
            LD_LOGE("ns_add_inherit ns[%{public}s] ns_inherited[%{public}s] calloc failed!",
                ns->ns_name,
                ns_inherited->ns_name);
            return;
        }
        inherit->inherited_ns = ns_inherited;
        need_add = true;
        LD_LOGD("ns_add_inherit ns[%{public}s] ns_inherited[%{public}s] need_add is true.",
            ns->ns_name,
            ns_inherited->ns_name);
    }

    if (inherit->shared_libs) {
        strlist_free(inherit->shared_libs);
        inherit->shared_libs = NULL;
    }

    /* if setted and not empty, split to list. */
    if (shared_libs) {
        char *s_libs = ld_strdup(shared_libs);
        if (strtrim(s_libs) > 0) inherit->shared_libs = strsplit(shared_libs, ":");
        __libc_free(s_libs);
    }

    if (!need_add) {
        LD_LOGD(
            "ns_add_inherit ns[%{public}s] ns_inherited[%{public}s] not need_add!", ns->ns_name, ns_inherited->ns_name);
        return;
    }

    if (!ns->ns_inherits) {
        ns->ns_inherits = nsinherits_alloc();
    }

    if (!ns->ns_inherits) {
        if (inherit->shared_libs) strlist_free(inherit->shared_libs);
        LD_LOGD("ns_add_inherit ns[%{public}s] ns_inherited[%{public}s] nsinherits_alloc failed!",
            ns->ns_name,
            ns_inherited->ns_name);
        __libc_free(inherit);
        return;
    }

    if (ns->ns_inherits->num == ns->ns_inherits->size) {
        /* if list is full, realloc size to double*/
        LD_LOGD("ns_add_inherit ns[%{public}s] ns_inherited[%{public}s] list is full, realloc size to double!",
            ns->ns_name,
            ns_inherited->ns_name);
        nsinherits_realloc(ns->ns_inherits);
    }

    if (ns->ns_inherits->num < ns->ns_inherits->size) {
        /* realloc succ */
        LD_LOGD("ns_add_inherit ns[%{public}s] ns_inherited[%{public}s] realloc success!",
            ns->ns_name,
            ns_inherited->ns_name);
        ns->ns_inherits->inherits[ns->ns_inherits->num] = inherit;
        ns->ns_inherits->num++;
    } else {
        /* realloc failed */
        LD_LOGD("ns_add_inherit ns[%{public}s] ns_inherited[%{public}s] realloc failed!",
            ns->ns_name,
            ns_inherited->ns_name);
        if (inherit->shared_libs) strlist_free(inherit->shared_libs);
        __libc_free(inherit);
    }
    return;
}

/* check library's pathname if accessible in this namespace */
bool is_accessible(ns_t *ns, const char *lib_pathname, bool is_asan, bool check_inherited)
{
    if (check_inherited && !ns->separated) {
        LD_LOGD("is_accessible ns [%{public}s] is not separated, return true.", ns->ns_name);
        return true;
    }
    if (ns->allowed_libs) {
        char *shortname = strrchr(lib_pathname, '/');
        if (shortname) {
            shortname += 1;
            size_t i = 0;
            for (; i < ns->allowed_libs->num; i++) {
                if (strcmp(shortname, ns->allowed_libs->strs[i]) == 0) {
                    break;
                }
            }
            if (i >= ns->allowed_libs->num) {
                LD_LOGD("is_accessible ns [%{public}s] lib_pathname [%{public}s] is not in allowed_libs, return false.",
                    ns->ns_name,
                    lib_pathname);
                return false;
            }
        }
    }
    strlist *paths;
    if (ns->env_paths && (paths = strsplit(ns->env_paths, ":"))) {
        for (size_t i = 0; i < paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname + len + 1, '/')) {
                LD_LOGD("is_accessible ns [%{public}s] lib_pathname [%{public}s] in env_paths, return true.",
                    ns->ns_name,
                    lib_pathname);
                strlist_free(paths);
                return true;
            }
        }
        strlist_free(paths);
    }

    if (is_asan) {
        if (check_asan_path(ns, lib_pathname)) {
            LD_LOGD("is_accessible ns [%{public}s] lib_pathname [%{public}s] check_asan_path success, return true.",
                ns->ns_name,
                lib_pathname);
            return true;
        }
    }

    if (ns->lib_paths && (paths = strsplit(ns->lib_paths, ":"))) {
        for (size_t i = 0; i < paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname + len + 1, '/')) {
                strlist_free(paths);
                LD_LOGD("is_accessible ns [%{public}s] lib_pathname [%{public}s] in lib_paths, return true.",
                    ns->ns_name,
                    lib_pathname);
                return true;
            }
        }
        strlist_free(paths);
    }

    if (ns->permitted_paths) {
        for (size_t i = 0; i < ns->permitted_paths->num; i++) {
            size_t len = strlen(ns->permitted_paths->strs[i]);
            if (strncmp(lib_pathname, ns->permitted_paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/') {
                LD_LOGD("is_accessible ns [%{public}s] lib_pathname [%{public}s] in permitted_paths, return true.",
                    ns->ns_name,
                    lib_pathname);
                return true;
            }
        }
    }
    return false;
}

bool check_asan_path(ns_t *ns, const char *lib_pathname)
{
    strlist *paths;
    if (ns->asan_lib_paths && (paths = strsplit(ns->asan_lib_paths, ":"))) {
        for (size_t i = 0; i < paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname + len + 1, '/')) {
                strlist_free(paths);
                LD_LOGD("check_asan_path ns [%{public}s] lib_pathname [%{public}s] in asan_lib_paths, return true.",
                    ns->ns_name,
                    lib_pathname);
                return true;
            }
        }
        strlist_free(paths);
    }
    if (ns->asan_permitted_paths) {
        for (size_t i = 0; i < ns->asan_permitted_paths->num; i++) {
            size_t len = strlen(ns->asan_permitted_paths->strs[i]);
            if (strncmp(lib_pathname, ns->asan_permitted_paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/') {
                LD_LOGD(
                    "check_asan_path ns [%{public}s] lib_pathname [%{public}s] in asan_permitted_paths, return true.",
                    ns->ns_name,
                    lib_pathname);
                return true;
            }
        }
    }
    LD_LOGD(
        "check_asan_path ns [%{public}s] lib_pathname [%{public}s] failed, return false.", ns->ns_name, lib_pathname);
    return false;
}

bool is_sharable(ns_inherit *inherit, const char *lib_name)
{
    if (inherit && lib_name && inherit->shared_libs) {
        for (size_t i = 0; i < inherit->shared_libs->num; i++) {
            if (strcmp(inherit->shared_libs->strs[i], lib_name) == 0 ||
                strcmp(inherit->shared_libs->strs[i], ALLOW_ALL_SHARED_LIBS) == 0) {
                LD_LOGD("is_sharable inherit [%{public}s] lib_name [%{public}s] found, return true.",
                    inherit->inherited_ns->ns_name,
                    lib_name);
                return true;
            }
        }
        LD_LOGD("is_sharable inherit [%{public}s] lib_name [%{public}s] not found, return false.",
            inherit->inherited_ns->ns_name,
            lib_name);
        return false;
    }
    LD_LOGD("is_sharable shared_libs not config, return true.");
    return true;
}

void ns_set_flag(ns_t *ns, int flag)
{
    if (!ns) {
        return;
    }
    ns->flag = flag;
    LD_LOGD("ns_set_flag ns[%{public}s] flag:%{public}d.", ns->ns_name, ns->flag);
}