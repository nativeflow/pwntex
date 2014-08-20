#define USERLOCK_FREE 0
#define USERLOCK_OCCUPIED 1

inline void userlock_wait(volatile const int *userlock)
{
    while (USERLOCK_OCCUPIED == *userlock);
}

inline void userlock_lock(volatile int *userlock)
{
    *userlock = USERLOCK_OCCUPIED;
}

inline void userlock_release(volatile int *userlock)
{
    *userlock = USERLOCK_FREE;
}
