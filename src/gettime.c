#include "rp6502.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

int __fastcall__ clock_gettime_(time_t* timep)
{
    RIA_OP = RIA_OP_GET_RTC;
    RIA_BLOCK();
    if (RIA_AXSREG == -1)
        return -1;
    *timep = RIA_AXSREG;
    return 0;
}


int __fastcall__ clock_gettime (clockid_t, struct timespec *tp)
{
    int res;
    res = clock_gettime_(&(tp->tv_sec));
    if (res < 0) {
        _seterrno(RIA_ERRNO_LO);
        return -1;
    }
    tp->tv_nsec = 0;
    return 0;
}
