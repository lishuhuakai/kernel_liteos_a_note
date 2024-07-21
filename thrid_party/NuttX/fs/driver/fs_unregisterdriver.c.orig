/****************************************************************************
 * fs/driver/fs_unregisterdriver.c
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
#include "fs/driver.h"
#include "vnode.h"
#include "string.h"
#include "errno.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: unregister_driver
 *
 * Description:
 *   Remove the character driver vnode at 'path' from the pseudo-file system
 *
 ****************************************************************************/

int unregister_driver(const char *path)
{
  struct Vnode *vnode = NULL;
  int ret;
  if (path == NULL)
    {
      return -EINVAL;
    }
  VnodeHold();
  ret = VnodeLookup(path, &vnode, V_DUMMY);
  if (ret != OK)
    {
      VnodeDrop();
      return ret;
    }
  if (vnode->type != VNODE_TYPE_CHR && vnode->type != VNODE_TYPE_BLK)
    {
      VnodeDrop();
      return -EPERM;
    }
  ret = VnodeFree(vnode);
  VnodeDrop();

  return ret;
}
