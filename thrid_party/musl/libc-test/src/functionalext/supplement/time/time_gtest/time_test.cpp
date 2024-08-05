#include <errno.h>
#include <gtest/gtest.h>
#include <sys/timeb.h>
#include <thread>
using namespace testing::ext;

class TimeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

const constexpr int BUFLEN = 512;

/**
 * @tc.name: clock_001
 * @tc.desc: Assess the accuracy of the clock function by comparing the CPU time measurements before and after a known
 *           delay, and verifying that the difference falls within an acceptable threshold of precision.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, clock_001, TestSize.Level1)
{
    clock_t time0 = clock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    clock_t time1 = clock();
    // 1s sleep should cost less than 5ms
    EXPECT_GT(5 * (CLOCKS_PER_SEC / 1000), time1 - time0);
}

/**
 * @tc.name: time_001
 * @tc.desc: Verify the correctness and behavior of the time function by comparing the retrieved system time values,
 *           checking the relationship between consecutive measurements, and validating the functionality of
 *           time(nullptr) in obtaining the current system time.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, time_001, TestSize.Level1)
{
    time_t time1;
    time_t p1 = time(&time1);
    EXPECT_NE(0, static_cast<int>(time1));
    EXPECT_NE(-1, static_cast<int>(time1));
    EXPECT_EQ(time1, p1);

    std::this_thread::sleep_for(std::chrono::milliseconds(1001));
    time_t time2;
    time_t p2 = time(&time2);
    EXPECT_NE(0, static_cast<int>(time2));
    EXPECT_NE(-1, static_cast<int>(time2));
    EXPECT_EQ(time2, p2);

    EXPECT_GT(p2, p1);
    EXPECT_GT(2, static_cast<int>(time2 - time1));

    EXPECT_LE(time(nullptr), p2);
    EXPECT_GE(1, static_cast<int>(time(nullptr) - time2));
}

/**
 * @tc.name: mktime_001
 * @tc.desc: Verify the correctness of the mktime function by checking the conversion of a struct tm to a time_t value
 *           in two different timezones. It validates that the function produces the expected time_t values for specific
 *           date and time inputs, considering the appropriate timezone information.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, mktime_001, TestSize.Level1)
{
    struct tm time;
    memset(&time, 0, sizeof(tm));
    time.tm_year = 2023 - 1900;
    time.tm_mon = 10;
    time.tm_mday = 16;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    EXPECT_EQ(mktime(&time), static_cast<time_t>(1700064000));

    memset(&time, 0, sizeof(tm));
    time.tm_year = 2023 - 1900;
    time.tm_mon = 10;
    time.tm_mday = 16;

    setenv("TZ", "", 1);
    tzset();
    EXPECT_EQ(mktime(&time), static_cast<time_t>(1700092800));
}

/**
 * @tc.name: mktime_002
 * @tc.desc: Verify the behavior of the mktime function when converting a struct tm representing a future date to a
 *           time_t value. It checks the correctness of the conversion in different timezones ("Asia/Shanghai" and
 *"UTC") and ensures that no errors occur during the process by checking the value of errno.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, mktime_002, TestSize.Level1)
{
    struct tm time;
    memset(&time, 0, sizeof(tm));
    time.tm_year = 2123 - 1900;
    time.tm_mon = 10;
    time.tm_mday = 16;
    EXPECT_LE(64U, sizeof(time_t) * 8);

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    errno = 0;
    EXPECT_EQ(mktime(&time), static_cast<time_t>(4855737600U));
    EXPECT_EQ(errno, 0);

    setenv("TZ", "UTC", 1);
    tzset();
    errno = 0;
    EXPECT_EQ(mktime(&time), static_cast<time_t>(4855766400U));
    EXPECT_EQ(errno, 0);
}

/**
 * @tc.name: strftime_001
 * @tc.desc: Verify the behavior of the strftime function in correctly formatting a given struct tm time value into a
 *           string representation. It checks the correctness of the formatted string using different format specifiers
 *           ("%s" and "%c") and ensures that the output matches the expected results in the UTC timezone.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strftime_001, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    tzset();

    struct tm time;
    memset(&time, 0, sizeof(tm));
    time.tm_year = 2123 - 1900;
    time.tm_mon = 9;
    time.tm_mday = 16;
    char buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));
    EXPECT_EQ(24U, strftime(buf, sizeof(buf), "%c", &time));
    EXPECT_STREQ("Sun Oct 16 00:00:00 2123", buf);
}

/**
 * @tc.name: strftime_002
 * @tc.desc: Verify the behavior of the strftime function in formatting the time zone information using
 *           the %Z specifier. It ensures that the formatted string reflects the correct time zone based on the local
 *           timezone settings,with the expected results for both the Shanghai and UTC timezones.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strftime_002, TestSize.Level1)
{
    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    time_t currentTime = time(nullptr);
    struct tm* tmInfo = localtime(&currentTime);
    char buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));

    EXPECT_EQ(5U, strftime(buf, sizeof(buf), "<%Z>", tmInfo));
    EXPECT_STREQ("<CST>", buf);

    setenv("TZ", "UTC", 1);
    tzset();
    EXPECT_EQ(5U, strftime(buf, sizeof(buf), "<%Z>", tmInfo));
    EXPECT_STREQ("<UTC>", buf);
}

/**
 * @tc.name: strftime_003
 * @tc.desc: Verify the behavior of strftime and strptime functions in handling time zone information and converting
 *           between Unix time and struct tm representations. It ensures that the generated Unix time and resulting
 *           struct tm representations are correct for both the Los Angeles timezone and UTC.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strftime_003, TestSize.Level1)
{
    char buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));
    const struct tm tim = { .tm_year = 2023 - 1900, .tm_mon = 9, .tm_mday = 16 };
    setenv("TZ", "Americe/Logs_Angeles", 1);
    strftime(buf, sizeof(buf), "<%s>", &tim);
    EXPECT_STREQ("<1697414400>", buf);

    setenv("TZ", "UTC", 1);
    strftime(buf, sizeof(buf), "<%s>", &tim);
    EXPECT_STREQ("<1697414400>", buf);

    struct tm tm;
    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    memset(&tm, 0xff, sizeof(tm));

    char* p = strptime("1697414400x", "%s", &tm);
    EXPECT_EQ('x', *p);
    EXPECT_EQ(0, tm.tm_sec);
    EXPECT_EQ(0, tm.tm_min);
    EXPECT_EQ(8, tm.tm_hour);
    EXPECT_EQ(16, tm.tm_mday);
    EXPECT_EQ(9, tm.tm_mon);
    EXPECT_EQ(123, tm.tm_year);
    EXPECT_EQ(1, tm.tm_wday);
    EXPECT_EQ(288, tm.tm_yday);
    EXPECT_EQ(0, tm.tm_isdst);
}

/**
 * @tc.name: gmtime_001
 * @tc.desc: Ensure that the gmtime function correctly converts the time value of 0 to a struct tm representation in
 *           UTC, with the expected values for the epoch.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, gmtime_001, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm* time = gmtime(&tim);
    ASSERT_NE(time, nullptr);
    EXPECT_EQ(20, time->tm_sec);
    EXPECT_EQ(33, time->tm_min);
    EXPECT_EQ(3, time->tm_hour);
    EXPECT_EQ(18, time->tm_mday);
    EXPECT_EQ(4, time->tm_mon);
    EXPECT_EQ(2033, time->tm_year + 1900);
}

/**
 * @tc.name: localtime_001
 * @tc.desc: Ensure that the localtime function can correctly convert a Unix time value to a struct tm representation
 *           in two different time zones (Asia/Shanghai and America/Atka) using the TZ environment variable.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, localtime_001, TestSize.Level1)
{
    time_t tim = 2000000000;
    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    struct tm* tmC = localtime(&tim);
    int ctime = tmC->tm_min;
    EXPECT_EQ(33, ctime);

    setenv("TZ", "America/Atka", 1);
    struct tm* tmA = localtime(&tim);
    int atime = tmA->tm_min;
    EXPECT_EQ(33, atime);
}

/**
 * @tc.name: asctime_001
 * @tc.desc: Ensure that the asctime function correctly converts a struct tm representation of a time value with all
 *           fields set to 0 to a string representation using the standard format.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, asctime_001, TestSize.Level1)
{
    struct tm time;
    memset(&time, 0, sizeof(tm));
    char* result = asctime(&time);
    EXPECT_STREQ("Sun Jan  0 00:00:00 1900\n", result);
}

/**
 * @tc.name: ctime_001
 * @tc.desc: Verify that the ctime function correctly converts a Unix time value (0) to a string representation using
 *           the standard format when operating in the UTC timezone.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, ctime_001, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    tzset();
    const time_t tim = 0;
    char* result = ctime(&tim);
    EXPECT_STREQ("Thu Jan  1 00:00:00 1970\n", result);
}

/**
 * @tc.name: timespec_get_001
 * @tc.desc: Verify that the timespec_get function can correctly retrieve the current time in the UTC format and store
 *           it in a struct timespec variable.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, timespec_get_001, TestSize.Level1)
{
    struct timespec ts;
    int result = timespec_get(&ts, TIME_UTC);
    EXPECT_EQ(1, result);
}

/**
 * @tc.name: strftime_l_001
 * @tc.desc: Verify that the strftime_l function correctly formats a struct tm time value to a string representation
 *           using the specified locale.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strftime_l_001, TestSize.Level1)
{
    char s[BUFLEN];
    memset(&s, 0, sizeof(s));
    size_t maxsize = sizeof(s);
    setenv("TZ", "UTC", 1);
    struct tm tp;
    memset(&tp, 0, sizeof(tm));
    tp.tm_year = 2023 - 1900;
    tp.tm_mon = 9;
    tp.tm_mday = 16;
    locale_t loc = newlocale(LC_ALL, "C.UTF-8", nullptr);
    locale_t oldLoc = uselocale(loc);
    size_t result = strftime_l(s, maxsize, "%c", &tp, loc);
    EXPECT_EQ(24U, result);
    EXPECT_STREQ("Sun Oct 16 00:00:00 2023", s);

    uselocale(oldLoc);
    freelocale(loc);
}

/**
 * @tc.name: gmtime_r_001
 * @tc.desc: Verify that the gmtime_r function correctly converts a given time in seconds since the epoch into a
 *           struct tm representation in UTC
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, gmtime_r_001, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm tm = {};

    struct tm* time = gmtime_r(&tim, &tm);
    EXPECT_EQ(time, &tm);
    EXPECT_EQ(20, time->tm_sec);
    EXPECT_EQ(33, time->tm_min);
    EXPECT_EQ(3, time->tm_hour);
    EXPECT_EQ(18, time->tm_mday);
    EXPECT_EQ(4, time->tm_mon);
    EXPECT_EQ(2033, time->tm_year + 1900);
}

/**
 * @tc.name: localtime_r_001
 * @tc.desc: Determine when the time zone is "Asia/Shanghai" When "localtime_r" Can correctly return to local time
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, localtime_r_001, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm tm;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    ASSERT_NE(localtime_r(&tim, &tm), nullptr);
    EXPECT_EQ(18, tm.tm_mday);
}

/**
 * @tc.name: localtime_r_002
 * @tc.desc: Determine when the time zone is "America/Los_Angeles" When "localtime_r" Can correctly return to local time
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, localtime_r_002, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm tm;
    setenv("TZ", "America/Los_Angeles", 1);
    tzset();
    ASSERT_NE(localtime_r(&tim, &tm), nullptr);
    EXPECT_EQ(17, tm.tm_mday);
}

/**
 * @tc.name: localtime_r_003
 * @tc.desc: Determine when the time zone is "Europe/London" When "localtime_r" Can correctly return to local time
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, localtime_r_003, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm tm;
    setenv("TZ", "Europe/London", 1);
    tzset();
    ASSERT_NE(localtime_r(&tim, &tm), nullptr);
    EXPECT_EQ(18, tm.tm_mday);
}

/**
 * @tc.name: localtime_r_004
 * @tc.desc: Determine when the time zone is "Pacific/Apia" When "localtime_r" Can correctly return to local time
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, localtime_r_004, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm tm;
    setenv("TZ", "Pacific/Apia", 1);
    tzset();
    ASSERT_NE(localtime_r(&tim, &tm), nullptr);
    EXPECT_EQ(18, tm.tm_mday);
}

/**
 * @tc.name: localtime_r_005
 * @tc.desc: Determine when the time zone is "Pacific/Honolulu" When "localtime_r" Can correctly return to local time
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, localtime_r_005, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm tm;
    setenv("TZ", "Pacific/Honolulu", 1);
    tzset();
    ASSERT_NE(localtime_r(&tim, &tm), nullptr);
    EXPECT_EQ(17, tm.tm_mday);
}

/**
 * @tc.name: localtime_r_006
 * @tc.desc: Determine when the time zone is "Asia/Magadan" When "localtime_r" Can correctly return to local time
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, localtime_r_006, TestSize.Level1)
{
    time_t tim = 2000000000;
    struct tm tm;
    setenv("TZ", "Asia/Magadan", 1);
    tzset();
    ASSERT_NE(localtime_r(&tim, &tm), nullptr);
    EXPECT_EQ(18, tm.tm_mday);
}

/**
 * @tc.name: asctime_r_001
 * @tc.desc: Verify that the asctime_r function correctly converts a struct tm representation of a time into a
 *           formatted string
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, asctime_r_001, TestSize.Level1)
{
    const struct tm tm = {};
    char buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));
    char* result = asctime_r(&tm, buf);
    EXPECT_EQ(buf, result);
    EXPECT_STREQ("Sun Jan  0 00:00:00 1900\n", buf);
}

/**
 * @tc.name: ctime_r_001
 * @tc.desc: Verify that the ctime_r function correctly converts a time value in seconds since the epoch into a
 *           formatted string.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, ctime_r_001, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    const time_t tim = 0;
    char buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));
    char* result = ctime_r(&tim, buf);
    EXPECT_EQ(buf, result);
    EXPECT_STREQ("Thu Jan  1 00:00:00 1970\n", buf);
}

/**
 * @tc.name: nanosleep_001
 * @tc.desc: Verify that the nanosleep function correctly suspends the execution of a thread for a specified duration.
 *           It checks that the function returns 0 when the sleep completes uninterrupted.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, nanosleep_001, TestSize.Level1)
{
    time_t time0 = time(nullptr);
    const timespec ts = { .tv_sec = 1 };
    int result = nanosleep(&ts, nullptr);
    EXPECT_EQ(0, result);
    time_t time1 = time(nullptr);
    EXPECT_LE(static_cast<int>(1), time1 - time0);
}

/**
 * @tc.name: nanosleep_002
 * @tc.desc: Verify that the nanosleep function correctly handles an invalid sleep duration. It checks that the
 *           function returns -1 and sets errno to EINVAL when provided with a negative nanosecond value.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, nanosleep_002, TestSize.Level1)
{
    timespec ts = { .tv_nsec = -10000000 };
    errno = 0;
    int result = nanosleep(&ts, nullptr);
    EXPECT_EQ(-1, result);
    EXPECT_EQ(EINVAL, errno);
}

/**
 * @tc.name: strptime_001
 * @tc.desc: Verify the correct behavior of the strptime and strftime functions in parsing and formatting time strings.
 *           It checks that the functions correctly parse the provided time strings according to the specified format
 *           and generate accurate formatted time strings.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strptime_001, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    struct tm tim;
    char buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));

    memset(&tim, 0, sizeof(tim));
    strptime("09:57", "%R", &tim);
    strftime(buf, sizeof(buf), "%H:%M", &tim);
    EXPECT_STREQ("09:57", buf);

    memset(&tim, 0, sizeof(tim));
    strptime("09:58:55", "%T", &tim);
    strftime(buf, sizeof(buf), "%H:%M:%S", &tim);
    EXPECT_STREQ("09:58:55", buf);
}

/**
 * @tc.name: strptime_002
 * @tc.desc: Ensure that the "strptime" function correctly parses dates in the "%F" format and produces the expected
 *           output, which is important for various applications that require date/time processing.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strptime_002, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    struct tm tim = {};
    char* result = strptime("2023-10-20", "%F", &tim);
    EXPECT_EQ('\0', *result);
    EXPECT_EQ(123, tim.tm_year);
    EXPECT_EQ(9, tim.tm_mon);
    EXPECT_EQ(20, tim.tm_mday);
}

/**
 * @tc.name: strftime_003
 * @tc.desc: Ensure that the 'strptime' function correctly parses the timestamp in the '% s' format and generates
 *           the expected output
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strptime_003, TestSize.Level1)
{
    char buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));
    struct tm tim = { .tm_year = 2023 - 1900, .tm_mon = 9, .tm_mday = 16, .tm_isdst = -1 };
    setenv("TZ", "Americe/Los_Angeles", 1);
    strftime(buf, sizeof(buf), "<%s>", &tim);
    EXPECT_STREQ("<1697414400>", buf);

    setenv("TZ", "UTC", 1);
    strftime(buf, sizeof(buf), "<%s>", &tim);
    EXPECT_STREQ("<1697414400>", buf);

    struct tm tm;

    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    memset(&tm, 0xff, sizeof(tm));

    char* p = strptime("1697414400x", "%s", &tm);
    EXPECT_EQ('x', *p);
    EXPECT_EQ(0, tm.tm_sec);
    EXPECT_EQ(0, tm.tm_min);
    EXPECT_EQ(8, tm.tm_hour);
    EXPECT_EQ(16, tm.tm_mday);
    EXPECT_EQ(9, tm.tm_mon);
    EXPECT_EQ(123, tm.tm_year);
    EXPECT_EQ(1, tm.tm_wday);
    EXPECT_EQ(288, tm.tm_yday);
    EXPECT_EQ(0, tm.tm_isdst);
}

/**
 * @tc.name: strptime_004
 * @tc.desc: Ensure that the "strptime" function correctly handles different representations of the AM/PM indicator
 *           ("%p" and "%P") and does not modify the hour field when the indicator is parsed.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strptime_004, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);

    struct tm tim = { .tm_hour = 12 };
    EXPECT_EQ('\0', *strptime("AM", "%p", &tim));
    EXPECT_EQ(0, tim.tm_hour);

    tim = { .tm_hour = 12 };
    EXPECT_EQ('\0', *strptime("am", "%p", &tim));
    EXPECT_EQ(0, tim.tm_hour);

    tim = { .tm_hour = 12 };
    EXPECT_EQ('\0', *strptime("AM", "%P", &tim));
    EXPECT_EQ(0, tim.tm_hour);

    tim = { .tm_hour = 12 };
    EXPECT_EQ('\0', *strptime("am", "%P", &tim));
    EXPECT_EQ(0, tim.tm_hour);
}

/**
 * @tc.name: strptime_005
 * @tc.desc: Ensure that the "strptime" function correctly parses and assigns the weekday information based on the "%u"
 *           format specifier, allowing for accurate conversions between weekday representations and "struct tm"
 *objects.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strptime_005, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    struct tm tim = {};
    char* result = strptime("5", "%u", &tim);
    EXPECT_EQ('\0', *result);
    EXPECT_EQ(5, tim.tm_wday);
}

/**
 * @tc.name: strptime_006
 * @tc.desc: Verify that the "strptime" function correctly parses and assigns the date components based on the "%v"
 *           format specifier, allowing for accurate conversions between date representations and "struct tm" objects.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strptime_006, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    struct tm tim = {};
    char* result = strptime("25-Mar-2023", "%v", &tim);
    EXPECT_EQ('\0', *result);
    EXPECT_EQ(123, tim.tm_year);
    EXPECT_EQ(2, tim.tm_mon);
    EXPECT_EQ(25, tim.tm_mday);
}

/**
 * @tc.name: strptime_007
 * @tc.desc: Verify that the "strptime" function properly handles invalid inputs and does not assign any
 *           values to the "struct tm" object in such cases.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, strptime_007, TestSize.Level1)
{
    struct tm tim;
    char* result = strptime("x", "%s", &tim);
    EXPECT_EQ(nullptr, result);
}

/**
 * @tc.name: wcsftime_001
 * @tc.desc: Verify that the "wcsftime" function correctly formats a "struct tm" object as a wide character string
 *           according to the specified format specifier and the current locale.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, wcsftime_001, TestSize.Level1)
{
    setenv("TZ", "UTC", 1);
    struct tm tim;
    memset(&tim, 0, sizeof(tm));
    tim.tm_year = 2023 - 1900;
    tim.tm_mon = 9;
    tim.tm_mday = 16;

    wchar_t buf[BUFLEN];
    memset(&buf, 0, sizeof(buf));
    size_t result = wcsftime(buf, sizeof(buf), L"%c", &tim);
    EXPECT_EQ(24U, result);
    EXPECT_STREQ(L"Sun Oct 16 00:00:00 2023", buf);
}

/**
 * @tc.name: gettimeofday_001
 * @tc.desc: Verify that the "gettimeofday" function is correctly retrieving the current time and that the obtained
 *           values are reasonably close to the system time obtained through other means.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTest, gettimeofday_001, TestSize.Level1)
{
    timeval tv1;
    EXPECT_EQ(gettimeofday(&tv1, nullptr), 0);
    auto currentTime = std::chrono::system_clock::now();
    auto duration = currentTime.time_since_epoch();

    auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    auto usec = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000000;

    long secDiff = sec - tv1.tv_sec;
    long usecDiff = usec - tv1.tv_usec;

    if (usecDiff < 0) {
        --secDiff;
        usecDiff += 1000000;
    }
    long maxSecDiff = 0;
    EXPECT_LE(secDiff, maxSecDiff);
}