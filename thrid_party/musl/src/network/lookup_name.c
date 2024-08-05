#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <resolv.h>
#include "lookup.h"
#include "stdio_impl.h"
#include "syscall.h"
#include <dlfcn.h>
#define BREAK 0
#define CONTINUE 1
#if OHOS_PERMISSION_INTERNET
uint8_t is_allow_internet(void);
#endif
#define FIXED_HOSTS_MAX_LENGTH 2
#define FIXED_HOSTS_STR_MAX_LENGTH 23

char fixed_hosts[][FIXED_HOSTS_STR_MAX_LENGTH] = {
	"127.0.0.1  localhost\r\n\0",
	"::1  ip6-localhost\r\n\0"
};

static int is_valid_hostname(const char *host)
{
	const unsigned char *s;
	if (strnlen(host, 255)-1 >= 254 || mbstowcs(0, host, 0) == -1) return 0;
	for (s=(void *)host; *s>=0x80 || *s=='.' || *s=='-' || isalnum(*s); s++);
	return !*s;
}

static int name_from_null(struct address buf[static 2], const char *name, int family, int flags)
{
	int cnt = 0;
	if (name) return 0;
	if (flags & AI_PASSIVE) {
		if (family != AF_INET6)
			buf[cnt++] = (struct address){ .family = AF_INET };
		if (family != AF_INET)
			buf[cnt++] = (struct address){ .family = AF_INET6 };
	} else {
		if (family != AF_INET6)
			buf[cnt++] = (struct address){ .family = AF_INET, .addr = { 127,0,0,1 } };
		if (family != AF_INET)
			buf[cnt++] = (struct address){ .family = AF_INET6, .addr = { [15] = 1 } };
	}
	return cnt;
}

static int name_from_numeric(struct address buf[static 1], const char *name, int family)
{
	return __lookup_ipliteral(buf, name, family);
}

static inline int get_hosts_str(char *line, int length, FILE *f, int *i)
{
	if (f) {
		return fgets(line, length, f);
	}
	if (*i < FIXED_HOSTS_MAX_LENGTH) {
		memcpy(line, fixed_hosts[*i], strlen(fixed_hosts[*i]));
		(*i)++;
		return 1;
	}
	return NULL;
}

static int name_from_hosts(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family)
{
	char line[512];
	size_t l = strlen(name);
	int cnt = 0, badfam = 0, have_canon = 0;
	unsigned char _buf[1032];
	FILE _f, *f = __fopen_rb_ca("/etc/hosts", &_f, _buf, sizeof _buf);
	int i = 0;
	while (i < FIXED_HOSTS_MAX_LENGTH && get_hosts_str(line, sizeof line, f, &i) && cnt < MAXADDRS) {
		char *p, *z;

		if ((p=strchr(line, '#'))) *p++='\n', *p=0;
		for(p=line+1; (p=strstr(p, name)) &&
			(!isspace(p[-1]) || !isspace(p[l])); p++);
		if (!p) continue;

		/* Isolate IP address to parse */
		for (p=line; *p && !isspace(*p); p++);
		*p++ = 0;
		switch (name_from_numeric(buf+cnt, line, family)) {
		case 1:
			cnt++;
			break;
		case 0:
			continue;
		default:
			badfam = EAI_NONAME;
			break;
		}

		if (have_canon) continue;

		/* Extract first name as canonical name */
		for (; *p && isspace(*p); p++);
		for (z=p; *z && !isspace(*z); z++);
		*z = 0;
		if (is_valid_hostname(p)) {
			have_canon = 1;
			memcpy(canon, p, z-p+1);
		}
	}
	if (f) {
		__fclose_ca(f);
	}
	return cnt ? cnt : badfam;
}

struct dpc_ctx {
	struct address *addrs;
	char *canon;
	int cnt;
	int rrtype;
};

