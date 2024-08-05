#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define NAMELEN 16

#define errExitEN(en, msg) \
            do { errno = en; perror(msg); exit(EXIT_FAILURE); \
        } while (0)

static void *threadfunc(void *parm)
{
    sleep(1);          // allow main program to set the thread name
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t thread;
    int rc;
    char thread_name[NAMELEN];
    char set_thread_name[] = "THREADFOO";
    char default_name[] = "pthread_getname";

    rc = pthread_getname_np(pthread_self(), thread_name, NAMELEN-1);
    if (rc != ERANGE) errExitEN(rc, "pthread_getname_np(invalid args[len]) failed");

    rc = pthread_getname_np(pthread_self(), thread_name, NAMELEN);
    if (rc != 0) errExitEN(rc, "pthread_getname_np failed");
    if(strcmp(thread_name, default_name) != 0) perror("pthread name comparison failed");

    rc = pthread_create(&thread, NULL, threadfunc, NULL);
    if (rc != 0) errExitEN(rc, "pthread_create failed");

    rc = pthread_setname_np(thread, (argc > 1) ? argv[1] : set_thread_name);
    if (rc != 0) errExitEN(rc, "pthread_setname_np failed");

    rc = pthread_getname_np(thread, thread_name, (argc > 2) ? atoi(argv[1]) : NAMELEN);
    if (rc != 0) errExitEN(rc, "pthread_getname_np failed");
    if(strcmp(thread_name, set_thread_name) != 0) perror("pthread name comparison failed");

    rc = pthread_join(thread, NULL);
    if (rc != 0) errExitEN(rc, "pthread_join failed");

    exit(EXIT_SUCCESS);
}