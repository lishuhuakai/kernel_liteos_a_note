/****************************************************************************
 * fs/tmpfs/fs_tmpfs.h
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

#ifndef __FS_TMPFS_FS_TMPFS_H
#define __FS_TMPFS_FS_TMPFS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <semaphore.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_FS_RAMFS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Indicates that there is no holder of the re-entrant semaphore */

#define TMPFS_NO_HOLDER   -1

/* Bit definitions for file object flags */

#define TFO_FLAG_UNLINKED (1 << 0)  /* Bit 0: File is unlinked */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* TMPFS memory object types */

enum tmpfs_objtype_e
{
  TMPFS_DIRECTORY = 0,  /* Directory */
  TMPFS_REGULAR         /* Regular file */
};

/* Values returned by tmpfs_foreach() */

enum tmpfs_foreach_e
{
  TMPFS_CONTINUE = 0,    /* Continue enumeration */
  TMPFS_HALT,            /* Stop enumeration */
  TMPFS_DELETED,         /* Object and directory entry deleted */
  TMPFS_UNLINKED         /* Only the directory entry was deleted */
};

/* Re-entrant semaphore */

struct tmpfs_sem_s
{
  sem_t    ts_sem;       /* The actual semaphore */
  pid_t    ts_holder;    /* Current older (-1 if not held) */
  uint16_t ts_count;     /* Number of counts held */
};

/* The form of one directory entry */

struct tmpfs_dirent_s
{
  LOS_DL_LIST tde_node;
  struct tmpfs_object_s *tde_object;
  char *tde_name;
  bool tde_inuse;
};

/* The generic form of a TMPFS memory object */

struct tmpfs_object_s
{
  struct tmpfs_dirent_s *to_dirent;
  struct tmpfs_sem_s to_exclsem;

  uint8_t  to_type;      /* See enum tmpfs_objtype_e */
  uint8_t  to_refs;      /* Reference count */
  time_t   to_ctime;     /* last changed status time */
  time_t   to_mtime;     /* last modified time */
  time_t   to_atime;     /* last access time */

  mode_t   mode;
  uint     gid;
  uint     uid;
};

/* The form of a directory memory object */

struct tmpfs_directory_s
{
  /* First fields must match common TMPFS object layout */

  struct tmpfs_dirent_s *tdo_dirent;
  struct tmpfs_sem_s tdo_exclsem;

  uint8_t  tdo_type;     /* See enum tmpfs_objtype_e */
  uint8_t  tdo_refs;     /* Reference count */
  time_t   tdo_ctime;    /* last changed status time */
  time_t   tdo_mtime;    /* last modified time */
  time_t   tdo_atime;    /* last access time */

  mode_t   mode;
  uint     gid;
  uint     uid;

  /* Remaining fields are unique to a directory object */

  uint8_t  tdo_count;    /* Number of times the directory was opened */
  uint16_t tdo_nentries; /* Number of directory entries */
  LOS_DL_LIST tdo_entry;
};

#define SIZEOF_TMPFS_DIRECTORY(n) \
  (sizeof(struct tmpfs_directory_s) + ((n) - 1) * sizeof(struct tmpfs_dirent_s))

/* The form of a regular file memory object
 *
 * NOTE that in this very simplified implementation, there is no per-open
 * state.  The file memory object also serves as the open file object,
 * saving an allocation.  This has the negative side effect that no per-
 * open state can be retained (such as open flags).
 */

struct tmpfs_file_s
{
  /* First fields must match common TMPFS object layout */

  struct tmpfs_dirent_s *tfo_dirent;
  struct tmpfs_sem_s tfo_exclsem;

  uint8_t  tfo_type;     /* See enum tmpfs_objtype_e */
  uint8_t  tfo_refs;     /* Reference count */
  time_t   tfo_ctime;    /* last changed status time */
  time_t   tfo_mtime;    /* last modified time */
  time_t   tfo_atime;    /* last access time */

  mode_t   mode;
  uint     gid;
  uint     uid;

  /* Remaining fields are unique to a directory object */

  uint8_t  tfo_flags;    /* See TFO_FLAG_* definitions */
  size_t   tfo_size;     /* Valid file size */
  char     *tfo_data;    /* File data pointer */
};

#define SIZEOF_TMPFS_FILE(n) (sizeof(struct tmpfs_file_s) + (n) - 1)

/* This structure represents one instance of a TMPFS file system */

struct tmpfs_s
{
  /* The root directory */
  struct tmpfs_dirent_s tfs_root;
  struct tmpfs_sem_s tfs_exclsem;
};

/* This is the type used the tmpfs_statfs_callout to accumulate memory usage */

struct tmpfs_statfs_s
{
  size_t tsf_alloc;      /* Total memory allocated */
  size_t tsf_inuse;      /* Total memory in use */
  off_t  tsf_files;      /* Total file nodes in the file system */
  off_t  tsf_ffree;      /* Free directory nodes in the file system */
};

/* This is the type of the for tmpfs_foreach callback */

typedef int (*tmpfs_foreach_t)(struct tmpfs_directory_s *tdo,
                               unsigned int index, void *arg);

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern void los_set_ramfs_unit(off_t size);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __FS_TMPFS_FS_TMPFS_H */
