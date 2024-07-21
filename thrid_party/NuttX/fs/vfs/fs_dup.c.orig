/****************************************************************************
 * fs/vfs/fs_dup.c
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
#if defined(LOSCFG_NET_LWIP_SACK)
#include "lwip/sockets.h"
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dup
 *
 * Description:
 *   Clone a file or socket descriptor to an arbitray descriptor number
 *
 ****************************************************************************/

int dup(int fd)
{
  int ret = OK;

  /* Check the range of the descriptor to see if we got a file or a socket
   * descriptor.
   */

#if CONFIG_NFILE_DESCRIPTORS > 0
  if ((unsigned int)fd < CONFIG_NFILE_DESCRIPTORS)
    {
      /* 0,1,2 fd is not opened in system, no need to dup them, return fd directly */

      if ((fd >= STDIN_FILENO) && (fd <= STDERR_FILENO))
        {
          return fd;
        }

      /* Its a valid file descriptor.. dup the file descriptor using any
       * other file descriptor.  fd_dupfd() sets the errno value in the
       * event of any failures.
       */

      ret = fs_dupfd(fd, 3); /* 3: file start fd */
    }
  else
#endif
    {
      /* Not a valid file descriptor.  Did we get a valid socket descriptor? */
#if defined(LOSCFG_NET_LWIP_SACK)
      if ((unsigned int)fd < (unsigned int)(CONFIG_NFILE_DESCRIPTORS+CONFIG_NSOCKET_DESCRIPTORS))
        {
          /* Yes.. dup the socket descriptor.  The errno value is not set. */

          set_errno(EBADF); /* When net_dupsd is still closed,errno should set */
          ret = VFS_ERROR;

        }
      else
#endif
        {
          /* No.. then it is a bad descriptor number */

          set_errno(EBADF);
          ret = VFS_ERROR;
        }
    }

  return ret;
}
