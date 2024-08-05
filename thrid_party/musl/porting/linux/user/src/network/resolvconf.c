/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "lookup.h"
#include "stdio_impl.h"
#include "hilog_adapter.h"
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

#if 1
#define SIGCHAIN_LOG_DOMAIN 0xD003F00
#define SIGCHAIN_LOG_TAG "ressolvconf"
#define GETADDRINFO_PRINT_DEBUG(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_INFO, \
	SIGCHAIN_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#else
#define GETADDRINFO_PRINT_DEBUG(...)
#endif

#define DNS_RESOLV_CONF_PATH "/etc/resolv.conf"

#if OHOS_DNS_PROXY_BY_NETSYS | OHOS_FWMARK_CLIENT_BY_NETSYS
#include "atomic.h"

#include <dlfcn.h>

static void *open_dns_lib(void)
{
	static void *dns_lib_handle = NULL;
	if (dns_lib_handle != NULL) {
		a_barrier();
		return dns_lib_handle;
	}

	void *lib = dlopen(DNS_SO_PATH, RTLD_LAZY);
	if (lib == NULL) {
		DNS_CONFIG_PRINT("%s: dlopen %s failed: %s",
			__func__, DNS_SO_PATH, dlerror());
		return NULL;
	}

	void *old_lib = a_cas_p(&dns_lib_handle, NULL, lib);
	if (old_lib == NULL) {
		DNS_CONFIG_PRINT("%s: %s loaded", __func__, DNS_SO_PATH);
		return lib;
	} else {
		/* Another thread has already loaded the library,
		 * dlclose is invoked to make refcount correct */
		DNS_CONFIG_PRINT("%s: %s has been loaded by another thread",
			__func__, DNS_SO_PATH);
		if (dlclose(lib)) {
			DNS_CONFIG_PRINT("%s: dlclose %s failed: %s",
				__func__, DNS_SO_PATH, dlerror());
		}
		return old_lib;
	}
}

static void *load_from_dns_lib(const char *symbol)
{
	void *lib_handle = open_dns_lib();
	if (lib_handle == NULL) {
		return NULL;
	}

	void *sym_addr = dlsym(lib_handle, symbol);
	if (sym_addr == NULL) {
		DNS_CONFIG_PRINT("%s: loading symbol %s with dlsym failed: %s",
			__func__, symbol, dlerror());
	}
	return sym_addr;
}

void resolve_dns_sym(void **holder, const char *symbol)
{
	if (*holder != NULL) {
		a_barrier();
		return;
	}

	void *ptr = load_from_dns_lib(symbol);
	if (ptr == NULL) {
		return;
	}

	void *old_ptr = a_cas_p(holder, NULL, ptr);
	if (old_ptr != NULL) {
		DNS_CONFIG_PRINT("%s: %s has been found by another thread",
			__func__, symbol);
	} else {
		DNS_CONFIG_PRINT("%s: %s found", __func__, symbol);
	}
}

static GetConfig load_config_getter(void)
{
	static GetConfig config_getter = NULL;
	resolve_dns_sym((void **) &config_getter, OHOS_GET_CONFIG_FUNC_NAME);
	return config_getter;
}

#endif

int __get_resolv_conf(struct resolvconf *conf, char *search, size_t search_sz)
{
       return get_resolv_conf_ext(conf, search, search_sz, 0);
}

