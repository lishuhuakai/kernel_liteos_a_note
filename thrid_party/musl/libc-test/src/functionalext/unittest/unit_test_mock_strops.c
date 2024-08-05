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

#include <ctype.h>
#include "strops.h"

/* string to lower */
void strlwc(char *str)
{
    if (str == NULL) return;
    while (*str != '\0') {
        *str = (char)tolower(*str);
        str++ ;
    }
    return;
}
/* trim head and tail spaces of string */
size_t strtrim(char *str)
{
    char *last = NULL;
    char *dest = str;

    if (str == NULL) return 0;

    last = str + strlen(str);
    while (isspace((int)*str) && *str) str++;
    while (last > str) {
        if (!isspace((int)*(last-1))) break;
        last--;
    }
    *last = (char)0;

    memmove(dest, str, last-str+1);
    return last-str;
}

static strlist slist;
static char* list[2];

strlist *strsplit(const char *str, const char *split_s)
{
    if(!str) return NULL;
    
    slist.num = 2;
    slist.size = 16;

    slist.strs = list;

    slist.strs[0] = "/data/test";
    slist.strs[1] = "/etc";
    return &slist;
}

#define STR_DEFAULT_SIZE   16

strlist *strlist_alloc(size_t size)
{
    return NULL;
}

static void strlist_realloc(strlist *strs)
{
}

void strlist_free(strlist *strs)
{
}

void strlist_set(strlist *strs,const char *str)
{
    if (!strs || !str) return;
    if (strs->num == strs->size) {
       strlist_realloc(strs);
    }
    if (strs->num < strs->size) {
        strs->strs[strs->num] = ld_strdup(str);
        if (strs->strs[strs->num]) strs->num++;
    }
}

static char starupbuf[80];
char *ld_strdup(const char *s)
{
    size_t l = strlen(s);
    return memcpy(starupbuf, s, l+1);
}