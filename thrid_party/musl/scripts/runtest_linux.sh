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

LOCAL="unknown_dir"
OUTDIR=out/rk3568
CMD=hdc_std
ARCH=arm

function usage {
    echo "usage: $0 -r {source root} -p {product name in out(eg rk3568)}"
    exit 1
}

while getopts ":r:p:" opt
do
    case $opt in
        r)
            LOCAL=$OPTARG
            ;;
        p)
            OUTDIR=out/$OPTARG
            ;;
        ?)
            echo "unknown options $opt"
            usage
            exit 1
            ;;
    esac
done

TESTDIR=${LOCAL}/${OUTDIR}/musl/libc-test
DYNLIB=${LOCAL}/${OUTDIR}/musl/libc-test-lib
SHDIR=${LOCAL}/third_party/musl/scripts
REMOTE=/data/tests/libc-test
REMOTESYSTEM=/system/lib

echo checking HDC device....
for device in $($CMD list targets); do 
    device=$(echo $device | tr -d '\n\r')
    if [ $device == "[Empty]" ]; then
        echo Device not found, please check your device.
        exit 1
    fi
done

ABILIST=$(${CMD} shell param get const.product.cpu.abilist)
echo $ABILIST

if [ $ABILIST == "arm64-v8a" ]; then
    ARCH=aarch64
fi
echo detect arch = ${ARCH}


echo checking TESTDIR
if [ ! -d ${TESTDIR} ]; then
    echo Error: path "${TESTDIR}" was not found.
    exit 1
fi

# Xts test require all so should exist in ${DYNLIB}
TOTAL_SO_IN_TESTDIR=$(ls ${TESTDIR} | grep -E "*.so$" | wc -l)
SO_NAMES=$(ls ${TESTDIR} | grep -E "*.so$")
if [ $TOTAL_SO_IN_TESTDIR -gt 0  ]; then
    echo Error: so shoul not exist in ${TESTDIR}, you should modify your part_name in gn to set so in ${DYNLIB}.
    echo "Total so:${TOTAL_SO_IN_TESTDIR}"
    echo -e "So list:\n${SO_NAMES}"
    exit 1
fi

echo now mkdir
${CMD} shell rm -rf ${REMOTE}
${CMD} shell mkdir /data/tests
${CMD} shell mkdir ${REMOTE}

#mk temp directory
${CMD} shell mkdir /tmp
${CMD} shell mkdir /dev/shm
echo Mkdir done.


