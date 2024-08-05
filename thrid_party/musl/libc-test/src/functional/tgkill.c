#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/tgkill.h>
#include "test.h"

#define ARGV_1  12345
#define ARGV_2  34567
int main(){

    int tgid, tid;
    tgid = ARGV_1;
    tid = ARGV_2;

    if (tgkill(tgid, tid, SIGPROF) == -1 && errno != ESRCH) {
        perror("tgkill failed");
    }

    return 0;
}