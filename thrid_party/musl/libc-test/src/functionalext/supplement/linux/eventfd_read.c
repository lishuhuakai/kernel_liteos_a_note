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

#include <fcntl.h>
#include <sys/eventfd.h>
#include "test.h"

/**
 * @tc.name      : eventfd_read_0100
 * @tc.desc      : Read the value of event through the file descriptor
 * @tc.level     : Level 0
 */
void eventfd_read_0100(void)
{
    unsigned int initial_value = 2;
    int fd = eventfd(initial_value, O_NONBLOCK);
    if (fd == -1) {
        t_error("%s eventfd failed\n", __func__);
    }

    eventfd_t value = 123;
    int result = eventfd_read(fd, &value);
    if (result != 0) {
        t_error("%s eventfd_read failed\n", __func__);
    }

    if (value != initial_value) {
        t_error("%s eventfd_read value invalid\n", __func__);
    }

    close(fd);
}

/**
 * @tc.name      : eventfd_read_0200
 * @tc.desc      : When the file descriptor is invalid, test the return value of this function
 * @tc.level     : Level 2
 */
void eventfd_read_0200(void)
{
    eventfd_t value = 123;
    int result = eventfd_read(-1, &value);
    if (result != -1) {
        t_error("%s eventfd_read should be failed\n", __func__);
    }
}

/**
 * @tc.name      : eventfd_read_0300
 * @tc.desc      : When the eventfd_t value is invalid, test the return value of this function
 * @tc.level     : Level 2
 */
void eventfd_read_0300(void)
{
    unsigned int initial_value = 2;
    int fd = eventfd(initial_value, O_NONBLOCK);
    if (fd == -1) {
        t_error("%s eventfd failed\n", __func__);
    }

    int result = eventfd_read(fd, NULL);
    if (result != -1) {
        t_error("%s eventfd_read should be failed\n", __func__);
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    eventfd_read_0100();
    eventfd_read_0200();
    eventfd_read_0300();
    return t_status;
}