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

#include "grp.h"
#include "pwd.h"
#include "util.h"
#if defined __APPLE__
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace std;

constexpr int NGROUPS = 64;

static void Bm_function_Getgrouplist(benchmark::State &state)
{
    const char *user = "root";
    struct passwd *pw = getpwnam(user);
    if (pw == nullptr) {
        perror("getpwnam");
    }

#if defined __APPLE__
    int groups[NGROUPS];
#else
    gid_t groups[NGROUPS];
#endif
    for (auto _ : state) {
        int ngroups = NGROUPS;
        benchmark::DoNotOptimize(getgrouplist(user, pw->pw_gid, groups, &ngroups));
    }
}

MUSL_BENCHMARK(Bm_function_Getgrouplist);
