#include "util.h"
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

static void Bm_function_Poll(benchmark::State &state)
{
    struct pollfd pfd;
    int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open poll");
        exit(-1);
    }

    pfd.fd = fd;
    pfd.events = POLLIN;

    for (auto _ : state) {
        benchmark::DoNotOptimize(poll(&pfd, 1, 0));
    }

    close(fd);
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Poll);