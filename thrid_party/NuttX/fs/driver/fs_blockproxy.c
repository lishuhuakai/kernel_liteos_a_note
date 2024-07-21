/****************************************************************************
 * fs/driver/fs_blockproxy.c
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

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "fs/driver.h"
#include "blockproxy.h"

#ifdef LOSCFG_FS_VFS_BLOCK_DEVICE

/****************************************************************************
 * Private Data
 ****************************************************************************/

static uint32_t g_devno;
static sem_t g_devno_sem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: unique_chardev
 *
 * Description:
 *   Create a unique temporary device name in the /dev/ directory of the
 *   pseudo-file system.  We cannot use mktemp for this because it will
 *   attempt to open() the file.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   The allocated path to the device.  This must be released by the caller
 *   to prevent memory links.  NULL will be returned only the case where
 *   we fail to allocate memory.
 *
 ****************************************************************************/

static char *unique_chardev(void)
{
    struct stat statbuf;
    char devbuf[16];
    uint32_t devno;
    int ret;

    /* Loop until we get a unique device name */

    for (; ;) {
        /* Get the semaphore protecting the path number */

        while (sem_wait(&g_devno_sem) != 0) {
            /* The only case that an error should occur here is if the wait
             * was awakened by a signal.
             */

            ret = get_errno();
            LOS_ASSERT(ret == EINTR);
        }

        /* Get the next device number and release the semaphore */

        devno = ++g_devno;
        (void)sem_post(&g_devno_sem);

        /* Construct the full device number */

        devno &= 0xffffff;
        ret = snprintf_s(devbuf, 16, 14, "/dev/tmp%06lx", (unsigned long)devno);
        /* length of the format string is 14 */
        if (ret < 0) {
            set_errno(ENAMETOOLONG);
            return strdup(devbuf);
        }

        /* Make sure that file name is not in use */

        ret = stat(devbuf, &statbuf);
        if (ret < 0) {
            DEBUGASSERT(errno == ENOENT);
            return strdup(devbuf);
        }

        /* It is in use, try again */
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: block_proxy
 *
 * Description:
 *   Create a temporary char driver using drivers/bch to mediate character
 *   oriented accessed to the block driver.
 *
 * Input Parameters:
 *   blkdev - The path to the block driver
 *   oflags - Character driver open flags
 *
 * Returned Value:
 *   If positive, non-zero file descriptor is returned on success.  This
 *   is the file descriptor of the nameless character driver that mediates
 *   accesses to the block driver.
 *
 *   Errors that may be returned:
 *
 *     ENOMEM - Failed to create a temporay path name.
 *
 *   Plus:
 *
 *     - Errors reported from bchdev_register()
 *     - Errors reported from open() or unlink()
 *
 ****************************************************************************/

int block_proxy(const char *blkdev, int oflags)
{
    struct file *filep = NULL;
    struct Vnode *vnode = NULL;
    char *chardev;
    bool readonly;
    int ret;
    int fd;

    DEBUGASSERT(blkdev);
    (void)sem_init(&g_devno_sem, 0, 1);

    /* Create a unique temporary file name for the character device */

    chardev = unique_chardev();
    if (chardev == NULL) {
        PRINTK("ERROR: Failed to create temporary device name\n");
        (void)sem_destroy(&g_devno_sem);
        return -ENOMEM;
    }

    /* Should this character driver be read-only? */

    readonly = (((unsigned int)oflags & O_ACCMODE) == O_RDONLY);

    /* Wrap the block driver with an instance of the BCH driver */

    ret = bchdev_register(blkdev, chardev, readonly);
    if (ret < 0) {
        PRINTK("ERROR: bchdev_register(%s, %s) failed: %d\n", blkdev, chardev, ret);
        goto errout_with_chardev;
    }

    /* Open the newly created character driver */

    oflags =(unsigned int)oflags & (~(O_CREAT | O_EXCL | O_APPEND | O_TRUNC));
    fd = open(chardev, oflags);
    if (fd < 0) {
        ret = -errno;
        PRINTK("ERROR: Failed to open %s: %d\n", chardev, ret);
        goto errout_with_bchdev;
    }

    ret = fs_getfilep(fd, &filep);
    if (ret < 0) {
        files_release(fd);
        ret = -get_errno();
        goto errout_with_bchdev;
    }

    vnode = filep->f_vnode;
    VnodeHold();
    vnode->type = VNODE_TYPE_BCHR;
    VnodeDrop();

    /* Free the allocate character driver name and return the open file
     * descriptor.
     */

    (void)free(chardev);
    (void)sem_destroy(&g_devno_sem);
    return fd;

errout_with_bchdev:
    (void)unregister_driver(chardev);
errout_with_chardev:
    (void)free(chardev);
    (void)sem_destroy(&g_devno_sem);
    return ret;
}

#endif
