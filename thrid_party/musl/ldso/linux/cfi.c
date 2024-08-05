/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/prctl.h>
#include "cfi.h"
#include "ld_log.h"
#include "namespace.h"

/* This module provides support for LLVM CFI Cross-DSO by implementing the __cfi_slowpath() and __cfi_slowpath_diag()
 * functions. These two functions will be called before visiting other dso's resources. The responsibility is to
 * calculate the __cfi_check() of the target dso, and call it. So use CFI shadow and shadow value to store the
 * relationship between dso and its __cfi_check addr while loading a dso. CFI shadow is an array which stores shadow
 * values. Shadow value is used to store the relationship. A shadow value can map 1 LIBRARY_ALIGNMENT memory range. So
 * each dso will be mapped to one or more shadow values in the CFI shadow, this depends on the address range of the
 * dso.
 * There are 3 types for shadow value:
 * - invalid(0) : the target addr does not belongs to any loaded dso.
 * - uncheck(1) : this LIBRARY_ALIGNMENT memory range belongs to a dso but it is no need to do the CFI check.
 * - valid(2 - 0xFFFF) : this LIBRARY_ALIGNMENT memory range belongs to a dso and need to do the CFI check.
 * The valid shadow value records the distance from the end of a LIBRARY_ALIGNMENT memory range to the __cfi_check addr
 * of the dso (The unit is 4096, because the __cfi_check is aligned with 4096).
 * The valid shadow value is calculated as below:
 *      sv = (AlignUp(__cfi_check, LIBRARY_ALIGNMENT) - __cfi_check + N * LIBRARY_ALIGNMENT) / 4096 + 2;
 *
 *      N   : starts at 0, is the index of LIBRARY_ALIGNMENT memory range that belongs to a dso.
 *      + 2 : to avoid conflict with invalid and uncheck shadow value.
 *
 * Below is a example for calculating shadow values of a dso.
 *                                               liba.so
 *                                                /\
 *           /''''''''''''''''''''''''''''''''''''  '''''''''''''''''''''''''''''''''''''\
 *           0x40000  __cfi_check addr = 0x42000               0x80000                  0xA0000                0xC0000
 *           +---------^----------------------------------------^-------------------------^-------------------------+
 *  Memory   |         |                                        |                         |                         |
 *           +------------------------------------------------------------------------------------------------------+
 *           \........... LIBRARY_ALIGNMENT ..................../\........... LIBRARY_ALIGNMENT ..................../
 *             \                                              /                                               /
 *               \                                          /                                          /
 *                 \                                      /                                     /
 *                   \                                  /                                /
 *                     \                              /                            /
 *            +-----------------------------------------------------------------------------------------------------+
 * CFI shadow |  invalid |           sv1              |           sv2              |            invalid             |
 *            +-----------------------------------------------------------------------------------------------------+
 *                          sv1 = (0x80000 - 0x42000 + 0 * LIBRARY_ALIGNMENT) / 4096 + 2 = 64
 *                          sv2 = (0x80000 - 0x42000 + 1 * LIBRARY_ALIGNMENT) / 4096 + 2 = 126
 *
 * Calculating the __cfi_check address is a reverse process:
 * - First align up the target addr with LIBRARY_ALIGNMENT to locate the corresponding shadow value.
 * - Then calculate the __cfi_check addr.
 *
 * In order for the algorithm to work well, the start addr of each dso should be aligned with LIBRARY_ALIGNMENT. */

#define MAX(a, b)                (((a) > (b)) ? (a) : (b))
#define MIN(a, b)                (((a) < (b)) ? (a) : (b))
#define ALIGN_UP(a, b)          (((a) + (b) - 1) & -(b))
#define ALIGN_DOWN(a, b)        ((a) & -(b))
#if DL_FDPIC
#define LADDR(p, v)             laddr((p), (v))
#else
#define LADDR(p, v)             (void *)((p)->base + (v))
#endif

/* Function ptr for __cfi_check() */
typedef int (*cfi_check_t)(uint64_t, void *, void *);

