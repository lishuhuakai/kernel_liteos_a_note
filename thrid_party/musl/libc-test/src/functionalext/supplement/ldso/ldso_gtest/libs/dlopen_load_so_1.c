#include <dlfcn.h>

void DlopenLoadSo1() {}

__attribute__((constructor)) void DlopenLoadSoImpl()
{
    void* handle = dlopen("libc.so", RTLD_NOW);
    dlclose(handle);
}