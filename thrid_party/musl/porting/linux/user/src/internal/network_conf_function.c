#include <string.h>
#include <stdio.h>
#include "network_conf_function.h"

char *g_fixedServices[] = {
#define PORT_DESC(a) a
#include "services.h"
#undef PORT_DESC(a)
};

#define FIXED_SERVICES_COUNT (sizeof(g_fixedServices) / sizeof(char*))

int get_services_str(char *line, FILE *f, int *indexPtr)
{
	if (f) {
		return fgets(line, sizeof line, f);
	}
	if (*indexPtr < FIXED_SERVICES_COUNT) {
		memcpy(line, g_fixedServices[*indexPtr], strlen(g_fixedServices[*indexPtr]));
		(*indexPtr)++;
		return 1;
	}
	return NULL;
}