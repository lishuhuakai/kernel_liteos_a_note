/**
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

#include <sys/stat.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>
#include "functionalext.h"

extern int __ppoll_time64(struct pollfd *, nfds_t, const struct timespec *, const sigset_t *);

/**
 * @tc.name      : ppoll_time64_0300
 * @tc.desc      : Listen for readable file event (illegal parameter)
 * @tc.level     : Level 2
 */
void ppoll_time64_0300(void)
{
    struct timespec timeout;
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    int ret = __ppoll_time64(NULL, 1, &timeout, NULL);
    EXPECT_EQ("ppoll_time64_0300", ret, ERREXPECT);
}

int main(void)
{
    ppoll_time64_0300();
    return t_status;
}