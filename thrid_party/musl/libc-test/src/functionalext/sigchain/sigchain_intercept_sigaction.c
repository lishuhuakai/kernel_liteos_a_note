/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sigchain.h>
#include <signal.h>
#include <wchar.h>
#include <stdlib.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

static int g_count = 0;
/**
 * @brief the special handler
 */
static void signal_handler1(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_001", signo, SIGHUP);
}

/**
 * @tc.name      : sigchain_intercept_sigaction_001
 * @tc.desc      : The signal is not registered with the special handler, test the influence of sigchain
 *                 on sigaction.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_001()
{
    struct sigaction siga1 = {
        .sa_handler = signal_handler1,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGHUP, &siga1, NULL);

    raise(SIGHUP);
    EXPECT_EQ("sigchain_intercept_sigaction_001", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_002", signo, SIGUSR2);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler2(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_002", signo, SIGUSR2);
}


/**
 * @tc.name      : sigchain_intercept_sigaction_002
 * @tc.desc      : The signals is registered with the special handler, test the influence of sigchain on sigaction.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_002()
{
    g_count = 0;
    struct signal_chain_action sigusr2 = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGUSR2, &sigusr2);

    struct sigaction siga2 = {
        .sa_handler = signal_handler2,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGUSR2, &siga2, NULL);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGUSR2};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigaction_002", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }

    raise(SIGUSR2);
    EXPECT_EQ("sigchain_intercept_sigaction_002", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGURG);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler3(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGURG);
}

/**
 * @tc.name      : sigchain_intercept_sigaction_003
 * @tc.desc      : the signal is registered with the special handler, and remove the special handler. Test
 *                 the influence of sigchain on sigaction.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_003()
{
    g_count = 0;
    struct signal_chain_action sigurg = {
        .sca_sigaction = sigchain_special_handler3,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGURG, &sigurg);

    struct sigaction siga2 = {
        .sa_handler = signal_handler3,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGURG, &siga2, NULL);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGURG};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigaction_003", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }

    remove_special_signal_handler(SIGURG, sigchain_special_handler3);
    raise(SIGURG);
    EXPECT_EQ("sigchain_intercept_sigaction_003", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

int main(void)
{
    sigchain_intercept_sigaction_001();
    sigchain_intercept_sigaction_002();
    sigchain_intercept_sigaction_003();
    return t_status;
}