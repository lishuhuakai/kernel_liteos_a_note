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

#include <dlfcn_ext.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "test.h"

#define PIPE_FD 2
#define TEST_COUNTS 32

const char *LIB_NAME = "./libldso_randomization_dep.so";
const char *LIB_NAME_A = "./libldso_randomization_dep_a.so";
typedef void (*TEST_FUNC)(void);

static uintptr_t compare(const void *a, const void *b)
{
    return (*(uintptr_t *)a - *(uintptr_t *)b);
}

static bool check_address(void **array, size_t length)
{
    qsort(array, length, sizeof(void *), (void *)compare);
    for (size_t i = 0, j = 1; j < length; i++, j++) {
        if (array[i] == array[j]) {
            return false;
        }
    }
    return true;
}

/**
 * @tc.name      : dlopen_randomization_0500
 * @tc.desc      : Repeatedly calling the dlopen interface to load the dynamic library,
 *                 and calling dlclose to unload each time
 * @tc.level     : Level3
 */
static void dlopen_randomization_0500(void)
{
    void *handles[TEST_COUNTS] = {0};
    void *handle = 0;
    int i = TEST_COUNTS;
    do {
        handle = dlopen(LIB_NAME, RTLD_NOW);
        handles[--i] = handle;
        if (handle) {
            dlclose(handle);
        }
    } while (i);
    if (check_address(handles, TEST_COUNTS)) {
        t_printf("%s handles are randomized\n", __FUNCTION__);
    } else {
        t_printf("%s exists the same handle!!!!\n", __FUNCTION__);
    }
}

/**
 * @tc.name      : dlopen_randomization_0700
 * @tc.desc      : Different processes call the dlopen interface to load dynamic libraries
 * @tc.level     : Level3
 */
static void dlopen_randomization_0700(void)
{
    void *parent_handle = 0;
    void *get_child_handle = 0;
    int fd[PIPE_FD];
    if (pipe(fd) < 0) {
        t_printf("%s create pipe error!\n", __FUNCTION__);
        return;
    }
    pid_t pid = fork();
    if (pid == 0) {
        void *child_handle = dlopen(LIB_NAME, RTLD_NOW);
        t_printf("%s child handle is %p \n", __FUNCTION__, child_handle);
        if (child_handle) {
            dlclose(child_handle);
        }
        write(fd[1], &child_handle, sizeof(void *));
        exit(0);
    }
    parent_handle = dlopen(LIB_NAME, RTLD_NOW);
    t_printf("%s parent handle is %p \n", __FUNCTION__, parent_handle);
    if (parent_handle) {
        dlclose(parent_handle);
    }
    int status;
    waitpid(pid, &status, 0);
    read(fd[0], &get_child_handle, sizeof(void *));
    t_printf("%s get child handle is %p\n", __FUNCTION__, get_child_handle);
    if (parent_handle == get_child_handle) {
        t_printf("%s the parent handle is the same as the child handle!!!!\n", __FUNCTION__);
    } else {
        t_printf("%s the parent handle is different form the child handle.\n", __FUNCTION__);
    }
    close(fd[0]);
    close(fd[1]);
}

/**
 * @tc.name      : dlopen_ext_randomization_0500
 * @tc.desc      : Repeatedly calling the dlopen_ext interface to load the dynamic library,
 *                 and calling dlclose to unload each time
 * @tc.level     : Level3
 */
static void dlopen_ext_randomization_0500(void)
{
    dl_extinfo extinfo = {0};
    void *handles[TEST_COUNTS] = {0};
    void *handle = 0;
    int i = TEST_COUNTS;
    do {
        handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
        handles[--i] = handle;
        if (handle) {
            dlclose(handle);
        }
    } while (i);
    if (check_address(handles, TEST_COUNTS)) {
        t_printf("%s handles are randomized\n", __FUNCTION__);
    } else {
        t_printf("%s exists the same handle!!!!\n", __FUNCTION__);
    }
}

/**
 * @tc.name      : dlopen_ext_randomization_0700
 * @tc.desc      : Different processes call the dlopen_ext interface to load dynamic libraries
 * @tc.level     : Level3
 */
static void dlopen_ext_randomization_0700(void)
{
    void *parent_handle = 0;
    void *get_child_handle = 0;
    dl_extinfo extinfo = {0};
    int fd[PIPE_FD];
    if (pipe(fd) < 0) {
        t_printf("%s create pipe error!\n", __FUNCTION__);
        return;
    }
    pid_t pid = fork();
    if (pid == 0) {
        void *child_handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
        t_printf("%s child handle is %p \n", __FUNCTION__, child_handle);
        if (child_handle) {
            dlclose(child_handle);
        }
        write(fd[1], &child_handle, sizeof(void *));
        exit(0);
    }
    parent_handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    t_printf("%s parent handle is %p \n", __FUNCTION__, parent_handle);
    if (parent_handle) {
        dlclose(parent_handle);
    }
    int status;
    waitpid(pid, &status, 0);
    read(fd[0], &get_child_handle, sizeof(void *));
    t_printf("%s get child handle is %p\n", __FUNCTION__, get_child_handle);
    if (parent_handle == get_child_handle) {
        t_printf("%s the parent handle is the same as the child handle!!!!\n", __FUNCTION__);
    } else {
        t_printf("%s the parent handle is different form the child handle.\n", __FUNCTION__);
    }
    close(fd[0]);
    close(fd[1]);
}

/**
 * @tc.name      : dlopen_ns_randomization_0500
 * @tc.desc      : Repeatedly calling the dlopen_ns interface to load the dynamic library,
 *                 and calling dlclose to unload each time
 * @tc.level     : Level3
 */
