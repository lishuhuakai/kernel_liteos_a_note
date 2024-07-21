/****************************************************************************
 * fs/vfs/fs_lseek.c
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

#include <vfs_config.h>

#include "sys/types.h"
#include "unistd.h"
#include "sched.h"
#include "assert.h"
#include "errno.h"
#include "vnode.h"

#if CONFIG_NFILE_DESCRIPTORS > 0

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: file_seek
 *
 * Description:
 *   This is the internal implementation of lseek.  See the comments in
 *   lseek() for further information.
 *
 * Input Parameters:
 *   file     File structure instance
 *   offset   Defines the offset to position to
 *   whence   Defines how to use offset
 *
 * Returned Value:
 *   The resulting offset on success.  A negated errno value is returned on
 *   any failure (see lseek comments).
 *
 ****************************************************************************/

off_t file_seek(struct file *filep, off_t offset, int whence)
{
    struct Vnode *vnode;
    int err = OK;
    off_t pos;

    vnode = filep->f_vnode;

    if (!vnode) {
        err = -EBADF;
        goto errout;
    }

    /* Invoke the file seek method if available */

    if (filep->ops != NULL && filep->ops->seek != NULL) {
        pos = filep->ops->seek(filep, offset, whence);
        if (pos < 0) {
            err = pos;
            goto errout;
        } else {
            filep->f_pos = pos;
        }
    } else {
        /* No... Just set the common file position value */
        switch (whence) {
            case SEEK_CUR:
                offset += (off_t)filep->f_pos;

            /* FALLTHROUGH */

            case SEEK_SET:
                if (offset >= 0) {
                    filep->f_pos = (loff_t)offset; /* Might be beyond the end-of-file */
                } else {
                    err = -EINVAL;
                    goto errout;
                }
                break;
            case SEEK_END:
                err = -ENOSYS;
                goto errout;

            default:
                err = -EINVAL;
                goto errout;
        }
    }

    return (off_t)filep->f_pos;

errout:
    set_errno(-err);
    return (off_t)VFS_ERROR;
}

/****************************************************************************
 * Name: lseek
 *
 * Description:
 *   The lseek() function repositions the offset of the open file associated
 *   with the file descriptor fd to the argument 'offset' according to the
 *   directive 'whence' as follows:
 *
 *   SEEK_SET
 *      The offset is set to offset bytes.
 *   SEEK_CUR
 *      The offset is set to its current location plus offset bytes.
 *   SEEK_END
 *      The offset is set to the size of the file plus offset bytes.
 *
 *  The lseek() function allows the file offset to be set beyond the end of the
 *  file (but this does not change the size of the file). If data is later written
 *  at this point, subsequent reads of the data in the gap (a "hole") return null
 *  bytes ('\0') until data is actually written into the gap.
 *
 * Input Parameters:
 *   fd       File descriptor of device
 *   offset   Defines the offset to position to
 *   whence   Defines how to use offset
 *
 * Returned Value:
 *   The resulting offset on success.  -1 on failure withi errno set properly:
 *
 *   EBADF      fd is not an open file descriptor.
 *   EINVAL     whence  is  not one of SEEK_SET, SEEK_CUR, SEEK_END; or the
 *              resulting file offset would be negative, or beyond the end of a
 *              seekable device.
 *   EOVERFLOW  The resulting file offset cannot be represented in an off_t.
 *   ESPIPE     fd is associated with a pipe, socket, or FIFO.
 *
 ****************************************************************************/

off_t lseek(int fd, off_t offset, int whence)
{
    struct file *filep;

    /* Get the file structure corresponding to the file descriptor. */

    int ret = fs_getfilep(fd, &filep);
    if (ret < 0) {
        /* The errno value has already been set */
        return (off_t)VFS_ERROR;
    }

    /* Then let file_seek do the real work */

    return file_seek(filep, offset, whence);
}
#endif
