/****************************************************************************
 * include/nuttx/fs/fs.h
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

#ifndef __INCLUDE_FS_FS_H
#define __INCLUDE_FS_FS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "mount.h"
#include "fs/driver.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/
struct fsmap_t
{
  const char                      *fs_filesystemtype;
  const struct                    MountOps *fs_mops;
  const BOOL                      is_mtd_support;
  const BOOL                      is_bdfs;
};

#define FSMAP_ENTRY(_l, _name, _mop, _is_mtd_support, _is_bdfs)                      \
struct fsmap_t _l LOS_HAL_TABLE_ENTRY(fsmap) =  \
{                                                       \
  _name,                                              \
  &_mop,                                               \
  _is_mtd_support,                                    \
  _is_bdfs                                                \
}

#define FAR     /* for hdf compatibility, should remove later */

/****************************************************************************
 * Name: fs_initialize
 *
 * Description:
 *   This is called from the OS initialization logic to configure the file
 *   system.
 *
 ****************************************************************************/

void fs_initialize(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_FS_FS_H */
