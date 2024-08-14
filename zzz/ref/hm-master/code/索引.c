#include "los_config.h"
#include "gic_common.h"
#include "los_arch_mmu.h"
#include "los_atomic.h"
#include "los_init_pri.h"
#include "los_printf.h"
#include "los_process_pri.h"
#include "los_sched_pri.h"
#include "los_swtmr_pri.h"
#include "los_task_pri.h"

extern INT32 OsMain(VOID);
extern VOID  OsSchedStart(VOID);

//核间中断==================================================================
extern VOID OsMpWakeHandler(VOID)     ;//     唤醒CPU
extern VOID OsMpScheduleHandler(VOID)  ;//    调度CPU    
extern VOID OsMpScheduleHandler(VOID)   ;//    停止CPU   让当前Cpu停止工作
//系统滴答中断
extern VOID OsTickHandler(VOID)        ;//    系统滴答中断
        extern INLINE BOOL OsSchedScanTimerList(VOID);
        extern VOID OsSwtmrScan(VOID);

//内核初始化顺序===============================================================

extern UINT32 EarliestInit(VOID);
#define LOS_INIT_LEVEL_EARLIEST          0	//最早级
        extern UINT32 OsTraceInit(void);
        extern UINT32 OsLkLoggerInit(VOID);
        extern UINT32 OsDmesgInit(VOID);      //开机信息

extern UINT32 ArchEarlyInit(VOID);         //中断相关
        extern VOID OsHwiInit(VOID);
        extern VOID OsExcInit(VOID);  //异常接管
        extern UINT32 OsTickInit(UINT32 systemClock, UINT32 tickPerSecond);  //内核定时器  系统滴答
#define LOS_INIT_LEVEL_ARCH_EARLY        1	//架构早期

extern UINT32 PlatformEarlyInit(VOID); //串口中断
        extern void uart_init(void);
#define LOS_INIT_LEVEL_PLATFORM_EARLY    2	//平台早期

extern VOID OsSystemInfo(VOID); //cpu软件和硬件信息
extern UINT32 OsTaskInit(VOID); //任务池 调度器 cpu私有变量
#define LOS_INIT_LEVEL_KMOD_PREVM        3	//内存模块即将开始

extern UINT32 OsSysMemInit(VOID);//内存初始化
         extern  VOID OsKSpaceInit(VOID);
         extern STATUS_T OsKHeapInit(size_t size);
         extern VOID OsVmPageStartup(VOID);
         extern VOID OsInitMappingStartUp(VOID);
#define LOS_INIT_LEVEL_VM_COMPLETE       4	//内存模块完成
        extern UINT32 ShmInit(VOID);

extern UINT32 OsIpcInit(VOID);   //内核对象  消息队列  信号量
extern UINT32 OsSystemProcessCreate(VOID);//进程池
extern UINT32 ArchInit(VOID);
#define LOS_INIT_LEVEL_ARCH              5	//架构级
        extern int OsBBoxDriverInit(void);

extern UINT32 PlatformInit(VOID);
#define LOS_INIT_LEVEL_PLATFORM          6	//平台级
        extern int OsBBoxSystemAdapterInit(void);

extern UINT32 KModInit(VOID);
    extern UINT32 OsSwtmrInit(VOID);
#define LOS_INIT_LEVEL_KMOD_BASIC        7	//基础级
        extern VOID los_vfs_init(VOID);   //虚拟文件系统  根文件系统

#define LOS_INIT_LEVEL_KMOD_EXTENDED     8	//扩展级
        extern void ProcFsInit(void);
               extern int mkdir(const char );
        extern UINT32 OsFutexInit(VOID);
        extern int OsHiLogDriverInit(VOID);
        extern int OsHiDumperDriverInit(void);
        extern UINT32 OsCpupInit(VOID);
        extern UINT32 OsLiteIpcInit(VOID);
        extern UINT32 OsPmInit(VOID);
        extern UINT32 OsVdsoInit(VOID);
        extern VOID OsSyscallHandleInit(VOID);

