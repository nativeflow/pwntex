#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define USERLOCK_FREE 0
#define USERLOCK_OCCUPIED 1

inline void userlock_wait(volatile const int *userlock)
{
    while (USERLOCK_OCCUPIED == *userlock) {
        usleep(10);
    }
}

inline void userlock_lock(volatile int *userlock)
{
    *userlock = USERLOCK_OCCUPIED;
}

inline void userlock_release(volatile int *userlock)
{
    *userlock = USERLOCK_FREE;
}

int get_voluntary_ctxt_switches(pid_t tid)
{
    FILE *fp;
    char proc_path[256];
    char buf[0x1000];
    char *ptr = buf;
    int count = -1;

    snprintf(proc_path, sizeof(proc_path), "/proc/self/task/%d/status", tid);
    fp = fopen(proc_path, "rb");
    if (fp != NULL) {
        fread(buf, sizeof(unsigned char), sizeof(buf), fp);
        ptr = strstr(buf, "voluntary_ctxt_switches:");
        ptr += strlen("voluntary_ctxt_switches:");
        count = atoi(ptr);
        fclose(fp);
    }

    return count;
}

void wait_for_thread_to_wait_in_kernel(pthread_t tid, int context_switch_count)
{
    while (get_voluntary_ctxt_switches(tid) <= context_switch_count) {
        usleep(10);
    }
}
