#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

#define SO_HAS_DEPENDENCES "libdlopen_so_dep_dlopen_ns_dso.so"
const char* dllName = "libdlopen_ns_dso.so";
const char* dllName2 = "libdlopen_dso.so";
const char* errPath_ns = "/src/common";
const char* ndk_so = "libdlopen_ns_dso_ndk.so";
const char* system_so = "libdlopen_ns_dso_sys.so";
typedef void*(*CALL_DLOPEN_PTR)(const char *);

// Test whether it is ok for main to dlopen ndk so.
void main_dlopen_ndk_so()
{
    void *handle = dlopen(ndk_so, RTLD_NOW);
    if (!handle)
        t_error("main dlopen ndk so %s failed: %s\n", ndk_so, dlerror());
    if (dlclose(handle))
        t_error("dlclose %s failed : %s \n", ndk_so, dlerror());
}

// Test whether it is ok for system so to dlopen ndk so.
void system_so_dlopen_ndk_so()
{
    void *handle = dlopen(system_so, RTLD_NOW);
    if (!handle) {
        t_error("dlopen %s failed : %s \n", system_so, dlerror());
    }
    CALL_DLOPEN_PTR call_dlopen = dlsym(handle, "call_dlopen");
    if (!call_dlopen) {
        t_error("dlsym %s failed : %s \n", "call_dlopen", dlerror());
    }
    void *res = call_dlopen(ndk_so);
    if (!res) {
        t_error("default ns so(%s) can't dlopen ndk so(%s)", system_so, ndk_so);
    }
}

// Test whether it is ok for ndk so to dlopen system so.
void ndk_so_dlopen_default_ns_so()
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ndk");
    void *handle = dlopen_ns(&dlns, ndk_so, RTLD_NOW);
    if (!handle) {
        t_error("dlopen_ns(ns=%s, so=%s) failed : %s \n", "ndk", ndk_so, dlerror());
    }
    CALL_DLOPEN_PTR call_dlopen = dlsym(handle, "call_dlopen");
    if (!call_dlopen) {
        t_error("dlsym %s failed : %s \n", "call_dlopen", dlerror());
    }
    void *res = call_dlopen(system_so);
    if (!res) {
        t_error("ndk so(%s) can't dlopen system so(%s)", ndk_so, system_so);
    }
}

// Test whether dlopen same so twice by same ns to get same handle.
void dlopen_same_so_twice_by_same_ns(char * dllPath_ns)
{
    Dl_namespace dlns1;
    dlns_init(&dlns1, "ns_for_dlopen_same_so_twice_by_same_ns");
    dlns_create(&dlns1, dllPath_ns);

    void* handle1 = dlopen_ns(&dlns1, dllName, RTLD_NOW);
    if(!handle1)
        t_error("dlopen_ns(ns=%s, name=%s) failed : %s \n", dlns1.name, dllName, dlerror());

    void* handle2 = dlopen_ns(&dlns1, dllName, RTLD_NOW);
    if(!handle2)
        t_error("dlopen_ns(ns=%s, name=%s) failed : %s \n", dlns1.name, dllName, dlerror());

    if (handle1 != handle2)
        t_error("dlopen same so(%s) by same ns but handle is different %s \n", dllName);

    if(dlclose(handle1))
        t_error("dlclose %s failed : %s \n", dllName, dlerror());

    if(dlclose(handle2))
        t_error("dlclose %s failed : %s \n", dllName, dlerror());
}

// Test whether dlopen same so by different ns to get different handle.
void dlopen_same_so_by_different_ns(char * dllPath_ns)
{
    Dl_namespace dlns1;
    dlns_init(&dlns1, "ns_for_dlopen_same_so_by_different_ns_1");
    dlns_create(&dlns1, dllPath_ns);

    Dl_namespace dlns2;
    dlns_init(&dlns2, "ns_for_dlopen_same_so_by_different_ns_2");
    dlns_create(&dlns2, dllPath_ns);

    void* handle1 = dlopen_ns(&dlns1, dllName, RTLD_NOW);
    if(!handle1)
        t_error("dlopen_ns(ns=%s, name=%s) failed : %s \n", dlns1.name, dllName, dlerror());

    void* handle2 = dlopen_ns(&dlns2, dllName, RTLD_NOW);
    if(!handle2)
        t_error("dlopen_ns(ns=%s, name=%s) failed : %s \n", dlns2.name, dllName, dlerror());

    if (handle1 == handle2)
        t_error("dlopen same so(%s) by same ns but handle is different %s \n", dllName);

    if(dlclose(handle1))
        t_error("dlclose %s failed : %s \n", dllName, dlerror());

    if(dlclose(handle2))
        t_error("dlclose %s failed : %s \n", dllName, dlerror());
}

