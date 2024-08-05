#include <pthread.h>
#include <string.h>
#include "test.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define TEST(c, ...) ((c) ? 1 : (t_error(#c" failed: " __VA_ARGS__),0))

void* pthread_test(void* arg)
{
    *((pid_t *)arg) = gettid();
    return NULL;
}

int main(int argc, char const *argv[])
{
    TEST(gettid() == pthread_gettid_np(pthread_self()),"pthread_gettid_np() is failed\n");

    pid_t tid;
    pthread_t t;
    pthread_create(&t,NULL,pthread_test,&tid);
    pid_t recv_result = pthread_gettid_np(t);
    TEST(0 == pthread_join(t,NULL),"pthread_join is failed\n");

    TEST(tid == recv_result,"the tid of pthread or tid of pthread_gettid_np() is wrong\n");
    return 0;
}