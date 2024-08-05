#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2020-2030. All rights reserved.
set -e

while getopts "o:i:t:h" arg
do
    case "${arg}" in
        "o")
            OUT_DIR=${OPTARG}
            ;;
        "i")
            SOURCE_DIR=${OPTARG}
            ;;
        "h")
            echo "help"
            ;;
        ?)
            echo "unkonw argument"
            exit 1
            ;;
    esac
done

if [ ! -d "${OUT_DIR}" ];then
    mkdir -p ${OUT_DIR}
fi

rm -rf ${OUT_DIR}/zh_CN
mkdir ${OUT_DIR}/zh_CN
mkdir ${OUT_DIR}/zh_CN/LC_MESSAGES
cp ${SOURCE_DIR}/src/functionalext/locale/test_dcngettext.mo ${OUT_DIR}/zh_CN/LC_MESSAGES/
