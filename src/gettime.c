#include "rp6502.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

time_t rtc(void)
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
        return 0;
    for (i = 0; i < count; i++)
        *ptr++ = RIA_VSTACK;
    current_time.tm_sec = system_time.sec;
    current_time.tm_min = system_time.min;
    current_time.tm_hour = system_time.hour;
    current_time.tm_mday = system_time.day;
    current_time.tm_mon = system_time.month - 1;
    current_time.tm_year = system_time.year - 1900;
    current_time.tm_isdst = -1;

    return mktime(&current_time);
}


int __fastcall__ clock_gettime (clockid_t, struct timespec *tp)
{
    tp->tv_sec = rtc();
    tp->tv_nsec = 0;

    return 0;
}
