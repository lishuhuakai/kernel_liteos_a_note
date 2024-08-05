/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef _ZIP_ARCHIVE_H
#define _ZIP_ARCHIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PATH_BUF_SIZE 512
/* used for read zip file */
static const uint32_t LOCAL_FILE_HEADER_SIGNATURE = 0x04034b50;
static const uint32_t CENTRAL_SIGNATURE = 0x02014b50;
static const uint32_t EOCD_SIGNATURE = 0x06054b50;
static const uint16_t COMPRESS_STORED = 0;
static const char* ZIP_FILE_PATH_SEPARATOR = "!/";

struct zip_info {
    int fd;
    bool found;
    uint64_t file_offset;
    uint16_t file_path_index;
    char path_buf[PATH_BUF_SIZE];
};

/* Zip Format:
 * -------------------------------------------------------
 * | Local file header 1             |                   |
 * -----------------------------------                   |
 * | File data 1                     |                   |
 * -----------------------------------                   |
 * | Data descriptor 1               |                   |
 * -----------------------------------    File Entry     |
 * | ...                             |                   |
 * -----------------------------------                   |
 * | Local file header n             |                   |
 * -----------------------------------                   |
 * | File data n                     |                   |
 * -----------------------------------                   |
 * | Data descriptor n               |                   |
 * -------------------------------------------------------
 * | Central dir entry 1             |                   |
 * -----------------------------------                   |
 * | ...                             | Central Directory |
 * -----------------------------------                   |
 * | Central dir entry n             |                   |
 * -------------------------------------------------------
 * | End of central directory record |       EOCD        |
 * ------------------------------------------------------- */

// Local file header
struct __attribute__((packed)) local_file_header {
    uint32_t signature;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t compression_method;
    uint16_t modified_time;
    uint16_t modified_date;
    uint32_t crc;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t name_size;
    uint16_t extra_size;
};

// Central dir entry
struct __attribute__((packed)) central_dir_entry {
    uint32_t signature;
    uint16_t version_made;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t compression_method;
    uint16_t modified_time;
    uint16_t modified_date;
    uint32_t crc;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t name_size;
    uint16_t extra_size;
    uint16_t comment_size;
    uint16_t disk_num_start;
    uint16_t internal_attr;
    uint32_t external_attr;
    uint32_t local_header_offset;
};

// End of central directory record
struct __attribute__((packed)) zip_end_locator {
    uint32_t signature;
    uint16_t num_disk;
    uint16_t start_disk_of_central_dir;
    uint16_t total_entries_in_disk;
    uint16_t total_entries; /* Total number of central directory entrys. */
    uint32_t size_of_central_dir;
    uint32_t offset;  /* Offset of start of central directory entry. */
    uint16_t comment_len;
};

int open_uncompressed_library_in_zipfile(const char *path, struct zip_info *z_info, char *separator);

#ifdef __cplusplus
}
#endif

#endif