#define RR_A 1
#define RR_CNAME 5
#define RR_AAAA 28
#define MAX_QUERY_SIZE 5
#define VALID_ANSWER 1
#define MIN_ANSWER_TYPE 1
#define MAX_ANSWER_TYPE 2
#define MAX_NAME_LENGTH 256

#define ABUF_SIZE 4800

static int dns_parse_callback(void *c, int rr, const void *data, int len, const void *packet, int plen)
{
	char tmp[256];
	int family;
	struct dpc_ctx *ctx = c;
	if (rr == RR_CNAME) {
		if (__dn_expand(packet, (const unsigned char *)packet + plen,
		    data, tmp, sizeof tmp) > 0 && is_valid_hostname(tmp))
			strcpy(ctx->canon, tmp);
		return 0;
	}
	if (ctx->cnt >= MAXADDRS) return 0;
	if (rr != ctx->rrtype) return 0;
	switch (rr) {
	case RR_A:
		if (len != 4) return -1;
		family = AF_INET;
		break;
	case RR_AAAA:
		if (len != 16) return -1;
		family = AF_INET6;
		break;
	}
	ctx->addrs[ctx->cnt].family = family;
	ctx->addrs[ctx->cnt].scopeid = 0;
	memcpy(ctx->addrs[ctx->cnt++].addr, data, len);
	return 0;
}

#if OHOS_DNS_PROXY_BY_NETSYS
static JudgeIpv6 load_ipv6_judger(void)
{
	static JudgeIpv6 ipv6_judger = NULL;
	resolve_dns_sym((void **) &ipv6_judger, OHOS_JUDGE_IPV6_FUNC_NAME);
	return ipv6_judger;
}
#endif

static int IsIpv6Enable(int netid)
{
    int ret = 0;
#if OHOS_DNS_PROXY_BY_NETSYS
    JudgeIpv6 func = load_ipv6_judger();
	if (!func) {
		return -1;
	}

	ret = func(netid);
	if (ret < 0) {
		return -1;
	}
#endif
    return ret;
}

static int IsAnswerValid(const unsigned char *answer, int alen)
{
	if (alen < 4 || (answer[3] & 15) == 2) {
		return EAI_AGAIN;
	}
	if ((answer[3] & 15) == 3) return 0;
	if ((answer[3] & 15) != 0) {
		return EAI_FAIL;
	}
	return VALID_ANSWER;
}