// Test whether dlopen same so by inherit ns to get same handle.
void dlopen_same_so_by_different_inherit_ns(char * dllPath_ns)
{
    Dl_namespace dlns1;
    dlns_init(&dlns1, "ns_for_dlopen_same_so_by_different_inherit_ns_1");
    dlns_create(&dlns1, dllPath_ns);

    Dl_namespace dlns2;
    dlns_init(&dlns2, "ns_for_dlopen_same_so_by_different_inherit_ns_2");
    dlns_create2(&dlns2, errPath_ns, 0);
    dlns_inherit(&dlns2, &dlns1, dllName);

    void* handle1 = dlopen_ns(&dlns1, dllName, RTLD_NOW);
    if(!handle1)
        t_error("dlopen_ns(ns=%s, name=%s) failed : %s \n", dlns1.name, dllName, dlerror());

    void* handle2 = dlopen_ns(&dlns2, dllName, RTLD_NOW);
    if(!handle2)
        t_error("dlopen_ns(ns=%s, name=%s) failed : %s \n", dlns2.name, dllName, dlerror());

    if (handle1 != handle2)
        t_error("dlopen same so(%s) by inherit ns but handle is different %s \n", dllName);

    if(dlclose(handle1))
        t_error("dlclose %s failed : %s \n", dllName, dlerror());

    if(dlclose(handle2))
        t_error("dlclose %s failed : %s \n", dllName, dlerror());
}

void dlopen_seperated(char * dllPath_ns)
{
    Dl_namespace dlns_default;
    dlns_get(NULL, &dlns_default);
    Dl_namespace dlns1;
    dlns_init(&dlns1, "ns_for_no_seperated");
    dlns_create2(&dlns1, errPath_ns, 0);
    dlns_inherit(&dlns1, &dlns_default, "libc++.so");
    void* handle = NULL;
    // current ns can't load this so.
    handle = dlopen_ns(&dlns1, dllName, RTLD_NOW);
    if(handle)
        t_error("dlopen_ns(ns=%s, name=%s) failed : %s \n", dlns1.name, dllName, dlerror());

    // current ns can load the so by absolute path.
    char absolute_path_1[512];
    snprintf(absolute_path_1, sizeof(absolute_path_1), "%s/%s", dllPath_ns, dllName);
    handle = dlopen_ns(&dlns1, absolute_path_1, RTLD_NOW);
    if (!handle)
        t_error("%s can load %s by absolute path but failed : %s \n", dlns1.name, absolute_path_1, dlerror());

    if(dlclose(handle))
        t_error("dlclose %s failed : %s \n", absolute_path_1, dlerror());

    // current ns can't load the so by absolute path if it has inaccessible dependent so.
    char absolute_path_2[512];
    snprintf(absolute_path_2, sizeof(absolute_path_2), "%s/%s", dllPath_ns, SO_HAS_DEPENDENCES);
    handle = dlopen_ns(&dlns1, absolute_path_2, RTLD_NOW);
    if (handle)
        t_error("%s can't load %s by absolute path because but it has inaccessible dependent so but succeed : %s \n", dlns1.name, absolute_path_2, dlerror());
}

void dlopen_inherit(char* dllPath_ns)
{
    Dl_namespace dlns_default;
    dlns_get(NULL, &dlns_default);
    Dl_namespace inherit_A, inherit_B;
    dlns_init(&inherit_A, "inherir_error_lib_A");
    dlns_init(&inherit_B, "inherir_error_lib_B");
    dlns_create2(&inherit_A, NULL, 0);
    dlns_create2(&inherit_B, dllPath_ns, 0);
    dlns_inherit(&inherit_B, &dlns_default, "libc++.so");

    // inherit_A can't load the so because search path is NULL.
    void* handle1 = dlopen_ns(&inherit_A, dllName, RTLD_LAZY);
    if(handle1){
        t_error("dlopen_ns_by_ini_no_inherit handle %s should not open successfully \n", dllName);
        dlclose(handle1);
    }

    // inherit_A can load the so by inherit_B.
    dlns_inherit(&inherit_A, &inherit_B, dllName);
    void* handle2 = dlopen_ns(&inherit_A, dllName, RTLD_LAZY);
    if(!handle2)
        t_error("dlopen_ns_by_dlns_inherir handle get error %s open failed : %s \n", dllName, dlerror());

    if(dlclose(handle2))
        t_error("dlclose_by_dlns_inherir handle %s close failed : %s\n", dllName, dlerror());

    // inherit_A can't load the so by inherit ns if the so isn't in shared libs.
    dlns_inherit(&inherit_A, &inherit_B, dllName2);
    void* handle3 = dlopen_ns(&inherit_A, dllName, RTLD_LAZY);
    if(handle3){
        t_error("dlopen_ns_by_ini_no_inherit handle2 %s should not open successfully \n", dllName);
        dlclose(handle3);
    }
}

