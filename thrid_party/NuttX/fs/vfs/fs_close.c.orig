/****************************************************************************
 * fs/vfs/fs_close.c
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

#if  defined(LOSCFG_NET_LWIP_SACK)
#include "lwip/sockets.h"
#endif

#include "mqueue.h"
#include "epoll.h"
#include "fs/file.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: close
 *
 * Description:
 *   close() closes a file descriptor, so that it no longer refers to any
 *   file and may be reused. Any record locks (see fcntl(2)) held on the file
 *   it was associated with, and owned by the process, are removed (regardless
 *   of the file descriptor that was used to obtain the lock).
 *
 *   If fd is the last copy of a particular file descriptor the resources
 *   associated with it are freed; if the descriptor was the last reference
 *   to a file which has been removed using unlink(2) the file is deleted.
 *
 * Input Parameters:
 *   fd   file descriptor to close
 *
 * Returned Value:
 *   0 on success; -1 on error with errno set appropriately.
 *
 * Assumptions:
 *
 ****************************************************************************/

int close(int fd)
{
  int err;
#if CONFIG_NFILE_DESCRIPTORS > 0
  int ret;

  /* Did we get a valid file descriptor? */

  if ((unsigned int)fd >= CONFIG_NFILE_DESCRIPTORS)
#endif
    {
      /* Close a socket descriptor */

#if defined(LOSCFG_NET_LWIP_SACK)
      if ((unsigned int)fd < (unsigned int)(CONFIG_NFILE_DESCRIPTORS+CONFIG_NSOCKET_DESCRIPTORS))
        {
          return socks_close(fd);
        }
      else
#endif
#if defined(LOSCFG_COMPAT_POSIX)
      if ((unsigned int)fd >= MQUEUE_FD_OFFSET && \
          (unsigned int)fd < (unsigned int)(MQUEUE_FD_OFFSET + CONFIG_NQUEUE_DESCRIPTORS))
        {
          return mq_close((mqd_t)fd);
        }
      if ((unsigned int)fd >= EPOLL_FD_OFFSET && \
          (unsigned int)fd < (unsigned int)(EPOLL_FD_OFFSET + CONFIG_EPOLL_DESCRIPTORS))
        {
          return epoll_close(fd);
        }
#endif
      else
        {
          err = EBADF;
          goto errout;
        }
    }

#if CONFIG_NFILE_DESCRIPTORS > 0
  /* Close the driver or mountpoint.  NOTES: (1) there is no
   * exclusion mechanism here, the driver or mountpoint must be
   * able to handle concurrent operations internally, (2) The driver
   * may have been opened numerous times (for different file
   * descriptors) and must also handle being closed numerous times.
   * (3) for the case of the mountpoint, we depend on the close
   * methods bing identical in signature and position in the operations
   * vtable.
   */

  ret = files_close(fd);
  if (ret < 0)
    {
      /* An error occurred while closing the driver */

      err = -ret;
      goto errout;
    }
  return OK;

#endif

errout:
  set_errno(err);
  return VFS_ERROR;
}
