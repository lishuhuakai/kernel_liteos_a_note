/****************************************************************************
 * fs/vfs/fs_fsync.c
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
#include "fcntl.h"
#include "errno.h"
#include "assert.h"



#include "vnode.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: file_fsync
 *
 * Description:
 *   Equivalent to the standard fsync() function except that is accepts a
 *   struct file instance instead of a file descriptor and it does not set
 *   the errno variable.
 *
 ****************************************************************************/

int file_fsync(struct file *filep)
{
    int ret;

    /* Was this file opened for write access? */

    if ((filep->f_oflags & O_ACCMODE) == 0) {
        ret = EBADF;
        goto errout;
    }

    /* Is this vnode a registered mountpoint? Does it support the
     * sync operations may be relevant to device drivers but only
     * the mountpoint operations vtable contains a sync method.
     */

    if (!filep || !filep->ops || !filep->ops->fsync) {
        ret = EINVAL;
        goto errout;
    }

    /* Yes, then tell the mountpoint to sync this file */

    ret = filep->ops->fsync(filep);
    if (ret >= 0) {
        return OK;
    }

    ret = -ret;

errout:
    set_errno(ret);
    return VFS_ERROR;
}

/****************************************************************************
 * Name: fsync
 *
 * Description:
 *   This func simply binds vnode sync methods to the sync system call.
 *
 ****************************************************************************/

int fsync(int fd)
{
    struct file *filep = NULL;

    /* Get the file structure corresponding to the file descriptor. */

    int ret = fs_getfilep(fd, &filep);
    if (ret < 0) {
        /* The errno value has already been set */
        return VFS_ERROR;
    }

    /* Perform the fsync operation */

    return file_fsync(filep);
}

