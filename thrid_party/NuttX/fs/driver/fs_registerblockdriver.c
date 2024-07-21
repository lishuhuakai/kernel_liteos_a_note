/****************************************************************************
 * fs/driver/fs_registerblockdriver.c
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
#include "sys/types.h"
#include "errno.h"
#include "fs/driver.h"
#include "vnode.h"
#include "string.h"
#include "path_cache.h"
#include "vnode.h"
#include "limits.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: register_blockdriver
 *
 * Description:
 *   Register a block driver vnode the pseudo file system.
 *
 * Input parameters:
 *   path - The path to the vnode to create
 *   bops - The block driver operations structure
 *   mode - inmode priviledges (not used)
 *   priv - Private, user data that will be associated with the vnode.
 *
 * Returned Value:
 *   Zero on success (with the vnode point in 'vnode'); A negated errno
 *   value is returned on a failure (all error values returned by
 *   vnode_reserve):
 *
 *   EINVAL - 'path' is invalid for this operation
 *   EEXIST - An vnode already exists at 'path'
 *   ENOMEM - Failed to allocate in-memory resources for the operation
 *
 ****************************************************************************/

int register_blockdriver(const char *path,
                         const struct block_operations *bops,
                         mode_t mode, void *priv)
{
    struct Vnode *vp = NULL;
    int ret;

    if (path == NULL || strlen(path) >= PATH_MAX || strncmp("/dev/", path, DEV_PATH_LEN) != 0) {
        return -EINVAL;
    }

    /* Insert an vnode for the device driver -- we need to hold the vnode
     * semaphore to prevent access to the tree while we this.  This is because
     * we will have a momentarily bad true until we populate the vnode with
     * valid data.
     */

    struct drv_data *data = (struct drv_data *)zalloc(sizeof(struct drv_data));

    data->ops = (void *)bops;
    data->mode = mode;
    data->priv = priv;

    VnodeHold();
    ret = VnodeLookup(path, &vp, V_CREATE | V_DUMMY);
    if (ret == OK) {
        /* We have it, now populate it with block driver specific information. */

        vp->type = VNODE_TYPE_BLK;
        vp->data = data;
        vp->mode = mode;
    }

    VnodeDrop();
    return ret;
}