${CMD} file send ${TESTDIR} ${REMOTE}/src
${CMD} shell mv ${REMOTE}/src/libc-test/* ${REMOTE}/src
for lib in ${DYNLIB}/*; do
    echo $lib
    ${CMD} file send $lib ${REMOTE}/src
done

${CMD} shell chmod +x ${REMOTE}/src/*
${CMD} shell mount -o rw,remount /
${CMD} shell chmod 777 /etc
${CMD} shell cp /etc/ld-musl-namespace-${ARCH}.ini /etc/ld-musl-namespace-${ARCH}.ini.bak
${CMD} file send ${LOCAL}/third_party/musl/config/ld-musl-namespace-${ARCH}-test.ini /etc/ld-musl-namespace-${ARCH}.ini
${CMD} shell mkdir ${REMOTE}/src/A
${CMD} shell mkdir ${REMOTE}/src/B
${CMD} shell mkdir ${REMOTE}/src/C
${CMD} shell mkdir ${REMOTE}/src/D
${CMD} shell mkdir -p ${REMOTE}/src/rpath_lib/rpath_support_A
${CMD} shell mkdir -p ${REMOTE}/src/rpath_lib/rpath_support_B
${CMD} shell mkdir -p ${REMOTE}/src/rpath_support_C
${CMD} shell mv ${REMOTE}/src/libdlns_dlsym_dep_a.so ${REMOTE}/src/A
${CMD} shell mv ${REMOTE}/src/libdlns_dlsym_dep_b.so ${REMOTE}/src/B
${CMD} shell mv ${REMOTE}/src/libdlns_dlsym_dep_c.so ${REMOTE}/src/C
${CMD} shell mv ${REMOTE}/src/libprimary_absolute.so ${REMOTE}/src/rpath_lib
${CMD} shell mv ${REMOTE}/src/libprimary_origin.so ${REMOTE}/src/rpath_lib
${CMD} shell mv ${REMOTE}/src/libprimary_multiple.so ${REMOTE}/src/rpath_lib
${CMD} shell mv ${REMOTE}/src/libsupport_002.so ${REMOTE}/src/rpath_support_C
${CMD} shell mv ${REMOTE}/src/libsupport_001.so ${REMOTE}/src/rpath_lib/rpath_support_A
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/B/libB.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/C/libC.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/D/libD.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/sharedlibtest.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/set_lib_path_0200.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/inherit_0300.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/inherit_0700.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/inherit_0800.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/inherit_1100.so
${CMD} shell cp ${REMOTE}/src/libdlopen_ns_dso.so /system/lib
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/lib_for_no_delete.so
${CMD} shell cp ${REMOTE}/src/libdlopen_dso.so ${REMOTE}/src/lib_for_dlopen.so
${CMD} shell cp ${REMOTE}/src/libdlopen_ns_dso.so /system/lib64/libdlopen_ns_dso_sys.so
${CMD} shell cp ${REMOTE}/src/libdlopen_ns_dso.so /system/lib64/ndk/libdlopen_ns_dso_ndk.so
${CMD} shell cp ${REMOTE}/src/libdlopen_ns_dso.so /system/lib/libdlopen_ns_dso_sys.so
${CMD} shell cp ${REMOTE}/src/libdlopen_ns_dso.so /system/lib/ndk/libdlopen_ns_dso_ndk.so
${CMD} shell cp ${REMOTE}/src/libdlclose_recursive.so /system/lib64/libdlclose_recursive.so
${CMD} shell cp ${REMOTE}/src/libdlclose_recursive_dlopen_so.so /system/lib64/libdlclose_recursive_dlopen_so.so
${CMD} shell cp ${REMOTE}/src/libdl_gnu_hash_so.so ${REMOTE}/src/libdl_gnu_hash_so.so
${CMD} shell cp ${REMOTE}/src/libdl_sysv_hash_so.so ${REMOTE}/src/libdl_sysv_hash_so.so

${CMD} shell mv ${REMOTE}/src/zh_CN /tmp/zh_CN

${CMD} file send ${SHDIR}/runtest.sh ${REMOTE}/runtest.sh
${CMD} shell chmod +x ${REMOTE}/runtest.sh
${CMD} shell ${REMOTE}/runtest.sh


echo hdc file recv ${REMOTE}/REPORT ${SHDIR}/REPORT
${CMD} file recv ${REMOTE}/REPORT ${SHDIR}/REPORT

if [ -f FileList.txt ]; then
	rm FileList.txt
fi
if [ -f Summary.txt ]; then
	rm Summary.txt
fi
${CMD} file recv ${REMOTE}/FileList.txt ${SHDIR}/FileList.txt
${CMD} file recv ${REMOTE}/SkipList.txt ${SHDIR}/SkipList.txt
#
#for /f "delims=:" %%a in ('dir /b "${TESTDIR}" ^| findstr /n .*') do set all=%%a
all=$(ls -l ${TESTDIR} | grep "^-" | wc -l)
#for /f %%b in (' find /c /v "" ^<"FileList.txt" ') do set /a run=%%b
run=$(cat -t ${SHDIR}/FileList.txt | grep -v ^\s*$ | wc -l)
#for /f %%c in (' find /c "FAIL" ^<"REPORT" ') do set fail=%%c
fail=$(cat -t ${SHDIR}/REPORT | grep 'FAIL ' | wc -l)
#
#
echo ===================================
pass=$[$run - $fail]
skip=$[$all - $run]
#
echo "SUMMARY
echo All: ${all} | Run: ${run} | Skip: ${skip}
echo Pass: [${pass}/${run}]
echo Fail: [${fail}/${run}]
" | tee Summary.txt

#
echo "[Failed List]: 
$(cat -t ${SHDIR}/REPORT | grep 'FAIL ')
" | tee -a Summary.txt
echo "[Skip List]:  " | tee -a Summary.txt

cat ${SHDIR}/SkipList.txt | tee -a Summary.txt
