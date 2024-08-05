#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <endian.h>
#include <errno.h>
#include "lookup.h"

#define COST_FOR_MS 1000
#define COST_FOR_NANOSEC 1000000
#define DNS_QUERY_SUCCESS 0
#define DNS_QUERY_COMMOM_FAIL (-1)
#define GETADDRINFO_PRINT_DEBUG(...)

int reportdnsresult(int netid, char* name, int usedtime, int queryret, struct addrinfo *res, struct queryparam *param)
{
#if OHOS_DNS_PROXY_BY_NETSYS
	if (dns_post_result_to_netsys_cache(netid, name, usedtime, queryret, res, param) == 0) {
		GETADDRINFO_PRINT_DEBUG("getaddrinfo_ext reportdnsresult fail\n");
	}
#endif
	return 0;
}

static custom_dns_resolver g_customdnsresolvehook;
static pthread_key_t g_recursiveKey;
static int* g_recursive;

int setdnsresolvehook(custom_dns_resolver hookfunc)
{
	int ret = -1;
	if (g_customdnsresolvehook) {
		return ret;
	}
	if (hookfunc) {
		g_customdnsresolvehook = hookfunc;
		pthread_key_create(&g_recursiveKey, NULL);
		ret = 0;
	}
	return ret;
}

int removednsresolvehook()
{
	g_customdnsresolvehook = NULL;
	if (g_recursive) {
		free(g_recursive);
		g_recursive = NULL;
	}
	if (g_recursiveKey) {
		pthread_key_delete(g_recursiveKey);
		g_recursiveKey = NULL;
	}
	return 0;
}

int getaddrinfo_hook(const char* host, const char* serv, const struct addrinfo* hints,
    struct addrinfo** res)
{
    if (g_customdnsresolvehook) {
        int ret = g_customdnsresolvehook(host, serv, hints, res);
        if (ret == 0) {
            return ret;
        }
    }
    return predefined_host_lookup_ip(host, serv, hints, res);
}

int getaddrinfo(const char *restrict host, const char *restrict serv, const struct addrinfo *restrict hint, struct addrinfo **restrict res)
{
	struct queryparam param = {0, 0, 0, 0, NULL};
	return getaddrinfo_ext(host, serv, hint, res, &param);
}

int getaddrinfo_ext(const char *restrict host, const char *restrict serv, const struct addrinfo *restrict hint,
					struct addrinfo **restrict res, struct queryparam *restrict param)
{
	int netid = 0;
	int type = 0;
	int usedtime = 0;
	struct timeval timeStart, timeEnd;

	if (!host && !serv) return EAI_NONAME;
	if (!param) {
		netid = 0;
		type = 0;
	} else {
		netid = param->qp_netid;
		type = param->qp_type;
	}

	if (g_customdnsresolvehook) {
		g_recursive = pthread_getspecific(g_recursiveKey);
		if (g_recursive == NULL) {
			int *newRecursive = malloc(sizeof(int));
			*newRecursive = 0;
			pthread_setspecific(g_recursiveKey, newRecursive);
			g_recursive = newRecursive;
		}
		if (*g_recursive == 0) {
			++(*g_recursive);
			int ret = g_customdnsresolvehook(host, serv, hint, res);
			--(*g_recursive);
			return ret;
		}
	}

#if OHOS_DNS_PROXY_BY_NETSYS
	GETADDRINFO_PRINT_DEBUG("getaddrinfo_ext netid:%{public}d type:%{public}d \n", netid, type);
	if (type == QEURY_TYPE_NORMAL && predefined_host_is_contain_host(host) == 0) {
		if (dns_get_addr_info_from_netsys_cache2(netid, host, serv, hint, res) == 0) {
			GETADDRINFO_PRINT_DEBUG("getaddrinfo_ext get from netsys cache OK\n");
			reportdnsresult(netid, host, 0, DNS_QUERY_SUCCESS, *res, param);
			return 0;
		}
	}
#endif

	struct service ports[MAXSERVS];
	struct address addrs[MAXADDRS];
	char canon[256], *outcanon;
	int nservs, naddrs, nais, canon_len, i, j, k;
	int family = AF_UNSPEC, flags = 0, proto = 0, socktype = 0;
	struct aibuf *out;

	if (hint) {
		family = hint->ai_family;
		flags = hint->ai_flags;
		proto = hint->ai_protocol;
		socktype = hint->ai_socktype;

		const int mask = AI_PASSIVE | AI_CANONNAME | AI_NUMERICHOST |
			AI_V4MAPPED | AI_ALL | AI_ADDRCONFIG | AI_NUMERICSERV;
		if ((flags & mask) != flags) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: bad hint ai_flag: %{public}d", __func__, __LINE__, flags);
#endif
			return EAI_BADFLAGS;
		}

