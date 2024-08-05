#define _GNU_SOURCE
#define SYSCALL_NO_TLS 1

#include "dynlink.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <elf.h>
#include <sys/mman.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <link.h>
#include <setjmp.h>
#include <pthread.h>
#include <ctype.h>
#include <dlfcn.h>
#include <semaphore.h>
#include <sys/membarrier.h>
#include <sys/time.h>
#include <time.h>
#include <sys/prctl.h>
#include <sys/queue.h>

#include "cfi.h"
#include "dlfcn_ext.h"
#include "dynlink_rand.h"
#include "ld_log.h"
#include "libc.h"
#include "musl_fdsan.h"
#include "namespace.h"
#include "ns_config.h"
#include "pthread_impl.h"
#include "fork_impl.h"
#include "strops.h"
#include "trace/trace_marker.h"

#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif
#ifdef LOAD_ORDER_RANDOMIZATION
#include "zip_archive.h"
#endif

#define malloc __libc_malloc
#define calloc __libc_calloc
#define realloc __libc_realloc
#define free __libc_free

static void error(const char *, ...);

#define MAXP2(a,b) (-(-(a)&-(b)))
#define ALIGN(x,y) ((x)+(y)-1 & -(y))
#define GNU_HASH_FILTER(ght, ghm, gho)                \
	const size_t *bloomwords = (const void *)(ght+4); \
	size_t f = bloomwords[gho & (ght[2]-1)];          \
	if (!(f & ghm)) continue;                         \
	f >>= (gh >> ght[3]) % (8 * sizeof f);            \
	if (!(f & 1)) continue;

#define container_of(p,t,m) ((t*)((char *)(p)-offsetof(t,m)))
#define countof(a) ((sizeof (a))/(sizeof (a)[0]))
#define DSO_FLAGS_NODELETE 0x1

#ifdef HANDLE_RANDOMIZATION
#define NEXT_DYNAMIC_INDEX 2
#define MIN_DEPS_COUNT 2
#define NAME_INDEX_ZERO 0
#define NAME_INDEX_ONE 1
#define NAME_INDEX_TWO 2
#define NAME_INDEX_THREE 3
#define TLS_CNT_INCREASE 3
#define INVALID_FD_INHIBIT_FURTHER_SEARCH (-2)
#endif

#define PARENTS_BASE_CAPACITY 8
#define RELOC_CAN_SEARCH_DSO_BASE_CAPACITY 32
#define ANON_NAME_MAX_LEN 70

#define KPMD_SIZE (1UL << 21)
#define HUGEPAGES_SUPPORTED_STR_SIZE (32)

#ifdef UNIT_TEST_STATIC
    #define UT_STATIC
#else
    #define UT_STATIC static
#endif

/* Used for dlclose */
#define UNLOAD_NR_DLOPEN_CHECK 1
#define UNLOAD_COMMON_CHECK 2
#define UNLOAD_ALL_CHECK 3
struct dso_entry {
	struct dso *dso;
	TAILQ_ENTRY(dso_entry) entries;
};

struct debug {
	int ver;
	void *head;
	void (*bp)(void);
	int state;
	void *base;
};

struct reserved_address_params {
	void* start_addr;
	size_t reserved_size;
	bool must_use_reserved;
	bool reserved_address_recursive;
#ifdef LOAD_ORDER_RANDOMIZATION
	struct dso *target;
#endif
};

typedef void (*stage3_func)(size_t *, size_t *, size_t *);

static struct builtin_tls {
	char c[8];
	struct pthread pt;
	void *space[16];
} builtin_tls[1];
#define MIN_TLS_ALIGN offsetof(struct builtin_tls, pt)

#define ADDEND_LIMIT 4096
static size_t *saved_addends, *apply_addends_to;
static bool g_is_asan;
static struct dso ldso;
static struct dso *head, *tail, *fini_head, *syms_tail, *lazy_head;
static struct dso_debug_info *debug_tail = NULL;
static char *env_path, *sys_path;
static unsigned long long gencnt;
static int runtime;
static int ldd_mode;
static int ldso_fail;
static int noload;
static int shutting_down;
static jmp_buf *rtld_fail;
static pthread_rwlock_t lock;
static struct debug debug;
static struct tls_module *tls_tail;
static size_t tls_cnt, tls_offset, tls_align = MIN_TLS_ALIGN;
static size_t static_tls_cnt;
static pthread_mutex_t init_fini_lock;
static pthread_cond_t ctor_cond;
static struct dso *builtin_deps[2];
static struct dso *const no_deps[1];
static struct dso *builtin_ctor_queue[4];
static struct dso **main_ctor_queue;
static struct fdpic_loadmap *app_loadmap;
static struct fdpic_dummy_loadmap app_dummy_loadmap;

struct debug *_dl_debug_addr = &debug;

extern hidden int __malloc_replaced;

hidden void (*const __init_array_start)(void)=0, (*const __fini_array_start)(void)=0;

extern hidden void (*const __init_array_end)(void), (*const __fini_array_end)(void);

#ifdef USE_GWP_ASAN
extern bool init_gwp_asan_by_libc(bool force_init);
#endif

weak_alias(__init_array_start, __init_array_end);
weak_alias(__fini_array_start, __fini_array_end);
#ifdef DFX_SIGNAL_LIBC
UT_STATIC void __InstallSignalHandler()
{
}
weak_alias(__InstallSignalHandler, DFX_InstallSignalHandler);
#endif

#ifdef HANDLE_RANDOMIZATION
static int do_dlclose(struct dso *p);
#endif

#ifdef LOAD_ORDER_RANDOMIZATION
static bool map_library_header(struct loadtask *task);
static bool task_map_library(struct loadtask *task, struct reserved_address_params *reserved_params);
static bool resolve_fd_to_realpath(struct loadtask *task);
static bool load_library_header(struct loadtask *task);
static void task_load_library(struct loadtask *task, struct reserved_address_params *reserved_params);
static void preload_direct_deps(struct dso *p, ns_t *namespace, struct loadtasks *tasks);
static void unmap_preloaded_sections(struct loadtasks *tasks);
static void preload_deps(struct dso *p, struct loadtasks *tasks);
static void run_loadtasks(struct loadtasks *tasks, struct reserved_address_params *reserved_params);
UT_STATIC void assign_tls(struct dso *p);
UT_STATIC void load_preload(char *s, ns_t *ns, struct loadtasks *tasks);
static void open_library_by_path(const char *name, const char *s, struct loadtask *task, struct zip_info *z_info);
static void handle_asan_path_open_by_task(int fd, const char *name, ns_t *namespace, struct loadtask *task, struct zip_info *z_info);
#endif

extern int __close(int fd);

/* Sharing relro */
static void handle_relro_sharing(struct dso *p, const dl_extinfo *extinfo, ssize_t *relro_fd_offset);

/* asan path open */
int handle_asan_path_open(int fd, const char *name, ns_t *namespace, char *buf, size_t buf_size);

static void set_bss_vma_name(char *path_name, void *addr, size_t zeromap_size);

static void find_and_set_bss_name(struct dso *p);

/* lldb debug function */
static void sync_with_debugger();
static void notify_addition_to_debugger(struct dso *p);
static void notify_remove_to_debugger(struct dso *p);
static void add_dso_info_to_debug_map(struct dso *p);
static void remove_dso_info_from_debug_map(struct dso *p);

/* add namespace function */
static void get_sys_path(ns_configor *conf);
static void dlclose_ns(struct dso *p);
static bool get_app_path(char *path, size_t size)
{
	int l = 0;
	l = readlink("/proc/self/exe", path, size);
	if (l < 0 || l >= size) {
		LD_LOGD("get_app_path readlink failed!");
		return false;
	}
	path[l] = 0;
	LD_LOGD("get_app_path path:%{public}s.", path);
	return true;
}

static void init_default_namespace(struct dso *app)
{
	ns_t *default_ns = get_default_ns();
	memset(default_ns, 0, sizeof *default_ns);
	ns_set_name(default_ns, NS_DEFAULT_NAME);
	if (env_path) ns_set_env_paths(default_ns, env_path);
	ns_set_lib_paths(default_ns, sys_path);
	ns_set_separated(default_ns, false);
	app->namespace = default_ns;
	ns_add_dso(default_ns, app);
	LD_LOGD("init_default_namespace default_namespace:"
			"nsname: default ,"
			"lib_paths:%{public}s ,"
			"env_path:%{public}s ,"
			"separated: false.",
			sys_path, env_path);
	return;
}

UT_STATIC void set_ns_attrs(ns_t *ns, ns_configor *conf)
{
	if(!ns || !conf) {
		return;
	}

	char *lib_paths, *asan_lib_paths, *permitted_paths, *asan_permitted_paths, *allowed_libs;

	ns_set_separated(ns, conf->get_separated(ns->ns_name));

	lib_paths = conf->get_lib_paths(ns->ns_name);
	if (lib_paths) ns_set_lib_paths(ns, lib_paths);

	asan_lib_paths = conf->get_asan_lib_paths(ns->ns_name);
	if (asan_lib_paths) ns_set_asan_lib_paths(ns, asan_lib_paths);

	permitted_paths = conf->get_permitted_paths(ns->ns_name);
	if (permitted_paths) ns_set_permitted_paths(ns, permitted_paths);

	asan_permitted_paths = conf->get_asan_permitted_paths(ns->ns_name);
	if (asan_permitted_paths) ns_set_asan_permitted_paths(ns, asan_permitted_paths);

	allowed_libs = conf->get_allowed_libs(ns->ns_name);
	if (allowed_libs) ns_set_allowed_libs(ns, allowed_libs);

	LD_LOGD("set_ns_attrs :"
			"ns_name: %{public}s ,"
			"separated:%{public}d ,"
			"lib_paths:%{public}s ,"
			"asan_lib_paths:%{public}s ,"
			"permitted_paths:%{public}s ,"
			"asan_permitted_paths:%{public}s ,"
			"allowed_libs: %{public}s .",
			ns->ns_name, ns->separated, ns->lib_paths, ns->asan_lib_paths, permitted_paths,
			asan_permitted_paths, allowed_libs);
}

UT_STATIC void set_ns_inherits(ns_t *ns, ns_configor *conf)
{
	if(!ns || !conf) {
		return;
	}

	strlist *inherits = conf->get_inherits(ns->ns_name);
	if (inherits) {
		for (size_t i=0; i<inherits->num; i++) {
			ns_t *inherited_ns = find_ns_by_name(inherits->strs[i]);
			if (inherited_ns) {
				char *shared_libs = conf->get_inherit_shared_libs(ns->ns_name, inherited_ns->ns_name);
				ns_add_inherit(ns, inherited_ns, shared_libs);
				LD_LOGD("set_ns_inherits :"
						"ns_name: %{public}s ,"
						"separated:%{public}d ,"
						"lib_paths:%{public}s ,"
						"asan_lib_paths:%{public}s ,",
						inherited_ns->ns_name, inherited_ns->separated, inherited_ns->lib_paths,
						inherited_ns->asan_lib_paths);
			}
		}
		strlist_free(inherits);
	} else {
		LD_LOGD("set_ns_inherits inherits is NULL!");
	}
}

static void init_namespace(struct dso *app)
{
	char app_path[PATH_MAX+1];
	if (!get_app_path(app_path, sizeof app_path)) {
		strcpy(app_path, app->name);
	}
	char *t = strrchr(app_path, '/');
	if (t) {
		*t = 0;
	} else {
		app_path[0] = '.';
		app_path[1] = 0;
	}

	nslist *nsl = nslist_init();
	ns_configor *conf = configor_init();
	char file_path[sizeof "/etc/ld-musl-namespace-" + sizeof (LDSO_ARCH) + sizeof ".ini" + 1] = {0};
	(void)snprintf(file_path, sizeof file_path, "/etc/ld-musl-namespace-%s.ini", LDSO_ARCH);
	LD_LOGI("init_namespace file_path:%{public}s", file_path);
	trace_marker_reset();
	trace_marker_begin(HITRACE_TAG_MUSL, "parse linker config", file_path);
	int ret = conf->parse(file_path, app_path);
	if (ret < 0) {
		LD_LOGE("init_namespace ini file parse failed!");
		/* Init_default_namespace is required even if the ini file parsing fails */
		if (!sys_path) get_sys_path(conf);
		init_default_namespace(app);
		configor_free();
		trace_marker_end(HITRACE_TAG_MUSL);
		return;
	}

	/* sys_path needs to be parsed through ini file */
	if (!sys_path) get_sys_path(conf);
	init_default_namespace(app);

	/* Init default namespace */
	ns_t *d_ns = get_default_ns();
	set_ns_attrs(d_ns, conf);

	/* Init other namespace */
	if (!nsl) {
		LD_LOGE("init nslist fail!");
		configor_free();
		trace_marker_end(HITRACE_TAG_MUSL);
		return;
	}
	strlist *s_ns = conf->get_namespaces();
	if (s_ns) {
		for (size_t i=0; i<s_ns->num; i++) {
			ns_t *ns = ns_alloc();
			ns_set_name(ns, s_ns->strs[i]);
			set_ns_attrs(ns, conf);
			ns_add_dso(ns, app);
			nslist_add_ns(ns);
		}
		strlist_free(s_ns);
	} 
	/* Set inherited namespace */
	set_ns_inherits(d_ns, conf);
	for (size_t i = 0; i < nsl->num; i++) {
		set_ns_inherits(nsl->nss[i], conf);
	}
	configor_free();
	trace_marker_end(HITRACE_TAG_MUSL);
	return;
}

/* Compute load address for a virtual address in a given dso. */
#if DL_FDPIC
void *laddr(const struct dso *p, size_t v)
{
	size_t j=0;
	if (!p->loadmap) return p->base + v;
	for (j=0; v-p->loadmap->segs[j].p_vaddr >= p->loadmap->segs[j].p_memsz; j++);
	return (void *)(v - p->loadmap->segs[j].p_vaddr + p->loadmap->segs[j].addr);
}
static void *laddr_pg(const struct dso *p, size_t v)
{
	size_t j=0;
	size_t pgsz = PAGE_SIZE;
	if (!p->loadmap) return p->base + v;
	for (j=0; ; j++) {
		size_t a = p->loadmap->segs[j].p_vaddr;
		size_t b = a + p->loadmap->segs[j].p_memsz;
		a &= -pgsz;
		b += pgsz-1;
		b &= -pgsz;
		if (v-a<b-a) break;
	}
	return (void *)(v - p->loadmap->segs[j].p_vaddr + p->loadmap->segs[j].addr);
}
static void (*fdbarrier(void *p))()
{
	void (*fd)();
	__asm__("" : "=r"(fd) : "0"(p));
	return fd;
}
#define fpaddr(p, v) fdbarrier((&(struct funcdesc){ \
	laddr(p, v), (p)->got }))
#else
#define laddr(p, v) (void *)((p)->base + (v))
#define laddr_pg(p, v) laddr(p, v)
#define fpaddr(p, v) ((void (*)())laddr(p, v))
#endif

static void decode_vec(size_t *v, size_t *a, size_t cnt)
{
	size_t i;
	for (i=0; i<cnt; i++) a[i] = 0;
	for (; v[0]; v+=2) if (v[0]-1<cnt-1) {
		if (v[0] < 8*sizeof(long)) {
			a[0] |= 1UL<<v[0];
		}
		a[v[0]] = v[1];
	}
}

static int search_vec(size_t *v, size_t *r, size_t key)
{
	for (; v[0]!=key; v+=2)
		if (!v[0]) return 0;
	*r = v[1];
	return 1;
}

UT_STATIC int check_vna_hash(Verdef *def, int16_t vsym, uint32_t vna_hash)
{
	int matched = 0;

	vsym &= 0x7fff;
	Verdef *verdef = def;
	for(;;) {
		if ((verdef->vd_ndx & 0x7fff) == vsym) {
			if (vna_hash == verdef->vd_hash) {
				matched = 1;
			}
			break;
		}
		if (matched) {
			break;
		}
		if (verdef->vd_next == 0) {
			break;
		}
		verdef = (Verdef *)((char *)verdef + verdef->vd_next);
	}
#if (LD_LOG_LEVEL & LD_LOG_DEBUG)
	if (!matched) {
		LD_LOGD("check_vna_hash no matched found. vsym=%{public}d vna_hash=%{public}x", vsym, vna_hash);
	}
#endif
	return matched;
}

UT_STATIC int check_verinfo(Verdef *def, int16_t *versym, uint32_t index, struct verinfo *verinfo, char *strings)
{
	/* if the versym and verinfo is null , then not need version. */
	if (!versym || !def) {
		if (strlen(verinfo->v) == 0) {
			return 1;
		} else {
			LD_LOGD("check_verinfo versym or def is null and verinfo->v exist, s:%{public}s v:%{public}s.",
				verinfo->s, verinfo->v);
			return 0;
		}
	}

	int16_t vsym = versym[index];

	/* find the verneed symbol. */
	if (verinfo->use_vna_hash) {
		if (vsym != VER_NDX_LOCAL && versym != VER_NDX_GLOBAL) {
			return check_vna_hash(def, vsym, verinfo->vna_hash);
		}
	}

	/* if the version length is zero and vsym not less than zero, then library hava default version symbol. */
	if (strlen(verinfo->v) == 0) {
		if (vsym >= 0) {
			return 1;
		} else {
			LD_LOGD("check_verinfo not default version. vsym:%{public}d s:%{public}s", vsym, verinfo->s);
			return 0;
		}
	}

	/* find the version of symbol. */
	vsym &= 0x7fff;
	for (;;) {
		if (!(def->vd_flags & VER_FLG_BASE) && (def->vd_ndx & 0x7fff) == vsym) {
			break;
		}
		if (def->vd_next == 0) {
			return 0;
		}
		def = (Verdef *)((char *)def + def->vd_next);
	}

	Verdaux *aux = (Verdaux *)((char *)def + def->vd_aux);

	int ret = !strcmp(verinfo->v, strings + aux->vda_name);
#if (LD_LOG_LEVEL & LD_LOG_DEBUG)
	if (!ret) {
		LD_LOGD("check_verinfo version not match. s=%{public}s v=%{public}s vsym=%{public}d vda_name=%{public}s",
			verinfo->s, verinfo->v, vsym, strings + aux->vda_name);
	}
#endif
	return ret;
}

static struct sym_info_pair sysv_hash(const char *s0)
{
	struct sym_info_pair s_info_p;
	const unsigned char *s = (void *)s0;
	uint_fast32_t h = 0;
	while (*s) {
		h = 16*h + *s++;
		h ^= h>>24 & 0xf0;
	}
	s_info_p.sym_h = h & 0xfffffff;
	s_info_p.sym_l = (char *)s - s0;
	return s_info_p;
}

struct sym_info_pair gnu_hash(const char *s0)
{
	struct sym_info_pair s_info_p;
	const unsigned char *s = (void *)s0;
	uint_fast32_t h = 5381;
	for (; *s; s++)
		h += h*32 + *s;
	s_info_p.sym_h = h;
	s_info_p.sym_l = (char *)s - s0;
	return s_info_p;
}

static Sym *sysv_lookup(struct verinfo *verinfo,  struct sym_info_pair s_info_p, struct dso *dso)
{
	size_t i;
	uint32_t h = s_info_p.sym_h;
	Sym *syms = dso->syms;
	Elf_Symndx *hashtab = dso->hashtab;
	char *strings = dso->strings;
	for (i=hashtab[2+h%hashtab[0]]; i; i=hashtab[2+hashtab[0]+i]) {
		if ((!dso->versym || (dso->versym[i] & 0x7fff) >= 0)
			&& (!memcmp(verinfo->s, strings+syms[i].st_name, s_info_p.sym_l))) {
			if (!check_verinfo(dso->verdef, dso->versym, i, verinfo, dso->strings)) {
				continue;
			}

			return syms+i;
		}

	}
	LD_LOGD("sysv_lookup not find the symbol, "
		"so:%{public}s s:%{public}s v:%{public}s use_vna_hash:%{public}d vna_hash:%{public}x",
		dso->name, verinfo->s, verinfo->v, verinfo->use_vna_hash, verinfo->vna_hash);
	return 0;
}

static Sym *gnu_lookup(struct sym_info_pair s_info_p, uint32_t *hashtab, struct dso *dso, struct verinfo *verinfo)
{
	uint32_t h1 = s_info_p.sym_h;
	uint32_t nbuckets = hashtab[0];
	uint32_t *buckets = hashtab + 4 + hashtab[2]*(sizeof(size_t)/4);
	uint32_t i = buckets[h1 % nbuckets];

	if (!i) {
		LD_LOGD("gnu_lookup symbol not found (bloom filter), so:%{public}s s:%{public}s", dso->name, verinfo->s);
		return 0;
	}

	uint32_t *hashval = buckets + nbuckets + (i - hashtab[1]);

	for (h1 |= 1; ; i++) {
		uint32_t h2 = *hashval++;
		if ((h1 == (h2|1)) && (!dso->versym || (dso->versym[i] & 0x7fff) >= 0)
			&& !memcmp(verinfo->s, dso->strings + dso->syms[i].st_name, s_info_p.sym_l)) {
			if (!check_verinfo(dso->verdef, dso->versym, i, verinfo, dso->strings)) {
				continue;
			}

			return dso->syms+i;
		}

		if (h2 & 1) break;
	}

	LD_LOGD("gnu_lookup symbol not found, "
		"so:%{public}s s:%{public}s v:%{public}s use_vna_hash:%{public}d vna_hash:%{public}x",
		dso->name, verinfo->s, verinfo->v, verinfo->use_vna_hash, verinfo->vna_hash);
	return 0;
}

static bool check_sym_accessible(struct dso *dso, ns_t *ns)
{
	if (!dso || !dso->namespace || !ns) {
		LD_LOGD("check_sym_accessible invalid parameter!");
		return false;
	}
	if (dso->namespace == ns) {
		return true;
	}
	for (int i = 0; i < dso->parents_count; i++) {
		if (dso->parents[i]->namespace == ns) {
			return true;
		}
	}
	LD_LOGD(
		"check_sym_accessible dso name [%{public}s] ns_name [%{public}s] not accessible!", dso->name, ns->ns_name);
	return false;
}

static inline bool is_dso_accessible(struct dso *dso, ns_t *ns)
{
	if (dso->namespace == ns) {
		return true;
	}
	for (int i = 0; i < dso->parents_count; i++) {
		if (dso->parents[i]->namespace == ns) {
			return true;
		}
	}
	LD_LOGD(
		"check_sym_accessible dso name [%{public}s] ns_name [%{public}s] not accessible!", dso->name, ns->ns_name);
	return false;
}

static int find_dso_parent(struct dso *p, struct dso *target)
{
	int index = -1;
	for (int i = 0; i < p->parents_count; i++) {
		if (p->parents[i] == target) {
			index = i;
			break;
		}
	}
	return index;
}

static void add_dso_parent(struct dso *p, struct dso *parent)
{
	int index = find_dso_parent(p, parent);
	if (index != -1) {
		return;
	}
	if (p->parents_count + 1 > p->parents_capacity) {
		if (p->parents_capacity == 0) {
			p->parents = (struct dso **)malloc(sizeof(struct dso *) * PARENTS_BASE_CAPACITY);
			if (!p->parents) {
				return;
			}
			p->parents_capacity = PARENTS_BASE_CAPACITY;
		} else {
			struct dso ** realloced = (struct dso **)realloc(
				p->parents, sizeof(struct dso *) * (p->parents_capacity + PARENTS_BASE_CAPACITY));
			if (!realloced) {
				return;
			}
			p->parents = realloced;
			p->parents_capacity += PARENTS_BASE_CAPACITY;
		}
	}
	p->parents[p->parents_count] = parent;
	p->parents_count++;
}

static void remove_dso_parent(struct dso *p, struct dso *parent)
{
	int index = find_dso_parent(p, parent);
	if (index == -1) {
		return;
	}
	int i;
	for (i = 0; i < index; i++) {
		p->parents[i] = p->parents[i];
	}
	for (i = index; i < p->parents_count - 1; i++) {
		p->parents[i] = p->parents[i + 1];
	}
	p->parents_count--;
}

static void add_reloc_can_search_dso(struct dso *p, struct dso *can_search_so)
{
	if (p->reloc_can_search_dso_count + 1 > p->reloc_can_search_dso_capacity) {
		if (p->reloc_can_search_dso_capacity == 0) {
			p->reloc_can_search_dso_list =
				(struct dso **)malloc(sizeof(struct dso *) * RELOC_CAN_SEARCH_DSO_BASE_CAPACITY);
			if (!p->reloc_can_search_dso_list) {
				return;
			}
			p->reloc_can_search_dso_capacity = RELOC_CAN_SEARCH_DSO_BASE_CAPACITY;
		} else {
			struct dso ** realloced = (struct dso **)realloc(
				p->reloc_can_search_dso_list,
				sizeof(struct dso *) * (p->reloc_can_search_dso_capacity + RELOC_CAN_SEARCH_DSO_BASE_CAPACITY));
			if (!realloced) {
				return;
			}
			p->reloc_can_search_dso_list = realloced;
			p->reloc_can_search_dso_capacity += RELOC_CAN_SEARCH_DSO_BASE_CAPACITY;
		}
	}
	p->reloc_can_search_dso_list[p->reloc_can_search_dso_count] = can_search_so;
	p->reloc_can_search_dso_count++;
}

static void free_reloc_can_search_dso(struct dso *p)
{
	if (p->reloc_can_search_dso_list) {
		free(p->reloc_can_search_dso_list);
		p->reloc_can_search_dso_list = NULL;
		p->reloc_can_search_dso_count = 0;
		p->reloc_can_search_dso_capacity = 0;
	}
}

/* The list of so that can be accessed during relocation include:
 * - The is_global flag of the so is true which means accessible by default.
 *   Global so includes exe, ld preload so and ldso.
 * - We only check whether ns is accessible for the so if is_reloc_head_so_dep is true.
 *
 *   How to set is_reloc_head_so_dep:
 *   When dlopen A, we set is_reloc_head_so_dep to true for
 *   all direct and indirect dependent sos of A, including A itself. */
static void add_can_search_so_list_in_dso(struct dso *dso_relocating, struct dso *start_check_dso) {
	struct dso *p = start_check_dso;
	for (; p; p = p->syms_next) {
		if (p->is_global) {
			add_reloc_can_search_dso(dso_relocating, p);
			continue;
		}

		if (p->is_reloc_head_so_dep) {
			if (dso_relocating->namespace && check_sym_accessible(p, dso_relocating->namespace)) {
				add_reloc_can_search_dso(dso_relocating, p);
			}
		}
	}

	return;
}

#define OK_TYPES (1<<STT_NOTYPE | 1<<STT_OBJECT | 1<<STT_FUNC | 1<<STT_COMMON | 1<<STT_TLS)
#define OK_BINDS (1<<STB_GLOBAL | 1<<STB_WEAK | 1<<STB_GNU_UNIQUE)

#ifndef ARCH_SYM_REJECT_UND
#define ARCH_SYM_REJECT_UND(s) 0
#endif

#if defined(__GNUC__)
__attribute__((always_inline))
#endif

