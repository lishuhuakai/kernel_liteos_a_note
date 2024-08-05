#ifndef _STDIO_IMPL_H
#define _STDIO_IMPL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "syscall.h"

#define UNGET 8

#define FFINALLOCK(f) ((f)->lock>=0 ? __lockfile((f)) : 0)
#define FLOCK(f) int __need_unlock = ((f)->lock>=0 ? __lockfile((f)) : 0)
#define FUNLOCK(f) do { if (__need_unlock) __unlockfile((f)); } while (0)

#define F_PERM 1
#define F_NORD 4
#define F_NOWR 8
#define F_EOF 16
#define F_ERR 32
#define F_SVB 64
#define F_APP 128
#define F_NOBUF 256
#define F_PBUF 512


#define FILE_LIST_NEXT(fl) (fl->next)
#define FILE_LIST_HEAD(head) (head)
#define FILE_LIST_EMPTY(head) ((head) == NULL)
#define	FILE_SAVELINK(name, link)	void **name = (void *)&(link)
#define	INVALID_LINK(x)	do {(x) = (void *)-1;} while (0)

#define FILE_LIST_CHECK_NEXT(fl) do { \
	if (FILE_LIST_NEXT(fl) != NULL && FILE_LIST_NEXT(fl)->prev != &fl->next) { \
		abort(); \
	} \
} while (0)

#define FILE_LIST_CHECK_PREV(fl) do { \
	if (*fl->prev != fl) { \
		abort(); \
	} \
} while (0)

#define FILE_LIST_CHECK_HEAD(head) do { \
	if (FILE_LIST_HEAD(head) != NULL && \
		FILE_LIST_HEAD(head)->prev != &FILE_LIST_HEAD(head)) { \
		abort(); \
	} \
} while (0)


#define FILE_LIST_INSERT_HEAD(head, fl) do { \
	FILE_LIST_CHECK_HEAD((head)); \
	if ((FILE_LIST_NEXT((fl)) = FILE_LIST_HEAD((head))) != NULL) { \
		FILE_LIST_HEAD((head))->prev = &FILE_LIST_NEXT((fl)); \
	} \
	FILE_LIST_HEAD(head) = (fl); \
	(fl)->prev = &FILE_LIST_HEAD((head)); \
} while (0)

#define FILE_LIST_REMOVE(fl) do { \
	FILE_SAVELINK(oldnext, (fl)->next); \
	FILE_SAVELINK(oldprev, (fl)->prev); \
	FILE_LIST_CHECK_NEXT(fl); \
	FILE_LIST_CHECK_PREV(fl); \
	if (FILE_LIST_NEXT(fl) != NULL) { \
		FILE_LIST_NEXT(fl)->prev = fl->prev; \
	} \
	*fl->prev = FILE_LIST_NEXT(fl); \
	INVALID_LINK(*oldnext); \
	INVALID_LINK(*oldprev); \
} while (0)

struct _IO_FILE {
	unsigned flags;
	unsigned char *rpos, *rend;
	int (*close)(FILE *);
	unsigned char *wend, *wpos;
	unsigned char *mustbezero_1;
	unsigned char *wbase;
	size_t (*read)(FILE *, unsigned char *, size_t);
	size_t (*readx)(FILE *, unsigned char *, size_t);
	size_t (*write)(FILE *, const unsigned char *, size_t);
	off_t (*seek)(FILE *, off_t, int);
	unsigned char *buf;
	size_t buf_size;
	/* when allocating buffer dynamically, base == buf - UNGET,
	 * free base when calling fclose.
	 * otherwise, base == NULL, cases:
	 * 1. in stdout, stdin, stdout, base is static array.
	 * 2. call setvbuf to set buffer or non-buffer.
	 * 3. call fmemopen, base == NULL && buf_size != 0.
	 */
	unsigned char *base;
#ifndef __LITEOS__
	FILE **prev, *next;
#else
	FILE *prev, *next;
#endif
	int fd;
	int pipe_pid;
	long lockcount;
	int mode;
	volatile int lock;
	int lbf;
	void *cookie;
	off_t off;
	char *getln_buf;
	void *mustbezero_2;
	unsigned char *shend;
	off_t shlim, shcnt;
	FILE *prev_locked, *next_locked;
	struct __locale_struct *locale;
};

extern hidden FILE *volatile __stdin_used;
extern hidden FILE *volatile __stdout_used;
extern hidden FILE *volatile __stderr_used;

hidden int __lockfile(FILE *);
hidden void __unlockfile(FILE *);

hidden size_t __stdio_read(FILE *, unsigned char *, size_t);
hidden size_t __stdio_readx(FILE *, unsigned char *, size_t);
hidden size_t __stdio_write(FILE *, const unsigned char *, size_t);
hidden size_t __stdout_write(FILE *, const unsigned char *, size_t);
hidden off_t __stdio_seek(FILE *, off_t, int);
hidden int __stdio_close(FILE *);

hidden int __fill_buffer(FILE *f);
hidden ssize_t __flush_buffer(FILE *f);

hidden int __toread(FILE *);
hidden int __towrite(FILE *);
hidden int __falloc_buf(FILE *);

hidden void __stdio_exit(void);
hidden void __stdio_exit_needed(void);

#if defined(__PIC__) && (100*__GNUC__+__GNUC_MINOR__ >= 303)
__attribute__((visibility("protected")))
#endif
int __overflow(FILE *, int), __uflow(FILE *);

hidden int __fseeko(FILE *, off_t, int);
hidden int __fseeko_unlocked(FILE *, off_t, int);
hidden off_t __ftello(FILE *);
hidden off_t __ftello_unlocked(FILE *);
hidden size_t __fwritex(const unsigned char *, size_t, FILE *);
hidden int __putc_unlocked(int, FILE *);

hidden FILE *__fdopen(int, const char *);
hidden FILE *__fdopenx(int, int);
hidden int __fmodeflags(const char *, int *);

hidden FILE *__ofl_add(FILE *f);
hidden FILE **__ofl_lock(void);
hidden void __ofl_unlock(void);
hidden void __ofl_free(FILE *f);
hidden FILE *__ofl_alloc();

struct __pthread;
hidden void __register_locked_file(FILE *, struct __pthread *);
hidden void __unlist_locked_file(FILE *);
hidden void __do_orphaned_stdio_locks(void);

#define MAYBE_WAITERS 0x40000000

hidden void __getopt_msg(const char *, const char *, const char *, size_t);

#define feof(f) ((f)->flags & F_EOF)
#define ferror(f) ((f)->flags & F_ERR)

#define getc_unlocked(f) \
	( ((f)->rpos != (f)->rend) ? *(f)->rpos++ : __uflow((f)) )

#define putc_unlocked(c, f) \
	( (((unsigned char)(c)!=(f)->lbf && (f)->wpos!=(f)->wend)) \
	? *(f)->wpos++ = (unsigned char)(c) \
	: __overflow((f),(unsigned char)(c)) )

/* Caller-allocated FILE * operations */
hidden FILE *__fopen_rb_ca(const char *, FILE *, unsigned char *, size_t);
hidden int __fclose_ca(FILE *);

static uint64_t __get_file_tag(FILE* fp)
{
	if (fp == stdin || fp == stderr || fp == stdout) {
		return 0;
	}
	return fdsan_create_owner_tag(FDSAN_OWNER_TYPE_FILE, (uint64_t)fp);
}
#endif
