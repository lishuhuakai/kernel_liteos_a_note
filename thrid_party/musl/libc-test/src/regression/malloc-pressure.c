#include <malloc.h>
#include <time.h>
#include "test.h"

#define SIZE_ALIGN (4 * sizeof(size_t))
#define THRESHOLD (0x1c00 * SIZE_ALIGN)
#define ITER_TIME 20

int main(int argc, char *argv[])
{
	struct timespec ts[2];

	clock_gettime(CLOCK_REALTIME, &ts[0]);
	for (int i = 0; i < ITER_TIME; ++i) {
		for (size_t size = 0; size < THRESHOLD; size += SIZE_ALIGN + 1) {
			void *ptr = malloc(size);
			if (!ptr) {
				t_error("Malloc failed for size %u\n", size);
				return -1;
			}
			free(ptr);
		}
	}

	return t_status;
}
