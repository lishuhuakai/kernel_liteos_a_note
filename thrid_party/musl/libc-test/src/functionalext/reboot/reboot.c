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

#include <sys/reboot.h>
#include "test.h"

#define TEST(c, ...) ((c) || (t_error(#c " failed: " __VA_ARGS__),0))

#define	LINUX_REBOOT_CMD_RESTART	0x01234567
#define	LINUX_REBOOT_CMD_CAD_ON		0x89ABCDEF
#define	LINUX_REBOOT_CMD_CAD_OFF	0x00000000

/**
 * @tc.name     : reboot_0010
 * @tc.desc     : test reboot with flag LINUX_REBOOT_CMD_CAD_ON
 * @tc.level    : Level 0
 */
static void reboot_0010(void)
{
    TEST(reboot(LINUX_REBOOT_CMD_CAD_ON) == 0);
}

/**
 * @tc.name     : reboot_0020
 * @tc.desc     : test reboot with flag LINUX_REBOOT_CMD_CAD_OFF
 * @tc.level    : Level 0
 */
static void reboot_0020(void)
{
    TEST(reboot(LINUX_REBOOT_CMD_CAD_OFF) == 0);
}

int main(void)
{
    geteuid();

    reboot_0010();
    reboot_0020();

	return t_status;
}
