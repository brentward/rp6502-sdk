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


int set_rtc(const datetime_t *datetime)
{
    const uint8_t *ptr;
    uint8_t i;
    int ax;
    uint8_t count = sizeof(*datetime);
    ptr = ((uint8_t *)datetime) + count - 1;
    for (i = 0;i < count; ++i)
        RIA_VSTACK = (uint8_t)(uint8_t *)*ptr--;
    RIA_CALL(RIA_OP_SET_RTC);
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
    tzset();
    current_time = *gmtime(&(*tp).tv_sec);
    system_time.sec = current_time.tm_sec;
    system_time.min = current_time.tm_min;
    system_time.hour= current_time.tm_hour;
    system_time.day = current_time.tm_mday;
    system_time.month = current_time.tm_mon + 1;
    system_time.year = current_time.tm_year + 1900;
    system_time.dotw = current_time.tm_wday;

    return set_rtc(&system_time);

}
