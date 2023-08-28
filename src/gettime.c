#include "rp6502.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

int rtc(time_t *t)
{
    struct tm current_time;
    datetime_t system_time;
    uint8_t *ptr;
    unsigned i;
    int ax;
    uint8_t count = sizeof(system_time);
    RIA_OP = RIA_OP_GET_RTC;
    RIA_BLOCK();
    ptr = (uint8_t *)&system_time;
    ax = RIA_AX;
    if (ax < 0)
        return -1;
    for (i = 0; i < count; i++)
        *ptr++ = RIA_VSTACK;
    current_time.tm_sec = system_time.sec;
    current_time.tm_min = system_time.min;
    current_time.tm_hour = system_time.hour;
    current_time.tm_mday = system_time.day;
    current_time.tm_mon = system_time.month - 1;
    current_time.tm_year = system_time.year - 1900;
    current_time.tm_isdst = -1;

    *t = mktime(&current_time);
    return 0;
}


int __fastcall__ clock_gettime (clockid_t, struct timespec *tp)
{
    int res;
    tzset();
    // _tz.daylight = 1;
    // _tz.timezone = 8*60*60;
    // _tz.tzname[0] = 'P';
    // _tz.tzname[1] = 'S';
    // _tz.tzname[2] = 'T';
    // _tz.tzname[3] = 0;
    // _tz.dstname[0] = 'P';
    // _tz.dstname[1] = 'D';
    // _tz.dstname[2] = 'T';
    // _tz.dstname[3] = 0;
    res = rtc(&(tp->tv_sec));
    if (res < 0) {
        _seterrno(RIA_ERRNO_LO);
        return -1;
    }
    tp->tv_nsec = 0;
    return 0;
}
