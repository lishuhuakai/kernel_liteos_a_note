/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

int cxa_thread_atexit_test_var = 0;

class A
{
public:
    A();
    ~A();
    void Test();
};

A::A()
{
    cxa_thread_atexit_test_var += 1;
}

A::~A()
{
    cxa_thread_atexit_test_var -= 1;
}

void A::Test()
{
    cxa_thread_atexit_test_var += 2;
}

#ifdef TEST_FOR_DLCLOSE
extern void HelloC();
#endif

static thread_local A obj;

extern "C" void ThreadFunc(void *arg)
{
    obj.Test();
#ifdef TEST_FOR_DLCLOSE
    HelloC();
#endif
}