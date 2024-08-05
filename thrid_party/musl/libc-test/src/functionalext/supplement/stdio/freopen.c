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
#include "functionalext.h"
#include "filepath_util.h"

/**
 * @tc.name      : freopen_0100
 * @tc.desc      : Each parameter is valid, f is stdin, which can realize input redirection.
 * @tc.level     : Level 0
 */
void freopen_0100(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("freopen.txt", ptr);

    FILE *fp;
    fp = freopen(ptr, "w+", stdin);
    EXPECT_PTRNE("freopen_0100", fp, NULL);

    fclose(stdin);
    fclose(fp);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    freopen_0100();
    return t_status;
}