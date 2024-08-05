#include <stdio.h>
#include <dlfcn.h>
void sayhello()
{
   printf ("hello world!\n"); 
}

void* call_dlopen(const char* name)
{
   return dlopen(name, RTLD_NOW);
}