static const uintptr_t shadow_granularity = LIBRARY_ALIGNMENT_BITS;
static const uintptr_t cfi_check_granularity = 12;
static const uintptr_t shadow_alignment = 1UL << shadow_granularity;
static uintptr_t shadow_size = 0;
/* Start addr of the CFI shadow */
static char *cfi_shadow_start = NULL;
/* List head of all the DSOs loaded by the process */
static struct dso *dso_list_head = NULL;
static struct dso *pldso = NULL;

/* Shadow value */
/* The related shadow value(s) will be set to `sv_invalid` when:
 * - init CFI shadow.
 * - removing a dso. */
static const uint16_t sv_invalid = 0;
/* The related shadow value(s) will be set to `sv_uncheck` if:
 * - the DSO does not enable CFI Cross-Dso.
 * - the DSO enabled CFI Cross-Dso, but this DSO is larger than 16G, for the part of the dso that exceeds 16G,
 *   its shadow value will be set to `sv_uncheck`. */
static const uint16_t sv_uncheck = 1;
/* If a DSO enabled CFI Cross-Dso, the DSO's shadow value should be valid. Because of the defination of `sv_invalid`
 * and `sv_unchecked`, the valid shadow value should be at least 2. */
static const uint16_t sv_valid_min = 2;

#if defined(__LP64__)
static const uintptr_t max_target_addr = 0xffffffffffff;
#else
static const uintptr_t max_target_addr = 0xffffffff;
#endif

/* Create a cfi shadow */
static int create_cfi_shadow(void);

/* Map dsos to CFI shadow */
static int add_dso_to_cfi_shadow(struct dso *dso);
static int fill_shadow_value_to_shadow(uintptr_t begin, uintptr_t end, uintptr_t cfi_check, uint16_t type);

/* Find the __cfi_check() of target dso and call it */
void __cfi_slowpath(uint64_t call_site_type_id, void *func_ptr);
void __cfi_slowpath_diag(uint64_t call_site_type_id, void *func_ptr, void *diag_data);

static inline uintptr_t addr_to_offset(uintptr_t addr, int bits)
{
    /* Convert addr to CFI shadow offset.
     * Shift left 1 bit because the shadow value is uint16_t. */
    return (addr >> bits) << 1;
}

static struct symdef find_cfi_check_sym(struct dso *p)
{
    LD_LOGD("[CFI] [%{public}s] start!\n", __FUNCTION__);

    struct verinfo verinfo = { .s = "__cfi_check", .v = "", .use_vna_hash = false };
    struct sym_info_pair s_info_p = gnu_hash(verinfo.s);
    return find_sym_impl(p, &verinfo, s_info_p, 0, p->namespace);
}

static int is_addr_in_ldso(size_t a)
{
    size_t i = 0;
    if (DL_FDPIC) {
        i = count_syms(pldso);
        if (a - (size_t)pldso->funcdescs < i * sizeof(*pldso->funcdescs))
            return 1;
    }
    if (DL_FDPIC && pldso->loadmap) {
        for (i = 0; i < pldso->loadmap->nsegs; i++) {
            if (a - pldso->loadmap->segs[i].p_vaddr
                < pldso->loadmap->segs[i].p_memsz)
                return 1;
        }
    } else {
        Phdr *ph = pldso->phdr;
        size_t phcnt = pldso->phnum;
        size_t entsz = pldso->phentsize;
        size_t base = (size_t)pldso->base;
        for (; phcnt--; ph = (void *)((char *)ph + entsz)) {
            if (ph->p_type != PT_LOAD) continue;
            if (a - base - ph->p_vaddr < ph->p_memsz)
                return 1;
        }
        if (a - (size_t)pldso->map < pldso->map_len)
            return 0;
    }
    return 0;
}

