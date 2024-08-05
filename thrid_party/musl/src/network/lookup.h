#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdint.h>
#include <stddef.h>
#include <features.h>
#include <netinet/in.h>
#include <netdb.h>
#ifndef __LITEOS__
#include <musl_log.h>
#endif

#if OHOS_DNS_PROXY_BY_NETSYS

#include <stdio.h>

#if DNS_CONFIG_DEBUG
#ifndef DNS_CONFIG_PRINT
#define DNS_CONFIG_PRINT(fmt, ...) printf("DNS " fmt "\n", ##__VA_ARGS__)
#endif
#else
#define DNS_CONFIG_PRINT(fmt, ...)
#endif
#endif

struct aibuf {
	struct addrinfo ai;
	union sa {
		struct sockaddr_in sin;
		struct sockaddr_in6 sin6;
	} sa;
	volatile int lock[1];
	short slot, ref;
};

struct address {
	int family;
	unsigned scopeid;
	uint8_t addr[16];
	int sortkey;
};

struct service {
	uint16_t port;
	unsigned char proto, socktype;
};

#define MAXNS 3

struct resolvconf {
	struct address ns[MAXNS];
	unsigned nns, attempts, ndots;
	unsigned timeout;
};

/* The limit of 48 results is a non-sharp bound on the number of addresses
 * that can fit in one 512-byte DNS packet full of v4 results and a second
 * packet full of v6 results. Due to headers, the actual limit is lower. */
#define MAXADDRS 48
#define MAXSERVS 2

hidden int __lookup_serv(struct service buf[static MAXSERVS], const char *name, int proto, int socktype, int flags);
hidden int lookup_name_ext(struct address buf[static MAXADDRS], char canon[static 256], const char *name,
							 int family, int flags, int netid);
hidden int __lookup_name(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family, int flags);
hidden int __lookup_ipliteral(struct address buf[static 1], const char *name, int family);

hidden int __get_resolv_conf(struct resolvconf *, char *, size_t);
hidden int get_resolv_conf_ext(struct resolvconf *, char *, size_t, int netid);
hidden int __res_msend_rc(int, const unsigned char *const *, const int *, unsigned char *const *, int *, int, const struct resolvconf *);
hidden int res_msend_rc_ext(int, int, const unsigned char *const *, const int *, unsigned char *const *,
							int *, int, const struct resolvconf *);

hidden int __dns_parse(const unsigned char *, int, int (*)(void *, int, const void *, int, const void *, int), void *);
hidden int predefined_host_name_from_hosts(struct address buf[static MAXADDRS],
	char canon[static 256], const char *name, int family);
hidden int predefined_host_is_contain_host(const char *host);
hidden int predefined_host_lookup_ip(const char* host, const char* serv,
    const struct addrinfo* hint, struct addrinfo** res);
hidden int res_bind_socket(int, int);

#if OHOS_DNS_PROXY_BY_NETSYS
#define DNS_SO_PATH "libnetsys_client.z.so"
#define MAX_SERVER_NUM 4
#define MAX_SERVER_LENGTH 50
#define OHOS_GET_CONFIG_FUNC_NAME "NetSysGetResolvConf"
#define OHOS_GET_CACHE_FUNC_NAME "NetSysGetResolvCache"
#define OHOS_SET_CACHE_FUNC_NAME "NetSysSetResolvCache"
#define OHOS_JUDGE_IPV6_FUNC_NAME "NetSysIsIpv6Enable"
#define OHOS_POST_DNS_RESULT_FUNC_NAME "NetSysPostDnsResult"
#define OHOS_GET_DEFAULT_NET_FUNC_NAME "NetSysGetDefaultNetwork"
#define MAX_RESULTS 32
#define MAX_CANON_NAME 256
#define MACRO_MIN(a, b) ((a) < (b) ? (a) : (b))

struct resolv_config {
	int32_t error;
	int32_t timeout_ms;
	uint32_t retry_count;
	char nameservers[MAX_SERVER_NUM][MAX_SERVER_LENGTH + 1];
};

typedef union {
	struct sockaddr sa;
	struct sockaddr_in6 sin6;
	struct sockaddr_in sin;
} aligned_sockAddr;

struct addr_info_wrapper {
	uint32_t ai_flags;
	uint32_t ai_family;
	uint32_t ai_sockType;
	uint32_t ai_protocol;
	uint32_t ai_addrLen;
	aligned_sockAddr ai_addr;
	char ai_canonName[MAX_CANON_NAME + 1];
};

struct param_wrapper {
	char *host;
	char *serv;
	struct addrinfo *hint;
};

typedef int32_t (*GetConfig)(uint16_t netId, struct resolv_config *config);

typedef int32_t (*GetCache)(uint16_t netId, struct param_wrapper param,
							struct addr_info_wrapper addr_info[static MAX_RESULTS],
							uint32_t *num);

typedef int32_t (*SetCache)(uint16_t netId, struct param_wrapper param, struct addrinfo *res);

typedef int (*JudgeIpv6)(uint16_t netId);

typedef int (*PostDnsResult)(int netid, char* name, int usedtime, int queryfail,
							 struct addrinfo *res, struct queryparam *param);

typedef int (*GetDefaultNet)(uint16_t netId, int32_t *currentnetid);

/* If the memory holder points to stores NULL value, try to load symbol from the
 * dns lib into holder; otherwise, it does nothing. */
hidden void resolve_dns_sym(void **holder, const char *symbol);

void
dns_set_addr_info_to_netsys_cache(const char *__restrict host, const char *__restrict serv,
								  const struct addrinfo *__restrict
								  hint, struct addrinfo *res);

void dns_set_addr_info_to_netsys_cache2(const int netid, const char *__restrict host, const char *__restrict serv,
										const struct addrinfo *__restrict hint, struct addrinfo *res);

int dns_get_addr_info_from_netsys_cache(const char *__restrict host, const char *__restrict serv,
										const struct addrinfo *__restrict hint, struct addrinfo **__restrict res);

int dns_get_addr_info_from_netsys_cache2(const int netid, const char *__restrict host, const char *__restrict serv,
										 const struct addrinfo *__restrict hint, struct addrinfo **__restrict res);

int dns_post_result_to_netsys_cache(int netid, char* name, int usedtime, int querypass,
									struct addrinfo *res, struct queryparam *param);

int dns_get_default_network(int *currentnetid);
#endif

#if OHOS_FWMARK_CLIENT_BY_NETSYS
#define FWMARKCLIENT_SO_PATH "libfwmark_client.z.so"
#define OHOS_BIND_SOCKET_FUNC_NAME "BindSocket"

typedef int32_t (*BindSocket)(int32_t fd, uint32_t netId);

#define OHOS_NETSYS_BIND_SOCKET_FUNC_NAME "NetSysBindSocket"

typedef int32_t (*BindSocket_Ext)(int32_t fd, uint32_t netId);

#endif
#endif
