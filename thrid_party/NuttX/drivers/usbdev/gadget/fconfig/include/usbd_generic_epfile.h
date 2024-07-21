/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS USB Driver Generic EP0 Data Stream HeadFile
 * Author: Yannik Li
 * Create: 2021-02-21
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
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
 * --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#ifndef LITEOS_USB_DEVICE_GENERIC_EPFILE_H
#define LITEOS_USB_DEVICE_GENERIC_EPFILE_H

#include "f_common.h"

#ifdef __cplusplus
#if __cplusplus
//extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

struct generic_io_data {
    uint32_t      buf;
    int32_t       result;
    uint32_t      xfrd;
    uint32_t      len;
};
struct generic_io_data_ret {
    uint32_t buf;
    uint32_t actual;
    int      status;
};

struct generic_request {
    struct list_head              entry;
    struct generic_io_data        io_data;
    void                          *user_buf;
};

struct generic_epfile {
    struct usb_obj                obj;
    pthread_mutex_t               mutex;

    struct generic_dev_s          *dev;
    struct generic_ep             *ep;
    spinlock_t                    list_lock;
    struct usb_obj                memory_obj;
    struct list_head              req_list;
    struct list_head              comp_list;
    struct generic_request        *cur_req;
    volatile atomic_t             busy_flag;
    uint32_t                      req_count;
    EVENT_CB_S                    sync_event;
    uint32_t                      event_mask;
    char                          name[5];
    unsigned char                 in;
    unsigned char                 isoc;
    unsigned char                 _pad;
    bool                          is_created;
};

extern int generic_epfiles_create(struct generic_dev_s *dev);
extern int generic_eps_enable(struct generic_dev_s *dev);
extern void generic_eps_disable(struct generic_dev_s *dev);
extern int generic_remove_epfiles(struct generic_dev_s *dev);

#ifdef __cplusplus
#if __cplusplus
//}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* LITEOS_USB_DEVICE_GNENRIC_EPFILE_H */