static int name_from_dns(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family, const struct resolvconf *conf, int netid)
{
	unsigned char qbuf[2][280], abuf[2][ABUF_SIZE];
	const unsigned char *qp[2] = { qbuf[0], qbuf[1] };
	unsigned char *ap[2] = { abuf[0], abuf[1] };
	int qlens[2], alens[2], qtypes[2];
	int queryNum = 2;
	struct dpc_ctx ctx = { .addrs = buf, .canon = canon };
	static const struct { int af; int rr; } afrr_ipv6_enable[2] = {
		{ .af = AF_INET, .rr = RR_AAAA },
		{ .af = AF_INET6, .rr = RR_A },
	};
	static const struct { int af; int rr; } afrr_ipv4_only[1] = {
		{ .af = AF_INET6, .rr = RR_A },
	};
	struct {int af; int rr;} *afrr = afrr_ipv6_enable;

	if (!IsIpv6Enable(netid) || (family == AF_INET)) {
		if (family == AF_INET6) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: Network scenario mismatch: %{public}d", __func__, __LINE__, EAI_SYSTEM);
#endif
			return EAI_SYSTEM;
		}
		queryNum = 1;
		afrr = afrr_ipv4_only;
	} else {
		queryNum = 2;
		afrr = afrr_ipv6_enable;
	}

	int cname_count = 0;
	const char *queryName = name;
	int checkBuf[MAX_ANSWER_TYPE] = {VALID_ANSWER, VALID_ANSWER};
	while (strnlen(queryName, MAX_NAME_LENGTH) != 0 && cname_count < MAX_QUERY_SIZE) {
		int i, nq = 0;
		for (i = 0; i < queryNum; i++) {
			if (family != afrr[i].af) {
				qlens[nq] = __res_mkquery(0, queryName, 1, afrr[i].rr,
					0, 0, 0, qbuf[nq], sizeof *qbuf);
				if (qlens[nq] == -1) {
#ifndef __LITEOS__
					MUSL_LOGE("%{public}s: %{public}d: Illegal querys: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
					return 0;
				}
				qtypes[nq] = afrr[i].rr;
				qbuf[nq][3] = 0; /* don't need AD flag */
				/* Ensure query IDs are distinct. */
				if (nq && qbuf[nq][0] == qbuf[0][0])
					qbuf[nq][0]++;
				nq++;
			}
		}

		if (res_msend_rc_ext(netid, nq, qp, qlens, ap, alens, sizeof *abuf, conf) < 0)
			return EAI_SYSTEM;

		for (i=0; i<nq; i++) {
			checkBuf[i] = IsAnswerValid(abuf[i], alens[i]);
		}
		if (checkBuf[MIN_ANSWER_TYPE - 1] != VALID_ANSWER &&
		   (nq < MAX_ANSWER_TYPE || checkBuf[MAX_ANSWER_TYPE - 1] != VALID_ANSWER)) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: Illegal answers, errno id: %{public}d",
				__func__, __LINE__, checkBuf[MIN_ANSWER_TYPE - 1]);
#endif
			return checkBuf[MIN_ANSWER_TYPE - 1];
		}

		for (i=nq-1; i>=0; i--) {
			ctx.rrtype = qtypes[i];
			if (alens[i] > sizeof(abuf[i])) alens[i] = sizeof abuf[i];
			__dns_parse(abuf[i], alens[i], dns_parse_callback, &ctx);
		}
		if (ctx.cnt) return ctx.cnt;
		queryName = ctx.canon;
		cname_count++;
	}
#ifndef __LITEOS__
	MUSL_LOGE("%{public}s: %{public}d: failed to parse dns : %{public}d", __func__, __LINE__, cname_count);
#endif
	return EAI_NONAME;
}

static int name_from_dns_search(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family, int netid)
{
#if OHOS_PERMISSION_INTERNET
	if (is_allow_internet() == 0) {
		errno = EPERM;
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d: internet is not allowed", __func__, __LINE__);
#endif
		return -1;
	}
#endif

	char search[256];
	struct resolvconf conf;
	size_t l, dots;
	char *p, *z;

	int res = get_resolv_conf_ext(&conf, search, sizeof search, netid);
	if (res < 0) return res;

	/* Count dots, suppress search when >=ndots or name ends in
	 * a dot, which is an explicit request for global scope. */
	for (dots=l=0; name[l]; l++) if (name[l]=='.') dots++;
	if (dots >= conf.ndots || name[l-1]=='.') *search = 0;

	/* Strip final dot for canon, fail if multiple trailing dots. */
	if (name[l-1]=='.') l--;
	if (!l || name[l-1]=='.') {
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d: fail when multiple trailing dots: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
		return EAI_NONAME;
	}

	/* This can never happen; the caller already checked length. */
	if (l >= 256) return EAI_NONAME;

	/* Name with search domain appended is setup in canon[]. This both
	 * provides the desired default canonical name (if the requested
	 * name is not a CNAME record) and serves as a buffer for passing
	 * the full requested name to name_from_dns. */
	memcpy(canon, name, l);
	canon[l] = '.';

	for (p=search; *p; p=z) {
		for (; isspace(*p); p++);
		for (z=p; *z && !isspace(*z); z++);
		if (z==p) break;
		if (z-p < 256 - l - 1) {
			memcpy(canon+l+1, p, z-p);
			canon[z-p+1+l] = 0;
			int cnt = name_from_dns(buf, canon, canon, family, &conf, netid);
			if (cnt) return cnt;
		}
	}

	canon[l] = 0;
	return name_from_dns(buf, canon, name, family, &conf, netid);
}