int get_resolv_conf_ext(struct resolvconf *conf, char *search, size_t search_sz, int netid)
{
	char line[256];
	unsigned char _buf[256];
	FILE *f, _f;
	int nns = 0;

	conf->ndots = 1;
	conf->timeout = 5;
	conf->attempts = 2;
	if (search) *search = 0;

#if OHOS_DNS_PROXY_BY_NETSYS
	GetConfig func = load_config_getter();
	if (!func) {
		DNS_CONFIG_PRINT("%s: loading %s failed, use %s as a fallback",
			__func__, OHOS_GET_CONFIG_FUNC_NAME, DNS_RESOLV_CONF_PATH);
		goto etc_resolv_conf;
	}

	struct resolv_config config = {0};
	int ret = func(netid, &config);
	if (ret < 0) {
		DNS_CONFIG_PRINT("__get_resolv_conf OHOS_GET_CONFIG_FUNC_NAME err %d\n", ret);
		return EAI_NONAME;
	}
	int32_t timeout_second = config.timeout_ms / 1000;

netsys_conf:
	if (timeout_second > 0) {
		if (timeout_second >= 60) {
			conf->timeout = 60;
		} else {
			conf->timeout = timeout_second;
		}
	}
	if (config.retry_count > 0) {
		if (config.retry_count >= 10) {
			conf->attempts = 10;
		} else {
			conf->attempts = config.retry_count;
		}
	}
	for (int i = 0; i < MAX_SERVER_NUM; ++i) {
		if (config.nameservers[i] == NULL || config.nameservers[i][0] == 0 || nns >= MAXNS) {
			continue;
		}
		if (__lookup_ipliteral(conf->ns + nns, config.nameservers[i], AF_UNSPEC) > 0) {
			nns++;
		}
	}

        if (nns != 0) {
            goto get_conf_ok;
        }

etc_resolv_conf:
#endif
	f = __fopen_rb_ca(DNS_RESOLV_CONF_PATH, &_f, _buf, sizeof _buf);
	if (!f) switch (errno) {
	case ENOENT:
	case ENOTDIR:
	case EACCES:
		goto no_resolv_conf;
	default:
		return -1;
	}

	while (fgets(line, sizeof line, f)) {
		char *p, *z;
		if (!strchr(line, '\n') && !feof(f)) {
			/* Ignore lines that get truncated rather than
			 * potentially misinterpreting them. */
			int c;
			do c = getc(f);
			while (c != '\n' && c != EOF);
			continue;
		}
		if (!strncmp(line, "options", 7) && isspace(line[7])) {
			p = strstr(line, "ndots:");
			if (p && isdigit(p[6])) {
				p += 6;
				unsigned long x = strtoul(p, &z, 10);
				if (z != p) conf->ndots = x > 15 ? 15 : x;
			}
			p = strstr(line, "attempts:");
			if (p && isdigit(p[9])) {
				p += 9;
				unsigned long x = strtoul(p, &z, 10);
				if (z != p) conf->attempts = x > 10 ? 10 : x;
			}
			p = strstr(line, "timeout:");
			if (p && (isdigit(p[8]) || p[8]=='.')) {
				p += 8;
				unsigned long x = strtoul(p, &z, 10);
				if (z != p) conf->timeout = x > 60 ? 60 : x;
			}
			continue;
		}
		if (!strncmp(line, "nameserver", 10) && isspace(line[10])) {
			if (nns >= MAXNS) continue;
			for (p=line+11; isspace(*p); p++);
			for (z=p; *z && !isspace(*z); z++);
			*z=0;
			if (__lookup_ipliteral(conf->ns+nns, p, AF_UNSPEC) > 0)
				nns++;
			continue;
		}

		if (!search) continue;
		if ((strncmp(line, "domain", 6) && strncmp(line, "search", 6))
		    || !isspace(line[6]))
			continue;
		for (p=line+7; isspace(*p); p++);
		size_t l = strlen(p);
		/* This can never happen anyway with chosen buffer sizes. */
		if (l >= search_sz) continue;
		memcpy(search, p, l+1);
	}

	__fclose_ca(f);

no_resolv_conf:
	if (!nns) {
		__lookup_ipliteral(conf->ns, "127.0.0.1", AF_UNSPEC);
		nns = 1;
	}

get_conf_ok:
	conf->nns = nns;

	return 0;
}

int res_bind_socket(int fd, int netid)
{
	int ret = -1;
	void* libhandler;

	GETADDRINFO_PRINT_DEBUG("res_bind_socket netid:%{public}d \n", netid);

#ifdef OHOS_FWMARK_CLIENT_BY_NETSYS
	libhandler = dlopen(FWMARKCLIENT_SO_PATH, RTLD_LAZY);
	if (libhandler == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache dlopen err %s\n", dlerror());
		return -1;
	}

	BindSocket_Ext func = dlsym(libhandler, OHOS_BIND_SOCKET_FUNC_NAME);
	if (func == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache dlsym err %s\n", dlerror());
		dlclose(libhandler);
		return -1;
	} else {
		ret = func(fd, netid);
		GETADDRINFO_PRINT_DEBUG("res_bind_socket ret %{public}d \n", ret);
		dlclose(libhandler);
	}
#endif
	return ret;
}

