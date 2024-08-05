#include <gtest/gtest.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

using namespace testing::ext;

class ThreadPthrdTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

class ThreadPthrdDeathTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr size_t BUF_LEN = 16;
constexpr size_t NAME_SIZE = 32;
constexpr size_t NAME_LONG_SIZE = 64;

static std::string g_dtorOutput;

static bool g_controlFlag = true;

static void CancelLoop()
{
    g_controlFlag = false;
}

static void* BeginLoop(void*)
{
    while (g_controlFlag) {
    }
    return nullptr;
}

static void* PthreadNameNp(void* arg)
{
    EXPECT_EQ(0, pthread_setname_np(pthread_self(), "shortname"));
    char name[NAME_SIZE];
    EXPECT_EQ(0, pthread_getname_np(pthread_self(), name, sizeof(name)));
    EXPECT_STREQ("shortname", name);

    EXPECT_EQ(0, pthread_setname_np(pthread_self(), "uselongnametest"));
    EXPECT_EQ(0, pthread_getname_np(pthread_self(), name, sizeof(name)));
    EXPECT_STREQ("uselongnametest", name);

    EXPECT_EQ(ERANGE, pthread_setname_np(pthread_self(), "namecannotoversixth"));

    EXPECT_EQ(0, pthread_getname_np(pthread_self(), name, BUF_LEN));
    EXPECT_EQ(ERANGE, pthread_getname_np(pthread_self(), name, BUF_LEN - 1));
    return nullptr;
}

static void* ReturnFunc(void* arg)
{
    return arg;
}

