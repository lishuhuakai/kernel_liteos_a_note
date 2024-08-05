/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <dirent.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <test.h>

#define NAME_BUFFER_SIZE 512

void check_log(const char *file, const char *pattern)
{
    FILE *fp = fopen(file, "r");
    if (!fp) {
        return;
    }
    if (fseek(fp, 0, SEEK_END) == -1) {
        fclose(fp);
        return;
    }
    int size = ftell(fp);
    if (size <= 0) {
        fclose(fp);
        t_error("FAIL %s size is <=0!\n", file);
    }
    if (fseek(fp, 0, SEEK_SET) == -1) {
        fclose(fp);
        return;
    }
    char *buffer = malloc(size);
    if (!buffer) {
        t_error("FAIL %s malloc %d failed!\n", size);
        return;
    }

    int rsize = fread(buffer, 1, size, fp);
    if (rsize == 0) {
        fclose(fp);
        return;
    }

    if (strstr(buffer, pattern) != NULL) {
        printf("It's ok to found %s in %s.\n", pattern, file);
    } else {
        // libctest use "FAIL" to determine whether test case failed.
        t_error("FAIL can't find %s in %s!\n", pattern, file);
    }
    int r = fclose(fp);
    if (r) {
        t_error("FAIL fclose failed!\n");
    }
    return;
}

void find_and_check_file(const char *log_dir, const char *file_tag, const char *pattern)
{
    struct dirent *ptr;
    int found = 0;
    if (!log_dir) {
        return;
    }
    DIR *dir = opendir(log_dir);
    if (!dir) {
        return;
    }
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, file_tag) != NULL) {
            found = 1;
            char target_file[NAME_BUFFER_SIZE];
            snprintf(target_file, NAME_BUFFER_SIZE, "%s%s", log_dir, ptr->d_name);
            check_log(target_file, pattern);
        }
    }
    if (!found) {
        t_error("FAIL can't find matched file, log_dir:%s file_tag:%s.\n", log_dir, file_tag);
    }
    closedir(dir);
}

void clear_log(const char *log_dir, const char *file_tag)
{
    struct dirent *ptr;
    if (!log_dir) {
        return;
    }
    DIR *dir = opendir(log_dir);
    if (!dir) {
        return;
    }
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, file_tag) != NULL) {
            char target_file[NAME_BUFFER_SIZE];
            snprintf(target_file, NAME_BUFFER_SIZE, "%s%s", log_dir, ptr->d_name);
            remove(target_file);
        }
    }
    closedir(dir);
}