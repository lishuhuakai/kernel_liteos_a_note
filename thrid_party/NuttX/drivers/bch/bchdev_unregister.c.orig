/****************************************************************************
 * drivers/bch/bchdev_unregister.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <errno.h>
#include <assert.h>
#include <sys/ioctl.h>
#include "bch.h"
#include "fs/driver.h"

#define _err PRINTK

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bchdev_unregister
 *
 * Description:
 *   Unregister character driver access to a block device that was created
 *   by a previous call to bchdev_register().
 *
 ****************************************************************************/

int bchdev_unregister(const char *chardev)
{
  struct bchlib_s *bch;
  int fd;
  int ret;

  /* Sanity check */

  if (!chardev)
    {
      return -EINVAL;
    }

  /* Open the character driver associated with chardev */

  fd = open(chardev, O_RDONLY);
  if (fd < 0)
    {
      _err("ERROR: Failed to open %s: %d\n", chardev, errno);
      return -errno;
    }

  /* Get a reference to the internal data structure.  On success, we
   * will hold a reference count on the state structure.
   */

  ret = ioctl(fd, DIOC_GETPRIV, (unsigned long)((uintptr_t)&bch));
  (void)close(fd);

  if (ret < 0)
    {
      _err("ERROR: ioctl failed: %d\n", errno);
      return -errno;
    }

  /* Lock out context switches.  If there are no other references
   * and no context switches, then we can assume that we can safely
   * teardown the driver.
   */

  LOS_TaskLock();

  /* Check if the internal structure is non-busy (we hold one reference). */

  if (bch->refs > 1)
    {
      ret = -EBUSY;
      goto errout_with_lock;
    }

  /* Unregister the driver (this cannot suspend or we lose our non-preemptive
   * state!).  Once the driver is successfully unregistered, we can assume
   * we have exclusive access to the state instance.
   */

  ret = unregister_driver(chardev);
  if (ret < 0)
    {
      goto errout_with_lock;
    }

  LOS_TaskUnlock();

  /* Release the internal structure */

  bch->refs = 0;
  return bchlib_teardown(bch);

errout_with_lock:
  bch->refs--;
  LOS_TaskUnlock();
  return ret;
}
