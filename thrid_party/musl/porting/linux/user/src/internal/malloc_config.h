/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MALLOC_CONFIG_H
#define MALLOC_CONFIG_H

#ifdef MALLOC_SECURE_ALL

#ifndef MALLOC_FREELIST_HARDENED
#define MALLOC_FREELIST_HARDENED
#endif

#ifndef MALLOC_FREELIST_QUARANTINE
#define MALLOC_FREELIST_QUARANTINE
#endif

#ifndef MALLOC_RED_ZONE
#define MALLOC_RED_ZONE
#endif

#endif // MALLOC_SECURE_ALL

#if defined(MALLOC_FREELIST_QUARANTINE) && !defined(MALLOC_FREELIST_HARDENED)
#error MALLOC_FREELIST_QUARANTINE can be only applied when MALLOC_FREELIST_HARDENED is set.
#endif

#if defined(MALLOC_RED_ZONE) && !defined(MALLOC_FREELIST_QUARANTINE)
#error MALLOC_RED_ZONE can be only applied when MALLOC_FREELIST_QUARANTINE is set.
#endif

#endif // MALLOC_CONFIG_H
