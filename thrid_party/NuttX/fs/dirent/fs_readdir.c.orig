/****************************************************************************
 * fs/dirent/fs_readdir.c
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
#include "string.h"
#include "dirent.h"
#include "errno.h"
#include "unistd.h"
#include "fs/dirent_fs.h"
#include "user_copy.h"
#include "fs/file.h"
#include "vnode.h"

/****************************************************************************
 * Name: do_readdir
 *
 * Description:
 *   The do_readdir() function returns a pointer to a dirent structure
 *   representing the next directory entry in the directory stream pointed
 *   to by dir.  It returns NULL on reaching the end-of-file or if an error
 *   occurred.
 *
 * Input Parameters:
 *   dirp -- An instance of type DIR created by a previous call to opendir();
 *
 * Returned Value:
 *   The do_readdir() function returns a pointer to a dirent structure, or NULL
 *   if an error occurs or end-of-file is reached.  On error, errno is set
 *   appropriately.
 *
 *   EBADF   - Invalid directory stream descriptor dir
 *
 ****************************************************************************/
static struct dirent *__readdir(DIR *dirp, int *lencnt)
{
  struct fs_dirent_s *idir = (struct fs_dirent_s *)dirp;
  struct Vnode *vnode_ptr = NULL;
  int ret = 0;
  int file_cnt = 0;

  /* Verify that we were provided with a valid directory structure */

  if (!idir)
    {
      ret = -EBADF;
      goto errout;
    }

  vnode_ptr = idir->fd_root;
  if (vnode_ptr == NULL)
    {
      /* End of file and error conditions are not distinguishable
       * with readdir.  We return NULL to signal either case.
       */

      ret = -ENOENT;
      goto errout;
    }

  /* Perform the readdir() operation */
#ifdef LOSCFG_ENABLE_READ_BUFFER
  idir->read_cnt = MAX_DIRENT_NUM;
#else
  idir->read_cnt = 1;
#endif

  if (vnode_ptr->vop != NULL && vnode_ptr->vop->Readdir != NULL)
    {
      file_cnt = vnode_ptr->vop->Readdir(vnode_ptr, idir);
    }
  else
    {
      ret = -ENOSYS;
      goto errout;
    }

  if (file_cnt > 0)
    {
      *lencnt = file_cnt * sizeof(struct dirent);
      return &(idir->fd_dir[0]);
    }

errout:
  if (ret < 0)
    {
      set_errno(ret);
    }
  else if (file_cnt <= 0)
    {
      set_errno(ENOENT);
    }

  return (struct dirent *)NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: readdir
 *
 * Description:
 *   The readdir() function returns a pointer to a dirent structure
 *   representing the next directory entry in the directory stream pointed
 *   to by dir.  It returns NULL on reaching the end-of-file or if an error
 *   occurred.
 *
 * Inputs:
 *   dirp -- An instance of type DIR created by a previous call to opendir();
 *
 * Return:
 *   The readdir() function returns a pointer to a dirent structure, or NULL
 *   if an error occurs or end-of-file is reached.  On error, errno is set
 *   appropriately.
 *
 *   EBADF   - Invalid directory stream descriptor dir
 *
 ****************************************************************************/
struct dirent *readdir(DIR *dirp)
{
  int ret;
  int old_err = get_errno();
  int dirent_len = 0;
  struct dirent *de = NULL;
  struct fs_dirent_s *idir = (struct fs_dirent_s *)dirp;

  if (idir->cur_pos != 0 && idir->cur_pos < MAX_DIRENT_NUM && idir->cur_pos < idir->end_pos)
    {
      de = &(idir->fd_dir[idir->cur_pos]);

      if (idir->cur_pos == MAX_DIRENT_NUM)
        {
          idir->cur_pos = 0;
        }
      idir->cur_pos++;

      return de;
    }
  else
    {
      de = __readdir(dirp, &dirent_len);
      idir->end_pos = dirent_len / sizeof(struct dirent);
      idir->cur_pos = 1;

      if (de == NULL)
        {
          idir->cur_pos = 0;
          ret = get_errno();
          /* Special case: ret = -ENOENT is end of file */

          if (ret == ENOENT)
            {
              set_errno(old_err);
            }
        }
    }
  return de;
}

/* readdir syscall routine */

int do_readdir(int fd, struct dirent **de, unsigned int count)
{
  struct dirent *de_src = NULL;
  int de_len = 0;
  /* Did we get a valid file descriptor? */

#if CONFIG_NFILE_DESCRIPTORS > 0
  struct file *filep = NULL;

  if (de == NULL)
    {
      return -EINVAL;
    }

  if ((fd < 3) || (unsigned int)fd >= CONFIG_NFILE_DESCRIPTORS)
    {
      return -EBADF;
    }
  else
    {
      /* The descriptor is in a valid range to file descriptor... do the
       * read.  First, get the file structure.
       */

      int ret = fs_getfilep(fd, &filep);
      if (ret < 0)
        {
          /* The errno value has already been set */
          return -get_errno();
        }

      /* Then let do_readdir do all of the work */

      de_src = __readdir(filep->f_dir, &de_len);
      if (de_src == NULL)
        {
          /* Special case: ret = -ENOENT is end of file */
          return -get_errno();
        }
      *de = de_src;

      return de_len;
    }
#endif
  return OK;
}
