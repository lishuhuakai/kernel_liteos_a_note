/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <langinfo.h>
#include <time.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
/* seconds per hour */
#define __STRPTIME_SECOND_IN_HOUR 3600
/* character-to-number base */
#define __STRPTIME_NUMBER_BASE 10
/* epoch time */
#define __STRPTIME_EPOCH 1900
/* receive data buffer size */
#define __STRPTIME_BUFFER_SIZE 16
/* Width of hours and minutes when formatting %z */
#define __STRPTIME_ZONE_WIDTH 2
/* time base */
#define __STRPTIME_TIME_BASE 60
/* number of weeks per year */
#define __STRPTIME_WEEKS_IN_YEAR 53
/* days of the week */
#define __STRPTIME_DAYS_IN_WEEK 7
/* 12 hour clock */
#define __STRPTIME_HOUR_CLOCK_12 12
/* 24 hour clock */
#define __STRPTIME_HOUR_CLOCK_24 24
/* days per year */
#define __STRPTIME_DAYS_PER_YEAR 366
/* Years in each century */
#define __STRPTIME_YEARS_PER_CENTURY 100

int __getzonename(const char *restrict s, struct tm *restrict tm)
{
    const char *p = s;
    struct tm old;
    memcpy(&old, tm, sizeof(struct tm));
    /* Possible time zone names like +XXX or -XXX */
    if (*p == '+' || *p == '-') {
        p++;
    }

    /* The time zone name is adjacent to the offset second data,
     * and the following symbol belongs to the offset second */
    while (*p && (*p != '+' && *p != '-' && *p != ' ')) {
        p++;
    }

    /* In the structure struct tm, tm_zone is declared as const char * type, so use static */
    static char buf[__STRPTIME_BUFFER_SIZE] = {0};
    memset(buf, 0x0, sizeof(buf));
    int len = p - s;
    memcpy(buf, s, len);
    tm->__tm_zone = buf;

    /* Re-fetch local data, extract tm_isdst flag. */
    time_t t = mktime(&old);
    struct tm *tmp = localtime(&t);
    if (tmp) {
        tm->tm_isdst = tmp->tm_isdst;
    }
    return len;
}

int __getgmtoff(const char *restrict s, struct tm *restrict tm)
{
    const char *p = s;
    int sign = 1;
    int i;
    int isexit = 0;
    long m = 0;
    long h = 0;

    /* The possible formats for time offset are HHMM(-HHMM) or HH:MM(-HH:MM) */
    if (*p == '-') {
        sign = -1;
    }
    p++;
    tm->__tm_gmtoff = 0;

    /* get hours */
    for (i=0; i<__STRPTIME_ZONE_WIDTH && *p; i++, p++) {
        if (isdigit(*p)) {
            h = h * __STRPTIME_NUMBER_BASE + (*p - 0x30);
        } else {
            p--;
            isexit = 1;
            break;
        }
    }

    if (!isexit) {
        /* Possible time zone formats are HH:MM. */
        if (*p == ':') {
            *p++;
        }

        /* get minutes */
        for (i=0; i<__STRPTIME_ZONE_WIDTH && *p; i++, p++) {
            if (isdigit(*p)) {
                m = m * __STRPTIME_NUMBER_BASE + (*p - 0x30);
            } else {
                p--;
                isexit = 1;
                break;
            }
        }
    }

    /* Convert hours and minutes to seconds */
    tm->__tm_gmtoff = sign * (h * __STRPTIME_SECOND_IN_HOUR + m * __STRPTIME_TIME_BASE);

    return p - s;
}

