#include <stdlib.h>
#include <langinfo.h>
#include <time.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>

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
    static char buf[16] = {0};
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
    for (i = 0; i < 2 && *p; i++, p++) {
        if (isdigit(*p)) {
            h = h * 10 + (*p - 0x30);
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
        for (i = 0; i < 2 && *p; i++, p++) {
            if (isdigit(*p)) {
                m = m * 10 + (*p - 0x30);
            } else {
                p--;
                isexit = 1;
                break;
            }
        }
    }

    /* Convert hours and minutes to seconds */
    tm->__tm_gmtoff = sign * (h * 3600 + m * 60);

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
			if (isspace(*f)) for (; *s && isspace(*s); s++);
			else if (*s != *f) return 0;
			else s++;
			f++;
			continue;
		}
		f++;
		if (*f == '+') f++;
		if (isdigit(*f)) {
			char *new_f;
			w=strtoul(f, &new_f, 10);
			f = new_f;
		} else {
			w=-1;
		}
		adj=0;
		switch (*f++) {
		case 'a': case 'A':
			dest = &tm->tm_wday;
			min = ABDAY_1;
			range = 7;
			goto symbolic_range;
		case 'b': case 'B': case 'h':
			dest = &tm->tm_mon;
			min = ABMON_1;
			range = 12;
			goto symbolic_range;
		case 'c':
			s = strptime(s, nl_langinfo(D_T_FMT), tm);
			if (!s) return 0;
			break;
		case 'C':
			dest = &century;
			if (w<0) w=2;
			want_century |= 2;
			goto numeric_digits;
		case 'd': case 'e':
			dest = &tm->tm_mday;
			min = 1;
			range = 31;
			goto numeric_range;
		case 'D':
			s = strptime(s, "%m/%d/%y", tm);
			if (!s) return 0;
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
			w = 2;
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
			range = 24;
			goto numeric_range;
		case 'l':
		case 'I':
			dest = &tm->tm_hour;
			min = 1;
			range = 12;
			goto numeric_range;
		case 'j':
			dest = &tm->tm_yday;
			min = 1;
			range = 366;
			adj = 1;
			goto numeric_range;
		case 'm':
			dest = &tm->tm_mon;
			min = 1;
			range = 12;
			adj = 1;
			goto numeric_range;
		case 'M':
			dest = &tm->tm_min;
			min = 0;
			range = 60;
			goto numeric_range;
		case 'n': case 't':
			for (; *s && isspace(*s); s++);
			break;
		case 'p':
		case 'P':
			ex = nl_langinfo(AM_STR);
			len = strlen(ex);
			if (!strncasecmp(s, ex, len)) {
				tm->tm_hour %= 12;
				s += len;
				break;
			}
			ex = nl_langinfo(PM_STR);
			len = strlen(ex);
			if (!strncasecmp(s, ex, len)) {
				tm->tm_hour %= 12;
				tm->tm_hour += 12;
				s += len;
				break;
			}
			return 0;
		case 'r':
			s = strptime(s, nl_langinfo(T_FMT_AMPM), tm);
			if (!s) return 0;
			break;
		case 'R':
			s = strptime(s, "%H:%M", tm);
			if (!s) return 0;
			break;
		case 's': {
			time_t secs = 0;
			if (!isdigit(*s)) {
				return 0;
			}
			do {
				secs *= 10;
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
			if (!s) return 0;
			break;
		case 'u': {
			if (!isdigit(*s)) {
				return 0;
			}
			int wday = 0;
			int rulim = 7;
			do {
				wday *= 10;
				wday += *s++ - '0';
				rulim /= 10;
			} while ((wday * 10 < 7) && rulim && isdigit(*s));
			if (wday < 1 || wday > 7) {
				return 0;
			}
			tm->tm_wday = wday % 7;
			continue;
		}
		case 'U':
		case 'W':
			/* Throw away result, for now. (FIXME?) */
			dest = &dummy;
			min = 0;
			range = 54;
			goto numeric_range;
		case 'w':
			dest = &tm->tm_wday;
			min = 0;
			range = 7;
			goto numeric_range;
		case 'v':
			if (!(s = strptime(s, "%e-%b-%Y", tm))) {
				return 0;
			}
			break;
		case 'V': {
			int r = 0;
			int rulim = 53;
			if (!isdigit(*s)) {
				return 0;
			}
			do {
				r *= 10;
				r += *s++ - '0';
				rulim /= 10;
			} while ((r * 10 < 53) && rulim && isdigit(*s));
			if (r < 0 || r > 53) {
				return 0;
			}
			continue;
		}
		case 'x':
			s = strptime(s, nl_langinfo(D_FMT), tm);
			if (!s) return 0;
			break;
		case 'X':
			s = strptime(s, nl_langinfo(T_FMT), tm);
			if (!s) return 0;
			break;
		case 'y':
			dest = &relyear;
			w = 2;
			want_century |= 1;
			goto numeric_digits;
		case 'Y':
			dest = &tm->tm_year;
			if (w<0) w=4;
			adj = 1900;
			want_century = 0;
			goto numeric_digits;
		case 'z':
			s += __getgmtoff((const char *)s, tm);
			continue;
		case 'Z':
			tzset();
			s += __getzonename((const char *)s, tm);
			continue;
		case '%':
			if (*s++ != '%') return 0;
			break;
		default:
			return 0;
		numeric_range:
			if (!isdigit(*s)) return 0;
			*dest = 0;
			for (i=1; i<=min+range && isdigit(*s); i*=10)
				*dest = *dest * 10 + *s++ - '0';
			if (*dest - min >= (unsigned)range) return 0;
			*dest -= adj;
			switch((char *)dest - (char *)tm) {
			case offsetof(struct tm, tm_yday):
				;
			}
			goto update;
		numeric_digits:
			neg = 0;
			if (*s == '+') s++;
			else if (*s == '-') neg=1, s++;
			if (!isdigit(*s)) return 0;
			for (*dest=i=0; i<w && isdigit(*s); i++)
				*dest = *dest * 10 + *s++ - '0';
			if (neg) *dest = -*dest;
			*dest -= adj;
			goto update;
		symbolic_range:
			for (i=2*range-1; i>=0; i--) {
				ex = nl_langinfo(min+i);
				len = strlen(ex);
				if (strncasecmp(s, ex, len)) continue;
				s += len;
				*dest = i % range;
				break;
			}
			if (i<0) return 0;
			goto update;
		update:
			//FIXME
			;
		}
	}
	if (want_century) {
		tm->tm_year = relyear;
		if (want_century & 2) tm->tm_year += century * 100 - 1900;
		else if (tm->tm_year <= 68) tm->tm_year += 100;
	}
	return (char *)s;
}
