#include <functional>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include <thread>
#include <threads.h>

using namespace testing::ext;

constexpr size_t TEST_MULTIPLE = 640;
constexpr size_t TEST_BASE = 1024;

class PthreadKeyTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: pthread_key_create_001
 * @tc.desc: Successfully created and deleted key.
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadKeyTest, pthread_key_create_001, TestSize.Level1)
{
    pthread_key_t pthreadKey;
    EXPECT_EQ(0, pthread_key_create(&pthreadKey, nullptr));
    EXPECT_EQ(0, pthread_key_delete(pthreadKey));
}

/**
 * @tc.name: pthread_key_create_002
 * @tc.desc: Set the key for the thread until the key namespace is used up and counted,
 *           delete all, and finally verify that the technology is equal to expected
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadKeyTest, pthread_key_create_002, TestSize.Level1)
{
    std::vector<pthread_key_t> pthreadKeys;
    int falseNum = 0;
    for (int i = 0; i < PTHREAD_KEYS_MAX; i++) {
        pthread_key_t pthreadKey;
        falseNum = pthread_key_create(&pthreadKey, nullptr);
        if (falseNum == EAGAIN) {
            break;
        }
        pthreadKeys.push_back(pthreadKey);
    }
    EXPECT_EQ(EAGAIN, falseNum);
    for (const auto& pthreadKey : pthreadKeys) {
        pthread_key_delete(pthreadKey);
    }
    pthreadKeys.clear();
}

/**
 * @tc.name: pthread_key_create_003
 * @tc.desc: Successfully set attrStack address and size
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadKeyTest, pthread_key_create_003, TestSize.Level1)
{
    pthread_key_t pthreadKey;
    pthread_attr_t pthreadAttr;
    const size_t stackSize = TEST_MULTIPLE * TEST_BASE;
    pthread_key_create(&pthreadKey, nullptr);
    void* attrStack = mmap(nullptr, stackSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memset(attrStack, 0xff, stackSize);
    pthread_attr_init(&pthreadAttr);
    EXPECT_EQ(0, pthread_attr_setstack(&pthreadAttr, attrStack, stackSize));
    auto dirtyKeyThread = [](void* key) -> void* {
        if (key == nullptr) {
            return nullptr;
        }
        return pthread_getspecific(*reinterpret_cast<pthread_key_t*>(key));
    };
    pthread_t thread;
    pthread_create(&thread, &pthreadAttr, dirtyKeyThread, &pthreadKey);
    void* attrStackResult;
    pthread_join(thread, &attrStackResult);
    EXPECT_EQ(nullptr, attrStackResult);
    EXPECT_EQ(0, pthread_key_delete(pthreadKey));
    munmap(attrStack, stackSize);
}

/**
 * @tc.name: pthread_key_delete_001
 * @tc.desc: Created a certain number of pthread keys, traverse the keys array in reverse order, and delete each key.
 * @tc.type: FUNC
 * */
HWTEST_F(PthreadKeyTest, pthread_key_delete_001, TestSize.Level1)
{
    int keyCounts = PTHREAD_KEYS_MAX / 2;
    std::vector<pthread_key_t> pthreadKeys;
    for (int i = 0; i < keyCounts; ++i) {
        pthread_key_t pthreadKey;
        EXPECT_EQ(0, pthread_key_create(&pthreadKey, nullptr));
        pthreadKeys.push_back(pthreadKey);
    }
    for (auto& pthreadKey : pthreadKeys) {
        EXPECT_EQ(0, pthread_key_delete(pthreadKey));
    }
    pthreadKeys.clear();
}