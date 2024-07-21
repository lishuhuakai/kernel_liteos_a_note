/****************************************************************************
 * fs/vfs/fs_dup2.c
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

#include "errno.h"
#include "unistd.h"
#include "sched.h"
#include "vnode.h"

/* This logic in this applies only when both socket and file descriptors are
 * in that case, this function descriminates which type of dup2 is being
 * performed.
 */

#if CONFIG_NFILE_DESCRIPTORS > 0 && defined(LOSCFG_NET_LWIP_SACK)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dup2
 *
 * Description:
 *   Clone a file descriptor or socket descriptor to a specific descriptor
 *   number
 *
 ****************************************************************************/

int dup2(int fd1, int fd2)
{
    /* Check the range of the descriptor to see if we got a file or a socket
     * descriptor.
     */

    if ((unsigned int)fd1 >= CONFIG_NFILE_DESCRIPTORS) {
        /* Not a valid file descriptor.  Did we get a valid socket descriptor? */

        if ((unsigned int)fd1 < (unsigned int)(CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS)) {
            /* Yes.. dup the socket descriptor. The errno value is not set. */

            set_errno(EBADF); /* When net_dupsd2 is still closed,errno should set */
            return VFS_ERROR; /* LWIP not support */
        } else {
            /* No.. then it is a bad descriptor number */

            set_errno(EBADF);
            return VFS_ERROR;
        }
    } else {
        /* Its a valid file descriptor.. dup the file descriptor.  fd_dupfd()
         * sets the errno value in the event of any failures.
         */

        return fs_dupfd2(fd1, fd2);
    }
}

#endif /* CONFIG_NFILE_DESCRIPTORS > 0 ... */
