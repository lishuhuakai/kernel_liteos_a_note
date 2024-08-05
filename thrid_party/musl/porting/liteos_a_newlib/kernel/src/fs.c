/*
 * Copyright (c) 2022-2022 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "los_config.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef LOSCFG_FS_VFS
int _open(const char *path, int oflag, ...)
{
    return open(path, oflag);
}

int _close(int fd)
{
    return close(fd);
}

ssize_t _read(int fd, void *buf, size_t nbyte)
{
    return read(fd, buf, nbyte);
}

ssize_t _write(int fd, const void *buf, size_t nbyte)
{
    return write(fd, buf, nbyte);
}

int _unlink(const char *path)
{
    return unlink(path);
}

int _fstat(int fd, struct stat *buf)
{
    return fstat(fd, buf);
}

int _stat(const char *path, struct stat *buf)
{
    return stat(path, buf);
}

off_t _lseek(int fd, off_t offset, int whence)
{
    return lseek(fd, offset, whence);
}

int __wrap__fseeko_r(struct _reent *ptr, register FILE *fp, _off_t offset, int whence)
{
    return _lseek(fp, offset, whence);
}

#else /* #ifdef LOSCFG_FS_VFS */

int _open(const char *path, int oflag, ...)
{
    return -1;
}

int _close(int fd)
{
    return -1;
}

ssize_t _read(int fd, void *buf, size_t nbyte)
{
    return -1;
}

ssize_t _write(int fd, const void *buf, size_t nbyte)
{
    return -1;
}

int _unlink(const char *path)
{
    return -1;
}

int _fstat(int fd, struct stat *buf)
{
    return -1;
}

int _stat(const char *path, struct stat *buf)
{
    return -1;
}

off_t _lseek(int fd, off_t offset, int whence)
{
    return -1;
}

int __wrap__fseeko_r(struct _reent *ptr, register FILE *fp, _off_t offset, int whence)
{
    return -1;
}

#endif