static const struct policy {
	unsigned char addr[16];
	unsigned char len, mask;
	unsigned char prec, label;
} defpolicy[] = {
	{ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1", 15, 0xff, 50, 0 },
	{ "\0\0\0\0\0\0\0\0\0\0\xff\xff", 11, 0xff, 35, 4 },
	{ "\x20\2", 1, 0xff, 30, 2 },
	{ "\x20\1", 3, 0xff, 5, 5 },
	{ "\xfc", 0, 0xfe, 3, 13 },
#if 0
	/* These are deprecated and/or returned to the address
	 * pool, so despite the RFC, treating them as special
	 * is probably wrong. */
	{ "", 11, 0xff, 1, 3 },
	{ "\xfe\xc0", 1, 0xc0, 1, 11 },
	{ "\x3f\xfe", 1, 0xff, 1, 12 },
#endif
	/* Last rule must match all addresses to stop loop. */
	{ "", 0, 0, 40, 1 },
};

static const struct policy *policyof(const struct in6_addr *a)
{
	int i;
	for (i=0; ; i++) {
		if (memcmp(a->s6_addr, defpolicy[i].addr, defpolicy[i].len))
			continue;
		if ((a->s6_addr[defpolicy[i].len] & defpolicy[i].mask)
		    != defpolicy[i].addr[defpolicy[i].len])
			continue;
		return defpolicy+i;
	}
}

static int labelof(const struct in6_addr *a)
{
	return policyof(a)->label;
}

static int scopeof(const struct in6_addr *a)
{
	if (IN6_IS_ADDR_MULTICAST(a)) return a->s6_addr[1] & 15;
	if (IN6_IS_ADDR_LINKLOCAL(a)) return 2;
	if (IN6_IS_ADDR_LOOPBACK(a)) return 2;
	if (IN6_IS_ADDR_SITELOCAL(a)) return 5;
	return 14;
}

static int prefixmatch(const struct in6_addr *s, const struct in6_addr *d)
{
	/* FIXME: The common prefix length should be limited to no greater
	 * than the nominal length of the prefix portion of the source
	 * address. However the definition of the source prefix length is
	 * not clear and thus this limiting is not yet implemented. */
	unsigned i;
	for (i=0; i<128 && !((s->s6_addr[i/8]^d->s6_addr[i/8])&(128>>(i%8))); i++);
	return i;
}

#define DAS_USABLE              0x40000000
#define DAS_MATCHINGSCOPE       0x20000000
#define DAS_MATCHINGLABEL       0x10000000
#define DAS_PREC_SHIFT          20
#define DAS_SCOPE_SHIFT         16
#define DAS_PREFIX_SHIFT        8
#define DAS_ORDER_SHIFT         0

static int addrcmp(const void *_a, const void *_b)
{
	const struct address *a = _a, *b = _b;
	return b->sortkey - a->sortkey;
}

int lookup_name_ext(struct address buf[static MAXADDRS], char canon[static 256], const char *name,
					int family, int flags, int netid)
{
	int cnt = 0, i, j;

#if OHOS_DNS_PROXY_BY_NETSYS
	DNS_CONFIG_PRINT("lookup_name_ext \n");
#endif

	*canon = 0;
	if (name) {
		/* reject empty name and check len so it fits into temp bufs */
		size_t l = strnlen(name, 255);
		if (l-1 >= 254) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: Illegal name length: %{public}zu", __func__, __LINE__, l);
#endif
			return EAI_NONAME;
		}
		memcpy(canon, name, l+1);
	}

	/* Procedurally, a request for v6 addresses with the v4-mapped
	 * flag set is like a request for unspecified family, followed
	 * by filtering of the results. */
	if (flags & AI_V4MAPPED) {
		if (family == AF_INET6) family = AF_UNSPEC;
		else flags -= AI_V4MAPPED;
	}

	/* Try each backend until there's at least one result. */
	cnt = name_from_null(buf, name, family, flags);
	if (!cnt) cnt = name_from_numeric(buf, name, family);
	if (!cnt && !(flags & AI_NUMERICHOST)) {
		cnt = predefined_host_name_from_hosts(buf, canon, name, family);
		if (!cnt) cnt = name_from_hosts(buf, canon, name, family);
		if (!cnt) cnt = name_from_dns_search(buf, canon, name, family, netid);
	}
	if (cnt<=0) return cnt ? cnt : EAI_NONAME;

	/* Filter/transform results for v4-mapped lookup, if requested. */
	if (flags & AI_V4MAPPED) {
		if (!(flags & AI_ALL)) {
			/* If any v6 results exist, remove v4 results. */
			for (i=0; i<cnt && buf[i].family != AF_INET6; i++);
			if (i<cnt) {
				for (j=0; i<cnt; i++) {
					if (buf[i].family == AF_INET6)
						buf[j++] = buf[i];
				}
				cnt = i = j;
			}
		}
		/* Translate any remaining v4 results to v6 */
		for (i=0; i<cnt; i++) {
			if (buf[i].family != AF_INET) continue;
			memcpy(buf[i].addr+12, buf[i].addr, 4);
			memcpy(buf[i].addr, "\0\0\0\0\0\0\0\0\0\0\xff\xff", 12);
			buf[i].family = AF_INET6;
		}
	}

	/* No further processing is needed if there are fewer than 2
	 * results or if there are only IPv4 results. */
	if (cnt<2 || family==AF_INET) return cnt;
	for (i=0; i<cnt; i++) if (buf[i].family != AF_INET) break;
	if (i==cnt) return cnt;

	int cs;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	/* The following implements a subset of RFC 3484/6724 destination
	 * address selection by generating a single 31-bit sort key for
	 * each address. Rules 3, 4, and 7 are omitted for having
	 * excessive runtime and code size cost and dubious benefit.
	 * So far the label/precedence table cannot be customized. */
	for (i=0; i<cnt; i++) {
		int family = buf[i].family;
		int key = 0;
		struct sockaddr_in6 sa6 = { 0 }, da6 = {
			.sin6_family = AF_INET6,
			.sin6_scope_id = buf[i].scopeid,
			.sin6_port = 65535
		};
		struct sockaddr_in sa4 = { 0 }, da4 = {
			.sin_family = AF_INET,
			.sin_port = 65535
		};
		void *sa, *da;
		socklen_t salen, dalen;
		if (family == AF_INET6) {
			memcpy(da6.sin6_addr.s6_addr, buf[i].addr, 16);
			da = &da6; dalen = sizeof da6;
			sa = &sa6; salen = sizeof sa6;
		} else {
			memcpy(sa6.sin6_addr.s6_addr,
				"\0\0\0\0\0\0\0\0\0\0\xff\xff", 12);
			memcpy(da6.sin6_addr.s6_addr+12, buf[i].addr, 4);
			memcpy(da6.sin6_addr.s6_addr,
				"\0\0\0\0\0\0\0\0\0\0\xff\xff", 12);
			memcpy(da6.sin6_addr.s6_addr+12, buf[i].addr, 4);
			memcpy(&da4.sin_addr, buf[i].addr, 4);
			da = &da4; dalen = sizeof da4;
			sa = &sa4; salen = sizeof sa4;
		}
		const struct policy *dpolicy = policyof(&da6.sin6_addr);
		int dscope = scopeof(&da6.sin6_addr);
		int dlabel = dpolicy->label;
		int dprec = dpolicy->prec;
		int prefixlen = 0;
		int fd = socket(family, SOCK_DGRAM|SOCK_CLOEXEC, IPPROTO_UDP);
		if (fd >= 0) {
			if (!connect(fd, da, dalen)) {
				key |= DAS_USABLE;
				if (!getsockname(fd, sa, &salen)) {
					if (family == AF_INET) memcpy(
						sa6.sin6_addr.s6_addr+12,
						&sa4.sin_addr, 4);
					if (dscope == scopeof(&sa6.sin6_addr))
						key |= DAS_MATCHINGSCOPE;
					if (dlabel == labelof(&sa6.sin6_addr))
						key |= DAS_MATCHINGLABEL;
					prefixlen = prefixmatch(&sa6.sin6_addr,
						&da6.sin6_addr);
				}
			}
			close(fd);
		}
		key |= dprec << DAS_PREC_SHIFT;
		key |= (15-dscope) << DAS_SCOPE_SHIFT;
		key |= prefixlen << DAS_PREFIX_SHIFT;
		key |= (MAXADDRS-i) << DAS_ORDER_SHIFT;
		buf[i].sortkey = key;
	}
	qsort(buf, cnt, sizeof *buf, addrcmp);

	pthread_setcancelstate(cs, 0);

	return cnt;
}

