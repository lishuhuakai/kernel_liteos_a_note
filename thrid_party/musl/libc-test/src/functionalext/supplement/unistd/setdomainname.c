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
#include <string.h>
#include <unistd.h>

#include "test.h"

const char name[] = "www.example.com";

/**
 * @tc.name      : setdomainname_0100
 * @tc.desc      : set NIS domain name
 * @tc.level     : Level 0
 */
void setdomainname_0100(void)
{
    // store current NIS domain name
    char cbuf[BUFSIZ] = {0};
    int result = getdomainname(cbuf, sizeof(cbuf));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    result = setdomainname(name, strlen(name));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    char buf[BUFSIZ] = {0};
    result = getdomainname(buf, sizeof(buf));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (strcmp(buf, name)) {
        t_error("%s failed: buf = %s\n", __func__, buf);
    }

    // restore NIS domain name
    result = setdomainname(cbuf, strlen(cbuf));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }
}

/**
 * @tc.name      : setdomainname_0200
 * @tc.desc      : set an empty NIS domain name
 * @tc.level     : Level 1
 */
void setdomainname_0200(void)
{
    // store current NIS domain name
    char cbuf[BUFSIZ] = {0};
    int result = getdomainname(cbuf, sizeof(cbuf));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    result = setdomainname(NULL, 0);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    char buf[BUFSIZ] = {0};
    result = getdomainname(buf, sizeof(buf));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (strlen(buf) != 0) {
        t_error("%s failed: buf = %s\n", __func__, buf);
    }

    // restore NIS domain name
    result = setdomainname(cbuf, strlen(cbuf));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }
}

/**
 * @tc.name      : setdomainname_0300
 * @tc.desc      : set NIS domain name with an invalid length
 * @tc.level     : Level 2
 */
void setdomainname_0300(void)
{
    int result = setdomainname(NULL, -1);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    setdomainname_0100();
    setdomainname_0200();
    setdomainname_0300();

    return t_status;
}
