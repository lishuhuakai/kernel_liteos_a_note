/****************************************************************************
 * fs/vfs/fs_truncate.c
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
#include "unistd.h"
#include "fcntl.h"
#include "sched.h"
#include "assert.h"
#include "errno.h"

#include "vnode.h"

/****************************************************************************
 * Name: file_truncate
 *
 * Description:
 *   Equivalent to the standard ftruncate() function except that is accepts
 *   a struct file instance instead of a file descriptor and it does not set
 *   the errno variable.
 *
 ****************************************************************************/
static int file_truncate(struct file *filep, off_t length)
{
    struct Vnode *vnode = NULL;
    int ret;
    int err;

    /* Was this file opened for write access? */

    if (((unsigned int)(filep->f_oflags) & O_ACCMODE) == O_RDONLY) {
        err = EACCES;
        goto errout;
    }

    /* Is this vnode a registered mountpoint? Does it support the
     * truncate operations may be relevant to device drivers but only
     * the mountpoint operations vtable contains a truncate method.
     */

    vnode = filep->f_vnode;
    if (!vnode || !vnode->vop || !vnode->vop->Truncate) {
        err = ENOSYS;
        goto errout;
    }

    /* Does the file system support the truncate method?  It should if it is
     * a write-able file system.
     */

    ret = vnode->vop->Truncate(vnode, length);
    if (ret < 0) {
        err = -ret;
        goto errout;
    }

    return ret;

errout:
    set_errno(err);
    return VFS_ERROR;
}

/****************************************************************************
 * Name: ftruncate
 *
 * Description:
 *   The ftruncate() function causes the regular file referenced by fd to
 *   have a size of length bytes.
 *
 *   If the file previously was larger than length, the extra data is
 *   discarded.  If it was previously shorter than length, it is unspecified
 *   whether the file is changed or its size increased.  If the file is
 *   extended, the extended area appears as if it were zero-filled.  If fd
 *   references a shared memory object, ftruncate() sets the size of the
 *   shared memory object to length. If the file is not a regular file or
 *   a shared memory object, the result is unspecified.

 *   With ftruncate(), the file must be open for writing; for truncate(),
 *   the process must have write permission for the file.
 *
 *   ftruncate() does not modify the file offset for any open file
 *   descriptions associated with the file.
 *
 * Input Parameters:
 *   fd     - A reference to an open, regular file or shared memory object
 *            to be truncated.
 *   length - The new length of the file or shared memory object.
 *
 * Returned Value:
 * On success, 0.
 * On error, -1 is returned, and errno is set appro-priately:
 *
 *
 ****************************************************************************/

int ftruncate(int fd, off_t length)
{
#if CONFIG_NFILE_DESCRIPTORS > 0
    struct file *filep = NULL;
#endif

    /* Did we get a valid file descriptor? */

#if CONFIG_NFILE_DESCRIPTORS > 0
    if ((unsigned int)fd >= CONFIG_NFILE_DESCRIPTORS)
#endif
    {
        set_errno(EBADF);
        return VFS_ERROR;
    }

#if CONFIG_NFILE_DESCRIPTORS > 0
    /* The descriptor is in the right range to be a file descriptor... write
     * to the file.
     */

    int ret = fs_getfilep(fd, &filep);
    if (ret < 0) {
        /* The errno value has already been set */

        return VFS_ERROR;
    }

    if (filep->f_oflags & O_DIRECTORY) {
        set_errno(EBADF);
        return VFS_ERROR;
    }

    /* Perform the truncate operation using the file descriptor as an index */

    return file_truncate(filep, length);
#endif
}

int truncate(const char *path, off_t length)
{
    int fd;
    int ret;

    fd = open(path, O_RDWR);
    if (fd == VFS_ERROR) {
        /* The errno value has already been set */
        return VFS_ERROR;
    }

    ret = ftruncate(fd, length);
    close(fd);

    return ret;
}
