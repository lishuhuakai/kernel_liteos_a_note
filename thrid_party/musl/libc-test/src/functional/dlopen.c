#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "test.h"
#include "global.h"

#define SO_FOR_NO_DELETE "lib_for_no_delete.so"
#define SO_FOR_DLOPEN "lib_for_dlopen.so"
#define SO_LOAD_BY_LOCAL "libdlopen_for_load_by_local_dso.so"
#define SO_LOAD_BY_GLOBAL "libdlopen_for_load_by_global_dso.so"
#define SO_CLOSE_RECURSIVE_OPEN_SO "libdlclose_recursive_dlopen_so.so"
#define NR_DLCLOSE_THREADS 10

typedef void(*TEST_PTR)(void);

void do_dlopen(const char *name, int mode)
{
	void* handle = dlopen(name, mode);

	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", name, mode, dlerror());

	if(dlclose(handle))
		t_error("dlclose %s failed : %s \n", name, dlerror());
}

void dlopen_lazy()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_LAZY);
}

void dlopen_now()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_NOW);
}

void dlopen_global()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_GLOBAL);
}

void dlopen_local()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_LOCAL);
}

void dlopen_so_used_by_dlsym()
{
	void* handle1 = dlopen(SO_LOAD_BY_LOCAL, RTLD_LOCAL);
	if(!handle1)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_LOAD_BY_LOCAL, RTLD_LOCAL, dlerror());

	// dlsym can't see the so which is loaded by RTLD_LOCAL.
	TEST_PTR for_local_ptr = dlsym(RTLD_DEFAULT, "for_local");
	if (for_local_ptr != NULL) {
		t_error("dlsym RTLD_LOCAL so(%s) should failed but get succeed.\n", "for_local");
	}

	if(dlclose(handle1))
		t_error("dlclose %s failed : %s \n", SO_LOAD_BY_LOCAL, dlerror());


	void* handle2 = dlopen(SO_LOAD_BY_GLOBAL, RTLD_GLOBAL);
	if(!handle2)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_LOAD_BY_GLOBAL, RTLD_LOCAL, dlerror());

	// dlsym can see the so which is loaded by RTLD_DEFAULT even without dependencies.
	TEST_PTR for_global_ptr = dlsym(RTLD_DEFAULT, "for_global");
	if (!for_global_ptr) {
		t_error("dlsym RTLD_GLOBAL so(%s) should succeed but get failed: %s \n", "for_global", dlerror());
	}

	if(dlclose(handle2))
		t_error("dlclose %s failed : %s \n", SO_LOAD_BY_GLOBAL, dlerror());
}

void dlopen_nodelete_and_noload()
{
	void* handle1 = dlopen(SO_FOR_NO_DELETE, RTLD_NODELETE);

	if(!handle1)
		t_error("dlopen(name=%s, mode=RTLD_NODELETE) failed: %s\n", SO_FOR_NO_DELETE, dlerror());

	if(dlclose(handle1))
		t_error("dlclose %s failed : %s \n", SO_FOR_NO_DELETE, dlerror());


	void* handle2 = dlopen(SO_FOR_NO_DELETE, RTLD_NOLOAD);
	if(!handle2)
		t_error("dlopen(name=%s, mode=RTLD_NOLOAD) failed: %s\n", SO_FOR_NO_DELETE, dlerror());

	if (handle1 != handle2) {
		t_error("dlopen %s by RTLD_NODELETE but get different handle when dlopen by RTLD_NOLOAD again.\n", SO_FOR_NO_DELETE);
	}
}

void dlopen_dlclose()
{
	void* handle = dlopen(SO_FOR_DLOPEN, RTLD_LOCAL);
	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());

	handle = dlopen(SO_FOR_DLOPEN, RTLD_LOCAL);
	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());

	if(dlclose(handle))
		t_error("dlclose %s failed : %s \n", SO_FOR_DLOPEN, dlerror());

	// lib should still exist in memory.
	handle = dlopen(SO_FOR_DLOPEN, RTLD_NOLOAD);
	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());

	if(dlclose(handle))
			t_error("dlclose %s failed : %s \n", SO_FOR_DLOPEN, dlerror());

	// It need to do one more dlclose because call dlopen by RTLD_NOLOAD add reference counting.
	if(dlclose(handle))
			t_error("dlclose %s failed : %s \n", SO_FOR_DLOPEN, dlerror());

	// dlopen and dlclose call counts match so the lib should not exist in memory.
	handle = dlopen(SO_FOR_DLOPEN, RTLD_NOLOAD);
	if(handle) {
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());
		dlclose(handle);
	}
}

#define DLOPEN_WEAK "libdlopen_weak.so"
typedef int (*func_ptr)();

void dlopen_dlclose_weak()
{
	void* handle = dlopen(DLOPEN_WEAK, RTLD_LAZY | RTLD_GLOBAL);
	if (!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", DLOPEN_WEAK, RTLD_LAZY | RTLD_GLOBAL, dlerror());
	func_ptr fn = (func_ptr)dlsym(handle, "test_number");
	if (fn) {
		int ret = fn();
		if (ret != GLOBAL_VALUE)
			t_error("weak symbol relocation error: so_name: %s, symbol: test_number\n", DLOPEN_WEAK);
	}
	dlclose(handle);
}

void dlclose_recursive()
{
    void *handle = dlopen(SO_CLOSE_RECURSIVE_OPEN_SO, RTLD_LAZY | RTLD_LOCAL);
    if (!handle)
        t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_CLOSE_RECURSIVE_OPEN_SO, RTLD_LAZY | RTLD_LOCAL, dlerror());

    /* close handle normally, if libc doesn't support close .so file recursivly
     * it will be deedlock, and timed out error will happen
     */
    dlclose(handle);
}

