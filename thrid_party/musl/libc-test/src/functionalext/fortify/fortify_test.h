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
#ifndef _FORTIFY_TEST_H
#define _FORTIFY_TEST_H
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include "test.h"

#define TEST(c, ...) ((c) || (t_error(#c " failed: \n" __VA_ARGS__),0))

void SignalHandler(int signum)
{
    kill(getpid(), SIGSTOP);
}

#endif