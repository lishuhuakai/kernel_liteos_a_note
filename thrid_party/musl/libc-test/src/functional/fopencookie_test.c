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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "test.h"

#define INIT_BUF_SIZE 4
#define EXPANSION_COEFFICIENT 2

struct cookie
{
    size_t pos;
    size_t len;
    size_t size;
    char *buf;
};

ssize_t mread(void *f, char *buf, size_t len)
{
    struct cookie *c = f;
    if (c->pos > c->len) {
        return 0;
    } else if (c->pos + len > c->len) {
        len = c->len - c->pos;
    }

    memcpy(buf, c->buf + c->pos, len);
    c->pos += len;
    return len;
}

ssize_t mwrite(void *f, const char *buf, size_t len)
{
    struct cookie *c = f;
    char *newBuf;
    if (c->pos + len > c->size) {
        while (c->pos + len > c->size) {
            c->size *= EXPANSION_COEFFICIENT;
        }
        newBuf = malloc(c->size);
        if (newBuf == NULL) {
            return -1;
        } else {
            memcpy(newBuf, c->buf, c->len);
            free(c->buf);
            c->size = c->pos + len;
            c->buf = newBuf;
        }
    }

    memcpy(c->buf + c->pos, buf, len);
    c->pos += len;
    if (c->pos > c->len) {
        c->len = c->pos;
    }
    return len;
}

int mseek(void *f, off_t *off, int whence)
{
    struct cookie *c = f;
    off_t newPos;
    if (whence == SEEK_SET) {
        newPos = *off;
    } else if (whence == SEEK_CUR) {
        newPos = *off + c->pos;
    } else if (whence == SEEK_END) {
        newPos = *off + c->len;
    } else {
        return -1;
    }
    if (newPos < 0) {
        return -1;
    }

    c->pos = newPos;
    *off = newPos;

    return 0;
}

int mclose(void *f)
{
    struct cookie *c = f;
    free(c->buf);
    c->buf = NULL;
    return 0;
}

int main(int argc, char *argv[])
{
    cookie_io_functions_t ioFunc = {.read = mread, .write = mwrite, .seek = mseek, .close = mclose};
    FILE *file;
    struct cookie myc;
    size_t nread;
    size_t memberSize = 1;
    size_t memberNumbers = 2;
    char buf[50];
    myc.buf = malloc(INIT_BUF_SIZE);
    if (myc.buf == NULL) {
        t_error("fopencookie malloc failed\n");
        return 1;
    }
    myc.pos = 0;
    myc.len = 0;
    myc.size = INIT_BUF_SIZE;

    file = fopencookie(&myc, "w+", ioFunc);
    if (file == NULL) {
        t_error("create fopencookie failed\n");
        return 1;
    }
    if (fputs("fopencookie", file) == EOF) {
        t_error("fopencookie fputs failed\n");
        return 1;
    }
    if (fseek(file, 0, SEEK_SET) == -1) {
        t_error("fopencookie fseek failed\n");
        return 1;
    }
    nread = fread(buf, memberSize, memberNumbers, file);
    if (nread == 0) {
        if (ferror(file) != 0) {
            t_error("fopencookie fread failed\n");
            return 1;
        }
    }
    fclose(file);
    return 0;
}