struct symdef find_sym_impl(
	struct dso *dso, struct verinfo *verinfo, struct sym_info_pair s_info_g, int need_def, ns_t *ns)
{
	Sym *sym;
	struct sym_info_pair s_info_s = {0, 0};
	uint32_t *ght;
	uint32_t gh = s_info_g.sym_h;
	uint32_t gho = gh / (8 * sizeof(size_t));
	size_t ghm = 1ul << gh % (8 * sizeof(size_t));
	struct symdef def = {0};
	if (ns && !check_sym_accessible(dso, ns))
		return def;

	if ((ght = dso->ghashtab)) {
		const size_t *bloomwords = (const void *)(ght + 4);
		size_t f = bloomwords[gho & (ght[2] - 1)];
		if (!(f & ghm))
			return def;

		f >>= (gh >> ght[3]) % (8 * sizeof f);
		if (!(f & 1))
			return def;

		sym = gnu_lookup(s_info_g, ght, dso, verinfo);
	} else {
		if (!s_info_s.sym_h)
			s_info_s = sysv_hash(verinfo->s);

		sym = sysv_lookup(verinfo, s_info_s, dso);
	}

	if (!sym)
		return def;

	if (!sym->st_shndx)
		if (need_def || (sym->st_info & 0xf) == STT_TLS || ARCH_SYM_REJECT_UND(sym))
			return def;

	if (!sym->st_value)
		if ((sym->st_info & 0xf) != STT_TLS)
			return def;

	if (!(1 << (sym->st_info & 0xf) & OK_TYPES))
		return def;

	if (!(1 << (sym->st_info >> 4) & OK_BINDS))
		return def;

	def.sym = sym;
	def.dso = dso;
	return def;
}

static inline struct symdef find_sym2(struct dso *dso, struct verinfo *verinfo, int need_def, int use_deps, ns_t *ns)
{
	struct sym_info_pair s_info_g = gnu_hash(verinfo->s);
	struct sym_info_pair s_info_s = {0, 0};
	uint32_t gh = s_info_g.sym_h, gho = gh / (8*sizeof(size_t)), *ght;
	size_t ghm = 1ul << gh % (8*sizeof(size_t));
	struct symdef def = {0};
	struct dso **deps = use_deps ? dso->deps : 0;
	for (; dso; dso=use_deps ? *deps++ : dso->syms_next) {
		Sym *sym;
		if (ns && !check_sym_accessible(dso, ns)) {
			continue;
		}
		if ((ght = dso->ghashtab)) {
			GNU_HASH_FILTER(ght, ghm, gho)
			sym = gnu_lookup(s_info_g, ght, dso, verinfo);
		} else {
			if (!s_info_s.sym_h) s_info_s = sysv_hash(verinfo->s);
			sym = sysv_lookup(verinfo, s_info_s, dso);
		}

		if (!sym) continue;
		if (!sym->st_shndx)
			if (need_def || (sym->st_info&0xf) == STT_TLS
				|| ARCH_SYM_REJECT_UND(sym))
				continue;
		if (!sym->st_value)
			if ((sym->st_info&0xf) != STT_TLS)
				continue;
		if (!(1<<(sym->st_info&0xf) & OK_TYPES)) continue;
		if (!(1<<(sym->st_info>>4) & OK_BINDS)) continue;
		def.sym = sym;
		def.dso = dso;
		break;
	}
	return def;
}

static inline struct symdef find_sym_by_deps(struct dso *dso, struct verinfo *verinfo, int need_def, ns_t *ns)
{
	struct sym_info_pair s_info_g = gnu_hash(verinfo->s);
	struct sym_info_pair s_info_s = {0, 0};
	uint32_t h = 0, gh = s_info_g.sym_h, gho = gh / (8*sizeof(size_t)), *ght;
	size_t ghm = 1ul << gh % (8*sizeof(size_t));
	struct symdef def = {0};
	struct dso **deps = dso->deps;
	for (; dso; dso=*deps++) {
		Sym *sym;
		if (!is_dso_accessible(dso, ns)) {
			continue;
		}
		if ((ght = dso->ghashtab)) {
			GNU_HASH_FILTER(ght, ghm, gho)
			sym = gnu_lookup(s_info_g, ght, dso, verinfo);
		} else {
			if (!s_info_s.sym_h) s_info_s = sysv_hash(verinfo->s);
			sym = sysv_lookup(verinfo, s_info_s, dso);
		}

		if (!sym) continue;
		if (!sym->st_shndx)
			if (need_def || (sym->st_info&0xf) == STT_TLS
				|| ARCH_SYM_REJECT_UND(sym))
				continue;
		if (!sym->st_value)
			if ((sym->st_info&0xf) != STT_TLS)
				continue;
		if (!(1<<(sym->st_info&0xf) & OK_TYPES)) continue;
		if (!(1<<(sym->st_info>>4) & OK_BINDS)) continue;
		def.sym = sym;
		def.dso = dso;
		break;
	}
	return def;
}

static inline struct symdef find_sym_by_saved_so_list(
	int sym_type, struct dso *dso, struct verinfo *verinfo, int need_def, struct dso *dso_relocating)
{
	struct sym_info_pair s_info_g = gnu_hash(verinfo->s);
	struct sym_info_pair s_info_s = {0, 0};
	uint32_t gh = s_info_g.sym_h, gho = gh / (8 * sizeof(size_t)), *ght;
	size_t ghm = 1ul << gh % (8 * sizeof(size_t));
	struct symdef def = {0};
	// skip head dso.
	int start_search_index = sym_type==REL_COPY ? 1 : 0;
	struct dso *dso_searching = 0;
	for (int i = start_search_index; i < dso_relocating->reloc_can_search_dso_count; i++) {
		dso_searching = dso_relocating->reloc_can_search_dso_list[i];
		Sym *sym;
		if ((ght = dso_searching->ghashtab)) {
			GNU_HASH_FILTER(ght, ghm, gho)
			sym = gnu_lookup(s_info_g, ght, dso_searching, verinfo);
		} else {
			if (!s_info_s.sym_h) s_info_s = sysv_hash(verinfo->s);
			sym = sysv_lookup(verinfo, s_info_s, dso_searching);
		}
		if (!sym) continue;
		if (!sym->st_shndx)
			if (need_def || (sym->st_info&0xf) == STT_TLS
				|| ARCH_SYM_REJECT_UND(sym))
				continue;
		if (!sym->st_value)
			if ((sym->st_info&0xf) != STT_TLS)
				continue;
		if (!(1<<(sym->st_info&0xf) & OK_TYPES)) continue;
		if (!(1<<(sym->st_info>>4) & OK_BINDS)) continue;
		def.sym = sym;
		def.dso = dso_searching;
		break;
	}
	return def;
}

static struct symdef find_sym(struct dso *dso, const char *s, int need_def)
{
	struct verinfo verinfo = { .s = s, .v = "", .use_vna_hash = false };
	return find_sym2(dso, &verinfo, need_def, 0, NULL);
}

static bool get_vna_hash(struct dso *dso, int sym_index, uint32_t *vna_hash)
{
	if (!dso->versym || !dso->verneed) {
		return false;
	}

	uint16_t vsym = dso->versym[sym_index];
	if (vsym == VER_NDX_LOCAL || vsym == VER_NDX_GLOBAL) {
		return false;
	}

	bool result = false;
	Verneed *verneed = dso->verneed;
	Vernaux *vernaux;
	vsym &= 0x7fff;

	for(;;) {
		vernaux = (Vernaux *)((char *)verneed + verneed->vn_aux);

		for (size_t cnt = 0; cnt < verneed->vn_cnt; cnt++) {
			if ((vernaux->vna_other & 0x7fff) == vsym) {
				result = true;
				*vna_hash = vernaux->vna_hash;
				break;
			}

			vernaux = (Vernaux *)((char *)vernaux + vernaux->vna_next);
		}

		if (result) {
			break;
		}

		if (verneed->vn_next == 0) {
			break;
		}

		verneed = (Verneed *)((char *)verneed + verneed->vn_next);
	}
	return result;
}

static void get_verinfo(struct dso *dso, int sym_index, struct verinfo *vinfo)
{
	char *strings = dso->strings;
	// try to get version number from .gnu.version
	int16_t vsym = dso->versym[sym_index];
	Verdef *verdef = dso->verdef;
	vsym &= 0x7fff;
	if (!verdef) {
		return;
	}
	int version_found = 0;
	for (;;) {
		if (!verdef) {
			break;
		}
		if (!(verdef->vd_flags & VER_FLG_BASE) && (verdef->vd_ndx & 0x7fff) == vsym) {
			version_found = 1;
			break;
		}
		if (verdef->vd_next == 0) {
			break;
		}
		verdef = (Verdef *)((char *)verdef + verdef->vd_next);
	}
	if (version_found) {
		Verdaux *aux = (Verdaux *)((char *)verdef + verdef->vd_aux);
		if (aux && aux->vda_name && strings && (dso->strings + aux->vda_name)) {
			vinfo->v = dso->strings + aux->vda_name;
		}
	}
}

static void do_relocs(struct dso *dso, size_t *rel, size_t rel_size, size_t stride)
{
	unsigned char *base = dso->base;
	Sym *syms = dso->syms;
	char *strings = dso->strings;
	Sym *sym;
	const char *name;
	void *ctx;
	int type;
	int sym_index;
	struct symdef def;
	size_t *reloc_addr;
	size_t sym_val;
	size_t tls_val;
	size_t addend;
	int skip_relative = 0, reuse_addends = 0, save_slot = 0;

	if (dso == &ldso) {
		/* Only ldso's REL table needs addend saving/reuse. */
		if (rel == apply_addends_to)
			reuse_addends = 1;
		skip_relative = 1;
	}

	for (; rel_size; rel+=stride, rel_size-=stride*sizeof(size_t)) {
		if (skip_relative && IS_RELATIVE(rel[1], dso->syms)) continue;
		type = R_TYPE(rel[1]);
		if (type == REL_NONE) continue;
		reloc_addr = laddr(dso, rel[0]);

		if (stride > 2) {
			addend = rel[2];
		} else if (type==REL_GOT || type==REL_PLT|| type==REL_COPY) {
			addend = 0;
		} else if (reuse_addends) {
			/* Save original addend in stage 2 where the dso
			 * chain consists of just ldso; otherwise read back
			 * saved addend since the inline one was clobbered. */
			if (head==&ldso)
				saved_addends[save_slot] = *reloc_addr;
			addend = saved_addends[save_slot++];
		} else {
			addend = *reloc_addr;
		}

		sym_index = R_SYM(rel[1]);
		if (sym_index) {
			sym = syms + sym_index;
			name = strings + sym->st_name;
			ctx = type==REL_COPY ? head->syms_next : head;
			struct verinfo vinfo = { .s = name, .v = ""};

			vinfo.use_vna_hash = get_vna_hash(dso, sym_index, &vinfo.vna_hash);
			if (!vinfo.use_vna_hash && dso->versym && (dso->versym[sym_index] & 0x7fff) >= 0) {
				get_verinfo(dso, sym_index, &vinfo);
			}
			if (dso->cache_sym_index == sym_index) {
				def = (struct symdef){ .dso = dso->cache_dso, .sym = dso->cache_sym };
			} else {
				def = (sym->st_info>>4) == STB_LOCAL
					? (struct symdef){ .dso = dso, .sym = sym }
					: dso != &ldso ? find_sym_by_saved_so_list(type, ctx, &vinfo, type==REL_PLT, dso)
					: find_sym2(ctx, &vinfo, type==REL_PLT, 0, dso->namespace);
				dso->cache_sym_index = sym_index;
				dso->cache_dso = def.dso;
				dso->cache_sym = def.sym;
			}

			if (!def.sym && (sym->st_shndx != SHN_UNDEF
				|| sym->st_info>>4 != STB_WEAK)) {
				if (dso->lazy && (type==REL_PLT || type==REL_GOT)) {
					dso->lazy[3*dso->lazy_cnt+0] = rel[0];
					dso->lazy[3*dso->lazy_cnt+1] = rel[1];
					dso->lazy[3*dso->lazy_cnt+2] = addend;
					dso->lazy_cnt++;
					continue;
				}
				LD_LOGE("relocating failed: symbol not found. "
					"dso=%{public}s s=%{public}s use_vna_hash=%{public}d van_hash=%{public}x",
					dso->name, name, vinfo.use_vna_hash, vinfo.vna_hash);
				error("Error relocating %s: %s: symbol not found",
					dso->name, name);
				if (runtime) longjmp(*rtld_fail, 1);
				continue;
			}
		} else {
			sym = 0;
			def.sym = 0;
			def.dso = dso;
		}

		sym_val = def.sym ? (size_t)laddr(def.dso, def.sym->st_value) : 0;
		tls_val = def.sym ? def.sym->st_value : 0;

		if ((type == REL_TPOFF || type == REL_TPOFF_NEG)
			&& def.dso->tls_id > static_tls_cnt) {
			error("Error relocating %s: %s: initial-exec TLS "
				"resolves to dynamic definition in %s",
				dso->name, name, def.dso->name);
			longjmp(*rtld_fail, 1);
		}

		switch(type) {
		case REL_OFFSET:
			addend -= (size_t)reloc_addr;
		case REL_SYMBOLIC:
		case REL_GOT:
		case REL_PLT:
			*reloc_addr = sym_val + addend;
			break;
		case REL_USYMBOLIC:
			memcpy(reloc_addr, &(size_t){sym_val + addend}, sizeof(size_t));
			break;
		case REL_RELATIVE:
			*reloc_addr = (size_t)base + addend;
			break;
		case REL_SYM_OR_REL:
			if (sym) *reloc_addr = sym_val + addend;
			else *reloc_addr = (size_t)base + addend;
			break;
		case REL_COPY:
			memcpy(reloc_addr, (void *)sym_val, sym->st_size);
			break;
		case REL_OFFSET32:
			*(uint32_t *)reloc_addr = sym_val + addend
				- (size_t)reloc_addr;
			break;
		case REL_FUNCDESC:
			*reloc_addr = def.sym ? (size_t)(def.dso->funcdescs
				+ (def.sym - def.dso->syms)) : 0;
			break;
		case REL_FUNCDESC_VAL:
			if ((sym->st_info&0xf) == STT_SECTION) *reloc_addr += sym_val;
			else *reloc_addr = sym_val;
			reloc_addr[1] = def.sym ? (size_t)def.dso->got : 0;
			break;
		case REL_DTPMOD:
			*reloc_addr = def.dso->tls_id;
			break;
		case REL_DTPOFF:
			*reloc_addr = tls_val + addend - DTP_OFFSET;
			break;
#ifdef TLS_ABOVE_TP
		case REL_TPOFF:
			*reloc_addr = tls_val + def.dso->tls.offset + TPOFF_K + addend;
			break;
#else
		case REL_TPOFF:
			*reloc_addr = tls_val - def.dso->tls.offset + addend;
			break;
		case REL_TPOFF_NEG:
			*reloc_addr = def.dso->tls.offset - tls_val + addend;
			break;
#endif
		case REL_TLSDESC:
			if (stride<3) addend = reloc_addr[1];
			if (def.dso->tls_id > static_tls_cnt) {
				struct td_index *new = malloc(sizeof *new);
				if (!new) {
					error(
					"Error relocating %s: cannot allocate TLSDESC for %s",
					dso->name, sym ? name : "(local)" );
					longjmp(*rtld_fail, 1);
				}
				new->next = dso->td_index;
				dso->td_index = new;
				new->args[0] = def.dso->tls_id;
				new->args[1] = tls_val + addend - DTP_OFFSET;
				reloc_addr[0] = (size_t)__tlsdesc_dynamic;
				reloc_addr[1] = (size_t)new;
			} else {
				reloc_addr[0] = (size_t)__tlsdesc_static;
#ifdef TLS_ABOVE_TP
				reloc_addr[1] = tls_val + def.dso->tls.offset
					+ TPOFF_K + addend;
#else
				reloc_addr[1] = tls_val - def.dso->tls.offset
					+ addend;
#endif
			}
#ifdef TLSDESC_BACKWARDS
			/* Some archs (32-bit ARM at least) invert the order of
			 * the descriptor members. Fix them up here. */
			size_t tmp = reloc_addr[0];
			reloc_addr[0] = reloc_addr[1];
			reloc_addr[1] = tmp;
#endif
			break;
		default:
			error("Error relocating %s: unsupported relocation type %d",
				dso->name, type);
			if (runtime) longjmp(*rtld_fail, 1);
			continue;
		}
	}
}

static void redo_lazy_relocs()
{
	struct dso *p = lazy_head, *next;
	lazy_head = 0;
	for (; p; p=next) {
		next = p->lazy_next;
		size_t size = p->lazy_cnt*3*sizeof(size_t);
		p->lazy_cnt = 0;
		do_relocs(p, p->lazy, size, 3);
		if (p->lazy_cnt) {
			p->lazy_next = lazy_head;
			lazy_head = p;
		} else {
			free(p->lazy);
			p->lazy = 0;
			p->lazy_next = 0;
		}
	}
}

/* A huge hack: to make up for the wastefulness of shared libraries
 * needing at least a page of dirty memory even if they have no global
 * data, we reclaim the gaps at the beginning and end of writable maps
 * and "donate" them to the heap. */

static void reclaim(struct dso *dso, size_t start, size_t end)
{
	if (start >= dso->relro_start && start < dso->relro_end) start = dso->relro_end;
	if (end   >= dso->relro_start && end   < dso->relro_end) end = dso->relro_start;
	if (start >= end) return;
	char *base = laddr_pg(dso, start);
	__malloc_donate(base, base+(end-start));
}

static void reclaim_gaps(struct dso *dso)
{
	Phdr *ph = dso->phdr;
	size_t phcnt = dso->phnum;

	for (; phcnt--; ph=(void *)((char *)ph+dso->phentsize)) {
		if (ph->p_type!=PT_LOAD) continue;
		if ((ph->p_flags&(PF_R|PF_W))!=(PF_R|PF_W)) continue;
		reclaim(dso, ph->p_vaddr & -PAGE_SIZE, ph->p_vaddr);
		reclaim(dso, ph->p_vaddr+ph->p_memsz,
			ph->p_vaddr+ph->p_memsz+PAGE_SIZE-1 & -PAGE_SIZE);
	}
}

static ssize_t read_loop(int fd, void *p, size_t n)
{
	for (size_t i=0; i<n; ) {
		ssize_t l = read(fd, (char *)p+i, n-i);
		if (l<0) {
			if (errno==EINTR) continue;
			else return -1;
		}
		if (l==0) return i;
		i += l;
	}
	return n;
}

UT_STATIC void *mmap_fixed(void *p, size_t n, int prot, int flags, int fd, off_t off)
{
	static int no_map_fixed;
	char *q;
	if (!n) return p;
	if (!no_map_fixed) {
		q = mmap(p, n, prot, flags|MAP_FIXED, fd, off);
		if (!DL_NOMMU_SUPPORT || q != MAP_FAILED || errno != EINVAL)
			return q;
		no_map_fixed = 1;
	}
	/* Fallbacks for MAP_FIXED failure on NOMMU kernels. */
	if (flags & MAP_ANONYMOUS) {
		memset(p, 0, n);
		return p;
	}
	ssize_t r;
	if (lseek(fd, off, SEEK_SET) < 0) return MAP_FAILED;
	for (q=p; n; q+=r, off+=r, n-=r) {
		r = read(fd, q, n);
		if (r < 0 && errno != EINTR) return MAP_FAILED;
		if (!r) {
			memset(q, 0, n);
			break;
		}
	}
	return p;
}

UT_STATIC void unmap_library(struct dso *dso)
{
	if (dso->loadmap) {
		size_t i;
		for (i=0; i<dso->loadmap->nsegs; i++) {
			if (!dso->loadmap->segs[i].p_memsz)
				continue;
			if (!is_dlclose_debug_enable()) {
				munmap((void *)dso->loadmap->segs[i].addr,
					dso->loadmap->segs[i].p_memsz);
			} else {
				(void)mprotect((void *)dso->loadmap->segs[i].addr,
					dso->loadmap->segs[i].p_memsz, PROT_NONE);
			}
		}
		free(dso->loadmap);
	} else if (dso->map && dso->map_len) {
		if (!is_dlclose_debug_enable()) {
			munmap(dso->map, dso->map_len);
		} else {
			mprotect(dso->map, dso->map_len, PROT_NONE);
		}
	}
}

UT_STATIC bool get_random(void *buf, size_t buflen)
{
	int ret;
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0) {
		return false;
	}

	ret = read(fd, buf, buflen);
	if (ret < 0) {
		close(fd);
		return false;
	}

	close(fd);
	return true;
}

UT_STATIC void fill_random_data(void *buf, size_t buflen)
{
	uint64_t x;
	int i;
	int pos = 0;
	struct timespec ts;
	/* Try to use urandom to get the random number first */
	if (!get_random(buf, buflen)) {
		/* Can't get random number from /dev/urandom, generate from addr based on ASLR and time */
		for (i = 1; i <= (buflen / sizeof(x)); i++) {
			(void)clock_gettime(CLOCK_REALTIME, &ts);
			x = (((uint64_t)get_random) << 32) ^ (uint64_t)fill_random_data ^ ts.tv_nsec;
			memcpy((char *)buf + pos, &x, sizeof(x));
			pos += sizeof(x);
		}
	}
	return;
}

static bool get_transparent_hugepages_supported(void)
{
	int fd = -1;
	ssize_t read_size = 0;
	bool enable = false;
	char buf[HUGEPAGES_SUPPORTED_STR_SIZE] = {'0'};

	fd = open("/sys/kernel/mm/transparent_hugepage/enabled", O_RDONLY);
	if (fd < 0)
		goto done;

	read_size = read(fd, buf, HUGEPAGES_SUPPORTED_STR_SIZE - 1);
	if (read_size < 0)
		goto close_fd;

	buf[HUGEPAGES_SUPPORTED_STR_SIZE - 1] = '\0';
	if (strstr(buf, "[never]") == NULL)
		enable = true;

close_fd:
	close(fd);
done:
	return enable;
}

static size_t phdr_table_get_maxinum_alignment(Phdr *phdr_table, size_t phdr_count)
{
#if defined(__LP64__)
	size_t maxinum_alignment = PAGE_SIZE;
	size_t i = 0;

	for (i = 0; i < phdr_count; ++i) {
		const Phdr *phdr = &phdr_table[i];

		/* p_align must be 0, 1, or a positive, integral power of two */
		if ((phdr->p_type != PT_LOAD) || ((phdr->p_align & (phdr->p_align - 1)) != 0))
			continue;

		if (phdr->p_align > maxinum_alignment)
			maxinum_alignment = phdr->p_align;
	}

	return maxinum_alignment;
#else
	return PAGE_SIZE;
#endif
}

