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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <langinfo.h>
#include <locale.h>
#include <time.h>
#include <limits.h>
#include "locale_impl.h"
#include "time_impl.h"

#define __YEAR_BASE__ 1900
#define __WEEKS_IN_YEAR__ 52
#define __WEEKS_IN_YEAR2__ 53
#define __DAY_LAST_WEEK__ 4
#define __DAY_LAST_WEEK2__ 3
#define __YEARS_PER_CENTURY__ 100
#define __DAYS_IN_YEAR__ 360
#define __LEN_YEAR__ 4
#define __HALF_HOUR__ 12
#define __LEN_DAY__ 3

static int is_leap(int y)
{
    /* Avoid overflow */
    if (y > INT_MAX - __YEAR_BASE__) {
        y -= 2000;
    }
    y += __YEAR_BASE__;
    return !(y%4) && ((y%100) || !(y%400));
}

static int week_num(const struct tm *tm)
{
    int val = (tm->tm_yday + 7U - (tm->tm_wday+6U)%7) / 7;
    /* If 1 Jan is just 1-3 days past Monday,
     * the previous week is also in this year. */
    if ((tm->tm_wday + 371U - tm->tm_yday - 2) % 7 <= 2)
        val++;
    if (!val) {
        val = __WEEKS_IN_YEAR__;
        /* If 31 December of prev year a Thursday,
         * or Friday of a leap year, then the
         * prev year has 53 weeks. */
        int dec31 = (tm->tm_wday + 7U - tm->tm_yday - 1) % 7;
        if (dec31 == 4 || (dec31 == 5 && is_leap(tm->tm_year%400-1)))
            val++;
    } else if (val == __WEEKS_IN_YEAR2__) {
        /* If 1 January is not a Thursday, and not
         * a Wednesday of a leap year, then this
         * year has only 52 weeks. */
        int jan1 = (tm->tm_wday + 371U - tm->tm_yday) % 7;
        if (jan1 != __DAY_LAST_WEEK__ && (jan1 != 3 || !is_leap(tm->tm_year)))
            val = 1;
    }
    return val;
}

const char *__strftime_fmt_1(char (*s)[100], size_t *l, int f, const struct tm *tm, locale_t loc, int pad)
{
    nl_item item;
    long long val;
    const char *fmt = "-";
    int width = 2, def_pad = '0';

    switch (f) {
        case 'a':
            if (tm->tm_wday > 6U) goto string;
            item = ABDAY_1 + tm->tm_wday;
            goto nl_strcat;
        case 'A':
            if (tm->tm_wday > 6U) goto string;
            item = DAY_1 + tm->tm_wday;
            goto nl_strcat;
        case 'h':
        case 'b':
            if (tm->tm_mon > 11U) {
                goto string;
            }
            item = ABMON_1 + tm->tm_mon;
            goto nl_strcat;
        case 'B':
            if (tm->tm_mon > 11U) {
                goto string;
            }
            item = MON_1 + tm->tm_mon;
            goto nl_strcat;
        case 'c':
            item = D_T_FMT;
            goto nl_strftime;
        case 'C':
            val = (1900LL+tm->tm_year) / __YEARS_PER_CENTURY__;
            goto number;
        case 'e':
            def_pad = '_';
        case 'd':
            val = tm->tm_mday;
            goto number;
        case 'D':
            fmt = "%m/%d/%y";
            goto recu_strftime;
        case 'F':
            fmt = "%Y-%m-%d";
            goto recu_strftime;
        case 'g':
        case 'G':
            val = tm->tm_year + 1900LL;
            if (tm->tm_yday < __DAY_LAST_WEEK2__ && week_num(tm) != 1) {
                val--;
            } else if (tm->tm_yday > __DAYS_IN_YEAR__ && week_num(tm) == 1) {
                val++;
            }
            if (f=='g') {
                val %= __YEARS_PER_CENTURY__;
            } else {
                width = __LEN_YEAR__;
            }
            goto number;
        case 'H':
            val = tm->tm_hour;
            goto number;
        case 'I':
            val = tm->tm_hour;
            if (!val) {
                val = __HALF_HOUR__;
            } else if (val > __HALF_HOUR__) {
                val -= __HALF_HOUR__;
            }
            goto number;
        case 'j':
            val = tm->tm_yday+1;
            width = __LEN_DAY__;
            goto number;
        case 'k':
            def_pad = '_';
            val = tm->tm_hour;
            goto number;
        case 'l':
            def_pad = '_';
            val = tm->tm_hour;
            if (!val) {
                val = __HALF_HOUR__;
            } else if (val > __HALF_HOUR__) {
                val -= __HALF_HOUR__;
            }
            goto number;
        case 'm':
            val = tm->tm_mon+1;
            goto number;
        case 'M':
            val = tm->tm_min;
            goto number;
        case 'n':
            *l = 1;
            return "\n";
        case 'p':
            item = tm->tm_hour >= __HALF_HOUR__ ? PM_STR : AM_STR;
            goto nl_strcat;
        case 'P':
            item = tm->tm_hour >= __HALF_HOUR__ ? PM_STR_LOWER : AM_STR_LOWER;
            goto nl_strcat;
        case 'r':
            item = T_FMT_AMPM;
            goto nl_strftime;
        case 'R':
            fmt = "%H:%M";
            goto recu_strftime;
        case 's':
            val = __tm_to_secs(tm) - tm->__tm_gmtoff;
            width = 1;
            goto number;
        case 'S':
            val = tm->tm_sec;
            goto number;
        case 't':
            *l = 1;
            return "\t";
        case 'T':
            fmt = "%H:%M:%S";
            goto recu_strftime;
        case 'u':
            val = tm->tm_wday ? tm->tm_wday : 7;
            width = 1;
            goto number;
        case 'U':
            val = (tm->tm_yday + 7U - tm->tm_wday) / 7;
            goto number;
        case 'W':
            val = (tm->tm_yday + 7U - (tm->tm_wday+6U)%7) / 7;
            goto number;
        case 'V':
            val = week_num(tm);
            goto number;
        case 'v':
            fmt = "%e-%b-%Y";
            goto recu_strftime;
        case 'w':
            val = tm->tm_wday;
            width = 1;
            goto number;
        case 'x':
            item = D_FMT;
            goto nl_strftime;
        case 'X':
            item = T_FMT;
            goto nl_strftime;
        case 'y':
            val = (tm->tm_year + 1900LL) % 100;
            if (val < 0) {
                val = -val;
            }
            goto number;
        case 'Y':
            val = tm->tm_year + 1900LL;
            if (val >= 10000) {
                *l = snprintf(*s, sizeof *s, "+%lld", val);
                return *s;
            }
            width = __LEN_YEAR__;
            goto number;
        case 'z':
            if (tm->tm_isdst < 0) {
                *l = 0;
                return "";
            }
            *l = snprintf(*s, sizeof *s, "%+.4ld",
                tm->__tm_gmtoff/3600*100 + tm->__tm_gmtoff%3600/60);
            return *s;
        case 'Z':
            if (tm->tm_isdst < 0) {
                *l = 0;
                return "";
            }
            fmt = __tm_to_tzname(tm);
            goto string;
        case '%':
            *l = 1;
            return "%";
        default:
            return 0;
    }
number:
    switch (pad ? pad : def_pad) {
        case '-':
            *l = snprintf(*s, sizeof *s, "%lld", val);
            break;
        case '_':
            *l = snprintf(*s, sizeof *s, "%*lld", width, val);
            break;
        case '0':
        default:
            *l = snprintf(*s, sizeof *s, "%0*lld", width, val);
            break;
    }
    return *s;
nl_strcat:
    fmt = __nl_langinfo_l(item, loc);
string:
    *l = strlen(fmt);
    return fmt;
nl_strftime:
    fmt = __nl_langinfo_l(item, loc);
recu_strftime:
    *l = __strftime_l(*s, sizeof *s, fmt, tm, loc);
    if (!*l) {
        return 0;
    }
    return *s;
}

