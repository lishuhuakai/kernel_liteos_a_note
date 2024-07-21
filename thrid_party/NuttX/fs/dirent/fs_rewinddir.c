/****************************************************************************
 * fs/dirent/fs_rewinddir.c
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
#include "fs/dirent_fs.h"
#include "vnode.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rewinddir
 *
 * Description:
 *   The  rewinddir() function resets the position of the
 *   directory stream dir to the beginning of the directory.
 *
 * Input Parameters:
 *   dirp -- An instance of type DIR created by a previous
 *     call to opendir();
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void rewinddir(DIR *dirp)
{
    struct fs_dirent_s *idir = (struct fs_dirent_s *)dirp;
    struct Vnode *vnode_ptr = NULL;

    if (!idir || !idir->fd_root || idir->fd_status != DIRENT_MAGIC) {
        set_errno(EBADF);
        return;
    }

    /* The way we handle the readdir depends on the type of vnode
     * that we are dealing with.
     */

    vnode_ptr = idir->fd_root;
    if (vnode_ptr->vop != NULL && vnode_ptr->vop->Rewinddir != NULL) {
        /* Perform the rewinddir() operation */

        vnode_ptr->vop->Rewinddir(vnode_ptr, idir);
    } else {
        set_errno(ENOSYS);
    }

    /* Reset position for telldir() */

    idir->fd_position = 0;
}
