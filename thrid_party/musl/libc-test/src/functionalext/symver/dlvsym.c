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

#include "dso_symver.h"

/**
 * @tc.name      : dlvsym_no_symver_0100
 * @tc.desc      : invoke a symbol with an invalid version
 * @tc.level     : Level 2
 */
void dlvsym_no_symver_0100(void)
{
    symver_log("start");

    void *handle = dlopen(dso_no_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_no_symver_symbol, dso_version_invalid);
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
 * @tc.name      : dlvsym_no_symver_0200
 * @tc.desc      : invoke an invalid symbol with an invalid version
 * @tc.level     : Level 2
 */
void dlvsym_no_symver_0200(void)
{
    symver_log("start");

    void *handle = dlopen(dso_no_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_symbol_invalid, dso_version_invalid);
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
 * @tc.name      : dlvsym_easy_symver_0100
 * @tc.desc      : invoke a symbol with the default version
 * @tc.level     : Level 0
 */
void dlvsym_easy_symver_0100(void)
{
    symver_log("start");

    void *handle = dlopen(dso_easy_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_easy_symver_symbol, dso_easy_symver_version_stable);
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
 * @tc.name      : dlvsym_easy_symver_0200
 * @tc.desc      : invoke a symbol with a non-default version
 * @tc.level     : Level 1
 */
void dlvsym_easy_symver_0200(void)
{
    symver_log("start");

    void *handle = dlopen(dso_easy_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_easy_symver_symbol, dso_easy_symver_version_old);
    const char *error = dlerror();
    if (error != NULL) {
        symver_error("%s", error);
        return;
    }

    const char *result = func();
    symver_streq(result, dso_easy_symver_symbol_old);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlvsym_easy_symver_0300
 * @tc.desc      : invoke an invalid symbol with an invalid version
 * @tc.level     : Level 2
 */
void dlvsym_easy_symver_0300(void)
{
    symver_log("start");

    void *handle = dlopen(dso_easy_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_symbol_invalid, dso_version_invalid);
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
 * @tc.name      : dlvsym_easy_symver_0400
 * @tc.desc      : invoke a symbol with an invalid version
 * @tc.level     : Level 2
 */
void dlvsym_easy_symver_0400(void)
{
    symver_log("start");

    void *handle = dlopen(dso_easy_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_easy_symver_symbol, dso_version_invalid);
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
 * @tc.name      : dlvsym_hard_symver_0100
 * @tc.desc      : invoke a symbol with the default version
 * @tc.level     : Level 0
 */
void dlvsym_hard_symver_0100(void)
{
    symver_log("start");

    void *handle = dlopen(dso_hard_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_hard_symver_if_symbol, dso_hard_symver_version_stable);
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
 * @tc.name      : dlvsym_hard_symver_0200
 * @tc.desc      : invoke a symbol with a non-default version
 * @tc.level     : Level 1
 */
void dlvsym_hard_symver_0200(void)
{
    symver_log("start");

    void *handle = dlopen(dso_hard_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_hard_symver_if_symbol, dso_hard_symver_version_old);
    const char *error = dlerror();
    if (error != NULL) {
        symver_error("%s", error);
        return;
    }

    const char *result = func();
    symver_streq(result, dso_hard_symver_if_symbol_old);

    dlclose(handle);

    symver_log("end");
}

/**
 * @tc.name      : dlvsym_hard_symver_0300
 * @tc.desc      : invoke an invalid symbol with an invalid version
 * @tc.level     : Level 2
 */
void dlvsym_hard_symver_0300(void)
{
    symver_log("start");

    void *handle = dlopen(dso_hard_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_symbol_invalid, dso_version_invalid);
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
 * @tc.name      : dlvsym_hard_symver_0400
 * @tc.desc      : invoke a symbol with an invalid version
 * @tc.level     : Level 2
 */
void dlvsym_hard_symver_0400(void)
{
    symver_log("start");

    void *handle = dlopen(dso_hard_symver_name, RTLD_LAZY);
    if (!handle) {
        symver_error("%s", dlerror());
        return;
    }

    // Clear any existing error
    dlerror();

    functype func = (functype)dlvsym(handle, dso_hard_symver_if_symbol, dso_version_invalid);
    const char *error = dlerror();
    if (error == NULL) {
        symver_error();
        return;
    }

    symver_log("error = %s", error);

    dlclose(handle);

    symver_log("end");
}

int main(int argc, char *argv[])
{
    symver_log("start");

    dlvsym_no_symver_0100();
    dlvsym_no_symver_0200();

    dlvsym_easy_symver_0100();
    dlvsym_easy_symver_0200();
    dlvsym_easy_symver_0300();
    dlvsym_easy_symver_0400();

    dlvsym_hard_symver_0100();
    dlvsym_hard_symver_0200();
    dlvsym_hard_symver_0300();
    dlvsym_hard_symver_0400();

    symver_log("t_status = %d", t_status);
    symver_log("end");

    return t_status;
}
