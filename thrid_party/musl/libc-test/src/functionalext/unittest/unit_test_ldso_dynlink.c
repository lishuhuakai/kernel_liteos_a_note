/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <errno.h>
#include "test.h"
#include "dlfcn.h"
#include "dlfcn_ext.h"
#include "namespace.h"
#include "ns_config.h"
#include "dynlink_rand.h"
#include "functionalext.h"
#include "ld_log.h"
#include "dynlink.h"
#include "link.h"
#include <sys/stat.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <libc.h>

typedef void (*TEST_FUN)(void);
int handle_asan_path_open(int fd, const char *name, ns_t *namespace, char *buf, size_t buf_size);

/**
 * @tc.name      : dynlink_test_0010
 * @tc.desc      : Test dlns_set_namespace_lib_path with invalid input 
 * @tc.level     : Level 2
 */
static void dynlink_test_0010(void)
{
    int ret = dlns_set_namespace_lib_path(NULL, NULL);
    EXPECT_EQ(__FUNCTION__, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0020
 * @tc.desc      : Test dlns_set_namespace_lib_path with invalid input 
 * @tc.level     : Level 2
 */
static void dynlink_test_0020(void)
{
    int ret = dlns_set_namespace_lib_path("abc", "abc");
    EXPECT_EQ(__FUNCTION__, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0030
 * @tc.desc      : Set path for default lib use dlns_set_namespace_lib_path 
 * @tc.level     : Level 1
 */
static void dynlink_test_0030(void)
{
    int ret = dlns_set_namespace_lib_path("default", "/data");
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0040
 * @tc.desc      : Test dlns_set_namespace_separated with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0040(void)
{
    int ret = dlns_set_namespace_separated(NULL, true);
    EXPECT_EQ(__FUNCTION__, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0050
 * @tc.desc      : Test dlns_set_namespace_separated with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0050(void)
{
    int ret = dlns_set_namespace_separated("abc", true);
    EXPECT_EQ(__FUNCTION__, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0060
 * @tc.desc      : Set separated for default lib use dlns_set_namespace_separated
 * @tc.level     : Level 0
 */
static void dynlink_test_0060(void)
{
    int ret = dlns_set_namespace_separated("default", true);
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0070
 * @tc.desc      : Test dlns_set_namespace_permitted_paths with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0070(void)
{
    int ret = dlns_set_namespace_permitted_paths(NULL, NULL);
    EXPECT_EQ(__FUNCTION__, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0080
 * @tc.desc      : Test dlns_set_namespace_permitted_paths with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0080(void)
{
    int ret = dlns_set_namespace_permitted_paths("abc", "abc");
    EXPECT_EQ(__FUNCTION__, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0090
 * @tc.desc      : Set path for default lib use dlns_set_namespace_permitted_paths
 * @tc.level     : Level 0
 */
static void dynlink_test_0090(void)
{
    int ret = dlns_set_namespace_permitted_paths("default", "/data");
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0100
 * @tc.desc      : Test dlns_set_namespace_allowed_libs with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0100(void)
{
    int ret = dlns_set_namespace_allowed_libs(NULL, NULL);
    EXPECT_EQ(__FUNCTION__, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0110
 * @tc.desc      : Test dlns_set_namespace_allowed_libs with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0110(void)
{
    int ret = dlns_set_namespace_allowed_libs("abc", "abc");
    EXPECT_EQ(__FUNCTION__, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0120
 * @tc.desc      : Set allowed libs for default lib use dlns_set_namespace_allowed_libs
 * @tc.level     : Level 0
 */
static void dynlink_test_0120(void)
{
    int ret = dlns_set_namespace_allowed_libs("default", "/data");
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0002
 * @tc.desc      : Test handle_asan_path_open with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0002(void)
{
    int ret = handle_asan_path_open(2, NULL, NULL, NULL, 0);
    EXPECT_EQ(__FUNCTION__, ret, 2);
}


/**
 * @tc.name      : dynlink_test_0003
 * @tc.desc      : Set asan lib path use handle_asan_path_open
 * @tc.level     : Level 0
 */
static void dynlink_test_0003(void)
{
    ns_t ns;
    ns.asan_lib_paths = "/lib:/system/lib";
    ns.lib_paths = NULL;
    char buf[512];
    int ret = handle_asan_path_open(-1, "libz.so", &ns, buf, 512);
    EXPECT_GT(__FUNCTION__, ret, 0);
    close(ret);
}

/**
 * @tc.name      : dynlink_test_0004
 * @tc.desc      : Set lib path use handle_asan_path_open
 * @tc.level     : Level 0
 */
static void dynlink_test_0004(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;
    ns.lib_paths = "/lib:/system/lib";
    char buf[512];
    int ret = handle_asan_path_open(-1, "libz.so", &ns, buf, 512);
    EXPECT_GT(__FUNCTION__, ret, 0);
    close(ret);
}

/**
 * @tc.name      : dynlink_test_0005
 * @tc.desc      : Set both asan lib path and lib path use handle_asan_path_open
 * @tc.level     : Level 1
 */
static void dynlink_test_0005(void)
{
    ns_t ns;
    ns.asan_lib_paths = "/lib:/system";
    ns.lib_paths = "/system/lib:/lib";
    char buf[512];
    int ret = handle_asan_path_open(-1, "libz.so", &ns, buf, 512);
    EXPECT_GT(__FUNCTION__, ret, 0);
    close(ret);
}

// Because the function logic branch in dynlink.c is very complex, some interfaces are covered by white-box unit testing.
#ifdef UNIT_TEST_STATIC
struct reserved_address_params {
    void* start_addr;
    size_t reserved_size;
    bool must_use_reserved;
    bool reserved_address_recursive;
#ifdef LOAD_ORDER_RANDOMIZATION
    struct dso *target;
#endif
};

void set_ns_attrs(ns_t *ns, ns_configor *conf);
void set_ns_inherits(ns_t *ns, ns_configor *conf);
void *mmap_fixed(void *p, size_t n, int prot, int flags, int fd, off_t off);
void fill_random_data(void *buf, size_t buflen);
void *map_library(int fd, struct dso *dso, struct reserved_address_params *reserved_params);
void unmap_library(struct dso *dso);
int fixup_rpath(struct dso *p, char *buf, size_t buf_size);
void __InstallSignalHandler();
bool get_random(void *buf, size_t buflen);
void assign_tls(struct dso *p);
size_t count_syms(struct dso *p);
int check_vna_hash(Verdef *def, int16_t vsym, uint32_t vna_hash);
int check_verinfo(Verdef *def, int16_t *versym, uint32_t index, struct verinfo *verinfo, char *strings);
struct dso *find_library_by_fstat(const struct stat *st, const ns_t *ns, bool check_inherited, uint64_t file_offset);
void load_preload(char *s, ns_t *ns, struct loadtasks *tasks);
bool get_ld_debug_dlclose_value();

/**
 * @tc.name      : dynlink_test_0130
 * @tc.desc      : Test set_ns_attrs with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0130(void)
{
    set_ns_attrs(NULL, NULL);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0140
 * @tc.desc      : Test set_ns_attrs with default ns
 * @tc.level     : Level 1
 */
static void dynlink_test_0140(void)
{
    ns_configor *conf = configor_init();
    ns_t *d_ns = get_default_ns();
    set_ns_attrs(d_ns, conf);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0150
 * @tc.desc      : Test set_ns_inherits with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0150(void)
{
    set_ns_inherits(NULL, NULL);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0160
 * @tc.desc      : Test set_ns_attrs with default ns
 * @tc.level     : Level 1
 */
static void dynlink_test_0160(void)
{
    ns_configor *conf = configor_init();
    ns_t *d_ns = get_default_ns();
    set_ns_inherits(d_ns, conf);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0200
 * @tc.desc      : Test mmap_fixed with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0200(void)
{
    void *p = mmap_fixed(0, -1, PROT_NONE, MAP_FIXED, -1, 0);
    EXPECT_EQ(__FUNCTION__, p, MAP_FAILED);
}

/**
 * @tc.name      : dynlink_test_0210
 * @tc.desc      : mmap_fixed test
 * @tc.level     : Level 1
 */
static void dynlink_test_0210(void)
{
    char buf[128];
    void *p = mmap_fixed(buf, 128, PROT_NONE, MAP_ANONYMOUS, -1, 0);
    EXPECT_PTRNE(dynlink_test_0200, p, NULL);
}

/**
 * @tc.name      : dynlink_test_0220
 * @tc.desc      : Test mmap_fixed with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0220(void)
{
    int fd = -1;
    char *path = "/dev/random";
    fd = open(path, O_RDWR);

    if (fd == -1){
        EXPECT_TRUE(__FUNCTION__, false);
        return;
    }

    char buf[128];
    void *p = mmap_fixed(buf, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_TRUE(__FUNCTION__, (p != MAP_FAILED));
    if (fd != -1){
        close(fd);
    }
}

/**
 * @tc.name      : dynlink_test_0230
 * @tc.desc      : Test fill_random_data with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0230(void)
{
    fill_random_data((void *)0, 0);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0240
 * @tc.desc      : Test get_random with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0240(void)
{
    bool ret = get_random("adb", 1);
    EXPECT_FALSE(__FUNCTION__, ret);
}

/**
 * @tc.name      : dynlink_test_0250
 * @tc.desc      : Test unmap_library when loadmap is not null
 * @tc.level     : Level 1
 */
static void dynlink_test_0250(void)
{
    struct dso temp_dso ;

    temp_dso.loadmap = (struct fdpic_loadmap *)calloc(1, sizeof(struct fdpic_loadmap) + 1 * sizeof(struct fdpic_loadseg));
    if (temp_dso.loadmap == NULL){
    	EXPECT_TRUE(__FUNCTION__, true);
	return;
    }
    temp_dso.loadmap->nsegs = 1;

    char *path = "/dev/zero";
    int fd = open(path, O_RDWR);
    if (fd == -1){
        EXPECT_TRUE(__FUNCTION__, false);
        return;
    }

    void *p = mmap(NULL, sizeof(uintptr_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED){
        EXPECT_TRUE(__FUNCTION__, false);
        return;
    }
    temp_dso.loadmap->segs[0].addr = (uintptr_t)p;
    temp_dso.loadmap->segs[0].p_memsz = 1;

    unmap_library(&temp_dso);
    EXPECT_EQ(__FUNCTION__, 0, 0);
    
    if (get_ld_debug_dlclose_value()){
    	munmap(p, sizeof(uintptr_t));
    }

    if (fd != -1){
        close(fd);
    }
}

/**
 * @tc.name      : dynlink_test_0260
 * @tc.desc      : Test map_library with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0260(void)
{
    struct dso temp_dso = {0};
    int fd = open("/system/lib/libutils.z.so", O_RDONLY|O_CLOEXEC);
    unsigned char *map = map_library(fd, &temp_dso, NULL);
    close(fd);
    EXPECT_PTRNE(__FUNCTION__, map, NULL);
}

/**
 * @tc.name      : dynlink_test_0270
 * @tc.desc      : Test assign_tls with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0270(void)
{
    struct tls_module tls = { .image = "1", .len = 1, .size = 1, .align = 0, .offset = 0};
    struct dso temp_dso = {.tls = tls};
    assign_tls(&temp_dso);
    EXPECT_TRUE(__FUNCTION__,true);
}

/**
 * @tc.name      : dynlink_test_0280
 * @tc.desc      : Test assign_tls with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0280(void)
{
    struct tls_module tls_next2 = { .image = "3", .len = 1, .size = 1, .align = 0, .offset = 0};
    struct tls_module tls_next = { .image = "2", .len = 1, .size = 1, .align = 0, .offset = 0, .next = &tls_next2};
    struct tls_module tls = { .image = "1", .len = 1, .size = 1, .align = 0, .offset = 0, .next = &tls_next};

    struct dso temp_dso = {.tls = tls};
    assign_tls(&temp_dso);
    EXPECT_TRUE(__FUNCTION__,true);
}

/**
 * @tc.name      : dynlink_test_0290
 * @tc.desc      : Test dlns_inherit with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0290(void)
{
    int ret = dlns_inherit(NULL, NULL, "");
    EXPECT_EQ(__FUNCTION__, ret, EINVAL);
}


/**
 * @tc.name      : dynlink_test_0300
 * @tc.desc      : Test dlns_inherit when input name is space string
 * @tc.level     : Level 1
 */
static void dynlink_test_0300(void)
{
    Dl_namespace dlns = {.name = ""};
    Dl_namespace inherited = {.name = ""};
    int ret = dlns_inherit(&dlns, &inherited, "");
    EXPECT_EQ(__FUNCTION__, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0310
 * @tc.desc      : Test dlns_create2 when input name is space string
 * @tc.level     : Level 1
 */
static void dynlink_test_0310(void)
{
    Dl_namespace dlns = {.name = ""};
    int ret = dlns_create2(&dlns, "", CREATE_INHERIT_CURRENT);
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0320
 * @tc.desc      : Test dlns_init with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0320(void)
{
    dlns_init(NULL, NULL);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0330
 * @tc.desc      : Test dlns_init when input name is space string
 * @tc.level     : Level 1
 */
static void dynlink_test_0330(void)
{
    Dl_namespace dlns = {.name = ""};
    dlns_init(&dlns, NULL);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0340
 * @tc.desc      : Test find_library_by_fstat with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0340(void)
{
    struct stat st = {};
    struct _ns_inherit_list_  ns_inherit_list= {};
    struct _dso_list_ dso_list= {.num = 0};
    ns_t ns = {.ns_inherits = &ns_inherit_list, .ns_dsos = &dso_list, .ns_name = "abc"};

    find_library_by_fstat(&st, &ns, true, 0);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0350
 * @tc.desc      : Test find_library_by_fstat with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0350(void)
{
    struct stat st = {};
    struct _dso_list_ dso_list= {.num = 0};
    ns_t second_ns = {.ns_dsos = &dso_list, .ns_name = "ns_2"};
    struct _namespace_inherit_ inherit = {.inherited_ns = &second_ns, .shared_libs = NULL};
    struct _namespace_inherit_ *inherits[] = {&inherit};
    struct _ns_inherit_list_  ns_inherit_list= {.inherits = inherits, .num = 1};
    ns_t ns = {.ns_inherits = &ns_inherit_list, .ns_dsos = &dso_list, .ns_name = "ns_1"};

    find_library_by_fstat(&st, &ns, true, 0);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

// use by case dynlink_test_0360
static strlist *dummy_config_get_inherits(const char *ns_name)
{
    strlist *list = (strlist *)calloc(1, sizeof(strlist));
    list->num = 1;
    list->size = 2;
    list->strs = (char **)calloc(2, sizeof *list->strs);
    char name[] = "default";
    list->strs[0] = malloc(strlen(name) + 1);
    memcpy(list->strs[0], name, strlen(name) + 1);
    return list;
}

/**
 * @tc.name      : dynlink_test_0360
 * @tc.desc      : Test set_ns_inherits with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0360(void)
{
    ns_configor *conf = configor_init();
    ns_t *d_ns = get_default_ns();
    ns_set_name(d_ns, "default");
    conf->get_inherits = dummy_config_get_inherits;
    set_ns_inherits(d_ns, conf);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_0370
 * @tc.desc      : Test fill_random_data with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_0370(void)
{
    char buf[1024];
    fill_random_data((void*)buf, 1024);
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_2000
 * @tc.desc      : Test fixup_rpath with normal input
 * @tc.level     : Level 0
 */
static void dynlink_test_2000(void)
{
    struct dso dso = {.rpath = NULL, .rpath_orig = "$ARIGIN"};
    char buf[128] = "";
    int ret = fixup_rpath(&dso, buf, 128);
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_2001
 * @tc.desc      : Test fixup_rpath when kernel_mapped is 0
 * @tc.level     : Level 1
 */
static void dynlink_test_2001(void)
{
    struct dso dso = {.rpath = NULL, .rpath_orig = "$ORIGIN", .kernel_mapped = 0, .name="abc/abc"};
    char buf[128] = "";
    int ret = fixup_rpath(&dso, buf, 128);
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_2002
 * @tc.desc      : Test fixup_rpath when kernel_mapped is 1
 * @tc.level     : Level 1
 */
static void dynlink_test_2002(void)
{
    struct dso dso = {.rpath = NULL, .rpath_orig = "$ORIGIN", .kernel_mapped = 1, .name = "abc/abc"};
    char buf[128] = "";
    int ret = fixup_rpath(&dso, buf, 128);
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_2003
 * @tc.desc      : __InstallSignalHandler coverage test case
 * @tc.level     : Level 0
 */
static void dynlink_test_2003(void)
{
    __InstallSignalHandler();
    EXPECT_EQ(__FUNCTION__, 0, 0);
}

/**
 * @tc.name      : dynlink_test_2004
 * @tc.desc      : Test check_vna_hash with normal input
 * @tc.level     : Level 0
 */
static void dynlink_test_2004(void)
{
    Verdef def;
    def.vd_ndx = 0xf;
    def.vd_hash = 0x123;
    int ret = check_vna_hash(&def, 0xf, 0x123);
    EXPECT_EQ(__FUNCTION__, ret, 1);
}

/**
 * @tc.name      : dynlink_test_2005
 * @tc.desc      : Test check_vna_hash with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_2005(void)
{
    Verdef def;
    def.vd_ndx = 0x0;
    def.vd_hash = 0x123;
    def.vd_next = 0;
    int ret = check_vna_hash(&def, 0xf, 0x123);
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_2006
 * @tc.desc      : Test check_verinfo with normal input
 * @tc.level     : Level 0
 */
static void dynlink_test_2006(void)
{
    Verdef def;
    int16_t versym[10];
    versym[0] = 0xf;
    struct verinfo info;
    info.use_vna_hash = true;
    info.vna_hash = 0x123;
    def.vd_ndx = 0xf;
    def.vd_hash = 0x123;
    def.vd_next = 0;
    int ret = check_verinfo(&def, versym, 0, &info, "abc");
    EXPECT_EQ(__FUNCTION__, ret, 1);
}

/**
 * @tc.name      : dynlink_test_2007
 * @tc.desc      : Test check_verinfo with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_2007(void)
{
    Verdef def;
    int16_t versym[10];
    versym[0] = 0;
    struct verinfo info;
    info.use_vna_hash = true;
    info.vna_hash = 0x123;
    info.v = "";
    def.vd_ndx = 0xf;
    def.vd_hash = 0x123;
    def.vd_next = 0;
    int ret = check_verinfo(&def, versym, 0, &info, "abc");
    EXPECT_EQ(__FUNCTION__, ret, 1);
}

/**
 * @tc.name      : dynlink_test_2008
 * @tc.desc      : Test check_verinfo with normal input
 * @tc.level     : Level 1
 */
static void dynlink_test_2008(void)
{
    Verdef def;
    int16_t versym[10];
    versym[0] = 0;
    struct verinfo info;
    info.use_vna_hash = true;
    info.vna_hash = 0x123;
    info.v = "123";
    def.vd_ndx = 0;
    def.vd_hash = 0x123;
    def.vd_next = 0;
    def.vd_flags = 0x1;
    int ret = check_verinfo(&def, versym, 0, &info, "abc");
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : dynlink_test_2009
 * @tc.desc      : Test count_syms with normal input
 * @tc.level     : Level 0
 */
static void dynlink_test_2009(void)
{
    struct dso dso;
    uint32_t buf[128] = {0};
    buf[0] = 1;
    buf[2] = 1;
    int i = 4 + (buf[2]*sizeof(size_t)/4);
    buf[i] = 1;  
    int j = i + buf[0] + (buf[0] - buf[1]);
    buf[j] = 1;
    dso.ghashtab = buf;
    size_t ret = count_syms(&dso);
    EXPECT_EQ(__FUNCTION__, ret, 2);
}

/**
 * @tc.name      : dynlink_test_2010
 * @tc.desc      : Test free_loadtasks with normal input
 * @tc.level     : Level 0
 */
static void dynlink_test_2010(void)
{
    struct loadtasks *tasks = malloc(sizeof(struct loadtasks));
    if (tasks) {
        tasks->array = NULL;
        tasks->capacity = 0;
        tasks->length = 0;
    }

    char name[] = "libz.so";
    load_preload(name, get_default_ns(), tasks);
    EXPECT_TRUE(__FUNCTION__, true);
    if (tasks){
        free_loadtasks(tasks);
    }
}
#endif

TEST_FUN G_Fun_Array[] = {
    dynlink_test_0002,
    dynlink_test_0003,
    dynlink_test_0004,
    dynlink_test_0005,
    dynlink_test_0010,
    dynlink_test_0020,
    dynlink_test_0030,
    dynlink_test_0040,
    dynlink_test_0050,
    dynlink_test_0060,
    dynlink_test_0070,
    dynlink_test_0080,
    dynlink_test_0090,
    dynlink_test_0100,
    dynlink_test_0110,
    dynlink_test_0120,
#ifdef UNIT_TEST_STATIC
    dynlink_test_0130,
    dynlink_test_0140,
    dynlink_test_0150,
    dynlink_test_0160,
    dynlink_test_0200,
    dynlink_test_0210,
    dynlink_test_0220,
    dynlink_test_0230,
    dynlink_test_0240,
    dynlink_test_0250,
    dynlink_test_0260,
    dynlink_test_0270,
    dynlink_test_0280,
    dynlink_test_0290,
    dynlink_test_0300,
    dynlink_test_0310,
    dynlink_test_0320,
    dynlink_test_0330,
    dynlink_test_0340,
    dynlink_test_0350,
    dynlink_test_0360,
    dynlink_test_0370,
    dynlink_test_2000,
    dynlink_test_2001,
    dynlink_test_2002,
    dynlink_test_2003,
    dynlink_test_2004,
    dynlink_test_2005,
    dynlink_test_2006,
    dynlink_test_2007,
    dynlink_test_2008,
    dynlink_test_2009,
    dynlink_test_2010,
#endif
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}