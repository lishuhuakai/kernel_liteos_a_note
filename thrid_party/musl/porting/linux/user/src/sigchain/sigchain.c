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
#include <locale.h>
#include <pthread.h>
#include <errno.h>
#include <threads.h>
#include <musl_log.h>
#include <stdlib.h>
#include <errno.h>
#include "syscall.h"

extern int __libc_sigaction(int sig, const struct sigaction *restrict sa,
                            struct sigaction *restrict old);

#define SIG_CHAIN_KEY_VALUE_1 1
#define SIGNAL_CHAIN_SPECIAL_ACTION_MAX 3

#define SIGCHAIN_LOG_TAG "MUSL-SIGCHAIN"

#if (defined(OHOS_ENABLE_PARAMETER) || defined(ENABLE_MUSL_LOG))
#define SIGCHAIN_PRINT_ERROR(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_ERROR, \
    MUSL_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#define SIGCHAIN_PRINT_INFO(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_INFO, \
    MUSL_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#define SIGCHAIN_PRINT_DEBUG(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_DEBUG, \
    MUSL_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#define SIGCHAIN_LOG_FATAL(...) ((void)HiLogAdapterPrint(LOG_CORE, LOG_FATAL, \
    MUSL_LOG_DOMAIN, SIGCHAIN_LOG_TAG, __VA_ARGS__))
#else
#define SIGCHAIN_PRINT_ERROR(...)
#define SIGCHAIN_PRINT_INFO(...)
#define SIGCHAIN_PRINT_DEBUG(...)
#define SIGCHAIN_LOG_FATAL(...)
#endif

#define SIGCHAIN_PRINT_FATAL(...)  do {                    \
    SIGCHAIN_LOG_FATAL(__VA_ARGS__);                      \
    abort();                                               \
} while(0)

struct sc_signal_chain {
    bool marked;
    struct sigaction sig_action;
    struct signal_chain_action sca_special_actions[SIGNAL_CHAIN_SPECIAL_ACTION_MAX];
};

/* Signal chain set, from 0 to 63. */
static struct sc_signal_chain sig_chains[_NSIG - 1];
/* static thread Keyword */
static pthread_key_t g_sigchain_key;
/* This is once flag! */
static once_flag g_flag = ONCE_FLAG_INIT;

/**
  * @brief Create the thread key
  * @retval void
  */
static void create_pthread_key(void)
{
    SIGCHAIN_PRINT_INFO("%{public}s create the thread key!", __func__);
    int rc = pthread_key_create(&g_sigchain_key, NULL);
    if (rc != 0) {
        SIGCHAIN_PRINT_FATAL("%{public}s failed to create sigchain pthread key, rc:%{public}d",
                __func__,  rc);
    }
}

/**
  * @brief Get the key of the signal thread.
  * @retval int32_t, the value of the sigchain key.
  */
static pthread_key_t get_handling_signal_key()
{
    call_once(&g_flag, create_pthread_key);
    return g_sigchain_key;
}

/**
  * @brief Get the value of the sigchain key
  * @retval bool, true if set the value of the key，or false.
  */
static bool get_handling_signal()
{
    void *result = pthread_getspecific(get_handling_signal_key());
    return result == NULL ? false : true;
}

/**
  * @brief Set the value of the sigchain key
  * @param[in] value, the value of the sigchain key
  * @retval void.
  */
static void set_handling_signal(bool value)
{
    pthread_setspecific(get_handling_signal_key(),
                        (void *)((uintptr_t)(value)));
}

/**
  * @brief Set the mask of the system. Its prototype comes from pthread_sigmask.
  * @param[in] how, the value of the mask operation .
  * @param[in] set, the new value of the sigset.
  * @param[in] old, the old value of the sigset.
  */