extern VOID release_secondary_cores(VOID);
#define LOS_INIT_LEVEL_KMOD_TASK          9	 //任务级
        extern UINT32 OsResourceFreeTaskCreate(VOID);
        extern UINT32 OsMpInit(VOID);
        extern UINT32 OomTaskInit(VOID);
        extern UINT32 OsCpupGuardCreator(VOID);
        extern UINT32 OsSystemInit(VOID);
             extern VOID SystemInit(VOID);                 // qume  模拟运行
                    extern int mem_dev_register(void);
                    extern int DevMmzRegister(void);
                    extern void net_init(void);
                    extern int DeviceManagerStart(void);   //hdf  驱动框架
                           extern struct HdfObject *DevmgrServiceCreate(void);  //创建 DevmgrService 结构体
                                                            // 创建 HdfVNodeAdapter 结构体  注册驱动    发布服务
                            extern  struct HdfIoService *HdfIoServiceAdapterPublish(const char *serviceName, uint32_t mode); 
                                      extern int SysMkdir(const char *pathname, mode_t mode);
                            // ||||||||||||------------第二部分-------------
                            extern int DevmgrServiceStartService(struct IDevmgrService *inst);  //启动服务
                            extern static int DevmgrServiceStartDeviceHosts(struct DevmgrService *inst); //启动服务
                            
                            extern struct IDriverInstaller *DriverInstallerGetInstance(void);
                                  extern struct HdfObject *DriverInstallerCreate(void);  // 创建 DriverInstaller 驱动安装
                            
                            extern bool HdfAttributeManagerGetHostList(struct HdfSList *hostList); // 找到管理节点
                                      extern const struct DeviceResourceNode *HcsGetRootNode(void);  //找设备树 
                                           extern static bool CreateHcsToTree(void);   // 如果没有设备树  转化设备树
                                              extern void HdfGetBuildInConfigData(const unsigned char **data, unsigned int *size);
                        // ----------第三部分---------------按优先级创建了 host链表, 现在要启动 host 里的设备
                        static int DriverInstallerStartDeviceHost(uint32_t devHostId, const char *devHostName);
                              extern struct HdfObject *DevHostServiceCreate(); //创建函数
                              extern static int DevHostServiceStartService(struct IDevHostService *service); // 启动服务
                              extern static int DevmgrServiceAttachDeviceHost( struct IDevmgrService *inst, uint16_t hostId, struct IDevHostService *hostService);
                              // while(1)
                              extern int DevHostServiceClntInstallDriver(struct DevHostServiceClnt *hostClnt);
                                  extern int DevHostServiceAddDevice(struct IDevHostService *inst, const struct HdfDeviceInfo *deviceInfo);
                                         extern struct HdfDeviceNode *HdfDriverLoaderLoadNode(struct IDriverLoader *loader, const struct HdfDeviceInfo *deviceInfo);
                                               extern struct HdfDriverEntry *HdfDriverLoaderGetDriverEntry(const struct HdfDeviceInfo *deviceInfo);                             
                                extern static int HdfDeviceAttach(struct IHdfDevice *devInst, struct HdfDeviceNode *devNode);
                       //---------第四部分---------------
                       extern  int HdfDeviceLaunchNode(struct HdfDeviceNode *devNode, struct IHdfDevice *devInst);
                             extern  static int HdfDeviceNodePublishService(struct HdfDeviceNode *devNode, const struct HdfDeviceInfo *deviceInfo, struct IHdfDevice *device);
                                     extern  static int DeviceNodeExtPublishService(struct HdfDeviceNode *inst, const char *serviceName);
                                          extern int DevSvcManagerAddService(struct IDevSvcManager *manager, const char *svcName, struct HdfDeviceObject *service);

                            static int HdfDeviceNodePublishLocalService(struct HdfDeviceNode *devNode, const struct HdfDeviceInfo *deviceInfo);

                             extern int DevmgrServiceClntAttachDevice(const struct HdfDeviceInfo *deviceInfo, struct IHdfDeviceToken *deviceToken);
                                   extern static int DevmgrServiceAttachDevice( struct IDevmgrService *inst, const struct HdfDeviceInfo *deviceInfo, struct IHdfDeviceToken *token);
                        // 内核接口+++++第五部分++++++++++++++++++++++++++++++++++++++
                        HDF_INIT(g_sampleDriverEntry); //驱动入口 注册
                        extern struct HdfIoService *HdfIoServiceBind(const char *serviceName);
                        extern struct HdfDeviceObject *DevSvcManagerGetObject(struct IDevSvcManager *inst, const char *svcName);

                        extern static struct HdfIoServiceKClient *HdfHdfIoServiceKClientInstance(struct HdfDeviceObject *deviceObject);

                        extern int HdfKIoServiceDispatch (struct HdfObject *service, int cmdId, struct HdfSBuf *data, struct HdfSBuf *reply);                 
                        
                        // 内核接口+++++第五部分++++++++++++++++++++++++++++++++++++++
                        extern struct HdfIoService *HdfIoServiceBind(const char *serviceName);
                        //  这个用户态函数
                        extern struct HdfIoService *HdfIoServiceAdapterObtain(const char *serviceName);
                                //系统调用 open 
                                extern int HdfVNodeAdapterOpen(struct OsalCdev *cdev, struct file *filep);
                
                        extern static int32_t HdfSyscallAdapterDispatch(struct HdfObject *object, int32_t code,  struct HdfSBuf *data, struct HdfSBuf *reply);           
                                //系统调用 ioctl
                                extern static long HdfVNodeAdapterIoctl(struct file *filep,  unsigned int cmd, unsigned long arg);
                                
                 //++++++++++++++++++++++++++++++++++++++++++++++++++++++

                    extern INT32 LOS_GetCmdLine();
                    extern INT32 LOS_ParseBootargs();
                    extern INT32 OsMountRootfs(VOID);       //挂载根文件系统
 
                        extern int VnodeLookup(const char *path, struct Vnode **vnode, uint32_t flags);
                     
                         extern INT32 add_mtd_partition(const CHAR *type, UINT32 startAddr, UINT32 length, UINT32 partitionNum) ;
                               extern static INT32 BlockDriverRegisterOperate(mtd_partition *newNode, const partition_param *param, UINT32 partitionNum);
                                         extern int register_blockdriver(const char *path,const struct block_operations *bops,mode_t mode, void *priv);
                                                FSMAP_ENTRY(jffs_fsmap, "jffs2", jffs_operations, TRUE, TRUE); //文件系统
                                         extern int VfsJffs2Bind(struct Mount *mnt, struct Vnode *blkDriver, const void *data);
                               extern static INT32 CharDriverRegisterOperate(mtd_partition *newNode,const partition_param *param, UINT32 partitionNum);
                                          extern int register_driver(const char *path,const struct file_operations_vfs *fops, mode_t mode,void *priv);
                        
                        extern STATIC INT32 MountPartitions(CHAR *fsType, UINT32 mountFlags);
                               extern int mount(const char *source, const char *target,const char *filesystemtype, unsigned long mountflags,const void *data);
                                      extern int SysMount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags,const void *data);
                    
                    
                    extern INT32 virtual_serial_init(const CHAR *deviceName);
                    extern system_console_init(const CHAR *deviceName);
                    extern UINT32 OsUserInitProcess(VOID); //启动第一个应用程序 
                                  extern VOID OsUserInit(VOID *args);  //根据 ld 地址地址 拷贝后启动 
                                        
                                        //触发软中断
                                        _osExceptSwiHdl(); //  R7 保持系统调用号 = __NR_execve       R0 R1 R2 依次保持后面三个参数
                                        extern VOID OsArmA32SyscallHandle(TaskContext *regs) ;
                                        extern int SysExecve(regs->R0, regs->R1, regs->R2);
                                        extern INT32 LOS_DoExecveFile(const CHAR *fileName, CHAR * const *argv, CHAR * const *envp)

                                           extern STATIC CHAR *g_initPath = "/bin/init" //由Init_lite在编译后,生成
#define LOS_INIT_LEVEL_FINISH             10



extern  UINT32 sys_call3(UINT32 nbr, UINT32 parm1, UINT32 parm2, UINT32 parm3)  //系统调用

extern int main(int argc, char * const *argv);  //       /bin/init  源码
       extern  pid_t fork(void);
             extern static inline long __syscall0(long n);  // 在  /musl/arch/arm 分类中
