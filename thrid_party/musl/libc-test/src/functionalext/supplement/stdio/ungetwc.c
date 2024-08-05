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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "filepath_util.h"

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : ungetwc_0100
 * @tc.desc      : push back a wide character
 * @tc.level     : Level 0
 */
void ungetwc_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    FILE *file = fopen(path, "a+");
    if (!file) {
        t_error("%s failed: fopen\n", __func__);
        return;
    }

    wint_t wc = L'a';
    wint_t result = ungetwc(wc, file);
    if (result != wc) {
        remove(path);

        t_error("%s failed: ungetwc\n", __func__);
        return;
    }

    result = getwc(file);
    if (result != wc) {
        remove(path);

        t_error("%s failed: getwc\n", __func__);
        return;
    }
}

/**
 * @tc.name      : ungetwc_0200
 * @tc.desc      : push back a wide character with an invalid FILE stream
 * @tc.level     : Level 2
 */
void ungetwc_0200(void)
{
    signal(SIGSEGV, handler);

    wint_t wc = L'a';
    ungetwc(wc, NULL);
}

int main(int argc, char *argv[])
{
    ungetwc_0100();
    ungetwc_0200();

    return t_status;
}
