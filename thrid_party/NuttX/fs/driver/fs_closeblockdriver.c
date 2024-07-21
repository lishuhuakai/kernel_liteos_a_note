/****************************************************************************
 * fs/driver/fs_closeblockdriver.c
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

#include "fs/driver.h"
#include "vfs_config.h"
#include "errno.h"
#include "vnode.h"
#include "disk.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: close_blockdriver
 *
 * Description:
 *   Call the close method and release the vnode
 *
 * Input Parameters:
 *   vnode - reference to the vnode of a block driver opened by open_blockdriver
 *
 * Returned Value:
 *   Returns zero on success or a negated errno on failure:
 *
 *   EINVAL  - vnode is NULL
 *   ENOTBLK - The vnode is not a block driver
 *
 ****************************************************************************/

int close_blockdriver(struct Vnode *vnode_ptr)
{
    int ret = 0; /* Assume success */
    los_part *part = NULL;
    los_disk *disk = NULL;
    struct block_operations *bop = NULL;

    /* Sanity checks */

    if (vnode_ptr == NULL || vnode_ptr->data == NULL) {
        ret = -EINVAL;
        goto errout;
    }

    bop = (struct block_operations*)(((struct drv_data*)vnode_ptr->data)->ops);

    if (bop == NULL) {
        PRINT_ERR("vnode ops is null, not a valid block driver\n");
        ret = -EINVAL;
        goto errout;
    }

    /* Verify that the vnode is a block driver. */

    if (vnode_ptr->type != VNODE_TYPE_BLK) {
        PRINT_ERR("vnode is not a block driver\n");
        ret = -ENOTBLK;
        goto errout;
    }


    part = los_part_find(vnode_ptr);
    if (part != NULL) {
        disk = get_disk(part->disk_id);
        if (disk == NULL) {
            ret = -EINVAL;
            goto errout;
        }

        if (pthread_mutex_lock(&disk->disk_mutex) != ENOERR) {
            PRINT_ERR("%s %d, mutex lock fail!\n", __FUNCTION__, __LINE__);
            return -EAGAIN;
        }

        if (disk->disk_status == STAT_INUSED) {
            /* Close the block driver.  Not that no mutually exclusive access
            * to the driver is enforced here.  That must be done in the driver
            * if needed.
            */

            if (bop->close != NULL) {
                ret = bop->close(vnode_ptr);
            }
        }

        if (pthread_mutex_unlock(&disk->disk_mutex) != ENOERR) {
            PRINT_ERR("%s %d, mutex unlock fail!\n", __FUNCTION__, __LINE__);
        }

    } else {
        if (bop->close != NULL) {
            ret = bop->close(vnode_ptr);
        }
    }

errout:
    return ret;
}
