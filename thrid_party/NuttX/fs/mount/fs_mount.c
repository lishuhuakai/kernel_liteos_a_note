/****************************************************************************
 * fs/mount/fs_mount.c
 *
 * Copyright (c) 2023 Huawei Device Co., Ltd. All rights reserved.
 * Based on NuttX originally from nuttx source (nuttx/fs/ and nuttx/drivers/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "vfs_config.h"

#include "sys/mount.h"
#include "string.h"
#include "errno.h"
#include "assert.h"
#include "vnode.h"
#include "stdlib.h"
#ifdef LOSCFG_DRIVERS_MTD
#include "mtd_partition.h"
#endif
#ifdef LOSCFG_FS_FAT_VIRTUAL_PARTITION
#include "errcode_fat.h"
#endif
#include "los_tables.h"
#ifdef LOSCFG_DRIVERS_RANDOM
#include "hisoc/random.h"
#else
#include "stdlib.h"
#endif
#include "path_cache.h"
#include "fs/mount.h"
#include "fs/driver.h"
#include "fs/fs.h"
#ifdef LOSCFG_FS_ZPFS
#include "zpfs.h"
#endif
#ifdef LOSCFG_MNT_CONTAINER
#include "los_mnt_container_pri.h"
#endif

/* At least one filesystem must be defined, or this file will not compile.
 * It may be desire-able to make filesystems dynamically registered at
 * some time in the future, but at present, this file needs to know about
 * every configured filesystem.
 */

#ifdef CONFIG_FS_READABLE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* In the canonical case, a file system is bound to a block driver.  However,
 * some less typical cases a block driver is not required.  Examples are
 * pseudo file systems (like BINFS or PROCFS) and MTD file systems (like NXFFS).
 *
 * These file systems all require block drivers:
 */

#define BLKDRVR_NOT_MOUNTED 2

extern struct fsmap_t g_fsmap[];
LOS_HAL_TABLE_BEGIN(g_fsmap, fsmap);

extern struct fsmap_t g_fsmap_end;
LOS_HAL_TABLE_END(g_fsmap_end, fsmap);
/* 内核通过FSMAP_ENTRY来注册静态表项,比如procfs的注册如下:
 * FSMAP_ENTRY(procfs_fsmap, "procfs", procfs_operations, FALSE, FALSE);
 */

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mount_findfs
 *
 * Description:
 *    find the specified filesystem
 *
 ****************************************************************************/

