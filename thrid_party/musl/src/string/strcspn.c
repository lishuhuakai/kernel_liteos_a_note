// from openbsd, will be rectified later

#include <string.h>

size_t strcspn(const char *string1, const char *string2)
{
	const char *a, *p;
	char sc, c;
    
	for (a = string1;;) {
		sc = *a++;
		p = string2;
		do {
			if ((c = *p++) == sc) {
				return (a - 1 - string1);
                        }
		} while (c != 0);
	}
	/* NOTREACHED */
}