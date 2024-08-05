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
#include <wchar.h>
#include <stdlib.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

static int g_count = 0;

/**
 * @brief the special handler
 */
static bool sigchain_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_handler_call_order_001", signo, SIGHUP);
    sigset_t set = {0};
    int signal[SIGCHIAN_TEST_SIGNAL_NUM_2] = {SIGHUP, SIGUSR2};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_handler_call_order_001", signal, SIGCHIAN_TEST_SIGNAL_NUM_2);
    raise(SIGUSR2);
    g_count++;
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_handler_call_order_001", signo, SIGUSR2);
    EXPECT_EQ("sigchain_handler_call_order_001", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
    g_count++;
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler(int signo)
{
    g_count++;
}

/**
 * @tc.name      : sigchain_handler_call_order_001
 * @tc.desc      : The signals are registered with the special handler, and mask and rasie the signal
 *                 at the special handler. Test the influence of sigchain on pthread_sigmask.
 * @tc.level     : Level 0
 */
static void sigchain_handler_call_order_001()
{
    signal(SIGHUP, signal_handler);
    signal(SIGUSR2, signal_handler);

    struct signal_chain_action sighup = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sighup);

    struct signal_chain_action sigusr2 = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGUSR2, &sigusr2);

    raise(SIGHUP);
    EXPECT_EQ("sigchain_handler_call_order_001", g_count, SIGCHIAN_TEST_SIGNAL_NUM_4);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_handler_call_order_002", signo, SIGPWR);
    g_count++;
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler3(int signo)
{
    EXPECT_EQ("sigchain_handler_call_order_002", signo, SIGPWR);
    EXPECT_EQ("sigchain_handler_call_order_002", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
    g_count++;
    return;
}

/**
 * @tc.name      : sigchain_handler_call_order_002
 * @tc.desc      : Add a special handler for a signal that is registered with
 *                 the kernel (Using signal interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_handler_call_order_002()
{
    g_count = 0;
    signal(SIGPWR, signal_handler3);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler3,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGPWR, &sigsegv);

    raise(SIGPWR);
    EXPECT_EQ("sigchain_handler_call_order_002", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
}

int main(void)
{
    sigchain_handler_call_order_001();
    sigchain_handler_call_order_002();
    return t_status;
}