UT_STATIC void *map_library(int fd, struct dso *dso, struct reserved_address_params *reserved_params)
{
	Ehdr buf[(896+sizeof(Ehdr))/sizeof(Ehdr)];
	void *allocated_buf=0;
	size_t phsize;
	size_t addr_min=SIZE_MAX, addr_max=0, map_len;
	size_t this_min, this_max;
	size_t nsegs = 0;
	off_t off_start;
	Ehdr *eh;
	Phdr *ph, *ph0;
	unsigned prot;
	unsigned char *map=MAP_FAILED, *base;
	size_t dyn=0;
	size_t tls_image=0;
	size_t i;
	int map_flags = MAP_PRIVATE;
	size_t start_addr;
	size_t start_alignment = PAGE_SIZE;
	bool hugepage_enabled = false;

	ssize_t l = read(fd, buf, sizeof buf);
	eh = buf;
	if (l<0) return 0;
	if (l<sizeof *eh || (eh->e_type != ET_DYN && eh->e_type != ET_EXEC))
		goto noexec;
	phsize = eh->e_phentsize * eh->e_phnum;
	if (phsize > sizeof buf - sizeof *eh) {
		allocated_buf = malloc(phsize);
		if (!allocated_buf) return 0;
		l = pread(fd, allocated_buf, phsize, eh->e_phoff);
		if (l < 0) goto error;
		if (l != phsize) goto noexec;
		ph = ph0 = allocated_buf;
	} else if (eh->e_phoff + phsize > l) {
		l = pread(fd, buf+1, phsize, eh->e_phoff);
		if (l < 0) goto error;
		if (l != phsize) goto noexec;
		ph = ph0 = (void *)(buf + 1);
	} else {
		ph = ph0 = (void *)((char *)buf + eh->e_phoff);
	}
	for (i=eh->e_phnum; i; i--, ph=(void *)((char *)ph+eh->e_phentsize)) {
		if (ph->p_type == PT_DYNAMIC) {
			dyn = ph->p_vaddr;
		} else if (ph->p_type == PT_TLS) {
			tls_image = ph->p_vaddr;
			dso->tls.align = ph->p_align;
			dso->tls.len = ph->p_filesz;
			dso->tls.size = ph->p_memsz;
		} else if (ph->p_type == PT_GNU_RELRO) {
			dso->relro_start = ph->p_vaddr & -PAGE_SIZE;
			dso->relro_end = (ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE;
		} else if (ph->p_type == PT_GNU_STACK) {
			if (!runtime && ph->p_memsz > __default_stacksize) {
				__default_stacksize =
					ph->p_memsz < DEFAULT_STACK_MAX ?
					ph->p_memsz : DEFAULT_STACK_MAX;
			}
		}
		if (ph->p_type != PT_LOAD) continue;
		nsegs++;
		if (ph->p_vaddr < addr_min) {
			addr_min = ph->p_vaddr;
			off_start = ph->p_offset;
			prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
				((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
				((ph->p_flags&PF_X) ? PROT_EXEC : 0));
		}
		if (ph->p_vaddr+ph->p_memsz > addr_max) {
			addr_max = ph->p_vaddr+ph->p_memsz;
		}
	}
	if (!dyn) goto noexec;
	if (DL_FDPIC && !(eh->e_flags & FDPIC_CONSTDISP_FLAG)) {
		dso->loadmap = calloc(1, sizeof *dso->loadmap
			+ nsegs * sizeof *dso->loadmap->segs);
		if (!dso->loadmap) goto error;
		dso->loadmap->nsegs = nsegs;
		for (ph=ph0, i=0; i<nsegs; ph=(void *)((char *)ph+eh->e_phentsize)) {
			if (ph->p_type != PT_LOAD) continue;
			prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
				((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
				((ph->p_flags&PF_X) ? PROT_EXEC : 0));
			map = mmap(0, ph->p_memsz + (ph->p_vaddr & PAGE_SIZE-1),
				prot, MAP_PRIVATE,
				fd, ph->p_offset & -PAGE_SIZE);
			if (map == MAP_FAILED) {
				unmap_library(dso);
				goto error;
			}
			dso->loadmap->segs[i].addr = (size_t)map +
				(ph->p_vaddr & PAGE_SIZE-1);
			dso->loadmap->segs[i].p_vaddr = ph->p_vaddr;
			dso->loadmap->segs[i].p_memsz = ph->p_memsz;
			i++;
			if (prot & PROT_WRITE) {
				size_t brk = (ph->p_vaddr & PAGE_SIZE-1)
					+ ph->p_filesz;
				size_t pgbrk = brk + PAGE_SIZE-1 & -PAGE_SIZE;
				size_t pgend = brk + ph->p_memsz - ph->p_filesz
					+ PAGE_SIZE-1 & -PAGE_SIZE;
				if (pgend > pgbrk && mmap_fixed(map+pgbrk,
					pgend-pgbrk, prot,
					MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,
					-1, off_start) == MAP_FAILED)
					goto error;
				memset(map + brk, 0, pgbrk-brk);
			}
		}
		map = (void *)dso->loadmap->segs[0].addr;
		map_len = 0;
		goto done_mapping;
	}
	addr_max += PAGE_SIZE-1;
	addr_max &= -PAGE_SIZE;
	addr_min &= -PAGE_SIZE;
	off_start &= -PAGE_SIZE;
	map_len = addr_max - addr_min + off_start;
	start_addr = addr_min;

	hugepage_enabled = get_transparent_hugepages_supported();
	if (hugepage_enabled) {
		size_t maxinum_alignment = phdr_table_get_maxinum_alignment(ph0, eh->e_phnum);

		start_alignment = maxinum_alignment == KPMD_SIZE ? KPMD_SIZE : PAGE_SIZE;
	}

	if (reserved_params) {
		if (map_len > reserved_params->reserved_size) {
			if (reserved_params->must_use_reserved) {
				goto error;
			}
		} else {
			start_addr = ((size_t)reserved_params->start_addr - 1 + PAGE_SIZE) & -PAGE_SIZE;
			map_flags |= MAP_FIXED;
		}
	}

	/* we will find a mapping_align aligned address as the start of dso
	 * so we need a tmp_map_len as map_len + mapping_align to make sure
	 * we have enough space to shift the dso to the correct location. */
	size_t mapping_align = start_alignment > LIBRARY_ALIGNMENT ? start_alignment : LIBRARY_ALIGNMENT;
	size_t tmp_map_len = ALIGN(map_len, mapping_align) + mapping_align - PAGE_SIZE;

	/* if reserved_params exists, we should use start_addr as prefered result to do the mmap operation */
	if (reserved_params) {
		map = DL_NOMMU_SUPPORT
			? mmap((void *)start_addr, map_len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
			: mmap((void *)start_addr, map_len, prot, map_flags, fd, off_start);
		if (map == MAP_FAILED) {
			goto error;
		}
		if (reserved_params && map_len < reserved_params->reserved_size) {
			reserved_params->reserved_size -= (map_len + (start_addr - (size_t)reserved_params->start_addr));
			reserved_params->start_addr = (void *)((uint8_t *)map + map_len);
		}
	/* if reserved_params does not exist, we should use real_map as prefered result to do the mmap operation */
	} else {
		/* use tmp_map_len to mmap enough space for the dso with anonymous mapping */
		unsigned char *temp_map = mmap((void *)NULL, tmp_map_len, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (temp_map == MAP_FAILED) {
			goto error;
		}

		/* find the mapping_align aligned address */
		unsigned char *real_map = (unsigned char*)ALIGN((uintptr_t)temp_map, mapping_align);

		/* mummap the space we mmap before so that we can mmap correct space again */
		munmap(temp_map, tmp_map_len);

		map = DL_NOMMU_SUPPORT
			? mmap(real_map, map_len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
			/* use map_len to mmap correct space for the dso with file mapping */
			: mmap(real_map, map_len, prot, map_flags, fd, off_start);
		if (map == MAP_FAILED) {
			goto error;
		}
	}
	dso->map = map;
	dso->map_len = map_len;
	/* If the loaded file is not relocatable and the requested address is
	 * not available, then the load operation must fail. */
	if (eh->e_type != ET_DYN && addr_min && map!=(void *)addr_min) {
		errno = EBUSY;
		goto error;
	}
	base = map - addr_min;
	dso->phdr = 0;
	dso->phnum = 0;
	for (ph=ph0, i=eh->e_phnum; i; i--, ph=(void *)((char *)ph+eh->e_phentsize)) {
		if (ph->p_type == PT_OHOS_RANDOMDATA) {
			fill_random_data((void *)(ph->p_vaddr + base), ph->p_memsz);
			continue;
		}
		if (ph->p_type != PT_LOAD) continue;
		/* Check if the programs headers are in this load segment, and
		 * if so, record the address for use by dl_iterate_phdr. */
		if (!dso->phdr && eh->e_phoff >= ph->p_offset
			&& eh->e_phoff+phsize <= ph->p_offset+ph->p_filesz) {
			dso->phdr = (void *)(base + ph->p_vaddr
				+ (eh->e_phoff-ph->p_offset));
			dso->phnum = eh->e_phnum;
			dso->phentsize = eh->e_phentsize;
		}
		this_min = ph->p_vaddr & -PAGE_SIZE;
		this_max = ph->p_vaddr+ph->p_memsz+PAGE_SIZE-1 & -PAGE_SIZE;
		off_start = ph->p_offset & -PAGE_SIZE;
		prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
			((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
			((ph->p_flags&PF_X) ? PROT_EXEC : 0));
		/* Reuse the existing mapping for the lowest-address LOAD */
		if ((ph->p_vaddr & -PAGE_SIZE) != addr_min || DL_NOMMU_SUPPORT)
			if (mmap_fixed(base+this_min, this_max-this_min, prot, MAP_PRIVATE|MAP_FIXED, fd, off_start) == MAP_FAILED)
				goto error;
		if ((ph->p_flags & PF_X) && (ph->p_align == KPMD_SIZE) && hugepage_enabled)
			madvise(base + this_min, this_max - this_min, MADV_HUGEPAGE);
		if (ph->p_memsz > ph->p_filesz && (ph->p_flags&PF_W)) {
			size_t brk = (size_t)base+ph->p_vaddr+ph->p_filesz;
			size_t pgbrk = brk+PAGE_SIZE-1 & -PAGE_SIZE;
			size_t zeromap_size = (size_t)base+this_max-pgbrk;
			memset((void *)brk, 0, pgbrk-brk & PAGE_SIZE-1);
			if (pgbrk-(size_t)base < this_max && mmap_fixed((void *)pgbrk, zeromap_size, prot, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) == MAP_FAILED)
				goto error;
			set_bss_vma_name(dso->name, (void *)pgbrk, zeromap_size);
		}
	}
	for (i=0; ((size_t *)(base+dyn))[i]; i+=2)
		if (((size_t *)(base+dyn))[i]==DT_TEXTREL) {
			if (mprotect(map, map_len, PROT_READ|PROT_WRITE|PROT_EXEC)
				&& errno != ENOSYS)
				goto error;
			break;
		}
done_mapping:
	dso->base = base;
	dso->dynv = laddr(dso, dyn);
	if (dso->tls.size) dso->tls.image = laddr(dso, tls_image);
	free(allocated_buf);
	return map;
noexec:
	errno = ENOEXEC;
error:
	if (map!=MAP_FAILED) unmap_library(dso);
	free(allocated_buf);
	return 0;
}

static int path_open(const char *name, const char *s, char *buf, size_t buf_size)
{
	size_t l;
	int fd;
	for (;;) {
		s += strspn(s, ":\n");
		l = strcspn(s, ":\n");
		if (l-1 >= INT_MAX) return -1;
		if (snprintf(buf, buf_size, "%.*s/%s", (int)l, s, name) < buf_size) {
			if ((fd = open(buf, O_RDONLY|O_CLOEXEC))>=0) return fd;
			switch (errno) {
			case ENOENT:
			case ENOTDIR:
			case EACCES:
			case ENAMETOOLONG:
				break;
			default:
				/* Any negative value but -1 will inhibit
				 * futher path search. */
				return -2;
			}
		}
		s += l;
	}
}

UT_STATIC int fixup_rpath(struct dso *p, char *buf, size_t buf_size)
{
	size_t n, l;
	const char *s, *t, *origin;
	char *d;
	if (p->rpath || !p->rpath_orig) return 0;
	if (!strchr(p->rpath_orig, '$')) {
		p->rpath = p->rpath_orig;
		return 0;
	}
	n = 0;
	s = p->rpath_orig;
	while ((t=strchr(s, '$'))) {
		if (strncmp(t, "$ORIGIN", 7) && strncmp(t, "${ORIGIN}", 9))
			return 0;
		s = t+1;
		n++;
	}
	if (n > SSIZE_MAX/PATH_MAX) return 0;

	if (p->kernel_mapped) {
		/* $ORIGIN searches cannot be performed for the main program
		 * when it is suid/sgid/AT_SECURE. This is because the
		 * pathname is under the control of the caller of execve.
		 * For libraries, however, $ORIGIN can be processed safely
		 * since the library's pathname came from a trusted source
		 * (either system paths or a call to dlopen). */
		if (libc.secure)
			return 0;
		l = readlink("/proc/self/exe", buf, buf_size);
		if (l == -1) switch (errno) {
		case ENOENT:
		case ENOTDIR:
		case EACCES:
			break;
		default:
			return -1;
		}
		if (l >= buf_size)
			return 0;
		buf[l] = 0;
		origin = buf;
	} else {
		origin = p->name;
	}
	t = strrchr(origin, '/');
	if (t) {
		l = t-origin;
	} else {
		/* Normally p->name will always be an absolute or relative
		 * pathname containing at least one '/' character, but in the
		 * case where ldso was invoked as a command to execute a
		 * program in the working directory, app.name may not. Fix. */
		origin = ".";
		l = 1;
	}
	/* Disallow non-absolute origins for suid/sgid/AT_SECURE. */
	if (libc.secure && *origin != '/')
		return 0;
	p->rpath = malloc(strlen(p->rpath_orig) + n*l + 1);
	if (!p->rpath) return -1;

	d = p->rpath;
	s = p->rpath_orig;
	while ((t=strchr(s, '$'))) {
		memcpy(d, s, t-s);
		d += t-s;
		memcpy(d, origin, l);
		d += l;
		/* It was determined previously that the '$' is followed
		 * either by "ORIGIN" or "{ORIGIN}". */
		s = t + 7 + 2*(t[1]=='{');
	}
	strcpy(d, s);
	return 0;
}

static void decode_dyn(struct dso *p)
{
	size_t dyn[DYN_CNT];
	size_t flags1 = 0;
	decode_vec(p->dynv, dyn, DYN_CNT);
	search_vec(p->dynv, &flags1, DT_FLAGS_1);
	if (flags1 & DF_1_GLOBAL) {
		LD_LOGI("Add DF_1_GLOBAL for %{public}s", p->name);
		p->is_global = true;
	}
	if (flags1 & DF_1_NODELETE) {
		p->flags |= DSO_FLAGS_NODELETE;
	}
	p->syms = laddr(p, dyn[DT_SYMTAB]);
	p->strings = laddr(p, dyn[DT_STRTAB]);
	if (dyn[0]&(1<<DT_HASH))
		p->hashtab = laddr(p, dyn[DT_HASH]);
	if (dyn[0]&(1<<DT_RPATH))
		p->rpath_orig = p->strings + dyn[DT_RPATH];
	if (dyn[0]&(1<<DT_RUNPATH))
		p->rpath_orig = p->strings + dyn[DT_RUNPATH];
	if (dyn[0]&(1<<DT_PLTGOT))
		p->got = laddr(p, dyn[DT_PLTGOT]);
	if (search_vec(p->dynv, dyn, DT_GNU_HASH))
		p->ghashtab = laddr(p, *dyn);
	if (search_vec(p->dynv, dyn, DT_VERSYM))
		p->versym = laddr(p, *dyn);
	if (search_vec(p->dynv, dyn, DT_VERDEF))
		p->verdef = laddr(p, *dyn);
	if (search_vec(p->dynv, dyn, DT_VERNEED))
		p->verneed = laddr(p, *dyn);
}

UT_STATIC size_t count_syms(struct dso *p)
{
	if (p->hashtab) return p->hashtab[1];

	size_t nsym, i;
	uint32_t *buckets = p->ghashtab + 4 + (p->ghashtab[2]*sizeof(size_t)/4);
	uint32_t *hashval;
	for (i = nsym = 0; i < p->ghashtab[0]; i++) {
		if (buckets[i] > nsym)
			nsym = buckets[i];
	}
	if (nsym) {
		hashval = buckets + p->ghashtab[0] + (nsym - p->ghashtab[1]);
		do nsym++;
		while (!(*hashval++ & 1));
	}
	return nsym;
}

static void *dl_mmap(size_t n)
{
	void *p;
	int prot = PROT_READ|PROT_WRITE, flags = MAP_ANONYMOUS|MAP_PRIVATE;
#ifdef SYS_mmap2
	p = (void *)__syscall(SYS_mmap2, 0, n, prot, flags, -1, 0);
#else
	p = (void *)__syscall(SYS_mmap, 0, n, prot, flags, -1, 0);
#endif
	return (unsigned long)p > -4096UL ? 0 : p;
}

static void makefuncdescs(struct dso *p)
{
	static int self_done;
	size_t nsym = count_syms(p);
	size_t i, size = nsym * sizeof(*p->funcdescs);

	if (!self_done) {
		p->funcdescs = dl_mmap(size);
		self_done = 1;
	} else {
		p->funcdescs = malloc(size);
	}
	if (!p->funcdescs) {
		if (!runtime) a_crash();
		error("Error allocating function descriptors for %s", p->name);
		longjmp(*rtld_fail, 1);
	}
	for (i=0; i<nsym; i++) {
		if ((p->syms[i].st_info&0xf)==STT_FUNC && p->syms[i].st_shndx) {
			p->funcdescs[i].addr = laddr(p, p->syms[i].st_value);
			p->funcdescs[i].got = p->got;
		} else {
			p->funcdescs[i].addr = 0;
			p->funcdescs[i].got = 0;
		}
	}
}

static void get_sys_path(ns_configor *conf)
{ 
	LD_LOGD("get_sys_path g_is_asan:%{public}d", g_is_asan);
	/* Use ini file's system paths when Asan is not enabled */
	if (!g_is_asan) {
		sys_path = conf->get_sys_paths();
	} else {
		/* Use ini file's asan system paths when the Asan is enabled
		 * Merge two strings when both sys_paths and asan_sys_paths are valid */
		sys_path = conf->get_asan_sys_paths();
		char *sys_path_default = conf->get_sys_paths();
		if (!sys_path) {
			sys_path = sys_path_default;
		} else if (sys_path_default) {
			size_t newlen = strlen(sys_path) + strlen(sys_path_default) + 2;
			char *new_syspath = malloc(newlen);
			memset(new_syspath, 0, newlen);
			strcpy(new_syspath, sys_path);
			strcat(new_syspath, ":");
			strcat(new_syspath, sys_path_default);
			sys_path = new_syspath;
		}
	}
	if (!sys_path) sys_path = "/lib:/usr/local/lib:/usr/lib:/lib64";
	LD_LOGD("get_sys_path sys_path:%{public}s", sys_path);
}

static struct dso *search_dso_by_name(const char *name, const ns_t *ns) {
	LD_LOGD("search_dso_by_name name:%{public}s, ns_name:%{public}s", name, ns ? ns->ns_name: "NULL");
	for (size_t i = 0; i < ns->ns_dsos->num; i++){
		struct dso *p = ns->ns_dsos->dsos[i];
		if (p->shortname && !strcmp(p->shortname, name)) {
			LD_LOGD("search_dso_by_name found name:%{public}s, ns_name:%{public}s", name, ns ? ns->ns_name: "NULL");
			return p;
		}
	}
	return NULL;
}

static struct dso *search_dso_by_fstat(const struct stat *st, const ns_t *ns, uint64_t file_offset) {
	LD_LOGD("search_dso_by_fstat ns_name:%{public}s", ns ? ns->ns_name : "NULL");
	for (size_t i = 0; i < ns->ns_dsos->num; i++){
		struct dso *p = ns->ns_dsos->dsos[i];
		if (p->dev == st->st_dev && p->ino == st->st_ino && p->file_offset == file_offset) {
			LD_LOGD("search_dso_by_fstat found dev:%{public}lu, ino:%{public}lu, ns_name:%{public}s",
				st->st_dev, st->st_ino, ns ? ns->ns_name : "NULL");
			return p;
		}
	}
	return NULL;
}

static inline int app_has_same_name_so(const char *so_name, const ns_t *ns)
{
   int fd = -1;
   /* Only check system app. */
   if (((ns->flag & LOCAL_NS_PREFERED) != 0) && ns->lib_paths) {
       char tmp_buf[PATH_MAX+1];
       fd = path_open(so_name, ns->lib_paths, tmp_buf, sizeof tmp_buf);
   }
   return fd;
}

/* Find loaded so by name */
static struct dso *find_library_by_name(const char *name, const ns_t *ns, bool check_inherited)
{
	LD_LOGD("find_library_by_name name:%{public}s, ns_name:%{public}s, check_inherited:%{public}d",
		name,
		ns ? ns->ns_name : "NULL",
		!!check_inherited);
	struct dso *p = search_dso_by_name(name, ns);
	if (p) return p;
	if (check_inherited && ns->ns_inherits) {
		for (size_t i = 0; i < ns->ns_inherits->num; i++){
			ns_inherit * inherit = ns->ns_inherits->inherits[i];
			p = search_dso_by_name(name, inherit->inherited_ns);
			if (p && is_sharable(inherit, name)) {
			    if (app_has_same_name_so(name, ns) != -1) {
			        return NULL;
			    }
			    return p;
			}
		}
	}
	return NULL;
}
/* Find loaded so by file stat */
UT_STATIC struct dso *find_library_by_fstat(const struct stat *st, const ns_t *ns, bool check_inherited, uint64_t file_offset) {
	LD_LOGD("find_library_by_fstat ns_name:%{public}s, check_inherited :%{public}d",
		ns ? ns->ns_name : "NULL",
		!!check_inherited);
	struct dso *p = search_dso_by_fstat(st, ns, file_offset);
	if (p) return p;
	if (check_inherited && ns->ns_inherits) {
		for (size_t i = 0; i < ns->ns_inherits->num; i++){
			ns_inherit *inherit = ns->ns_inherits->inherits[i];
			p = search_dso_by_fstat(st, inherit->inherited_ns, file_offset);
			if (p && is_sharable(inherit, p->shortname)) return p;
		}
	}
	return NULL;
}

#ifndef LOAD_ORDER_RANDOMIZATION
/* add namespace function */
struct dso *load_library(
	const char *name, struct dso *needed_by, ns_t *namespace, bool check_inherited, struct reserved_address_params *reserved_params)
{
	char buf[PATH_MAX+1];
	const char *pathname;
	unsigned char *map;
	struct dso *p, temp_dso = {0};
	int fd;
	struct stat st;
	size_t alloc_size;
	int n_th = 0;
	int is_self = 0;

	if (!*name) {
		errno = EINVAL;
		return 0;
	}

	/* Catch and block attempts to reload the implementation itself */
	if (name[0]=='l' && name[1]=='i' && name[2]=='b') {
		static const char reserved[] =
			"c.pthread.rt.m.dl.util.xnet.";
		const char *rp, *next;
		for (rp=reserved; *rp; rp=next) {
			next = strchr(rp, '.') + 1;
			if (strncmp(name+3, rp, next-rp) == 0)
				break;
		}
		if (*rp) {
			if (ldd_mode) {
				/* Track which names have been resolved
				 * and only report each one once. */
				static unsigned reported;
				unsigned mask = 1U<<(rp-reserved);
				if (!(reported & mask)) {
					reported |= mask;
					dprintf(1, "\t%s => %s (%p)\n",
						name, ldso.name,
						ldso.base);
				}
			}
			is_self = 1;
		}
	}
	if (!strcmp(name, ldso.name)) is_self = 1;
	if (is_self) {
		if (!ldso.prev) {
			tail->next = &ldso;
			ldso.prev = tail;
			tail = &ldso;
			ldso.namespace = namespace;
			ns_add_dso(namespace, &ldso);
		}
		return &ldso;
	}
	if (strchr(name, '/')) {
		pathname = name;

		if (!is_accessible(namespace, pathname, g_is_asan, check_inherited)) {
			fd = -1;
			LD_LOGD("load_library is_accessible return false,fd = -1");
		} else {
			fd = open(name, O_RDONLY|O_CLOEXEC);
			LD_LOGD("load_library is_accessible return true, open file fd:%{public}d .", fd);
		}
	} else {
		/* Search for the name to see if it's already loaded */
		/* Search in namespace */
		p = find_library_by_name(name, namespace, check_inherited);
		if (p) {
			LD_LOGD("load_library find_library_by_name found p, return it!");
			return p;
		}
		if (strlen(name) > NAME_MAX) {
			LD_LOGE("load_library name exceeding the maximum length, return 0!");
			return 0;
		}
		fd = -1;
		if (namespace->env_paths) fd = path_open(name, namespace->env_paths, buf, sizeof buf);
		for (p = needed_by; fd == -1 && p; p = p->needed_by) {
			if (fixup_rpath(p, buf, sizeof buf) < 0) {
				LD_LOGD("load_library Inhibit further search,fd = -2.");
				fd = -2; /* Inhibit further search. */
			}
			if (p->rpath) {
				fd = path_open(name, p->rpath, buf, sizeof buf);
				LD_LOGD("load_library  p->rpath path_open fd:%{public}d.", fd);
			}

		}
		if (g_is_asan) {
			fd = handle_asan_path_open(fd, name, namespace, buf, sizeof buf);
			LD_LOGD("load_library handle_asan_path_open fd:%{public}d.", fd);
		} else {
			if (fd == -1 && namespace->lib_paths) {
				fd = path_open(name, namespace->lib_paths, buf, sizeof buf);
				LD_LOGD("load_library no asan lib_paths path_open fd:%{public}d.", fd);
			}
		}
		pathname = buf;
		LD_LOGD("load_library lib_paths pathname:%{public}s.", pathname);
	}
	if (fd < 0) {
		if (!check_inherited || !namespace->ns_inherits) return 0;
		/* Load lib in inherited namespace. Do not check inherited again.*/
		for (size_t i = 0; i < namespace->ns_inherits->num; i++) {
			ns_inherit *inherit = namespace->ns_inherits->inherits[i];
			if (strchr(name, '/')==0 && !is_sharable(inherit, name)) continue;
			p = load_library(name, needed_by, inherit->inherited_ns, false, reserved_params);
			if (p) {
				LD_LOGD("load_library search in inherited, found p ,inherited_ns name:%{public}s",
						inherit->inherited_ns->ns_name);
				return p;
			}
		}
		return 0;
	}
	if (fstat(fd, &st) < 0) {
		close(fd);
		LD_LOGE("load_library fstat < 0,return 0!");
		return 0;
	}
	/* Search in namespace */
	p = find_library_by_fstat(&st, namespace, check_inherited, 0);
	if (p) {
		/* If this library was previously loaded with a
		* pathname but a search found the same inode,
		* setup its shortname so it can be found by name. */
		if (!p->shortname && pathname != name)
			p->shortname = strrchr(p->name, '/')+1;
		close(fd);
		LD_LOGD("load_library find_library_by_fstat, found p and return it!");
		return p;
	}
	map = noload ? 0 : map_library(fd, &temp_dso, reserved_params);
	close(fd);
	if (!map) return 0;

	/* Avoid the danger of getting two versions of libc mapped into the
	 * same process when an absolute pathname was used. The symbols
	 * checked are chosen to catch both musl and glibc, and to avoid
	 * false positives from interposition-hack libraries. */
	decode_dyn(&temp_dso);
	if (find_sym(&temp_dso, "__libc_start_main", 1).sym &&
		find_sym(&temp_dso, "stdin", 1).sym) {
		unmap_library(&temp_dso);
		return load_library("libc.so", needed_by, namespace, true, reserved_params);
	}
	/* Past this point, if we haven't reached runtime yet, ldso has
	 * committed either to use the mapped library or to abort execution.
	 * Unmapping is not possible, so we can safely reclaim gaps. */
	if (!runtime) reclaim_gaps(&temp_dso);

	/* Allocate storage for the new DSO. When there is TLS, this
	 * storage must include a reservation for all pre-existing
	 * threads to obtain copies of both the new TLS, and an
	 * extended DTV capable of storing an additional slot for
	 * the newly-loaded DSO. */
	alloc_size = sizeof *p + strlen(pathname) + 1;
	if (runtime && temp_dso.tls.image) {
		size_t per_th = temp_dso.tls.size + temp_dso.tls.align
			+ sizeof(void *) * (tls_cnt+3);
		n_th = libc.threads_minus_1 + 1;
		if (n_th > SSIZE_MAX / per_th) alloc_size = SIZE_MAX;
		else alloc_size += n_th * per_th;
	}
	p = calloc(1, alloc_size);
	if (!p) {
		unmap_library(&temp_dso);
		return 0;
	}
	memcpy(p, &temp_dso, sizeof temp_dso);
	p->dev = st.st_dev;
	p->ino = st.st_ino;
	p->needed_by = needed_by;
	p->name = p->buf;
	p->runtime_loaded = runtime;
	strcpy(p->name, pathname);
	/* Add a shortname only if name arg was not an explicit pathname. */
	if (pathname != name) p->shortname = strrchr(p->name, '/')+1;
	if (p->tls.image) {
		p->tls_id = ++tls_cnt;
		tls_align = MAXP2(tls_align, p->tls.align);
#ifdef TLS_ABOVE_TP
		p->tls.offset = tls_offset + ( (p->tls.align-1) &
			(-tls_offset + (uintptr_t)p->tls.image) );
		tls_offset = p->tls.offset + p->tls.size;
#else
		tls_offset += p->tls.size + p->tls.align - 1;
		tls_offset -= (tls_offset + (uintptr_t)p->tls.image)
			& (p->tls.align-1);
		p->tls.offset = tls_offset;
#endif
		p->new_dtv = (void *)(-sizeof(size_t) &
			(uintptr_t)(p->name+strlen(p->name)+sizeof(size_t)));
		p->new_tls = (void *)(p->new_dtv + n_th*(tls_cnt+1));
		if (tls_tail) tls_tail->next = &p->tls;
		else libc.tls_head = &p->tls;
		tls_tail = &p->tls;
	}

	tail->next = p;
	p->prev = tail;
	tail = p;

	/* Add dso to namespace */
	p->namespace = namespace;
	ns_add_dso(namespace, p);
	if (runtime)
		p->by_dlopen = 1;

	if (DL_FDPIC) makefuncdescs(p);

	if (ldd_mode) dprintf(1, "\t%s => %s (%p)\n", name, pathname, p->base);

	return p;
}

static void load_direct_deps(struct dso *p, ns_t *namespace, struct reserved_address_params *reserved_params)
{
	size_t i, cnt=0;

	if (p->deps) return;
	/* For head, all preloads are direct pseudo-dependencies.
	 * Count and include them now to avoid realloc later. */
	if (p==head) for (struct dso *q=p->next; q; q=q->next)
		cnt++;
	for (i=0; p->dynv[i]; i+=2)
		if (p->dynv[i] == DT_NEEDED) cnt++;
	/* Use builtin buffer for apps with no external deps, to
	 * preserve property of no runtime failure paths. */
	p->deps = (p==head && cnt<2) ? builtin_deps :
		calloc(cnt+1, sizeof *p->deps);
	if (!p->deps) {
		error("Error loading dependencies for %s", p->name);
		if (runtime) longjmp(*rtld_fail, 1);
	}
	cnt=0;
	if (p==head) for (struct dso *q=p->next; q; q=q->next)
		p->deps[cnt++] = q;
	for (i=0; p->dynv[i]; i+=2) {
		if (p->dynv[i] != DT_NEEDED) continue;
		struct dso *dep = load_library(p->strings + p->dynv[i+1], p, namespace, true, reserved_params);
		LD_LOGD("loading shared library %{public}s: (needed by %{public}s)", p->strings + p->dynv[i+1], p->name);
		if (!dep) {
			error("Error loading shared library %s: %m (needed by %s)",
				p->strings + p->dynv[i+1], p->name);
			if (runtime) longjmp(*rtld_fail, 1);
			continue;
		}
		p->deps[cnt++] = dep;
	}
	p->deps[cnt] = 0;
	p->ndeps_direct = cnt;
	for (i = 0; i < p->ndeps_direct; i++) {
		add_dso_parent(p->deps[i], p);
	}
}

static void load_deps(struct dso *p, struct reserved_address_params *reserved_params)
{
	if (p->deps) return;
	for (; p; p=p->next)
		load_direct_deps(p, p->namespace, reserved_params);
}
#endif

static void extend_bfs_deps(struct dso *p)
{
	size_t i, j, cnt, ndeps_all;
	struct dso **tmp;

	/* Can't use realloc if the original p->deps was allocated at
	 * program entry and malloc has been replaced, or if it's
	 * the builtin non-allocated trivial main program deps array. */
	int no_realloc = (__malloc_replaced && !p->runtime_loaded)
		|| p->deps == builtin_deps;

	if (p->bfs_built) return;
	ndeps_all = p->ndeps_direct;

	/* Mark existing (direct) deps so they won't be duplicated. */
	for (i=0; p->deps[i]; i++)
		p->deps[i]->mark = 1;

	/* For each dependency already in the list, copy its list of direct
	 * dependencies to the list, excluding any items already in the
	 * list. Note that the list this loop iterates over will grow during
	 * the loop, but since duplicates are excluded, growth is bounded. */
	for (i=0; p->deps[i]; i++) {
		struct dso *dep = p->deps[i];
		for (j=cnt=0; j<dep->ndeps_direct; j++)
			if (!dep->deps[j]->mark) cnt++;
		tmp = no_realloc ? 
			malloc(sizeof(*tmp) * (ndeps_all+cnt+1)) :
			realloc(p->deps, sizeof(*tmp) * (ndeps_all+cnt+1));
		if (!tmp) {
			error("Error recording dependencies for %s", p->name);
			if (runtime) longjmp(*rtld_fail, 1);
			continue;
		}
		if (no_realloc) {
			memcpy(tmp, p->deps, sizeof(*tmp) * (ndeps_all+1));
			no_realloc = 0;
		}
		p->deps = tmp;
		for (j=0; j<dep->ndeps_direct; j++) {
			if (dep->deps[j]->mark) continue;
			dep->deps[j]->mark = 1;
			p->deps[ndeps_all++] = dep->deps[j];
		}
		p->deps[ndeps_all] = 0;
	}
	p->bfs_built = 1;
	for (p=head; p; p=p->next)
		p->mark = 0;
}

#ifndef LOAD_ORDER_RANDOMIZATION
static void load_preload(char *s, ns_t *ns)
{
	int tmp;
	char *z;
	for (z=s; *z; s=z) {
		for (   ; *s && (isspace(*s) || *s==':'); s++);
		for (z=s; *z && !isspace(*z) && *z!=':'; z++);
		tmp = *z;
		*z = 0;
		load_library(s, 0, ns, true, NULL);
		*z = tmp;
	}
}
#endif

static void add_syms(struct dso *p)
{
	if (!p->syms_next && syms_tail != p) {
		syms_tail->syms_next = p;
		syms_tail = p;
	}
}

static void revert_syms(struct dso *old_tail)
{
	struct dso *p, *next;
	/* Chop off the tail of the list of dsos that participate in
	 * the global symbol table, reverting them to RTLD_LOCAL. */
	for (p=old_tail; p; p=next) {
		next = p->syms_next;
		p->syms_next = 0;
	}
	syms_tail = old_tail;
}

static void do_mips_relocs(struct dso *p, size_t *got)
{
	size_t i, j, rel[2];
	unsigned char *base = p->base;
	i=0; search_vec(p->dynv, &i, DT_MIPS_LOCAL_GOTNO);
	if (p==&ldso) {
		got += i;
	} else {
		while (i--) *got++ += (size_t)base;
	}
	j=0; search_vec(p->dynv, &j, DT_MIPS_GOTSYM);
	i=0; search_vec(p->dynv, &i, DT_MIPS_SYMTABNO);
	Sym *sym = p->syms + j;
	rel[0] = (unsigned char *)got - base;
	for (i-=j; i; i--, sym++, rel[0]+=sizeof(size_t)) {
		rel[1] = R_INFO(sym-p->syms, R_MIPS_JUMP_SLOT);
		do_relocs(p, rel, sizeof rel, 2);
	}
}

static uint8_t* sleb128_decoder(uint8_t* current, uint8_t* end, size_t* value)
{    
	size_t result = 0;
	static const size_t size = CHAR_BIT * sizeof(result);

	size_t shift = 0;
	uint8_t byte;

	do {
		if (current >= end) {
			a_crash();
		}

		byte = *current++;
		result |= ((size_t)(byte & 127) << shift);
		shift += 7;
	} while (byte & 128);

	if (shift < size && (byte & 64)) {
		result |= -((size_t)(1) << shift);
	}

	*value = result;
	
	return current;
}

static void do_android_relocs(struct dso *p, size_t dt_name, size_t dt_size)
{
	size_t android_rel_addr = 0, android_rel_size = 0;
	uint8_t *android_rel_curr, *android_rel_end;

	search_vec(p->dynv, &android_rel_addr, dt_name);
	search_vec(p->dynv, &android_rel_size, dt_size);

	if (!android_rel_addr || (android_rel_size < 4)) {
		return;
	}

	android_rel_curr = laddr(p, android_rel_addr);
	if (memcmp(android_rel_curr, "APS2", ANDROID_REL_SIGN_SIZE)) {
		return;
	}

	android_rel_curr += ANDROID_REL_SIGN_SIZE;
	android_rel_size -= ANDROID_REL_SIGN_SIZE;

	android_rel_end = android_rel_curr + android_rel_size;
	
	size_t relocs_num;
	size_t rel[3] = {0};

	android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &relocs_num);
	android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &rel[0]);

	for (size_t i = 0; i < relocs_num;) {

		size_t group_size, group_flags;

		android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &group_size);
		android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &group_flags);

		size_t group_r_offset_delta = 0;

		if (group_flags & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) {
			android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &group_r_offset_delta);
		}
		
		if (group_flags & RELOCATION_GROUPED_BY_INFO_FLAG) {
			android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &rel[1]);
		}

		const size_t addend_flags = group_flags & (RELOCATION_GROUP_HAS_ADDEND_FLAG | RELOCATION_GROUPED_BY_ADDEND_FLAG);

		if (addend_flags == RELOCATION_GROUP_HAS_ADDEND_FLAG) {
		} else if (addend_flags == (RELOCATION_GROUP_HAS_ADDEND_FLAG | RELOCATION_GROUPED_BY_ADDEND_FLAG)) {
			size_t addend;
			android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &addend);
			rel[2] += addend;
		} else {
			rel[2] = 0;
		}

		for (size_t j = 0; j < group_size; j++) {
			if (group_flags & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) {
				rel[0] += group_r_offset_delta;
			} else {
				size_t offset_detla;
				android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &offset_detla);

				rel[0] += offset_detla;
			}

			if ((group_flags & RELOCATION_GROUPED_BY_INFO_FLAG) == 0) {
				android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &rel[1]);
			}

			if (addend_flags == RELOCATION_GROUP_HAS_ADDEND_FLAG) {
				size_t addend;
				android_rel_curr = sleb128_decoder(android_rel_curr, android_rel_end, &addend);
				rel[2] += addend;
			}

			if (dt_name == DT_ANDROID_REL) {
				do_relocs(p, rel, sizeof(size_t)*2, 2);
			} else {
				do_relocs(p, rel, sizeof(size_t)*3, 3);
			}
		}

		i += group_size;
	}
}