int __lookup_name(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family, int flags)
{
	return lookup_name_ext(buf, canon, name, family, flags, 0);
}

typedef struct _linknode {
    struct _linknode *_next;
    void *_data;
}linknode;

typedef struct _linkList {
    linknode *_phead;
    int _count;
}linkList;

static linkList *create_linklist(void);
static int destory_linklist(linkList *plist);
static int linklist_size(linkList *plist);
static linknode *get_linknode(linkList *plist, int index);
static int linklist_append_last(linkList *plist, void *pdata);
static int linklist_delete(linkList *plist, int index);
static int linklist_delete_first(linkList *plist);
static int linklist_delete_last(linkList *plist);

typedef struct {
	char*    host;
	char*    ip;
}host_ip_pair;

static const char GROUP_SEPARATOR[] = "|";
static const char SEPARATOR[]       = ",";

static linkList   *host_ips_list_ = NULL;

static char *strsep(char **s, const char *ct)
{
	char *sbegin = *s;
	char *end;
	if (sbegin == NULL) {
		return NULL;
	}
	end = strpbrk(sbegin, ct);
	if (end) {
		*end++ = '\0';
	}
	*s = end;
	return sbegin;
}

int predefined_host_clear_all_hosts(void)
{
	if (host_ips_list_ != NULL) {
		linknode *pnode = host_ips_list_->_phead;
		while (pnode != NULL) {
			free(pnode->_data);
			pnode->_data = NULL;
			pnode = pnode->_next;
		}

		destory_linklist(host_ips_list_);
		free(host_ips_list_);
		host_ips_list_ = NULL;
		return 0;
	}
	return -1;
}

