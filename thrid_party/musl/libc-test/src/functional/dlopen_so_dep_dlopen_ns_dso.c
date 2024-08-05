extern void* call_dlopen(const char *name);

void* call(const char *name)
{
    return call_dlopen(name);
}