static int sigchain_sigmask(int how, const sigset_t *restrict set, sigset_t *restrict old)
{
    int ret;
    if (set && (unsigned)how - SIG_BLOCK > 2U) return EINVAL;
    ret = -__syscall(SYS_rt_sigprocmask, how, set, old, _NSIG/8);
    if (!ret && old) {
        if (sizeof old->__bits[0] == 8) {
            old->__bits[0] &= ~0x380000000ULL;
        } else {
            old->__bits[0] &= ~0x80000000UL;
            old->__bits[1] &= ~0x3UL;
        }
    }
    return ret;
}

/**
  * @brief Judge whether the signal is marked
  * @param[in] signo, the value of the signal.
  * @retval true if the signal is marked, or false.
  */
static bool ismarked(int signo)
{
    return sig_chains[signo - 1].marked;
}

/**
  * @brief This is a callback function, which is registered to the kernel
  * @param[in] signo, the value of the signal.
  * @param[in] siginfo, the information of the signal.
  * @param[in] ucontext_raw, the context of the signal.
  * @retval void
  */
static void signal_chain_handler(int signo, siginfo_t* siginfo, void* ucontext_raw)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    /* First call special handler. */
    /* If a process crashes, the sigchain'll call the corresponding  handler */
    if (!get_handling_signal()) {
        for (int i = 0; i < SIGNAL_CHAIN_SPECIAL_ACTION_MAX; i++) {
            if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction == NULL) {
                continue;
            }
            /* The special handler might not return. */
            bool noreturn = (sig_chains[signo - 1].sca_special_actions[i].sca_flags &
                             SIGCHAIN_ALLOW_NORETURN);
            sigset_t previous_mask;
            sigchain_sigmask(SIG_SETMASK, &sig_chains[signo - 1].sca_special_actions[i].sca_mask,
                            &previous_mask);

            bool previous_value = get_handling_signal();
            if (!noreturn) {
                set_handling_signal(true);
            }
            SIGCHAIN_PRINT_ERROR("%{public}s call %{public}d rd sigchain action for signal: %{public}d", __func__, i, signo);
            if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction(signo,
                                                            siginfo, ucontext_raw)) {
                set_handling_signal(previous_value);
                SIGCHAIN_PRINT_ERROR("%{public}s call %{public}d rd sigchain action for signal: %{public}d directly return", __func__, i, signo);
                return;
            }

            sigchain_sigmask(SIG_SETMASK, &previous_mask, NULL);
            set_handling_signal(previous_value);
        }
    }
    /* Then Call the user's signal handler */
    int sa_flags = sig_chains[signo - 1].sig_action.sa_flags;
    ucontext_t* ucontext = (ucontext_t*)(ucontext_raw);

    sigset_t mask;
    sigorset(&mask, &ucontext->uc_sigmask, &sig_chains[signo - 1].sig_action.sa_mask);

    if (!(sa_flags & SA_NODEFER)) {
        sigaddset(&mask, signo);
    }

    sigchain_sigmask(SIG_SETMASK, &mask, NULL);

    if ((sa_flags & SA_SIGINFO)) {
        SIGCHAIN_PRINT_INFO("%{public}s call usr sigaction for signal:"
            "%{public}d sig_action.sa_sigaction=%{public}lx",
            __func__, signo, (unsigned long)sig_chains[signo - 1].sig_action.sa_sigaction);
        sig_chains[signo - 1].sig_action.sa_sigaction(signo, siginfo, ucontext_raw);
    } else {
        if (sig_chains[signo - 1].sig_action.sa_handler == SIG_IGN) {
            SIGCHAIN_PRINT_ERROR("%{public}s SIG_IGN handler for signal: %{public}d", __func__, signo);
            return;
        } else if (sig_chains[signo - 1].sig_action.sa_handler == SIG_DFL) {
            SIGCHAIN_PRINT_ERROR("%{public}s SIG_DFL handler for signal: %{public}d", __func__, signo);
            remove_all_special_handler(signo);
            if (__syscall(SYS_rt_tgsigqueueinfo, __syscall(SYS_getpid), __syscall(SYS_gettid), signo, siginfo) != 0) {
                SIGCHAIN_PRINT_ERROR("Failed to rethrow sig(%{public}d), errno(%{public}d).", signo, errno);
            } else {
                SIGCHAIN_PRINT_ERROR("pid(%{public}d) rethrow sig(%{public}d) success.", __syscall(SYS_getpid), signo);
            }
        } else {
            SIGCHAIN_PRINT_INFO("%{public}s call usr sa_handler: %{public}p for signal:"
                "%{public}d sig_action.sa_handler=%{public}lx",
                __func__, signo, (unsigned long)sig_chains[signo - 1].sig_action.sa_handler);
            sig_chains[signo - 1].sig_action.sa_handler(signo);
        }
    }

    return;
}

