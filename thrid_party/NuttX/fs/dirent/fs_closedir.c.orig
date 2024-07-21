/****************************************************************************
 * fs/dirent/fs_closedir.c
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
#include "dirent.h"
#include "errno.h"
#include "stdlib.h"
#include "fs/dirent_fs.h"
#include "vnode.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: closedir
 *
 * Description:
 *    The closedir() function closes the directory stream associated with
 *    'dirp'.  The directory stream descriptor 'dirp' is not available after
 *    this call.
 *
 * Input Parameters:
 *   dirp -- An instance of type DIR created by a previous call to opendir();
 *
 * Returned Value:
 *   The closedir() function returns 0 on success.  On error, -1 is
 *   returned, and errno is set appropriately.
 *
 ****************************************************************************/

int closedir(DIR *dirp)
{
  struct fs_dirent_s *idir = (struct fs_dirent_s *)dirp;
  struct Vnode *vnode = NULL;
  int ret;

  /* Verify that we were provided with a valid directory structure */

  if (!idir || idir->fd_status != DIRENT_MAGIC)
    {
      ret = -EBADF;
      goto errout;
    }

  if (idir->fd_root)
    {
      /* This is the 'root' vnode of the directory.  This means different
       * things wih different filesystems.
       */
      vnode = idir->fd_root;
      /* Perform the closedir() operation */
      if (vnode->vop && vnode->vop->Closedir)
        {
          ret = vnode->vop->Closedir(vnode, idir);
          if (ret < 0)
            {
              goto errout_with_vnode;
            }
        }
      else
        {
          ret = -ENOSYS;
          goto errout_with_vnode;
        }
      VnodeHold();
      vnode->useCount--;
      VnodeDrop();
    }

  /* Then release the container */

  idir->fd_status = 0;
  free(idir);

  return OK;

errout_with_vnode:
  free(idir);

errout:
  set_errno(-ret);
  return VFS_ERROR;
}
