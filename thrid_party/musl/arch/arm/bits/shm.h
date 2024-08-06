#define SHMLBA 4096

struct shmid_ds {
	struct ipc_perm shm_perm; // 操作许可,里面包含共享内存的用户ID,组ID等信息
	size_t shm_segsz; // 共享内存段的大小,单位为字节
	unsigned long __shm_atime_lo; // 最后一个进程访问共享内存的时间
	unsigned long __shm_atime_hi;
	unsigned long __shm_dtime_lo; // 最后一个进程离开共享内存的时间
	unsigned long __shm_dtime_hi;
	unsigned long __shm_ctime_lo; // 创建时间
	unsigned long __shm_ctime_hi;
	pid_t shm_cpid; // 创建共享内存的进程id
	pid_t shm_lpid; // 最后操作共享内存的进程id
	unsigned long shm_nattch; // 当前使用该共享内存段的进程的数量
	unsigned long __pad1;
	unsigned long __pad2;
	unsigned long __pad3;
	time_t shm_atime;
	time_t shm_dtime;
	time_t shm_ctime;
};

struct shminfo {
	unsigned long shmmax, shmmin, shmmni, shmseg, shmall, __unused1[4];
};

struct shm_info {
	int __used_ids;
	unsigned long shm_tot, shm_rss, shm_swp;
	unsigned long __swap_attempts, __swap_successes;
};
