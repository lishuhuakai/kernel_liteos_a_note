#include <cstdio>

struct base_t {
  virtual void f();
};

void base_t::f() {
  puts("base_t::f");
}

void do_f(base_t *ptr) {
  if (ptr)
    ptr->f();
}
