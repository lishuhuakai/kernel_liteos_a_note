/****************************************************************************
 * fs/vfs/fs_write.c
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
#include "sys/socket.h"
#include "console.h"
#include "user_copy.h"
#include "vnode.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: file_write
 *
 * Description:
 *   Equivalent to the standard write() function except that is accepts a
 *   struct file instance instead of a file descriptor.  It is functionally
 *   equivalent to write() except that in addition to the differences in
 *   input paramters:
 *
 *  - It does not modify the errno variable,
 *  - It is not a cancellation point, and
 *  - It does not handle socket descriptors.
 *
 * Input Parameters:
 *   filep  - Instance of struct file to use with the write
 *   buf    - Data to write
 *   nbytes - Length of data to write
 *
 * Returned Value:
 *  On success, the number of bytes written are returned (zero indicates
 *  nothing was written).  On any failure, a negated errno value is returned
 *  (see comments withwrite() for a description of the appropriate errno
 *  values).
 *
 ****************************************************************************/
ssize_t file_write(struct file *filep, const void *buf, size_t nbytes)
{
    int ret;
    int err;

    if (buf == NULL) {
        err = EFAULT;
        goto errout;
    }

    /* Was this file opened for write access? */

    if ((((unsigned int)(filep->f_oflags)) & O_ACCMODE) == O_RDONLY) {
        err = EACCES;
        goto errout;
    }

    /* Is a driver registered? Does it support the write method? */

    if (!filep->ops || !filep->ops->write) {
        err = EBADF;
        goto errout;
    }

    /* Yes, then let the driver perform the write */

    ret = filep->ops->write(filep, (const char *)buf, nbytes);
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
 * Name: write
 *
 * Description:
 *  write() writes up to nytes bytes to the file referenced by the file
 *  descriptor fd from the buffer starting at buf.
 *
 * Input Parameters:
 *   fd     - file descriptor (or socket descriptor) to write to
 *   buf    - Data to write
 *   nbytes - Length of data to write
 *
 * Returned Value:
 *  On success, the number of bytes written are returned (zero indicates
 *  nothing was written). On error, -1 is returned, and errno is set appro-
 *  priately:
 *
 *  EAGAIN
 *    Non-blocking I/O has been selected using O_NONBLOCK and the write
 *    would block.
 *  EBADF
 *    fd is not a valid file descriptor or is not open for writing.
 *  EFAULT
 *    buf is outside your accessible address space.
 *  EFBIG
 *    An attempt was made to write a file that exceeds the implementation
 *    defined maximum file size or the process' file size limit, or
 *    to write at a position past the maximum allowed offset.
 *  EINTR
 *    The call was interrupted by a signal before any data was written.
 *  EINVAL
 *    fd is attached to an object which is unsuitable for writing; or
 *    the file was opened with the O_DIRECT flag, and either the address
 *    specified in buf, the value specified in count, or the current
 *     file offset is not suitably aligned.
 *  EIO
 *    A low-level I/O error occurred while modifying the vnode.
 *  ENOSPC
 *    The device containing the file referred to by fd has no room for
 *    the data.
 *  EPIPE
 *    fd is connected to a pipe or socket whose reading end is closed.
 *    When this happens the writing process will also receive a SIGPIPE
 *    signal. (Thus, the write return value is seen only if the program
 *    catches, blocks or ignores this signal.)
 *
 ****************************************************************************/

ssize_t write(int fd, const void *buf, size_t nbytes)
{
#if CONFIG_NFILE_DESCRIPTORS > 0
    struct file *filep;
#endif

    /* Did we get a valid file descriptor? */

#if CONFIG_NFILE_DESCRIPTORS > 0
    if ((unsigned int)fd >= CONFIG_NFILE_DESCRIPTORS)
#endif
    {
        /* Write to a socket descriptor is equivalent to send with flags == 0 */

#if defined(LOSCFG_NET_LWIP_SACK)
        const void *bufbak = buf;
        ssize_t ret;
        if (LOS_IsUserAddress((VADDR_T)(uintptr_t)buf)) {
            if (buf != NULL && nbytes > 0) {
                buf = malloc(nbytes);
                if (buf == NULL) {
                    set_errno(ENOMEM);
                    return VFS_ERROR;
                }
                if (LOS_ArchCopyFromUser((void*)buf, bufbak, nbytes) != 0) {
                    free((void*)buf);
                    set_errno(EFAULT);
                    return VFS_ERROR;
                }
            }
        }
        ret = send(fd, buf, nbytes, 0);
        if (buf != bufbak) {
            free((void*)buf);
        }
        return ret;
#else
        set_errno(EBADF);
        return VFS_ERROR;
#endif
    }

#if CONFIG_NFILE_DESCRIPTORS > 0
    /* The descriptor is in the right range to be a file descriptor... write
     * to the file.
     */

    if (fd <= STDERR_FILENO && fd >= STDIN_FILENO) { /* fd : [0,2] */
        fd = ConsoleUpdateFd();
        if (fd < 0) {
            set_errno(EBADF);
            return VFS_ERROR;
        }
    }

    int ret = fs_getfilep(fd, &filep);
    if (ret < 0) {
        /* The errno value has already been set */
        return VFS_ERROR;
    }

    if (filep->f_oflags & O_DIRECTORY) {
        set_errno(EBADF);
        return VFS_ERROR;
    }

    if (filep->f_oflags & O_APPEND) {
        if (file_seek64(filep, 0, SEEK_END) == -1) {
            return VFS_ERROR;
        }
    }

    /* Perform the write operation using the file descriptor as an index */

    return file_write(filep, buf, nbytes);
#endif
}