int predefined_host_remove_host(const char *host)
{
	int remove_cnt = 0;
	if (host_ips_list_ != NULL) {
		linknode     *pnode = NULL;
		host_ip_pair *pinfo = NULL;
		int cnt = linklist_size(host_ips_list_);

		for (int i = cnt - 1; i >= 0; i--) {
			pnode = get_linknode(host_ips_list_, i);
			if (pnode != NULL) {
				pinfo = (host_ip_pair*)pnode->_data;
				if (strcmp(pinfo->host, host) == 0) {
					free(pinfo);
					linklist_delete(host_ips_list_, i);
					remove_cnt++;
				}
			}
		}
	}

	return remove_cnt == 0 ? -1 : 0;
}

static int _predefined_host_is_contain_host_ip(const char* host, const char* ip, int is_check_ip)
{
	if (host_ips_list_ != NULL) {
		linknode *pnode = host_ips_list_->_phead;

		while (pnode != NULL) {
			host_ip_pair *pinfo = (host_ip_pair*)pnode->_data;
			if (strcmp(pinfo->host, host) == 0) {
				if (is_check_ip) {
					if (strcmp(pinfo->ip, ip) == 0) {
						return 1;
					}
				} else {
					return 1;
				}
			}
			pnode = pnode->_next;
		}
	}
	return 0;
}

int predefined_host_is_contain_host(const char *host)
{
	return _predefined_host_is_contain_host_ip(host, NULL, 0);
}

