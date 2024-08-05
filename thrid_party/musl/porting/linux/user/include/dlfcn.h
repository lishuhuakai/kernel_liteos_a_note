#ifndef	_DLFCN_H
#define	_DLFCN_H

#include <features.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_NOLOAD 4
#define RTLD_NODELETE 4096
#define RTLD_GLOBAL 256
#define RTLD_LOCAL  0

#define RTLD_NEXT    ((void *)-1)
#define RTLD_DEFAULT ((void *)0)

#define RTLD_DI_LINKMAP 2


/* create flags for dlns_create */
#define CREATE_INHERIT_DEFAULT 0x1
#define CREATE_INHERIT_CURRENT 0x2
#define LOCAL_NS_PREFERED 0x4 /* Use app's library when app's library has same name as system library. */

int    dlclose(void *);
char  *dlerror(void);
void  *dlopen(const char *, int);
void  *dlsym(void *__restrict, const char *__restrict);
void  *dlvsym(void *__restrict, const char *__restrict, const char *__restrict);

/* namespace apis */
#define NS_NAME_MAX 255
typedef struct {
	char name[NS_NAME_MAX+1];
} Dl_namespace;

/**
  * @brief Initialize a namespace structure for operating namespaces through related functional interfaces.
  * @param Dl_namespace * namespace handle.
  * @param char * namespace name.
  * @return void.
  * @retval none.
  */
void dlns_init(Dl_namespace *, const char *);

/**
  * @brief Gets the current namespace handle, or verifies that the given name namespace exists.
  * @param char * Namespace name.Gets the current caller namespace handle when name is null.
  * @param Dl_namespace * namespace handle.
  * @return return 0 on success,fail other values.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    ENOKEY(126) Required key not available.
  */
int dlns_get(const char *, Dl_namespace *);

/**
  * @brief open dso in given namespace which has own lib search paths, when namespace is null, it's same to dlopen().
  *        avoid using "default" as namespace, which is the default namespace.
  * @param Dl_namespace * namespace handle.
  * @param char * the name of the so file you want to open.
  * @param int open file mode.
  *    -- RTLD_LAZY.
  *    -- RTLD_NOW.
  *    -- RTLD_NOLOAD.
  *    -- RTLD_NODELETE.
  *    -- RTLD_GLOBAL.
  *    -- RTLD_LOCAL.
  * @return success: dynamic library handleoid,failed: NULL.
  * @retval none.
  */
void *dlopen_ns(Dl_namespace *, const char *, int);

/**
  * @brief create the namespace and set lib search paths of namespace,
  *        the paths should be splited by ':'. When namespace already exist,return error.
  *        avoid using "default" as namespace, which is the default namespace.
  * @param Dl_namespace * namespace handle.
  * @param char * lib path library that can be specified.
  * @return return 0 on success,fail other values.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    EEXIST(17) File exists.
  *    ENOMEM(12) Out of memory.
  */
int dlns_create(Dl_namespace *, const char *);

/**
  * @brief create the namespace and set lib search paths of namespace,
  *        like dlns_create, except can use flags to set parent ns.
  * @param Dl_namespace * namespace handle.
  * @param char * lib path library that can be specified.
  * #param int flags for create namespace, CREATE_INHERIT_CURRENT or CREATE_INHERIT_DEFAULT.
  * @return return 0 on success,fail other values.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    EEXIST(17) File exists.
  *    ENOMEM(12) Out of memory.
  */
int dlns_create2(Dl_namespace *, const char *, int);

/**
  * @brief make one namespace inherit another, and so it can use shared libs by the inherited one.
  *        param1: namespace, param2: inherited namespace, param3: shared libs.
  *        the shared libs should be splited by ':'. when it is null or empty, all libs can be shared.
  *        one namespace can inherit or be inherited by multiple ones.
  *        When namespaces do not exist, return error.
  * @param Dl_namespace * The first parameter is the namespace to inherit from.
  * @param Dl_namespace * The second parameter is the inherited namespace.
  * @param char * some library names to inherit.
  * @return return 0 on success,fail other values.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    ENOKEY(126) Required key not available.
  */
int dlns_inherit(Dl_namespace *, Dl_namespace *, const char *);

/**
  * @brief Set namespace lib_path.
  * @param name namespace name.
  * @param lib_path The lib path name that needs to be reset, it can be multiple, link with ":".
  * @return Returns 0 on success, other on failure.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    ENOKEY(126) Required key not available.
  */
int dlns_set_namespace_lib_path(const char *name, const char *lib_path);

/**
  * @brief Set namespace separated.
  * @param name namespace name.
  * @param separated separated.
  * @return Returns 0 on success, other on failure.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    ENOKEY(126) Required key not available.
  */
int dlns_set_namespace_separated(const char *name, const bool separated);

/**
  * @brief Set namespace permitted_paths.
  * @param name namespace name.
  * @param permitted_paths set new permitted_paths.
  * @return Returns 0 on success, other on failure.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    ENOKEY(126) Required key not available.
  */
int dlns_set_namespace_permitted_paths(const char *name, const char *permitted_paths);

/**
  * @brief Set namespace allowed_libs.
  * @param name namespace name.
  * @param allowed_libs set new allowed_libs.
  * @return Returns 0 on success, other on failure.
  * @retval
  *    EINVAL(22) Invalid argument.
  *    ENOKEY(126) Required key not available.
  */
int dlns_set_namespace_allowed_libs(const char *name, const char *allowed_libs);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
typedef struct {
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} Dl_info;
int dladdr(const void *, Dl_info *);
int dlinfo(void *, int, void *);
#endif

#if _REDIR_TIME64
__REDIR(dlsym, __dlsym_time64);
#endif

#ifdef __cplusplus
}
#endif

#endif
