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

#include <ctype.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

/*
 * @tc.name      : __sched_cpucount_0100
 * @tc.desc      : Each parameter value is valid, and the number of CPUs included in the collection can be obtained.
 * @tc.level     : Level 0
 */
void __sched_cpucount_0100(void)
{
    int ret = 0;
    cpu_set_t *cpusetp;
    size_t size;
    int num_cpus;
    num_cpus = sysconf(_SC_NPROCESSORS_CONF);
    cpusetp = CPU_ALLOC(num_cpus);
    EXPECT_PTRNE("__sched_cpucount_0100", cpusetp, NULL);

    size = CPU_ALLOC_SIZE(num_cpus);
    CPU_ZERO_S(size, cpusetp);
    for (int cpu = 0; cpu < num_cpus; cpu++) {
        CPU_SET_S(cpu, size, cpusetp);
    }
    ret = __sched_cpucount(size, cpusetp);
    EXPECT_EQ("__sched_cpucount_0100", ret, num_cpus);
    CPU_FREE(cpusetp);
}

/*
 * @tc.name      : __sched_cpucount_0200
 * @tc.desc      : The size parameter is invalid (0), the set parameter is valid, and the number of CPUs
 *                 contained in the set cannot be obtained.
 * @tc.level     : Level 2
 */
void __sched_cpucount_0200(void)
{
    int ret = 0;
    cpu_set_t *cpusetp;
    size_t size;
    int num_cpus;
    num_cpus = sysconf(_SC_NPROCESSORS_CONF);
    cpusetp = CPU_ALLOC(num_cpus);
    EXPECT_PTRNE("__sched_cpucount_0200", cpusetp, NULL);

    size = CPU_ALLOC_SIZE(num_cpus);
    CPU_ZERO_S(size, cpusetp);
    for (int cpu = 0; cpu < num_cpus; cpu++) {
        CPU_SET_S(cpu, size, cpusetp);
    }
    ret = __sched_cpucount(0, cpusetp);
    EXPECT_EQ("__sched_cpucount_0200", ret, 0);
    CPU_FREE(cpusetp);
}

/*
 * @tc.name      : __sched_cpucount_0300
 * @tc.desc      : The size parameter is valid, the set parameter is invalid (null), and the number of CPUs
 *                 contained in the set cannot be obtained.
 * @tc.level     : Level 2
 */
void __sched_cpucount_0300(void)
{
    int ret = 0;
    cpu_set_t *cpusetp;
    size_t size;
    int num_cpus;
    num_cpus = sysconf(_SC_NPROCESSORS_CONF);
    cpusetp = CPU_ALLOC(num_cpus);
    EXPECT_PTRNE("__sched_cpucount_0300", cpusetp, NULL);

    size = CPU_ALLOC_SIZE(num_cpus);
    CPU_ZERO_S(size, cpusetp);
    ret = __sched_cpucount(size, cpusetp);
    EXPECT_EQ("__sched_cpucount_0300", ret, 0);
    CPU_FREE(cpusetp);
}

int main(int argc, char *argv[])
{
    __sched_cpucount_0100();
    __sched_cpucount_0200();
    __sched_cpucount_0300();
    return t_status;
}