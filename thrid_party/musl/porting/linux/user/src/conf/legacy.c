#include <sys/sysinfo.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "syscall.h"

#define _GNU_SOURCE
#include <dirent.h>

int get_cpu_default() {
	int i, cnt;
	unsigned char set[128] = {1};
	__syscall(SYS_sched_getaffinity, 0, sizeof(set), set);
	for ((i=cnt=0); (i < sizeof(set)); i++)
		for (; set[i]; (set[i]&=set[i]-1), cnt++);
	return cnt;
}

int get_nprocs_conf()
{
	struct dirent *de;
	int cnt = 0;
	DIR *d = opendir("/sys/devices/system/cpu");
	if (!d)
		return get_cpu_default();
	while ((de = readdir(d))) {
		if ((de->d_type == DT_DIR) &&
			(strlen(de->d_name) > 3) &&
			(de->d_name[0] == 'c') &&
			(de->d_name[1] == 'p') &&
			(de->d_name[2] == 'u') &&
			(isdigit(de->d_name[3])))
			cnt++;
	}
	closedir(d);
	return cnt;
}

int get_nprocs()
{
	int cnt;
	FILE* fp = fopen("/sys/devices/system/cpu/online", "re");
	if (fp != NULL) {
		char buf[128];
		memset(buf, 0, sizeof(buf));
		if (!fgets(buf, sizeof(buf), fp)) {
			return 1;
		}
		cnt = 0;
		char *tmp, *p;
		tmp = strtok_r(buf, ",", &p);
		while (tmp != NULL) {
			char *tok = strstr(tmp, "-");
			if (tok) {
				tok++;
				cnt += atoi(tok) - atoi(tmp) + 1;
			} else {
				cnt += 1;
			}
			tmp = strtok_r(NULL, ",", &p);
		}
		fclose(fp);
		return cnt;
	}
	return get_cpu_default();
}

long get_phys_pages()
{
	return sysconf(_SC_PHYS_PAGES);	
}

long get_avphys_pages()
{
	return sysconf(_SC_AVPHYS_PAGES);	
}
