#include <time.h>
#include <errno.h>
#include <string.h>
#include "test.h"

#define TEST(c, ...) \
	( (c) || (t_error(#c " failed: " __VA_ARGS__),0) )

extern int __clock_gettime64(clockid_t, struct timespec *);

int main()
{
	struct timespec ts;
	TEST(clock_gettime(CLOCK_REALTIME, &ts) == 0 && errno == 0, "%s\n", strerror(errno));
	TEST(__clock_gettime64(CLOCK_REALTIME, &ts) == 0 && errno == 0, "%s\n", strerror(errno));
	return t_status;
}