static void do_relr_relocs(struct dso *dso, size_t *relr, size_t relr_size)
{
	unsigned char *base = dso->base;
	size_t *reloc_addr;
	for (; relr_size; relr++, relr_size-=sizeof(size_t))
		if ((relr[0]&1) == 0) {
			reloc_addr = laddr(dso, relr[0]);
			*reloc_addr++ += (size_t)base;
		} else {
			int i = 0;
			for (size_t bitmap=relr[0]; (bitmap>>=1); i++)
				if (bitmap&1)
					reloc_addr[i] += (size_t)base;
			reloc_addr += 8*sizeof(size_t)-1;
		}
}

static void reloc_all(struct dso *p, const dl_extinfo *extinfo)
{
	ssize_t relro_fd_offset = 0;
	size_t dyn[DYN_CNT];
	for (; p; p=p->next) {
		if (p->relocated) continue;
		if (p != &ldso) {
			add_can_search_so_list_in_dso(p, head);
		}
		decode_vec(p->dynv, dyn, DYN_CNT);
		if (NEED_MIPS_GOT_RELOCS)
			do_mips_relocs(p, laddr(p, dyn[DT_PLTGOT]));
		do_relocs(p, laddr(p, dyn[DT_JMPREL]), dyn[DT_PLTRELSZ],
			2+(dyn[DT_PLTREL]==DT_RELA));
		do_relocs(p, laddr(p, dyn[DT_REL]), dyn[DT_RELSZ], 2);
		do_relocs(p, laddr(p, dyn[DT_RELA]), dyn[DT_RELASZ], 3);
		if (!DL_FDPIC)
			do_relr_relocs(p, laddr(p, dyn[DT_RELR]), dyn[DT_RELRSZ]);

		do_android_relocs(p, DT_ANDROID_REL, DT_ANDROID_RELSZ);
		do_android_relocs(p, DT_ANDROID_RELA, DT_ANDROID_RELASZ);

		if (head != &ldso && p->relro_start != p->relro_end &&
			mprotect(laddr(p, p->relro_start), p->relro_end-p->relro_start, PROT_READ)
			&& errno != ENOSYS) {
			error("Error relocating %s: RELRO protection failed: %m",
				p->name);
			if (runtime) longjmp(*rtld_fail, 1);
		}
		/* Handle serializing/mapping the RELRO segment */
		handle_relro_sharing(p, extinfo, &relro_fd_offset);

		p->relocated = 1;
		free_reloc_can_search_dso(p);
	}
}

