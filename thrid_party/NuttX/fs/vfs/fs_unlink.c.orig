/****************************************************************************
 * fs/vfs/fs_unlink.c
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
#include "fcntl.h"

#include "vnode.h"
#include "stdlib.h"
#include "fs/mount.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int check_target(struct Vnode *vnode)
{
  if (vnode->type == VNODE_TYPE_DIR)
    {
      return -EISDIR;
    }

  if ((vnode->originMount) && (vnode->originMount->mountFlags & MS_RDONLY))
    {
      return -EROFS;
    }

  if (vnode->useCount > 0)
    {
      return -EBUSY;
    }

  if (VfsVnodePermissionCheck(vnode->parent, (WRITE_OP | EXEC_OP)))
    {
      return -EACCES;
    }
  return OK;
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: do_unlink
 *
 * Description:  Remove a file managed a mountpoint
 *
 ****************************************************************************/

int do_unlink(int dirfd, const char *pathname)
{
  struct Vnode *vnode = NULL;
  int ret;
  char *name = NULL;
  char *fullpath = NULL;
  char *relativepath = NULL;

  /* Get relative path by dirfd*/
  ret = get_path_from_fd(dirfd, &relativepath);
  if (ret < 0)
    {
      goto errout;
    }

  ret = vfs_normalize_path((const char *)relativepath, pathname, &fullpath);
  if (relativepath)
    {
      free(relativepath);
    }

  if (ret < 0)
    {
      goto errout;
    }

  VnodeHold();
  ret = VnodeLookup(fullpath, &vnode, 0);
  if (ret < 0)
    {
      goto errout_with_lock;
    }

  ret = check_target(vnode);
  if (ret < 0)
    {
      goto errout_with_lock;
    }
  name = strrchr(fullpath, '/') + 1;

  if (vnode && vnode->vop && vnode->vop->Unlink)
    {
      ret = vnode->vop->Unlink(vnode->parent, vnode, name);
    }
  else if (vnode && vnode->fop && vnode->fop->unlink)
    {
      ret = vnode->fop->unlink(vnode);
      if (ret == OK) {
        goto done;
      }
    }
  else
    {
      ret = -ENOSYS;
    }

  if (ret != OK)
    {
      goto errout_with_lock;
    }

  VnodeFree(vnode);

done:
  VnodeDrop();

  /* Successfully unlinked */

  free(fullpath);
  return OK;

errout_with_lock:
  VnodeDrop();

errout:
  if (fullpath)
    {
      free(fullpath);
    }
  set_errno(-ret);

  return VFS_ERROR;
}

/****************************************************************************
 * Name: unlink
 *
 * Description:  Remove a file managed a mountpoint
 *
 ****************************************************************************/

int unlink(const char *pathname)
{
  return do_unlink(AT_FDCWD, pathname);
}

/****************************************************************************
 * Name: unlinkat
 *
 * Description:  Remove a file managed a mountpoint by dirfd
 *
 ****************************************************************************/
extern int do_rmdir(int dirfd, const char *pathname);

int unlinkat(int dirfd, const char *pathname, int flag)
{
  int ret;

  if (pathname == NULL)
    {
      ret = -EINVAL;
      goto errout;
    }

  /* Now flag only support 0 && AT_REMOVEDIR */
  if ((flag & ~AT_REMOVEDIR) != 0)
    {
      ret = -EINVAL;
      goto errout;
    }

  if (flag & AT_REMOVEDIR)
    return do_rmdir(dirfd, pathname);

  return do_unlink(dirfd, pathname);

errout:
  set_errno(-ret);
  return VFS_ERROR;
}
