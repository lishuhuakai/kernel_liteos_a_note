#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "__dirent.h"
#include "syscall.h"

uint64_t __get_dir_tag(DIR* dir)
{
	return fdsan_create_owner_tag(FDSAN_OWNER_TYPE_DIRECTORY, (uint64_t)dir);
}

DIR *opendir(const char *name)
{
	int fd;
	DIR *dir;

	if ((fd = open(name, O_RDONLY|O_DIRECTORY|O_CLOEXEC)) < 0)
		return 0;
	if (!(dir = calloc(1, sizeof *dir))) {
		__syscall(SYS_close, fd);
		return 0;
	}
	dir->fd = fd;
	fdsan_exchange_owner_tag(fd, 0, __get_dir_tag(dir));
	return dir;
}
