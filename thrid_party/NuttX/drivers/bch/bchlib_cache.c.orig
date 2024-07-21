/****************************************************************************
 * drivers/bch/bchlib_cache.c
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
#include <sys/types.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
#include "bch.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bchlib_flushsector
 *
 * Description:
 *   Flush the current contents of the sector buffer (if dirty)
 *
 * Assumptions:
 *   Caller must assume mutual exclusion
 *
 ****************************************************************************/

int bchlib_flushsector(struct bchlib_s *bch)
{
  int ret = OK;

  /* Check if the sector has been modified and is out of synch with the
   * media.
   */

  if (bch->dirty)
    {
      /* Write the sector to the media */

      ret = los_disk_write(bch->disk->disk_id, (const void *)bch->buffer, bch->sector, 1);
      if (ret < 0)
        {
          PRINTK("bchlib_flushsector Write failed: %d\n", ret);
          return ret;
        }

      /* The sector is now in sync with the media */

      bch->dirty = false;
    }

  return ret;
}

/****************************************************************************
 * Name: bchlib_readsector
 *
 * Description:
 *   Flush the current contents of the sector buffer (if dirty)
 *
 * Assumptions:
 *   Caller must assume mutual exclusion
 *
 ****************************************************************************/

int bchlib_readsector(struct bchlib_s *bch, unsigned long long sector)
{
  int ret = OK;

  if (bch->sector != sector)
    {
      ret = bchlib_flushsector(bch);
      if (ret < 0)
        {
          return ret;
        }

      bch->sector = (unsigned long long)-1;
      /* useRead is set TRUE, it'll use read block for not reading large data */
      ret = los_disk_read(bch->disk->disk_id, (void *)bch->buffer, sector, 1, TRUE);
      if (ret < 0)
        {
          PRINTK("Read failed: %d\n", ret);
          return ret;
        }
      bch->sector = sector;
    }
  return ret;
}

