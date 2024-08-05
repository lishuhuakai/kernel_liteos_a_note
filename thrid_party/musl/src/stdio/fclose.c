#include "stdio_impl.h"
#include <stdlib.h>
#ifndef __LITEOS__
#include <errno.h>
#endif

static void dummy(FILE *f) { }
weak_alias(dummy, __unlist_locked_file);

int fclose(FILE *f)
{
	int r;
	
	FLOCK(f);
#ifndef __LITEOS__
	if (!f || f->fd < 0) {
		errno = EBADF;
		FUNLOCK(f);
		return -EBADF;
	}
#endif

	r = fflush(f);
	r |= f->close(f);
	FUNLOCK(f);

	/* Past this point, f is closed and any further explict access
	 * to it is undefined. However, it still exists as an entry in
	 * the open file list and possibly in the thread's locked files
	 * list, if it was closed while explicitly locked. Functions
	 * which process these lists must tolerate dead FILE objects
	 * (which necessarily have inactive buffer pointers) without
	 * producing any side effects. */

	if (f->flags & F_PERM) return r;

	__unlist_locked_file(f);

	free(f->getln_buf);
	/* release base instead of buf which may be modified by setvbuf
	 * or iniitalize by local variable */
	free(f->base);

#ifndef __LITEOS__
	/* set file to invalid descriptor */
	f->fd = -EBADF;
#endif

	__ofl_free(f);

	return r;
}
