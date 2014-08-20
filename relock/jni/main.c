#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "futaux.h"

int mutex = 0;

void *thread(void *arg)
{
    int ret = 0;
    pid_t tid = gettid();

    printf("Entering thread[%d]\n", tid);
    ret = futex_lock_pi(&mutex);
    if (ret) {
        perror("Thread could not aqcuire lock\n");
        goto Wait_forever;
    }
    printf("Mutex acquired (mutex = %d)\n", mutex);

Wait_forever:
    printf("Exiting thread[%d]\n", tid);
    while (1) {
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    pthread_t t;

    printf("pid = %d\n", getpid());
    printf("mutex = %d\n", mutex);
    printf("Acquiring lock\n");
    if (futex_lock_pi(&mutex)) {
        perror("Lock error");
        return -1;
    }
    printf("Mutex acquired (mutex = %d)\n", mutex);
    if (argc > 1) {
        printf("Releasing lock\n");
        mutex = 0;
        printf("mutex = %d\n", mutex);
    }
    if (pthread_create(&t, NULL, thread, NULL)) {
        perror("Could not create thread");
        return -1;
    }

    while (1) {
        sleep(1);
    }
    return 0;
}
