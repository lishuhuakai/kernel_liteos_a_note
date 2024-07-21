/****************************************************************************
 * drivers/bch/bchlib_read.c
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
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "bch.h"
#include <stdlib.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bchlib_read
 *
 * Description:
 *   Read from the block device set-up by bchlib_setup as if it were a character
 *   device.
 *
 ****************************************************************************/

ssize_t bchlib_read(void *handle, char *buffer, loff_t offset, size_t len)
{
  struct bchlib_s *bch = (struct bchlib_s *)handle;
  size_t   nsectors;
  unsigned long long   sector;
  uint16_t sectoffset;
  size_t   nbytes;
  size_t   bytesread;
  int      ret;

  /* Get rid of this special case right away */

  if (len < 1)
    {
      return 0;
    }

  /* Convert the file position into a sector number an offset. */

  sector     = offset / bch->sectsize;
  sectoffset = offset - sector * bch->sectsize;

  if (sector >= bch->nsectors)
    {
      /* Return end-of-file */

      return 0;
    }

  /* Read the initial partial sector */

  bytesread = 0;
  if (sectoffset > 0)
    {
      /* Read the sector into the sector buffer */

      ret = bchlib_readsector(bch, sector + bch->sectstart);
      if (ret < 0)
        {
          return bytesread;
        }

      /* Copy the tail end of the sector to the user buffer */

      if (sectoffset + len > bch->sectsize)
        {
          nbytes = bch->sectsize - sectoffset;
        }
      else
        {
          nbytes = len;
        }

      ret = LOS_CopyFromKernel(buffer, len, &bch->buffer[sectoffset], nbytes);
      if (ret != EOK)
        {
          PRINTK("ERROR: bchlib_read failed: %d\n", ret);
          return bytesread;
        }

      /* Adjust pointers and counts */

      ++sector;

      if (sector >= bch->nsectors)
        {
          return nbytes;
        }

      bytesread  = nbytes;
      buffer    += nbytes;
      len       -= nbytes;
    }

  /* Then read all of the full sectors following the partial sector directly
   * into the user buffer.
   */

  if (len >= bch->sectsize)
    {
      nsectors = len / bch->sectsize;
      if (sector + nsectors > bch->nsectors)
        {
          nsectors = bch->nsectors - sector;
        }
      /* No need for reading large contiguous data, useRead(param 4) is set TRUE */
      ret = los_disk_read(bch->disk->disk_id, (void *)buffer, sector + bch->sectstart, nsectors, TRUE);

      if (ret < 0)
        {
          PRINTK("ERROR: Read failed: %d\n", ret);
          return bytesread;
        }

      /* Adjust pointers and counts */

      sector    += nsectors;
      nbytes     = nsectors * bch->sectsize;
      bytesread += nbytes;

      if (sector >= bch->nsectors)
        {
          return bytesread;
        }

      buffer    += nbytes;
      len       -= nbytes;
    }

  /* Then read any partial final sector */

  if (len > 0)
    {
      /* Read the sector into the sector buffer */

      ret = bchlib_readsector(bch, sector + bch->sectstart);
      if (ret < 0)
        {
          return bytesread;
        }

      /* Copy the head end of the sector to the user buffer */

      ret = LOS_CopyFromKernel(buffer, len, bch->buffer, len);
      if (ret != EOK)
        {
          PRINTK("ERROR: bchlib_read failed: %d\n", ret);
          return bytesread;
        }

      /* Adjust counts */

      bytesread += len;
    }

  return bytesread;
}
