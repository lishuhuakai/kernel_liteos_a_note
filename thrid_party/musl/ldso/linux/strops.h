/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _STR_OPS_H
#define _STR_OPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <stdlib.h>
#include <string.h>

typedef struct _str_list_ {
  size_t num;     /** Number of entries in list */
  size_t size;  /** Storage size */
  char **strs;  /** string list */
} strlist;

hidden strlist *strlist_alloc(size_t size);
hidden void strlist_free(strlist *strs);

hidden void strlwc(char *str);
hidden size_t strtrim(char *str);
hidden strlist *strsplit(const char *str, const char *split_s);
hidden void strlist_set(strlist *strs, const char *str);

hidden char *ld_strdup(const char *);

#ifdef __cplusplus
}
#endif

#endif
