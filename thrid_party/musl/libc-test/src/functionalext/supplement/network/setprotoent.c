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

#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include "test.h"

/**
 * @tc.name      : setprotoent_0100
 * @tc.desc      : set protocol entry
 * @tc.level     : Level 0
 */
void setprotoent_0100(void)
{
    endprotoent();

    struct protoent *result = getprotoent();
    if (result == NULL) {
        t_error("%s failed: result = %p\n", __func__, result);
    }

    char buf[BUFSIZ] = {0};
    strcpy(buf, result->p_name);

    setprotoent(0);
    result = getprotoent();
    if (result == NULL) {
        t_error("%s failed: result = %p\n", __func__, result);
    }

    if (strcmp(result->p_name, buf)) {
        t_error("%s failed: result->p_name = %s\n", __func__, result->p_name);
    }
}

int main(int argc, char *argv[])
{
    setprotoent_0100();

    return t_status;
}
