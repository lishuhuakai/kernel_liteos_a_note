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

#include <sys/eventfd.h>
#include "functionalext.h"

/**
 * @tc.name      : eventfd_0100
 * @tc.desc      : The flags parameter is equal to EFD_SEMAPHORE, which creates an eventfd object.
 * @tc.level     : Level 0
 */
void eventfd_0100(void)
{
    int ret = eventfd(0, EFD_SEMAPHORE);
    EXPECT_TRUE("eventfd_0100", ret > 0);
}

/**
 * @tc.name      : eventfd_0200
 * @tc.desc      : The flags parameter is equal to EFD_CLOEXEC, which creates an eventfd object.
 * @tc.level     : Level 1
 */
void eventfd_0200(void)
{
    int ret = eventfd(0, EFD_CLOEXEC);
    EXPECT_TRUE("eventfd_0200", ret > 0);
}

/**
 * @tc.name      : eventfd_0300
 * @tc.desc      : The flags parameter is equal to EFD_NONBLOCK, which creates an eventfd object.
 * @tc.level     : Level 1
 */
void eventfd_0300(void)
{
    int ret = eventfd(0, EFD_NONBLOCK);
    EXPECT_TRUE("eventfd_0300", ret > 0);
}

int main(int argc, char *argv[])
{
    eventfd_0100();
    eventfd_0200();
    eventfd_0300();
    return t_status;
}