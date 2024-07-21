/****************************************************************************
 * fs/vfs/fs_stat.c
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
#include "sys/stat.h"
#include "string.h"
#include "stdlib.h"
#include "vnode.h"
/****************************************************************************
 * Global Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stat
 *
 * Returned Value:
 *   Zero on success; -1 on failure with errno set:
 *
 *   EACCES  Search permission is denied for one of the directories in the
 *           path prefix of path.
 *   EFAULT  Bad address.
 *   ENOENT  A component of the path path does not exist, or the path is an
 *           empty string.
 *   ENOMEM  Out of memory
 *   ENOTDIR A component of the path is not a directory.
 *
 ****************************************************************************/

int stat(const char *path, struct stat *buf)
{
    struct Vnode *vp = NULL;
    int ret;

    /* Sanity checks */

    if (!path || !buf) {
        ret = -EFAULT;
        goto errout;
    }

    if (!path[0]) {
        ret = -ENOENT;
        goto errout;
    }

    /* Get an vnode for this file */
    VnodeHold();
    ret = VnodeLookup(path, &vp, 0);
    if (ret < 0) {
        VnodeDrop();
        goto errout;
    }

    /* The way we handle the stat depends on the type of vnode that we
     * are dealing with.
     */

    if (vp->vop != NULL && vp->vop->Getattr != NULL) {
        vp->useCount++;
        VnodeDrop();
        ret = vp->vop->Getattr(vp, buf);
        VnodeHold();
        vp->useCount--;
        VnodeDrop();
    } else {
        VnodeDrop();
        ret = -ENOSYS;
        goto errout;
    }

    if (ret < 0) {
        goto errout;
    }

    return OK;

    /* Failure conditions always set the errno appropriately */

errout:
    set_errno(-ret);
    return VFS_ERROR;
}
