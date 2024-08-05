#include <dlfcn.h>
#include "cfi_util.h"

struct base_t;
void do_f(base_t *ptr);

int main() {
  do_f(nullptr); // do not omit the library
  ClearCfiLog();
  using create_t = void (*)();
  auto *lib = dlopen("libcfi_cross_dso_dtor_test_use.z.so", RTLD_NOW);
  auto *fn = (create_t)dlsym(lib, "create");
  fn();
  dlclose(lib);
  ExpectCfiOk();
  return 0;
}
