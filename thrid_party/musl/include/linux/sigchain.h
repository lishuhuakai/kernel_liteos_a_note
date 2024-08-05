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

#ifndef _SIGCHAIN_H
#define _SIGCHAIN_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

static const int SIGCHAIN_ALLOW_NORETURN = 0x1UL;

/* The action of the sigchain. */
struct signal_chain_action {
    bool (*sca_sigaction)(int, siginfo_t*, void*);
    sigset_t sca_mask;
    int sca_flags;
};

/* Mark the signal to the sigchain, add the special handler to the sigchain. */
void add_special_signal_handler(int signo, struct signal_chain_action* sa);
/* Add the special handler at the last of sigchain chains */
void add_special_handler_at_last(int signo, struct signal_chain_action* sa);
/* Remove the special the handler from the sigchain. */
void remove_special_signal_handler(int signo, bool (*fn)(int, siginfo_t*, void*));
/* Remove all special the handler from the sigchain */
void remove_all_special_handler(int signo);

#ifdef __cplusplus
}
#endif

#endif