#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "futaux.h"
#include "userlock.h"

int A = 0, B = 0;
volatile int invoke_futex_wait_requeue_pi = 0;
volatile pid_t thread_tid = -1;

void *thread(void *arg)
{
    thread_tid = gettid();
    printf("[2]\n");
    userlock_wait(&invoke_futex_wait_requeue_pi);
    futex_wait_requeue_pi(&A, &B);
    printf("Someone woke me up\n");
    while (1) {
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    pthread_t t;
    int context_switch_count = 0;

    printf("[1]\n");
    futex_lock_pi(&B);

    userlock_lock(&invoke_futex_wait_requeue_pi);

    pthread_create(&t, NULL, thread, NULL);

    /* Wait for the thread to be in a system call */
    while (thread_tid < 0) {
        usleep(10);
    }
    context_switch_count = get_voluntary_ctxt_switches(thread_tid);
    userlock_release(&invoke_futex_wait_requeue_pi);
    wait_for_thread_to_wait_in_kernel(thread_tid, context_switch_count);

    printf("[3]\n");
    futex_requeue_pi(&A, &B, A);

    printf("[4]\n");
    B = 0;

    printf("[5]\n");
    futex_requeue_pi(&B, &B, B);

    while (1) {
        sleep(1);
    }
    return 0;
}
