struct ipc_perm {
	key_t __ipc_perm_key; // 调用shmget()时给出的关键字
	uid_t uid; // 共享内存所有者的有效用户ID
	gid_t gid; // 共享内存所有者所属组的ID
	uid_t cuid; // 共享内存创建者的有效用户ID
	gid_t cgid; // 共享内存创建者的有效组ID
	mode_t mode; // 权限 + SHM_DEST / SHM_LOCKED / SHM_HUGETLB标志位
	int __ipc_perm_seq; // 序列号
	long __pad1;
	long __pad2;
};
