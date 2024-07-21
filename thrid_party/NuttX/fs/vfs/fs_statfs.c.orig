/****************************************************************************
 * fs/vfs/fs_statfs.c
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

#include "sys/statfs.h"
#include "string.h"
#include "sched.h"
#include "vnode.h"
#include "fs/mount.h"
#include "errno.h"
#include "stdlib.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: statfs
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
 *   ENOSYS  The file system does not support this call.
 *
 ****************************************************************************/

int statfs(const char *path, struct statfs *buf)
{
  struct Vnode *vnode = NULL;
  struct Mount *mnt = NULL;
  int ret = OK;

  /* Sanity checks */
  if (!path || !buf)
    {
      ret = -EFAULT;
      goto errout;
    }
  if (!path[0])
    {
      ret = -ENOENT;
      goto errout;
    }
  /* Get an vnode for this file */
  VnodeHold();
  ret = VnodeLookup(path, &vnode, 0);
  if (ret < 0)
    {
      VnodeDrop();
      goto errout;
    }
  vnode->useCount++;
  VnodeDrop();

  mnt = vnode->originMount;
  if (mnt == NULL || mnt->ops == NULL || mnt->ops->Statfs == NULL)
    {
      ret = -ENOSYS;
      goto errout_with_useCount;
    }
  else
    {
      ret = mnt->ops->Statfs(mnt, buf);
      if (ret < 0)
        {
          goto errout_with_useCount;
        }
    }

  VnodeHold();
  vnode->useCount--;
  VnodeDrop();

  return OK;

  /* Failure conditions always set the errno appropriately */

errout_with_useCount:
  VnodeHold();
  vnode->useCount--;
  VnodeDrop();
errout:
  set_errno(-ret);
  return VFS_ERROR;
}
