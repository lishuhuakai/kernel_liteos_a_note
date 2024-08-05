#include "stdlib.h"
#include "string.h"

void abort(void) {
  printf("System was being aborted\n");
  while (1);
}
