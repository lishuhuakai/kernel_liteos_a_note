/*
 * Copyright (c) 2022-2022 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef  __ARM_FEATURE_MVE

    .syntax     unified
    .globl      memcmp          @ -- Begin function memcmp
    .p2align    2
    .type       memcmp,%function
memcmp:
    @ r0 = str1
    @ r1 = str2
    @ r2 = count
    .fnstart
    push    {r4, r5, r6, r7, r8, lr}
    pld     [r0, #0]
    pld     [r1, #0]

    /**
     * if (str1 == str2) || (n == 0) return;
     */
    cmp     r0, r1
    it      ne
    cmpne   r2, #0
    beq     Lreturn_0

    /**
     * Determine whether the first byte is different.
     */
    ldrb    r3, [r0]        @ r3 = *str1
    ldrb    r4, [r1]        @ r4 = *str2
    pld     [r0, #64]
    pld     [r1, #64]
    cmp     r3, r4
    itt     ne
    subne   r0, r3, r4
    bne     Lreturn

#ifdef LOSCFG_ARCH_UNALIGNED_EXC
    /**
     * Check address alignment.
     */
    and     r5, r0, #3
    and     r6, r1, #3
    orrs    r5, r6
    bgt     Lunaligned_cmp
#endif

/**
 * Comparing 32 bytes each time, using floating-point registers to improve efficiency.
 */
L32_byte_cmp:
    cmp     r2, #32
    blo     L16_byte_cmp
    sub     r2, r2, #32
    vldmia.32   r0!, {s0 - s7}
    vldmia.32   r1!, {s8 - s15}
    vsub.i8     q0, q0, q2      @ q0: Difference of the first 16 bytes
    vsub.i8     q1, q1, q3      @ q1: Difference of the last 16 bytes
    pld     [r0, #64]
    pld     [r1, #64]

    /**
     * Swap d1 and d2 so that after bitwise OR, 
     * d4 represents the result of the first half of the comparison, 
     * d5 represents the result of the second half of the comparison.
     */
    vmov    d6, d1
    vmov    d1, d2
    vmov    d2, d6
    vorr    q2, q0, q1

    /**
     * Determine whether q2 is zero.
     */
    vmov    r3, r4, d4
    orr     r5, r3, r4
    vmov    r3, r4, d5
    orr     r6, r3, r4
    orr     r5, r5, r6
    cmp     r5, #0
    beq     L32_byte_cmp

L32_byte_diff_pre:
    /**
     * Restore the swapped d1 and d2.
     */
    vmov    d6, d1
    vmov    d1, d2
    vmov    d2, d6

/**
 * Going to the diff branch shows that a certain byte must be different at this time.
 * We use r3 to indicate whether the first half of the multibytes are equal,
 * and r4 to indicate whether the second half of the multibytes are equal.
 */
L32_byte_diff:
    vmov    r3, r4, d4
    orr     r3, r3, r4
    /**
     * Adjust the two pointers back.
     */
    sub     r0, #32
    sub     r1, #32
    cmp     r3, #0
    ittt    eq
    addeq   r0, #16
    addeq   r1, #16
    beq     L16_byte_diff_back
    vmov    r3, r4, d0
    vmov    r5, r6, d1
    b       L16_byte_diff

L16_byte_diff_back:
    vmov    r3, r4, d2
    vmov    r5, r6, d3

L16_byte_diff:
    orr     r7, r3, r4
    cmp     r7, #0
    ittt    eq
    addeq   r0, #8
    addeq   r1, #8
    beq     L8_byte_diff_back
    b       L8_byte_diff

L8_byte_diff_back:
    mov     r3, r5
    mov     r4, r6

L8_byte_diff:
    cmp     r3, #0
    ittt    eq
    addeq   r0, #4
    addeq   r1, #4
    beq     L4_byte_diff

Lunaligned_cmp:
L4_byte_diff:
    ldrb    r5, [r0], #1
    ldrb    r6, [r1], #1
    subs    r5, r5, r6
    beq     L4_byte_diff
    mov     r0, r5
    b       Lreturn

/**
 * The dichotomy handles the case of less than 32 bytes.
 */
L16_byte_cmp:
    cmp     r2, #16
    blo     L8_byte_cmp
    sub     r2, r2, #16
    vldmia.32   r0!, {s0 - s3}
    vldmia.32   r1!, {s8 - s11}
    vsub.i8     q0, q0, q2
    pld     [r0, #64]
    pld     [r1, #64]

    vmov    r3, r4, d0
    orr     r5, r3, r4
    vmov    r3, r4, d1
    orr     r6, r3, r4
    orr     r5, r5, r6
    cmp     r5, #0
    beq     L8_byte_cmp

    sub     r0, #16
    sub     r1, #16
    vmov    r3, r4, d0
    vmov    r5, r6, d1
    b       L16_byte_diff

L8_byte_cmp:
    cmp     r2, #8
    blo     L4_byte_cmp
    sub     r2, r2, #8
    vldmia.32   r0!, {s0 - s1}
    vldmia.32   r1!, {s8 - s9}
    vsub.i8     q0, q0, q2

    vmov    r3, r4, d0
    orr     r7, r3, r4
    cmp     r7, #0
    beq     L4_byte_cmp
    sub     r0, #8
    sub     r1, #8
    b       L8_byte_diff

L4_byte_cmp:
    cmp     r2, #4
    blo     Lless_4_byte_cmp
    sub     r2, r2, #4
    ldr     r3, [r0], #4
    ldr     r4, [r1], #4
    cmp     r3, r4
    beq     Lless_4_byte_cmp
    sub     r0, #4
    sub     r1, #4
    b       L4_byte_diff

Lless_4_byte_cmp:
    cmp     r2, #0
    beq     Lreturn_0
    sub     r2, r2, #1
    ldrb    r3, [r0], #1
    ldrb    r4, [r1], #1
    sub     r5, r3, r4
    cmp     r5, #0
    itt     ne
    movne   r0, r5
    bne     Lreturn
    b       Lless_4_byte_cmp

Lreturn_0:
    mov r0, #0
Lreturn:
    pop {r4, r5, r6, r7, r8, pc}
Lfunc_end:
    .size memcmp, Lfunc_end - memcmp
    .cantunwind
    .fnend                      @ -- End function

#else
#error 'MVE is not enabled in compile options'
#endif
