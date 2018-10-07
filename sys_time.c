/**
 * File:   sys_time.c
 * Author: Ericson Joseph
 *
 * Created on September 30, 2018, 11:20 PM
 */

#include "sys_time.h"
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>

char formatTime[26];

long long int millis(){
    struct timeb timer_msec;
    long long int timestamp_msec; /* timestamp in millisecond. */
    if (!ftime(&timer_msec)) {
        timestamp_msec = ((long long int) timer_msec.time) * 1000ll +
            (long long int) timer_msec.millitm;
    } else {
        timestamp_msec = -1;
    }
    return timestamp_msec;
}


char * getFormatTime(){
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(formatTime, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return formatTime;
}