static uintptr_t get_cfi_check_addr(uint16_t value, void* func_ptr)
{
    LD_LOGD("[CFI] [%{public}s] start!\n", __FUNCTION__);

    uintptr_t addr = (uintptr_t)func_ptr;
    uintptr_t aligned_addr = ALIGN_DOWN(addr, shadow_alignment) + shadow_alignment;
    uintptr_t cfi_check_func_addr = aligned_addr - ((uintptr_t)(value - sv_valid_min) << cfi_check_granularity);
#ifdef __arm__
    LD_LOGD("[CFI] [%{public}s] __arm__ defined!\n", __FUNCTION__);
    cfi_check_func_addr++;
#endif
    LD_LOGD("[CFI] [%{public}s] cfi_check_func_addr[%{public}p] in dso[%{public}s]\n",
            __FUNCTION__, cfi_check_func_addr, ((struct dso *)addr2dso((size_t)cfi_check_func_addr))->name);

    return cfi_check_func_addr;
}

static inline void cfi_slowpath_common(uint64_t call_site_type_id, void *func_ptr, void *diag_data)
{
    uint16_t value = sv_invalid;

    if (func_ptr == NULL) {
        return;
    }

#if defined(__aarch64__)
    LD_LOGD("[CFI] [%{public}s] __aarch64__ defined!\n", __FUNCTION__);
    uintptr_t addr = (uintptr_t)func_ptr & ((1ULL << 56) - 1);
#else
    LD_LOGD("[CFI] [%{public}s] __aarch64__ not defined!\n", __FUNCTION__);
    uintptr_t addr = func_ptr;
#endif

    /* Get shadow value */
    uintptr_t offset = addr_to_offset(addr, shadow_granularity);

    if (cfi_shadow_start == NULL) {
        LD_LOGE("[CFI] [%{public}s] the cfi_shadow_start is null!\n", __FUNCTION__);
        __builtin_trap();
    }

    if (offset > shadow_size) {
        value = sv_invalid;
    } else {
        value = *((uint16_t*)(cfi_shadow_start + offset));
    }
    LD_LOGD("[CFI] [%{public}s] called from %{public}s to %{public}s func_ptr:0x%{public}p shadow value:%{public}d diag_data:0x%{public}p call_site_type_id[%{public}p.\n",
             __FUNCTION__,
             ((struct dso *)addr2dso((size_t)__builtin_return_address(0)))->name,
             ((struct dso *)addr2dso((size_t)func_ptr))->name,
             func_ptr, value, diag_data, call_site_type_id);

    struct dso *dso = NULL;
    switch (value)
    {
    case sv_invalid:
        /* The ldso is an exception because it is loaded by kernel and is not mapped to the CFI shadow.
         * Do not check it. */
        if (is_addr_in_ldso((size_t)func_ptr)) {
            LD_LOGI("[CFI] [%{public}s] uncheck for ldso\n", __FUNCTION__);
            return;
        }

        dso = (struct dso *)addr2dso((size_t)__builtin_return_address(0));
        if (dso == NULL) {
            LD_LOGE("[CFI] [%{public}s] can not find the dso!\n", __FUNCTION__);
            __builtin_trap();
        }
        LD_LOGD("[CFI] [%{public}s] dso name[%{public}s]!\n", __FUNCTION__, dso->name);

        struct symdef cfi_check_sym = find_cfi_check_sym(dso);
        if (!cfi_check_sym.sym) {
            LD_LOGE("[CFI] [%{public}s] can not find the __cfi_check in the dso!\n", __FUNCTION__);
            __builtin_trap();
        }
        LD_LOGD("[CFI] [%{public}s] cfi_check addr[%{public}p]!\n", __FUNCTION__,
                LADDR(cfi_check_sym.dso, cfi_check_sym.sym->st_value));
        ((cfi_check_t)LADDR(cfi_check_sym.dso, cfi_check_sym.sym->st_value))(call_site_type_id, func_ptr, diag_data);
        break;
    case sv_uncheck:
        break;
    default:
        ((cfi_check_t)get_cfi_check_addr(value, func_ptr))(call_site_type_id, func_ptr, diag_data);
        break;
    }

    return;
}

int init_cfi_shadow(struct dso *dso_list, struct dso *ldso)
{
    LD_LOGD("[CFI] [%{public}s] start!\n", __FUNCTION__);

    if (dso_list == NULL) {
        LD_LOGW("[CFI] [%{public}s] has null param!\n", __FUNCTION__);
        return CFI_SUCCESS;
    }

    /* Save the head node of dso list */
    dso_list_head = dso_list;
    pldso = ldso;

    return map_dso_to_cfi_shadow(dso_list);
}

