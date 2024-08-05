#include <sys/reboot.h>
#include "syscall.h"

int __reboot(int type)
{
	return syscall(SYS_reboot, RB_MAGIC1, RB_MAGIC2, type);
}
weak_alias(__reboot, reboot);
