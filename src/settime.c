#include "rp6502.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

int __fastcall__ clock_settime_(const time_t* timep)
{
    int ax;
    RIA_CALL_AXSREG(RIA_OP_SET_RTC, *timep);
    RIA_BLOCK();
    ax = RIA_AX;
    if (ax < 0)
        return _mappederrno(RIA_ERRNO_LO);
    return 0;
}

int __fastcall__ clock_settime(clockid_t, const struct timespec *tp)
{
    struct tm current_time;
    datetime_t system_time;
    return clock_settime_(&(*tp).tv_sec);
}
