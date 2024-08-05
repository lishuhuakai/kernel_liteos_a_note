#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "syscall.h"

static const size_t index_offset_offset = 12;
static const size_t data_offset_offset = 16;
static const size_t per_index_size = 48;

const char unsigned *__map_file(const char *pathname, size_t *size)
{
	struct stat st;
	const unsigned char *map = MAP_FAILED;
	int fd = sys_open(pathname, O_RDONLY|O_CLOEXEC|O_NONBLOCK);
	if (fd < 0) return 0;
	if (!__fstat(fd, &st)) {
		map = __mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
		*size = st.st_size;
	}
#ifdef __LITEOS_A__
	if (map == MAP_FAILED) {
		__syscall(SYS_close, fd);
	}
#else
	__syscall(SYS_close, fd);
#endif

	return map == MAP_FAILED ? 0 : map;
}

static size_t convert_byte_to_size_t(const unsigned char *map)
{
	/* Parse 4 continue bytes to size_t. 0, 1, 2, 3 is the index of bytes, 24, 16, 8 is the offset of byte in size_t*/
	return ((size_t)map[0] << 24) + ((size_t)map[1] << 16) + ((size_t)map[2] << 8) + (size_t)map[3];
}

const char unsigned *__map_tzdata_file(const char * tzdata_path, const char *tz_id, size_t *tzdata_size,
	size_t *offset, size_t *size)
{
	struct stat st;
	const unsigned char *map = MAP_FAILED;
	int fd = sys_open(tzdata_path, O_RDONLY|O_CLOEXEC|O_NONBLOCK);
	if (fd < 0) return 0;
	if (!__fstat(fd, &st)) {
		map = __mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
		*tzdata_size = st.st_size;
	}
	if (map == MAP_FAILED) {
		__syscall(SYS_close, fd);
		return 0;
	}
	size_t index_offset = convert_byte_to_size_t(map + index_offset_offset);
	size_t data_offset = convert_byte_to_size_t(map + data_offset_offset);
	int flag = 0;
	while (index_offset < data_offset) {
		if (!strncmp((const char *)map + index_offset, tz_id, strlen(tz_id))) {
			/* 40 is the offset of file offset value in per index data*/
			*offset = data_offset + convert_byte_to_size_t(map + index_offset + 40);
			/* 44 is the offset of file length value in per index data*/
			*size = convert_byte_to_size_t(map + index_offset + 44);
			flag = 1;
			break;
		}
		index_offset += per_index_size;
	}
	__syscall(SYS_close, fd);
	if (flag) {
		return map;
	}
	return 0;
}
