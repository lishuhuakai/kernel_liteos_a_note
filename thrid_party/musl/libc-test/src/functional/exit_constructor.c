#include <stdlib.h>
#include "test.h"

__attribute__((constructor))
void B_tor()
{
	exit(t_status);
}

int main()
{
	return t_status;
}