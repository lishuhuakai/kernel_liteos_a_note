#include <stdio.h>
#include <string.h>
#include <unsupported_api.h>

char *ctermid(char *s)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return s ? strcpy(s, "/dev/tty") : "/dev/tty";
}
