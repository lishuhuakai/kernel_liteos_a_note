#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *getenv(const char *name)
{
	if (name == NULL || __environ == NULL)
		return 0;
	size_t i, l = 0;
	const char *np;
	char **p, *ep;
	for (; *(name + l) && *(name + l) != '='; ++l);
	for (p = __environ; (ep = *p) != NULL; ++p) {
		for (np = name, i = l; i && *ep; i--) {
			if (*ep++ != *np++) {
				break;
			}
		}
		if (i == 0 && *ep++ == '=') {
			return (ep);
		}
	}
	return 0;
}