static void kernel_mapped_dso(struct dso *p)
{
	size_t min_addr = -1, max_addr = 0, cnt;
	Phdr *ph = p->phdr;
	for (cnt = p->phnum; cnt--; ph = (void *)((char *)ph + p->phentsize)) {
		if (ph->p_type == PT_DYNAMIC) {
			p->dynv = laddr(p, ph->p_vaddr);
		} else if (ph->p_type == PT_GNU_RELRO) {
			p->relro_start = ph->p_vaddr & -PAGE_SIZE;
			p->relro_end = (ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE;
		} else if (ph->p_type == PT_GNU_STACK) {
			if (!runtime && ph->p_memsz > __default_stacksize) {
				__default_stacksize =
					ph->p_memsz < DEFAULT_STACK_MAX ?
					ph->p_memsz : DEFAULT_STACK_MAX;
			}
		}
		if (ph->p_type != PT_LOAD) continue;
		if (ph->p_vaddr < min_addr)
			min_addr = ph->p_vaddr;
		if (ph->p_vaddr+ph->p_memsz > max_addr)
			max_addr = ph->p_vaddr+ph->p_memsz;
	}
	min_addr &= -PAGE_SIZE;
	max_addr = (max_addr + PAGE_SIZE-1) & -PAGE_SIZE;
	p->map = p->base + min_addr;
	p->map_len = max_addr - min_addr;
	p->kernel_mapped = 1;
}

void __libc_exit_fini()
{
	struct dso *p;
	size_t dyn[DYN_CNT];
	pthread_t self = __pthread_self();

	/* Take both locks before setting shutting_down, so that
	 * either lock is sufficient to read its value. The lock
	 * order matches that in dlopen to avoid deadlock. */
	pthread_rwlock_wrlock(&lock);
	pthread_mutex_lock(&init_fini_lock);
	shutting_down = 1;
	pthread_rwlock_unlock(&lock);
	for (p=fini_head; p; p=p->fini_next) {
		while (p->ctor_visitor && p->ctor_visitor!=self)
			pthread_cond_wait(&ctor_cond, &init_fini_lock);
		if (!p->constructed) continue;
		decode_vec(p->dynv, dyn, DYN_CNT);
		if (dyn[0] & (1<<DT_FINI_ARRAY)) {
			size_t n = dyn[DT_FINI_ARRAYSZ]/sizeof(size_t);
			size_t *fn = (size_t *)laddr(p, dyn[DT_FINI_ARRAY])+n;
			while (n--) ((void (*)(void))*--fn)();
		}
#ifndef NO_LEGACY_INITFINI
		if ((dyn[0] & (1<<DT_FINI)) && dyn[DT_FINI])
			fpaddr(p, dyn[DT_FINI])();
#endif
	}
}

void __ldso_atfork(int who)
{
	if (who<0) {
		pthread_rwlock_wrlock(&lock);
		pthread_mutex_lock(&init_fini_lock);
	} else {
		pthread_mutex_unlock(&init_fini_lock);
		pthread_rwlock_unlock(&lock);
	}
}

static struct dso **queue_ctors(struct dso *dso)
{
	size_t cnt, qpos, spos, i;
	struct dso *p, **queue, **stack;

	if (ldd_mode) return 0;

	/* Bound on queue size is the total number of indirect deps.
	 * If a bfs deps list was built, we can use it. Otherwise,
	 * bound by the total number of DSOs, which is always safe and
	 * is reasonable we use it (for main app at startup). */
	if (dso->bfs_built) {
		for (cnt=0; dso->deps[cnt]; cnt++)
			dso->deps[cnt]->mark = 0;
		cnt++; /* self, not included in deps */
	} else {
		for (cnt=0, p=head; p; cnt++, p=p->next)
			p->mark = 0;
	}
	cnt++; /* termination slot */
	if (dso==head && cnt <= countof(builtin_ctor_queue))
		queue = builtin_ctor_queue;
	else
		queue = calloc(cnt, sizeof *queue);

	if (!queue) {
		error("Error allocating constructor queue: %m\n");
		if (runtime) longjmp(*rtld_fail, 1);
		return 0;
	}

	/* Opposite ends of the allocated buffer serve as an output queue
	 * and a working stack. Setup initial stack with just the argument
	 * dso and initial queue empty... */
	stack = queue;
	qpos = 0;
	spos = cnt;
	stack[--spos] = dso;
	dso->next_dep = 0;
	dso->mark = 1;

	/* Then perform pseudo-DFS sort, but ignoring circular deps. */
	while (spos<cnt) {
		p = stack[spos++];
		while (p->next_dep < p->ndeps_direct) {
			if (p->deps[p->next_dep]->mark) {
				p->next_dep++;
			} else {
				stack[--spos] = p;
				p = p->deps[p->next_dep];
				p->next_dep = 0;
				p->mark = 1;
			}
		}
		queue[qpos++] = p;
	}
	queue[qpos] = 0;
	for (i=0; i<qpos; i++) queue[i]->mark = 0;

	return queue;
}

static void do_init_fini(struct dso **queue)
{
	struct dso *p;
	size_t dyn[DYN_CNT], i;
	pthread_t self = __pthread_self();

	pthread_mutex_lock(&init_fini_lock);
	for (i=0; (p=queue[i]); i++) {
		while ((p->ctor_visitor && p->ctor_visitor!=self) || shutting_down)
			pthread_cond_wait(&ctor_cond, &init_fini_lock);
		if (p->ctor_visitor || p->constructed)
			continue;
		p->ctor_visitor = self;
		
		decode_vec(p->dynv, dyn, DYN_CNT);
		if (dyn[0] & ((1<<DT_FINI) | (1<<DT_FINI_ARRAY))) {
			p->fini_next = fini_head;
			fini_head = p;
		}

		pthread_mutex_unlock(&init_fini_lock);

#ifndef NO_LEGACY_INITFINI
		if ((dyn[0] & (1<<DT_INIT)) && dyn[DT_INIT])
			fpaddr(p, dyn[DT_INIT])();
#endif
		if (dyn[0] & (1<<DT_INIT_ARRAY)) {
			size_t n = dyn[DT_INIT_ARRAYSZ]/sizeof(size_t);
			size_t *fn = laddr(p, dyn[DT_INIT_ARRAY]);
			if (p != &ldso) {
				trace_marker_begin(HITRACE_TAG_MUSL, "calling constructors: ", p->name);
			}
			while (n--) ((void (*)(void))*fn++)();
			if (p != &ldso) {
				trace_marker_end(HITRACE_TAG_MUSL);
			}
		}

		pthread_mutex_lock(&init_fini_lock);
		p->ctor_visitor = 0;
		p->constructed = 1;
		pthread_cond_broadcast(&ctor_cond);
	}
	pthread_mutex_unlock(&init_fini_lock);
}

void __libc_start_init(void)
{
	do_init_fini(main_ctor_queue);
	if (!__malloc_replaced && main_ctor_queue != builtin_ctor_queue)
		free(main_ctor_queue);
	main_ctor_queue = 0;
}

static void dl_debug_state(void)
{
}

weak_alias(dl_debug_state, _dl_debug_state);

void __init_tls(size_t *auxv)
{
}

static void update_tls_size()
{
	libc.tls_cnt = tls_cnt;
	libc.tls_align = tls_align;
	libc.tls_size = ALIGN(
		(1+tls_cnt) * sizeof(void *) +
		tls_offset +
		sizeof(struct pthread) +
		tls_align * 2,
	tls_align);
}

static void install_new_tls(void)
{
	sigset_t set;
	pthread_t self = __pthread_self(), td;
	struct dso *dtv_provider = container_of(tls_tail, struct dso, tls);
	uintptr_t (*newdtv)[tls_cnt+1] = (void *)dtv_provider->new_dtv;
	struct dso *p;
	size_t i, j;
	size_t old_cnt = self->dtv[0];

	__block_app_sigs(&set);
	__tl_lock();
	/* Copy existing dtv contents from all existing threads. */
	for (i=0, td=self; !i || td!=self; i++, td=td->next) {
		memcpy(newdtv+i, td->dtv,
			(old_cnt+1)*sizeof(uintptr_t));
		newdtv[i][0] = tls_cnt;
	}
	/* Install new dtls into the enlarged, uninstalled dtv copies. */
	for (p=head; ; p=p->next) {
		if (p->tls_id <= old_cnt) continue;
		unsigned char *mem = p->new_tls;
		for (j=0; j<i; j++) {
			unsigned char *new = mem;
			new += ((uintptr_t)p->tls.image - (uintptr_t)mem)
				& (p->tls.align-1);
			memcpy(new, p->tls.image, p->tls.len);
			newdtv[j][p->tls_id] =
				(uintptr_t)new + DTP_OFFSET;
			mem += p->tls.size + p->tls.align;
		}
		if (p->tls_id == tls_cnt) break;
	}

	/* Broadcast barrier to ensure contents of new dtv is visible
	 * if the new dtv pointer is. The __membarrier function has a
	 * fallback emulation using signals for kernels that lack the
	 * feature at the syscall level. */

	__membarrier(MEMBARRIER_CMD_PRIVATE_EXPEDITED, 0);

	/* Install new dtv for each thread. */
	for (j=0, td=self; !j || td!=self; j++, td=td->next) {
		td->dtv = newdtv[j];
	}

	__tl_unlock();
	__restore_sigs(&set);
}

/* Stage 1 of the dynamic linker is defined in dlstart.c. It calls the
 * following stage 2 and stage 3 functions via primitive symbolic lookup
 * since it does not have access to their addresses to begin with. */

/* Stage 2 of the dynamic linker is called after relative relocations 
 * have been processed. It can make function calls to static functions
 * and access string literals and static data, but cannot use extern
 * symbols. Its job is to perform symbolic relocations on the dynamic
 * linker itself, but some of the relocations performed may need to be
 * replaced later due to copy relocations in the main program. */

hidden void __dls2(unsigned char *base, size_t *sp)
{
	size_t *auxv;
	for (auxv=sp+1+*sp+1; *auxv; auxv++);
	auxv++;
	if (DL_FDPIC) {
		void *p1 = (void *)sp[-2];
		void *p2 = (void *)sp[-1];
		if (!p1) {
			size_t aux[AUX_CNT];
			decode_vec(auxv, aux, AUX_CNT);
			if (aux[AT_BASE]) ldso.base = (void *)aux[AT_BASE];
			else ldso.base = (void *)(aux[AT_PHDR] & -4096);
		}
		app_loadmap = p2 ? p1 : 0;
		ldso.loadmap = p2 ? p2 : p1;
		ldso.base = laddr(&ldso, 0);
	} else {
		ldso.base = base;
	}
	size_t aux[AUX_CNT];
	decode_vec(auxv, aux, AUX_CNT);
	libc.page_size = aux[AT_PAGESZ];
	Ehdr *ehdr = (void *)ldso.base;
	ldso.name = ldso.shortname = "libc.so";
	ldso.phnum = ehdr->e_phnum;
	ldso.phdr = laddr(&ldso, ehdr->e_phoff);
	ldso.phentsize = ehdr->e_phentsize;
	ldso.is_global = true;
	kernel_mapped_dso(&ldso);
	decode_dyn(&ldso);

	if (DL_FDPIC) makefuncdescs(&ldso);

	/* Prepare storage for to save clobbered REL addends so they
	 * can be reused in stage 3. There should be very few. If
	 * something goes wrong and there are a huge number, abort
	 * instead of risking stack overflow. */
	size_t dyn[DYN_CNT];
	decode_vec(ldso.dynv, dyn, DYN_CNT);
	size_t *rel = laddr(&ldso, dyn[DT_REL]);
	size_t rel_size = dyn[DT_RELSZ];
	size_t symbolic_rel_cnt = 0;
	apply_addends_to = rel;
	for (; rel_size; rel+=2, rel_size-=2*sizeof(size_t))
		if (!IS_RELATIVE(rel[1], ldso.syms)) symbolic_rel_cnt++;
	if (symbolic_rel_cnt >= ADDEND_LIMIT) a_crash();
	size_t addends[symbolic_rel_cnt+1];
	saved_addends = addends;

	head = &ldso;
	reloc_all(&ldso, NULL);

	ldso.relocated = 0;

	/* Call dynamic linker stage-2b, __dls2b, looking it up
	 * symbolically as a barrier against moving the address
	 * load across the above relocation processing. */
	struct symdef dls2b_def = find_sym(&ldso, "__dls2b", 0);
	if (DL_FDPIC) ((stage3_func)&ldso.funcdescs[dls2b_def.sym-ldso.syms])(sp, auxv, aux);
	else ((stage3_func)laddr(&ldso, dls2b_def.sym->st_value))(sp, auxv, aux);
}

/* Stage 2b sets up a valid thread pointer, which requires relocations
 * completed in stage 2, and on which stage 3 is permitted to depend.
 * This is done as a separate stage, with symbolic lookup as a barrier,
 * so that loads of the thread pointer and &errno can be pure/const and
 * thereby hoistable. */

void __dls2b(size_t *sp, size_t *auxv, size_t *aux)
{
	/* Setup early thread pointer in builtin_tls for ldso/libc itself to
	 * use during dynamic linking. If possible it will also serve as the
	 * thread pointer at runtime. */
	search_vec(auxv, &__hwcap, AT_HWCAP);
	libc.auxv = auxv;
	libc.tls_size = sizeof builtin_tls;
	libc.tls_align = tls_align;
	if (__init_tp(__copy_tls((void *)builtin_tls)) < 0) {
		a_crash();
	}
	__pthread_self()->stack = (void *)(sp + 1);
	struct symdef dls3_def = find_sym(&ldso, "__dls3", 0);
	if (DL_FDPIC) ((stage3_func)&ldso.funcdescs[dls3_def.sym-ldso.syms])(sp, auxv, aux);
	else ((stage3_func)laddr(&ldso, dls3_def.sym->st_value))(sp, auxv, aux);
}

/* Stage 3 of the dynamic linker is called with the dynamic linker/libc
 * fully functional. Its job is to load (if not already loaded) and
 * process dependencies and relocations for the main application and
 * transfer control to its entry point. */

void __dls3(size_t *sp, size_t *auxv, size_t *aux)
{
	static struct dso app, vdso;
	size_t i;
	char *env_preload=0;
	char *replace_argv0=0;
	size_t vdso_base;
	int argc = *sp;
	char **argv = (void *)(sp+1);
	char **argv_orig = argv;
	char **envp = argv+argc+1;

	/* Find aux vector just past environ[] and use it to initialize
	 * global data that may be needed before we can make syscalls. */
	__environ = envp;
	search_vec(auxv, &__sysinfo, AT_SYSINFO);
	__pthread_self()->sysinfo = __sysinfo;
	libc.secure = ((aux[0]&0x7800)!=0x7800 || aux[AT_UID]!=aux[AT_EUID]
		|| aux[AT_GID]!=aux[AT_EGID] || aux[AT_SECURE]);

	/* Only trust user/env if kernel says we're not suid/sgid */
	if (!libc.secure) {
		env_path = getenv("LD_LIBRARY_PATH");
		env_preload = getenv("LD_PRELOAD");
	}
#ifdef OHOS_ENABLE_PARAMETER
	InitParameterClient();
#endif
// we may abort when linking other libs, load signal handler before stage start
#ifdef DFX_SIGNAL_LIBC
	DFX_InstallSignalHandler();
#endif
	__init_fdsan();
	/* If the main program was already loaded by the kernel,
	 * AT_PHDR will point to some location other than the dynamic
	 * linker's program headers. */
	if (aux[AT_PHDR] != (size_t)ldso.phdr) {
		size_t interp_off = 0;
		size_t tls_image = 0;
		/* Find load address of the main program, via AT_PHDR vs PT_PHDR. */
		Phdr *phdr = app.phdr = (void *)aux[AT_PHDR];
		app.phnum = aux[AT_PHNUM];
		app.phentsize = aux[AT_PHENT];
		for (i=aux[AT_PHNUM]; i; i--, phdr=(void *)((char *)phdr + aux[AT_PHENT])) {
			if (phdr->p_type == PT_PHDR)
				app.base = (void *)(aux[AT_PHDR] - phdr->p_vaddr);
			else if (phdr->p_type == PT_INTERP)
				interp_off = (size_t)phdr->p_vaddr;
			else if (phdr->p_type == PT_TLS) {
				tls_image = phdr->p_vaddr;
				app.tls.len = phdr->p_filesz;
				app.tls.size = phdr->p_memsz;
				app.tls.align = phdr->p_align;
			}
		}
		if (DL_FDPIC) app.loadmap = app_loadmap;
		if (app.tls.size) app.tls.image = laddr(&app, tls_image);
		if (interp_off) ldso.name = laddr(&app, interp_off);
		if ((aux[0] & (1UL<<AT_EXECFN))
			&& strncmp((char *)aux[AT_EXECFN], "/proc/", 6))
			app.name = (char *)aux[AT_EXECFN];
		else
			app.name = argv[0];
		kernel_mapped_dso(&app);
	} else {
		int fd;
		char *ldname = argv[0];
		size_t l = strlen(ldname);
		if (l >= 3 && !strcmp(ldname+l-3, "ldd")) ldd_mode = 1;
		argv++;
		while (argv[0] && argv[0][0]=='-' && argv[0][1]=='-') {
			char *opt = argv[0]+2;
			*argv++ = (void *)-1;
			if (!*opt) {
				break;
			} else if (!memcmp(opt, "list", 5)) {
				ldd_mode = 1;
			} else if (!memcmp(opt, "library-path", 12)) {
				if (opt[12]=='=') env_path = opt+13;
				else if (opt[12]) *argv = 0;
				else if (*argv) env_path = *argv++;
			} else if (!memcmp(opt, "preload", 7)) {
				if (opt[7]=='=') env_preload = opt+8;
				else if (opt[7]) *argv = 0;
				else if (*argv) env_preload = *argv++;
			} else if (!memcmp(opt, "argv0", 5)) {
				if (opt[5]=='=') replace_argv0 = opt+6;
				else if (opt[5]) *argv = 0;
				else if (*argv) replace_argv0 = *argv++;
			} else {
				argv[0] = 0;
			}
		}
		argv[-1] = (void *)(argc - (argv-argv_orig));
		if (!argv[0]) {
			dprintf(2, "musl libc (" LDSO_ARCH ")\n"
				"Version %s\n"
				"Dynamic Program Loader\n"
				"Usage: %s [options] [--] pathname%s\n",
				__libc_version, ldname,
				ldd_mode ? "" : " [args]");
			_exit(1);
		}
		fd = open(argv[0], O_RDONLY);
		if (fd < 0) {
			dprintf(2, "%s: cannot load %s: %s\n", ldname, argv[0], strerror(errno));
			_exit(1);
		}
		Ehdr *ehdr = map_library(fd, &app, NULL);
		if (!ehdr) {
			dprintf(2, "%s: %s: Not a valid dynamic program\n", ldname, argv[0]);
			_exit(1);
		}
		close(fd);
		ldso.name = ldname;
		app.name = argv[0];
		aux[AT_ENTRY] = (size_t)laddr(&app, ehdr->e_entry);
		/* Find the name that would have been used for the dynamic
		 * linker had ldd not taken its place. */
		if (ldd_mode) {
			for (i=0; i<app.phnum; i++) {
				if (app.phdr[i].p_type == PT_INTERP)
					ldso.name = laddr(&app, app.phdr[i].p_vaddr);
			}
			dprintf(1, "\t%s (%p)\n", ldso.name, ldso.base);
		}
	}
	if (app.tls.size) {
		libc.tls_head = tls_tail = &app.tls;
		app.tls_id = tls_cnt = 1;
#ifdef TLS_ABOVE_TP
		app.tls.offset = GAP_ABOVE_TP;
		app.tls.offset += (-GAP_ABOVE_TP + (uintptr_t)app.tls.image)
			& (app.tls.align-1);
		tls_offset = app.tls.offset + app.tls.size;
#else
		tls_offset = app.tls.offset = app.tls.size
			+ ( -((uintptr_t)app.tls.image + app.tls.size)
			& (app.tls.align-1) );
#endif
		tls_align = MAXP2(tls_align, app.tls.align);
	}
	decode_dyn(&app);
	if (DL_FDPIC) {
		makefuncdescs(&app);
		if (!app.loadmap) {
			app.loadmap = (void *)&app_dummy_loadmap;
			app.loadmap->nsegs = 1;
			app.loadmap->segs[0].addr = (size_t)app.map;
			app.loadmap->segs[0].p_vaddr = (size_t)app.map
				- (size_t)app.base;
			app.loadmap->segs[0].p_memsz = app.map_len;
		}
		argv[-3] = (void *)app.loadmap;
	}
	app.is_global = true;

	/* Initial dso chain consists only of the app. */
	head = tail = syms_tail = &app;

	/* Donate unused parts of app and library mapping to malloc */
	reclaim_gaps(&app);
	reclaim_gaps(&ldso);

	find_and_set_bss_name(&app);
	find_and_set_bss_name(&ldso);

	/* Load preload/needed libraries, add symbols to global namespace. */
	ldso.deps = (struct dso **)no_deps;
	/* Init g_is_asan */
	g_is_asan = false;
	LD_LOGD("__dls3 ldso.name:%{public}s.", ldso.name);
	/* Through ldso Name to judge whether the Asan function is enabled */
	if (strstr(ldso.name, "-asan")) {
		g_is_asan = true;
		LD_LOGD("__dls3 g_is_asan is true.");
	}
	/* Init all namespaces by config file. there is a default namespace always*/
	init_namespace(&app);

#ifdef LOAD_ORDER_RANDOMIZATION
	struct loadtasks *tasks = create_loadtasks();
	if (!tasks) {
		_exit(1);
	}
	if (env_preload) {
		load_preload(env_preload, get_default_ns(), tasks);
	}
	for (struct dso *q=head; q; q=q->next) {
		q->is_global = true;
	}
	preload_deps(&app, tasks);
	unmap_preloaded_sections(tasks);
	shuffle_loadtasks(tasks);
	run_loadtasks(tasks, NULL);
	free_loadtasks(tasks);
	assign_tls(app.next);
#else
	if (env_preload) load_preload(env_preload, get_default_ns());
	for (struct dso *q=head; q; q=q->next) {
		q->is_global = true;
	}
 	load_deps(&app, NULL);
#endif

	/* Set is_reloc_head_so_dep to true for all direct and indirect dependent sos of app, including app self. */
	for (struct dso *p=head; p; p=p->next) {
		p->is_reloc_head_so_dep = true;
		add_syms(p);
	}

	/* Attach to vdso, if provided by the kernel, last so that it does
	 * not become part of the global namespace.  */
	if (search_vec(auxv, &vdso_base, AT_SYSINFO_EHDR) && vdso_base) {
		Ehdr *ehdr = (void *)vdso_base;
		Phdr *phdr = vdso.phdr = (void *)(vdso_base + ehdr->e_phoff);
		vdso.phnum = ehdr->e_phnum;
		vdso.phentsize = ehdr->e_phentsize;
		for (i=ehdr->e_phnum; i; i--, phdr=(void *)((char *)phdr + ehdr->e_phentsize)) {
			if (phdr->p_type == PT_DYNAMIC)
				vdso.dynv = (void *)(vdso_base + phdr->p_offset);
			if (phdr->p_type == PT_LOAD)
				vdso.base = (void *)(vdso_base - phdr->p_vaddr + phdr->p_offset);
		}
		vdso.name = "";
		vdso.shortname = "linux-gate.so.1";
		vdso.relocated = 1;
		vdso.deps = (struct dso **)no_deps;
		decode_dyn(&vdso);
		vdso.prev = tail;
		tail->next = &vdso;
		tail = &vdso;
		vdso.namespace = get_default_ns();
		ns_add_dso(vdso.namespace, &vdso);
	}

	for (i=0; app.dynv[i]; i+=2) {
		if (!DT_DEBUG_INDIRECT && app.dynv[i]==DT_DEBUG)
			app.dynv[i+1] = (size_t)&debug;
		if (DT_DEBUG_INDIRECT && app.dynv[i]==DT_DEBUG_INDIRECT) {
			size_t *ptr = (size_t *) app.dynv[i+1];
			*ptr = (size_t)&debug;
		}
	}

	/* This must be done before final relocations, since it calls
	 * malloc, which may be provided by the application. Calling any
	 * application code prior to the jump to its entry point is not
	 * valid in our model and does not work with FDPIC, where there
	 * are additional relocation-like fixups that only the entry point
	 * code can see to perform. */
	main_ctor_queue = queue_ctors(&app);

	/* Initial TLS must also be allocated before final relocations
	 * might result in calloc being a call to application code. */
	update_tls_size();
	void *initial_tls = builtin_tls;
	if (libc.tls_size > sizeof builtin_tls || tls_align > MIN_TLS_ALIGN) {
		initial_tls = calloc(libc.tls_size, 1);
		if (!initial_tls) {
			dprintf(2, "%s: Error getting %zu bytes thread-local storage: %m\n",
				argv[0], libc.tls_size);
			_exit(127);
		}
	}
	static_tls_cnt = tls_cnt;

	/* The main program must be relocated LAST since it may contain
	 * copy relocations which depend on libraries' relocations. */
	reloc_all(app.next, NULL);
	reloc_all(&app, NULL);
	for (struct dso *q=head; q; q=q->next) {
		q->is_reloc_head_so_dep = false;
	}

	/* Actual copying to new TLS needs to happen after relocations,
	 * since the TLS images might have contained relocated addresses. */
	if (initial_tls != builtin_tls) {
		pthread_t self = __pthread_self();
		pthread_t td = __copy_tls(initial_tls);
		if (__init_tp(td) < 0) {
			a_crash();
		}
		td->tsd = self->tsd;
	} else {
		size_t tmp_tls_size = libc.tls_size;
		pthread_t self = __pthread_self();
		/* Temporarily set the tls size to the full size of
		 * builtin_tls so that __copy_tls will use the same layout
		 * as it did for before. Then check, just to be safe. */
		libc.tls_size = sizeof builtin_tls;
		if (__copy_tls((void*)builtin_tls) != self) a_crash();
		libc.tls_size = tmp_tls_size;
	}

	if (init_cfi_shadow(head, &ldso) == CFI_FAILED) {
		error("[%s] init_cfi_shadow failed: %m", __FUNCTION__);
	}

	if (ldso_fail) _exit(127);
	if (ldd_mode) _exit(0);

	/* Determine if malloc was interposed by a replacement implementation
	 * so that calloc and the memalign family can harden against the
	 * possibility of incomplete replacement. */
	if (find_sym(head, "malloc", 1).dso != &ldso)
		__malloc_replaced = 1;
	if (find_sym(head, "aligned_alloc", 1).dso != &ldso)
		__aligned_alloc_replaced = 1;

	/* Switch to runtime mode: any further failures in the dynamic
	 * linker are a reportable failure rather than a fatal startup
	 * error. */
	runtime = 1;

	sync_with_debugger();

	if (replace_argv0) argv[0] = replace_argv0;

#ifdef USE_GWP_ASAN
	init_gwp_asan_by_libc(false);
#endif

	errno = 0;

	CRTJMP((void *)aux[AT_ENTRY], argv-1);
	for(;;);
}

static void prepare_lazy(struct dso *p)
{
	size_t dyn[DYN_CNT], n, flags1=0;
	decode_vec(p->dynv, dyn, DYN_CNT);
	search_vec(p->dynv, &flags1, DT_FLAGS_1);
	if (dyn[DT_BIND_NOW] || (dyn[DT_FLAGS] & DF_BIND_NOW) || (flags1 & DF_1_NOW))
		return;
	n = dyn[DT_RELSZ]/2 + dyn[DT_RELASZ]/3 + dyn[DT_PLTRELSZ]/2 + 1;
	if (NEED_MIPS_GOT_RELOCS) {
		size_t j=0; search_vec(p->dynv, &j, DT_MIPS_GOTSYM);
		size_t i=0; search_vec(p->dynv, &i, DT_MIPS_SYMTABNO);
		n += i-j;
	}
	p->lazy = calloc(n, 3*sizeof(size_t));
	if (!p->lazy) {
		error("Error preparing lazy relocation for %s: %m", p->name);
		longjmp(*rtld_fail, 1);
	}
	p->lazy_next = lazy_head;
	lazy_head = p;
}

static void *dlopen_post(struct dso* p, int mode) {
	if (p == NULL) {
		return p;
	}
	bool is_dlclose_debug = false;
	if (is_dlclose_debug_enable()) {
		is_dlclose_debug = true;
	}
	p->nr_dlopen++;
	if (is_dlclose_debug) {
		LD_LOGE("[dlclose]: %{public}s nr_dlopen++ when dlopen %{public}s, nr_dlopen:%{public}d ",
				p->name, p->name, p->nr_dlopen);
	}
	if (p->bfs_built) {
		for (int i = 0; p->deps[i]; i++) {
			p->deps[i]->nr_dlopen++;
			if (is_dlclose_debug) {
				LD_LOGE("[dlclose]: %{public}s nr_dlopen++ when dlopen %{public}s, nr_dlopen:%{public}d",
						p->deps[i]->name, p->name, p->deps[i]->nr_dlopen);
			}
			if (mode & RTLD_NODELETE) {
				p->deps[i]->flags |= DSO_FLAGS_NODELETE;
			}
		}
	}

#ifdef HANDLE_RANDOMIZATION
	void *handle = assign_valid_handle(p);
	if (handle == NULL) {
		LD_LOGE("dlopen_post: generate random handle failed");
		do_dlclose(p);
	}

	return handle;
#endif

	return p;
}

static char *dlopen_permitted_list[] =
{
	"default",
	"ndk",
};

#define PERMITIED_TARGET  "nweb_ns"
static bool in_permitted_list(char *caller, char *target)
{
	for (int i = 0; i < sizeof(dlopen_permitted_list)/sizeof(char*); i++) {
		if (strcmp(dlopen_permitted_list[i], caller) == 0) {
			return true;
		}
	}

	if (strcmp(PERMITIED_TARGET, target) == 0) {
		return true;
	}

	return false;
}

static bool is_permitted(const void *caller_addr, char *target)
{
	struct dso *caller;
	ns_t *ns;
	caller = (struct dso *)addr2dso((size_t)caller_addr);
	if ((caller == NULL) || (caller->namespace == NULL)) {
		LD_LOGE("caller ns get error");
		return false;
	}

	ns = caller->namespace;
	if (in_permitted_list(ns->ns_name, target) == false) {
		LD_LOGE("caller ns: %{public}s have no permission, target is %{public}s", ns->ns_name, target);
		return false;
	}

	return true;
}

/* Add namespace function.
 * Some limitations come from sanitizer:
 *  Sanitizer requires this interface to be exposed.
 *  Pay attention to call __builtin_return_address in this interface because sanitizer can hook and call this interface.
 */
void *dlopen_impl(
	const char *file, int mode, const char *namespace, const void *caller_addr, const dl_extinfo *extinfo)
{
	struct dso *volatile p, *orig_tail, *orig_syms_tail, *orig_lazy_head, *next;
	struct tls_module *orig_tls_tail;
	size_t orig_tls_cnt, orig_tls_offset, orig_tls_align;
	size_t i;
	int cs;
	jmp_buf jb;
	struct dso **volatile ctor_queue = 0;
	ns_t *ns;
	struct dso *caller;
	bool reserved_address = false;
	bool reserved_address_recursive = false;
	struct reserved_address_params reserved_params = {0};
#ifdef LOAD_ORDER_RANDOMIZATION
	struct loadtasks *tasks = NULL;
	struct loadtask *task = NULL;
	bool is_task_appended = false;
#endif

	if (!file) {
		LD_LOGD("dlopen_impl file is null, return head.");
		return dlopen_post(head, mode);
	}

	if (extinfo) {
		reserved_address_recursive = extinfo->flag & DL_EXT_RESERVED_ADDRESS_RECURSIVE;
		if (extinfo->flag & DL_EXT_RESERVED_ADDRESS) {
			reserved_address = true;
			reserved_params.start_addr = extinfo->reserved_addr;
			reserved_params.reserved_size = extinfo->reserved_size;
			reserved_params.must_use_reserved = true;
			reserved_params.reserved_address_recursive = reserved_address_recursive;
		} else if (extinfo->flag & DL_EXT_RESERVED_ADDRESS_HINT) {
			reserved_address = true;
			reserved_params.start_addr = extinfo->reserved_addr;
			reserved_params.reserved_size = extinfo->reserved_size;
			reserved_params.must_use_reserved = false;
			reserved_params.reserved_address_recursive = reserved_address_recursive;
		}
	}

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	pthread_rwlock_wrlock(&lock);
	__inhibit_ptc();
	trace_marker_reset();
	trace_marker_begin(HITRACE_TAG_MUSL, "dlopen: ", file);

	/* When namespace does not exist, use caller's namespce
	 * and when caller does not exist, use default namespce. */
	caller = (struct dso *)addr2dso((size_t)caller_addr);
	ns = find_ns_by_name(namespace);
	if (!ns) ns = ((caller && caller->namespace) ? caller->namespace : get_default_ns());

	p = 0;
	if (shutting_down) {
		error("Cannot dlopen while program is exiting.");
		goto end;
	}
	orig_tls_tail = tls_tail;
	orig_tls_cnt = tls_cnt;
	orig_tls_offset = tls_offset;
	orig_tls_align = tls_align;
	orig_lazy_head = lazy_head;
	orig_syms_tail = syms_tail;
	orig_tail = tail;
	noload = mode & RTLD_NOLOAD;

	rtld_fail = &jb;
	if (setjmp(*rtld_fail)) {
		/* Clean up anything new that was (partially) loaded */
		revert_syms(orig_syms_tail);
		for (p=orig_tail->next; p; p=next) {
			next = p->next;
			while (p->td_index) {
				void *tmp = p->td_index->next;
				free(p->td_index);
				p->td_index = tmp;
			}
			free(p->funcdescs);
			if (p->rpath != p->rpath_orig)
				free(p->rpath);
			if (p->deps) {
				for (int i = 0; i < p->ndeps_direct; i++) {
					remove_dso_parent(p->deps[i], p);
				}
			}
			free(p->deps);
			dlclose_ns(p);
			unmap_library(p);
			if (p->parents) {
				free(p->parents);
			}
			free_reloc_can_search_dso(p);
		}
		for (p=orig_tail->next; p; p=next) {
			next = p->next;
			free(p);
		}
		free(ctor_queue);
		ctor_queue = 0;
		if (!orig_tls_tail) libc.tls_head = 0;
		tls_tail = orig_tls_tail;
		if (tls_tail) tls_tail->next = 0;
		tls_cnt = orig_tls_cnt;
		tls_offset = orig_tls_offset;
		tls_align = orig_tls_align;
		lazy_head = orig_lazy_head;
		tail = orig_tail;
		tail->next = 0;
		p = 0;
		goto end;
	} else {
#ifdef LOAD_ORDER_RANDOMIZATION
		tasks = create_loadtasks();
		if (!tasks) {
			LD_LOGE("dlopen_impl create loadtasks failed");
			goto end;
		}
		task = create_loadtask(file, head, ns, true);
		if (!task) {
			LD_LOGE("dlopen_impl create loadtask failed");
			goto end;
		}
		trace_marker_begin(HITRACE_TAG_MUSL, "loading: entry so", file);
		if (!load_library_header(task)) {
			error(noload ?
				"Library %s is not already loaded" :
				"Error loading shared library %s: %m",
				file);
			LD_LOGE("dlopen_impl load library header failed for %{public}s", task->name);
			trace_marker_end(HITRACE_TAG_MUSL); // "loading: entry so" trace end.
			goto end;
		}
		if (reserved_address) {
			reserved_params.target = task->p;
		}
	}
	if (!task->p) {
		LD_LOGE("dlopen_impl load library failed for %{public}s", task->name);
		error(noload ?
			"Library %s is not already loaded" :
			"Error loading shared library %s: %m",
			file);
		trace_marker_end(HITRACE_TAG_MUSL); // "loading: entry so" trace end.
		goto end;
	}
	if (!task->isloaded) {
		is_task_appended = append_loadtasks(tasks, task);
	}
	preload_deps(task->p, tasks);
	unmap_preloaded_sections(tasks);
	if (!reserved_address_recursive) {
		shuffle_loadtasks(tasks);
	}
	run_loadtasks(tasks, reserved_address ? &reserved_params : NULL);
	p = task->p;
	if (!task->isloaded) {
		assign_tls(p);
	}
	if (!is_task_appended) {
		free_task(task);
		task = NULL;
	}
	free_loadtasks(tasks);
	tasks = NULL;
#else
		trace_marker_begin(HITRACE_TAG_MUSL, "loading: entry so", file);
		p = load_library(file, head, ns, true, reserved_address ? &reserved_params : NULL);
	}

	if (!p) {
		error(noload ?
			"Library %s is not already loaded" :
			"Error loading shared library %s: %m",
			file);
		trace_marker_end(HITRACE_TAG_MUSL); // "loading: entry so" trace end.
		goto end;
	}
	/* First load handling */
	load_deps(p, reserved_address && reserved_address_recursive ? &reserved_params : NULL);
#endif
	trace_marker_end(HITRACE_TAG_MUSL); // "loading: entry so" trace end.
	extend_bfs_deps(p);
	pthread_mutex_lock(&init_fini_lock);
	int constructed = p->constructed;
	pthread_mutex_unlock(&init_fini_lock);
	if (!constructed) ctor_queue = queue_ctors(p);
	if (!p->relocated && (mode & RTLD_LAZY)) {
		prepare_lazy(p);
		for (i=0; p->deps[i]; i++)
			if (!p->deps[i]->relocated)
				prepare_lazy(p->deps[i]);
	}
	if (!p->relocated || (mode & RTLD_GLOBAL)) {
		/* Make new symbols global, at least temporarily, so we can do
		 * relocations. If not RTLD_GLOBAL, this is reverted below. */
		add_syms(p);
		/* Set is_reloc_head_so_dep to true for all direct and indirect dependent sos of p, including p self. */
		p->is_reloc_head_so_dep = true;
		for (i=0; p->deps[i]; i++) {
			p->deps[i]->is_reloc_head_so_dep = true;
			add_syms(p->deps[i]);
		}
	}
	struct dso *reloc_head_so = p;
	trace_marker_begin(HITRACE_TAG_MUSL, "linking: entry so", p->name);
	if (!p->relocated) {
		reloc_all(p, extinfo);
	}
	trace_marker_end(HITRACE_TAG_MUSL);
	reloc_head_so->is_reloc_head_so_dep = false;
	for (size_t i=0; reloc_head_so->deps[i]; i++) {
		reloc_head_so->deps[i]->is_reloc_head_so_dep = false;
	}

	/* If RTLD_GLOBAL was not specified, undo any new additions
	 * to the global symbol table. This is a nop if the library was
	 * previously loaded and already global. */
	if (!(mode & RTLD_GLOBAL))
		revert_syms(orig_syms_tail);

	/* Processing of deferred lazy relocations must not happen until
	 * the new libraries are committed; otherwise we could end up with
	 * relocations resolved to symbol definitions that get removed. */
	redo_lazy_relocs();

	if (map_dso_to_cfi_shadow(p) == CFI_FAILED) {
		error("[%s] map_dso_to_cfi_shadow failed: %m", __FUNCTION__);
		longjmp(*rtld_fail, 1);
	}

	if (mode & RTLD_NODELETE) {
		p->flags |= DSO_FLAGS_NODELETE;
	}

	update_tls_size();
	if (tls_cnt != orig_tls_cnt)
		install_new_tls();

	if (orig_tail != tail) {
		notify_addition_to_debugger(orig_tail->next);
	}

	orig_tail = tail;

	p = dlopen_post(p, mode);
end:
#ifdef LOAD_ORDER_RANDOMIZATION
	if (!is_task_appended) {
		free_task(task);
	}
	free_loadtasks(tasks);
#endif
	__release_ptc();
	if (p) gencnt++;
	pthread_rwlock_unlock(&lock);
	if (ctor_queue) {
		do_init_fini(ctor_queue);
		free(ctor_queue);
	}
	pthread_setcancelstate(cs, 0);
	trace_marker_end(HITRACE_TAG_MUSL); // "dlopen: " trace end.
	return p;
}

void *dlopen(const char *file, int mode)
{
	const void *caller_addr = __builtin_return_address(0);
	musl_log_reset();
	ld_log_reset();
	LD_LOGI("dlopen file:%{public}s, mode:%{public}x ,caller_addr:%{public}p .", file, mode, caller_addr);
	return dlopen_impl(file, mode, NULL, caller_addr, NULL);
}

void dlns_init(Dl_namespace *dlns, const char *name)
{
	if (!dlns) {
		return;
	}
	if (!name) {
		dlns->name[0] = 0;
		return;
	}

	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, name) == false) {
		return;
	}

	snprintf(dlns->name, sizeof dlns->name, name);
	LD_LOGI("dlns_init dlns->name:%{public}s .", dlns->name);
}

int dlns_get(const char *name, Dl_namespace *dlns)
{
	if (!dlns) {
		LD_LOGE("dlns_get dlns is null.");
		return EINVAL;
	}
	int ret = 0;
	ns_t *ns = NULL;
	pthread_rwlock_rdlock(&lock);
	if (!name) {
		struct dso *caller;
		const void *caller_addr = __builtin_return_address(0);
		caller = (struct dso *)addr2dso((size_t)caller_addr);
		ns = ((caller && caller->namespace) ? caller->namespace : get_default_ns());
		(void)snprintf(dlns->name, sizeof dlns->name, ns->ns_name);
		LD_LOGI("dlns_get name is null, current dlns dlns->name:%{public}s.", dlns->name);
	} else {
		ns = find_ns_by_name(name);
		if (ns) {
			(void)snprintf(dlns->name, sizeof dlns->name, ns->ns_name);
			LD_LOGI("dlns_get found ns, current dlns dlns->name:%{public}s.", dlns->name);
		} else {
			LD_LOGI("dlns_get not found ns! name:%{public}s.", name);
			ret = ENOKEY;
		}
	}
	pthread_rwlock_unlock(&lock);
	return ret;
}

void *dlopen_ns(Dl_namespace *dlns, const char *file, int mode)
{
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, dlns->name) == false) {
		return NULL;
	}

	musl_log_reset();
	ld_log_reset();
	LD_LOGI("dlopen_ns file:%{public}s, mode:%{public}x , caller_addr:%{public}p , dlns->name:%{public}s.",
		file,
		mode,
		caller_addr,
		dlns ? dlns->name : "NULL");
	return dlopen_impl(file, mode, dlns->name, caller_addr, NULL);
}

void *dlopen_ns_ext(Dl_namespace *dlns, const char *file, int mode, const dl_extinfo *extinfo)
{
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, dlns->name) == false) {
		return NULL;
	}

	musl_log_reset();
	ld_log_reset();
	LD_LOGI("dlopen_ns_ext file:%{public}s, mode:%{public}x , caller_addr:%{public}p , "
			"dlns->name:%{public}s. , extinfo->flag:%{public}x",
		file,
		mode,
		caller_addr,
		dlns->name,
		extinfo ? extinfo->flag : 0);
	return dlopen_impl(file, mode, dlns->name, caller_addr, extinfo);
}

