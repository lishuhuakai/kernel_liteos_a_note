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
static bool sigchain_special_sigabrt_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_001", true);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_sigabrt_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_001", true);
    return true;
}

/**
 * @tc.name      : sigchain_rm_special_handler_001
 * @tc.desc      : The signal is not registered with the kernel, call remove_special_signal_handler to remove
 *                 two special handlers
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_001()
{
    struct signal_chain_action sigcont = {
        .sca_sigaction = sigchain_special_sigabrt_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCONT, &sigcont);

    struct signal_chain_action sigcont1 = {
        .sca_sigaction = sigchain_special_sigabrt_handler2,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGCONT, &sigcont1);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGCONT};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_001", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
    remove_special_signal_handler(SIGCONT, sigchain_special_sigabrt_handler1);
    remove_special_signal_handler(SIGCONT, sigchain_special_sigabrt_handler2);

    raise(SIGCONT);
    EXPECT_EQ("sigchain_rm_special_handler_001", g_count, 0);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_sighup_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_002", true);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_sighup_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_002", true);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_sighup_handler(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_rm_special_handler_002", signo, SIGHUP);
}

/**
 * @tc.name      : sigchain_rm_special_handler_002
 * @tc.desc      : The signal is registered with the kernel(Using signal), call
 *                 remove_special_signal_handler to remove two special handlers
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_002()
{
    g_count = 0;
    signal(SIGHUP, signal_sighup_handler);

    struct signal_chain_action sighup = {
        .sca_sigaction = sigchain_special_sighup_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sighup);

    struct signal_chain_action sighup1 = {
        .sca_sigaction = sigchain_special_sighup_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sighup1);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGHUP};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_002", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
    remove_special_signal_handler(SIGHUP, sigchain_special_sighup_handler1);
    remove_special_signal_handler(SIGHUP, sigchain_special_sighup_handler2);

    raise(SIGHUP);
    EXPECT_EQ("sigchain_rm_special_handler_002", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_sigusr2_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_003", true);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_sigusr2_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_003", true);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_sigusr2_sigaction(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_rm_special_handler_003", signo, SIGUSR2);
}

/**
 * @tc.name      : sigchain_rm_special_handler_003
 * @tc.desc      : The signal is registered with the kernel(Using sigaction), call
 *                 remove_special_signal_handler to remove two special handlers
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_003()
{
    g_count = 0;
    struct sigaction sigac = {
        .sa_handler = signal_sigusr2_sigaction,
    };
    sigaction(SIGUSR2, &sigac, NULL);

    struct signal_chain_action sigusr2 = {
        .sca_sigaction = sigchain_special_sigusr2_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGUSR2, &sigusr2);

    struct signal_chain_action sigusr21 = {
        .sca_sigaction = sigchain_special_sigusr2_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGUSR2, &sigusr21);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGUSR2};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_003", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }

    remove_special_signal_handler(SIGUSR2, sigchain_special_sigusr2_handler1);
    remove_special_signal_handler(SIGUSR2, sigchain_special_sigusr2_handler2);
    raise(SIGUSR2);
    EXPECT_EQ("sigchain_rm_special_handler_003", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_sigterm_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_004", true);
    return true;
}

/**
 * @tc.name      : sigchain_rm_special_handler_004
 * @tc.desc      : The signal is not registered with the kernel, call remove_special_signal_handler to remove
 *                 a special handler.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_004()
{
    g_count = 0;
    struct signal_chain_action sigcont = {
        .sca_sigaction = sigchain_special_sigterm_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCONT, &sigcont);


    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGCONT};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_004", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
    remove_special_signal_handler(SIGCONT, sigchain_special_sigterm_handler);

    raise(SIGCONT);
    EXPECT_EQ("sigchain_rm_special_handler_004", g_count, 0);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_64_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_005", true);
    g_count++;
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_64_handler(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_rm_special_handler_005", signo, SIGCHAIN_SIGNAL_64);
}

/**
 * @tc.name      : sigchain_rm_special_handler_005
 * @tc.desc      : The signal is registered with the kernel(Using signal), call remove_special_signal_handler to remove
 *                 a special handler.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_005()
{
    g_count = 0;
    signal(SIGCHAIN_SIGNAL_64, signal_64_handler);

    struct signal_chain_action sighup = {
        .sca_sigaction = sigchain_special_64_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_64, &sighup);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGCHAIN_SIGNAL_64};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_005", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
    remove_special_signal_handler(SIGCHAIN_SIGNAL_64, sigchain_special_64_handler);

    raise(SIGCHAIN_SIGNAL_64);
    EXPECT_EQ("sigchain_rm_special_handler_005", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_37_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_006", true);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_37_sigaction(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_rm_special_handler_006", signo, SIGCHAIN_SIGNAL_37);
}

/**
 * @tc.name      : sigchain_rm_special_handler_006
 * @tc.desc      : The signal is registered with the kernel(Using sigaction), call remove_special_signal_handler
 *                 to remove a special handler.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_006()
{
    g_count = 0;
    struct sigaction sigac = {
        .sa_handler = signal_37_sigaction,
    };
    sigaction(SIGCHAIN_SIGNAL_37, &sigac, NULL);

    struct signal_chain_action sig37 = {
        .sca_sigaction = sigchain_special_37_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_37, &sig37);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGCHAIN_SIGNAL_37};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_006", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
    remove_special_signal_handler(SIGCHAIN_SIGNAL_37, sigchain_special_37_handler);

    raise(SIGCHAIN_SIGNAL_37);
    EXPECT_EQ("sigchain_rm_special_handler_006", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_quit_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_all_special_handler_007", true);
    return false;
}

static bool sigchain_special_quit_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_all_special_handler_007", true);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_quit_sigaction(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_rm_all_special_handler_007", signo, SIGQUIT);
}

/**
 * @tc.name      : sigchain_rm_all_special_handler_007
 * @tc.desc      : The signal is registered with the kernel(Using sigaction), call remove_special_signal_handler
 *                 to remove a special handler.
 * @tc.level     : Level 0
 */
static void sigchain_rm_all_special_handler_007()
{
    g_count = 0;
    struct sigaction sigac = {
        .sa_handler = signal_quit_sigaction,
    };
    sigaction(SIGQUIT, &sigac, NULL);

    struct signal_chain_action sigquit = {
        .sca_sigaction = sigchain_special_quit_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGQUIT, &sigquit);

    struct signal_chain_action sigquit1 = {
        .sca_sigaction = sigchain_special_quit_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGQUIT, &sigquit1);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGQUIT};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_all_special_handler_007", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
    remove_all_special_handler(SIGQUIT);

    raise(SIGQUIT);
    EXPECT_EQ("sigchain_rm_all_special_handler_007", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

int main(void)
{
    sigchain_rm_special_handler_001();
    sigchain_rm_special_handler_002();
    sigchain_rm_special_handler_003();
    sigchain_rm_special_handler_004();
    sigchain_rm_special_handler_005();
    sigchain_rm_special_handler_006();
    sigchain_rm_all_special_handler_007();
    return t_status;
}