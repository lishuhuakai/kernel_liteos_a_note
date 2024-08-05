#include <dlfcn.h>
#include <gtest/gtest.h>
#include <string>
#include <thread>

using namespace std;
using namespace testing::ext;

class LdsoDlErrorTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

void DlerrorChildThread(string& strError)
{
    void* childFunc = dlsym(nullptr, "ChildSymbolNotExist");
    ASSERT_EQ(childFunc, nullptr);
    const char* dlErr = dlerror();
    ASSERT_NE(dlErr, nullptr);
    strError = dlErr;
}

/**
 * @tc.name: dlerror_001
 * @tc.desc: Check dlerror buffer does not conflict.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlErrorTest, dlerror_001, TestSize.Level1)
{
    string::size_type mainCheck;
    string::size_type childCheck;
    void* mainFunc = dlsym(nullptr, "MainSymbolNotExist");
    ASSERT_EQ(mainFunc, nullptr);
    string mainErr = (string)(dlerror());
    mainCheck = mainErr.find("MainSymbolNotExist");
    EXPECT_NE(mainCheck, string::npos);
    string childErr;
    thread t1(DlerrorChildThread, ref(childErr));
    t1.join();
    childCheck = childErr.find("ChildSymbolNotExist");
    EXPECT_NE(childCheck, string::npos);

    mainCheck = mainErr.find("MainSymbolNotExist");
    EXPECT_NE(mainCheck, string::npos);
}

/**
 * @tc.name: dlerror_002
 * @tc.desc: Check dlerror buffer running under concurrent conditions.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlErrorTest, dlerror_002, TestSize.Level1)
{
    string::size_type mainCheck;
    string::size_type childCheck;
    void* mainFunc = dlsym(nullptr, "MainSymbolNotExist");
    ASSERT_EQ(mainFunc, nullptr);

    string childErr;
    thread t1(DlerrorChildThread, ref(childErr));
    t1.join();
    string mainErr = (string)(dlerror());
    mainCheck = mainErr.find("MainSymbolNotExist");
    EXPECT_NE(mainCheck, string::npos);
    childCheck = childErr.find("ChildSymbolNotExist");
    EXPECT_NE(childCheck, string::npos);
}