/**
  * @brief Register the signal chain with the kernel if needed
  * @param[in] signo, the value of the signal.
  * @retval void
  */
static void sigchain_register(int signo)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    struct sigaction signal_action = {};
    sigfillset(&signal_action.sa_mask);

    signal_action.sa_sigaction = signal_chain_handler;
    signal_action.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
    __libc_sigaction(signo, &signal_action, &sig_chains[signo - 1].sig_action);
}

/**
  * @brief Unregister the signal from sigchain, register the signal's user handler with the kernel if needed
  * @param[in] signo, the value of the signal.
  * @retval void
  */
static void unregister_sigchain(int signo)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    __libc_sigaction(signo, &sig_chains[signo - 1].sig_action, NULL);
    sig_chains[signo - 1].marked = false;
}

/**
  * @brief Mark the signal to the sigchain.
  * @param[in] signo, the value of the signal.
  * @retval void
  */
static void mark_signal_to_sigchain(int signo)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (!sig_chains[signo - 1].marked) {
        sigchain_register(signo);
        sig_chains[signo - 1].marked = true;
    }
}

/**
  * @brief Set the action of the signal.
  * @param[in] signo, the value of the signal.
  * @param[in] new_sa, the new action of the signal.
  * @retval void
  */
static void setaction(int signo, const struct sigaction *restrict new_sa)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    sig_chains[signo - 1].sig_action = *new_sa;
}

/**
  * @brief Get the action of the signal.
  * @param[in] signo, the value of the signal.
  * @retval The current action of the signal
  */
static struct sigaction getaction(int signo)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    return sig_chains[signo - 1].sig_action;
}

/**
  * @brief Add the special handler to the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] sa, the action with special handler.
  * @retval void
  */
static void add_special_handler(int signo, struct signal_chain_action* sa)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    for (int i = 0; i < SIGNAL_CHAIN_SPECIAL_ACTION_MAX; i++) {
        if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction == NULL) {
            sig_chains[signo - 1].sca_special_actions[i] = *sa;
            SIGCHAIN_PRINT_INFO("%{public}s signo %{public}d is registered with special handler!", __func__, signo);
            return;
        }
    }

    SIGCHAIN_PRINT_FATAL("Add too many the special handlers!");
}

/**
  * @brief Remove the special handler from the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] fn, the special handler of the signal.
  * @retval void
  */
static void rm_special_handler(int signo, bool (*fn)(int, siginfo_t*, void*))
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    int len = SIGNAL_CHAIN_SPECIAL_ACTION_MAX;
    for (int i = 0; i < len; i++) {
        if (sig_chains[signo - 1].sca_special_actions[i].sca_sigaction == fn) {
            sig_chains[signo - 1].sca_special_actions[i].sca_sigaction = NULL;
            int count = 0;
            for (int k = 0; k < len; k++) {
                if (sig_chains[signo - 1].sca_special_actions[k].sca_sigaction == NULL) {
                    count++;
                }
            }
            if (count == len) {
                unregister_sigchain(signo);
            }
            return;
        }
    }

    SIGCHAIN_PRINT_FATAL("%{public}s failed to remove the special handler!. signo: %{public}d",
            __func__, signo);
}

/**
  * @brief This is an external interface, 
  *        Mark the signal to sigchain ,add the special handler to the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] sa, the action with special handler.
  * @retval void
  */
