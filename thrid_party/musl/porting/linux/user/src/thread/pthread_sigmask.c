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

#include <signal.h>
#include <errno.h>
#include <sigchain.h>
#include <stddef.h>
#include <hilog_adapter.h>
#include <string.h>
#include "syscall.h"
#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif

extern void intercept_pthread_sigmask(int how, sigset_t *restrict set);
static const char *param_name = "musl.sigchain.procmask";

/**
  * @brief Get whether sigchain mask is enabled
  * @retval True if the sigchain mask is enable, or false.
  */
bool get_sigchain_mask_enable()
{
#ifdef OHOS_ENABLE_PARAMETER
	static CachedHandle sigchain_procmask_handle = NULL;
	if (sigchain_procmask_handle == NULL) {
		sigchain_procmask_handle = CachedParameterCreate(param_name, "false");
	}
	char *param_value = CachedParameterGet(sigchain_procmask_handle);
	if (param_value != NULL) {
		if (strcmp(param_value, "true") == 0) {
			return true;
		}
	}
#endif
	return false;
}

int pthread_sigmask(int how, const sigset_t *restrict set, sigset_t *restrict old)
{
	int ret;
	if (set && (unsigned)how - SIG_BLOCK > 2U) return EINVAL;
	/* sigchain intercepts pthread_sigmask */
	if (set && get_sigchain_mask_enable()) {
		sigset_t tmpset = *set;
		intercept_pthread_sigmask(how, &tmpset);
		const sigset_t *new_set_ptr = &tmpset;
		ret = -__syscall(SYS_rt_sigprocmask, how, new_set_ptr, old, _NSIG/8);
	} else {
		ret = -__syscall(SYS_rt_sigprocmask, how, set, old, _NSIG/8);
	}

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
