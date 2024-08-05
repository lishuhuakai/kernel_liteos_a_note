/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
 */
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "test.h"
#define BUFFER_SIZE 1024
#define RENAME_ZERO 0

static int g_error = 0;
static char g_dirPath[] = "/data/local/tmp";
static char g_oldPath[] = "/data/local/tmp/renameat2_test.txt";
static char g_oldName[] = "renameat2_test.txt";
static char g_oldMsg[] = "renameat2";
static char g_newPath[] = "/data/local/tmp/newrenameat2_test.txt";
static char g_newName[] = "newrenameat2_test.txt";
static char g_newMsg[] = "newrenameat2";

static void WriteFile(char *path, char *msg)
{
    FILE *file = fopen(path, "w+");
    if (file == NULL) {
        g_error++;
        return;
    }
    size_t len = strlen(msg);
    size_t n = fwrite(msg, sizeof(char), len, file);
    if (fclose(file)) {
        g_error++;
        return;
    }
    if (n != len) {
        g_error++;
        return;
    }
}

static void ReadFile(char *path, size_t len, char *buf)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        g_error++;
        return;
    }
    size_t n = fread(buf, sizeof(char), len, file);
    if (fclose(file)) {
        g_error++;
        return;
    }
    if (n != len) {
        g_error++;
        return;
    }
}

static void CompareFileString(char *path, size_t readCount, char *compareString)
{
    char buf[BUFFER_SIZE] = {0};
    ReadFile(path, readCount, buf);
    if (strcmp(compareString, buf)) {
        g_error++;
    }
}

static void RemoveFile()
{
    if (access(g_oldPath, F_OK) == 0) {
        if (remove(g_oldPath)) {
            printf("information: file removed fail\n");
        }
    }
    if (access(g_newPath, F_OK) == 0) {
        if (remove(g_newPath)) {
            printf("information: file removed fail\n");
        }
    }
}

static void RenameNoReplaceTest(int oldFolder, int newFolder)
{
    WriteFile(g_oldPath, g_oldMsg);
    if (renameat2(oldFolder, g_oldName, newFolder, g_newName, RENAME_NOREPLACE) == -1) {
        g_error++;
        return;
    }
    if (access(g_oldPath, F_OK) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_newPath, strlen(g_oldMsg), g_oldMsg);
    RemoveFile();
    
    WriteFile(g_oldPath, g_oldMsg);
    WriteFile(g_newPath, g_newMsg);
    if (renameat2(oldFolder, g_oldName, newFolder, g_newName, RENAME_NOREPLACE) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_oldPath, strlen(g_oldMsg), g_oldMsg);
    CompareFileString(g_newPath, strlen(g_newMsg), g_newMsg);
    RemoveFile();

    WriteFile(g_oldPath, g_oldMsg);
    if (renameat2(AT_FDCWD, g_oldPath, AT_FDCWD, g_newPath, RENAME_NOREPLACE) == -1) {
        g_error++;
        return;
    }
    if (access(g_oldPath, F_OK) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_newPath, strlen(g_oldMsg), g_oldMsg);
    RemoveFile();
    
    WriteFile(g_oldPath, g_oldMsg);
    WriteFile(g_newPath, g_newMsg);
    if (renameat2(AT_FDCWD, g_oldPath, AT_FDCWD, g_newPath, RENAME_NOREPLACE) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_oldPath, strlen(g_oldMsg), g_oldMsg);
    CompareFileString(g_newPath, strlen(g_newMsg), g_newMsg);
    RemoveFile();
}

static void ZeroTest(int oldFolder, int newFolder)
{
    WriteFile(g_oldPath, g_oldMsg);
    if (renameat2(oldFolder, g_oldName, newFolder, g_newName, RENAME_ZERO) == -1) {
        g_error++;
        return;
    }
    if (access(g_oldPath, F_OK) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_newPath, strlen(g_oldMsg), g_oldMsg);
    RemoveFile();

    WriteFile(g_oldPath, g_oldMsg);
    WriteFile(g_newPath, g_newMsg);
    if (renameat2(oldFolder, g_oldName, newFolder, g_newName, RENAME_ZERO) == -1) {
        g_error++;
        return;
    }
    if (access(g_oldPath, F_OK) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_newPath, strlen(g_oldMsg), g_oldMsg);
    RemoveFile();

    WriteFile(g_oldPath, g_oldMsg);
    if (renameat2(AT_FDCWD, g_oldPath, AT_FDCWD, g_newPath, RENAME_ZERO) == -1) {
        g_error++;
        return;
    }
    if (access(g_oldPath, F_OK) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_newPath, strlen(g_oldMsg), g_oldMsg);
    RemoveFile();

    WriteFile(g_oldPath, g_oldMsg);
    WriteFile(g_newPath, g_newMsg);
    if (renameat2(AT_FDCWD, g_oldPath, AT_FDCWD, g_newPath, RENAME_ZERO) == -1) {
        g_error++;
        return;
    }
    if (access(g_oldPath, F_OK) == 0) {
        g_error++;
        return;
    }
    CompareFileString(g_newPath, strlen(g_oldMsg), g_oldMsg);
    RemoveFile();
}

static void CloseFolder(DIR *dir)
{
    if (closedir(dir)) {
        printf("information: close folder fail\n");
    }
}

int main(void)
{
    DIR *dir = opendir(g_dirPath);
    if (dir == NULL) {
        t_error("%s open dir failed, errno: %d\n", __func__, errno);
        return 1;
    }
    int dirFD = dirfd(dir);
    if (dirFD == -1) {
        t_error("%s open dirfd failed, errno: %d\n", __func__, errno);
        return 1;
    }

    g_error = 0;
    RenameNoReplaceTest(dirFD, dirFD);
    if (g_error) {
        t_error("%s renameat2 failed,flags: RENAME_NOREPLACE\n", __func__);
        RemoveFile();
        CloseFolder(dir);
        return 1;
    }

    g_error = 0;
    ZeroTest(dirFD, dirFD);
    if (g_error) {
        t_error("%s renameat2 failed,flags: zero\n", __func__);
        RemoveFile();
        CloseFolder(dir);
        return 1;
    }
    
    CloseFolder(dir);
    return 0;
}