void dlopen_inherit_check_can_pass(char* dllPath_ns)
{
    Dl_namespace dlns_default;
    dlns_get(NULL, &dlns_default);
    Dl_namespace transitivity_A, transitivity_B, transitivity_C;
    dlns_init(&transitivity_A, "transitivity_A");
    dlns_init(&transitivity_B, "transitivity_B");
    dlns_init(&transitivity_C, "transitivity_C");
    dlns_create2(&transitivity_A, NULL, 0);
    dlns_create2(&transitivity_B, NULL, 0);
    dlns_create2(&transitivity_C, dllPath_ns, 0);
    dlns_inherit(&transitivity_A, &transitivity_B, NULL);
    dlns_inherit(&transitivity_B, &transitivity_C, dllName);
    dlns_inherit(&transitivity_C, &dlns_default, "libc++.so");

    void* handleB = dlopen_ns(&transitivity_B, dllName, RTLD_LAZY);
    if(!handleB)
        t_error("dlopen_ns_by_inherit_transitivity handleB get error %s open failed : %s \n", dllName, dlerror());

    void* handleC = dlopen_ns(&transitivity_C, dllName, RTLD_LAZY);
    if(!handleC)
        t_error("dlopen_ns_by_inherit_transitivity handleC get error %s open failed : %s \n", dllName, dlerror());

    if(dlclose(handleC))
        t_error("dlclose_by_inherit_transitivity handleC %s close failed : %s\n", dllName, dlerror());
    if(dlclose(handleB))
        t_error("dlclose_by_inherit_transitivity handleB %s close failed : %s\n", dllName, dlerror());

    // transitivity_A can't load so because inherit can't pass.
    void* handleA = dlopen_ns(&transitivity_A, dllName, RTLD_LAZY);
    if(handleA){
        t_error("dlopen_ns_by_inherit_transitivity handleA %s should not open successfully \n", dllName);
        dlclose(handleA);
    }
}

void dlopen_test_dlns_create2()
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_for_create2");
    // ns_for_create2 doesn't exist, it will use default ns to search so.
    void* handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    if(!handle)
        t_error("dlopen_ns %s use a non-existent ns failed: %s\n", dllName, dlerror());

    if(dlclose(handle))
        t_error("dlclose %s failed : %s \n", dllName, dlerror());

    // 0: ns_for_create2 doesn't inherit default ns.
    dlns_create2(&dlns, errPath_ns, 0);
    void* handle2 = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    if(handle2)
        t_error("%s namespace can't see %s but dlopen succee.\n", "ns_for_create2", dllName);
}

int main(int argc, char *argv[])
{
    char buf[512],path[512];
    char* i;
    //带so的路径
    if (!t_pathrel(buf, sizeof buf, argv[0], "libdlopen_ns_dso.so")) {
        t_error("failed to obtain relative path to libdlopen_ns_dso.so\n");
        return 1;
	}
    //包含so的路径
    if (!t_pathrel(path, sizeof path, argv[0], "")) {
        t_error("failed to obtain relative path to path\n");
        return 1;
	}
    path[strlen (path) -1 ] ='\0';
    main_dlopen_ndk_so();
    system_so_dlopen_ndk_so();
    ndk_so_dlopen_default_ns_so();
    dlopen_same_so_twice_by_same_ns(path);
    dlopen_same_so_by_different_ns(path);
    dlopen_same_so_by_different_inherit_ns(path);
    dlopen_seperated(path);
    dlopen_test_dlns_create2();
    dlopen_inherit(path);
    dlopen_inherit_check_can_pass(path);

    return t_status;
}