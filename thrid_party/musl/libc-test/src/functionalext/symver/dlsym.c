/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>

#include "dso_easy_symver.h"
#include "dso_hard_symver.h"
#include "dso_no_symver.h"
#include "dso_symver.h"

extern void  *__dlsym_time64(void *__restrict, const char *__restrict);

/**
 * @tc.name      : dlsym_no_symver_0100
 * @tc.desc      : invoke a symbol programmatically
 * @tc.level     : Level 0
 */
void dlsym_no_symver_0100(void)
{
    symver_log("start");

    void *handle = dlopen(dso_no_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlsym(handle, dso_no_symver_symbol);
    const char *error = dlerror();
    if (error != NULL) {
        symver_error("%s", error);
        return;
    }

    const char *result = func();
    symver_streq(result, dso_no_symver_symbol);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_no_symver_0200
 * @tc.desc      : invoke an invalid symbol programmatically
 * @tc.level     : Level 2
 */
void dlsym_no_symver_0200(void)
{
    symver_log("start");

    void *handle = dlopen(dso_no_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlsym(handle, dso_symbol_invalid);
    const char *error = dlerror();
    if (error == NULL) {
        symver_error();
        return;
    }

    symver_log("error = %s", error);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_easy_symver_0100
 * @tc.desc      : invoke a symbol directly after specifying a non-default version
 * @tc.level     : Level 0
 */
void dlsym_easy_symver_0100(void)
{
    symver_log("start");

    __asm__(".symver dso_easy_symver, dso_easy_symver@OLD");
    const char *result = dso_easy_symver();
    symver_streq(result, dso_easy_symver_symbol_old);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_easy_symver_0200
 * @tc.desc      : invoke a symbol with versions programmatically
 * @tc.level     : Level 1
 */
void dlsym_easy_symver_0200(void)
{
    symver_log("start");

    void *handle = dlopen(dso_easy_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlsym(handle, dso_easy_symver_symbol);
    const char *error = dlerror();
    if (error != NULL) {
        symver_error("%s", error);
        return;
    }

    const char *result = func();
    symver_streq(result, dso_easy_symver_symbol_stable);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_easy_symver_0300
 * @tc.desc      : invoke an invalid symbol programmatically
 * @tc.level     : Level 2
 */
void dlsym_easy_symver_0300(void)
{
    symver_log("start");

    void *handle = dlopen(dso_easy_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlsym(handle, dso_symbol_invalid);
    const char *error = dlerror();
    if (error == NULL) {
        symver_error();
        return;
    }

    symver_log("error = %s", error);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_hard_symver_0100
 * @tc.desc      : invoke a symbol directly after specifying a non-default version
 * @tc.level     : Level 0
 */
void dlsym_hard_symver_0100(void)
{
    symver_log("start");

    __asm__(".symver dso_hard_symver_ld, dso_hard_symver_ld@OLD");
    const char *result = dso_hard_symver_ld();
    symver_streq(result, dso_hard_symver_ld_symbol_old);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_hard_symver_0200
 * @tc.desc      : invoke a symbol with versions programmatically
 * @tc.level     : Level 1
 */
void dlsym_hard_symver_0200(void)
{
    symver_log("start");

    void *handle = dlopen(dso_hard_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlsym(handle, dso_hard_symver_if_symbol);
    const char *error = dlerror();
    if (error != NULL) {
        symver_error("%s", error);
        return;
    }

    const char *result = func();
    symver_streq(result, dso_hard_symver_if_symbol_stable);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_hard_symver_0300
 * @tc.desc      : invoke an invalid symbol programmatically
 * @tc.level     : Level 2
 */
void dlsym_hard_symver_0300(void)
{
    symver_log("start");

    void *handle = dlopen(dso_hard_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlsym(handle, dso_symbol_invalid);
    const char *error = dlerror();
    if (error == NULL) {
        symver_error();
        return;
    }

    symver_log("error = %s", error);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlsym_time64_no_symver_0100
 * @tc.desc      : invoke a symbol programmatically
 * @tc.level     : Level 0
 */
void dlsym_time64_no_symver_0100(void)
{
    symver_log("start");

    void *handle = dlopen(dso_no_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)__dlsym_time64(handle, dso_no_symver_symbol);
    const char *error = dlerror();
    if (error != NULL) {
        symver_error("%s", error);
        return;
    }

    const char *result = func();
    symver_streq(result, dso_no_symver_symbol);

    dlclose(handle);

    symver_log("end");
}

int main(int argc, char *argv[])
{
    symver_log("start");

    dlsym_no_symver_0100();
    dlsym_no_symver_0200();
    dlsym_time64_no_symver_0100();

    dlsym_easy_symver_0100();
    dlsym_easy_symver_0200();
    dlsym_easy_symver_0300();

    dlsym_hard_symver_0100();
    dlsym_hard_symver_0200();
    dlsym_hard_symver_0300();

    symver_log("t_status = %d", t_status);
    symver_log("end");

    return t_status;
}
