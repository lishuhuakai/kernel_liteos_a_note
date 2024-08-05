#include <chrono>
#include <errno.h>
#include <gtest/gtest.h>
#include <pthread.h>
#include <string.h>
#include <thread>
#include <time.h>

using namespace testing::ext;

class TimerTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};
#define SLEEPTIME 100
class SignalProcessor {
public:
    SignalProcessor(int signalNum, void (*handler)(int), int saFlags = 0)
        : signalNumber_(signalNum), oldAction_(new struct sigaction)
    {
        action_ = { .sa_flags = saFlags, .sa_handler = handler };
        sigaction(signalNumber_, &action_, oldAction_);
    }

    SignalProcessor(int signalNum, void (*action)(int, siginfo_t*, void*), int saFlags = SA_SIGINFO)
        : signalNumber_(signalNum), oldAction_(new struct sigaction)
    {
        action_ = { .sa_flags = saFlags, .sa_sigaction = action };
        sigaction(signalNumber_, &action_, oldAction_);
    }

    ~SignalProcessor()
    {
        sigaction(signalNumber_, oldAction_, nullptr);
        delete oldAction_;
    }

private:
    const int signalNumber_;
    struct sigaction action_;
    struct sigaction* oldAction_;
};

class Scaler {
private:
    std::atomic<int> value_;
    timer_t timerId_;
    struct sigevent se_;
    bool timerValid_;

    void CreateTimer()
    {
        if (!timerValid_) {
            if (timer_create(CLOCK_REALTIME, &se_, &timerId_) == 0) {
                timerValid_ = true;
            }
        }
    }

public:
    explicit Scaler(void (*fn)(sigval)) : value_(0), timerValid_(false)
    {
        memset(&se_, 0, sizeof(se_));
        se_.sigev_notify = SIGEV_THREAD;
        se_.sigev_notify_function = fn;
        se_.sigev_value.sival_ptr = this;
        CreateTimer();
    }

    ~Scaler()
    {
        if (timerValid_) {
            if (timerValid_) {
                if (timer_delete(timerId_) == 0) {
                    timerValid_ = false;
                }
            }
        }
    }

    int Value() const
    {
        return value_.load();
    }

    bool GetTimerValid()
    {
        return timerValid_;
    }

    timer_t GetTimerid()
    {
        return timerId_;
    }

    void Change()
    {
        timerValid_ = false;
    }

    void SetTimer(time_t valueS, time_t valueNs, time_t intervalS, time_t intervalNs)
    {
        itimerspec newValue;
        newValue.it_value.tv_sec = valueS;
        newValue.it_value.tv_nsec = valueNs;
        newValue.it_interval.tv_sec = intervalS;
        newValue.it_interval.tv_nsec = intervalNs;
        timer_settime(timerId_, 0, &newValue, nullptr);
    }

    bool ValUpdate()
    {
        int currentValue = value_;
        time_t start = time(nullptr);
        while (currentValue == value_ && (time(nullptr) - start) < 5) {
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
        }
        return currentValue != value_;
    }

    static void ScalerFunc(sigval value_)
    {
        Scaler* cd = reinterpret_cast<Scaler*>(value_.sival_ptr);
        ++cd->value_;
    }

    static void ScaNotifyFunc(sigval value_)
    {
        Scaler* cd = reinterpret_cast<Scaler*>(value_.sival_ptr);
        ++cd->value_;
        cd->SetTimer(0, 0, 1, 0);
    }
};

static void NotifyFunction(sigval) {}
static int g_count = 0;
void Handler(int sig)
{
    g_count++;
    return;
}

/**
 * @tc.name: timer_create_001
 * @tc.desc: Verify that the "timer_create" function correctly creates a timer with the specified attributes and
 *           that the "timer_delete" function successfully deletes the created timer.
 * @tc.type: FUNC
 **/
HWTEST_F(TimerTest, timer_create_001, TestSize.Level1)
{
    struct sigevent sev;
    timer_t timerid;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = &timerid;
    signal(SIGUSR1, Handler);
    int result = timer_create(CLOCK_REALTIME, &sev, &timerid);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(0, timer_delete(timerid));
}

/**
 * @tc.name: timer_create_002
 * @tc.desc: Verify that the "timer_create" function properly handles invalid input arguments and returns the expected
 *           error code in the event of such errors.
 * @tc.type: FUNC
 **/
HWTEST_F(TimerTest, timer_create_002, TestSize.Level1)
{
    clockid_t invalid = 13;
    timer_t timer;
    EXPECT_EQ(timer_create(invalid, nullptr, &timer), -1);
    EXPECT_EQ(EINVAL, errno);

    sigevent sev;
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = NotifyFunction;
    EXPECT_EQ(-1, timer_create(invalid, &sev, &timer));
    EXPECT_EQ(EINVAL, errno);
}

/**
 * @tc.name: timer_delete_001
 * @tc.desc: Check if the timer is properly deleted and if the scaler's value remains unchanged after the
 *           timer is deleted.
 * @tc.type: FUNC
 **/
HWTEST_F(TimerTest, timer_delete_001, TestSize.Level1)
{
    Scaler scaler(Scaler::ScalerFunc);
    EXPECT_EQ(0, scaler.Value());

    scaler.SetTimer(0, 2, 0, 2);

    EXPECT_TRUE(scaler.ValUpdate());
    EXPECT_TRUE(scaler.ValUpdate());
    bool timerValid = scaler.GetTimerValid();
    timer_t timerid = scaler.GetTimerid();
    if (timerValid) {
        if (timer_delete(timerid) == 0) {
            scaler.Change();
        }
    }
    std::chrono::nanoseconds duration1(500000);
    std::this_thread::sleep_for(duration1);
    int value = scaler.Value();
    std::chrono::nanoseconds duration2(500000);
    std::this_thread::sleep_for(duration2);
    EXPECT_EQ(value, scaler.Value());
}

/**
 * @tc.name: timer_settime_001
 * @tc.desc: Verify that the "SetTimer" function of the "Scaler" object correctly sets the expiration time for a timer
 *           associated with the object, allowing for proper management and triggering of timers in the system.
 * @tc.type: FUNC
 **/
HWTEST_F(TimerTest, timer_settime_001, TestSize.Level1)
{
    Scaler scaler(Scaler::ScaNotifyFunc);
    EXPECT_EQ(0, scaler.Value());
    scaler.SetTimer(0, 500000000, 1, 0);
    sleep(1);
    EXPECT_EQ(1, scaler.Value());
}

/**
 * @tc.name: timer_settime_002
 * @tc.desc: Verify that the "SetTimer" function of the "Scaler" object correctly sets a timer with a reload interval
 *           and that the timer can be successfully deleted.
 * @tc.type: FUNC
 **/
HWTEST_F(TimerTest, timer_settime_002, TestSize.Level1)
{
    Scaler scaler(Scaler::ScalerFunc);
    EXPECT_EQ(0, scaler.Value());

    scaler.SetTimer(0, 1, 0, 10);
    // Increase the probability of the timer reaching the set time
    EXPECT_TRUE(scaler.ValUpdate());
    EXPECT_TRUE(scaler.ValUpdate());
    bool timerValid = scaler.GetTimerValid();
    timer_t timerid = scaler.GetTimerid();
    if (timerValid) {
        if (timer_delete(timerid) == 0) {
            scaler.Change();
        }
    }
    std::chrono::nanoseconds duration(500000);
    std::this_thread::sleep_for(duration);
}