void *dlclose_recursive_thread()
{
	dlclose_recursive();
	return NULL;
}

void dlclose_recursive_by_multipthread()
{
	pthread_t testThreads[NR_DLCLOSE_THREADS] = {0};
	for (int i = 0; i < NR_DLCLOSE_THREADS; ++i) {
		pthread_create(&testThreads[i], NULL, dlclose_recursive_thread, NULL);
	}

	for (int i = 0; i < NR_DLCLOSE_THREADS; ++i) {
		pthread_join(testThreads[i], NULL);
	}
}

#define DLOPEN_GLOBAL "libdlopen_global.so"
#define DLOPEN_LOCAL "libdlopen_local.so"

void dlopen_global_test()
{
	int value;
	void *global_handler = dlopen(DLOPEN_GLOBAL, RTLD_GLOBAL);
	if (!global_handler)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", DLOPEN_GLOBAL, RTLD_GLOBAL, dlerror());
	func_ptr global_fn = (func_ptr)dlsym(global_handler, "global_caller");
	if (global_fn) {
		value = global_fn();
		if (value != GLOBAL_VALUE)
			t_error("global caller returned: %d, expected: %d\n", value, GLOBAL_VALUE);
	}

	void *local_handler = dlopen(DLOPEN_LOCAL, RTLD_LOCAL);
	if (!local_handler)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", DLOPEN_LOCAL, RTLD_LOCAL, dlerror());
	func_ptr local_fn = (func_ptr)dlsym(local_handler, "local_caller");
	if (local_fn) {
		value = local_fn();
		if (value != GLOBAL_VALUE)
			t_error("local caller returned: %d, expected: %d\n", value, GLOBAL_VALUE);
	}
	dlclose(global_handler);
	dlclose(local_handler);
}

#define DLCLOSE_EXIT_DEAD_LOCK "libdl_multithread_test_dso.so"

void dlclose_exit_test()
{
	int status;
	void* handle;
	int pid = fork();
	switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            handle = dlopen(DLCLOSE_EXIT_DEAD_LOCK, RTLD_GLOBAL);
			if (!handle) {
				t_error("dlclose_exit_test dlopen %s failed: %s", DLCLOSE_EXIT_DEAD_LOCK, dlerror());
				exit(EXIT_FAILURE);
			}
            exit(EXIT_SUCCESS);
        default:
            waitpid(pid, &status, WUNTRACED);
			if (WIFEXITED(status)) {
				if (WEXITSTATUS(status) != EXIT_SUCCESS) {
					t_error("dlclose_exit_test failed");
				};
			}
            break;
    }
    return;
}

#define DLCLOSE_WITH_TLS "libdlclose_tls.so"
typedef void *(*functype)(void);
void dlclose_with_tls_test()
{
	void* handle = dlopen(DLCLOSE_WITH_TLS, RTLD_GLOBAL);
	if (!handle) {
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", DLCLOSE_WITH_TLS, RTLD_GLOBAL, dlerror());
	}
	functype func = (functype)dlsym(handle, "foo_ctor");
	func();
	dlclose(handle);
}

int main(int argc, char *argv[])
{
	void *h, *g;
	int *i, *i2;
	char *s;
	void (*f)(void);
	char buf[512];

	if (!t_pathrel(buf, sizeof buf, argv[0], "libdlopen_dso.so")) {
		t_error("failed to obtain relative path to dlopen_dso.so\n");
		return 1;
	}
	h = dlopen(buf, RTLD_LAZY|RTLD_LOCAL);
	if (!h)
		t_error("dlopen %s failed: %s\n", buf, dlerror());
	i = dlsym(h, "i");
	if (!i)
		t_error("dlsym i failed: %s\n", dlerror());
	if (*i != 1)
		t_error("initialization failed: want i=1 got i=%d\n", *i);
	f = (void (*)(void))dlsym(h, "f");
	if (!f)
		t_error("dlsym f failed: %s\n", dlerror());
	f();
	if (*i != 2)
		t_error("f call failed: want i=2 got i=%d\n", *i);

	g = dlopen(0, RTLD_LAZY|RTLD_LOCAL);
	if (!g)
		t_error("dlopen 0 failed: %s\n", dlerror());
	i2 = dlsym(g, "i");
	s = dlerror();
	if (i2 || s == 0)
		t_error("dlsym i should have failed\n");
	if (dlsym(g, "main") != (void*)main)
		t_error("dlsym main failed: %s\n", dlerror());

	/* close+open reinitializes the dso with glibc but not with musl */
	h = dlopen(buf, RTLD_LAZY|RTLD_GLOBAL);
	i2 = dlsym(g, "i");
	if (!i2)
		t_error("dlsym i failed: %s\n", dlerror());
	if (i2 != i)
		t_error("reopened dso should have the same symbols, want %p, got %p\n", i, i2);
	if (*i2 != 2)
		t_error("reopened dso should have the same symbols, want i2==2, got i2==%d\n", *i2);
	if (dlclose(g))
		t_error("dlclose failed: %s\n", dlerror());
	if (dlclose(h))
		t_error("dlclose failed: %s\n", dlerror());

	dlopen_lazy();
	dlopen_now();
	dlopen_global();
	dlopen_local();
	dlopen_so_used_by_dlsym();
	dlopen_nodelete_and_noload();
	dlopen_dlclose();
	dlopen_dlclose_weak();
	dlclose_recursive();
	dlclose_recursive_by_multipthread();
	dlopen_global_test();
	dlclose_exit_test();
	dlclose_with_tls_test();

	return t_status;
}
