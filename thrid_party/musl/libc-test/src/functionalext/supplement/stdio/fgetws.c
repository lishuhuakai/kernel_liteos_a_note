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

#include <fcntl.h>
#include <wchar.h>
#include "functionalext.h"

/**
 * @tc.name      : fgetws_0100
 * @tc.desc      : Verify that a string of wide characters can be read from the specified file
 * @tc.level     : Level 0
 */
void fgetws_0100()
{
    wchar_t mystring[100];
    char str[] = "test";

    FILE *fp = fopen("/data/test.txt", "w+");
    EXPECT_PTRNE("fgetws_0100", fp, NULL);

    size_t fw = fwrite(str, sizeof(char), sizeof(str), fp);
    EXPECT_TRUE("fgetws_0100", fw > 0);
    fseek(fp, 0, SEEK_SET);

    wchar_t *ret = fgetws(mystring, 100, fp);
    EXPECT_PTRNE("fgetws_0100", ret, NULL);

    if (wcscmp(mystring, L"test")) {
        t_error("%s str invalid\n", __func__);
    }
    fclose(fp);
}

/**
 * @tc.name      : fgetws_0200
 * @tc.desc      : Verify that a string of wide characters cannot be read from the specified file
 * @tc.level     : Level 2
 */
void fgetws_0200()
{
    wchar_t wrstring[100];
    wchar_t line[] = L"hello";
    const char *ptr = "/data/test.txt";

    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fgetws_0100", fptr, NULL);

    fwrite(line, sizeof(line) / sizeof(wchar_t), sizeof(*line), fptr);
    fflush(fptr);

    wchar_t *ch = fgetws(wrstring, EINVAL, fptr);
    EXPECT_PTREQ("fgetws_0200", ch, NULL);

    fclose(fptr);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    fgetws_0100();
    fgetws_0200();
    return t_status;
}