#include <cstdio>

struct base_t {
  virtual void f();
};

void do_f(base_t *ptr);

struct derived_t : base_t {
  void f() override;
};

void derived_t::f() {
  puts("derived_t::f");
}

static struct dummy {
  base_t *ptr = nullptr;

  ~dummy() {
    if (ptr)
      do_f(ptr);
  }
} __dummy;

extern "C" void create() {
  puts("create");
  __dummy.ptr = new derived_t();
}
