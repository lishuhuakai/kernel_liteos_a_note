#include <gtest/gtest.h>
#include <iostream>
#include <time.h>
using namespace testing::ext;

class TimeTzsetTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tzset_001
 * @tc.desc: Verify the behavior of the tzset() function by setting the time zone using the "TZ" environment variable,
 *           calling tzset() to apply the changes, obtaining local time representations, and comparing the results to
 *           the expected timezone abbreviations.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTzsetTest, tzset_001, TestSize.Level1)
{
    setenv("TZ", "Asia/Shanghai", 1);
    tzset();
    std::time_t t1 = std::time(nullptr);
    std::tm* localTime1 = std::localtime(&t1);
    EXPECT_STREQ("CST", localTime1->tm_zone);

    setenv("TZ", "America/Logs_Angeles", 1);
    tzset();
    std::time_t t2 = std::time(nullptr);
    std::tm* localTime2 = std::localtime(&t2);
    EXPECT_STREQ("Americ", localTime2->tm_zone);
}