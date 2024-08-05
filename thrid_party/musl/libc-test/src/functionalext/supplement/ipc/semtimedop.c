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
#include <string.h>
#include <sys/sem.h>

#include "filepath_util.h"

extern int __semtimedop_time64(int, struct sembuf *, size_t, const struct timespec *);

/**
 * @tc.name      : semtimedop_0100
 * @tc.desc      : System V semaphore operations
 * @tc.level     : Level 0
 */
void semtimedop_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd < 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
    }

    char str[] = "hello";
    write(fd, str, sizeof(str));
    close(fd);

    // create a semaphore
    key_t key = ftok(path, 1);
    int id = semget(key, 1, IPC_CREAT | 0666);

    // check semaphore info
    struct semid_ds ds;
    memset(&ds, 0, sizeof(ds));

    semctl(id, 0, IPC_STAT, &ds);
    semctl(id, 0, GETVAL);

    // increment
    struct sembuf ops[] = {{.sem_num = 0, .sem_op = 1, .sem_flg = 0}};
    semop(id, ops, 1);
    semctl(id, 0, GETVAL);

    // test timeouts
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 100};
    ops[0].sem_num = 0;
    ops[0].sem_op = 0;
    ops[0].sem_flg = 0;

    errno = 0;
    int result = semtimedop(id, ops, sizeof(ops) / sizeof(struct sembuf), &ts);
    if (result != -1) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != EAGAIN) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    semctl(id, 0, GETVAL);

    // decrement.
    ops[0].sem_num = 0;
    ops[0].sem_op = -1;
    ops[0].sem_flg = 0;

    semop(id, ops, 1);
    semctl(id, 0, GETVAL);

    // destroy the semaphore
    semctl(id, 0, IPC_RMID);
}

/**
 * @tc.name      : semtimedop_0200
 * @tc.desc      : System V semaphore operations with invalid parameters
 * @tc.level     : Level 2
 */
void semtimedop_0200(void)
{
    errno = 0;
    int result = semtimedop(-1, NULL, -1, NULL);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : semtimedop_time64_0200
 * @tc.desc      : System V semaphore operations with invalid parameters
 * @tc.level     : Level 2
 */
void semtimedop_time64_0200(void)
{
    errno = 0;
    int result = __semtimedop_time64(-1, NULL, -1, NULL);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    // semtimedop_0100();
    // semtimedop_0200();
    // semtimedop_time64_0200();

    return t_status;
}
