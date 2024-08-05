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
/* split string to list by given string */
strlist *strsplit(const char *str, const char *split_s)
{
    char *cur, *next;
    if(!str) return NULL;

    strlist *sl = strlist_alloc(0);
    char *ss = ld_strdup(str);
    if (!sl || !ss) {
        strlist_free(sl);
        __libc_free(ss);
        return NULL;
    }

    cur = ss;
    while (next = strstr(cur, split_s)) {
        *next = 0;
        strtrim(cur);
        strlist_set(sl, cur);
        cur = next + strlen(split_s);
    }
    strtrim(cur);
    strlist_set(sl, cur);
    __libc_free(ss);
    return sl;
}

#define STR_DEFAULT_SIZE   16

strlist *strlist_alloc(size_t size)
{
    strlist *strs;
    if (size < STR_DEFAULT_SIZE) size = STR_DEFAULT_SIZE ;

    strs = (strlist *)__libc_calloc(1, sizeof *strs) ;

    if (strs) {
        strs->strs  = (char **)__libc_calloc(size, sizeof *strs->strs);
        if (strs->strs) {
            strs->size = size;
        } else {
            __libc_free(strs);
            strs = NULL;
        }
    }
    return strs ;
}

static void strlist_realloc(strlist *strs)
{
    if(!strs) return;
    size_t size = 2*strs->size;
    if (size) {
        char **ss = (char **)__libc_realloc(strs->strs, size * (sizeof *strs->strs));
        if (ss) {
            strs->size = size;
            strs->strs = ss;
        }
    }
    return;
}

void strlist_free(strlist *strs)
{
    if (!strs) return;
    for (size_t i=0; i < strs->num; i++) {
        __libc_free(strs->strs[i]);
    }
    __libc_free(strs->strs);
    __libc_free(strs);
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

char *ld_strdup(const char *s)
{
    size_t l = strlen(s);
    char *d = __libc_malloc(l+1);
    if (!d) return NULL;
    return memcpy(d, s, l+1);
}