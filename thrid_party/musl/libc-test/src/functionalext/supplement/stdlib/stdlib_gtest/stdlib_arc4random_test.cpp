#include <gtest/gtest.h>
#include <stdlib.h>
using namespace testing::ext;

constexpr int CANCEL_NUM = 1000;
constexpr int AVER_NUM = 500;
constexpr double EXPECT_NUM = 50.0;
constexpr int BUF_SIZE = 100;

class StdlibArc4randomTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: arc4random_001
 * @tc.desc: Verify the randomness of this arc4random.
 * @tc.type: FUNC
 * */
HWTEST_F(StdlibArc4randomTest, arc4random_001, TestSize.Level1)
{
    int num, sum = 0;
    for (int i = 0; i < CANCEL_NUM; i++) {
        num = arc4random() % CANCEL_NUM;
        EXPECT_GE(CANCEL_NUM, num);
        sum += num;
    }
    double dev = static_cast<double>(sum) / CANCEL_NUM;
    EXPECT_NEAR(dev, AVER_NUM, EXPECT_NUM);
}

/**
 * @tc.name: arc4random_uniform_001
 * @tc.desc: Verify the randomness of this arc4random_uniform.
 * @tc.type: FUNC
 * */
HWTEST_F(StdlibArc4randomTest, arc4random_uniform_001, TestSize.Level1)
{
    int num, sum = 0;
    for (int i = 0; i < CANCEL_NUM; i++) {
        num = arc4random_uniform(CANCEL_NUM);
        EXPECT_GE(CANCEL_NUM, num);
        sum += num;
    }
    double dev = static_cast<double>(sum) / CANCEL_NUM;
    EXPECT_NEAR(dev, AVER_NUM, EXPECT_NUM);
}

/**
 * @tc.name: arc4random_buf_001
 * @tc.desc: Verify the randomness of this arc4random_buf.
 * @tc.type: FUNC
 * */
HWTEST_F(StdlibArc4randomTest, arc4random_buf_001, TestSize.Level1)
{
    uint8_t buffer[BUF_SIZE];
    uint8_t buffer2[BUF_SIZE];
    arc4random_buf(buffer, BUF_SIZE);
    arc4random_buf(buffer2, BUF_SIZE);
    bool isRandom = false;
    for (int i = 0; i < BUF_SIZE; ++i) {
        if (buffer[i] != buffer2[i]) {
            isRandom = true;
            break;
        }
    }
    EXPECT_TRUE(isRandom);
}