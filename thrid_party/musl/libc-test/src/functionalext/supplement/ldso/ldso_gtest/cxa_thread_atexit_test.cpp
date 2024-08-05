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

#include <dlfcn.h>
#include <gtest/gtest.h>
#include <iostream>
#include <sys/mman.h>
#include <thread>

#include "libs/ldso_gtest_util.h"

using namespace testing::ext;
using namespace std;

static int num_for_child_thread = 0;
typedef void(*FuncPtr)(void*);

class CxaThreadAtexitTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

class ClassForChildThread {
public:
    ClassForChildThread()
    {
        cout << "Running ClassForChildThread()... \n";
        num_for_child_thread += 2;
    }

    void TestForChildThread() 
    {
        cout << "Running TestForChildThread... \n";
    }

    ~ClassForChildThread()
    {
        cout << "Running ~ClassForChildThread()... \n";
        num_for_child_thread -= 1;
    }
};

static thread_local ClassForChildThread thread_local_obj_for_child_thread;

void* ChildThreadFunc(void *arg)
{
    thread_local_obj_for_child_thread.TestForChildThread();
    return nullptr;
}

/**
 * @tc.name: cxa_thread_atexit_1
 * @tc.desc: Test dtor of thread_local object is called when child thread exit.
 * @tc.type: FUNC
 */
HWTEST_F(CxaThreadAtexitTest, cxa_thread_atexit_1, TestSize.Level1)
{
    pthread_t thr1, thr2;
    ASSERT_EQ(0, pthread_create(&thr1, nullptr, ChildThreadFunc, nullptr));
    ASSERT_EQ(0, pthread_join(thr1, nullptr));
    ASSERT_EQ(1, num_for_child_thread);

    ASSERT_EQ(0, pthread_create(&thr2, nullptr, ChildThreadFunc, nullptr));
    ASSERT_EQ(0, pthread_join(thr2, nullptr));
    ASSERT_EQ(2, num_for_child_thread);
}

class ClassForMainThread {
public:
    ClassForMainThread()
    {
        cout << "Running ClassForMainThread()... \n";
    }

    void TestForMainThread()
    {
        cout << "Running TestForMainThread... \n";
    }

    ~ClassForMainThread()
    {
        cout << "Running ~ClassForMainThread()... \n";
        fprintf(stderr, "%s", "main thread");  // Used for ASSERT_EXIT.
    }
};

static thread_local ClassForMainThread thread_local_obj_for_main_thread;

void* MainThreadFunc(void *arg)
{
    thread_local_obj_for_main_thread.TestForMainThread();
    exit(0);
}

/**
 * @tc.name: cxa_thread_atexit_2
 * @tc.desc: Test dtor of thread_local object is called when main thread exit.
 * @tc.type: FUNC
 */
HWTEST_F(CxaThreadAtexitTest, cxa_thread_atexit_2, TestSize.Level1)
{
    ASSERT_EXIT(MainThreadFunc(nullptr), testing::ExitedWithCode(0), "main thread");
}

extern "C" int __cxa_thread_atexit_impl(void (*func)(void*), void *arg, void *dso_handle);

static int used_for_multiple_dtors = 0;

void dtor1(void *arg)
{
    used_for_multiple_dtors += 1;
}

void dtor2(void *arg)
{
    used_for_multiple_dtors += 1;
}

void* TestForMultipleDtors(void *arg)
{
    __cxa_thread_atexit_impl(dtor1, arg, nullptr);
    __cxa_thread_atexit_impl(dtor2, arg, nullptr);
    return nullptr;
}

/**
 * @tc.name: cxa_thread_atexit_3
 * @tc.desc: Test can add multiple destructors.
 * @tc.type: FUNC
 */
HWTEST_F(CxaThreadAtexitTest, cxa_thread_atexit_3, TestSize.Level1)
{
    pthread_t thr;
    ASSERT_EQ(0, pthread_create(&thr, nullptr, TestForMultipleDtors, nullptr));
    ASSERT_EQ(0, pthread_join(thr, nullptr));
    ASSERT_EQ(used_for_multiple_dtors, 2);
}

/**
 * @tc.name: cxa_thread_atexit_4
 * @tc.desc: Test that we can't dlclose so when other threads have the corresponding destructor.
 * @tc.type: FUNC
 */
HWTEST_F(CxaThreadAtexitTest, cxa_thread_atexit_4, TestSize.Level1)
{

    FuncPtr fn = nullptr;
    void* handle = dlopen("libldso_cxa_thread_atexit_test.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    fn= reinterpret_cast<FuncPtr>(dlsym(handle, "ThreadFunc"));
    ASSERT_NE(fn, nullptr);
    fn(nullptr); // 
    dlclose(handle);

	// lib should still exist in memory.
	handle = dlopen("libldso_cxa_thread_atexit_test.so", RTLD_NOLOAD);
    ASSERT_NE(fn, nullptr);
}

/*
 * A --> B --> C
 * Test we can't unload B and C if we already add a thread_local dtor for B after dlopen A.
 * Test we can unload B and C when thread exit.
 */
void TestDlcloseSo(void)
{
    FuncPtr fn = nullptr;
    void* handle = dlopen("libldso_cxa_thread_atexit_test_A.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    fn= reinterpret_cast<FuncPtr>(dlsym(handle, "ThreadFunc"));
    ASSERT_NE(fn, nullptr);
    fn(nullptr);
    dlclose(handle);

    // This lib shouldn't exist in memory.
	handle = dlopen("libldso_cxa_thread_atexit_test_A.so", RTLD_NOLOAD);
    ASSERT_EQ(handle, nullptr);

    // These libs should still exist in memory.
	handle = dlopen("libldso_cxa_thread_atexit_test_B.so", RTLD_NOLOAD);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);
	handle = dlopen("libldso_cxa_thread_atexit_test_C.so", RTLD_NOLOAD);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);
    return;
}
void* TestDlcloseSoStub(void *arg)
{
    TestDlcloseSo();
    return nullptr;
}

/**
 * @tc.name: cxa_thread_atexit_5
 * @tc.desc: 
 * - Test that we can't dlclose so when other threads have the corresponding destructor.
 * - Test that we can unload so when no other threads have the corresponding destructor.
 * @tc.type: FUNC
 */
HWTEST_F(CxaThreadAtexitTest, cxa_thread_atexit_5, TestSize.Level1)
{
    void* handle;
    pthread_t thr;
    ASSERT_EQ(0, pthread_create(&thr, nullptr, TestDlcloseSoStub, nullptr));
    ASSERT_EQ(0, pthread_join(thr, nullptr));

	// They should have been unloaded when the thread exited.
	handle = dlopen("libldso_cxa_thread_atexit_test_B.so", RTLD_NOLOAD);
    ASSERT_EQ(handle, nullptr);
	handle = dlopen("libldso_cxa_thread_atexit_test_C.so", RTLD_NOLOAD);
    ASSERT_EQ(handle, nullptr);
}