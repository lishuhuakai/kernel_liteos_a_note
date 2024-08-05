/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <resolv.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

const int op = QUERY;
const int class = C_IN;
const int type = T_TXT;
const char dname[] = "www.example.com";
const unsigned char *data = (const unsigned char *)"";
const char qbuf[] = "\0\1\0\0\0\0\0\0";
const char qname[] = "\3www\7example\3com";

/**
 * @tc.name      : res_send_0100
 * @tc.desc      : Send a preformatted query given in msg of length msglen
 * @tc.level     : Level 0
 */
void res_send_0100(void)
{
    unsigned char buf[BUFSIZ] = {0};
    unsigned char answer[512];
    int ret = res_mkquery(op, dname, class, type, data, 0, NULL, buf, sizeof(buf));
    if (ret < 0) {
        t_error("%s failed: ret = %d\n", __func__, ret);
    }

    int result = res_send(buf, ret, answer, sizeof(answer));
    if (result == -1) {
        t_error("%s res_send failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    res_send_0100();
    return t_status;
}