int dlns_create2(Dl_namespace *dlns, const char *lib_path, int flags)
{
	if (!dlns) {
		LD_LOGE("dlns_create2 dlns is null.");
		return EINVAL;
	}
	ns_t *ns;

	pthread_rwlock_wrlock(&lock);
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, dlns->name) == false) {
		pthread_rwlock_unlock(&lock);
		return EPERM;
	}

	ns = find_ns_by_name(dlns->name);
	if (ns) {
		LD_LOGE("dlns_create2 ns is exist.");
		pthread_rwlock_unlock(&lock);
		return EEXIST;
	}
	ns = ns_alloc();
	if (!ns) {
		LD_LOGE("dlns_create2 no memery.");
		pthread_rwlock_unlock(&lock);
		return ENOMEM;
	}
	ns_set_name(ns, dlns->name);
	ns_set_flag(ns, flags);
	ns_add_dso(ns, get_default_ns()->ns_dsos->dsos[0]); /* add main app to this namespace*/
	nslist_add_ns(ns); /* add ns to list*/
	ns_set_lib_paths(ns, lib_path);

	if ((flags & CREATE_INHERIT_DEFAULT) != 0) {
		ns_add_inherit(ns, get_default_ns(), NULL);
	}

	if ((flags & CREATE_INHERIT_CURRENT) != 0) {
		struct dso *caller;
		caller_addr = __builtin_return_address(0);
		caller = (struct dso *)addr2dso((size_t)caller_addr);
		if (caller && caller->namespace) {
			ns_add_inherit(ns, caller->namespace, NULL);
		}
	}

	LD_LOGI("dlns_create2:"
			"ns_name: %{public}s ,"
			"separated:%{public}d ,"
			"lib_paths:%{public}s ",
			ns->ns_name, ns->separated, ns->lib_paths);
	pthread_rwlock_unlock(&lock);

	return 0;
}

int dlns_create(Dl_namespace *dlns, const char *lib_path)
{
	LD_LOGI("dlns_create lib_paths:%{public}s", lib_path);
	return dlns_create2(dlns, lib_path, CREATE_INHERIT_DEFAULT);
}

int dlns_inherit(Dl_namespace *dlns, Dl_namespace *inherited, const char *shared_libs)
{
	if (!dlns || !inherited) {
		LD_LOGE("dlns_inherit dlns or inherited is null.");
		return EINVAL;
	}

	pthread_rwlock_wrlock(&lock);
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, dlns->name) == false) {
		pthread_rwlock_unlock(&lock);
		return EPERM;
	}

	ns_t* ns = find_ns_by_name(dlns->name);
	ns_t* ns_inherited = find_ns_by_name(inherited->name);
	if (!ns || !ns_inherited) {
		LD_LOGE("dlns_inherit ns or ns_inherited is not found.");
		pthread_rwlock_unlock(&lock);
		return ENOKEY;
	}
	ns_add_inherit(ns, ns_inherited, shared_libs);
	pthread_rwlock_unlock(&lock);

	return 0;
}

static void dlclose_ns(struct dso *p)
{
	if (!p) return;
	ns_t * ns = p->namespace;
	if (!ns||!ns->ns_dsos) return;
	for (size_t i=0; i<ns->ns_dsos->num; i++) {
		if (p == ns->ns_dsos->dsos[i]) {
			for (size_t j=i+1; j<ns->ns_dsos->num; j++) {
				ns->ns_dsos->dsos[j-1] = ns->ns_dsos->dsos[j];
			}
			ns->ns_dsos->num--;
			return;
		}
	}
}

hidden int __dl_invalid_handle(void *h)
{
	struct dso *p;
	for (p=head; p; p=p->next) if (h==p) return 0;
	error("Invalid library handle %p", (void *)h);
	return 1;
}

void *addr2dso(size_t a)
{
	struct dso *p;
	size_t i;
	for (p=head; p; p=p->next) {
		if (a < p->map || a - (size_t)p->map >= p->map_len) continue;
		Phdr *ph = p->phdr;
		size_t phcnt = p->phnum;
		size_t entsz = p->phentsize;
		size_t base = (size_t)p->base;
		for (; phcnt--; ph=(void *)((char *)ph+entsz)) {
			if (ph->p_type != PT_LOAD) continue;
			if (a-base-ph->p_vaddr < ph->p_memsz)
				return p;
		}
		if (a-(size_t)p->map < p->map_len)
			return 0;
	}
	return 0;
}

static void *do_dlsym(struct dso *p, const char *s, const char *v, void *ra)
{
	int use_deps = 0;
	bool ra2dso = false;
	ns_t *ns = NULL;
	struct dso *caller = NULL;
	if (p == head || p == RTLD_DEFAULT) {
		p = head;
		ra2dso = true;
	} else if (p == RTLD_NEXT) {
		p = addr2dso((size_t)ra);
		if (!p) p=head;
		p = p->next;
		ra2dso = true;
#ifndef HANDLE_RANDOMIZATION
	} else if (__dl_invalid_handle(p)) {
		return 0;
#endif
	} else {
		use_deps = 1;
		ns = p->namespace;
	}
	if (ra2dso) {
		caller = (struct dso *)addr2dso((size_t)ra);
		if (caller && caller->namespace) {
			ns = caller->namespace;
		}
	}
	trace_marker_begin(HITRACE_TAG_MUSL, "dlsym: ", (s == NULL ? "(NULL)" : s));
	struct verinfo verinfo = { .s = s, .v = v, .use_vna_hash = false };
	struct symdef def = use_deps ? find_sym_by_deps(p, &verinfo, 0, ns) :
		find_sym2(p, &verinfo, 0, use_deps, ns);
	trace_marker_end(HITRACE_TAG_MUSL);
	if (!def.sym) {
		LD_LOGW("do_dlsym failed: symbol not found. so=%{public}s s=%{public}s v=%{public}s",
			(p == NULL ? "NULL" : p->name), s, v);
		error("do_dlsym failed: Symbol not found: %s, version: %s so=%s",
			s, strlen(v) > 0 ? v : "null", (p == NULL ? "NULL" : p->name));
		return 0;
	}
	if ((def.sym->st_info&0xf) == STT_TLS)
		return __tls_get_addr((tls_mod_off_t []){def.dso->tls_id, def.sym->st_value-DTP_OFFSET});
	if (DL_FDPIC && (def.sym->st_info&0xf) == STT_FUNC)
		return def.dso->funcdescs + (def.sym - def.dso->syms);
	return laddr(def.dso, def.sym->st_value);
}

extern int invalidate_exit_funcs(struct dso *p);

static int so_can_unload(struct dso *p, int check_flag)
{
	if ((check_flag & UNLOAD_COMMON_CHECK) != 0) {
		if (__dl_invalid_handle(p)) {
			LD_LOGE("[dlclose]: invalid handle %{public}p", p);
			error("[dlclose]: Handle is invalid.");
			return 0;
		}

		if (!p->by_dlopen) {
			LD_LOGD("[dlclose]: skip unload %{public}s because it's not loaded by dlopen", p->name);
			return 0;
		}

		/* dso is marked  as RTLD_NODELETE library, do nothing here. */
		if ((p->flags & DSO_FLAGS_NODELETE) != 0) {
			LD_LOGD("[dlclose]: skip unload %{public}s because flags is RTLD_NODELETE", p->name);
			return 0;
		}
	}

	if ((check_flag & UNLOAD_NR_DLOPEN_CHECK) != 0) {
		if (p->nr_dlopen > 0) {
			LD_LOGD("[dlclose]: skip unload %{public}s because nr_dlopen=%{public}d > 0", p->name, p->nr_dlopen);
			return 0;
		}
	}

	return 1;
}

static int dlclose_post(struct dso *p)
{
	if (p == NULL) {
		return -1;
	}
#ifdef ENABLE_HWASAN
		if (libc.unload_hook) {
			libc.unload_hook((unsigned long int)p->base, p->phdr, p->phnum);
		}
#endif
	unmap_library(p);
	if (p->parents) {
		free(p->parents);
	}
	free_reloc_can_search_dso(p);
	if (p->tls.size == 0) {
		free(p);
	}

	return 0;
}

static int dlclose_impl(struct dso *p)
{
	struct dso *d;

	trace_marker_reset();
	trace_marker_begin(HITRACE_TAG_MUSL, "dlclose", p->name);

	/* remove dso symbols from global list */
	if (p->syms_next) {
		for (d = head; d->syms_next != p; d = d->syms_next)
			; /* NOP */
		d->syms_next = p->syms_next;
	} else if (p == syms_tail) {
		for (d = head; d->syms_next != p; d = d->syms_next)
			; /* NOP */
		d->syms_next = NULL;
		syms_tail = d;
	}

	/* remove dso from lazy list if needed */
	if (p == lazy_head) {
		lazy_head = p->lazy_next;
	} else if (p->lazy_next) {
		for (d = lazy_head; d->lazy_next != p; d = d->lazy_next)
			; /* NOP */
		d->lazy_next = p->lazy_next;
	}

	pthread_mutex_lock(&init_fini_lock);
	/* remove dso from fini list */
	if (p == fini_head) {
		fini_head = p->fini_next;
	} else if (p->fini_next) {
		for (d = fini_head; d->fini_next != p; d = d->fini_next)
			; /* NOP */
		d->fini_next = p->fini_next;
	}
	pthread_mutex_unlock(&init_fini_lock);

	/* empty tls image */
	if (p->tls.size != 0) {
		p->tls.image = NULL;
	}

	/* remove dso from global dso list */
	if (p == tail) {
		tail = p->prev;
		tail->next = NULL;
	} else {
		p->next->prev = p->prev;
		p->prev->next = p->next;
	}

	/* remove dso from namespace */
	dlclose_ns(p);

	/* */
	void* handle = find_handle_by_dso(p);
	if (handle) {
		remove_handle_node(handle);
	}

	/* after destruct, invalidate atexit funcs which belong to this dso */
#if (defined(FEATURE_ATEXIT_CB_PROTECT))
	invalidate_exit_funcs(p);
#endif

	notify_remove_to_debugger(p);

	unmap_dso_from_cfi_shadow(p);
	
	if (p->lazy != NULL)
		free(p->lazy);
	if (p->deps != no_deps)
		free(p->deps);

	trace_marker_end(HITRACE_TAG_MUSL);

	return 0;
}

static int do_dlclose(struct dso *p)
{
	struct dso_entry *ef = NULL;
	struct dso_entry *ef_tmp = NULL;
	size_t n;
	int unload_check_result;
	TAILQ_HEAD(unload_queue, dso_entry) unload_queue;
	TAILQ_HEAD(need_unload_queue, dso_entry) need_unload_queue;
	unload_check_result = so_can_unload(p, UNLOAD_COMMON_CHECK);
	if (unload_check_result != 1) {
		return unload_check_result;
	}
	// Unconditionally subtract 1 because unconditionally add 1 at dlopen_post.
	if (p->nr_dlopen > 0) {
		--(p->nr_dlopen);
	} else {
		LD_LOGE("[dlclose]: number of dlopen and dlclose of %{public}s doesn't match when dlclose %{public}s",
		        p->name, p->name);
		return 0;
	}
	
	if (p->bfs_built) {
		for (int i = 0; p->deps[i]; i++) {
			if (p->deps[i]->nr_dlopen > 0) {
				p->deps[i]->nr_dlopen--;
			} else {
				LD_LOGE("[dlclose]: number of dlopen and dlclose of %{public}s doesn't match when dlclose %{public}s",
						p->deps[i]->name, p->name);
				return 0;
			}
		}
	}
	unload_check_result = so_can_unload(p, UNLOAD_NR_DLOPEN_CHECK);
	if (unload_check_result != 1) {
		return unload_check_result;
	}
	TAILQ_INIT(&unload_queue);
	TAILQ_INIT(&need_unload_queue);
	struct dso_entry *start_entry = (struct dso_entry *)malloc(sizeof(struct dso_entry));
	start_entry->dso = p;
	TAILQ_INSERT_TAIL(&unload_queue, start_entry, entries);

	while (!TAILQ_EMPTY(&unload_queue)) {
		struct dso_entry *ecur = TAILQ_FIRST(&unload_queue);
		struct dso *cur = ecur->dso;
		TAILQ_REMOVE(&unload_queue, ecur, entries);
		bool already_in_need_unload_queue = false;
		TAILQ_FOREACH(ef, &need_unload_queue, entries) {
			if (ef->dso == cur) {
				already_in_need_unload_queue = true;
				break;
			}
		}
		if (already_in_need_unload_queue) {
			continue;
		}
		TAILQ_INSERT_TAIL(&need_unload_queue, ecur, entries);
		for (int i = 0; i < cur->ndeps_direct; i++) {
			remove_dso_parent(cur->deps[i], cur);
			if ((cur->deps[i]->parents_count == 0) && (so_can_unload(cur->deps[i], UNLOAD_ALL_CHECK) == 1)) {
				bool already_in_unload_queue = false;
				TAILQ_FOREACH(ef, &unload_queue, entries) {
					if (ef->dso == cur->deps[i]) {
						already_in_unload_queue = true;
						break;
					}
				}
				if (already_in_unload_queue) {
					continue;
				}

				struct dso_entry *edeps = (struct dso_entry *)malloc(sizeof(struct dso_entry));
				edeps->dso = cur->deps[i];
				TAILQ_INSERT_TAIL(&unload_queue, edeps, entries);
			}
		} /* for */
	} /* while */

	if (is_dlclose_debug_enable()) {
		TAILQ_FOREACH(ef, &need_unload_queue, entries) {
			LD_LOGE("[dlclose]: unload %{public}s succeed when dlclose %{public}s", ef->dso->name, p->name);
		}
		for (size_t deps_num = 0; p->deps[deps_num]; deps_num++) {
			bool ready_to_unload = false;
			TAILQ_FOREACH(ef, &need_unload_queue, entries) {
				if (ef->dso == p->deps[deps_num]) {
					ready_to_unload = true;
					break;
				}
			}
			if (!ready_to_unload) {
				LD_LOGE("[dlclose]: unload %{public}s failed when dlclose %{public}s,"
				        "nr_dlopen:%{public}d, by_dlopen:%{public}d, parents_count:%{public}d",
						p->deps[deps_num]->name, p->name, p->deps[deps_num]->nr_dlopen,
						p->deps[deps_num]->by_dlopen, p->deps[deps_num]->parents_count);
			}
		}
	}

	TAILQ_FOREACH(ef, &need_unload_queue, entries) {
		dlclose_impl(ef->dso);
	}

	TAILQ_FOREACH(ef, &need_unload_queue, entries) {
		/* call destructors if needed */
		pthread_mutex_lock(&init_fini_lock);
		int constructed = ef->dso->constructed;
		pthread_mutex_unlock(&init_fini_lock);

		if (constructed) {
			size_t dyn[DYN_CNT];
			decode_vec(ef->dso->dynv, dyn, DYN_CNT);
			if (dyn[0] & (1<<DT_FINI_ARRAY)) {
				n = dyn[DT_FINI_ARRAYSZ] / sizeof(size_t);
				size_t *fn = (size_t *)laddr(ef->dso, dyn[DT_FINI_ARRAY]) + n;
				trace_marker_begin(HITRACE_TAG_MUSL, "calling destructors:", ef->dso->name);

				pthread_rwlock_unlock(&lock);
				while (n--)
					((void (*)(void))*--fn)();
				pthread_rwlock_wrlock(&lock);

				trace_marker_end(HITRACE_TAG_MUSL);
			}
			pthread_mutex_lock(&init_fini_lock);
			ef->dso->constructed = 0;
			pthread_mutex_unlock(&init_fini_lock);
		}
	}
	// Unload all sos at the end because weak symbol may cause later unloaded so to access the previous so's function.
	TAILQ_FOREACH(ef, &need_unload_queue, entries) {
		dlclose_post(ef->dso);
	}
	// Free dso_entry.
	TAILQ_FOREACH_SAFE(ef, &need_unload_queue, entries, ef_tmp) {
		if (ef) {
			free(ef);
		}
	}

	return 0;
}

hidden int __dlclose(void *p)
{
	int rc;
	pthread_rwlock_wrlock(&lock);
	if (shutting_down) {
		error("Cannot dlclose while program is exiting.");
		pthread_rwlock_unlock(&lock);
		return -1;
	}
#ifdef HANDLE_RANDOMIZATION
	struct dso *dso = find_dso_by_handle(p);
	if (dso == NULL) {
		errno = EINVAL;
		error("Handle is invalid.");
		LD_LOGE("Handle is not find.");
		pthread_rwlock_unlock(&lock);
		return -1;
	}
	rc = do_dlclose(dso);
#else
	rc = do_dlclose(p);
#endif
	pthread_rwlock_unlock(&lock);
	return rc;
}

static inline int sym_is_matched(const Sym* sym, size_t addr_offset_so) {
	return sym->st_value &&
		(1<<(sym->st_info&0xf) != STT_TLS) &&
		(addr_offset_so >= sym->st_value) &&
		(addr_offset_so < sym->st_value + sym->st_size);
}

static inline Sym* find_addr_by_elf(size_t addr_offset_so, struct dso *p) {
	uint32_t nsym = p->hashtab[1];
	Sym *sym = p->syms;
	for (; nsym; nsym--, sym++) {
		if (sym_is_matched(sym, addr_offset_so)) {
			return sym;
		}
	}

	return NULL;
}

static inline Sym* find_addr_by_gnu(size_t addr_offset_so, struct dso *p) {

	size_t i, nsym, first_hash_sym_index;
	uint32_t *hashval;
	Sym *sym_tab = p->syms;
	uint32_t *buckets= p->ghashtab + 4 + (p->ghashtab[2]*sizeof(size_t)/4);
	// Points to the first defined symbol, all symbols before it are undefined.
	first_hash_sym_index = buckets[0];
	Sym *sym = &sym_tab[first_hash_sym_index];

	// Get the location pointed by the last bucket.
	for (i = nsym = 0; i < p->ghashtab[0]; i++) {
		if (buckets[i] > nsym)
			nsym = buckets[i];
	}

	for (i = first_hash_sym_index; i < nsym; i++) {
		if (sym_is_matched(sym, addr_offset_so)) {
			return sym;
		}
		sym++;
	}

	// Start traversing the hash list from the position pointed to by the last bucket.
	if (nsym) {
		hashval = buckets + p->ghashtab[0] + (nsym - p->ghashtab[1]);
		do {
			nsym++;
			if (sym_is_matched(sym, addr_offset_so)) {
				return sym;
			}
			sym++;
		}
		while (!(*hashval++ & 1));
	}

	return NULL;
}


int dladdr(const void *addr_arg, Dl_info *info)
{
	size_t addr = (size_t)addr_arg;
	struct dso *p;
	Sym *match_sym = NULL;
	char *strings;

	pthread_rwlock_rdlock(&lock);
	p = addr2dso(addr);
	pthread_rwlock_unlock(&lock);

	if (!p) return 0;

	strings = p->strings;
	size_t addr_offset_so = addr - (size_t)p->base;

	info->dli_fname = p->name;
	info->dli_fbase = p->map;

	if (p->ghashtab) {
		match_sym = find_addr_by_gnu(addr_offset_so, p);

	} else {
		match_sym = find_addr_by_elf(addr_offset_so, p);
	}

	if (!match_sym) {
		info->dli_sname = 0;
		info->dli_saddr = 0;
		return 1;
	}
	info->dli_sname = strings + match_sym->st_name;
	info->dli_saddr = (void *)laddr(p, match_sym->st_value);
	return 1;
}

hidden void *__dlsym(void *restrict p, const char *restrict s, void *restrict ra)
{
	void *res;
	pthread_rwlock_rdlock(&lock);
#ifdef HANDLE_RANDOMIZATION
	if ((p != RTLD_DEFAULT) && (p != RTLD_NEXT)) {
		struct dso *dso = find_dso_by_handle(p);
		if (dso == NULL) {
			pthread_rwlock_unlock(&lock);
			return 0;
		}
		res = do_dlsym(dso, s, "", ra);
	} else {
		res = do_dlsym(p, s, "", ra);
	}
#else
	res = do_dlsym(p, s, "", ra);
#endif
	pthread_rwlock_unlock(&lock);
	return res;
}

hidden void *__dlvsym(void *restrict p, const char *restrict s, const char *restrict v, void *restrict ra)
{
	void *res;
	pthread_rwlock_rdlock(&lock);
#ifdef HANDLE_RANDOMIZATION
	if ((p != RTLD_DEFAULT) && (p != RTLD_NEXT)) {
		struct dso *dso = find_dso_by_handle(p);
		if (dso == NULL) {
			pthread_rwlock_unlock(&lock);
			return 0;
		}
		res = do_dlsym(dso, s, v, ra);
	} else {
		res = do_dlsym(p, s, v, ra);
	}
#else
	res = do_dlsym(p, s, v, ra);
#endif
	pthread_rwlock_unlock(&lock);
	return res;
}

hidden void *__dlsym_redir_time64(void *restrict p, const char *restrict s, void *restrict ra)
{
#if _REDIR_TIME64
	const char *suffix, *suffix2 = "";
	char redir[36];

	/* Map the symbol name to a time64 version of itself according to the
	 * pattern used for naming the redirected time64 symbols. */
	size_t l = strnlen(s, sizeof redir);
	if (l<4 || l==sizeof redir) goto no_redir;
	if (s[l-2]=='_' && s[l-1]=='r') {
		l -= 2;
		suffix2 = s+l;
	}
	if (l<4) goto no_redir;
	if (!strcmp(s+l-4, "time")) suffix = "64";
	else suffix = "_time64";

	/* Use the presence of the remapped symbol name in libc to determine
	 * whether it's one that requires time64 redirection; replace if so. */
	snprintf(redir, sizeof redir, "__%.*s%s%s", (int)l, s, suffix, suffix2);
	if (find_sym(&ldso, redir, 1).sym) s = redir;
no_redir:
#endif
	return __dlsym(p, s, ra);
}

int dl_iterate_phdr(int(*callback)(struct dl_phdr_info *info, size_t size, void *data), void *data)
{
	struct dso *current;
	struct dl_phdr_info info;
	int ret = 0;
	for(current = head; current;) {
		info.dlpi_addr      = (uintptr_t)current->base;
		info.dlpi_name      = current->name;
		info.dlpi_phdr      = current->phdr;
		info.dlpi_phnum     = current->phnum;
		info.dlpi_adds      = gencnt;
		info.dlpi_subs      = 0;
		info.dlpi_tls_modid = current->tls_id;
		info.dlpi_tls_data = !current->tls_id ? 0 :
			__tls_get_addr((tls_mod_off_t[]){current->tls_id,0});

		ret = (callback)(&info, sizeof (info), data);

		if (ret != 0) break;

		pthread_rwlock_rdlock(&lock);
		current = current->next;
		pthread_rwlock_unlock(&lock);
	}
	return ret;
}

static void error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	if (!runtime) {
		vdprintf(2, fmt, ap);
		dprintf(2, "\n");
		ldso_fail = 1;
		va_end(ap);
		return;
	}
	__dl_vseterr(fmt, ap);
	va_end(ap);
}

int dlns_set_namespace_lib_path(const char * name, const char * lib_path)
{
	if (!name || !lib_path) {
		LD_LOGE("dlns_set_namespace_lib_path name or lib_path is null.");
		return EINVAL;
	}

	pthread_rwlock_wrlock(&lock);
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, name) == false) {
		pthread_rwlock_unlock(&lock);
		return EPERM;
	}

	ns_t* ns = find_ns_by_name(name);
	if (!ns) {
		pthread_rwlock_unlock(&lock);
		LD_LOGE("dlns_set_namespace_lib_path fail, input ns name : [%{public}s] is not found.", name);
		return ENOKEY;
	}

	ns_set_lib_paths(ns, lib_path);
	pthread_rwlock_unlock(&lock);
	return 0;
}

int dlns_set_namespace_separated(const char * name, const bool separated)
{
	if (!name) {
		LD_LOGE("dlns_set_namespace_separated name  is null.");
		return EINVAL;
	}

	pthread_rwlock_wrlock(&lock);
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, name) == false) {
		pthread_rwlock_unlock(&lock);
		return EPERM;
	}

	ns_t* ns = find_ns_by_name(name);
	if (!ns) {
		pthread_rwlock_unlock(&lock);
		LD_LOGE("dlns_set_namespace_separated fail, input ns name : [%{public}s] is not found.", name);
		return ENOKEY;
	}

	ns_set_separated(ns, separated);
	pthread_rwlock_unlock(&lock);
	return 0;
}

int dlns_set_namespace_permitted_paths(const char * name, const char * permitted_paths)
{
	if (!name || !permitted_paths) {
		LD_LOGE("dlns_set_namespace_permitted_paths name or permitted_paths is null.");
		return EINVAL;
	}

	pthread_rwlock_wrlock(&lock);
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, name) == false) {
		pthread_rwlock_unlock(&lock);
		return EPERM;
	}

	ns_t* ns = find_ns_by_name(name);
	if (!ns) {
		pthread_rwlock_unlock(&lock);
		LD_LOGE("dlns_set_namespace_permitted_paths fail, input ns name : [%{public}s] is not found.", name);
		return ENOKEY;
	}

	ns_set_permitted_paths(ns, permitted_paths);
	pthread_rwlock_unlock(&lock);
	return 0;
}

int dlns_set_namespace_allowed_libs(const char * name, const char * allowed_libs)
{
	if (!name || !allowed_libs) {
		LD_LOGE("dlns_set_namespace_allowed_libs name or allowed_libs is null.");
		return EINVAL;
	}

	pthread_rwlock_wrlock(&lock);
	const void *caller_addr = __builtin_return_address(0);
	if (is_permitted(caller_addr, name) == false) {
		pthread_rwlock_unlock(&lock);
		return EPERM;
	}

	ns_t* ns = find_ns_by_name(name);
	if (!ns) {
		pthread_rwlock_unlock(&lock);
		LD_LOGE("dlns_set_namespace_allowed_libs fail, input ns name : [%{public}s] is not found.", name);
		return ENOKEY;
	}

	ns_set_allowed_libs(ns, allowed_libs);
	pthread_rwlock_unlock(&lock);
	return 0;
}

int handle_asan_path_open(int fd, const char *name, ns_t *namespace, char *buf, size_t buf_size)
{
	LD_LOGD("handle_asan_path_open fd:%{public}d, name:%{public}s , namespace:%{public}s .",
		fd,
		name,
		namespace ? namespace->ns_name : "NULL");
	int fd_tmp = fd;
	if (fd == -1 && (namespace->asan_lib_paths || namespace->lib_paths)) {
		if (namespace->lib_paths && namespace->asan_lib_paths) {
			size_t newlen = strlen(namespace->asan_lib_paths) + strlen(namespace->lib_paths) + 2;
			char *new_lib_paths = malloc(newlen);
			memset(new_lib_paths, 0, newlen);
			strcpy(new_lib_paths, namespace->asan_lib_paths);
			strcat(new_lib_paths, ":");
			strcat(new_lib_paths, namespace->lib_paths);
			fd_tmp = path_open(name, new_lib_paths, buf, buf_size);
			LD_LOGD("handle_asan_path_open path_open new_lib_paths:%{public}s ,fd: %{public}d.", new_lib_paths, fd_tmp);
			free(new_lib_paths);
		} else if (namespace->asan_lib_paths) {
			fd_tmp = path_open(name, namespace->asan_lib_paths, buf, buf_size);
			LD_LOGD("handle_asan_path_open path_open asan_lib_paths:%{public}s ,fd: %{public}d.",
				namespace->asan_lib_paths,
				fd_tmp);
		} else {
			fd_tmp = path_open(name, namespace->lib_paths, buf, buf_size);
			LD_LOGD(
				"handle_asan_path_open path_open lib_paths:%{public}s ,fd: %{public}d.", namespace->lib_paths, fd_tmp);
		}
	}
	return fd_tmp;
}

