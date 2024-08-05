/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "cfi_util.h"

struct A {
    virtual void Test();
};

void A::Test()
{
    printf("A::Test()\n");
}

struct B : A {
    void Test() override;
};

void B::Test()
{
    printf("B::Test()\n");
}

struct C : A {
};

struct D {
    virtual void Test();
};

void D::Test()
{
    printf("D::Test()\n");
}

struct CallTestA {
    virtual void VcallFunc();
    void CallFunc();
};

void CallTestA::VcallFunc()
{
    printf("CallTestA::VcallFunc()\n");
}

void CallTestA::CallFunc()
{
    printf("CallTestA::CallFunc()\n");
}

struct CallTestB {
    virtual void VcallFunc();
    void CallFunc();
};

void CallTestB::VcallFunc()
{
    printf("CallTestB::VcallFunc()\n");
}

void CallTestB::CallFunc()
{
    printf("CallTestB::CallFunc()\n");
}

void CfiCastStrict()
{
    C *c = new C;
    A a;
    c = static_cast<C *>(&a);
}

void CfiDerivedCast()
{
    B *b = new B;
    A a;
    b = static_cast<B *>(&a);
}

void CfiUnrelatedCast()
{
    D *d = new D;
    A a;
    d = ((D *)&a);
}

void Icall()
{
    printf("Icall()\n");
}

void CfiIcall()
{
    ((void (*)(int))Icall)(42);
}

void CfiVcall()
{
    CallTestA *a;
    void *p = (void *)(new CallTestB());
    memcpy(&a, &p, sizeof(a));
    a->VcallFunc();
}

void CfiNvcall()
{
    CallTestA *a;
    void *p = (void *)(new CallTestB());
    memcpy(&a, &p, sizeof(a));
    a->CallFunc();
}


int main()
{
    if (DEBUG) {
        ShowCfiLogFile();
    }
    ClearCfiLog();
    if (DEBUG) {
        ShowCfiLogFile();
    }
    // clang allow it by default. It can be disabled with -fsanitize=cfi-cast-strict.
    CfiCastStrict();

    CfiDerivedCast();
    FindAndCheck("runtime error: control flow integrity check for type 'B' failed during base-to-derived cast");

    CfiUnrelatedCast();
    FindAndCheck("runtime error: control flow integrity check for type 'D' failed during cast to unrelated type");

    CfiNvcall();
    FindAndCheck("runtime error: control flow integrity check for type 'CallTestA' failed during non-virtual call");

    CfiVcall();
    FindAndCheck("runtime error: control flow integrity check for type 'CallTestA' failed during virtual call");

    CfiIcall();
    FindAndCheck(
        "runtime error: control flow integrity check for type 'void (int)' failed during indirect function call");

    if (DEBUG) {
        ShowCfiLogFile();
    }
}
