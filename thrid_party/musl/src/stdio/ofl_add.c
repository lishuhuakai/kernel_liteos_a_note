#include "stdio_impl.h"

FILE *__ofl_add(FILE *f)
{
	FILE **head = __ofl_lock();
#ifndef __LITEOS__
    FILE_LIST_INSERT_HEAD(*head, f);
#else
	f->next = *head;
	if (*head) (*head)->prev = f;
	*head = f;
#endif
	__ofl_unlock();
	return f;
}
