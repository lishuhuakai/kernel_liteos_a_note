struct __dirstream
{
	off_t tell;
	int fd;
	int buf_pos;
	int buf_end;
	volatile int lock[1];
#ifdef __LITEOS_A__
	// change buf len from 2048 to 4096 to support read 14 dirs at one readdir syscall
	char buf[4096];
#else
	/* Any changes to this struct must preserve the property:
	 * offsetof(struct __dirent, buf) % sizeof(off_t) == 0 */
	char buf[2048];
#endif
};
