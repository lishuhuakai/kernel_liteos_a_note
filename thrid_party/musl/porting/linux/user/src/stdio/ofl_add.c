#include "stdio_impl.h"

FILE *__ofl_add(FILE *f)
{
	FILE **head = __ofl_lock();
	FILE_LIST_INSERT_HEAD(*head, f);
	__ofl_unlock();
	return f;
}