size_t __strftime_l(char *restrict s, size_t n, const char *restrict f, const struct tm *restrict tm, locale_t loc)
{
    size_t l, k;
    char buf[100];
    char *p;
    const char *t;
    int pad, plus;
    unsigned long width;
    for (l=0; l<n; f++) {
        if (!*f) {
            s[l] = 0;
            return l;
        }
        if (*f != '%') {
            s[l++] = *f;
            continue;
        }
        f++;
        pad = 0;
        if (*f == '-' || *f == '_' || *f == '0') {
            pad = *f++;
        }
        if ((plus = (*f == '+'))) {
            f++;
        }
        width = strtoul(f, &p, 10);
        if (*p == 'C' || *p == 'F' || *p == 'G' || *p == 'Y') {
            if (!width && p!=f) {
                width = 1;
            }
        } else {
            width = 0;
        }
        f = p;
        if (*f == 'E' || *f == 'O') {
            f++;
        }
        t = __strftime_fmt_1(&buf, &k, *f, tm, loc, pad);
        if (!t) {
            break;
        }
        if (width) {
            /* Trim off any sign and leading zeros, then
             * count remaining digits to determine behavior
             * for the + flag. */
            if (*t=='+' || *t=='-') {
                t++, k--;
            }
            for (; *t=='0' && t[1]-'0'<10U; t++, k--) {}
            if (width < k) {
                width = k;
            }
            size_t d;
            for (d=0; t[d]-'0'<10U; d++) {}
            if (tm->tm_year < -1900) {
                s[l++] = '-';
                width--;
            } else if (plus && d+(width-k) >= (*p=='C'?3:5)) {
                s[l++] = '+';
                width--;
            }
            for (; width > k && l < n; width--) {
                s[l++] = '0';
            }
        }
        if (k > n-l) {
            k = n-l;
        }
        memcpy(s+l, t, k);
        l += k;
    }
    if (n) {
        if (l==n) {
            l=n-1;
        }
        s[l] = 0;
    }
    return 0;
}

size_t strftime(char *restrict s, size_t n, const char *restrict f, const struct tm *restrict tm)
{
    return __strftime_l(s, n, f, tm, CURRENT_LOCALE);
}

weak_alias(__strftime_l, strftime_l);