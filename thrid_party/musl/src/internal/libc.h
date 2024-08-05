#ifndef LIBC_H
#define LIBC_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <link.h>
#include <locale.h>

struct __locale_map;

struct __locale_struct {
	const struct __locale_map *cat[LC_ALL];
};

struct tls_module {
	struct tls_module *next;
	void *image;
	size_t len, size, align, offset;
};

struct __libc {
	char can_do_threads;
	char threaded;
	char secure;
	volatile signed char need_locks;
	int threads_minus_1;
	size_t *auxv;
	struct tls_module *tls_head;
	size_t tls_size, tls_align, tls_cnt;
	size_t page_size;
	struct __locale_struct global_locale;
#ifdef ENABLE_HWASAN
	void (*load_hook)(unsigned long int base, const Elf64_Phdr* phdr, int phnum);
	void (*unload_hook)(unsigned long int base, const Elf64_Phdr* phdr, int phnum);
#endif
#ifdef __LITEOS_A__
	int exit;
#endif
};

#ifndef PAGE_SIZE
#define PAGE_SIZE libc.page_size
#endif

extern hidden struct __libc __libc;
#define libc __libc

hidden void __init_libc(char **, char *);
hidden void __init_tls(size_t *);
hidden void __init_ssp(void *);
hidden void __libc_start_init(void);
hidden void __funcs_on_exit(void);
hidden void __funcs_on_quick_exit(void);
hidden void __libc_exit_fini(void);
hidden void __fork_handler(int);
#ifdef __LITEOS_A__
hidden void __sig_init(void);
hidden void arm_do_signal(int);
#endif

extern hidden size_t __hwcap;
extern hidden size_t __sysinfo;
extern char *__progname, *__progname_full;

extern hidden const char __libc_version[];

hidden void __synccall(void (*)(void *), void *);
hidden int __setxid(int, int, int, int);

#endif