void* dlopen_ext(const char *file, int mode, const dl_extinfo *extinfo)
{
	const void *caller_addr = __builtin_return_address(0);
	musl_log_reset();
	ld_log_reset();
	if (extinfo != NULL) {
		if ((extinfo->flag & ~(DL_EXT_VALID_FLAG_BITS)) != 0) {
			LD_LOGE("Error dlopen_ext %{public}s: invalid flag %{public}x", file, extinfo->flag);
			return NULL;
		}
	}
	LD_LOGI("dlopen_ext file:%{public}s, mode:%{public}x , caller_addr:%{public}p , extinfo->flag:%{public}x",
		file,
		mode,
		caller_addr,
		extinfo ? extinfo->flag : 0);
	return dlopen_impl(file, mode, NULL, caller_addr, extinfo);
}

#ifdef LOAD_ORDER_RANDOMIZATION
static void open_library_by_path(const char *name, const char *s, struct loadtask *task, struct zip_info *z_info)
{
	char *buf = task->buf;
	size_t buf_size = sizeof task->buf;
	size_t l;
	for (;;) {
		s += strspn(s, ":\n");
		l = strcspn(s, ":\n");
		if (l-1 >= INT_MAX) return;
		if (snprintf(buf, buf_size, "%.*s/%s", (int)l, s, name) < buf_size) {
			char *separator = strstr(buf, ZIP_FILE_PATH_SEPARATOR);
			if (separator != NULL) {
				int res = open_uncompressed_library_in_zipfile(buf, z_info, separator);
				if (res == 0) {
					task->fd = z_info->fd;
					task->file_offset = z_info->file_offset;
					break;
				} else {
					memset(z_info->path_buf, 0, sizeof(z_info->path_buf));
				}
			} else {
				if ((task->fd = open(buf, O_RDONLY|O_CLOEXEC))>=0) break;
			}
		}
		s += l;
	}
	return;
}

static void handle_asan_path_open_by_task(int fd, const char *name, ns_t *namespace, struct loadtask *task,
										  struct zip_info *z_info)
{
	LD_LOGD("handle_asan_path_open_by_task fd:%{public}d, name:%{public}s , namespace:%{public}s .",
			fd,
			name,
			namespace ? namespace->ns_name : "NULL");
	if (fd == -1 && (namespace->asan_lib_paths || namespace->lib_paths)) {
		if (namespace->lib_paths && namespace->asan_lib_paths) {
			size_t newlen = strlen(namespace->asan_lib_paths) + strlen(namespace->lib_paths) + 2;
			char *new_lib_paths = malloc(newlen);
			memset(new_lib_paths, 0, newlen);
			strcpy(new_lib_paths, namespace->asan_lib_paths);
			strcat(new_lib_paths, ":");
			strcat(new_lib_paths, namespace->lib_paths);
			open_library_by_path(name, new_lib_paths, task, z_info);
			LD_LOGD("handle_asan_path_open_by_task open_library_by_path new_lib_paths:%{public}s ,fd: %{public}d.",
					new_lib_paths,
					task->fd);
			free(new_lib_paths);
		} else if (namespace->asan_lib_paths) {
			open_library_by_path(name, namespace->asan_lib_paths, task, z_info);
			LD_LOGD("handle_asan_path_open_by_task open_library_by_path asan_lib_paths:%{public}s ,fd: %{public}d.",
					namespace->asan_lib_paths,
					task->fd);
		} else {
			open_library_by_path(name, namespace->lib_paths, task, z_info);
			LD_LOGD("handle_asan_path_open_by_task open_library_by_path lib_paths:%{public}s ,fd: %{public}d.",
					namespace->lib_paths,
					task->fd);
		}
	}
	return;
}

/* Used to get an uncompress library offset in zip file, then we can use the offset to mmap the library directly. */
int open_uncompressed_library_in_zipfile(const char *path, struct zip_info *z_info, char *separator)
{
	struct local_file_header zip_file_header;
	struct central_dir_entry c_dir_entry;
	struct zip_end_locator end_locator;

	/* Use "'!/' to split the path into zipfile path and library path in zipfile.
	 * For example:
	 * - path: x/xx/xxx.zip!/x/xx/xxx.so
	 * - zipfile path: x/xx/xxx.zip
	 * - library path in zipfile: x/xx/xxx.so  */
	if (strlcpy(z_info->path_buf, path, PATH_BUF_SIZE) >= PATH_BUF_SIZE) {
		LD_LOGE("Open uncompressed library: input path %{public}s is too long.", path);
		return -1;
	}
	z_info->path_buf[separator - path] = '\0';
	z_info->file_path_index = separator - path + 2;
	char *zip_file_path = z_info->path_buf;
	char *lib_path = &z_info->path_buf[z_info->file_path_index];
	if (zip_file_path == NULL || lib_path == NULL) {
		LD_LOGE("Open uncompressed library: get zip and lib path failed.");
		return -1;
	}
	LD_LOGD("Open uncompressed library: input path: %{public}s, zip file path: %{public}s, library path: %{public}s.",
			path, zip_file_path, lib_path);

	// Get zip file length
	FILE *zip_file = fopen(zip_file_path, "re");
	if (zip_file == NULL) {
		LD_LOGE("Open uncompressed library: fopen %{public}s failed.", zip_file_path);
		return -1;
	}
	if (fseek(zip_file, 0, SEEK_END) != 0) {
		LD_LOGE("Open uncompressed library: fseek SEEK_END failed.");
		fclose(zip_file);
		return -1;
	}
	int64_t zip_file_len = ftell(zip_file);
	if (zip_file_len == -1) {
		LD_LOGE("Open uncompressed library: get zip file length failed.");
		fclose(zip_file);
		return -1;
	}

	// Read end of central directory record.
	size_t end_locator_len = sizeof(end_locator);
	size_t end_locator_pos = zip_file_len - end_locator_len;
	if (fseek(zip_file, end_locator_pos, SEEK_SET) != 0) {
		LD_LOGE("Open uncompressed library: fseek end locator position failed.");
		fclose(zip_file);
		return -1;
	}
	if (fread(&end_locator, sizeof(end_locator), 1, zip_file) != 1 || end_locator.signature != EOCD_SIGNATURE) {
		LD_LOGE("Open uncompressed library: fread end locator failed.");
		fclose(zip_file);
		return -1;
	}

	char file_name[PATH_BUF_SIZE];
	uint64_t current_dir_pos = end_locator.offset;
	for (uint16_t i = 0; i < end_locator.total_entries; i++) {
		// Read central dir entry.
		if (fseek(zip_file, current_dir_pos, SEEK_SET) != 0) {
			LD_LOGE("Open uncompressed library: fseek current centra dir entry position failed.");
			fclose(zip_file);
			return -1;
		}
		if (fread(&c_dir_entry, sizeof(c_dir_entry), 1, zip_file) != 1 || c_dir_entry.signature != CENTRAL_SIGNATURE) {
			LD_LOGE("Open uncompressed library: fread centra dir entry failed.");
			fclose(zip_file);
			return -1;
		}

		if (fread(file_name, c_dir_entry.name_size, 1, zip_file) != 1) {
			LD_LOGE("Open uncompressed library: fread file name failed.");
			fclose(zip_file);
			return -1;
		}
		if (strcmp(file_name, lib_path) == 0) {
			// Read local file header.
			if (fseek(zip_file, c_dir_entry.local_header_offset, SEEK_SET) != 0) {
				LD_LOGE("Open uncompressed library: fseek local file header failed.");
				fclose(zip_file);
				return -1;
			}
			if (fread(&zip_file_header, sizeof(zip_file_header), 1, zip_file) != 1) {
				LD_LOGE("Open uncompressed library: fread local file header failed.");
				fclose(zip_file);
				return -1;
			}
			if (zip_file_header.signature != LOCAL_FILE_HEADER_SIGNATURE) {
				LD_LOGE("Open uncompressed library: read local file header signature error.");
				fclose(zip_file);
				return -1;
			}

			z_info->file_offset = c_dir_entry.local_header_offset + sizeof(zip_file_header) +
									zip_file_header.name_size + zip_file_header.extra_size;
			if (zip_file_header.compression_method != COMPRESS_STORED || z_info->file_offset % PAGE_SIZE != 0) {
				LD_LOGE("Open uncompressed library: open %{public}s in %{public}s failed because of misalignment or saved with compression."
						"compress method %{public}d, file offset %{public}lu",
						lib_path, zip_file_path, zip_file_header.compression_method, z_info->file_offset);
				fclose(zip_file);
				return -2;
			}
			z_info->found = true;
			break;
		}

		memset(file_name, 0, sizeof(file_name));
		current_dir_pos += sizeof(c_dir_entry);
		current_dir_pos += c_dir_entry.name_size + c_dir_entry.extra_size + c_dir_entry.comment_size;
	}
	if(!z_info->found) {
		LD_LOGE("Open uncompressed library: %{public}s was not found in %{public}s.", lib_path, zip_file_path);
		fclose(zip_file);
		return -3;
	}
	z_info->fd = fileno(zip_file);

	return 0;
}

