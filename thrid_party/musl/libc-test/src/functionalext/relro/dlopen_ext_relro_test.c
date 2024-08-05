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
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "functionalext.h"
#include "test.h"

#define LIB_NAME "/data/tests/libc-test/src/libdlopen_ext_relro_dso.so"
#define RELRO_FILE_PATH "./TemporaryFile-XXXXXX"
#define RESERVED_ADDRESS_SIZE (64 * 1024 * 1024)
#define MIN_RESERVED_ADDRESS_SIZE 4096

const char *RELRO_NS_PATH = "./";
const int RELRO_INVALID_FLAG = 0x20;
const int LIB_PATH_SIZE = 512;
typedef void (*TEST_FUN)(void);

static int create_temp_relro_file(char *path)
{
    int fd = -1;
#if defined(_WIN32)
    fd = mkstemp(path, strlen(RELRO_FILE_PATH) + 1);
#else
    fd = mkstemp(path);
#endif
    if (fd != -1) {
        close(fd);
    }
    return 0;
}

static void tear_down(const int fd, const char *relro_path)
{
    if (fd != -1) {
        close(fd);
    }

    if (relro_path != NULL) {
        unlink(relro_path);
    }
}

static void clear_handle(void *handle)
{
    if (handle) {
        dlclose(handle);
    }
}

static void test_write_relro_file(const char *lib, const int relro_fd)
{
    pid_t pid = fork();
    if (pid == 0) {
        dl_extinfo extinfo = {
            .flag = DL_EXT_WRITE_RELRO,
            .relro_fd = relro_fd,
        };
        void *handle = dlopen_ext(lib, RTLD_NOW, &extinfo);
        EXPECT_PTRNE(__FUNCTION__, handle, NULL);
        clear_handle(handle);
        exit(-1);
    }
    int status = 0;
    waitpid(pid, &status, 0);
}

static void test_write_relro_file_ext(
    const char *lib, const int relro_fd, const int flag, void *addr, const size_t map_size)
{
    pid_t pid = fork();
    if (pid == 0) {
        dl_extinfo extinfo = {
            .flag = flag,
            .relro_fd = relro_fd,
            .reserved_addr = addr,
            .reserved_size = map_size,
        };
        void *handle = dlopen_ext(lib, RTLD_NOW, &extinfo);
        EXPECT_PTRNE(__FUNCTION__, handle, NULL);
        clear_handle(handle);
        exit(-1);
    }
    int status = 0;
    waitpid(pid, &status, 0);
}

/**
 * @tc.name      : dlopen_ext_relro_0100
 * @tc.desc      : extinfo is NULL, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_0100(void)
{
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, NULL);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0200
 * @tc.desc      : extinfo flag is 0, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_0200(void)
{
    dl_extinfo extinfo = {
        .flag = 0,
        .relro_fd = -1,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0300
 * @tc.desc      : extinfo flag is 20, call dlopen_ext, return handle is NULL.
 * @tc.level     : Level 2
 */
