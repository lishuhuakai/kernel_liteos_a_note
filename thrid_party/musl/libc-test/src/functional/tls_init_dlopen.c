#include <string.h>
#include <dlfcn.h>
#include "test.h"

int main(int argc, char *argv[])
{
	void *h;
	char *(*f)(void);
	char *s;
	char buf[512];

	t_pathrel(buf, sizeof buf, argv[0], "libtls_init_dso.so");
	h = dlopen(buf, RTLD_NOW|RTLD_GLOBAL);
	if (!h)
		t_error("dlopen failed: %s\n", dlerror());
	f = dlsym(h, "gettls");
	if (!f)
		t_error("dlsym failed: %s\n", dlerror());
	s = f();
	if (!s)
		t_error("tls was not initialized at dlopen\n");
	if (strcmp(s, "foobar")!=0)
		t_error("tls was not initialized correctly at dlopen (got \"%s\", want \"foobar\"\n", s);

	return t_status;
}