int map_dso_to_cfi_shadow(struct dso *dso)
{
    bool has_cfi_check = false;

    if (dso == NULL) {
        LD_LOGW("[CFI] [%{public}s] has null param!\n", __FUNCTION__);
        return CFI_SUCCESS;
    }

    /* If the cfi shadow does not exist, create it and map all the dsos and its dependents to it. */
    if (cfi_shadow_start == NULL) {
        /* Find __cfi_check symbol in dso list */
        for (struct dso *p = dso; p; p = p->next) {
            if (find_cfi_check_sym(p).sym) {
                LD_LOGD("[CFI] [%{public}s] find __cfi_check function in dso %{public}s!\n", __FUNCTION__, p->name);
                has_cfi_check = true;
                break;
            }
        }

        if (has_cfi_check) {
            if (create_cfi_shadow() == CFI_FAILED) {
                LD_LOGE("[CFI] [%{public}s] create cfi shadow failed!\n", __FUNCTION__);
                return CFI_FAILED;
            }
            add_dso_to_cfi_shadow(dso_list_head);
            prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, cfi_shadow_start, shadow_size, "cfi_shadow:musl");
        }
    /* If the cfi shadow exists, map the current dso and its dependents to it. */
    } else {
        add_dso_to_cfi_shadow(dso);
        prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, cfi_shadow_start, shadow_size, "cfi_shadow:musl");
    }

    return CFI_SUCCESS;
}

void unmap_dso_from_cfi_shadow(struct dso *dso)
{
    if (dso == NULL) {
        LD_LOGD("[CFI] [%{public}s] has null param!\n", __FUNCTION__);
        return;
    }

    LD_LOGD("[CFI] [%{public}s] unmap dso %{public}s from shadow!\n", __FUNCTION__, dso->name);

    if (cfi_shadow_start == NULL)
        return;

    if (dso->map == 0 || dso->map_len == 0)
        return;

    if (dso->is_mapped_to_shadow == false)
        return;

    /* Set the dso's shadow value as invalid. */
    fill_shadow_value_to_shadow(dso->map, dso->map + dso->map_len, 0, sv_invalid);
    dso->is_mapped_to_shadow = false;
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, cfi_shadow_start, shadow_size, "cfi_shadow:musl");

    return;
}

