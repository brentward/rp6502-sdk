#include "rp6502.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

void tzset(void)
{
    _tz.daylight = 1;
    _tz.timezone = -8*60*60;
    // _tz.tzname = "PST";
    // _tz.dstname = "PDT";
    _tz.tzname[0] = 'P';
    _tz.tzname[1] = 'S';
    _tz.tzname[2] = 'T';
    _tz.tzname[3] = 0;
    _tz.dstname[0] = 'P';
    _tz.dstname[1] = 'D';
    _tz.dstname[2] = 'T';
    _tz.dstname[3] = 0;
}


int __fastcall__ clock_settime_(const time_t* timep)
{
    int ax;
    RIA_CALL_AXSREG(RIA_OP_SET_RTC, *timep);
    RIA_BLOCK();
    ax = RIA_AX;
    if (ax < 0)
        _seterrno(RIA_ERRNO_LO);
        return -1;
    return 0;
}

int __fastcall__ clock_settime (clockid_t, const struct timespec *tp)
{
    struct tm current_time;
    datetime_t system_time;
    return clock_settime_(&(*tp).tv_sec);
}
