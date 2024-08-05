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
#include <stdlib.h>
#include <wchar.h>

#include "test.h"

#define SIZEOF_WCHAR(x) (sizeof(x) / sizeof(wchar_t))

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : wcsncpy_0100
 * @tc.desc      : copy a fixed-size string of wide characters
 * @tc.level     : Level 0
 */
void wcsncpy_0100(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t buf[SIZEOF_WCHAR(src) + 1];
    wmemset(buf, L'A', SIZEOF_WCHAR(buf));
    wchar_t *dest = (wchar_t *)buf;

    if (dest != wcsncpy(dest, src, SIZEOF_WCHAR(src))) {
        t_error("%s failed: wcsncpy. src = %ls, dest = %ls\n", __func__, src, dest);
    }
}

/**
 * @tc.name      : wcsncpy_0200
 * @tc.desc      : copy a 0-size string of wide characters
 * @tc.level     : Level 1
 */
void wcsncpy_0200(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t buf[SIZEOF_WCHAR(src)];
    wmemset(buf, L'A', SIZEOF_WCHAR(buf));
    wchar_t *dest = (wchar_t *)buf;

    if (dest != wcsncpy(dest, src, 0)) {
        t_error("%s failed: wcsncpy. src = %ls, dest = %ls\n", __func__, src, dest);
    }
}

/**
 * @tc.name      : wcsncpy_0300
 * @tc.desc      : copy a fixed-size string of wide characters with a larger size
 * @tc.level     : Level 1
 */
void wcsncpy_0300(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t buf[SIZEOF_WCHAR(src) + SIZEOF_WCHAR(src)];
    wmemset(buf, L'A', SIZEOF_WCHAR(buf));
    wchar_t *dest = (wchar_t *)buf;

    if (dest != wcsncpy(dest, src, SIZEOF_WCHAR(src) + SIZEOF_WCHAR(src) - 1)) {
        t_error("%s failed: wcsncpy. src = %ls, dest = %ls\n", __func__, src, dest);
    }

    int i = 0;
    for (i = SIZEOF_WCHAR(src) + 1; i < SIZEOF_WCHAR(buf) - 1; i++) {
        if (buf[i] != L'\0') {
            t_error("%s failed: buf[%d] = %lc\n", __func__, i, buf[i]);
        }
    }

    if (buf[i] != L'A') {
        t_error("%s failed: buf[%d] = %lc\n", __func__, i, buf[i]);
    }
}

/**
 * @tc.name      : wcsncpy_0400
 * @tc.desc      : copy a fixed-size string of wide characters to a NULL pointer
 * @tc.level     : Level 2
 */
void wcsncpy_0400(void)
{
    signal(SIGSEGV, handler);

    const wchar_t src[] = L"Hello";
    wcsncpy(NULL, src, SIZEOF_WCHAR(src));
}

int main(int argc, char *argv[])
{
    wcsncpy_0100();
    wcsncpy_0200();
    wcsncpy_0300();
    wcsncpy_0400();

    return t_status;
}
