/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef _SSPRET_H
#define _SSPRET_H

#if defined(__SSP_RET_STRONG__) || defined(__SSP_RET_ALL__)
#if defined(__aarch64__)
# define SSPRET_CALC_RETCOOKIE(reg) \
    eor     reg, reg, x30

# define SSPRET_LOAD_COOKIE(x, reg) \
    mov     reg, x29

# define SSPRET_SETUP(x, reg) \
    SSPRET_LOAD_COOKIE(x, reg); \
    SSPRET_CALC_RETCOOKIE(reg)

# define SSPRET_CHECK(x, reg) \
    SSPRET_LOAD_COOKIE(x, x9); \
    SSPRET_CALC_RETCOOKIE(x9); \
    subs    reg, reg, x9; \
    cbz     reg, 6788f; \
    brk     #0x1; \
6788:

# define SSPRET_PUSH(reg) \
    str     reg, [sp, #-16]!

# define SSPRET_POP(reg) \
    ldr     reg, [sp, #16]!

#endif /* __aarch64__ */
#else
# define SSPRET_CALC_RETCOOKIE(reg)
# define SSPRET_LOAD_COOKIE(x, reg)
# define SSPRET_SETUP(x, reg)
# define SSPRET_CHECK(x, reg)
# define SSPRET_PUSH(reg)
# define SSPRET_POP(reg)
#endif /* __SSP_RET_STRONG__ || __SSP_RET_ALL__ */

#endif /* _SSPRET_H */