static int create_cfi_shadow(void)
{
    LD_LOGD("[CFI] [%{public}s] start!\n", __FUNCTION__);

    /* Each process can load up to (max_target_addr >> shadow_granularity) dsos. Shift left 1 bit because the shadow
     * value is uint16_t. The size passed to mmap() should be aligned with 4096, so shadow_size should be aligned. */
    shadow_size = ALIGN_UP(((max_target_addr >> shadow_granularity) << 1), PAGE_SIZE);

    uintptr_t *mmap_addr = mmap(NULL, shadow_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

    if (mmap_addr == MAP_FAILED) {
        LD_LOGE("[CFI] [%{public}s] mmap failed!\n", __FUNCTION__);
        return CFI_FAILED;
    }

    cfi_shadow_start = (char*)mmap_addr;
    LD_LOGD("[CFI] [%{public}s] the cfi_shadow_start addr is %{public}p!\n", __FUNCTION__, cfi_shadow_start);

    return CFI_SUCCESS;
}

static int add_dso_to_cfi_shadow(struct dso *dso)
{
    LD_LOGD("[CFI] [%{public}s] start with %{public}s !\n", __FUNCTION__, dso->name);
    for (struct dso *p = dso; p; p = p->next) {
        LD_LOGD("[CFI] [%{public}s] adding %{public}s to cfi shadow!\n", __FUNCTION__, p->name);
        if (p->map == 0 || p->map_len == 0) {
            LD_LOGW("[CFI] [%{public}s] the dso has no data! map[%{public}p] map_len[0x%{public}x]\n",
                    __FUNCTION__, p->map, p->map_len);
            continue;
        }

        if (p->is_mapped_to_shadow == true) {
            LD_LOGW("[CFI] [%{public}s] %{public}s is already in shadow!\n", __FUNCTION__, p->name);
            continue;
        }

        struct symdef cfi_check_sym = find_cfi_check_sym(p);
        /* If the dso doesn't have __cfi_check(), set it's shadow value unchecked. */
        if (!cfi_check_sym.sym) {
            LD_LOGD("[CFI] [%{public}s] %{public}s has no __cfi_check()!\n", __FUNCTION__, p->name);
            if (fill_shadow_value_to_shadow(p->map, p->map + p->map_len, 0, sv_uncheck) == CFI_FAILED) {
                LD_LOGE("[CFI] [%{public}s] add dso to cfi shadow failed!\n", __FUNCTION__);
                return CFI_FAILED;
            }
        /* If the dso has __cfi_check(), set it's shadow value valid. */
        } else {
            LD_LOGD("[CFI] [%{public}s] %{public}s has __cfi_check()!\n", __FUNCTION__, p->name);
            uintptr_t end = p->map + p->map_len;
            uintptr_t cfi_check = LADDR(cfi_check_sym.dso, cfi_check_sym.sym->st_value);

            if (cfi_check == 0) {
                LD_LOGE("[CFI] [%{public}s] %{public}s has null cfi_check func!\n", __FUNCTION__, p->name);
                return CFI_FAILED;
            }
            if (fill_shadow_value_to_shadow(p->map, end, cfi_check, sv_valid_min) == CFI_FAILED) {
                LD_LOGE("[CFI] [%{public}s] add %{public}s to cfi shadow failed!\n", __FUNCTION__, p->name);
                return CFI_FAILED;
            }
        }
        p->is_mapped_to_shadow = true;
        LD_LOGD("[CFI] [%{public}s] add %{public}s to cfi shadow succeed.\n", __FUNCTION__, p->name);
    }
    LD_LOGD("[CFI] [%{public}s] %{public}s done.\n", __FUNCTION__, dso->name);

    return CFI_SUCCESS;
}

static int fill_shadow_value_to_shadow(uintptr_t begin, uintptr_t end, uintptr_t cfi_check, uint16_t type)
{
    LD_LOGD("[CFI] [%{public}s] begin[%{public}x] end[%{public}x] cfi_check[%{public}x] type[%{public}x]!\n",
            __FUNCTION__, begin, end, cfi_check, type);

    /* To ensure the atomicity of the CFI shadow operation, we create a temp_shadow, write the shadow value to
     * the temp_shadow, and then write it back to the CFI shadow by mremap(). */
    begin = ALIGN_DOWN(MAX(begin, cfi_check), shadow_alignment);
    char* shadow_begin = cfi_shadow_start + addr_to_offset(begin, LIBRARY_ALIGNMENT_BITS);
    char* shadow_end = (char*)(((uint16_t*)(cfi_shadow_start + addr_to_offset(end - 1, LIBRARY_ALIGNMENT_BITS))) + 1);
    char* aligned_shadow_begin = (char*)ALIGN_DOWN((uintptr_t)shadow_begin, PAGE_SIZE);
    char* aligned_shadow_end = (char*)ALIGN_UP((uintptr_t)shadow_end, PAGE_SIZE);

    uint16_t tmp_shadow_size = aligned_shadow_end - aligned_shadow_begin;
    uint16_t offset_begin = shadow_begin - aligned_shadow_begin;
    uint16_t offset_end = shadow_end - aligned_shadow_begin;

    char* tmp_shadow_start = (char*)mmap(NULL, tmp_shadow_size,
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (tmp_shadow_start == MAP_FAILED) {
        LD_LOGE("[CFI] [%{public}s] mmap failed!\n", __FUNCTION__);
        return CFI_FAILED;
    }

    LD_LOGD("[CFI] [%{public}s] tmp_shadow_start is %{public}p\t tmp_shadow_size is 0x%{public}x!\n",
        __FUNCTION__, tmp_shadow_start, tmp_shadow_size);
    if (mprotect(aligned_shadow_begin, tmp_shadow_size, PROT_READ) == -1) {
        LD_LOGE("[CFI] [%{public}s] mprotect failed!\n", __FUNCTION__);
        return CFI_FAILED;
    }
    memcpy(tmp_shadow_start, aligned_shadow_begin, offset_begin);
    memcpy(tmp_shadow_start + offset_end, shadow_end, aligned_shadow_end - shadow_end);

    /* If the dso has __cfi_check(), calculate valid shadow value */
    if (type == sv_valid_min) {
#ifdef __arm__
        uint16_t shadow_value_begin = ((begin + shadow_alignment - (cfi_check - 1))
            >> cfi_check_granularity) + sv_valid_min;
#else
        uint16_t shadow_value_begin = ((begin + shadow_alignment - cfi_check)
            >> cfi_check_granularity) + sv_valid_min;
#endif
        LD_LOGD("[CFI] [%{public}s] shadow_value_begin is 0x%{public}x!\n", __FUNCTION__, shadow_value_begin);
        uint16_t shadow_value_step = 1 << (shadow_granularity - cfi_check_granularity);
        uint16_t shadow_value = shadow_value_begin;

        /* Set shadow_value */
        for (uint16_t *shadow_addr = tmp_shadow_start + offset_begin;
            shadow_addr != tmp_shadow_start + offset_end; shadow_addr++) {
            /* If a dso is larger than 16G( = max_shadow_value * shadow_alignment / 1G),
             * the excess is not checked. */
            if (shadow_value < shadow_value_begin) {
                *shadow_addr = sv_uncheck;
                continue;
            }
            *shadow_addr = (*shadow_addr == sv_invalid) ? shadow_value : sv_uncheck;
            shadow_value += shadow_value_step;
        }
    /* in these cases, shadow_value will always be sv_uncheck or sv_invalid */
    } else if (type == sv_uncheck || type == sv_invalid) {
        /* Set shadow_value */
        for (uint16_t *shadow_addr = tmp_shadow_start + offset_begin;
            shadow_addr != tmp_shadow_start + offset_end; shadow_addr++) {
            *shadow_addr = type;
        }
    } else {
        LD_LOGE("[CFI] [%{public}s] has error param!\n", __FUNCTION__);
        munmap(tmp_shadow_start, tmp_shadow_size);
        return CFI_FAILED;
    }

    mprotect(tmp_shadow_start, tmp_shadow_size, PROT_READ);
    /* Remap temp_shadow to CFI shadow. */
    uint16_t* mremap_addr = mremap(tmp_shadow_start, tmp_shadow_size, tmp_shadow_size,
        MREMAP_MAYMOVE | MREMAP_FIXED, aligned_shadow_begin);

    if (mremap_addr == MAP_FAILED) {
        LD_LOGE("[CFI] [%{public}s] mremap failed!\n", __FUNCTION__);
        munmap(tmp_shadow_start, tmp_shadow_size);
        return CFI_FAILED;
    }

    LD_LOGD("[CFI] [%{public}s] fill completed!\n", __FUNCTION__);
    return CFI_SUCCESS;
}

void __cfi_slowpath(uint64_t call_site_type_id, void *func_ptr)
{
    LD_LOGD("[CFI] [%{public}s] called from dso[%{public}s] to dso[%{public}s] func_ptr[%{public}p]\n",
            __FUNCTION__,
            ((struct dso *)addr2dso((size_t)__builtin_return_address(0)))->name,
            ((struct dso *)addr2dso((size_t)func_ptr))->name,
            func_ptr);

    cfi_slowpath_common(call_site_type_id, func_ptr, NULL);
    return;
}

void __cfi_slowpath_diag(uint64_t call_site_type_id, void *func_ptr, void *diag_data)
{
    LD_LOGD("[CFI] [%{public}s] called from dso[%{public}s] to dso[%{public}s] func_ptr[%{public}p]\n",
            __FUNCTION__,
            ((struct dso *)addr2dso((size_t)__builtin_return_address(0)))->name,
            ((struct dso *)addr2dso((size_t)func_ptr))->name,
            func_ptr);

    cfi_slowpath_common(call_site_type_id, func_ptr, diag_data);
    return;
}
