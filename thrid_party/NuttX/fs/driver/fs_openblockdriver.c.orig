/****************************************************************************
 * fs/driver/fs_openblockdriver.c
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
#include "fs/driver.h"
#include "vnode.h"
#include "disk.h"
#include <linux/kernel.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: open_blockdriver
 *
 * Description:
 *   Return the vnode of the block driver specified by 'pathname'
 *
 * Input Parameters:
 *   pathname - the full path to the block driver to be opened
 *   mountflags - if MS_RDONLY is not set, then driver must support write
 *     operations (see include/sys/mount.h)
 *   ppvnode - address of the location to return the vnode reference
 *
 * Returned Value:
 *   Returns zero on success or a negated errno on failure:
 *
 *   EINVAL  - pathname or pvnode is NULL
 *   ENOENT  - No block driver of this name is registered
 *   ENOTBLK - The vnode associated with the pathname is not a block driver
 *   EACCESS - The MS_RDONLY option was not set but this driver does not
 *     support write access
 *
 ****************************************************************************/


extern int find_blockdriver(const char *pathname, int mountflags, struct Vnode **vpp);

int open_blockdriver(const char *pathname, int mountflags,
                     struct Vnode **ppvnode)
{
  struct Vnode *vnode_ptr = NULL;
  los_part *part = NULL;
  los_disk *disk = NULL;
  int ret;

  /* Minimal sanity checks */

#ifdef CONFIG_DEBUG
  if (ppvnode == NULL)
    {
      ret = -EINVAL;
      goto errout;
    }
#endif

  /* Find the vnode associated with this block driver name.  find_blockdriver
   * will perform all additional error checking.
   */

  ret = find_blockdriver(pathname, mountflags, &vnode_ptr);
  if (ret < 0)
    {
      PRINT_DEBUG("Failed to file %s block driver\n", pathname);
      goto errout;
    }

  /* Open the block driver.  Note that no mutually exclusive access
   * to the driver is enforced here.  That must be done in the driver
   * if needed.
   */

  struct drv_data* drv = (struct drv_data*)vnode_ptr->data;
  struct block_operations *ops = (struct block_operations *)drv->ops;

  part = los_part_find(vnode_ptr);
  if (part != NULL)
    {
      disk = get_disk(part->disk_id);
      if (disk == NULL)
        {
          ret = -EINVAL;
          goto errout_with_vnode;
        }

      if (pthread_mutex_lock(&disk->disk_mutex) != ENOERR)
        {
          PRINT_ERR("%s %d, mutex lock fail!\n", __FUNCTION__, __LINE__);
          return -1;
        }
      if (disk->disk_status == STAT_INUSED)
        {

          if (ops->open != NULL)
            {
              ret = ops->open(vnode_ptr);
              if (ret < 0)
                {
                  PRINT_DEBUG("%s driver open failed\n", pathname);
                  (void)pthread_mutex_unlock(&disk->disk_mutex);
                  goto errout_with_vnode;
                }
            }
         }

      if (pthread_mutex_unlock(&disk->disk_mutex) != ENOERR)
        {
          PRINT_ERR("%s %d, mutex unlock fail!\n", __FUNCTION__, __LINE__);
          return -1;
        }

    }
  else
    {
      if (ops->open != NULL)
        {
          ret = ops->open(vnode_ptr);
          if (ret < 0)
            {
              PRINT_DEBUG("%s driver open failed\n", pathname);
              goto errout_with_vnode;
            }
        }
    }

  *ppvnode = vnode_ptr;
  return OK;

errout_with_vnode:
errout:
  return ret;
}
