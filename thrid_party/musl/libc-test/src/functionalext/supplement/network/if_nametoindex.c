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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <net/if.h>
#include "test.h"

/**
 * @tc.name      : if_nametoindex_0100
 * @tc.desc      : Get index by network interface name
 * @tc.level     : Level 0
 */
void if_nametoindex_0100(void)
{
    int saved_errno = errno;
    char if_name[IFNAMSIZ] = {'\0'};
    unsigned int if_index = (unsigned int)atoi("1");

    char *name = if_indextoname(if_index, if_name);
    if (name == NULL && errno == ENXIO) {
        t_error("%s if_indextoname get name is NULL", __func__);
    }
    unsigned int result = if_nametoindex(name);
    if (result != 1) {
        t_error("%s if_nametoindex by name :'%s' get result is %d are not want 1\n", __func__, name, result);
    }
}

int main(int argc, char *argv[])
{
    if_nametoindex_0100();
    return t_status;
}