void add_special_signal_handler(int signo, struct signal_chain_action* sa)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_FATAL("%{public}s Invalid signal %{public}d", __func__, signo);
        return;
    }

    // Add the special hander to the sigchain
    add_special_handler(signo, sa);
    mark_signal_to_sigchain(signo);
}

/**
  * @brief This is an external interface, remove the special handler from the sigchain.
  * @param[in] signo, the value of the signal.
  * @param[in] fn, the special handler of the signal.
  * @retval void
  */
void remove_special_signal_handler(int signo, bool (*fn)(int, siginfo_t*, void*))
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_FATAL("%{public}s Invalid signal %{public}d", __func__, signo);
        return;
    }

    if (ismarked(signo)) {
        // remove the special handler from the sigchain.
        rm_special_handler(signo, fn);
    }
}

/**
  * @brief This is an external interface, remove all special handler from the sigchain.
  * @param[in] signo, the value of the signal.
  * @retval void
  */
void remove_all_special_handler(int signo)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_FATAL("%{public}s Invalid signal %{public}d", __func__, signo);
        return;
    }

    if (ismarked(signo)) {
        // remove all special handler from the sigchain.
        for (int i = 0; i < SIGNAL_CHAIN_SPECIAL_ACTION_MAX; i++) {
            sig_chains[signo - 1].sca_special_actions[i].sca_sigaction = NULL;
        }
        unregister_sigchain(signo);
    }
}

/**
  * @brief This is an external interface, add the special handler at the last of sigchain chains.
  * @param[in] signo, the value of the signal.
  * @param[in] sa, the action with special handler.
  * @retval void
  */
void add_special_handler_at_last(int signo, struct signal_chain_action* sa)
{
    SIGCHAIN_PRINT_INFO("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_FATAL("%{public}s Invalid signal %{public}d", __func__, signo);
        return;
    }

    if (sig_chains[signo - 1].sca_special_actions[SIGNAL_CHAIN_SPECIAL_ACTION_MAX - 1].sca_sigaction == NULL) {
        sig_chains[signo - 1].sca_special_actions[SIGNAL_CHAIN_SPECIAL_ACTION_MAX - 1] = *sa;
        mark_signal_to_sigchain(signo);
        return;
    }

    SIGCHAIN_PRINT_FATAL("Add too many the special handlers at last!");
}

/**
  * @brief Intercept the signal and sigaction.
  * @param[in] signo, the value of the signal.
  * @param[in] sa, the new action with the signal handler.
  * @param[out] old, the old action with the signal handler.
  * @retval true if the signal if intercepted, or false.
  */
bool intercept_sigaction(int signo, const struct sigaction *restrict sa,
                         struct sigaction *restrict old)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s signo: %{public}d", __func__, signo);
    if (signo <= 0 || signo >= _NSIG) {
        SIGCHAIN_PRINT_ERROR("%{public}s Invalid signal %{public}d", __func__, signo);
        return false;
    }

    if (ismarked(signo)) {
        struct sigaction saved_action = getaction(signo);

        if (sa != NULL) {
            setaction(signo, sa);
        }
        if (old != NULL) {
            *old = saved_action;
        }
        return true;
    }

    return false;
}

/**
  * @brief Intercept the pthread_sigmask.
  * @param[in] how, the value of the mask operation .
  * @param[out] set, the value of the sigset.
  * @retval void.
  */
void intercept_pthread_sigmask(int how, sigset_t *restrict set)
{
    SIGCHAIN_PRINT_DEBUG("%{public}s how: %{public}d", __func__, how);
    // Forward directly to the system mask When this sigchain is handling a signal.
    if (get_handling_signal()) {
        return;
    }

    sigset_t tmpset;
    if (set != NULL) {
        tmpset = *set;
        if (how == SIG_BLOCK || how == SIG_SETMASK) {
            for (int i = 1; i < _NSIG; ++i) {
                if (ismarked(i) && sigismember(&tmpset, i)) {
                    sigdelset(&tmpset, i);
                }
            }
        }
        *set = tmpset;
    }

    return;
}
