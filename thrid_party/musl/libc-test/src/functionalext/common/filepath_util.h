/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FUNCTIONALEXT_FILEPATH_UTIL_H__
#define __FUNCTIONALEXT_FILEPATH_UTIL_H__

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "test.h"

#define STR_SLASH "/"
#define STR_FILE_TXT "file.txt"
#define STR_FILE_LINK_TXT "linkfile.txt"
#define STR_FILE_IN_TXT "file_in.txt"
#define STR_FILE_OUT_TXT "file_out.txt"
#define STR_FILE_FROM_TXT "fromfile.txt"
#define STR_FILE_TO_TXT "tofile.txt"
#define STR_FILE_SWAP "swapfile"
#define STR_FILE_SYNC_TXT "sync_file_range.txt"
#define STR_TEST_TXT "test.txt"
#define STR_STAT_TEST_TXT "stattest.txt"
#define STR_FREAD_TEST_TXT "Freadtest.txt"
#define STR_FPUTC_TXT "fputc.txt"
#define STR_VFSCANF_TXT "vfscanf.txt"
#define STR_FACCESSAT_TEST_TXT "faccessattest.txt"
#define STR_FCHOWNAT_TEST_TXT "fchownattest.txt"
#define STR_WRITE_TEST_TXT "test_write.txt"

#define FILE_ABSOLUTE_PATH(file_name, path) do { \
    if(!getcwd(path, sizeof(path))) { \
        t_error("%s getcwd for path failed\n", __func__); \
        return; \
    } \
    strncat(path, STR_SLASH, 1); \
    strncat(path, file_name, strlen(file_name)); \
} while (0)

#define FILE_ABSOLUTE_DIR(path) do { \
    if(!getcwd(path, sizeof(path))) { \
        t_error("%s getcwd for dir failed\n", __func__); \
        return; \
    } \
} while (0)

#endif
