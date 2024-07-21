/****************************************************************************
 * fs/driver/fs_findblockdriver.c
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
#include "sys/mount.h"
#include "errno.h"
#include "fs/driver.h"
#include "string.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: find_blockdriver
 *
 * Description:
 *   Return the inode of the block driver specified by 'pathname'
 *
 * Input Parameters:
 *   pathname   - The full path to the block driver to be located
 *   mountflags - If MS_RDONLY is not set, then driver must support write
 *                operations (see include/sys/mount.h)
 *   ppinode    - Address of the location to return the inode reference
 *
 * Returned Value:
 *   Returns zero on success or a negated errno on failure:
 *
 *   ENOENT  - No block driver of this name is registered
 *   ENOTBLK - The inode associated with the pathname is not a block driver
 *   EACCESS - The MS_RDONLY option was not set but this driver does not
 *             support write access
 *
 ****************************************************************************/
int find_blockdriver(const char *pathname, int mountflags, struct Vnode **vpp)
{
  int ret;
  struct Vnode *vp = NULL;

  /* Sanity checks */

  /* Find the vnode registered with this pathname */
  VnodeHold();
  ret = VnodeLookup(pathname, &vp, V_DUMMY);
  if (ret < 0)
    {
      ret = -EACCES;
      goto errout;
    }

  /* Verify that the vnode is a block driver. */
  if (vp->type != VNODE_TYPE_BLK)
    {
      PRINT_DEBUG("%s is not a block driver\n", pathname);
      ret = -ENOTBLK;
      goto errout;
    }

  /* Make sure that the vnode supports the requested access */

  struct block_operations *i_bops = (struct block_operations *)((struct drv_data *)vp->data)->ops;

  if (i_bops == NULL || i_bops->read == NULL || (i_bops->write == NULL && (mountflags & MS_RDONLY) == 0))
    {
      PRINT_DEBUG("%s does not support requested access\n", pathname);
      ret = -EACCES;
      goto errout;
    }

  *vpp = vp;
  VnodeDrop();
  return OK;

errout:
  VnodeDrop();
  return ret;
}
