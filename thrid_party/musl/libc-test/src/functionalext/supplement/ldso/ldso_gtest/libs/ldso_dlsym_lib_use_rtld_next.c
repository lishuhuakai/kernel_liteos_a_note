#include <dlfcn.h>

void* RtldNextFunc()
{
    return dlsym(RTLD_NEXT, "printf");
}