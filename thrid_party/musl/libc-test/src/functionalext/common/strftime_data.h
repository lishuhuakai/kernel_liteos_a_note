/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STRFTIME_DATA_H__
#define __STRFTIME_DATA_H__

#include "time_test_data.h"

static const struct time_test_string_struct test_strftime_data[] = {
    {"Europe/Madrid", "Sat Jul 30 12:40:14 2022 CEST+0200"},
    {"Europe/Astrakhan", "Sat Jul 30 14:40:14 2022 +04+0400"},
    {"Europe/Kaliningrad", "Sat Jul 30 12:40:14 2022 EET+0200"},
    {"Europe/Moscow", "Sat Jul 30 13:40:14 2022 MSK+0300"},
    {"Europe/Samara", "Sat Jul 30 14:40:14 2022 +04+0400"},
    {"Europe/Ulyanovsk", "Sat Jul 30 14:40:14 2022 +04+0400"},
    {"Europe/Berlin", "Sat Jul 30 12:40:14 2022 CEST+0200"},
    {"Europe/Bucharest", "Sat Jul 30 13:40:14 2022 EEST+0300"},
    {"Europe/Paris", "Sat Jul 30 12:40:14 2022 CEST+0200"},
    {"Europe/Sofia", "Sat Jul 30 13:40:14 2022 EEST+0300"},
    {"Europe/Warsaw", "Sat Jul 30 12:40:14 2022 CEST+0200"},
    {"Europe/London", "Sat Jul 30 11:40:14 2022 BST+0100"},
    {"Europe/Belgrade", "Sat Jul 30 12:40:14 2022 CEST+0200"},
    {"Europe/Volgograd", "Sat Jul 30 13:40:14 2022 MSK+0300"},
    {"Europe/Rome", "Sat Jul 30 12:40:14 2022 CEST+0200"},
    {"Australia/Adelaide", "Sat Jul 30 20:10:14 2022 ACST+0930"},
    {"Australia/Darwin", "Sat Jul 30 20:10:14 2022 ACST+0930"},
    {"Australia/Melbourne", "Sat Jul 30 20:40:14 2022 AEST+1000"},
    {"Australia/Perth", "Sat Jul 30 18:40:14 2022 AWST+0800"},
    {"Australia/Eucla", "Sat Jul 30 19:25:14 2022 +0845+0845"},
    {"Australia/Sydney", "Sat Jul 30 20:40:14 2022 AEST+1000"},
    {"Australia/Brisbane", "Sat Jul 30 20:40:14 2022 AEST+1000"},
    {"Australia/Hobart", "Sat Jul 30 20:40:14 2022 AEST+1000"},
    {"Australia/Lord_Howe", "Sat Jul 30 21:10:14 2022 +1030+1030"},
    {"America/Edmonton", "Sat Jul 30 04:40:14 2022 MDT-0600"},
    {"America/Regina", "Sat Jul 30 04:40:14 2022 CST-0600"},
    {"America/Chicago", "Sat Jul 30 05:40:14 2022 CDT-0500"},
    {"America/Mexico_City", "Sat Jul 30 05:40:14 2022 CDT-0500"},
    {"America/Toronto", "Sat Jul 30 06:40:14 2022 EDT-0400"},
    {"America/Vancouver", "Sat Jul 30 03:40:14 2022 PDT-0700"},
    {"America/Anchorage", "Sat Jul 30 02:40:14 2022 AKDT-0800"},
    {"America/Denver", "Sat Jul 30 04:40:14 2022 MDT-0600"},
    {"America/New_York", "Sat Jul 30 06:40:14 2022 EDT-0400"},
    {"America/Los_Angeles", "Sat Jul 30 03:40:14 2022 PDT-0700"},
    {"America/St_Johns", "Sat Jul 30 08:10:14 2022 NDT-0230"},
    {"America/Halifax", "Sat Jul 30 07:40:14 2022 ADT-0300"},
    {"America/Cancun", "Sat Jul 30 05:40:14 2022 EST-0500"},
    {"HST", "Sat Jul 30 00:40:14 2022 HST-1000"},
    {"Asia/Irkutsk", "Sat Jul 30 18:40:14 2022 +08+0800"},
    {"Asia/Phnom_Penh", "Sat Jul 30 17:40:14 2022 +07+0700"},
    {"Asia/Hong_Kong", "Sat Jul 30 18:40:14 2022 HKT+0800"},
    {"Asia/Sakhalin", "Sat Jul 30 21:40:14 2022 +11+1100"},
    {"Asia/Anadyr", "Sat Jul 30 22:40:14 2022 +12+1200"},
    {"Asia/Yekaterinburg", "Sat Jul 30 15:40:14 2022 +05+0500"},
    {"Asia/Urumqi", "Sat Jul 30 16:40:14 2022 +06+0600"},
    {"Asia/Shanghai", "Sat Jul 30 18:40:14 2022 CST+0800"},
    {"Asia/Ho_Chi_Minh", "Sat Jul 30 17:40:14 2022 +07+0700"},
    {"Asia/Macau", "Sat Jul 30 18:40:14 2022 CST+0800"},
    {"Asia/Krasnoyarsk", "Sat Jul 30 17:40:14 2022 +07+0700"},
    {"Asia/Omsk", "Sat Jul 30 16:40:14 2022 +06+0600"},
    {"Asia/Chita", "Sat Jul 30 19:40:14 2022 +09+0900"},
    {"Asia/Singapore", "Sat Jul 30 18:40:14 2022 +08+0800"},
    {"Asia/Vladivostok", "Sat Jul 30 20:40:14 2022 +10+1000"},
    {"Asia/Kuching", "Sat Jul 30 18:40:14 2022 +08+0800"},
    {"Asia/Kolkata", "Sat Jul 30 16:10:14 2022 IST+0530"},
    {"Asia/Tokyo", "Sat Jul 30 19:40:14 2022 JST+0900"},
    {"Asia/Kamchatka", "Sat Jul 30 22:40:14 2022 +12+1200"},
    {"Asia/Vientiane", "Sat Jul 30 17:40:14 2022 +07+0700"},
    {"Asia/Taipei", "Sat Jul 30 18:40:14 2022 CST+0800"},
    {"Asia/Novosibirsk", "Sat Jul 30 17:40:14 2022 +07+0700"},
    {"Asia/Riyadh", "Sat Jul 30 13:40:14 2022 +03+0300"},
    {"Asia/Bangkok", "Sat Jul 30 17:40:14 2022 +07+0700"},
    {"Asia/Magadan", "Sat Jul 30 21:40:14 2022 +11+1100"},
    {"Asia/Novokuznetsk", "Sat Jul 30 17:40:14 2022 +07+0700"},
    {"Asia/Khandyga", "Sat Jul 30 19:40:14 2022 +09+0900"},
    {"Asia/Kuala_Lumpur", "Sat Jul 30 18:40:14 2022 +08+0800"},
    {"Asia/Jakarta", "Sat Jul 30 17:40:14 2022 WIB+0700"},
    {"Asia/Manila", "Sat Jul 30 18:40:14 2022 PST+0800"},
    {"Asia/Yangon", "Sat Jul 30 17:10:14 2022 +0630+0630"},
    {"Asia/Seoul", "Sat Jul 30 19:40:14 2022 KST+0900"},
    {"Africa/Johannesburg", "Sat Jul 30 12:40:14 2022 SAST+0200"},
    {"PST8PDT", "Sat Jul 30 03:40:14 2022 PDT-0700"},
};

#endif