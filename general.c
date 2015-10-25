#include "general.h"

int nanosleep(const struct timespec *req, struct timespec *rem);

int az_fork(void (*child_fun)())
{
     pid_t child_pid;
     switch (child_pid = fork()) {
         case -1:
             errExit("failed to fork");
         case 0:
             (*child_fun)();
             _exit(0);
         default:
             return child_pid;
     }
}

int az_fork2(void (*child_fun)(void*), void *arguments)
{
     pid_t child_pid;
     switch (child_pid = fork()) {
         case -1:
             errExit("failed to fork");
         case 0:
             (*child_fun)(arguments);
             _exit(0);
         default:
             return child_pid;
     }
}


int az_fork_slp(void (*child_fun)(void*), int msecs, void *arguments)
{
     pid_t child_pid;
     switch (child_pid = fork()) {
         case -1:
             errExit("failed to fork");
         case 0:
             sleep_ms(msecs);
             (*child_fun)(arguments);
             _exit(0);
         default:
             return child_pid;
     }
}

void sleep_ms (int msecs)
{
#if (defined (WIN32))
    Sleep (msecs);
#else
    struct timespec t;
    t.tv_sec  =  msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
#endif
}

int64_t clock_ms()
{
#if (defined (WIN32))
    SYSTEMTIME st;
    GetSystemTime (&st);
    return (int64_t) st.wSecond * 1000 + st.wMilliseconds;
#else
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}
