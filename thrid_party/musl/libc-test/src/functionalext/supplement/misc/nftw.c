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

#include <errno.h>
#include <ftw.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include "functionalext.h"

#define TEST_FD_LIMIT 128
#define TEST_FLAG_SIZE 4
#define TEST_DIGIT_TWO 2
#define TEST_PATH_DEPTH 5
#define TEST_NFTW_PATH "/data/local/tmp/nftwPath"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static int nftw_callback(const char *pathname, const struct stat *sb, int flag, struct FTW *ftw)
{
    EXPECT_TRUE("nftw_callback", pathname != NULL);
    EXPECT_TRUE("nftw_callback", sb != NULL);
    if (flag == FTW_NS) {
        struct stat st;
        EXPECT_EQ("nftw_callback", stat(pathname, &st), -1);
        return 0;
    }

    if (S_ISDIR(sb->st_mode)) {
        if (access(pathname, R_OK) == 0) {
            EXPECT_TRUE("nftw_callback", flag == FTW_D || flag == FTW_DP);
        } else {
            EXPECT_EQ("nftw_callback", flag, FTW_DNR);
        }
    } else if (S_ISLNK(sb->st_mode)) {
        EXPECT_TRUE("nftw_callback", flag == FTW_SL || flag == FTW_SLN);
    } else {
        EXPECT_EQ("nftw_callback", flag, FTW_F);
    }
    return 0;
}

void remove_directory(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    char filepath[PATH_MAX];
  
    if (!(dir = opendir(path))) {
        return;
    }
  
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
  
            sprintf(filepath, "%s/%s", path, entry->d_name);
            remove_directory(filepath);
        } else {
            sprintf(filepath, "%s/%s", path, entry->d_name);
            if (remove(filepath) == -1) {
                t_error("%s error in remove test nftw filepath! \n", __func__);
            }
        }
    }
  
    closedir(dir);
  
    // Now we can remove the empty directory
    if (rmdir(path) == -1) {
        t_error("%s error in rmdir test nftw path! \n", __func__);
    }
}

void nftw_build_testfile(const char *path)
{
    // Create directory
    if (mkdir(path, 0755) == -1) {
        t_error("%s error in mkdir test nftw path! %s \n", __func__, path);
        return;
    }
    
    char file[PATH_MAX];
    sprintf(file, "%s/normal_file.txt", path);
    // Create plain file
    int fd = open(file, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        t_error("%s error in open normal_file.txt! \n", __func__);
        return;
    }
    close(fd);

    sprintf(file, "%s/non-executable_file.txt", path);
    // Create non-executable file
    fd = open(file, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        t_error("%s error in open normal_file.txt! \n", __func__);
        return;
    }
    close(fd);

    sprintf(file, "%s/unauthorized_file.txt", path);
    // Create unauthorized file
    fd = open(file, O_WRONLY | O_CREAT, 0000);
    if (fd == -1) {
        t_error("%s error in open normal_file.txt! \n", __func__);
        return;
    }
    close(fd);

    sprintf(file, "%s/.hidden_file.txt", path);
    // Create Hidden Files
    fd = open(file, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        t_error("%s error in open hidden_file.txt! \n", __func__);
        return;
    }
    close(fd);
  
    sprintf(file, "%s/read_only_file.txt", path);
    //Create Read-only files
    fd = open(file, O_WRONLY | O_CREAT, 0444);
    if (fd == -1) {
        t_error("%s error in open read_only_file.txt! \n", __func__);
        return;
    }
    close(fd);
  
    sprintf(file, "%s/symlink_to_normal_file", path);
    // Create Symbolic links
    if (symlink("normal_file.txt", file) == -1) {
        t_error("%s error in open symlink_to_normal_file.txt! \n", __func__);
        return;
    }
}

void nftw_build_testDir()
{
    nftw_build_testfile(TEST_NFTW_PATH);
    char path[PATH_MAX];
    sprintf(path, "%s", TEST_NFTW_PATH);
    for (int i = 0 ; i < TEST_PATH_DEPTH ; i++) {
        sprintf(path, "%s/data", path);
        nftw_build_testfile(path);
    }
}

/**
 * @tc.name      : nftw_0100
 * @tc.desc      : Traverse directory /data
 * @tc.level     : Level 0
 */
void nftw_0100(void)
{
    int flag[TEST_FLAG_SIZE] = {FTW_PHYS, FTW_MOUNT, FTW_CHDIR, FTW_DEPTH};
    int i;
    for (i = 0; i < TEST_FLAG_SIZE; i++) {
        int ret = nftw(TEST_NFTW_PATH, nftw_callback, TEST_FD_LIMIT, flag[i]);
        EXPECT_EQ("nftw_0100", ret, 0);
    }
}

/**
 * @tc.name      : nftw_0200
 * @tc.desc      : Traverse directory /data, but the maximum number of file descriptors is 0
 * @tc.level     : Level 0
 */
void nftw_0200(void)
{
    int ret = nftw(TEST_NFTW_PATH, nftw_callback, 0, FTW_PHYS);
    EXPECT_EQ("nftw_0200", ret, 0);
}

/**
 * @tc.name      : nftw_0300
 * @tc.desc      : The file path length exceeds PATH_MAX, traverse the directory
 * @tc.level     : Level 2
 */
void nftw_0300(void)
{
    char path[PATH_MAX * TEST_DIGIT_TWO];
    memset(path, 'a', sizeof(path));
    path[PATH_MAX * TEST_DIGIT_TWO - 1] = 0;
    int ret = nftw(path, nftw_callback, TEST_FD_LIMIT, FTW_PHYS);
    EXPECT_EQ("nftw_0300", ret, -1);
    EXPECT_EQ("nftw_0300", errno, ENAMETOOLONG);
}

int main(void)
{
    pthread_mutex_lock(&g_mutex);
    nftw_build_testDir();
    nftw_0100();
    nftw_0200();
    nftw_0300();
    remove_directory(TEST_NFTW_PATH);
    pthread_mutex_unlock(&g_mutex);
    return t_status;
}