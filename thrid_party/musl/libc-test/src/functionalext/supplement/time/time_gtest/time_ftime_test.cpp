#include <gtest/gtest.h>
#include <sys/timeb.h>
using namespace testing::ext;

class TimeFtimeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};
constexpr int NUM = 1000000;
/**
 * @tc.name: ftime_001
 * @tc.desc: Test the functionality of the ftime function and compares the system time obtained from
 *           std::chrono::system_clock::now() with the time obtained from ftime function.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeFtimeTest, ftime_001, TestSize.Level1)
{
    struct timeb tmb;
    int result = ftime(&tmb);
    EXPECT_EQ(0, result);
    ftime(&tmb);
    auto currentTime = std::chrono::system_clock::now();
    auto duration = currentTime.time_since_epoch();

    auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    auto usec = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % NUM;

    long secDiff = sec - tmb.time;
    long usecDiff = usec - tmb.millitm;
    if (usecDiff < 0) {
        --secDiff;
        usecDiff += NUM;
    }
    long maxSecDiff = 0;
    EXPECT_LE(secDiff, maxSecDiff);
}