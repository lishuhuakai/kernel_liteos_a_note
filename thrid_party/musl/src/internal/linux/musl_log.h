/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef _MUSL_LOG_H
#define _MUSL_LOG_H

#include <hilog_adapter.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MUSL_LOG_TYPE LOG_CORE
#define MUSL_LOG_DOMAIN 0xD003F00
#define MUSL_LOG_TAG "MUSL"

#if (defined(OHOS_ENABLE_PARAMETER) || defined(ENABLE_MUSL_LOG))
#define MUSL_LOGE(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))
#define MUSL_LOGW(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_WARN, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))
#define MUSL_LOGI(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_INFO, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))
#define MUSL_LOGD(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_DEBUG, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))

#else
#define MUSL_LOGE(...)
#define MUSL_LOGW(...)
#define MUSL_LOGI(...)
#define MUSL_LOGD(...)
#endif

#ifdef __cplusplus
}
#endif
#endif