static bool map_library_header(struct loadtask *task)
{
	off_t off_start;
	Phdr *ph;
	size_t i;
	size_t str_size;
	off_t str_table;

	ssize_t l = pread(task->fd, task->ehdr_buf, sizeof task->ehdr_buf, task->file_offset);
	task->eh = task->ehdr_buf;
	if (l < 0) {
		LD_LOGE("Error mapping header %{public}s: failed to read fd errno: %{public}d", task->name, errno);
		return false;
	}
	if (l < sizeof(Ehdr) || (task->eh->e_type != ET_DYN && task->eh->e_type != ET_EXEC)) {
		LD_LOGE("Error mapping header %{public}s: invaliled Ehdr l=%{public}d", task->name, l);
		goto noexec;
	}
	task->phsize = task->eh->e_phentsize * task->eh->e_phnum;
	if (task->phsize > sizeof task->ehdr_buf - sizeof(Ehdr)) {
		task->allocated_buf = malloc(task->phsize);
		if (!task->allocated_buf) {
			LD_LOGE("Error mapping header %{public}s: failed to alloc memory errno: %{public}d", task->name, errno);
			return false;
		}
		l = pread(task->fd, task->allocated_buf, task->phsize, task->eh->e_phoff + task->file_offset);
		if (l < 0) {
			LD_LOGE("Error mapping header %{public}s: failed to pread errno: %{public}d", task->name, errno);
			goto error;
		}
		if (l != task->phsize) {
			LD_LOGE("Error mapping header %{public}s: unmatched phsize errno: %{public}d", task->name, errno);
			goto noexec;
		}
		ph = task->ph0 = task->allocated_buf;
	} else if (task->eh->e_phoff + task->phsize > l) {
		l = pread(task->fd, task->ehdr_buf + 1, task->phsize, task->eh->e_phoff + task->file_offset);
		if (l < 0) {
			LD_LOGE("Error mapping header %{public}s: failed to pread errno: %{public}d", task->name, errno);
			goto error;
		}
		if (l != task->phsize) {
			LD_LOGE("Error mapping header %{public}s: unmatched phsize", task->name);
			goto noexec;
		}
		ph = task->ph0 = (void *)(task->ehdr_buf + 1);
	} else {
		ph = task->ph0 = (void *)((char *)task->ehdr_buf + task->eh->e_phoff);
	}

	for (i = task->eh->e_phnum; i; i--, ph = (void *)((char *)ph + task->eh->e_phentsize)) {
		if (ph->p_type == PT_DYNAMIC) {
			task->dyn = ph->p_vaddr;
		} else if (ph->p_type == PT_TLS) {
			task->tls_image = ph->p_vaddr;
			task->tls.align = ph->p_align;
			task->tls.len = ph->p_filesz;
			task->tls.size = ph->p_memsz;
		}

		if (ph->p_type != PT_DYNAMIC) {
			continue;
		}
		// map the dynamic segment and the string table of the library
		off_start = ph->p_offset;
		off_start &= -PAGE_SIZE;
		task->dyn_map_len = ph->p_memsz + (ph->p_offset - off_start);
		/* The default value of file_offset is 0.
		 * The value of file_offset may be greater than 0 when opening library from zip file.
		 * The value of file_offset ensures PAGE_SIZE aligned. */
		task->dyn_map = mmap(0, task->dyn_map_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
		if (task->dyn_map == MAP_FAILED) {
			LD_LOGE("Error mapping header %{public}s: failed to map dynamic section errno: %{public}d", task->name, errno);
			goto error;
		}
		task->dyn_addr = (size_t *)((unsigned char *)task->dyn_map + (ph->p_offset - off_start));
		size_t dyn_tmp;
		if (search_vec(task->dyn_addr, &dyn_tmp, DT_STRTAB)) {
			str_table = dyn_tmp;
		} else {
			LD_LOGE("Error mapping header %{public}s: DT_STRTAB not found", task->name);
			goto error;
		}
		if (search_vec(task->dyn_addr, &dyn_tmp, DT_STRSZ)) {
			str_size = dyn_tmp;
		} else {
			LD_LOGE("Error mapping header %{public}s: DT_STRSZ not found", task->name);
			goto error;
		}
	}

	task->shsize = task->eh->e_shentsize * task->eh->e_shnum;
	off_start = task->eh->e_shoff;
	off_start &= -PAGE_SIZE;
	task->shsize += task->eh->e_shoff - off_start;
	task->shdr_allocated_buf = mmap(0, task->shsize, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
	if (task->shdr_allocated_buf == MAP_FAILED) {
		LD_LOGE("Error mapping section header %{public}s: failed to map shdr_allocated_buf errno: %{public}d",
			task->name, errno);
		goto error;
	}
	Shdr *sh = (char *)task->shdr_allocated_buf + task->eh->e_shoff - off_start;
	for (i = task->eh->e_shnum; i; i--, sh = (void *)((char *)sh + task->eh->e_shentsize)) {
		if (sh->sh_type != SHT_STRTAB || sh->sh_addr != str_table || sh->sh_size != str_size) {
			continue;
		}
		off_start = sh->sh_offset;
		off_start &= -PAGE_SIZE;
		task->str_map_len = sh->sh_size + (sh->sh_offset - off_start);
		task->str_map = mmap(0, task->str_map_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
		if (task->str_map == MAP_FAILED) {
			LD_LOGE("Error mapping section header %{public}s: failed to map string section errno: %{public}d",
				task->name, errno);
			goto error;
		}
		task->str_addr = (char *)task->str_map + sh->sh_offset - off_start;
		break;
	}
	if (!task->dyn) {
		LD_LOGE("Error mapping header %{public}s: dynamic section not found", task->name);
		goto noexec;
	}
	return true;
noexec:
	errno = ENOEXEC;
error:
	free(task->allocated_buf);
	task->allocated_buf = NULL;
	if (task->shdr_allocated_buf != MAP_FAILED) {
		munmap(task->shdr_allocated_buf, task->shsize);
		task->shdr_allocated_buf = MAP_FAILED;
	}
	return false;
}

static bool task_map_library(struct loadtask *task, struct reserved_address_params *reserved_params)
{
	size_t addr_min = SIZE_MAX, addr_max = 0, map_len;
	size_t this_min, this_max;
	size_t nsegs = 0;
	off_t off_start;
	Phdr *ph = task->ph0;
	unsigned prot;
	unsigned char *map = MAP_FAILED, *base;
	size_t i;
	int map_flags = MAP_PRIVATE;
	size_t start_addr;
	size_t start_alignment = PAGE_SIZE;
	bool hugepage_enabled = false;

	for (i = task->eh->e_phnum; i; i--, ph = (void *)((char *)ph + task->eh->e_phentsize)) {
		if (ph->p_type == PT_GNU_RELRO) {
			task->p->relro_start = ph->p_vaddr & -PAGE_SIZE;
			task->p->relro_end = (ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE;
		} else if (ph->p_type == PT_GNU_STACK) {
			if (!runtime && ph->p_memsz > __default_stacksize) {
				__default_stacksize =
					ph->p_memsz < DEFAULT_STACK_MAX ?
					ph->p_memsz : DEFAULT_STACK_MAX;
			}
		}
		if (ph->p_type != PT_LOAD) {
			continue;
		}
		nsegs++;
		if (ph->p_vaddr < addr_min) {
			addr_min = ph->p_vaddr;
			off_start = ph->p_offset;
			prot = (((ph->p_flags & PF_R) ? PROT_READ : 0) |
				((ph->p_flags & PF_W) ? PROT_WRITE : 0) |
				((ph->p_flags & PF_X) ? PROT_EXEC : 0));
		}
		if (ph->p_vaddr + ph->p_memsz > addr_max) {
			addr_max = ph->p_vaddr + ph->p_memsz;
		}
	}
	if (!task->dyn) {
		LD_LOGE("Error mapping library: !task->dyn dynamic section not found task->name=%{public}s", task->name);
		goto noexec;
	}
	if (DL_FDPIC && !(task->eh->e_flags & FDPIC_CONSTDISP_FLAG)) {
		task->p->loadmap = calloc(1, sizeof(struct fdpic_loadmap) + nsegs * sizeof(struct fdpic_loadseg));
		if (!task->p->loadmap) {
			LD_LOGE("Error mapping library: calloc failed errno=%{public}d nsegs=%{public}d", errno, nsegs);
			goto error;
		}
		task->p->loadmap->nsegs = nsegs;
		for (ph = task->ph0, i = 0; i < nsegs; ph = (void *)((char *)ph + task->eh->e_phentsize)) {
			if (ph->p_type != PT_LOAD) {
				continue;
			}
			prot = (((ph->p_flags & PF_R) ? PROT_READ : 0) |
				((ph->p_flags & PF_W) ? PROT_WRITE : 0) |
				((ph->p_flags & PF_X) ? PROT_EXEC : 0));
			map = mmap(0, ph->p_memsz + (ph->p_vaddr & PAGE_SIZE - 1),
				prot, MAP_PRIVATE,
				task->fd, ph->p_offset & -PAGE_SIZE + task->file_offset);
			if (map == MAP_FAILED) {
				unmap_library(task->p);
				LD_LOGE("Error mapping library: PT_LOAD mmap failed task->name=%{public}s errno=%{public}d map_len=%{public}d",
					task->name, errno, ph->p_memsz + (ph->p_vaddr & PAGE_SIZE - 1));
				goto error;
			}
			task->p->loadmap->segs[i].addr = (size_t)map +
				(ph->p_vaddr & PAGE_SIZE - 1);
			task->p->loadmap->segs[i].p_vaddr = ph->p_vaddr;
			task->p->loadmap->segs[i].p_memsz = ph->p_memsz;
			i++;
			if (prot & PROT_WRITE) {
				size_t brk = (ph->p_vaddr & PAGE_SIZE - 1) + ph->p_filesz;
				size_t pgbrk = (brk + PAGE_SIZE - 1) & -PAGE_SIZE;
				size_t pgend = (brk + ph->p_memsz - ph->p_filesz + PAGE_SIZE - 1) & -PAGE_SIZE;
				if (pgend > pgbrk && mmap_fixed(map + pgbrk,
					pgend - pgbrk, prot,
					MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS,
					-1, off_start) == MAP_FAILED)
					LD_LOGE("Error mapping library: PROT_WRITE mmap_fixed failed errno=%{public}d", errno);
					goto error;
				memset(map + brk, 0, pgbrk - brk);
			}
		}
		map = (void *)task->p->loadmap->segs[0].addr;
		map_len = 0;
		goto done_mapping;
	}
	addr_max += PAGE_SIZE - 1;
	addr_max &= -PAGE_SIZE;
	addr_min &= -PAGE_SIZE;
	off_start &= -PAGE_SIZE;
	map_len = addr_max - addr_min + off_start;
	start_addr = addr_min;

	hugepage_enabled = get_transparent_hugepages_supported();
	if (hugepage_enabled) {
		size_t maxinum_alignment = phdr_table_get_maxinum_alignment(task->ph0, task->eh->e_phnum);

		start_alignment = maxinum_alignment == KPMD_SIZE ? KPMD_SIZE : PAGE_SIZE;
	}

	if (reserved_params) {
		if (map_len > reserved_params->reserved_size) {
			if (reserved_params->must_use_reserved) {
				LD_LOGE("Error mapping library: map len is larger than reserved address task->name=%{public}s", task->name);
				goto error;
			}
		} else {
			start_addr = ((size_t)reserved_params->start_addr - 1 + PAGE_SIZE) & -PAGE_SIZE;
			map_flags |= MAP_FIXED;
		}
	}

	/* we will find a mapping_align aligned address as the start of dso
	 * so we need a tmp_map_len as map_len + mapping_align to make sure
	 * we have enough space to shift the dso to the correct location. */
	size_t mapping_align = start_alignment > LIBRARY_ALIGNMENT ? start_alignment : LIBRARY_ALIGNMENT;
	size_t tmp_map_len = ALIGN(map_len, mapping_align) + mapping_align - PAGE_SIZE;

	/* map the whole load segments with PROT_READ first for security consideration. */
	prot = PROT_READ;

	/* if reserved_params exists, we should use start_addr as prefered result to do the mmap operation */
	if (reserved_params) {
		map = DL_NOMMU_SUPPORT
			? mmap((void *)start_addr, map_len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
			: mmap((void *)start_addr, map_len, prot, map_flags, task->fd, off_start + task->file_offset);
		if (map == MAP_FAILED) {
			LD_LOGE("Error mapping library: reserved_params mmap failed errno=%{public}d DL_NOMMU_SUPPORT=%{public}d"
				" task->fd=%{public}d task->name=%{public}s map_len=%{public}d",
				errno, DL_NOMMU_SUPPORT, task->fd, task->name, map_len);
			goto error;
		}
		if (reserved_params && map_len < reserved_params->reserved_size) {
			reserved_params->reserved_size -= (map_len + (start_addr - (size_t)reserved_params->start_addr));
			reserved_params->start_addr = (void *)((uint8_t *)map + map_len);
		}
	/* if reserved_params does not exist, we should use real_map as prefered result to do the mmap operation */
	} else {
		/* use tmp_map_len to mmap enough space for the dso with anonymous mapping */
		unsigned char *temp_map = mmap((void *)NULL, tmp_map_len, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (temp_map == MAP_FAILED) {
			LD_LOGE("Error mapping library: !reserved_params mmap failed errno=%{public}d tmp_map_len=%{public}d",
				errno, tmp_map_len);
			goto error;
		}

		/* find the mapping_align aligned address */
		unsigned char *real_map = (unsigned char*)ALIGN((uintptr_t)temp_map, mapping_align);

		/* mummap the space we mmap before so that we can mmap correct space again */
		munmap(temp_map, tmp_map_len);

		map = DL_NOMMU_SUPPORT
			? mmap(real_map, map_len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
			/* use map_len to mmap correct space for the dso with file mapping */
			: mmap(real_map, map_len, prot, map_flags, task->fd, off_start + task->file_offset);
		if (map == MAP_FAILED) {
			LD_LOGE("Error mapping library: !reserved_params mmap failed errno=%{public}d DL_NOMMU_SUPPORT=%{public}d"
				"task->fd=%{public}d task->name=%{public}s map_len=%{public}d",
				errno, DL_NOMMU_SUPPORT, task->fd, task->name, map_len);
			goto error;
		}
	}
	task->p->map = map;
	task->p->map_len = map_len;
	/* If the loaded file is not relocatable and the requested address is
	 * not available, then the load operation must fail. */
	if (task->eh->e_type != ET_DYN && addr_min && map != (void *)addr_min) {
		LD_LOGE("Error mapping library: ET_DYN task->name=%{public}s", task->name);
		errno = EBUSY;
		goto error;
	}
	base = map - addr_min;
	task->p->phdr = 0;
	task->p->phnum = 0;
	for (ph = task->ph0, i = task->eh->e_phnum; i; i--, ph = (void *)((char *)ph + task->eh->e_phentsize)) {
		if (ph->p_type == PT_OHOS_RANDOMDATA) {
			fill_random_data((void *)(ph->p_vaddr + base), ph->p_memsz);
			continue;
		}
		if (ph->p_type != PT_LOAD) {
			continue;
		}
		/* Check if the programs headers are in this load segment, and
		 * if so, record the address for use by dl_iterate_phdr. */
		if (!task->p->phdr && task->eh->e_phoff >= ph->p_offset
			&& task->eh->e_phoff + task->phsize <= ph->p_offset + ph->p_filesz) {
			task->p->phdr = (void *)(base + ph->p_vaddr + (task->eh->e_phoff - ph->p_offset));
			task->p->phnum = task->eh->e_phnum;
			task->p->phentsize = task->eh->e_phentsize;
		}
		this_min = ph->p_vaddr & -PAGE_SIZE;
		this_max = ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1 & -PAGE_SIZE;
		off_start = ph->p_offset & -PAGE_SIZE;
		prot = (((ph->p_flags & PF_R) ? PROT_READ : 0) |
			((ph->p_flags & PF_W) ? PROT_WRITE : 0) |
			((ph->p_flags & PF_X) ? PROT_EXEC : 0));
		/* Reuse the existing mapping for the lowest-address LOAD */
		if (mmap_fixed(
				base + this_min,
				this_max - this_min,
				prot, MAP_PRIVATE | MAP_FIXED,
				task->fd,
				off_start + task->file_offset) == MAP_FAILED) {
			LD_LOGE("Error mapping library: mmap fix failed task->name=%{public}s errno=%{public}d", task->name, errno);
			goto error;
		}
		if ((ph->p_flags & PF_X) && (ph->p_align == KPMD_SIZE) && hugepage_enabled)
			madvise(base + this_min, this_max - this_min, MADV_HUGEPAGE);
		if (ph->p_memsz > ph->p_filesz && (ph->p_flags & PF_W)) {
			size_t brk = (size_t)base + ph->p_vaddr + ph->p_filesz;
			size_t pgbrk = brk + PAGE_SIZE - 1 & -PAGE_SIZE;
			size_t zeromap_size = (size_t)base + this_max - pgbrk;
			memset((void *)brk, 0, pgbrk - brk & PAGE_SIZE - 1);
			if (pgbrk - (size_t)base < this_max && mmap_fixed(
				(void *)pgbrk,
				zeromap_size,
				prot,
				MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS,
				-1,
				0) == MAP_FAILED) {
				LD_LOGE("Error mapping library: PF_W mmap fix failed errno=%{public}d task->name=%{public}s zeromap_size=%{public}d",
					errno, task->name, zeromap_size);
				goto error;
			}
			set_bss_vma_name(task->p->name, (void *)pgbrk, zeromap_size);
		}
	}
	for (i = 0; ((size_t *)(base + task->dyn))[i]; i += NEXT_DYNAMIC_INDEX) {
		if (((size_t *)(base + task->dyn))[i] == DT_TEXTREL) {
			if (mprotect(map, map_len, PROT_READ | PROT_WRITE | PROT_EXEC) && errno != ENOSYS) {
				LD_LOGE("Error mapping library: mprotect failed task->name=%{public}s errno=%{public}d", task->name, errno);
				goto error;
			}
			break;
		}
	}
done_mapping:
	task->p->base = base;
	task->p->dynv = laddr(task->p, task->dyn);
	if (task->p->tls.size) {
		task->p->tls.image = laddr(task->p, task->tls_image);
	}
	free(task->allocated_buf);
	task->allocated_buf = NULL;
	if (task->shdr_allocated_buf != MAP_FAILED) {
		munmap(task->shdr_allocated_buf, task->shsize);
		task->shdr_allocated_buf = MAP_FAILED;
	}
	return true;
noexec:
	errno = ENOEXEC;
error:
	if (map != MAP_FAILED) {
		unmap_library(task->p);
	}
	free(task->allocated_buf);
	task->allocated_buf = NULL;
	if (task->shdr_allocated_buf != MAP_FAILED) {
		munmap(task->shdr_allocated_buf, task->shsize);
		task->shdr_allocated_buf = MAP_FAILED;
	}
	return false;
}

static bool resolve_fd_to_realpath(struct loadtask *task)
{
	char proc_self_fd[32];
	static char resolved_path[PATH_MAX];

	int ret = snprintf(proc_self_fd, sizeof(proc_self_fd), "/proc/self/fd/%d", task->fd);
	if (ret < 0 || ret >= sizeof(proc_self_fd)) {
		return false;
	}
	ssize_t len = readlink(proc_self_fd, resolved_path, sizeof(resolved_path) - 1);
	if (len < 0) {
		return false;
	}
	resolved_path[len] = '\0';
	strncpy(task->buf, resolved_path, PATH_MAX);

	return true;
}

static bool load_library_header(struct loadtask *task)
{
	const char *name = task->name;
	struct dso *needed_by = task->needed_by;
	ns_t *namespace = task->namespace;
	bool check_inherited = task->check_inherited;
	struct zip_info z_info;

	bool map = false;
	struct stat st;
	size_t alloc_size;
	int n_th = 0;
	int is_self = 0;

	if (!*name) {
		errno = EINVAL;
		return false;
	}

	/* Catch and block attempts to reload the implementation itself */
	if (name[NAME_INDEX_ZERO] == 'l' && name[NAME_INDEX_ONE] == 'i' && name[NAME_INDEX_TWO] == 'b') {
		static const char reserved[] =
			"c.pthread.rt.m.dl.util.xnet.";
		const char *rp, *next;
		for (rp = reserved; *rp; rp = next) {
			next = strchr(rp, '.') + 1;
			if (strncmp(name + NAME_INDEX_THREE, rp, next - rp) == 0) {
				break;
			}
		}
		if (*rp) {
			if (ldd_mode) {
				/* Track which names have been resolved
				 * and only report each one once. */
				static unsigned reported;
				unsigned mask = 1U << (rp - reserved);
				if (!(reported & mask)) {
					reported |= mask;
					dprintf(1, "\t%s => %s (%p)\n",
						name, ldso.name,
						ldso.base);
				}
			}
			is_self = 1;
		}
	}
	if (!strcmp(name, ldso.name)) {
		is_self = 1;
	}
	if (is_self) {
		if (!ldso.prev) {
			tail->next = &ldso;
			ldso.prev = tail;
			tail = &ldso;
			ldso.namespace = namespace;
			ns_add_dso(namespace, &ldso);
		}
		task->isloaded = true;
		task->p = &ldso;
		return true;
	}
	if (strchr(name, '/')) {
		char *separator = strstr(name, ZIP_FILE_PATH_SEPARATOR);
		if (separator != NULL) {
			int res = open_uncompressed_library_in_zipfile(name, &z_info, separator);
			if (!res) {
				task->pathname = name;
				if (!is_accessible(namespace, task->pathname, g_is_asan, check_inherited)) {
					LD_LOGE("Open uncompressed library: check ns accessible failed, pathname %{public}s namespace %{public}s.",
							task->pathname, namespace ? namespace->ns_name : "NULL");
					task->fd = -1;
				} else {
					task->fd = z_info.fd;
					task->file_offset = z_info.file_offset;
				}
			} else {
				LD_LOGE("Open uncompressed library in zip file failed, name:%{public}s res:%{public}d", name, res);
				return false;
			}
		} else {
			task->pathname = name;
			if (!is_accessible(namespace, task->pathname, g_is_asan, check_inherited)) {
				LD_LOGE("Open absolute_path library: check ns accessible failed, pathname %{public}s namespace %{public}s.",
						task->pathname, namespace ? namespace->ns_name : "NULL");
				task->fd = -1;
			} else {
				task->fd = open(name, O_RDONLY | O_CLOEXEC);
			}
		}
	} else {
		/* Search for the name to see if it's already loaded */
		/* Search in namespace */
		task->p = find_library_by_name(name, namespace, check_inherited);
		if (task->p) {
			task->isloaded = true;
			LD_LOGD("find_library_by_name(name=%{public}s ns=%{public}s) already loaded by %{public}s in %{public}s namespace  ",
					name, namespace->ns_name, task->p->name, task->p->namespace->ns_name);
			return true;
		}
		if (strlen(name) > NAME_MAX) {
			LD_LOGE("load_library name length is larger than NAME_MAX:%{public}s.", name);
			return false;
		}
		task->fd = -1;
		if (namespace->env_paths) {
			open_library_by_path(name, namespace->env_paths, task, &z_info);
		}
		for (task->p = needed_by; task->fd == -1 && task->p; task->p = task->p->needed_by) {
			if (fixup_rpath(task->p, task->buf, sizeof task->buf) < 0) {
				task->fd = INVALID_FD_INHIBIT_FURTHER_SEARCH; /* Inhibit further search. */
			}
			if (task->p->rpath) {
				open_library_by_path(name, task->p->rpath, task, &z_info);
				if (task->fd != -1 && resolve_fd_to_realpath(task)) {
					if (!is_accessible(namespace, task->buf, g_is_asan, check_inherited)) {
						LD_LOGE("Open library: check ns accessible failed, name %{public}s namespace %{public}s.",
								name, namespace ? namespace->ns_name : "NULL");
						close(task->fd);
						task->fd = -1;
					}
				}
			}
		}
		if (g_is_asan) {
			handle_asan_path_open_by_task(task->fd, name, namespace, task, &z_info);
			LD_LOGD("load_library handle_asan_path_open_by_task fd:%{public}d.", task->fd);
		} else {
			if (task->fd == -1 && namespace->lib_paths) {
				open_library_by_path(name, namespace->lib_paths, task, &z_info);
				LD_LOGD("load_library no asan lib_paths path_open fd:%{public}d.", task->fd);
			}
		}
		task->pathname = task->buf;
	}
	if (task->fd < 0) {
		if (!check_inherited || !namespace->ns_inherits) {
			LD_LOGE("Error loading header %{public}s, namespace %{public}s has no inherits, errno=%{public}d",
				task->name, namespace->ns_name, errno);
			return false;
		}
		/* Load lib in inherited namespace. Do not check inherited again.*/
		for (size_t i = 0; i < namespace->ns_inherits->num; i++) {
			ns_inherit *inherit = namespace->ns_inherits->inherits[i];
			if (strchr(name, '/') == 0 && !is_sharable(inherit, name)) {
				continue;
			}
			task->namespace = inherit->inherited_ns;
			task->check_inherited = false;
			if (load_library_header(task)) {
				return true;
			}
		}
		LD_LOGE("Error loading header: can't find library %{public}s in namespace: %{public}s",
			task->name, namespace->ns_name);
		return false;
	}

	if (fstat(task->fd, &st) < 0) {
		LD_LOGE("Error loading header %{public}s: failed to get file state errno=%{public}d", task->name, errno);
		close(task->fd);
		task->fd = -1;
		return false;
	}
	/* Search in namespace */
	task->p = find_library_by_fstat(&st, namespace, check_inherited, task->file_offset);
	if (task->p) {
		/* If this library was previously loaded with a
		* pathname but a search found the same inode,
		* setup its shortname so it can be found by name. */
		if (!task->p->shortname && task->pathname != name) {
			task->p->shortname = strrchr(task->p->name, '/') + 1;
		}
		close(task->fd);
		task->fd = -1;
		task->isloaded = true;
		LD_LOGD("find_library_by_fstat(name=%{public}s ns=%{public}s) already loaded by %{public}s in %{public}s namespace  ",
				name, namespace->ns_name, task->p->name, task->p->namespace->ns_name);
		return true;
	}

	map = noload ? 0 : map_library_header(task);
	if (!map) {
		LD_LOGE("Error loading header %{public}s: failed to map header", task->name);
		close(task->fd);
		task->fd = -1;
		return false;
	}

	/* Allocate storage for the new DSO. When there is TLS, this
	 * storage must include a reservation for all pre-existing
	 * threads to obtain copies of both the new TLS, and an
	 * extended DTV capable of storing an additional slot for
	 * the newly-loaded DSO. */
	alloc_size = sizeof(struct dso) + strlen(task->pathname) + 1;
	if (runtime && task->tls.size) {
		size_t per_th = task->tls.size + task->tls.align + sizeof(void *) * (tls_cnt + TLS_CNT_INCREASE);
		n_th = libc.threads_minus_1 + 1;
		if (n_th > SSIZE_MAX / per_th) {
			alloc_size = SIZE_MAX;
		} else {
			alloc_size += n_th * per_th;
		}
	}
	task->p = calloc(1, alloc_size);
	if (!task->p) {
		LD_LOGE("Error loading header %{public}s: failed to allocate dso", task->name);
		close(task->fd);
		task->fd = -1;
		return false;
	}
	task->p->dev = st.st_dev;
	task->p->ino = st.st_ino;
	task->p->file_offset = task->file_offset;
	task->p->needed_by = needed_by;
	task->p->name = task->p->buf;
	strcpy(task->p->name, task->pathname);
	task->p->tls = task->tls;
	task->p->dynv = task->dyn_addr;
	task->p->strings = task->str_addr;
	size_t rpath_offset;
	size_t runpath_offset;
	if (search_vec(task->p->dynv, &rpath_offset, DT_RPATH))
		task->p->rpath_orig = task->p->strings + rpath_offset;
	if (search_vec(task->p->dynv, &runpath_offset, DT_RUNPATH))
		task->p->rpath_orig = task->p->strings + runpath_offset;

	/* Add a shortname only if name arg was not an explicit pathname. */
	if (task->pathname != name) {
		task->p->shortname = strrchr(task->p->name, '/') + 1;
	}

	if (task->p->tls.size) {
		task->p->tls_id = ++tls_cnt;
		task->p->new_dtv = (void *)(-sizeof(size_t) &
			(uintptr_t)(task->p->name + strlen(task->p->name) + sizeof(size_t)));
		task->p->new_tls = (void *)(task->p->new_dtv + n_th * (tls_cnt + 1));
	}

	tail->next = task->p;
	task->p->prev = tail;
	tail = task->p;

	/* Add dso to namespace */
	task->p->namespace = namespace;
	ns_add_dso(namespace, task->p);
	return true;
}

static void task_load_library(struct loadtask *task, struct reserved_address_params *reserved_params)
{
	LD_LOGD("load_library loading ns=%{public}s name=%{public}s by_dlopen=%{public}d", task->namespace->ns_name, task->p->name, runtime);
	bool map = noload ? 0 : task_map_library(task, reserved_params);
	__close(task->fd);
	task->fd = -1;
	if (!map) {
		LD_LOGE("Error loading library %{public}s: failed to map library noload=%{public}d errno=%{public}d",
			task->name, noload, errno);
		error("Error loading library %s: failed to map library noload=%d errno=%d", task->name, noload, errno);
		if (runtime) {
			longjmp(*rtld_fail, 1);
		}
		return;
	};

	/* Avoid the danger of getting two versions of libc mapped into the
	 * same process when an absolute pathname was used. The symbols
	 * checked are chosen to catch both musl and glibc, and to avoid
	 * false positives from interposition-hack libraries. */
	decode_dyn(task->p);
	if (find_sym(task->p, "__libc_start_main", 1).sym &&
		find_sym(task->p, "stdin", 1).sym) {
		do_dlclose(task->p);
		task->p = NULL;
		free((void*)task->name);
		task->name = ld_strdup("libc.so");
		task->check_inherited = true;
		if (!load_library_header(task)) {
			LD_LOGE("Error loading library %{public}s: failed to load libc.so", task->name);
			error("Error loading library %s: failed to load libc.so", task->name);
			if (runtime) {
				longjmp(*rtld_fail, 1);
			}
		}
		return;
	}
	/* Past this point, if we haven't reached runtime yet, ldso has
	 * committed either to use the mapped library or to abort execution.
	 * Unmapping is not possible, so we can safely reclaim gaps. */
	if (!runtime) {
		reclaim_gaps(task->p);
	}
	task->p->runtime_loaded = runtime;
	if (runtime)
		task->p->by_dlopen = 1;

	if (DL_FDPIC) {
		makefuncdescs(task->p);
	}

	if (ldd_mode) {
		dprintf(1, "\t%s => %s (%p)\n", task->name, task->pathname, task->p->base);
	}

#ifdef ENABLE_HWASAN
	if (libc.load_hook) {
		libc.load_hook((long unsigned int)task->p->base, task->p->phdr, task->p->phnum);
	}
#endif
}

static void preload_direct_deps(struct dso *p, ns_t *namespace, struct loadtasks *tasks)
{
	size_t i, cnt = 0;
	if (p->deps) {
		return;
	}
	/* For head, all preloads are direct pseudo-dependencies.
	 * Count and include them now to avoid realloc later. */
	if (p == head) {
		for (struct dso *q = p->next; q; q = q->next) {
			cnt++;
		}
	}
	for (i = 0; p->dynv[i]; i += NEXT_DYNAMIC_INDEX) {
		if (p->dynv[i] == DT_NEEDED) {
			cnt++;
		}
	}
	/* Use builtin buffer for apps with no external deps, to
	 * preserve property of no runtime failure paths. */
	p->deps = (p == head && cnt < MIN_DEPS_COUNT) ? builtin_deps :
		calloc(cnt + 1, sizeof *p->deps);
	if (!p->deps) {
		LD_LOGE("Error loading dependencies for %{public}s", p->name);
		error("Error loading dependencies for %s", p->name);
		if (runtime) {
			longjmp(*rtld_fail, 1);
		}
	}
	cnt = 0;
	if (p == head) {
		for (struct dso *q = p->next; q; q = q->next) {
			p->deps[cnt++] = q;
		}
	}
	for (i = 0; p->dynv[i]; i += NEXT_DYNAMIC_INDEX) {
		if (p->dynv[i] != DT_NEEDED) {
			continue;
		}
		const char* dtneed_name = p->strings + p->dynv[i + 1];
		LD_LOGD("load_library %{public}s adding DT_NEEDED task %{public}s namespace(%{public}s)", p->name, dtneed_name, namespace->ns_name);
		struct loadtask *task = create_loadtask(dtneed_name, p, namespace, true);
		if (!task) {
			LD_LOGE("Error loading dependencies %{public}s : create load task failed", p->name);
			error("Error loading dependencies for %s : create load task failed", p->name);
			if (runtime) {
				longjmp(*rtld_fail, 1);
			}
			continue;
		}
		LD_LOGD("loading shared library %{public}s: (needed by %{public}s)", p->strings + p->dynv[i+1], p->name);
		if (!load_library_header(task)) {
			free_task(task);
			task = NULL;
			LD_LOGE("Error loading shared library %{public}s: (needed by %{public}s)",
				p->strings + p->dynv[i + 1],
				p->name);
			error("Error loading shared library %s: %m (needed by %s)",
				p->strings + p->dynv[i + 1], p->name);
			if (runtime) {
				longjmp(*rtld_fail, 1);
			}
			continue;
		}
		p->deps[cnt++] = task->p;
		if (task->isloaded) {
			free_task(task);
			task = NULL;
		} else {
			append_loadtasks(tasks, task);
		}
	}
	p->deps[cnt] = 0;
	p->ndeps_direct = cnt;
	for (i = 0; i < p->ndeps_direct; i++) {
		add_dso_parent(p->deps[i], p);
	}
}

static void unmap_preloaded_sections(struct loadtasks *tasks)
{
	struct loadtask *task = NULL;
	for (size_t i = 0; i < tasks->length; i++) {
		task = get_loadtask(tasks, i);
		if (!task) {
			continue;
		}
		if (task->dyn_map_len) {
			munmap(task->dyn_map, task->dyn_map_len);
			task->dyn_map = NULL;
			task->dyn_map_len = 0;
			if (task->p) {
				task->p->dynv = NULL;
			}
		}
		if (task->str_map_len) {
			munmap(task->str_map, task->str_map_len);
			task->str_map = NULL;
			task->str_map_len = 0;
			if (task->p) {
				task->p->strings = NULL;
			}
		}
	}
}

static void preload_deps(struct dso *p, struct loadtasks *tasks)
{
	if (p->deps) {
		return;
	}
	for (; p; p = p->next) {
		preload_direct_deps(p, p->namespace, tasks);
	}
}

static void run_loadtasks(struct loadtasks *tasks, struct reserved_address_params *reserved_params)
{
	struct loadtask *task = NULL;
	bool reserved_address = false;
	for (size_t i = 0; i < tasks->length; i++) {
		task = get_loadtask(tasks, i);
		if (task) {
			if (reserved_params) {
				reserved_address = reserved_params->reserved_address_recursive || (reserved_params->target == task->p);
			}
			task_load_library(task, reserved_address ? reserved_params : NULL);
		}
	}
}

UT_STATIC void assign_tls(struct dso *p)
{
	while (p) {
		if (p->tls.image) {
			tls_align = MAXP2(tls_align, p->tls.align);
#ifdef TLS_ABOVE_TP
			p->tls.offset = tls_offset + ((p->tls.align - 1) &
				(-tls_offset + (uintptr_t)p->tls.image));
			tls_offset = p->tls.offset + p->tls.size;
#else
			tls_offset += p->tls.size + p->tls.align - 1;
			tls_offset -= (tls_offset + (uintptr_t)p->tls.image)
				& (p->tls.align - 1);
			p->tls.offset = tls_offset;
#endif
			if (tls_tail) {
				tls_tail->next = &p->tls;
			} else {
				libc.tls_head = &p->tls;
			}
			tls_tail = &p->tls;
		}

		p = p->next;
	}
}

UT_STATIC void load_preload(char *s, ns_t *ns, struct loadtasks *tasks)
{
	int tmp;
	char *z;

	struct loadtask *task = NULL;
	for (z = s; *z; s = z) {
		for (; *s && (isspace(*s) || *s == ':'); s++) {
			;
		}
		for (z = s; *z && !isspace(*z) && *z != ':'; z++) {
			;
		}
		tmp = *z;
		*z = 0;
		task = create_loadtask(s, NULL, ns, true);
		if (!task) {
			continue;
		}
		if (load_library_header(task)) {
			if (!task->isloaded) {
				append_loadtasks(tasks, task);
				task = NULL;
			}
		}
		if (task) {
			free_task(task);
		}
		*z = tmp;
	}
}
#endif

static int serialize_gnu_relro(int fd, struct dso *dso, ssize_t *file_offset)
{
	ssize_t count = dso->relro_end - dso->relro_start;
	ssize_t offset = 0;
	while (count > 0) {
		ssize_t write_size = TEMP_FAILURE_RETRY(write(fd, laddr(dso, dso->relro_start + offset), count));
		if (-1 == write_size) {
			LD_LOGE("Error serializing relro %{public}s: failed to write GNU_RELRO", dso->name);
			return -1;
		}
		offset += write_size;
		count -= write_size;
	}

	ssize_t size = dso->relro_end - dso->relro_start;
	void *map = mmap(
		laddr(dso, dso->relro_start),
		size,
		PROT_READ,
		MAP_PRIVATE | MAP_FIXED,
		fd,
		*file_offset);
	if (map == MAP_FAILED) {
		LD_LOGE("Error serializing relro %{public}s: failed to map GNU_RELRO", dso->name);
		return -1;
	}
	*file_offset += size;
	return 0;
}

static int map_gnu_relro(int fd, struct dso *dso, ssize_t *file_offset)
{
	ssize_t ext_fd_file_size = 0;
	struct stat ext_fd_file_stat;
	if (TEMP_FAILURE_RETRY(fstat(fd, &ext_fd_file_stat)) != 0) {
		LD_LOGE("Error mapping relro %{public}s: failed to get file state", dso->name);
		return -1;
	}
	ext_fd_file_size = ext_fd_file_stat.st_size;

	void *ext_temp_map = MAP_FAILED;
	ext_temp_map = mmap(NULL, ext_fd_file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ext_temp_map == MAP_FAILED) {
		LD_LOGE("Error mapping relro %{public}s: failed to map fd", dso->name);
		return -1;
	}

	char *file_base = (char *)(ext_temp_map) + *file_offset;
	char *mem_base = (char *)(laddr(dso, dso->relro_start));
	ssize_t start_offset = 0;
	ssize_t size = dso->relro_end - dso->relro_start;

	if (size > ext_fd_file_size - *file_offset) {
		LD_LOGE("Error mapping relro %{public}s: invalid file size", dso->name);
		return -1;
	}
	while (start_offset < size) {
		// Find start location.
		while (start_offset < size) {
			if (memcmp(mem_base + start_offset, file_base + start_offset, PAGE_SIZE) == 0) {
				break;
			}
			start_offset += PAGE_SIZE;
		}

		// Find end location.
		ssize_t end_offset = start_offset;
		while (end_offset < size) {
			if (memcmp(mem_base + end_offset, file_base + end_offset, PAGE_SIZE) != 0) {
				break;
			}
			end_offset += PAGE_SIZE;
		}

		// Map pages.
		ssize_t map_length = end_offset - start_offset;
		ssize_t map_offset = *file_offset + start_offset;
		if (map_length > 0) {
			void *map = mmap(
				mem_base + start_offset, map_length, PROT_READ, MAP_PRIVATE | MAP_FIXED, fd, map_offset);
			if (map == MAP_FAILED) {
				LD_LOGE("Error mapping relro %{public}s: failed to map GNU_RELRO", dso->name);
				munmap(ext_temp_map, ext_fd_file_size);
				return -1;
			}
		}

		start_offset = end_offset;
	}
	*file_offset += size;
	munmap(ext_temp_map, ext_fd_file_size);
	return 0;
}

static void handle_relro_sharing(struct dso *p, const dl_extinfo *extinfo, ssize_t *relro_fd_offset)
{
	if (extinfo == NULL) {
		return;
	}
	if (extinfo->flag & DL_EXT_WRITE_RELRO) {
		LD_LOGD("Serializing GNU_RELRO %{public}s", p->name);
		if (serialize_gnu_relro(extinfo->relro_fd, p, relro_fd_offset) < 0) {
			LD_LOGE("Error serializing GNU_RELRO %{public}s", p->name);
			error("Error serializing GNU_RELRO");
			if (runtime) longjmp(*rtld_fail, 1);
		}
	} else if (extinfo->flag & DL_EXT_USE_RELRO) {
		LD_LOGD("Mapping GNU_RELRO %{public}s", p->name);
		if (map_gnu_relro(extinfo->relro_fd, p, relro_fd_offset) < 0) {
			LD_LOGE("Error mapping GNU_RELRO %{public}s", p->name);
			error("Error mapping GNU_RELRO");
			if (runtime) longjmp(*rtld_fail, 1);
		}
	}
}

static void set_bss_vma_name(char *path_name, void *addr, size_t zeromap_size)
{
	char so_bss_name[ANON_NAME_MAX_LEN];
	if (path_name == NULL) {
		snprintf(so_bss_name, ANON_NAME_MAX_LEN, ".bss");
	} else {
		char *t = strrchr(path_name, '/');
		if (t) {
			snprintf(so_bss_name, ANON_NAME_MAX_LEN, "%s.bss", ++t);
		} else {
			snprintf(so_bss_name, ANON_NAME_MAX_LEN, "%s.bss", path_name);
		}
	}

	prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, addr, zeromap_size, so_bss_name);
}

static void find_and_set_bss_name(struct dso *p)
{
	size_t  cnt;
	Phdr *ph = p->phdr;
	for (cnt = p->phnum; cnt--; ph = (void *)((char *)ph + p->phentsize)) {
		if (ph->p_type != PT_LOAD) continue;
		size_t seg_start = p->base + ph->p_vaddr;
		size_t seg_file_end = seg_start + ph->p_filesz + PAGE_SIZE - 1 & -PAGE_SIZE;
		size_t seg_max_addr = seg_start + ph->p_memsz + PAGE_SIZE - 1 & -PAGE_SIZE;
		size_t zeromap_size = seg_max_addr - seg_file_end;
		if (zeromap_size > 0 && (ph->p_flags & PF_W)) {
			set_bss_vma_name(p->name, (void *)seg_file_end, zeromap_size);
		}
	}
}

static void sync_with_debugger(void)
{
	debug.ver = 1;
	debug.bp = dl_debug_state;
	debug.head = NULL;
	debug.base = ldso.base;

	add_dso_info_to_debug_map(head);

	debug.state = RT_CONSISTENT;
	_dl_debug_state();
}

static void notify_addition_to_debugger(struct dso *p)
{
	debug.state = RT_ADD;
	_dl_debug_state();

	add_dso_info_to_debug_map(p);

	debug.state = RT_CONSISTENT;
	_dl_debug_state();
}

static void notify_remove_to_debugger(struct dso *p)
{
	debug.state = RT_DELETE;
	_dl_debug_state();

	remove_dso_info_from_debug_map(p);

	debug.state = RT_CONSISTENT;
	_dl_debug_state();
}

static void add_dso_info_to_debug_map(struct dso *p)
{
	for (struct dso *so = p; so != NULL; so = so->next) {
		struct dso_debug_info *debug_info = malloc(sizeof(struct dso_debug_info));
		if (debug_info == NULL) {
			LD_LOGE("malloc error! dso name: %{public}s.", so->name);
			continue;
		}
#if DL_FDPIC
		debug_info->loadmap = so->loadmap;
#else
		debug_info->base = so->base;
#endif
		debug_info->name = so->name;
		debug_info->dynv = so->dynv;
		if (debug.head == NULL) {
			debug_info->prev = NULL;
			debug_info->next = NULL;
			debug.head = debug_tail = debug_info;
		} else {
			debug_info->prev = debug_tail;
			debug_info->next = NULL;
			debug_tail->next = debug_info;
			debug_tail = debug_info;
		}
		so->debug_info = debug_info;
	}
}

static void remove_dso_info_from_debug_map(struct dso *p)
{
	struct dso_debug_info *debug_info = p->debug_info;
	if (debug_info == debug_tail) {
		debug_tail = debug_tail->prev;
		debug_tail->next = NULL;
	} else {
		debug_info->next->prev = debug_info->prev;
		debug_info->prev->next = debug_info->next;
	}
	free(debug_info);
}
