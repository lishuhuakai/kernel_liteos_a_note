#include <netdb.h>
#ifdef __LITEOS_A__
#include <string.h>
#endif

struct netent *getnetbyaddr(uint32_t net, int addrtype)
{
#ifdef __LITEOS_A__
	struct netent *ne = NULL;
	setnetent(1);
	while (1) {
		ne = getnetent();
		if (!ne)
			break;
		if (ne->n_net == net && ne->n_addrtype == addrtype) {
			setnetent(0);
			endnetent();
			return ne;
		}
	}
	setnetent(0);
	endnetent();
	return NULL;
#else
	return 0;
#endif
}

struct netent *getnetbyname(const char *netname)
{
#ifdef __LITEOS_A__
	struct netent *ne = NULL;
	setnetent(1);
	while (1) {
		ne = getnetent();
		if (!ne)
			break;
		if (strcmp(ne->n_name, netname) == 0) {
			setnetent(0);
			endnetent();
			return ne;
		}
	}
	setnetent(0);
	endnetent();
	return NULL;
#else
	return 0;
#endif
}