static int _predefined_host_add_record(char* host, char* ip)
{
	int head_len        = sizeof(host_ip_pair);
	int host_len        = strlen(host);
	int ip_len          = strlen(ip);
	int total           = host_len + 1 + ip_len + 1 + head_len;
	char *pdata         = calloc(1, total);
	host_ip_pair *pinfo = (host_ip_pair*)pdata;

	if (pdata == NULL) {
		return EAI_NONAME;
	}

	char*  i_host = (char*)(pdata + head_len);
	char*  i_ip   = (char*)(pdata + head_len + host_len + 1);

	memcpy(i_host, host, host_len + 1);
	memcpy(i_ip, ip, ip_len + 1);

	pinfo->host = i_host;
	pinfo->ip   = i_ip;

	linklist_append_last(host_ips_list_, pdata);
	return 0;
}

static int _predefined_host_parse_host_ips(char* params)
{
	char* cmd  = NULL;
	int   ret  = 0;
	int   cnt  = 0;

	while ((cmd = strsep(&params, GROUP_SEPARATOR)) != NULL) {
		char* host = strsep(&cmd, SEPARATOR);
		char* ip = NULL;

		while ((ip = strsep(&cmd, SEPARATOR)) != NULL) {
			cnt++;
			if (!_predefined_host_is_contain_host_ip(host, ip, 1)) {
				ret = _predefined_host_add_record(host, ip);
				if (ret != 0) {
					return ret;
				}
			}
		}
	}

	return cnt > 0 ? 0 : -1;
}

int predefined_host_lookup_ip(const char* host, const char* serv,
    const struct addrinfo* hint, struct addrinfo** res)
{
	int status = -1;
    if (host_ips_list_ == NULL) {
		return -1;
	}
    linknode *pnode = host_ips_list_->_phead;
    while (pnode != NULL) {
        host_ip_pair *pinfo = (host_ip_pair*)pnode->_data;
        if (strcmp(pinfo->host, host) == 0) {
            status = getaddrinfo(pinfo->ip, NULL, hint, res);
            if (status == 0) {
                return status;
            }
        }
        pnode = pnode->_next;
    }
    return status;
}

int predefined_host_set_hosts(const char* host_ips)
{
	if (host_ips == NULL) {
	    return EAI_NONAME;
	}

	int len = strlen(host_ips);
	if (len == 0) {
		return EAI_NONAME;
	}

	if (host_ips_list_ == NULL) {
		host_ips_list_ = create_linklist();
		if (host_ips_list_ == NULL) {
			return EAI_MEMORY;
		}
	}

	char *host_ips_str = calloc(1, len + 1);
	if (host_ips_str == NULL) {
		return EAI_MEMORY;
	}

	memcpy(host_ips_str, host_ips, len + 1);

	int ret = _predefined_host_parse_host_ips(host_ips_str);

	free(host_ips_str);

	return ret;
}

int predefined_host_set_host(const char* host, const char* ip)
{
	if (host == NULL || strlen(host) == 0 || ip == NULL || strlen(ip) == 0) {
		return -1;
	}

	if (host_ips_list_ == NULL) {
		host_ips_list_ = create_linklist();
		if (host_ips_list_ == NULL) {
			return EAI_NONAME;
		}
	}

	if (_predefined_host_is_contain_host_ip(host, ip, 1)) {
		return 0;
	}

	return _predefined_host_add_record(host, ip);
}

int predefined_host_name_from_hosts(
		struct address buf[static MAXADDRS],
		char canon[static 256], const char *name, int family)
{
	int size = 256;
	int cnt = 0;
	if (host_ips_list_ != NULL) {
		linknode *pnode = host_ips_list_->_phead;

		while (pnode != NULL && cnt < MAXADDRS) {
			host_ip_pair *pinfo = (host_ip_pair*)pnode->_data;
			if (strcmp(pinfo->host, name) == 0) {
				if (__lookup_ipliteral(buf+cnt, pinfo->ip, family) == 1) {
					cnt++;
				}
			}
			pnode = pnode->_next;
		}
	}

	if (cnt > 0) {
		memcpy(canon, name, size);
	}
	return cnt;
}

