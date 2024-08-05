#include <iostream>
#include <memory>

class Foo {
public:
    Foo(int e)
    {
        a = e;
    }
    ~Foo()
    {
        if (a) {
            a = 0;
        }
    }
private:
    int a;
};

struct FooTlsDeleter
{
    void operator()(Foo* object)
    {
        if (object == nullptr) {
            return;
        }
        object->~Foo();
    }
};

extern "C" void foo_ctor()
{
    thread_local std::unique_ptr<Foo, FooTlsDeleter> foo(nullptr);
    foo = std::unique_ptr<Foo, FooTlsDeleter>(new Foo(1));
}
