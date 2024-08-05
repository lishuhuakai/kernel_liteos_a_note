/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
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

#include <sys/ioctl.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "util.h"

#define DEVICE_FILE "/dev/tty"

// Used to perform control operations on device files
// set baud rate
static void Bm_function_Ioctl_baudrate(benchmark::State &state)
{
    struct termios ttydev;
    int ret;
    int fd = open(DEVICE_FILE, O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open ioctl");
    }
    ret = tcgetattr(fd, &ttydev);
    if (ret == -1) {
        perror("Property fetch failed");
    }
    int speed = cfgetospeed(&ttydev);
    cfsetospeed(&ttydev, B19200);
    cfsetispeed(&ttydev, B19200);
    for (auto _ : state) {
        ret = ioctl(fd, TCSETS, &ttydev);
        if (ret < 0) {
            perror("ioctl");
        }
        benchmark::DoNotOptimize(ret);
    }
    cfsetospeed(&ttydev, speed);
    cfsetispeed(&ttydev, speed);
    ret = ioctl(fd, TCSETS, &ttydev);
    if (ret == -1) {
        perror("Failed to restore baud rate");
    }
    close(fd);
}

MUSL_BENCHMARK(Bm_function_Ioctl_baudrate);
