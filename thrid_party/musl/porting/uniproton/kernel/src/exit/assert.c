#include "assert.h"

void __assert_fail(const char* expr, const char* file, int line, const char* func) {
  printf("%s:%d: %s: assertion \"%s\" failed\n", file, line, func, expr);
  while (1);
}
