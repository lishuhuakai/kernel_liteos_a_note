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
#include "test.h"
#include "namespace.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

struct dso {
    char* name;
};

/**
 * @tc.name      : namespace_test_0010
 * @tc.desc      : ns_free test arg is null
 * @tc.level     : Level 2
 */
static void namespace_test_0010(void)
{
    ns_free(NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0020
 * @tc.desc      : ns_free test normal condition
 * @tc.level     : Level 0
 */
static void namespace_test_0020(void)
{
    ns_t ns;
    int tmp = 1;
    ns.ns_dsos = (dsolist*)&tmp;
    ns.ns_name = (char*)&tmp;
    ns.env_paths = (char*)&tmp;
    ns.lib_paths = (char*)&tmp;
    ns.asan_lib_paths = (char*)&tmp;
    ns.permitted_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.allowed_libs = NULL;
    ns_inherit_list list;
    list.num = 1;
    ns_inherit inherit;
    inherit.shared_libs = NULL;
    ns_inherit* pInherit = &inherit;
    list.inherits = &pInherit;
    ns.ns_inherits = &list;

    ns_free(&ns);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0030
 * @tc.desc      : ns_free test arg ns's memebers are NULL 
 * @tc.level     : Level 2
 */
static void namespace_test_0030(void)
{
    ns_t ns;
    ns.ns_dsos = NULL;
    ns.ns_name = NULL;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.permitted_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.allowed_libs = NULL;
    ns.ns_inherits = NULL;

    ns_free(&ns);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0040
 * @tc.desc      : ns_add_dso test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0040(void)
{
    ns_add_dso(NULL, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0050
 * @tc.desc      : ns_add_dso test arg dso is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0050(void)
{
    ns_t ns;
    ns_add_dso(&ns, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0060
 * @tc.desc      : ns_add_dso test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0060(void)
{
    struct dso d;
    ns_add_dso(NULL, &d);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0070
 * @tc.desc      : ns_add_dso test normal condition
 * @tc.level     : Level 1
 */
static void namespace_test_0070(void)
{
    ns_t ns;
    struct dso d;
    dsolist list;
    list.num = 1;
    list.size = 1;
    ns.ns_dsos = &list;
    ns_add_dso(&ns, &d);
    EXPECT_EQ(__FUNCTION__, list.size, 2);
}

/**
 * @tc.name      : namespace_test_0080
 * @tc.desc      : nslist_add_ns test many times
 * @tc.level     : Level 1
 */
static void namespace_test_0080(void)
{
    ns_t ns;
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0090
 * @tc.desc      : nslist_add_ns test arg is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0090(void)
{
    nslist_add_ns(NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0100
 * @tc.desc      : ns_set_name test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0100(void)
{
    ns_set_name(NULL, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0110
 * @tc.desc      : ns_set_name test arg name is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0110(void)
{
    ns_t ns;
    ns_set_name(&ns, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0120
 * @tc.desc      : ns_set_name test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0120(void)
{
    ns_set_name(NULL, "abc");
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0130
 * @tc.desc      : ns_set_env_paths test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0130(void)
{
    ns_set_env_paths(NULL, "abc");
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0140
 * @tc.desc      : ns_set_env_paths test arg ns.env_paths is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0140(void)
{
    ns_t ns;
    ns.env_paths = NULL;
    ns_set_env_paths(&ns, "abc");
    EXPECT_EQ(__FUNCTION__, strcmp(ns.env_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0150
 * @tc.desc      : ns_set_env_paths test arg ns.env_paths is equal to arg name
 * @tc.level     : Level 1
 */
static void namespace_test_0150(void)
{
    ns_t ns;
    ns.env_paths = "abc";
    ns_set_env_paths(&ns, "abc");
    EXPECT_EQ(__FUNCTION__, strcmp(ns.env_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0160
 * @tc.desc      : ns_set_env_paths test arg name is NULL and ns.env_paths is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0160(void)
{
    ns_t ns;
    ns.env_paths = NULL;
    ns_set_env_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.env_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0170
 * @tc.desc      : ns_set_env_paths test arg name is NULL and ns.env_paths is not NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0170(void)
{
    ns_t ns;
    ns.env_paths = "abc";
    ns_set_env_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.env_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0180
 * @tc.desc      : ns_set_asan_lib_paths test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0180(void)
{
    ns_set_asan_lib_paths(NULL, "abc");
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0190
 * @tc.desc      : ns_set_asan_lib_paths test arg ns.asan_lib_paths is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0190(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;
    ns_set_asan_lib_paths(&ns, "abc");
    EXPECT_EQ(__FUNCTION__, strcmp(ns.asan_lib_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0200
 * @tc.desc      : ns_set_asan_lib_paths test arg ns.asan_lib_paths is equal to arg name
 * @tc.level     : Level 1
 */
static void namespace_test_0200(void)
{
    ns_t ns;
    ns.asan_lib_paths = "abc";
    ns_set_asan_lib_paths(&ns, "abc");
    EXPECT_EQ(__FUNCTION__, strcmp(ns.asan_lib_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0210
 * @tc.desc      : ns_set_asan_lib_paths test arg name is NULL and ns.asan_lib_paths is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0210(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;
    ns_set_asan_lib_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.asan_lib_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0220
 * @tc.desc      : ns_set_asan_lib_paths test arg name is NULL and ns.asan_lib_paths is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0220(void)
{
    ns_t ns;
    ns.asan_lib_paths = "abc";
    ns_set_asan_lib_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.asan_lib_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0230
 * @tc.desc      : ns_set_permitted_paths test arg permitted_paths is NULL and ns.permitted_paths is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0230(void)
{
    ns_t ns;
    strlist list;
    ns.permitted_paths = &list;
    ns_set_permitted_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0240
 * @tc.desc      : ns_set_permitted_paths test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0240(void)
{
    ns_set_permitted_paths(NULL, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0250
 * @tc.desc      : ns_set_permitted_paths test arg permitted_paths is NULL and ns.permitted_paths is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0250(void)
{
    ns_t ns;
    ns.permitted_paths = NULL;
    ns_set_permitted_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0260
 * @tc.desc      : ns_set_asan_permitted_paths test arg asan_permitted_paths is NULL and ns.asan_permitted_paths is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0260(void)
{
    ns_t ns;
    strlist list;
    ns.asan_permitted_paths = &list;
    ns_set_asan_permitted_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.asan_permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0270
 * @tc.desc      : ns_set_asan_permitted_paths test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0270(void)
{
    ns_set_asan_permitted_paths(NULL, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0280
 * @tc.desc      : ns_set_asan_permitted_paths test arg asan_permitted_paths is NULL and ns.asan_permitted_paths is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0280(void)
{
    ns_t ns;
    ns.asan_permitted_paths = NULL;
    ns_set_asan_permitted_paths(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.asan_permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0290
 * @tc.desc      : ns_set_separated test arg ns is NULL and separated is false
 * @tc.level     : Level 2
 */
static void namespace_test_0290(void)
{
    ns_set_separated(NULL, false);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0300
 * @tc.desc      : ns_set_allowed_libs test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0300(void)
{
    ns_set_allowed_libs(NULL, "abc");
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0310
 * @tc.desc      : ns_set_allowed_libs test arg ns is ns.allowed_libs NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0310(void)
{
    ns_t ns;
    ns.allowed_libs = NULL;
    ns_set_allowed_libs(&ns, "abc");
    EXPECT_EQ(__FUNCTION__, (ns.allowed_libs != NULL), true);
}

/**
 * @tc.name      : namespace_test_0320
 * @tc.desc      : ns_set_allowed_libs test normal condition
 * @tc.level     : Level 0
 */
static void namespace_test_0320(void)
{
    ns_t ns;
    strlist list;
    ns.allowed_libs = &list;
    ns_set_allowed_libs(&ns, "abc");
    EXPECT_EQ(__FUNCTION__, (ns.allowed_libs != NULL), true);
}

/**
 * @tc.name      : namespace_test_0330
 * @tc.desc      : ns_set_allowed_libs test allowed_libs is NULL and ns.allowed_libs is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0330(void)
{
    ns_t ns;
    ns.allowed_libs = NULL;
    ns_set_allowed_libs(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.allowed_libs, NULL);
}

/**
 * @tc.name      : namespace_test_0400
 * @tc.desc      : ns_set_allowed_libs test allowed_libs is NULL and ns.allowed_libs is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0400(void)
{
    ns_t ns;
    strlist list;
    ns.allowed_libs = &list;
    ns_set_allowed_libs(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ns.allowed_libs, NULL);
}

/**
 * @tc.name      : namespace_test_0340
 * @tc.desc      : ns_add_inherit test arg ns_inherited and shared_libs are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0340(void)
{
    ns_t ns;
    ns_add_inherit(&ns, NULL, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0350
 * @tc.desc      : ns_add_inherit test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0350(void)
{

    ns_add_inherit(NULL, NULL, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0360
 * @tc.desc      : ns_add_inherit test arg ns and shared_libs are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0360(void)
{
    ns_t ns;
    ns_add_inherit(NULL, &ns, NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : namespace_test_0370
 * @tc.desc      : is_accessible test arg lib_pathname is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0370(void)
{
    ns_t ns;
    ns.separated = true;
    bool ret = is_accessible(&ns, NULL , true, true);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0380
 * @tc.desc      : is_accessible test args is (&ns, NULL, true, false) and ns.separated is false
 * @tc.level     : Level 2
 */
static void namespace_test_0380(void)
{
    ns_t ns;
    ns.separated = false;
    bool ret = is_accessible(&ns, NULL, true, false);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0390
 * @tc.desc      : is_accessible test args is (&ns, "/data/test.so", true, false) and ns.separated is false
 * @tc.level     : Level 2
 */
static void namespace_test_0390(void)
{
    ns_t ns;
    ns.separated = false;
    strlist list;
    list.num = 1;
    char *name = "test.so";
    list.strs = &name;

    ns.allowed_libs = &list;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/test.so", true, false);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0410
 * @tc.desc      : is_sharable test args are NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0410(void)
{
    bool ret = is_sharable(NULL, NULL);
    EXPECT_EQ(__FUNCTION__, ret, true);    
}

/**
 * @tc.name      : namespace_test_0420
 * @tc.desc      : is_sharable test args lib_name are NULL
 * @tc.level     : Level 1
 */
void namespace_test_0420(void)
{
    ns_inherit ns;
    bool ret = is_sharable(&ns, NULL);
    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0440
 * @tc.desc      : is_sharable test args shared_libs are NULL
 * @tc.level     : Level 1
 */
void namespace_test_0440(void)
{
    ns_inherit ns;
    ns.shared_libs = NULL;
    bool ret = is_sharable(&ns, "lib.so");
    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0450
 * @tc.desc      : is_accessible test args allowed_libs are NULL
 * @tc.level     : Level 2
 */
void namespace_test_0450(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    bool ret = is_accessible(&ns, "/data/test.so", true, false);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0460
 * @tc.desc      : is_accessible test args allowed_libs are not NULL
 * @tc.level     : Level 2
 */
void namespace_test_0460(void)
{
    ns_t ns;
    ns.separated = false;
    strlist list;
    list.num = 0;
    ns.allowed_libs = &list;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "test.so", true, false);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0470
 * @tc.desc      : is_accessible test args env_paths are not NULL
 * @tc.level     : Level 2
 */
void namespace_test_0470(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = "/data/tests:/etc";
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/abc/", true, false);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0480
 * @tc.desc      : is_accessible test param is NULL
 * @tc.level     : Level 2
 */
void namespace_test_0480(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/test.so", true, false);

    ns.env_paths = "";
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0490
 * @tc.desc      : is_accessible test env_paths is matching
 * @tc.level     : Level 1
 */
void namespace_test_0490(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = "/data/test:/etc";
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/test/", true, false);

    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0500
 * @tc.desc      : is_accessible test env_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0500(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = "/data/test:/etc";
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/test", true, false);

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0510
 * @tc.desc      : is_accessible test env_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0510(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = "/data/test:/etc";
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/test//", true, false);

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0520
 * @tc.desc      : is_accessible test asan_lib_paths is matching
 * @tc.level     : Level 2
 */
void namespace_test_0520(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = "/data/test:/etc";
    ns.lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/test/", true, false);

    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0530
 * @tc.desc      : is_accessible test permitted_paths is matching
 * @tc.level     : Level 1
 */
void namespace_test_0530(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;

    strlist list;
    list.num = 1;
    char *name = "/test";
    list.strs = &name;

    ns.permitted_paths = &list;

    bool ret = is_accessible(&ns, "/test/", true, false);

    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0540
 * @tc.desc      : is_accessible test permitted_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0540(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;

    strlist list;
    list.num = 1;
    char *name = "/test";
    list.strs = &name;

    ns.permitted_paths = &list;

    bool ret = is_accessible(&ns, "/test", true, false);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0550
 * @tc.desc      : is_accessible test permitted_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0550(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;

    strlist list;
    list.num = 1;
    char *name = "/abc";
    list.strs = &name;

    ns.permitted_paths = &list;

    bool ret = is_accessible(&ns, "/test", true, false);

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0560
 * @tc.desc      : is_accessible test env_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0560(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = "/data/test:/etc";
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/abc/", true, false);
    
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0570
 * @tc.desc      : check_asan_path test wiht invalid input
 * @tc.level     : Level 2
 */
void namespace_test_0570(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = NULL;
    bool ret = check_asan_path(&ns, "/data/test/");

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0580
 * @tc.desc      : check_asan_path test asan_lib_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0580(void)
{
    ns_t ns;
    ns.asan_lib_paths = "";
    ns.asan_permitted_paths = NULL;
    bool ret = check_asan_path(&ns, "/abc/");

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0590
 * @tc.desc      : check_asan_path test asan_lib_paths is matching
 * @tc.level     : Level 1
 */
void namespace_test_0590(void)
{
    ns_t ns;
    ns.asan_lib_paths = "/data/test:/etc";
    ns.asan_permitted_paths = NULL;
    bool ret = check_asan_path(&ns, "/data/test/");
    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0600
 * @tc.desc      : check_asan_path test asan_lib_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0600(void)
{
    ns_t ns;
    ns.asan_lib_paths = "/data/test:/etc";
    ns.asan_permitted_paths = NULL;
    bool ret = check_asan_path(&ns, "/data/test");
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0610
 * @tc.desc      : check_asan_path test asan_lib_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0610(void)
{
    ns_t ns;
    ns.asan_lib_paths = "/data/test:/etc";
    ns.asan_permitted_paths = NULL;
    bool ret = check_asan_path(&ns, "/data/test//");
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0630
 * @tc.desc      : is_accessible test lib_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0630(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;

    ns.lib_paths = "";

    bool ret = is_accessible(&ns, "/test/", true, false);

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0640
 * @tc.desc      : is_accessible test lib_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0640(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;

    ns.lib_paths = "/data/test:/ext";

    bool ret = is_accessible(&ns, "/bct/", true, false);

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0650
 * @tc.desc      : is_accessible test lib_paths is matching
 * @tc.level     : Level 1
 */
void namespace_test_0650(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;

    ns.lib_paths = "/data/test:/ext";

    bool ret = is_accessible(&ns, "/data/test/", true, false);

    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0660
 * @tc.desc      : is_accessible test lib_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0660(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;

    ns.lib_paths = "/data/test:/ext";

    bool ret = is_accessible(&ns, "/data/test//", true, false);

    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0670
 * @tc.desc      : check_asan_path test asan_permitted_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0670(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;

    strlist list;
    list.num = 1;
    char *name = "/abc";
    list.strs = &name;

    ns.asan_permitted_paths = &list;
    bool ret = check_asan_path(&ns, "/test//");
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0680
 * @tc.desc      : check_asan_path test asan_permitted_paths is matching
 * @tc.level     : Level 1
 */
void namespace_test_0680(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;

    strlist list;
    list.num = 1;
    char *name = "/test";
    list.strs = &name;

    ns.asan_permitted_paths = &list;
    bool ret = check_asan_path(&ns, "/test/");
    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0690
 * @tc.desc      : check_asan_path test asan_permitted_paths is matching
 * @tc.level     : Level 1
 */
void namespace_test_0690(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;

    strlist list;
    list.num = 1;
    char *name = "/test";
    list.strs = &name;

    ns.asan_permitted_paths = &list;
    bool ret = check_asan_path(&ns, "/test//");
    EXPECT_EQ(__FUNCTION__, ret, true);
}

/**
 * @tc.name      : namespace_test_0700
 * @tc.desc      : check_asan_path test asan_permitted_paths is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0700(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;

    strlist list;
    list.num = 0;

    ns.asan_permitted_paths = &list;
    bool ret = check_asan_path(&ns, "/test//");
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0710
 * @tc.desc      : is_accessible test input param are null
 * @tc.level     : Level 2
 */
void namespace_test_0710(void)
{
    ns_t ns;
    ns.separated = false;
    ns.allowed_libs = NULL;
    ns.env_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    ns.lib_paths = NULL;
    bool ret = is_accessible(&ns, NULL , false, false);
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : namespace_test_0720
 * @tc.desc      : is_sharable test shared_libs is matching
 * @tc.level     : Level 1
 */
void namespace_test_0720(void)
{
    ns_inherit ns;

    strlist list;
    list.num = 1;
    char *name = "/test";
    list.strs = &name;
    ns.shared_libs = &list;

    bool ret = is_sharable(&ns, "/test");
    EXPECT_EQ(__FUNCTION__, ret, true);    
}

/**
 * @tc.name      : namespace_test_0730
 * @tc.desc      : is_sharable test shared_libs is not matching
 * @tc.level     : Level 2
 */
void namespace_test_0730(void)
{
    ns_inherit ns;

    strlist list;
    list.num = 0;
    ns.shared_libs = &list;

    bool ret = is_sharable(&ns, "/test");
    EXPECT_EQ(__FUNCTION__, ret, false);    
}

// Because below function are static in file and call it difficult through External API, below interfaces are covered by white-box unit testing.
#ifdef UNIT_TEST_STATIC
void nsinherits_realloc(ns_inherit_list *nsinl);

/**
 * @tc.name      : namespace_test_0740
 * @tc.desc      : Test nsinherits_realloc with invalid input
 * @tc.level     : Level 2
 */
void namespace_test_0740(void)
{
    nsinherits_realloc(NULL);
    EXPECT_TRUE(__FUNCTION__, true);    
}

/**
 * @tc.name      : namespace_test_0750
 * @tc.desc      : Test nsinherits_realloc with normal input
 * @tc.level     : Level 0
 */
void namespace_test_0750(void)
{
    ns_inherit_list nsinl;
    nsinl.size = 1;
    nsinherits_realloc(&nsinl);
    EXPECT_EQ(__FUNCTION__, nsinl.size, 2);    
}
#endif

TEST_FUN G_Fun_Array[] = {
    namespace_test_0010,
    namespace_test_0020,
    namespace_test_0030,
    namespace_test_0040,
    namespace_test_0050,
    namespace_test_0060,
    namespace_test_0070,
    namespace_test_0080,
    namespace_test_0090,
    namespace_test_0100,
    namespace_test_0110,
    namespace_test_0120,
    namespace_test_0130,
    namespace_test_0140,
    namespace_test_0150,
    namespace_test_0160,
    namespace_test_0170,
    namespace_test_0180,
    namespace_test_0190,
    namespace_test_0200,
    namespace_test_0210,
    namespace_test_0220,
    namespace_test_0230,
    namespace_test_0250,
    namespace_test_0260,
    namespace_test_0270,
    namespace_test_0280,
    namespace_test_0290,
    namespace_test_0300,
    namespace_test_0310,
    namespace_test_0320,
    namespace_test_0330,
    namespace_test_0340,
    namespace_test_0350,
    namespace_test_0360,
    namespace_test_0370,
    namespace_test_0380,
    namespace_test_0390,
    namespace_test_0400,
    namespace_test_0410,
    namespace_test_0420,
    namespace_test_0440,
    namespace_test_0450,
    namespace_test_0460,
    namespace_test_0470,
    namespace_test_0480,
    namespace_test_0490,
    namespace_test_0500,
    namespace_test_0510,
    namespace_test_0520,
    namespace_test_0530,
    namespace_test_0540,
    namespace_test_0550,
    namespace_test_0560,
    namespace_test_0570,
    namespace_test_0580,
    namespace_test_0590,
    namespace_test_0600,
    namespace_test_0610,
    namespace_test_0630,
    namespace_test_0640,
    namespace_test_0650,
    namespace_test_0660,
    namespace_test_0670,
    namespace_test_0680,
    namespace_test_0690,
    namespace_test_0700,
    namespace_test_0710,
    namespace_test_0720,
    namespace_test_0730,
#ifdef UNIT_TEST_STATIC
    namespace_test_0740,
    namespace_test_0750,
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
