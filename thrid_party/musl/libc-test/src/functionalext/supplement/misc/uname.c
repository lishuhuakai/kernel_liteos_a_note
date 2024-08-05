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
#include <string.h>
#include <sys/utsname.h>

#include "test.h"

/**
 * @tc.name      : uname_0100
 * @tc.desc      : get system infomation
 * @tc.level     : Level 0
 */
void uname_0100(void)
{
    struct utsname buf;
    int result = uname(&buf);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    if (strlen(buf.sysname) == 0) {
        t_error("%s failed: sysname\n", __func__, result);
    }

    if (strlen(buf.nodename) == 0) {
        t_error("%s failed: nodename\n", __func__);
    }

    if (strlen(buf.release) == 0) {
        t_error("%s failed: release\n", __func__);
    }

    if (strlen(buf.version) == 0) {
        t_error("%s failed: version\n", __func__);
    }

    if (strlen(buf.machine) == 0) {
        t_error("%s failed: machine\n", __func__);
    }

#ifdef _GNU_SOURCE
    if (strlen(buf.domainname) == 0) {
        t_error("%s failed: domainname\n", __func__);
    }
#else
    if (strlen(buf.__domainname) == 0) {
        t_error("%s failed: __domainname\n", __func__);
    }
#endif
}

/**
 * @tc.name      : uname_0200
 * @tc.desc      : get system infomation with NULL
 * @tc.level     : Level 2
 */
void uname_0200(void)
{
    int result = uname(NULL);
    if (result == 0) {
        t_error("%s failed\n", __func__);
        return;
    }

    if (errno != EFAULT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
        return;
    }
}

int main(int argc, char *argv[])
{
    uname_0100();
    uname_0200();

    return t_status;
}
