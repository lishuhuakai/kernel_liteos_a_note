/****************************************************************************
 * fs/mount/fs_foreachmountpoint.c
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
#include "fs/mount.h"
#include "vnode.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: foreach_mountpoint
 *
 * Description:
 *   Visit each mountpoint in the pseudo-file system.  The traversal is
 *   terminated when the callback 'handler' returns a non-zero value, or when
 *   all of the mountpoints have been visited.
 *
 *   This is just a front end "filter" to foreach_vnode() that forwards only
 *   mountpoint vnodes.  It is intended to support the mount() command to
 *   when the mount command is used to enumerate mounts.
 *
 *   NOTE 1: Use with caution... The pseudo-file system is locked throughout
 *   the traversal.
 *   NOTE 2: The search algorithm is recursive and could, in principle, use
 *   an indeterminant amount of stack space.  This will not usually be a
 *   real work issue.
 *
 ****************************************************************************/

int foreach_mountpoint(foreach_mountpoint_t handler, void *arg)
{
  int ret;
  struct Mount *mnt = NULL;
  struct statfs statBuf = {0};
  LIST_HEAD *mntList = GetMountList();
  LOS_DL_LIST_FOR_EACH_ENTRY(mnt, mntList, struct Mount, mountList)
    {
      if (mnt->ops->Statfs != NULL)
        {
           ret = mnt->ops->Statfs(mnt, &statBuf);
           if (ret == OK)
             {
                (void)handler(mnt->devName, mnt->pathName, &statBuf, arg);
             }
        }
    }
  return 0;
}
