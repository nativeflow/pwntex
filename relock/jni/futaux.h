#include <linux/futex.h>
#include <sys/syscall.h>

#define FUTEX_WAIT_REQUEUE_PI 11
#define FUTEX_CMP_REQUEUE_PI 12

inline int futex_lock_pi(int *uaddr)
{
    return syscall(__NR_futex, uaddr, FUTEX_LOCK_PI, 1, NULL, NULL, 0);
}

inline int futex_wait_requeue_pi(int *uaddr1, int *uaddr2)
{
    return syscall(__NR_futex, uaddr1, FUTEX_WAIT_REQUEUE_PI, 0, NULL, uaddr2, 0);
}

inline int futex_requeue_pi(int *uaddr1, int *uaddr2, int cmpval)
{
    return syscall(__NR_futex, uaddr1, FUTEX_CMP_REQUEUE_PI, 1, NULL, uaddr2, cmpval);
}
