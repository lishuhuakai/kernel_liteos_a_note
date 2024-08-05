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

#include <unistd.h>
#include <sys/quota.h>
#include "functionalext.h"

/**
 * @tc.name      : quotactl_0100
 * @tc.desc      : Since there is no kernel with CONFIG_QUOTA enabled, the abnormal
 *                 return value of the judgment function
 * @tc.level     : Level 2
 */
void quotactl_0100(void)
{
    int cmd = QCMD(Q_GETQUOTA, USRQUOTA);
    struct dqblk strinfo;
    int ret = quotactl(cmd, "/dev", getuid(), (char *)(&strinfo));

    EXPECT_EQ("quotactl_0100", ret, ERREXPECT);
}

int main(void)
{
    quotactl_0100();
    return t_status;
}