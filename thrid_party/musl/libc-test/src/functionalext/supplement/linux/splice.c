/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "filepath_util.h"

const char *path = "/proc/version";

/**
 * @tc.name      : splice_0100
 * @tc.desc      : splice 0 data to/from a pipe
 * @tc.level     : Level 0
 */
void splice_0100(void)
{
    char fromfile[PATH_MAX] = {0};
    char tofile[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_FROM_TXT, fromfile);
    FILE_ABSOLUTE_PATH(STR_FILE_TO_TXT, tofile);

    int fromfd = open(fromfile, O_RDWR | O_CREAT, TEST_MODE);
    if (fromfd < 0) {
        t_error("%s failed: fromfd = %d\n", __func__, fromfd);
    }

    int tofd = open(tofile, O_RDWR | O_CREAT, TEST_MODE);
    if (tofd < 0) {
        t_error("%s failed: tofd = %d\n", __func__, tofd);
    }

    off_t off = 0;
    size_t len = 0;
    int flags = 0;

    errno = 0;
    ssize_t result = splice(fromfd, &off, tofd, &off, len, flags);
    if (result != 0) {
        t_error("%s failed: result = %ld\n", __func__, result);
    }

    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    remove(fromfile);
    remove(tofile);
}

/**
 * @tc.name      : splice_0200
 * @tc.desc      : splice data to/from a pipe
 * @tc.level     : Level 1
 */
void splice_0200(void)
{
    char buf[BUFSIZ] = {0};
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        t_error("%s failed: fopen\n", __func__);
        return;
    }

    fgets(buf, sizeof(buf), f);
    fclose(f);

    int fromfd = open(path, O_RDONLY);
    if (fromfd < 0) {
        t_error("%s failed: fromfd = %d\n", __func__, fromfd);
    }

    int pipe1[1 + 1];
    int result = pipe(pipe1);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    size_t len = BUFSIZ;
    int flags = SPLICE_F_MORE | SPLICE_F_MOVE;

    ssize_t bytes = splice(fromfd, NULL, pipe1[1], NULL, len, flags);
    if (bytes <= 0) {
        t_error("%s failed: bytes = %ld\n", __func__, bytes);
        return;
    }

    close(pipe1[1]);

    char buf2[BUFSIZ] = {0};
    f = fdopen(pipe1[0], "r");
    if (f == NULL) {
        t_error("%s failed: fdopen\n", __func__);
        return;
    }

    fgets(buf2, sizeof(buf2), f);
    fclose(f);

    if (strcmp(buf2, buf)) {
        t_error("%s failed: buf2 = %s\n", __func__, buf2);
        return;
    }
}

/**
 * @tc.name      : splice_0300
 * @tc.desc      : splice data to/from a pipe with invalid parameters
 * @tc.level     : Level 2
 */
void splice_0300(void)
{
    errno = 0;
    ssize_t bytes = splice(-1, NULL, -1, NULL, -1, -1);
    if (bytes >= 0) {
        t_error("%s failed: bytes = %ld\n", __func__, bytes);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %ld\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    splice_0100();
    splice_0200();
    splice_0300();

    return t_status;
}