static inline void free_linknodedata(linknode *pnode)
{
	if (NULL != pnode) {
		free(pnode);
	}
}

static linknode *create_linknode(void *data)
{
	linknode *pnode = (linknode *)calloc(1, sizeof(linknode));
	if (NULL == pnode) {
		return NULL;
	}
	pnode->_data = data;
	pnode->_next = NULL;

	return pnode;
}

static linkList *create_linklist(void)
{
	linkList *plist = (linkList *)calloc(1, sizeof(linkList));
	if (NULL == plist) {
		return NULL;
	}
	plist->_phead = NULL;
	plist->_count = 0;

	return plist;
}

static int destory_linklist(linkList *plist)
{
	if (NULL == plist) {
		return -1;
	}

	linknode *pnode = plist->_phead;
	linknode *ptmp = NULL;
	while (pnode != NULL) {
		ptmp = pnode;
		pnode = pnode->_next;
		free_linknodedata(ptmp);
	}

	plist->_phead = NULL;
	plist->_count = 0;

	return 0;
}

static int linklist_size(linkList *plist)
{
	if (NULL == plist) {
		return 0;
	}

	return plist->_count;
}

static linknode *get_linknode(linkList *plist, int index)
{
	if (index < 0 || index >= plist->_count) {
		return NULL;
	}

	int i = 0;
	linknode *pnode = plist->_phead;
	if (NULL == pnode) {
		return NULL;
	}
	while ((i++) < index && NULL != pnode) {
		pnode = pnode->_next;
	}

	return pnode;
}

static int linklist_append_last(linkList *plist, void *pdata)
{
	if (NULL == plist) {
		return -1;
	}

	linknode *pnode = create_linknode(pdata);
	if (NULL == pnode) {
		return -1;
	}

	if (NULL == plist->_phead) {
		plist->_phead = pnode;
		plist->_count++;
	} else {
		linknode *plastnode = get_linknode(plist, plist->_count - 1);
		if (NULL == plastnode) {
			return -1;
		}
		plastnode->_next = pnode;
		plist->_count++;
	}

	return 0;
}

static int linklist_delete(linkList *plist, int index)
{
	if (NULL == plist || NULL == plist->_phead || plist->_count <= 0) {
		return -1;
	}

	if (index == 0) {
		return linklist_delete_first(plist);
	} else if (index == (plist->_count - 1)) {
		return linklist_delete_last(plist);
	} else {
		linknode *pindex = get_linknode(plist, index);
		if (NULL == pindex) {
			return -1;
		}
		linknode *preindex = get_linknode(plist, index - 1);
		if (NULL == preindex) {
			return -1;
		}

		preindex->_next = pindex->_next;

		free_linknodedata(pindex);
		plist->_count--;
	}

	return 0;
}

static int linklist_delete_first(linkList *plist)
{
	if (NULL == plist || NULL == plist->_phead || plist->_count <= 0) {
		return -1;
	}

	linknode *phead = plist->_phead;
	plist->_phead = plist->_phead->_next;

	free_linknodedata(phead);
	plist->_count--;

	return 0;
}

static int linklist_delete_last(linkList *plist)
{
	if (NULL == plist || NULL == plist->_phead || plist->_count <= 0) {
		return -1;
	}

	linknode *plastsecondnode = get_linknode(plist, plist->_count - 2);
	if (NULL != plastsecondnode) {
		linknode *plastnode = plastsecondnode->_next;
		plastsecondnode->_next = NULL;

		free_linknodedata(plastnode);
		plist->_count--;
	} else {
		linknode *plastnode = get_linknode(plist, plist->_count - 1);
		plist->_phead = NULL;
		plist->_count = 0;

		free_linknodedata(plastnode);
	}

	return 0;
}
