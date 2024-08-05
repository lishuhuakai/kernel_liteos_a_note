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

#include <errno.h>
#include <grp.h>
#include <stdio.h>
#include <string.h>

#include "test.h"

/**
 * @tc.name      : setgrent_0100
 * @tc.desc      : rewinds to the beginning of the group database
 * @tc.level     : Level 0
 */
void setgrent_0100(void)
{
    errno = 0;
    struct group *result = getgrent();
    if (result == NULL) {
        t_error("%s failed: getgrent\n", __func__);
    }

    if (errno != 0) {
        t_error("%s failed: errno = %ld\n", __func__, errno);
    }

    char buf[BUFSIZ] = {0};
    strcpy(buf, result->gr_name);

    setgrent();

    result = getgrent();
    if (result == NULL) {
        t_error("%s failed: getgrent\n", __func__);
    }

    if (strcmp(result->gr_name, buf)) {
        t_error("%s failed: result->gr_name = %s\n", __func__, result->gr_name);
    }
}

int main(int argc, char *argv[])
{
    setgrent_0100();

    return t_status;
}