/**
 * @tc.name: pthread_setname_np_001
 * @tc.desc: 1. Set a unique name with a char type size of 32, and determine if it is the same after obtaining it.
 *           2. Failed to set a name smaller than 16 bytes for the thread, with a minimum of 16 bytes
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_setname_np_001, TestSize.Level1)
{
    void* arg = nullptr;
    PthreadNameNp(arg);
}

/**
 * @tc.name: pthread_setname_np_002
 * @tc.desc: In a multithreaded environment, setting flags to control the operation of a thread ensures that the logic
 *           of setting a name for the thread can be used normally during multithreaded operation.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_setname_np_002, TestSize.Level1)
{
    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, nullptr, PthreadNameNp, nullptr));
    EXPECT_EQ(0, pthread_join(thread, nullptr));
}

/**
 * @tc.name: pthread_setname_np_003
 * @tc.desc: Set a dead process and set a unique name for the dead process for death testing, returning 'Death'
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_setname_np_003, TestSize.Level1)
{
    pthread_t deadThread;
    pthread_create(&deadThread, nullptr, ReturnFunc, nullptr);
    pthread_join(deadThread, nullptr);

    EXPECT_DEATH(pthread_setname_np(deadThread, "short name"), ".*");
}

/**
 * @tc.name: pthread_setname_np_004
 * @tc.desc: Set a null process and set a unique name for the dead process for death testing, returning 'Death'
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_setname_np_004, TestSize.Level1)
{
    pthread_t nullThread = 0;
    EXPECT_DEATH(pthread_setname_np(nullThread, "short name"), ".*");
}

/**
 * @tc.name: pthread_getname_np_001
 * @tc.desc: Set a dead process and set a unique name for the dead process for death testing, returning 'Death'
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_getname_np_001, TestSize.Level1)
{
    pthread_t deadThread;
    pthread_create(&deadThread, nullptr, ReturnFunc, nullptr);
    pthread_join(deadThread, nullptr);
    char name[NAME_LONG_SIZE];
    EXPECT_DEATH(pthread_getname_np(deadThread, name, sizeof(name)), ".*");
}

/**
 * @tc.name: pthread_getname_np_002
 * @tc.desc: Set a null process and set a unique name for the dead process for death testing, returning 'Death'
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_getname_np_002, TestSize.Level1)
{
    pthread_t nullThread = 0;
    char name[NAME_LONG_SIZE];
    EXPECT_DEATH(pthread_getname_np(nullThread, name, sizeof(name)), ".*");
}

/**
 * @tc.name: pthread_sigmask_001
 * @tc.desc: Modify the status of the SIGUSR1 signal and verify that the results are correct.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_sigmask_001, TestSize.Level1)
{
    sigset_t originalPthreadSignal;
    sigset_t loadPthreadSignal;
    sigset_t finalPthreadSignal;

    sigemptyset(&originalPthreadSignal);
    EXPECT_EQ(0, pthread_sigmask(SIG_BLOCK, nullptr, &originalPthreadSignal));
    EXPECT_FALSE(sigismember(&originalPthreadSignal, SIGUSR1));
    sigemptyset(&loadPthreadSignal);
    sigaddset(&loadPthreadSignal, SIGUSR1);
    EXPECT_EQ(0, pthread_sigmask(SIG_BLOCK, &loadPthreadSignal, nullptr));
    sigemptyset(&finalPthreadSignal);
    EXPECT_EQ(0, pthread_sigmask(SIG_BLOCK, nullptr, &finalPthreadSignal));
    EXPECT_TRUE(sigismember(&finalPthreadSignal, SIGUSR1));
}

static void* WaitSignalHandler(void* arg)
{
    sigset_t waitSetBegin;
    sigfillset(&waitSetBegin);
    return reinterpret_cast<void*>(sigwait(&waitSetBegin, reinterpret_cast<int*>(arg)));
}

/**
 * @tc.name: pthread_sigmask_002
 * @tc.desc: Create a new thread, wait for the SIGUSR1 signal to be received, and verify that the received signal
 *           is correct
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_sigmask_002, TestSize.Level1)
{
    sigset_t originalPthreadSignal;
    pthread_t signalThread;
    int receivedPthreadSignal;

    sigemptyset(&originalPthreadSignal);
    EXPECT_EQ(0, pthread_sigmask(SIG_BLOCK, &originalPthreadSignal, nullptr));
    EXPECT_FALSE(sigismember(&originalPthreadSignal, SIGUSR1));
    EXPECT_EQ(0, pthread_create(&signalThread, nullptr, WaitSignalHandler, &receivedPthreadSignal));
    pthread_kill(signalThread, SIGUSR1);
    pthread_join(signalThread, nullptr);
    EXPECT_EQ(SIGUSR1, receivedPthreadSignal);
}

/**
 * @tc.name: pthread_create_001
 * @tc.desc: The normal function creation was successfully set, and the thread returned the same result as the initial
 *           creation
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_create_001, TestSize.Level1)
{
    void* getRes = reinterpret_cast<void*>(666);

    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, nullptr, ReturnFunc, getRes));

    void* realRes;
    EXPECT_EQ(0, pthread_join(thread, &realRes));
    EXPECT_EQ(getRes, realRes);
}

static void* ThreadStr(void* arg)
{
    g_dtorOutput += *reinterpret_cast<std::string*>(arg);
    return nullptr;
}

/**
 * @tc.name: pthread_create_002
 * @tc.desc: When creating and waiting for a thread, the specified function of the object in the thread's
 *           local storage can be correctly triggered, and the expected output result of the specified
 *           function can be obtained in the main thread
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_create_002, TestSize.Level1)
{
    std::string msg("dtor");
    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, nullptr, ThreadStr, &msg));
    EXPECT_EQ(0, pthread_join(thread, nullptr));
    EXPECT_EQ("dtor", g_dtorOutput);
}

/**
 * @tc.name: pthread_setcancelstate_001
 * @tc.desc: Verify that the thread cancellation state was successfully set and verify the old state
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_setcancelstate_001, TestSize.Level1)
{
#ifdef FEATURE_PTHREAD_CANCEL
    int oldstate;
    EXPECT_EQ(0, pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate));
    EXPECT_EQ(oldstate, PTHREAD_CANCEL_ENABLE);

    EXPECT_EQ(0, pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate));
    EXPECT_EQ(oldstate, PTHREAD_CANCEL_DISABLE);

    EXPECT_EQ(0, pthread_setcancelstate(PTHREAD_CANCEL_MASKED, &oldstate));
    EXPECT_EQ(oldstate, PTHREAD_CANCEL_ENABLE);

    EXPECT_EQ(0, pthread_setcancelstate(PTHREAD_CANCEL_DEFERRED, &oldstate));
    EXPECT_EQ(oldstate, PTHREAD_CANCEL_MASKED);

    EXPECT_EQ(0, pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, &oldstate));
    EXPECT_EQ(oldstate, PTHREAD_CANCEL_DEFERRED);
#endif
}

/**
 * @tc.name: pthread_kill_001
 * @tc.desc: When creating and waiting for a thread, the specified function of the object in the thread's
 *           local storage can be correctly triggered, and the expected output result of the specified
 *           function can be obtained in the main thread.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_kill_001, TestSize.Level1)
{
    EXPECT_EQ(0, pthread_kill(pthread_self(), 0));
}

/**
 * @tc.name: pthread_kill_002
 * @tc.desc: Sending invalid signal to current thread, returning error.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_kill_002, TestSize.Level1)
{
    EXPECT_EQ(EINVAL, pthread_kill(pthread_self(), -6));
}

/**
 * @tc.name: pthread_kill_003
 * @tc.desc: sending a signal to an empty thread returned an error
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_kill_003, TestSize.Level1)
{
    pthread_t nullThread = 0;
    EXPECT_DEATH(pthread_kill(nullThread, 0), ".*");
}

/**
 * @tc.name: pthread_setschedparam_001
 * @tc.desc: Testing the interface using dead threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_setschedparam_001, TestSize.Level1)
{
    pthread_t deadThread;
    pthread_create(&deadThread, nullptr, ReturnFunc, nullptr);
    pthread_join(deadThread, nullptr);

    int loadPolicy = 0;
    sched_param pthreadSchedParam;
    EXPECT_DEATH(pthread_setschedparam(deadThread, loadPolicy, &pthreadSchedParam), ".*");
}

/**
 * @tc.name: pthread_setschedparam_002
 * @tc.desc: Testing the interface using null threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_setschedparam_002, TestSize.Level1)
{
    pthread_t nullThread = 0;
    int loadPolicy = 0;
    sched_param pthreadSchedParam;
    EXPECT_DEATH(pthread_setschedparam(nullThread, loadPolicy, &pthreadSchedParam), ".*");
}

/**
 * @tc.name: pthread_setschedparam_003
 * @tc.desc: Correct handling of invalid scheduling loadPolicy values
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_setschedparam_003, TestSize.Level1)
{
    sched_param pthreadSchedParam = { .sched_priority = -6 };
    EXPECT_EQ(EINVAL, pthread_setschedparam(pthread_self(), -6, &pthreadSchedParam));
}

/**
 * @tc.name: pthread_getschedparam_001
 * @tc.desc: Testing the interface using dead threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_getschedparam_001, TestSize.Level1)
{
    pthread_t deadThread;
    pthread_create(&deadThread, nullptr, ReturnFunc, nullptr);
    pthread_join(deadThread, nullptr);

    int loadPolicy;
    sched_param pthreadSchedParam;
    EXPECT_DEATH(pthread_getschedparam(deadThread, &loadPolicy, &pthreadSchedParam), ".*");
}

/**
 * @tc.name: pthread_setschedprio_001
 * @tc.desc: Testing the interface using dead threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_setschedprio_001, TestSize.Level1)
{
    pthread_t deadThread;
    pthread_create(&deadThread, nullptr, ReturnFunc, nullptr);
    pthread_join(deadThread, nullptr);

    EXPECT_DEATH(pthread_setschedprio(deadThread, 10), ".*");
}

/**
 * @tc.name: pthread_setschedprio_002
 * @tc.desc: Testing the interface using null threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_setschedprio_002, TestSize.Level1)
{
    pthread_t nullThread = 0;
    EXPECT_DEATH(pthread_setschedprio(nullThread, 10), ".*");
}

/**
 * @tc.name: pthread_setschedprio_003
 * @tc.desc: Correct handling of invalid scheduling loadPolicy values
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_setschedprio_003, TestSize.Level1)
{
    EXPECT_EQ(EINVAL, pthread_setschedprio(pthread_self(), 1000));
}

/**
 * @tc.name: pthread_getcpuclockid_001
 * @tc.desc: Successfully called the function on the thread
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_getcpuclockid_001, TestSize.Level1)
{
    g_controlFlag = true;
    pthread_t thread;
    clockid_t realClock;
    timespec times;
    EXPECT_EQ(0, pthread_create(&thread, nullptr, BeginLoop, nullptr));
    EXPECT_EQ(0, pthread_getcpuclockid(thread, &realClock));
    EXPECT_EQ(0, clock_gettime(realClock, &times));
    CancelLoop();
    EXPECT_EQ(0, pthread_join(thread, nullptr));
}

/**
 * @tc.name: pthread_getcpuclockid_002
 * @tc.desc: Using dead threads for death testing This interface returns invalid threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_getcpuclockid_002, TestSize.Level1)
{
    pthread_t deadThread;
    clockid_t realClock;
    pthread_create(&deadThread, nullptr, ReturnFunc, nullptr);
    pthread_join(deadThread, nullptr);
    EXPECT_DEATH(pthread_getcpuclockid(deadThread, &realClock), ".*");
}

/**
 * @tc.name: pthread_getcpuclockid_003
 * @tc.desc: Using null threads for death testing This interface returns invalid threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_getcpuclockid_003, TestSize.Level1)
{
    pthread_t nullThread = 0;
    clockid_t c;
    EXPECT_DEATH(pthread_getcpuclockid(nullThread, &c), ".*");
}

static void* JoinThrd(void* arg)
{
    return reinterpret_cast<void*>(pthread_join(reinterpret_cast<pthread_t>(arg), nullptr));
}

/**
 * @tc.name: pthread_join_002
 * @tc.desc: Create thread thread1, strongly convert thread1 into a parameter of thread thread2, and set the destructor
 * of thread2 to add thread1 to pthread_join, call pthread again_ Invalid return for detach (thread1), determine if
 * thread1 does not end with a detached attribute
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_join_002, TestSize.Level1)
{
    g_controlFlag = true;
    pthread_t thread1;
    pthread_create(&thread1, nullptr, BeginLoop, nullptr);
    pthread_t thread2;
    pthread_create(&thread2, nullptr, JoinThrd, reinterpret_cast<void*>(thread1));

    sleep(1);
    EXPECT_DEATH(pthread_detach(thread1), ".*");
    pthread_attr_t detachAttr;
    EXPECT_DEATH(pthread_getattr_np(thread1, &detachAttr), ".*");
    int detachState;
    pthread_attr_getdetachstate(&detachAttr, &detachState);
    pthread_attr_destroy(&detachAttr);
    EXPECT_EQ(PTHREAD_CREATE_JOINABLE, detachState);

    CancelLoop();

    void* joinResult;
    EXPECT_EQ(0, pthread_join(thread2, &joinResult));
    EXPECT_EQ(0U, reinterpret_cast<uintptr_t>(joinResult));
}

/**
 * @tc.name: pthread_join_003
 * @tc.desc: Test that two identical threads call the function successively. The first time it succeeds, the second time
 *           it fails, and finally verify the results of the first call
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_join_003, TestSize.Level1)
{
    g_controlFlag = true;
    pthread_t thread1;
    pthread_create(&thread1, nullptr, BeginLoop, nullptr);

    pthread_t thread2;
    pthread_create(&thread2, nullptr, JoinThrd, reinterpret_cast<void*>(thread1));

    sleep(1);

    EXPECT_DEATH(pthread_join(thread1, nullptr), ".*");

    CancelLoop();

    void* joinResult;
    EXPECT_EQ(0, pthread_join(thread2, &joinResult));
    EXPECT_EQ(0U, reinterpret_cast<uintptr_t>(joinResult));
}

/**
 * @tc.name: pthread_join_004
 * @tc.desc: Testing the interface using dead threads returns invalid threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_join_004, TestSize.Level1)
{
    pthread_t deadThread;
    pthread_create(&deadThread, nullptr, ReturnFunc, nullptr);
    pthread_join(deadThread, nullptr);

    EXPECT_DEATH(pthread_join(deadThread, nullptr), ".*");
}

/**
 * @tc.name: pthread_join_005
 * @tc.desc: Testing the interface using null threads returns invalid threads
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdDeathTest, pthread_join_005, TestSize.Level1)
{
    pthread_t nullThread = 0;
    EXPECT_DEATH(pthread_join(nullThread, nullptr), ".*");
}

struct PthreadGettidArg {
    pid_t tid = 0;
    pthread_mutex_t mutex;
};

static void* ThreadGettidNpFn(PthreadGettidArg* arg)
{
    if (arg == nullptr) {
        return nullptr;
    }
    arg->tid = gettid();
    pthread_mutex_lock(&arg->mutex);
    pthread_mutex_unlock(&arg->mutex);
    return nullptr;
}

/**
 * @tc.name: pthread_gettid_np_001
 * @tc.desc: Get the ID of the current thread for comparison and equality.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_gettid_np_001, TestSize.Level1)
{
    EXPECT_EQ(gettid(), pthread_gettid_np(pthread_self()));
}

/**
 * @tc.name: pthread_gettid_np_002
 * @tc.desc: 1. Get the ID of the current thread for comparison and equality.
 *           2. Call the getid function in the new thread to get the ID equal to the main function ID

 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthrdTest, pthread_gettid_np_002, TestSize.Level1)
{
    pthread_t thread;
    pthread_mutex_t pMutex = PTHREAD_MUTEX_INITIALIZER;
    PthreadGettidArg arg;
    arg.mutex = pMutex;
    pthread_mutex_lock(&arg.mutex);
    pthread_create(&thread, nullptr, reinterpret_cast<void* (*)(void*)>(ThreadGettidNpFn), &arg);
    pthread_mutex_unlock(&arg.mutex);
    pid_t resultTid = pthread_gettid_np(thread);
    EXPECT_EQ(0, pthread_join(thread, nullptr));
    EXPECT_EQ(arg.tid, resultTid);
}

void* ThreadExitFunc(void* arg)
{
    const char* message = "pthread_exit";
    pthread_exit(const_cast<void*>(static_cast<const void*>(message)));
}

/**
 * @tc.name: pthread_exit_001
 * @tc.desc: Verify pthread_exit process success
 * @tc.type: FUNC
 */
HWTEST_F(ThreadPthrdTest, pthread_exit_001, TestSize.Level1)
{
    pthread_t ph;
    EXPECT_EQ(0, pthread_create(&ph, nullptr, ThreadExitFunc, nullptr));
    void* resultFn = nullptr;
    EXPECT_EQ(0, pthread_join(ph, &resultFn));
    EXPECT_STREQ("pthread_exit", (char*)resultFn);
}