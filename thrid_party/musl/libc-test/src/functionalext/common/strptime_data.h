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

#ifndef __STRPTIME_DATA_H__
#define __STRPTIME_DATA_H__

#include "time_test_data.h"

static const struct time_test_string_struct test_strptime_data[] = {
    {"Europe/Madrid", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Astrakhan", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Europe/Kaliningrad", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=EET"},
    {"Europe/Moscow", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=MSK"},
    {"Europe/Samara", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Europe/Ulyanovsk", "2022-6-30 14:40:14 wday=6,yday=0,isdst=0,gmtoff=14400,zone=+04"},
    {"Europe/Berlin", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Bucharest", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Paris", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Sofia", "2022-6-30 13:40:14 wday=6,yday=0,isdst=1,gmtoff=10800,zone=EEST"},
    {"Europe/Warsaw", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/London", "2022-6-30 11:40:14 wday=6,yday=0,isdst=1,gmtoff=3600,zone=BST"},
    {"Europe/Belgrade", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Europe/Volgograd", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=MSK"},
    {"Europe/Rome", "2022-6-30 12:40:14 wday=6,yday=0,isdst=1,gmtoff=7200,zone=CEST"},
    {"Australia/Adelaide", "2022-6-30 20:10:14 wday=6,yday=0,isdst=0,gmtoff=34200,zone=ACST"},
    {"Australia/Darwin", "2022-6-30 20:10:14 wday=6,yday=0,isdst=0,gmtoff=34200,zone=ACST"},
    {"Australia/Melbourne", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Perth", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=AWST"},
    {"Australia/Eucla", "2022-6-30 19:25:14 wday=6,yday=0,isdst=0,gmtoff=31500,zone=+0845"},
    {"Australia/Sydney", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Brisbane", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Hobart", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=AEST"},
    {"Australia/Lord_Howe", "2022-6-30 21:10:14 wday=6,yday=0,isdst=0,gmtoff=37800,zone=+1030"},
    {"America/Edmonton", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/Regina", "2022-6-30 4:40:14 wday=6,yday=0,isdst=0,gmtoff=-21600,zone=CST"},
    {"America/Chicago", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Mexico_City", "2022-6-30 5:40:14 wday=6,yday=0,isdst=1,gmtoff=-18000,zone=CDT"},
    {"America/Toronto", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Vancouver", "2022-6-30 3:40:14 wday=6,yday=0,isdst=1,gmtoff=-25200,zone=PDT"},
    {"America/Anchorage", "2022-6-30 2:40:14 wday=6,yday=0,isdst=1,gmtoff=-28800,zone=AKDT"},
    {"America/Denver", "2022-6-30 4:40:14 wday=6,yday=0,isdst=1,gmtoff=-21600,zone=MDT"},
    {"America/New_York", "2022-6-30 6:40:14 wday=6,yday=0,isdst=1,gmtoff=-14400,zone=EDT"},
    {"America/Los_Angeles", "2022-6-30 3:40:14 wday=6,yday=0,isdst=1,gmtoff=-25200,zone=PDT"},
    {"America/St_Johns", "2022-6-30 8:10:14 wday=6,yday=0,isdst=1,gmtoff=-9000,zone=NDT"},
    {"America/Halifax", "2022-6-30 7:40:14 wday=6,yday=0,isdst=1,gmtoff=-10800,zone=ADT"},
    {"America/Cancun", "2022-6-30 5:40:14 wday=6,yday=0,isdst=0,gmtoff=-18000,zone=EST"},
    {"HST", "2022-6-30 0:40:14 wday=6,yday=0,isdst=0,gmtoff=-36000,zone=HST"},
    {"Asia/Irkutsk", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Phnom_Penh", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Hong_Kong", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=HKT"},
    {"Asia/Sakhalin", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Asia/Anadyr", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Asia/Yekaterinburg", "2022-6-30 15:40:14 wday=6,yday=0,isdst=0,gmtoff=18000,zone=+05"},
    {"Asia/Urumqi", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Shanghai", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=CST"},
    {"Asia/Ho_Chi_Minh", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Macau", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=CST"},
    {"Asia/Krasnoyarsk", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Omsk", "2022-6-30 16:40:14 wday=6,yday=0,isdst=0,gmtoff=21600,zone=+06"},
    {"Asia/Chita", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=+09"},
    {"Asia/Singapore", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Vladivostok", "2022-6-30 20:40:14 wday=6,yday=0,isdst=0,gmtoff=36000,zone=+10"},
    {"Asia/Kuching", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Kolkata", "2022-6-30 16:10:14 wday=6,yday=0,isdst=0,gmtoff=19800,zone=IST"},
    {"Asia/Tokyo", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=JST"},
    {"Asia/Kamchatka", "2022-6-30 22:40:14 wday=6,yday=0,isdst=0,gmtoff=43200,zone=+12"},
    {"Asia/Vientiane", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Taipei", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=CST"},
    {"Asia/Novosibirsk", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Riyadh", "2022-6-30 13:40:14 wday=6,yday=0,isdst=0,gmtoff=10800,zone=+03"},
    {"Asia/Bangkok", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Magadan", "2022-6-30 21:40:14 wday=6,yday=0,isdst=0,gmtoff=39600,zone=+11"},
    {"Asia/Novokuznetsk", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=+07"},
    {"Asia/Khandyga", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=+09"},
    {"Asia/Kuala_Lumpur", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=+08"},
    {"Asia/Jakarta", "2022-6-30 17:40:14 wday=6,yday=0,isdst=0,gmtoff=25200,zone=WIB"},
    {"Asia/Manila", "2022-6-30 18:40:14 wday=6,yday=0,isdst=0,gmtoff=28800,zone=PST"},
    {"Asia/Yangon", "2022-6-30 17:10:14 wday=6,yday=0,isdst=0,gmtoff=23400,zone=+0630"},
    {"Asia/Seoul", "2022-6-30 19:40:14 wday=6,yday=0,isdst=0,gmtoff=32400,zone=KST"},
    {"Africa/Johannesburg", "2022-6-30 12:40:14 wday=6,yday=0,isdst=0,gmtoff=7200,zone=SAST"},
    {"PST8PDT", "2022-6-30 3:40:14 wday=6,yday=0,isdst=1,gmtoff=-25200,zone=PDT"},
};


#endif