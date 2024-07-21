/****************************************************************************
 * fs/vfs/fs_mkdir.c
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
#include "errno.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "stdlib.h"
#include "vnode.h"
#include "string.h"
#include "capability_api.h"
#include "path_cache.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
int do_mkdir(int dirfd, const char *pathname, mode_t mode)
{
    struct Vnode *parentVnode = NULL;
    struct Vnode *vnode = NULL;
    struct Mount *mount = NULL;
    int ret;
    char *fullpath = NULL;
    char *relativepath = NULL;
    char *dirname = NULL;

    mode &= ~GetUmask();
    mode &= (S_IRWXU|S_IRWXG|S_IRWXO);

    /* Get absolute path by dirfd*/
    ret = get_path_from_fd(dirfd, &relativepath);
    if (ret < 0) {
        goto errout;
    }

    ret = vfs_normalize_path((const char *)relativepath, pathname, &fullpath);
    if (relativepath) {
        free(relativepath);
    }

    if (ret < 0) {
        goto errout;
    }
#ifdef LOSCFG_KERNEL_PLIMITS
    if (!strncmp(fullpath, "/dev", 4) || !strcmp(fullpath, "/proc"))
#else
    if (!strncmp(fullpath, "/dev", 4) || !strncmp(fullpath, "/proc", 5))
#endif
    {
        // virtual root create virtual dir
        VnodeHold();
        ret = VnodeLookup(fullpath, &vnode, V_DUMMY|V_CREATE);
        if (ret != OK) {
            goto errout_with_lock;
        }
        vnode->mode = mode | S_IFDIR;
        vnode->type = VNODE_TYPE_DIR;
        VnodeDrop();
        goto out;
    }

    dirname = strrchr(fullpath, '/') + 1;

    VnodeHold();
    ret = VnodeLookup(fullpath, &parentVnode, 0);
    if (ret == OK) {
        ret = -EEXIST;
        goto errout_with_lock;
    }

    if (parentVnode == NULL) {
        ret = -ENOENT;
        goto errout_with_lock;
    }
    mount = parentVnode->originMount;
    if ((mount != NULL) && (mount->mountFlags & MS_RDONLY)) {
        ret = -EROFS;
        goto errout_with_lock;
    }

    parentVnode->useCount++;

    if (VfsVnodePermissionCheck(parentVnode, (WRITE_OP | EXEC_OP))) {
        ret = -EACCES;
        goto errout_with_count;
    }

    if ((parentVnode->vop != NULL) && (parentVnode->vop->Mkdir != NULL)) {
        ret = parentVnode->vop->Mkdir(parentVnode, dirname, mode, &vnode);
    } else {
        ret = -ENOSYS;
    }

    if (ret < 0) {
        goto errout_with_count;
    }

    struct PathCache *dt = PathCacheAlloc(parentVnode, vnode, dirname, strlen(dirname));
    if (dt == NULL) {
        // alloc name cache failed is not a critical problem, let it go.
        PRINT_ERR("alloc path cache %s failed\n", dirname);
    }
    vnode->filePath = strdup(fullpath);
    parentVnode->useCount--;
    VnodeDrop();
out:
    /* Directory successfully created */
    free(fullpath);

    return OK;
errout_with_count:
    parentVnode->useCount--;
errout_with_lock:
    VnodeDrop();
errout:
    set_errno(-ret);
    if (fullpath) {
        free(fullpath);
    }
    return VFS_ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mkdir
 *
 * Description:  Create a directory
 *
 ****************************************************************************/

int mkdir(const char *pathname, mode_t mode)
{
    return do_mkdir(AT_FDCWD, pathname, mode);
}

/****************************************************************************
 * Name: mkdirat
 *
 * Description:  Create a directory by dirfd
 *
 ****************************************************************************/

int mkdirat(int dirfd, const char *pathname, mode_t mode)
{
    return do_mkdir(dirfd, pathname, mode);
}
