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
#include "ns_config.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

/**
 * @tc.name      : ns_config_0010
 * @tc.desc      : Set default error callback for configor
 * @tc.level     : Level 1
 */
static void ns_config_test_0010(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(NULL);
    configor_free();

    EXPECT_TRUE(__FUNCTION__, true);
}

int errorback(const char* format, ...)
{
    return 0;
}

/**
 * @tc.name      : ns_config_0030
 * @tc.desc      : Set custom error callback for configor
 * @tc.level     : Level 0
 */
static void ns_config_test_0020(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(&errorback);
    configor_free();

    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : ns_config_0030
 * @tc.desc      : Test parse with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0030(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(NULL);
    int ret = configor->parse("ttttttttt.txt", "ttttttttt");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, -2);
}

/**
 * @tc.name      : ns_config_0040
 * @tc.desc      : Test get_namespaces with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0040(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    strlist *ret = configor->get_namespaces();
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0050
 * @tc.desc      : Test get_namespaces with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0050(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    kvlist kv;
    kv.num = 1;
    char *key = "a";
    char *val = "b";
    kv.key = &key;
    kv.val = &val;
    configor->kvs = &kv;
    strlist *ret = configor->get_namespaces();
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0060
 * @tc.desc      : Test get_namespaces with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0060(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    kvlist kv;
    kv.num = 1;
    char *key = "added.nslist";
    char *val = "b";
    kv.key = &key;
    kv.val = &val;
    configor->kvs = &kv;
    strlist *ret = configor->get_namespaces();
    configor_free();
    EXPECT_PTRNE(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0070
 * @tc.desc      : Test get_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0070(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_lib_paths(NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0080
 * @tc.desc      : Test get_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0080(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_lib_paths("test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0090
 * @tc.desc      : Test get_asan_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0090(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_asan_lib_paths(NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0100
 * @tc.desc      : Test get_asan_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0100(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_asan_lib_paths("test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0110
 * @tc.desc      : Test get_permitted_paths with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0110(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_permitted_paths(NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0120
 * @tc.desc      : Test get_permitted_paths with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0120(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_permitted_paths("test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0130
 * @tc.desc      : Test get_inherits with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0130(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    strlist* ret = configor->get_inherits(NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret , NULL);
}

/**
 * @tc.name      : ns_config_0140
 * @tc.desc      : Test get_inherits with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0140(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    strlist* ret = configor->get_inherits("test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0150
 * @tc.desc      : Test get_separated with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0150(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    bool ret = configor->get_separated(NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : ns_config_0160
 * @tc.desc      : Test get_separated with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0160(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    bool ret = configor->get_separated("test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, false);
}

/**
 * @tc.name      : ns_config_0170
 * @tc.desc      : Test get_allowed_libs with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0170(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_allowed_libs(NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0180
 * @tc.desc      : Test get_allowed_libs with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0180(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_allowed_libs("test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0180
 * @tc.desc      : Test get_inherit_shared_libs with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0190(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_inherit_shared_libs(NULL, NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0200
 * @tc.desc      : Test get_inherit_shared_libs with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0200(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_inherit_shared_libs("test", "abc");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0210
 * @tc.desc      : get_inherit_shared_libs test coverage
 * @tc.level     : Level 1
 */
void ns_config_test_0210(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_asan_sys_paths();
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0220
 * @tc.desc      : Test get_asan_permitted_paths with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0220(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_asan_permitted_paths(NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0230
 * @tc.desc      : Test get_asan_permitted_paths with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0230(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_asan_permitted_paths("test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0240
 * @tc.desc      : Test get_inherit_shared_libs with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0240(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_inherit_shared_libs(NULL, "test");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0250
 * @tc.desc      : Test get_inherit_shared_libs with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0250(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_inherit_shared_libs("test", NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, NULL);
}

/**
 * @tc.name      : ns_config_0260
 * @tc.desc      : Test parse with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0260(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(NULL);
    int ret = configor->parse("/etc/ld-musl-namespace-arm-test.ini", "/data/tests/libc-test/src");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : ns_config_0270
 * @tc.desc      : Test parse with normal input
 * @tc.level     : Level 1
 */
void ns_config_test_0270(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(NULL);
    int ret = configor->parse(NULL, "/");
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

/**
 * @tc.name      : ns_config_0280
 * @tc.desc      : Test parse with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0280(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(NULL);
    int ret = configor->parse("/etc/ld-musl-namespace-arm-test.ini", NULL);
    configor_free();
    EXPECT_EQ(__FUNCTION__, ret, -1);
}

// Because below function are static in file and call it difficult through External API, below interfaces are covered by white-box unit testing.
#ifdef UNIT_TEST_STATIC
void kvlist_realloc(kvlist *kvs);
void sections_realloc(section_list *sections);

/**
 * @tc.name      : ns_config_test_0290
 * @tc.desc      : Test kvlist_realloc with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0290(void)
{
    kvlist_realloc(NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : ns_config_test_0300
 * @tc.desc      : Test kvlist_realloc with normal input
 * @tc.level     : Level 0
 */
void ns_config_test_0300(void)
{
    kvlist kvs;
    kvs.size = 1;
    kvs.key = (char **)calloc(kvs.size, sizeof *kvs.key);
    kvs.val = (char **)calloc(kvs.size, sizeof *kvs.val);
    kvlist_realloc(&kvs);
    EXPECT_EQ(__FUNCTION__, kvs.size, 2);
    if (kvs.key){
        free(kvs.key);
    }
    if (kvs.val){
        free(kvs.val);
    }
}

/**
 * @tc.name      : ns_config_test_0310
 * @tc.desc      : Test sections_realloc with invalid input
 * @tc.level     : Level 2
 */
void ns_config_test_0310(void)
{
    sections_realloc(NULL);
    EXPECT_TRUE(__FUNCTION__, true);
}

/**
 * @tc.name      : ns_config_test_0320
 * @tc.desc      : Test sections_realloc with normal input
 * @tc.level     : Level 0
 */
void ns_config_test_0320(void)
{
    section_list sections;
    sections.size = 1;
    sections.names = (char**)calloc(sections.size, sizeof *sections.names);
    sections.kvs = (kvlist**)calloc(sections.size, sizeof *sections.kvs);
    sections_realloc(&sections);
    EXPECT_EQ(__FUNCTION__, sections.size, 2);
    if (sections.names){
        free(sections.names);
    }
    if (sections.kvs){
        free(sections.kvs);
    }
}
#endif

TEST_FUN G_Fun_Array[] = {
    ns_config_test_0010,
    ns_config_test_0020,
    ns_config_test_0030,
    ns_config_test_0040,
    ns_config_test_0050,
    ns_config_test_0060,
    ns_config_test_0070,
    ns_config_test_0080,
    ns_config_test_0090,
    ns_config_test_0100,
    ns_config_test_0110,
    ns_config_test_0120,
    ns_config_test_0130,
    ns_config_test_0140,
    ns_config_test_0150,
    ns_config_test_0160,
    ns_config_test_0170,
    ns_config_test_0180,
    ns_config_test_0190,
    ns_config_test_0200,
    ns_config_test_0210,
    ns_config_test_0220,
    ns_config_test_0230,
    ns_config_test_0240,
    ns_config_test_0250,
    ns_config_test_0260,
    ns_config_test_0270,
    ns_config_test_0280,
#ifdef UNIT_TEST_STATIC
    ns_config_test_0290,
    ns_config_test_0300,
    ns_config_test_0310,
    ns_config_test_0320,
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