static void dlopen_ns_randomization_0500(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, __FUNCTION__);
    dlns_create(&dlns, "./");

    void *handles[TEST_COUNTS] = {0};
    void *handle = 0;
    int i = TEST_COUNTS;
    do {
        handle = dlopen_ns(&dlns, LIB_NAME, RTLD_NOW);
        handles[--i] = handle;
        if (handle) {
            dlclose(handle);
        }
    } while (i);
    if (check_address(handles, TEST_COUNTS)) {
        t_printf("%s handles are randomized\n", __FUNCTION__);
    } else {
        t_printf("%s exists the same handle!!!!\n", __FUNCTION__);
    }
}

/**
 * @tc.name      : dlopen_ns_randomization_0700
 * @tc.desc      : Different processes call the dlopen_ns interface to load dynamic libraries
 * @tc.level     : Level3
 */
static void dlopen_ns_randomization_0700(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, __FUNCTION__);
    dlns_create(&dlns, "./");

    void *parent_handle = 0;
    void *get_child_handle = 0;
    int fd[PIPE_FD];
    if (pipe(fd) < 0) {
        t_printf("%s create pipe error!\n", __FUNCTION__);
        return;
    }
    pid_t pid = fork();
    if (pid == 0) {
        void *child_handle = dlopen_ns(&dlns, LIB_NAME, RTLD_NOW);
        t_printf("%s child handle is %p \n", __FUNCTION__, child_handle);
        if (child_handle) {
            dlclose(child_handle);
        }
        write(fd[1], &child_handle, sizeof(void *));
        exit(0);
    }
    parent_handle = dlopen_ns(&dlns, LIB_NAME, RTLD_NOW);
    t_printf("%s parent handle is %p \n", __FUNCTION__, parent_handle);
    if (parent_handle) {
        dlclose(parent_handle);
    }
    int status;
    waitpid(pid, &status, 0);
    read(fd[0], &get_child_handle, sizeof(void *));
    t_printf("%s get child handle is %p\n", __FUNCTION__, get_child_handle);
    if (parent_handle == get_child_handle) {
        t_printf("%s the parent handle is the same as the child handle!!!!\n", __FUNCTION__);
    } else {
        t_printf("%s the parent handle is different form the child handle.\n", __FUNCTION__);
    }
    close(fd[0]);
    close(fd[1]);
}

/**
 * @tc.name      : dlsym_randomization_0100
 * @tc.desc      : Repeatedly calling the dlsym interface to get symbol address, and calling dlclose to unload each time
 * @tc.level     : Level3
 */
static void dlsym_randomization_0100(void)
{
    void *handle = dlopen(LIB_NAME, RTLD_NOW);
    if (!handle) {
        t_printf("%s dlopen failed: %s\n", __FUNCTION__, LIB_NAME);
        return;
    };
    void *sym = dlsym(handle, "test");
    t_printf("%s symbol address is %p\n", __FUNCTION__, sym);
    dlclose(handle);
}

/**
 * @tc.name      : dlsym_randomization_0200
 * @tc.desc      : Different processes call the dlsym interface to get symbol address
 * @tc.level     : Level3
 */
static void dlsym_randomization_0200(void)
{
    void *parent_handle = 0;
    void *child_handle = 0;
    void *parent_sym = 0;
    void *child_sym = 0;
    void *get_child_sym = 0;
    int fd[PIPE_FD];
    if (pipe(fd) < 0) {
        t_printf("%s create pipe error!\n", __FUNCTION__);
        return;
    }
    pid_t pid = fork();
    if (pid == 0) {
        child_handle = dlopen(LIB_NAME_A, RTLD_NOW);
        if (!child_handle) {
            t_printf("%s child process dlopen failed: %s\n", __FUNCTION__, LIB_NAME_A);
            exit(-1);
        }
        child_sym = dlsym(child_handle, "test");
        t_printf("%s child sym is %p\n", __FUNCTION__, child_sym);
        write(fd[1], &child_sym, sizeof(void *));
        dlclose(child_handle);
        exit(0);
    }
    parent_handle = dlopen(LIB_NAME_A, RTLD_NOW);
    if (!parent_handle) {
        t_printf("%s parent process dlopen failed: %s\n", __FUNCTION__, LIB_NAME_A);
        return;
    };
    parent_sym = dlsym(parent_handle, "test");
    t_printf("%s parent sym is %p\n", __FUNCTION__, parent_sym);
    dlclose(parent_handle);
    int status;
    waitpid(pid, &status, 0);
    read(fd[0], &get_child_sym, sizeof(void *));
    if (parent_sym == get_child_sym) {
        t_printf("%s the parent sym is the same as the child sym!!!!\n", __FUNCTION__);
    } else {
        t_printf("%s the parent sym is different form the child sym.\n", __FUNCTION__);
    }
    close(fd[0]);
    close(fd[1]);
}

TEST_FUNC test_cases[] = {
    dlopen_randomization_0500,
    dlopen_randomization_0700,
    dlopen_ext_randomization_0500,
    dlopen_ext_randomization_0700,
    dlopen_ns_randomization_0500,
    dlopen_ns_randomization_0700,
    dlsym_randomization_0100,
    dlsym_randomization_0200,
};

int main(int argc, char *argv[])
{
    size_t case_counts = sizeof test_cases / sizeof(TEST_FUNC);
    for (size_t case_num = 0; case_num < case_counts; case_num++) {
        test_cases[case_num]();
    }
    return t_status;
}