/****************************************************************************
 * fs/vfs/fs_rmdir.c
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

#include "unistd.h"
#include "errno.h"
#include "stdlib.h"
#include "vnode.h"
#include "sys/stat.h"
#include "string.h"
#include "limits.h"
#include "fs/mount.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int check_target(struct Vnode *vnode, char *name)
{
    if (vnode == NULL) {
        return -ENOENT;
    }

    if ((vnode->originMount) && (vnode->originMount->mountFlags & MS_RDONLY)) {
        return -EROFS;
    }

    if (vnode->type != VNODE_TYPE_DIR) {
        return -ENOTDIR;
    }

    if (vnode->useCount > 0) {
        return -EBUSY;
    }

    if ((vnode->flag & VNODE_FLAG_MOUNT_ORIGIN)
        || (vnode->flag & VNODE_FLAG_MOUNT_NEW)) {
        return -EBUSY;
    }

    char cwd[PATH_MAX];
    char *pret = getcwd(cwd, PATH_MAX);
    if (pret != NULL) {
        struct Vnode *cwdnode = NULL;
        int ret = VnodeLookup(cwd, &cwdnode, 0);
        if (ret == OK && (cwdnode == vnode)) {
            return -EBUSY;
        }
    }
    return OK;
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: do_rmdir
 *
 * Description:  Remove a file managed a mountpoint
 *
 ****************************************************************************/

int do_rmdir(int dirfd, const char *pathname)
{
    struct Vnode *vnode = NULL;
    char             *fullpath     = NULL;
    char             *relativepath = NULL;
    char             *name         = NULL;
    int               ret;

    if (pathname == NULL) {
        ret = -EINVAL;
        goto errout;
    }

    /* Get relative path by dirfd*/
    ret = get_path_from_fd(dirfd, &relativepath);
    if (ret < 0) {
        goto errout;
    }

    if (relativepath) {
        ret = vfs_normalize_path((const char *)relativepath, pathname, &fullpath);
        free(relativepath);
        if (ret < 0) {
            goto errout;
        }

        name = strrchr(fullpath, '/');
        VnodeHold();
        ret = VnodeLookup(fullpath, &vnode, 0);
    } else {
        name = strrchr(pathname, '/');
        VnodeHold();
        if (name == NULL) {
            name = (char *)pathname;
        } else {
            name++;
        }
        ret = VnodeLookup(pathname, &vnode, 0);
    }

    if (ret != OK) {
        goto errout_with_lock;
    }

    ret = check_target(vnode, name);
    if (ret != OK) {
        PRINT_ERR("rmdir failed err = %d\n", ret);
        goto errout_with_lock;
    }

    if (VfsVnodePermissionCheck(vnode->parent, (WRITE_OP | EXEC_OP))) {
        ret = -EACCES;
        goto errout_with_lock;
    }

    if (vnode && vnode->vop && vnode->vop->Rmdir) {
        ret = vnode->vop->Rmdir(vnode->parent, vnode, name);
    } else {
        ret = -ENOSYS;
    }
    if (ret < 0) {
        goto errout_with_lock;
    }
    VnodeFree(vnode);
    VnodeDrop();

    /* Successfully unlinked */
    if (fullpath) {
        free(fullpath);
    }

    return OK;

errout_with_lock:
    VnodeDrop();

errout:
    if (fullpath) {
        free(fullpath);
    }

    set_errno(-ret);
    return VFS_ERROR;
}

/****************************************************************************
 * Name: rmdir
 *
 * Description:  Remove a file managed a mountpoint
 *
 ****************************************************************************/

int rmdir(const char *pathname)
{
    return do_rmdir(AT_FDCWD, pathname);
}
