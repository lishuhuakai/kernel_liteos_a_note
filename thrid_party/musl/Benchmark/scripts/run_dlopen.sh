#!/bin/bash
# Copyright (c) 2023 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

HDC='/home/yinchuang/Task/dlopen/toolchains//hdc_std -s 10.121.251.143:8710 '  

function lock_cpu() {
    $HDC shell "mount -o rw,remount /vendor"
    $HDC shell "rm -rf /vendor/etc/soc_perf/*"
    $HDC shell "echo 1700000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
    $HDC shell "echo 2343000 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq"
    $HDC shell "echo 3130000 > /sys/devices/system/cpu/cpu7/cpufreq/scaling_min_freq"
    $HDC shell "echo 4-7 > /dev/cpuset/top-app/cpuset.cpus"
    $HDC shell "echo 4-7 > /dev/cpuset/graphic/cpuset.cpus"
    echo "current freqency: "
    $HDC shell "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
    $HDC shell "cat /sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq"
    $HDC shell "cat /sys/devices/system/cpu/cpu2/cpufreq/scaling_cur_freq"
    $HDC shell "cat /sys/devices/system/cpu/cpu3/cpufreq/scaling_cur_freq"
    $HDC shell "cat /sys/devices/system/cpu/cpu4/cpufreq/scaling_cur_freq"
    $HDC shell "cat /sys/devices/system/cpu/cpu5/cpufreq/scaling_cur_freq"
    $HDC shell "cat /sys/devices/system/cpu/cpu6/cpufreq/scaling_cur_freq"
    $HDC shell "cat /sys/devices/system/cpu/cpu7/cpufreq/scaling_cur_freq"

    $HDC shell power-shell setmode 602
}

function usage() {
    echo "usage: $0 -r {the number of runs} -p {case path in phone}"
    exit 1
}

REPS=50
CASE_PATH="/data/local/tmp/musl_dlopen"

while getopts "hp:r:" OPT
do
    case "$OPT" in
        p) CASE_PATH=$OPTARG;;
        r) REPS=$OPTARG;;
        h) usage;;
        *) 
        echo "unknown options $opt"
        usage
        exit 1
        ;;
    esac
done

echo REPS:$REPS
echo CASE_PATH:$CASE_PATH
time=$(date "+%Y%m%d-%H%M%S")
result_file="dlopen_cost_${time}.txt"

lock_cpu

for((i=1;i<=${REPS};i++));
do
    $HDC shell $CASE_PATH | grep "dlopen" >> ${result_file}
done

echo "save results to dlopen_cost_${time}.txt"

err=$(cat ${result_file} | grep "dlopen error" | wc -l )
if [ $err -gt 0 ];
then
    echo "dlopen failed!!!"
    exit 1
fi

cat ${result_file} | awk 'BEGIN{ave=0; num=0}  {ave+=$4;num++}  END{print "dlopen cost(ave): " ave/num "(ms)", "num:" num}'