static void dlopen_ext_relro_0300(void)
{
    dl_extinfo extinfo = {
        .flag = RELRO_INVALID_FLAG,
        .relro_fd = -1,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_EQ(__FUNCTION__, handle, NULL);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0400
 * @tc.desc      : extinfo flag is DL_EXT_WRITE_RELRO, relro_fd is valid, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_0400(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    dl_extinfo extinfo = {
        .flag = DL_EXT_WRITE_RELRO,
        .relro_fd = relro_fd,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0500
 * @tc.desc      : extinfo flag is DL_EXT_WRITE_RELRO, relro_fd is not valid, call dlopen_ext, return handle is NULL.
 * @tc.level     : Level 2
 */
static void dlopen_ext_relro_0500(void)
{
    dl_extinfo extinfo = {
        .flag = DL_EXT_WRITE_RELRO,
        .relro_fd = -1,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_EQ(__FUNCTION__, handle, NULL);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0600
 * @tc.desc      : extinfo flag is DL_EXT_USE_RELRO, relro_fd is valid, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_0600(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    test_write_relro_file(LIB_NAME, relro_fd);
    dl_extinfo extinfo = {
        .flag = DL_EXT_USE_RELRO,
        .relro_fd = relro_fd,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0700
 * @tc.desc      : extinfo flag is DL_EXT_USE_RELRO, relro_fd is not valid, call dlopen_ext, return handle is NULL.
 * @tc.level     : Level 2
 */
static void dlopen_ext_relro_0700(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    test_write_relro_file(LIB_NAME, relro_fd);
    dl_extinfo extinfo = {
        .flag = DL_EXT_USE_RELRO,
        .relro_fd = -1,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_EQ(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0800
 * @tc.desc      : extinfo flag is DL_EXT_WRITE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE, relro_fd is valid,
 *                 call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_0800(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    dl_extinfo extinfo = {
        .flag = DL_EXT_WRITE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE,
        .relro_fd = relro_fd,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_0900
 * @tc.desc      : extinfo flag is DL_EXT_WRITE_RELRO, relro_fd is not valid, call dlopen_ext, return handle is NULL.
 * @tc.level     : Level 2
 */
static void dlopen_ext_relro_0900(void)
{
    dl_extinfo extinfo = {
        .flag = DL_EXT_WRITE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE,
        .relro_fd = -1,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_EQ(__FUNCTION__, handle, NULL);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_1000
 * @tc.desc      : extinfo flag is DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE, relro_fd is valid,
 *                 call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_1000(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    test_write_relro_file(LIB_NAME, relro_fd);
    dl_extinfo extinfo = {
        .flag = DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE,
        .relro_fd = relro_fd,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_1100
 * @tc.desc      : extinfo flag is DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE, relro_fd is not valid,
 *                 call dlopen_ext, return handle is NULL.
 * @tc.level     : Level 2
 */
static void dlopen_ext_relro_1100(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    test_write_relro_file(LIB_NAME, relro_fd);
    dl_extinfo extinfo = {
        .flag = DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS_RECURSIVE,
        .relro_fd = -1,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_EQ(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_1200
 * @tc.desc      : extinfo flag is DL_EXT_USE_RELRO when relro file do not write,
 *                 when call dlopen_ext, return handle is NULL.
 * @tc.level     : Level 2
 */
static void dlopen_ext_relro_1200(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    dl_extinfo extinfo = {
        .flag = DL_EXT_USE_RELRO,
        .relro_fd = relro_fd,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_EQ(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_1500
 * @tc.desc      : extinfo flag is DL_EXT_USE_RELRO | DL_EXT_WRITE_RELRO, relro_fd is valid,
 *                 call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_1500(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    test_write_relro_file(LIB_NAME, relro_fd);
    dl_extinfo extinfo = {
        .flag = DL_EXT_WRITE_RELRO | DL_EXT_USE_RELRO,
        .relro_fd = relro_fd,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
}

/**
 * @tc.name      : dlopen_ext_relro_1600
 * @tc.desc      : extinfo flag is DL_EXT_RESERVED_ADDRESS, relro_fd is valid, reserved address size is enough,
 *                 call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_1600(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    dl_extinfo extinfo = {
        .flag = DL_EXT_RESERVED_ADDRESS,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_1700
 * @tc.desc      : extinfo flag is DL_EXT_RESERVED_ADDRESS_HINT, relro_fd is valid, reserved address size is enough,
 *                 call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_1700(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    dl_extinfo extinfo = {
        .flag = DL_EXT_RESERVED_ADDRESS_HINT,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_1800
 * @tc.desc      : extinfo flag is DL_EXT_RESERVED_ADDRESS | DL_EXT_RESERVED_ADDRESS_RECURSIVE, relro_fd is valid,
 *                 reserved address size is enough, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_1800(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    dl_extinfo extinfo = {.flag = DL_EXT_RESERVED_ADDRESS | DL_EXT_RESERVED_ADDRESS_RECURSIVE,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size

    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_1900
 * @tc.desc      : extinfo flag is DL_EXT_RESERVED_ADDRESS_HINT | DL_EXT_RESERVED_ADDRESS_RECURSIVE, relro_fd is valid,
 *                 reserved address size is enough, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_1900(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    dl_extinfo extinfo = {.flag = DL_EXT_RESERVED_ADDRESS_HINT | DL_EXT_RESERVED_ADDRESS_RECURSIVE,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size};
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_2000
 * @tc.desc      : extinfo flag is DL_EXT_RESERVED_ADDRESS, relro_fd is valid, reserved address size is not enough
 *                 call dlopen_ext, return handle is NULL.
 * @tc.level     : Level 2
 */
static void dlopen_ext_relro_2000(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = MIN_RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    dl_extinfo extinfo = {
        .flag = DL_EXT_RESERVED_ADDRESS,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTREQ(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_2100
 * @tc.desc      : extinfo flag is DL_EXT_RESERVED_ADDRESS_HINT, relro_fd is valid, reserved address size is not enough
 *                 call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_2100(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = MIN_RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    dl_extinfo extinfo = {
        .flag = DL_EXT_RESERVED_ADDRESS_HINT,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_2300
 * @tc.desc      : child process extinfo flag is DL_EXT_WRITE_RELRO | DL_EXT_RESERVED_ADDRESS,
 *                 parent process extinfo flag is DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS,
 *                 relro_fd is valid, reserved address size is enough, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_2200(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    test_write_relro_file_ext(LIB_NAME, relro_fd, DL_EXT_WRITE_RELRO | DL_EXT_RESERVED_ADDRESS, map, map_size);
    dl_extinfo extinfo = {
        .flag = DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_2300
 * @tc.desc      : child process extinfo flag is DL_EXT_WRITE_RELRO | DL_EXT_RESERVED_ADDRESS_HINT,
 *                 parent process extinfo flag is DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS_HINT,
 *                 relro_fd is valid, reserved address size is enough, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_2300(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    test_write_relro_file_ext(LIB_NAME, relro_fd, DL_EXT_WRITE_RELRO | DL_EXT_RESERVED_ADDRESS_HINT, map, map_size);
    dl_extinfo extinfo = {
        .flag = DL_EXT_USE_RELRO | DL_EXT_RESERVED_ADDRESS_HINT,
        .relro_fd = relro_fd,
        .reserved_addr = map,
        .reserved_size = map_size,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

/**
 * @tc.name      : dlopen_ext_relro_2400
 * @tc.desc      : extinfo flag is DL_EXT_RESERVED_ADDRESS, relro_fd is valid, reserved addresses are not page aligned,
 *                 reserved address size is enough, call dlopen_ext, return handle is not NULL.
 * @tc.level     : Level 1
 */
static void dlopen_ext_relro_2400(void)
{
    char relro_file[] = RELRO_FILE_PATH;
    if (create_temp_relro_file(relro_file) < 0) {
        return;
    }
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    if (relro_fd < 0) {
        t_error("%s relro file %s open failed error is : %s \n", __FUNCTION__, relro_file, dlerror());
        return;
    }
    size_t map_size = RESERVED_ADDRESS_SIZE;
    void *map = mmap(NULL, map_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        t_error("%s map reserved address failed.\n", __FUNCTION__);
        tear_down(relro_fd, relro_file);
        return;
    }
    void *start_addr = (uint8_t *)map + 0x1;
    dl_extinfo extinfo = {
        .flag = DL_EXT_RESERVED_ADDRESS,
        .relro_fd = relro_fd,
        .reserved_addr = start_addr,
        .reserved_size = map_size,
    };
    void *handle = dlopen_ext(LIB_NAME, RTLD_NOW, &extinfo);
    EXPECT_PTRNE(__FUNCTION__, handle, NULL);
    tear_down(relro_fd, relro_file);
    clear_handle(handle);
    munmap(map, map_size);
}

TEST_FUN g_fun_array[] = {
    dlopen_ext_relro_0100,
    dlopen_ext_relro_0200,
    dlopen_ext_relro_0300,
    dlopen_ext_relro_0400,
    dlopen_ext_relro_0500,
    dlopen_ext_relro_0600,
    dlopen_ext_relro_0700,
    dlopen_ext_relro_0800,
    dlopen_ext_relro_0900,
    dlopen_ext_relro_1000,
    dlopen_ext_relro_1100,
    dlopen_ext_relro_1200,
    dlopen_ext_relro_1500,
    dlopen_ext_relro_1600,
    dlopen_ext_relro_1700,
    dlopen_ext_relro_1800,
    dlopen_ext_relro_1900,
    dlopen_ext_relro_2000,
    dlopen_ext_relro_2100,
    dlopen_ext_relro_2200,
    dlopen_ext_relro_2300,
    dlopen_ext_relro_2400,
};

int main(int argc, char *argv[])
{
    int num = sizeof(g_fun_array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        g_fun_array[pos]();
    }
    return t_status;
}