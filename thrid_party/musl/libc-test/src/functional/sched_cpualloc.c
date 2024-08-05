// sched_cpualloc(0) should return unique pointers
// (often expected and gnulib replaces sched_cpualloc if sched_cpualloc(0) returns 0)
#include <stdlib.h>
#include <sched.h>
#include "test.h"

int main(void)
{
	cpu_set_t *p = __sched_cpualloc(0);
	cpu_set_t *q = __sched_cpualloc(0);
	cpu_set_t *r = __sched_cpualloc(0);
	if (!p || !q || !r)
		t_error("sched_cpualloc(0) returned NULL\n");
	if (p == q || p == r || q == r)
		t_error("sched_cpualloc(0) returned non-unique pointers: %p, %p, %p\n", p, q, r);
	free(q);
	free(p);
	free(r);
	return t_status;
}
