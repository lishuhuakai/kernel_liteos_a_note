/****************************************************************************
 * fs/dirent/fs_seekdir.c
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
#include "dirent.h"
#include "errno.h"
#include "fs/dirent_fs.h"
#include "vnode.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: seekmountptdir
 ****************************************************************************/

static inline void seekmountptdir(struct fs_dirent_s *idir, off_t offset)
{
    struct Vnode *vnode;
    off_t pos;

    /* Determine a starting point for the seek.  If the seek
     * is "forward" from the current position, then we will
     * start at the current poisition.  Otherwise, we will
     * "rewind" to the root dir.
     */

    vnode = idir->fd_root;
    if (offset < idir->fd_position) {
        if (vnode->vop != NULL && vnode->vop->Rewinddir != NULL) {
            /* Perform the rewinddir() operation */

            vnode->vop->Rewinddir(vnode, idir);
            pos = 0;
        } else {
            /* We can't do the seek and there is no way to return
             * an error indication.
             */

            return;
        }
    } else {
        pos = idir->fd_position;
    }

    /* This is a brute force approach... we will just read
     * directory entries until we are at the desired position.
     */

    idir->read_cnt = 1;

    while (pos < offset) {
        if (!vnode->vop || !vnode->vop->Readdir ||
            vnode->vop->Readdir(vnode, idir) <= 0) {
            /* We can't read the next entry and there is no way to return
             * an error indication.
             */

            return;
        }

        /* Increment the position on each successful read */

        pos++;
    }

    /* If we get here the directory position has been successfully set */
    idir->fd_position = pos;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: seekdir
 *
 * Description:
 *   The seekdir() function sets the location in the directory stream from
 *   which the next readdir() call will start.  seekdir() should be used with
 *   an offset returned by telldir().
 *
 * Input Parameters:
 *   dirp -- An instance of type DIR created by a previous
 *     call to opendir();
 *   offset -- offset to seek to
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void seekdir(DIR *dirp, long offset)
{
    struct fs_dirent_s *idir = (struct fs_dirent_s *)dirp;

    if (!idir || !idir->fd_root || idir->fd_status != DIRENT_MAGIC) {
        set_errno(EBADF);
        return;
    }

    if (offset < 0) {
        set_errno(EINVAL);
        return;
    }

    seekmountptdir(idir, offset);
}