static const struct fsmap_t *mount_findfs(const char *filesystemtype)
{
    struct fsmap_t *m = NULL;

    for (m = &g_fsmap[0]; m != &g_fsmap_end; ++m) {
        if (m->fs_filesystemtype &&
            strcmp(filesystemtype, m->fs_filesystemtype) == 0) {
            return m;
        }
    }

    return (const struct fsmap_t *)NULL;
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mount
 *
 * Description:
 *   mount() attaches the filesystem specified by the 'source' block device
 *   name into the root file system at the path specified by 'target.'
 *
 * Return:
 *   Zero is returned on success; -1 is returned on an error and errno is
 *   set appropriately:
 *
 *   EACCES A component of a path was not searchable or mounting a read-only
 *      filesystem was attempted without giving the MS_RDONLY flag.
 *   EBUSY 'source' is already  mounted.
 *   EFAULT One of the pointer arguments points outside the user address
 *      space.
 *   EINVAL 'source' had an invalid superblock.
 *   ENODEV 'filesystemtype' not configured
 *   ENOENT A pathname was empty or had a nonexistent component.
 *   ENOMEM Could not allocate a memory to copy filenames or data into.
 *   ENOTBLK 'source' is not a block device
 *
 ****************************************************************************/

int mount(const char *source, const char *target,
          const char *filesystemtype, unsigned long mountflags,
          const void *data)
{
    int ret;
    int errcode = 0;
    struct Mount* mnt = NULL;
    struct Vnode *device = NULL;
    struct Vnode *mountpt_vnode = NULL;
    const struct fsmap_t *fsmap = NULL;
    const struct MountOps *mops = NULL;
    LIST_HEAD *mount_list = NULL;
#ifdef LOSCFG_DRIVERS_MTD
    mtd_partition *partition = NULL;
#endif

    if (filesystemtype == NULL) {
        errcode = -EINVAL;
        goto errout;
    }

    /* Verify required pointer arguments */

    DEBUGASSERT(target && filesystemtype);

    /* Find the specified filesystem.  Try the block driver file systems first */
    // 找到指定的文件系统,先尝试块驱动文件系统
    if ((fsmap = mount_findfs(filesystemtype)) == NULL || (fsmap->is_bdfs && !source)) {
        PRINT_ERR("Failed to find file system %s\n", filesystemtype);
        errcode = -ENODEV;
        goto errout;
    }

    mops = fsmap->fs_mops;

    if (fsmap->is_bdfs && source) { // 如果是块设备文件
        /* Make sure that a block driver argument was provided */

        DEBUGASSERT(source);

        /* Find the block driver */

        ret = find_blockdriver(source, mountflags, &device); // 查找对应的驱动
        if (ret < 0) {
            PRINT_ERR("Failed to find block driver %s\n", source);
            errcode = ret;
            goto errout;
        }
    }

    VnodeHold();
    ret = VnodeLookup(target, &mountpt_vnode, 0); // 查找对应Vnode,挂载点必须要存在

    /* The mount point must be an existed vnode. */

    if (ret != OK) {
        PRINT_ERR("Failed to find valid mountpoint %s\n", target);
        errcode = -EINVAL;
        goto errout_with_lock;
    }
    if (mountpt_vnode->flag & VNODE_FLAG_MOUNT_NEW) {
#ifdef LOSCFG_MNT_CONTAINER
        struct Mount *tMnt = NULL;
        LOS_DL_LIST_FOR_EACH_ENTRY(tMnt, GetMountList(), struct Mount, mountList) {
            if (tMnt->vnodeCovered == mountpt_vnode) {
                PRINT_ERR("can't mount to %s, already mounted.\n", target);
                errcode = -EINVAL;
                goto errout_with_lock;
            }
        }
#else
        PRINT_ERR("can't mount to %s, already mounted.\n", target); // 不能重复挂载
        errcode = -EINVAL;
        goto errout_with_lock;
#endif
    }

#ifdef LOSCFG_MNT_CONTAINER
    struct Mount *cacheMnt = NULL;
    if (source != NULL) {
        LOS_DL_LIST_FOR_EACH_ENTRY(cacheMnt, GetMountCache(), struct Mount, mountList) {
            if (strcmp(cacheMnt->devName, source) == 0) {
                struct Mount *newMnt = (struct Mount *)zalloc(sizeof(struct Mount));
                if (newMnt == NULL) {
                    PRINT_ERR("New mount alloc failed no memory!\n");
                    errcode = -EINVAL;
                    goto errout;
                }
                *newMnt = *cacheMnt;
                LOS_ListTailInsert(GetMountList(), &(newMnt->mountList));
                cacheMnt->vnodeCovered->mntCount++;
                VnodeDrop();
                return OK;
            }
        }
    }
#endif
    /* Bind the block driver to an instance of the file system.  The file
     * system returns a reference to some opaque, fs-dependent structure
     * that encapsulates this binding.
     */

    if (mops->Mount == NULL) { // 挂载回调
        /* The filesystem does not support the bind operation ??? */

        PRINTK("ERROR: Filesystem does not support bind\n");
        errcode = -ENOSYS;
        goto errout_with_lock;
    }

    /* Increment reference count for the reference we pass to the file system */
#ifdef LOSCFG_DRIVERS_MTD
    if (fsmap->is_mtd_support && (device != NULL)) {
        partition = (mtd_partition *)((struct drv_data *)device->data)->priv;
        partition->mountpoint_name = (char *)zalloc(strlen(target) + 1);
        if (partition->mountpoint_name == NULL) {
            errcode = -ENOMEM;
            goto errout_with_lock;
        }
        (void)strncpy_s(partition->mountpoint_name, strlen(target) + 1, target, strlen(target));
        partition->mountpoint_name[strlen(target)] = '\0';
    }
#endif

    mnt = MountAlloc(mountpt_vnode, (struct MountOps*)mops);

#ifdef LOSCFG_FS_ZPFS
    if (strcmp(filesystemtype, ZPFS_NAME) == 0) {
        ret = ZpfsPrepare(source, target, mnt);
        if (ret < 0) {
            errcode = ret;
            goto errout_with_mountpt;
        }
    }
#endif

    mnt->mountFlags = mountflags;

    mountpt_vnode->useCount++;
    ret = mops->Mount(mnt, device, data);
    mountpt_vnode->useCount--;
    if (ret != 0) {
        /* The vnode is unhappy with the blkdrvr for some reason.  Back out
         * the count for the reference we failed to pass and exit with an
         * error.
         */

        PRINT_ERR("Bind method failed: %d\n", ret);
        errcode = ret;
#ifdef LOSCFG_DRIVERS_MTD
        if (fsmap->is_mtd_support && (device != NULL) && (partition != NULL)) {
            free(partition->mountpoint_name);
            partition->mountpoint_name = NULL;
        }
#endif
        goto errout_with_mountpt;
    }
    mnt->vnodeBeCovered->flag |= VNODE_FLAG_MOUNT_ORIGIN;
    mnt->vnodeCovered->flag |= VNODE_FLAG_MOUNT_NEW; // 表示已经被挂载了
    mnt->vnodeCovered->filePath = strdup(mountpt_vnode->filePath);
    mnt->vnodeDev = device;
    mnt->ops = mops;
    if (target && (strlen(target) != 0)) {
        ret = strcpy_s(mnt->pathName, PATH_MAX, target);
        if (ret != EOK) {
            PRINT_ERR("Failed to copy mount point pathname, errno %d\n", ret);
        }
    }

    if (source && (strlen(source) != 0)) {
        ret = strcpy_s(mnt->devName, PATH_MAX, source);
        if (ret != EOK) {
            PRINT_ERR("Failed to copy dev name, errno %d\n", ret);
        }
    }

    //* We have it, now populate it with driver specific information. */

    mount_list = GetMountList();
    LOS_ListAdd(mount_list, &mnt->mountList);

#ifdef LOSCFG_MNT_CONTAINER
    if (source != NULL) {
        struct Mount *newMnt = (struct Mount *)zalloc(sizeof(struct Mount));
        if (newMnt == NULL) {
            PRINT_ERR("New mount alloc failed no memory!\n");
            errcode = -EINVAL;
            goto errout;
        }
        *newMnt = *mnt;
        LOS_ListTailInsert(GetMountCache(), &(newMnt->mountList));
    }
#endif

    if (!strcmp("/", target)) {
        ChangeRoot(mnt->vnodeCovered);
    }

    VnodeDrop();

    /* We can release our reference to the blkdrver_vnode, if the filesystem
     * wants to retain the blockdriver vnode (which it should), then it must
     * have called vnode_addref().  There is one reference on mountpt_vnode
     * that will persist until umount() is called.
     */

    return OK;

    /* A lot of goto's!  But they make the error handling much simpler */

errout_with_mountpt:
    if (mnt) {
        free(mnt);
    }
errout_with_lock:
    VnodeDrop();
errout:
    set_errno(-errcode);
    return VFS_ERROR;
}
#endif /* CONFIG_FS_READABLE */
