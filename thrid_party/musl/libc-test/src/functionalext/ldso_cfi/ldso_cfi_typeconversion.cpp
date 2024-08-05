#include<cstdio>
#include<memory>
#include"cfi_util.h"

class A {
public:
    virtual void f(){
        printf("baseA");
    }
    virtual ~A(){}
};

class B : public A{
public:
    void f() override{
        printf("subclassB");
    }
    ~B() override{}
};

class C: public A{
public:
    void f() override{
        printf("subclassC");
    }
    ~C() override{}
};

int main(){

    if(DEBUG){
        ShowCfiLogFile();
    }
    ClearCfiLog();
    if(DEBUG){
        ShowCfiLogFile();
    }
    
    //This test will trigger a subclass parent class conversion CFI check failure
    std::shared_ptr<B> b_ptr = std::make_shared<B>();
    std::shared_ptr<A> a_ptr = b_ptr;
    int* a_vtable = (int*)a_ptr.get();
    printf("a_vtable: %x",*a_vtable);
    std::shared_ptr<C> c_ptr = std::static_pointer_cast<C>(a_ptr);
    FindAndCheck("'C' failed during base-to-derived cast");
    
    ClearCfiLog();
    if(DEBUG){
        ShowCfiLogFile();
    }
    return 0;
}
