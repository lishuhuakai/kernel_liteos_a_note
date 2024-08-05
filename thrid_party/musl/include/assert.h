#include <features.h>

#undef assert
#ifndef MUSL_ASSERT_H
#define MUSL_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif
typedef enum Assert_Status {
    ASSERT_ABORT,
    ASSERT_RETRY,
    ASSERT_IGNORE
}Assert_Status;

typedef struct AssertFailureInfo {
    char *expression;
    char *file;
    char *function;
    int line;
}AssertFailureInfo;

typedef Assert_Status(*assert_call)(AssertFailureInfo assert_fail);
void set_assert_callback(assert_call cb);

#ifdef __cplusplus
}
#endif
#endif

#ifdef NDEBUG
#define	assert(x) (void)0
#else
#define assert(x) ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__),0)))
#endif

#if __STDC_VERSION__ >= 201112L && !defined(__cplusplus)
#define static_assert _Static_assert
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __assert_fail (const char *, const char *, int, const char *);

#ifdef __cplusplus
}
#endif