char *strptime(const char *restrict s, const char *restrict f, struct tm *restrict tm)
{
    int i, w, neg, adj, min, range, *dest, dummy;
    const char *ex;
    size_t len;
    int want_century = 0, century = 0, relyear = 0;
    while (*f) {
        if (*f != '%') {
            if (isspace(*f)) {
                for (; *s && isspace(*s); s++);
            } else if (*s != *f) {
                return 0;
            } else {
                s++;
            }
            f++;
            continue;
        }
        f++;
        if (*f == '+') {
            f++;
        }
        if (isdigit(*f)) {
            char *new_f;
            w=strtoul(f, &new_f, __STRPTIME_NUMBER_BASE);
            f = new_f;
        } else {
            w=-1;
        }
        adj=0;
        switch (*f++) {
            case 'a': case 'A':
                dest = &tm->tm_wday;
                min = ABDAY_1;
                range = __STRPTIME_DAYS_IN_WEEK;
                goto symbolic_range;
            case 'b': case 'B': case 'h':
                dest = &tm->tm_mon;
                min = ABMON_1;
                range = __STRPTIME_HOUR_CLOCK_12;
                goto symbolic_range;
            case 'c':
                s = strptime(s, nl_langinfo(D_T_FMT), tm);
                if (!s) {
                    return 0;
                }
                break;
            case 'C':
                dest = &century;
                if (w<0) {
                    w=__STRPTIME_ZONE_WIDTH;
                }
                want_century |= __STRPTIME_ZONE_WIDTH;
                goto numeric_digits;
            case 'd': case 'e':
                dest = &tm->tm_mday;
                min = 1;
                range = 31;
                goto numeric_range;
            case 'D':
                s = strptime(s, "%m/%d/%y", tm);
                if (!s) {
                    return 0;
                }
                break;
            case 'F':
                s = strptime(s, "%Y-%m-%d", tm);
                if (!s) {
                    return 0;
                }
                break;
            case 'g':
                dest = &tm->tm_year;
                min = 0;
                range = 99;
                w = __STRPTIME_ZONE_WIDTH;
                want_century = 0;
                goto numeric_digits;
            case 'G':
                do {
                    ++s;
                } while (isdigit(*s));
                continue;
            case 'k':
            case 'H':
                dest = &tm->tm_hour;
                min = 0;
                range = __STRPTIME_HOUR_CLOCK_24;
                goto numeric_range;
            case 'l':
            case 'I':
                dest = &tm->tm_hour;
                min = 1;
                range = __STRPTIME_HOUR_CLOCK_12;
                goto numeric_range;
            case 'j':
                dest = &tm->tm_yday;
                min = 1;
                range = __STRPTIME_DAYS_PER_YEAR;
                adj = 1;
                goto numeric_range;
            case 'm':
                dest = &tm->tm_mon;
                min = 1;
                range = __STRPTIME_HOUR_CLOCK_12;
                adj = 1;
                goto numeric_range;
            case 'M':
                dest = &tm->tm_min;
                min = 0;
                range = __STRPTIME_TIME_BASE;
                goto numeric_range;
            case 'n': case 't':
                for (; *s && isspace(*s); s++) {}
                break;
            case 'p':
            case 'P':
                ex = nl_langinfo(AM_STR);
                len = strlen(ex);
                if (!strncasecmp(s, ex, len)) {
                    tm->tm_hour %= __STRPTIME_HOUR_CLOCK_12;
                    s += len;
                    break;
                }
                ex = nl_langinfo(PM_STR);
                len = strlen(ex);
                if (!strncasecmp(s, ex, len)) {
                    tm->tm_hour %= __STRPTIME_HOUR_CLOCK_12;
                    tm->tm_hour += __STRPTIME_HOUR_CLOCK_12;
                    s += len;
                    break;
                }
                return 0;
            case 'r':
                s = strptime(s, nl_langinfo(T_FMT_AMPM), tm);
                if (!s) {
                    return 0;
                }
                break;
            case 'R':
                s = strptime(s, "%H:%M", tm);
                if (!s) {
                    return 0;
                }
                break;
            case 's': {
                time_t secs = 0;
                if (!isdigit(*s)) {
                    return 0;
                }
                do {
                    secs *= __STRPTIME_NUMBER_BASE;
                    secs += *s - '0';
                    s++;
                } while (isdigit(*s));
                if (localtime_r(&secs, tm) == NULL) {
                    return 0;
                }
                break;
            }
            case 'S':
                dest = &tm->tm_sec;
                min = 0;
                range = 61;
                goto numeric_range;
            case 'T':
                s = strptime(s, "%H:%M:%S", tm);
                if (!s) {
                    return 0;
                }
                break;
            case 'u': {
                if (!isdigit(*s)) {
                    return 0;
                }
                int wday = 0;
                int rulim = __STRPTIME_DAYS_IN_WEEK;
                do {
                    wday *= __STRPTIME_NUMBER_BASE;
                    wday += *s++ - '0';
                    rulim /= __STRPTIME_NUMBER_BASE;
                } while ((wday * __STRPTIME_NUMBER_BASE < __STRPTIME_DAYS_IN_WEEK) && rulim && isdigit(*s));
                if (wday < 1 || wday > __STRPTIME_DAYS_IN_WEEK) {
                    return 0;
                }
                tm->tm_wday = wday % __STRPTIME_DAYS_IN_WEEK;
                continue;
            }
            case 'U':
            case 'W':
                dest = &dummy;
                min = 0;
                range = __STRPTIME_WEEKS_IN_YEAR + 1;
                goto numeric_range;
            case 'w':
                dest = &tm->tm_wday;
                min = 0;
                range = __STRPTIME_DAYS_IN_WEEK;
                goto numeric_range;
            case 'v':
                if (!(s = strptime(s, "%e-%b-%Y", tm))) {
                    return 0;
                }
                break;
            case 'V': {
                int r = 0;
                int rulim = __STRPTIME_WEEKS_IN_YEAR;
                if (!isdigit(*s)) {
                    return 0;
                }
                do {
                    r *= __STRPTIME_NUMBER_BASE;
                    r += *s++ - '0';
                    rulim /= __STRPTIME_NUMBER_BASE;
                } while ((r * __STRPTIME_NUMBER_BASE < __STRPTIME_WEEKS_IN_YEAR) && rulim && isdigit(*s));
                if (r < 0 || r > __STRPTIME_WEEKS_IN_YEAR) {
                    return 0;
                }
                continue;
            }
            case 'x':
                s = strptime(s, nl_langinfo(D_FMT), tm);
                if (!s) {
                    return 0;
                }
                break;
            case 'X':
                s = strptime(s, nl_langinfo(T_FMT), tm);
                if (!s) {
                    return 0;
                }
                break;
            case 'y':
                dest = &relyear;
                w = __STRPTIME_ZONE_WIDTH;
                want_century |= 1;
                goto numeric_digits;
            case 'Y':
                dest = &tm->tm_year;
                if (w<0) {
                    w=4;
                }
                adj = __STRPTIME_EPOCH;
                want_century = 0;
                goto numeric_digits;
            case 'Z':
                tzset();
                s += __getzonename((const char *)s, tm);
                continue;
            case 'z':
                s += __getgmtoff((const char *)s, tm);
                continue;
            case '%':
                if (*s++ != '%') {
                    return 0;
                }
                break;
            default:
                return 0;
                numeric_range:
                if (!isdigit(*s)) {
                    return 0;
                }
                *dest = 0;
                for (i=1; i<=min+range && isdigit(*s); i*=__STRPTIME_NUMBER_BASE) {
                    *dest = *dest * __STRPTIME_NUMBER_BASE + *s++ - '0';
                }
                if (*dest - min >= (unsigned)range) {
                    return 0;
                }
                *dest -= adj;
                switch ((char *)dest - (char *)tm) {
                    case offsetof(struct tm, tm_yday):
                        ;
                }
                goto update;
                numeric_digits:
                neg = 0;
                if (*s == '+') {
                    s++;
                } else if (*s == '-') {
                    neg=1, s++;
                }
                if (!isdigit(*s)) {
                    return 0;
                }
                for (*dest=i=0; i<w && isdigit(*s); i++) {
                    *dest = *dest * __STRPTIME_NUMBER_BASE + *s++ - '0';
                }
                if (neg) {
                    *dest = -*dest;
                }
                *dest -= adj;
                goto update;
                symbolic_range:
                for (i=__STRPTIME_ZONE_WIDTH*range-1; i>=0; i--) {
                    ex = nl_langinfo(min+i);
                    len = strlen(ex);
                    if (strncasecmp(s, ex, len)) continue;
                    s += len;
                    *dest = i % range;
                    break;
                }
                if (i<0) {
                    return 0;
                }
                goto update;
            update:
                ;
            }
        }
    if (want_century) {
        tm->tm_year = relyear;
        if (want_century & __STRPTIME_ZONE_WIDTH) {
            tm->tm_year += century * __STRPTIME_YEARS_PER_CENTURY - __STRPTIME_EPOCH;
        }
        else if (tm->tm_year <= 68) tm->tm_year += __STRPTIME_YEARS_PER_CENTURY;
    }
    return (char *)s;
}