#define _GNU_SOURCE
#include <utmpx.h>
#include <stddef.h>
#include <errno.h>
#include <unsupported_api.h>

void endutxent(void)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
}

void setutxent(void)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
}

struct utmpx *getutxent(void)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return NULL;
}

struct utmpx *getutxid(const struct utmpx *ut)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return NULL;
}

struct utmpx *getutxline(const struct utmpx *ut)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return NULL;
}

struct utmpx *pututxline(const struct utmpx *ut)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return NULL;
}

void updwtmpx(const char *f, const struct utmpx *u)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
}

static int __utmpxname(const char *f)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	errno = ENOTSUP;
	return -1;
}

weak_alias(endutxent, endutent);
weak_alias(setutxent, setutent);
weak_alias(getutxent, getutent);
weak_alias(getutxid, getutid);
weak_alias(getutxline, getutline);
weak_alias(pututxline, pututline);
weak_alias(updwtmpx, updwtmp);
weak_alias(__utmpxname, utmpname);
weak_alias(__utmpxname, utmpxname);