		switch (family) {
		case AF_INET:
		case AF_INET6:
		case AF_UNSPEC:
			break;
		default:
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: wrong family in hint: %{public}d", __func__, __LINE__, family);
#endif
			return EAI_FAMILY;
		}
	}

	if (flags & AI_ADDRCONFIG) {
		/* Define the "an address is configured" condition for address
		 * families via ability to create a socket for the family plus
		 * routability of the loopback address for the family. */
		static const struct sockaddr_in lo4 = {
			.sin_family = AF_INET, .sin_port = 65535,
			.sin_addr.s_addr = __BYTE_ORDER == __BIG_ENDIAN
				? 0x7f000001 : 0x0100007f
		};
		static const struct sockaddr_in6 lo6 = {
			.sin6_family = AF_INET6, .sin6_port = 65535,
			.sin6_addr = IN6ADDR_LOOPBACK_INIT
		};
		int tf[2] = { AF_INET, AF_INET6 };
		const void *ta[2] = { &lo4, &lo6 };
		socklen_t tl[2] = { sizeof lo4, sizeof lo6 };
		for (i = 0; i < 2; i++) {
			if (family == tf[1 - i]) continue;
			int s = socket(tf[i], SOCK_CLOEXEC | SOCK_DGRAM,
				IPPROTO_UDP);
			if (s >= 0) {
				int cs;
				pthread_setcancelstate(
					PTHREAD_CANCEL_DISABLE, &cs);
				int r = connect(s, ta[i], tl[i]);
				int saved_errno = errno;
				pthread_setcancelstate(cs, 0);
				close(s);
				if (!r) continue;
				errno = saved_errno;
			}
			switch (errno) {
			case EADDRNOTAVAIL:
			case EAFNOSUPPORT:
			case EHOSTUNREACH:
			case ENETDOWN:
			case ENETUNREACH:
				break;
			default:
				return EAI_SYSTEM;
			}
			if (family == tf[i]) {
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: family mismatch: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
                return EAI_NONAME;
			}
			family = tf[1 - i];
		}
	}

	int timeStartRet = gettimeofday(&timeStart, NULL);
	nservs = __lookup_serv(ports, serv, proto, socktype, flags);
	if (nservs < 0) return nservs;

	naddrs = lookup_name_ext(addrs, canon, host, family, flags, netid);
	int timeEndRet = gettimeofday(&timeEnd, NULL);
	int t_cost = 0;
	if (timeStartRet == 0 && timeEndRet == 0) {
		t_cost = COST_FOR_NANOSEC * (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_usec - timeStart.tv_usec);
		t_cost /= COST_FOR_MS;
	}
	if (naddrs < 0) {
		reportdnsresult(netid, host, t_cost, naddrs, NULL, param);
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d: reportdnsresult: %{public}d in process %{public}d",
			__func__, __LINE__, naddrs, getpid());
#endif
		return naddrs;
	}

	nais = nservs * naddrs;
	canon_len = strlen(canon);
	out = calloc(1, nais * sizeof(*out) + canon_len + 1);
	if (!out) return EAI_MEMORY;

	if (canon_len) {
		outcanon = (void *)&out[nais];
		memcpy(outcanon, canon, canon_len + 1);
	} else {
		outcanon = 0;
	}

	for (k = i = 0; i < naddrs; i++) for (j = 0; j < nservs; j++, k++) {
		out[k].slot = k;
		out[k].ai = (struct addrinfo) {
			.ai_family = addrs[i].family,
			.ai_socktype = ports[j].socktype,
			.ai_protocol = ports[j].proto,
			.ai_addrlen = addrs[i].family == AF_INET
				? sizeof(struct sockaddr_in)
				: sizeof(struct sockaddr_in6),
			.ai_addr = (void *)&out[k].sa,
			.ai_canonname = outcanon };
		if (k) out[k-1].ai.ai_next = &out[k].ai;
		switch (addrs[i].family) {
		case AF_INET:
			out[k].sa.sin.sin_family = AF_INET;
			out[k].sa.sin.sin_port = htons(ports[j].port);
			memcpy(&out[k].sa.sin.sin_addr, &addrs[i].addr, 4);
			break;
		case AF_INET6:
			out[k].sa.sin6.sin6_family = AF_INET6;
			out[k].sa.sin6.sin6_port = htons(ports[j].port);
			out[k].sa.sin6.sin6_scope_id = addrs[i].scopeid;
			memcpy(&out[k].sa.sin6.sin6_addr, &addrs[i].addr, 16);
			break;
		}
	}
	out[0].ref = nais;
	*res = &out->ai;

	reportdnsresult(netid, host, t_cost, DNS_QUERY_SUCCESS, *res, param);
	int cnt = predefined_host_is_contain_host(host);
#if OHOS_DNS_PROXY_BY_NETSYS
	if (type == QEURY_TYPE_NORMAL && cnt == 0) {
		dns_set_addr_info_to_netsys_cache2(netid, host, serv, hint, *res);
	}
#endif
	return 0;
}
