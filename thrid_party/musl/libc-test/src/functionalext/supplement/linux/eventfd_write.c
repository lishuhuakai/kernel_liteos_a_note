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
 * @tc.name      : eventfd_write_0100
 * @tc.desc      : Write the value through this function
 * @tc.level     : Level 0
 */
void eventfd_write_0100(void)
{
    eventfd_t value;
    unsigned int initial_value = 0;
    int fd = eventfd(initial_value, O_NONBLOCK);
    if (fd == -1) {
        t_error("%s eventfd failed\n", __func__);
    }

    int ret = eventfd_write(fd, 1);
    if (ret != 0) {
        t_error("%s eventfd_write failed\n", __func__);
    }
    ret = eventfd_write(fd, 1);
    if (ret != 0) {
        t_error("%s eventfd_write failed\n", __func__);
    }
    ret = eventfd_write(fd, 1);
    if (ret != 0) {
        t_error("%s eventfd_write failed\n", __func__);
    }

    ret = eventfd_read(fd, &value);
    if (ret != 0) {
        t_error("%s eventfd_read failed\n", __func__);
    }
    if (value != 3U) {
        t_error("%s value is %d, not three\n", __func__);
    }
    close(fd);
}

/**
 * @tc.name      : eventfd_write_0200
 * @tc.desc      : When the file descriptor is invalid, test the return value of this function
 * @tc.level     : Level 2
 */
void eventfd_write_0200(void)
{
    int ret = eventfd_write(-1, 1);
    if (ret != -1) {
        t_error("%s eventfd_write should be failed\n", __func__);
    }
}

/**
 * @tc.name      : eventfd_write_0300
 * @tc.desc      : When the eventfd_t value is invalid, test the return value of this function
 * @tc.level     : Level 2
 */
void eventfd_write_0300(void)
{
    unsigned int initial_value = 2;
    int fd = eventfd(initial_value, O_NONBLOCK);
    if (fd == -1) {
        t_error("%s eventfd failed\n", __func__);
    }

    int result = eventfd_write(fd, -1);
    if (result != -1) {
        t_error("%s eventfd_write should be failed\n", __func__);
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    eventfd_write_0100();
    eventfd_write_0200();
    eventfd_write_0300();
    return t_status;
}