#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include "test.h"

#define TEST(c, ...) ((c) ? 1 : (t_error(#c" failed: " __VA_ARGS__),0))
#define TESTVAL(v,op,x) TEST(v op x, "%jd\n", (intmax_t)(v))

extern int __fstat_time64(int, struct stat *);
extern int __futimens_time64(int, const struct timespec [2]);

int main(void)
{
	struct stat st;
	FILE *f;
	int fd;
	time_t t;

	TEST(utimensat(AT_FDCWD, "/dev/null/invalid", ((struct timespec[2]){{.tv_nsec=UTIME_OMIT},{.tv_nsec=UTIME_OMIT}}), 0)==0 || errno==ENOTDIR,
		"%s\n", strerror(errno));
	TEST(__futimens_time64(-1, ((struct timespec[2]){{.tv_nsec=UTIME_OMIT},{.tv_nsec=UTIME_OMIT}}))==0 || errno==EBADF,
		"%s\n", strerror(errno));

	if (!TEST(f = tmpfile())) return t_status;
	fd = fileno(f);

	TEST(__futimens_time64(fd, (struct timespec[2]){0}) == 0, "\n");
	TEST(__fstat_time64(fd, &st) == 0, "\n");
	TESTVAL(st.st_atim.tv_sec,==,0);
	TESTVAL(st.st_atim.tv_nsec,==,0);
	TESTVAL(st.st_mtim.tv_sec,==,0);
	TESTVAL(st.st_mtim.tv_nsec,==,0);

	TEST(__futimens_time64(fd, ((struct timespec[2]){{.tv_sec=1,.tv_nsec=UTIME_OMIT},{.tv_sec=1,.tv_nsec=UTIME_OMIT}})) == 0, "\n");
	TEST(__fstat_time64(fd, &st) == 0, "\n");
	TESTVAL(st.st_atim.tv_sec,==,0);
	TESTVAL(st.st_atim.tv_nsec,==,0);
	TESTVAL(st.st_mtim.tv_sec,==,0);
	TESTVAL(st.st_mtim.tv_nsec,==,0);

	t = time(0);

	TEST(__futimens_time64(fd, ((struct timespec[2]){{.tv_nsec=UTIME_NOW},{.tv_nsec=UTIME_OMIT}})) == 0, "\n");
	TEST(__fstat_time64(fd, &st) == 0, "\n");
	TESTVAL(st.st_atim.tv_sec,>=,t);
	TESTVAL(st.st_mtim.tv_sec,==,0);
	TESTVAL(st.st_mtim.tv_nsec,==,0);
	
	TEST(__futimens_time64(fd, (struct timespec[2]){0}) == 0, "\n");
	TEST(__futimens_time64(fd, ((struct timespec[2]){{.tv_nsec=UTIME_OMIT},{.tv_nsec=UTIME_NOW}})) == 0, "\n");
	TEST(__fstat_time64(fd, &st) == 0, "\n");
	TESTVAL(st.st_atim.tv_sec,==,0);
	TESTVAL(st.st_mtim.tv_sec,>=,t);

	TEST(__futimens_time64(fd, ((struct timespec[2]){{.tv_nsec=UTIME_NOW},{.tv_nsec=UTIME_OMIT}})) == 0, "\n");
	TEST(__fstat_time64(fd, &st) == 0, "\n");
	TESTVAL(st.st_atim.tv_sec,>=,t);
	TESTVAL(st.st_mtim.tv_sec,>=,t);

	if (TEST((time_t)(1LL<<32) == (1LL<<32), "implementation has Y2038 EOL\n")) {
		if (TEST(__futimens_time64(fd, ((struct timespec[2]){{.tv_sec=1LL<<32},{.tv_sec=1LL<<32}})) == 0, "%s\n", strerror(errno))) {
			TEST(__fstat_time64(fd, &st) == 0, "\n");
			TESTVAL(st.st_atim.tv_sec, ==, 1LL<<32);
			TESTVAL(st.st_mtim.tv_sec, ==, 1LL<<32);
		}
	}

	fclose(f);

	return t_status;
}
