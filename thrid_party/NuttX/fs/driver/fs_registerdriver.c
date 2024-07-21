/****************************************************************************
 * fs/driver/fs_registerdriver.c
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
#include "limits.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: register_driver
 *
 * Description:
 *   Register a character driver vnode the pseudo file system.
 *
 * Input Parameters:
 *   path - The path to the vnode to create
 *   fops - The file operations structure
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
/*!
 * 注册驱动
 */
int register_driver(const char *path, const struct file_operations_vfs *fops,
                    mode_t mode, void *priv)
{
    struct Vnode *vnode = NULL;
    int ret;

    if (path == NULL || strlen(path) >= PATH_MAX || strncmp("/dev/", path, DEV_PATH_LEN) != 0) {
        return -EINVAL;
    }

    VnodeHold();
    ret = VnodeLookup(path, &vnode, 0); // 根据路径查找Vnode节点
    if (ret == 0) {
        VnodeDrop();
        return -EEXIST;
    }

    /* Insert a dummy node -- we need to hold the vnode semaphore because we
     * will have a momentarily bad structure.
     */

    struct drv_data *data = (struct drv_data *)zalloc(sizeof(struct drv_data));

    data->ops = (void *)fops; // 记录下驱动
    data->mode = mode;
    data->priv = priv; // 私有数据

    ret = VnodeLookup(path, &vnode, V_CREATE | V_DUMMY);
    if (ret == OK) {
        /* We have it, now populate it with driver specific information.
         * NOTE that the initial reference count on the new vnode is zero.
         */
        vnode->type = VNODE_TYPE_CHR; // 字符设备
        vnode->data = data;
        vnode->mode = mode;
        vnode->fop = (struct file_operations_vfs *)fops; // 记录下驱动
    }

    VnodeDrop();

    return ret;
}
