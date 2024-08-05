#include <errno.h>
#include <gtest/gtest.h>
#include <thread>
using namespace testing::ext;

class SysClockTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: clock_getres_001
 * @tc.desc: Ensure that the clock_getres function returns the expected time precision when the CLOCK_REALTIME
 *           parameter is used. By passing this test, it confirms that the time precision of the system's real-time
 *           clock (CLOCK_REALTIME) meets the expected requirements.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_001, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_REALTIME, &times);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, times.tv_sec);
    EXPECT_EQ(1, times.tv_nsec);
}

/**
 * @tc.name: clock_getres_002
 * @tc.desc: Ensure that the clock_getres function returns the expected time precision when the CLOCK_MONOTONIC
 *           parameter is used.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_002, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_MONOTONIC, &times);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, times.tv_sec);
    EXPECT_EQ(1, times.tv_nsec);
}

/**
 * @tc.name: clock_getres_003
 * @tc.desc: Ensure that the clock_getres function can successfully execute with the CLOCK_PROCESS_CPUTIME_ID
 *           parameter without causing any errors or exceptions.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_003, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_PROCESS_CPUTIME_ID, &times);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: clock_getres_004
 * @tc.desc: Ensure that the clock_getres function can successfully execute with the CLOCK_THREAD_CPUTIME_ID parameter
 *           without causing any errors or exceptions.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_004, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_THREAD_CPUTIME_ID, &times);
    EXPECT_EQ(0, result);
}
/**
 * @tc.name: clock_getres_005
 * @tc.desc: Verify that the resolution of the CLOCK_MONOTONIC_RAW clock is 0 seconds and 1 nanosecond, as expected.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_005, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_MONOTONIC_RAW, &times);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, times.tv_sec);
    EXPECT_EQ(1, times.tv_nsec);
}
/**
 * @tc.name: clock_getres_006
 * @tc.desc: Verify that the clock_getres function correctly retrieves the resolution of the specified clock and
 *           that the resolution is as expected (0 seconds and 1 nanosecond in this case).
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_006, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_REALTIME_COARSE, &times);
    EXPECT_EQ(0, result);
}
/**
 * @tc.name: clock_getres_007
 * @tc.desc: Verify that the clock_getres() function can successfully retrieve the resolution of the specified clock
 *           (CLOCK_MONOTONIC_COARSE in this case) and that the function call itself is successful.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_007, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_MONOTONIC_COARSE, &times);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: clock_getres_008
 * @tc.desc: Verify that the clock_getres() function can successfully retrieve the resolution of the
 *           specified clock (CLOCK_BOOTTIME in this case), and it checks if the retrieved resolution matches the
 *           expected values of 0 seconds and 1 nanosecond.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_008, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_BOOTTIME, &times);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, times.tv_sec);
    EXPECT_EQ(1, times.tv_nsec);
}
/**
 * @tc.name: clock_getres_009
 * @tc.desc: Verify that the clock_getres() function can successfully retrieve the resolution of the
 *           specified clock (CLOCK_REALTIME_ALARM in this case), and it checks if the retrieved resolution matches
 *           the expected values of 0 seconds and 1 nanosecond.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_009, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_REALTIME_ALARM, &times);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, times.tv_sec);
    EXPECT_EQ(1, times.tv_nsec);
}
/**
 * @tc.name: clock_getres_010
 * @tc.desc: verifies that the clock_getres() function can successfully retrieve the resolution of the specified
 *           clock (CLOCK_BOOTTIME_ALARM in this case), and it checks if the retrieved resolution matches the
 *           expected values of 0 seconds and 1 nanosecond.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_010, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_BOOTTIME_ALARM, &times);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, times.tv_sec);
    EXPECT_EQ(1, times.tv_nsec);
}
/**
 * @tc.name: clock_getres_011
 * @tc.desc: verifies that the clock_getres() function can successfully retrieve the resolution of the specified
 *           clock (CLOCK_TAI in this case), and it checks if the retrieved resolution matches the expected
 *           values of 0 seconds and 1 nanosecond.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_011, TestSize.Level1)
{
    timespec times;
    int result = clock_getres(CLOCK_TAI, &times);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, times.tv_sec);
    EXPECT_EQ(1, times.tv_nsec);
}

/**
 * @tc.name: clock_getres_012
 * @tc.desc: Confirm that when an invalid clock ID is passed to the clock_getres function, the function handles
 *           the error correctly by returning -1 and setting errno to EINVAL.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getres_012, TestSize.Level1)
{
    errno = 0;
    timespec times = { -10, -10 };
    int result = clock_getres(-1, &times);
    EXPECT_EQ(-1, result);
    EXPECT_EQ(EINVAL, errno);
    EXPECT_EQ(-10, times.tv_sec);
    EXPECT_EQ(-10, times.tv_nsec);
}

/**
 * @tc.name: clock_gettime_001
 * @tc.desc: Confirm that the clock_gettime function can obtain the current time accurately from the CLOCK_MONOTONIC
 *           clock ID, and that the provided timestamps are precise and consistent.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_gettime_001, TestSize.Level1)
{
    timespec ts1;
    int result = clock_gettime(CLOCK_MONOTONIC, &ts1);
    EXPECT_EQ(result, 0);

    timespec ts2;
    result = clock_gettime(CLOCK_MONOTONIC, &ts2);
    EXPECT_EQ(result, 0);
    auto start = std::chrono::seconds(ts1.tv_sec) + std::chrono::nanoseconds(ts1.tv_nsec);
    auto end = std::chrono::seconds(ts2.tv_sec) + std::chrono::nanoseconds(ts2.tv_nsec);
    auto duration = end - start;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000000;

    EXPECT_EQ(seconds, 0);
    EXPECT_GT(10'000'000, nanoseconds);
}

/**
 * @tc.name: clock_gettime_002
 * @tc.desc: Confirm that the clock_gettime function can obtain the current time accurately from the CLOCK_REALTIME
 *           clock ID. The CLOCK_REALTIME clock represents the system's best-guess time since the epoch (typically
 *           the system's startup time) and is affected by changes in the system time.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_gettime_002, TestSize.Level1)
{
    timespec times;
    int result = clock_gettime(CLOCK_REALTIME, &times);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: clock_gettime_003
 * @tc.desc: Confirm that the clock_gettime function can obtain the current time accurately from the CLOCK_MONOTONIC
 *           clock ID. The CLOCK_MONOTONIC clock represents the monotonic time since some unspecified starting point,
 *           which is immune to changes in the system time and unaffected by adjustments to the system clock.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_gettime_003, TestSize.Level1)
{
    timespec times;
    int result = clock_gettime(CLOCK_MONOTONIC, &times);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: clock_gettime_004
 * @tc.desc: Confirm that the clock_gettime function can obtain the process CPU time accurately from the
 *           CLOCK_PROCESS_CPUTIME_ID clock ID. This clock represents the CPU time used by the calling process, and
 *           each process has its own instance of this clock.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_gettime_004, TestSize.Level1)
{
    timespec times;
    int result = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &times);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: clock_gettime_005
 * @tc.desc: Confirm that the clock_gettime function can obtain the thread CPU time accurately from the
 *           CLOCK_THREAD_CPUTIME_ID clock ID. This clock represents the CPU time used by the calling thread, and each
 *           thread has its own instance of this clock.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_gettime_005, TestSize.Level1)
{
    timespec times;
    int result = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &times);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: clock_gettime_006
 * @tc.desc: Confirm that the clock_gettime function can obtain the boot time accurately from the CLOCK_BOOTTIME
 *           clock ID. This clock represents the system-wide monotonic boot time, including time spent in sleep, and
 *           is immune to changes in the system time.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_gettime_006, TestSize.Level1)
{
    timespec times;
    int result = clock_gettime(CLOCK_BOOTTIME, &times);
    EXPECT_EQ(0, result);
}

/**
 * @tc.name: clock_gettime_007
 * @tc.desc: Aim to validate the error handling mechanism of clock_gettime when an invalid clock ID is encountered,
 *           ensuring the function behaves as expected in such scenarios.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_gettime_007, TestSize.Level1)
{
    errno = 0;
    timespec times;
    int result = clock_gettime(-1, &times);
    EXPECT_EQ(-1, result);
    EXPECT_EQ(EINVAL, errno);
}

/**
 * @tc.name: clock_settime_001
 * @tc.desc: Aim to validate the error handling mechanism of the clock_settime function when an invalid clock ID is
 *           encountered, ensuring the function behaves as expected in such scenarios.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_settime_001, TestSize.Level1)
{
    errno = 0;
    timespec times;
    int result = clock_settime(-1, &times);
    EXPECT_EQ(-1, result);
    EXPECT_EQ(EINVAL, errno);
}

/**
 * @tc.name: clock_nanosleep_001
 * @tc.desc: Aim to assess the accuracy and reliability of the clock_nanosleep function by measuring the elapsed
 *           time between two steady clock readings and confirming that it matches the expected sleep duration.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_nanosleep_001, TestSize.Level1)
{
    auto time0 = std::chrono::steady_clock::now();
    const timespec req = { .tv_nsec = 5000000 };
    int result = clock_nanosleep(CLOCK_MONOTONIC, 0, &req, nullptr);
    EXPECT_EQ(0, result);
    auto time1 = std::chrono::steady_clock::now();
    EXPECT_GE(std::chrono::duration_cast<std::chrono::nanoseconds>(time1 - time0).count(), 5000000);
}

/**
 * @tc.name: clock_nanosleep_002
 * @tc.desc: Validate the error handling mechanism of the clock_nanosleep function when an invalid clock ID is
 *           encountered, ensuring the function behaves as expected in such scenarios.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_nanosleep_002, TestSize.Level1)
{
    timespec req;
    timespec rem;
    int err = clock_nanosleep(-1, 0, &req, &rem);
    EXPECT_EQ(EINVAL, err);
}

/**
 * @tc.name: clock_nanosleep_003
 * @tc.desc: Validate the error handling mechanism of the clock_nanosleep function when an invalid flag is encountered,
 *           ensuring the function behaves as expected in such scenarios.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_nanosleep_003, TestSize.Level1)
{
    timespec req;
    req.tv_sec = 1;
    req.tv_nsec = 0;
    int result = clock_nanosleep(CLOCK_THREAD_CPUTIME_ID, 0, &req, nullptr);
    EXPECT_EQ(EINVAL, result);
}

/**
 * @tc.name: clock_getcpuclockid_001
 * @tc.desc: Verify that clock_getcpuclockid can successfully retrieve the CPU clock ID for the current process and
 *           subsequently use it to obtain the current time using clock_gettime.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getcpuclockid_001, TestSize.Level1)
{
    clockid_t id;
    int result1 = clock_getcpuclockid(getpid(), &id);
    EXPECT_EQ(0, result1);
    timespec times;
    int result2 = clock_gettime(id, &times);
    EXPECT_EQ(0, result2);
}

/**
 * @tc.name: clock_getcpuclockid_002
 * @tc.desc: Verify that clock_getcpuclockid can successfully retrieve the CPU clock ID for the parent process of the
 *           current process and subsequently use it to obtain the current time using clock_gettime.
 * @tc.type: FUNC
 **/
HWTEST_F(SysClockTest, clock_getcpuclockid_002, TestSize.Level1)
{
    clockid_t id;
    int result = clock_getcpuclockid(getppid(), &id);
    EXPECT_EQ(0, result);
    timespec times;
    int result2 = clock_gettime(id, &times);
    EXPECT_EQ(0, result2);
}
