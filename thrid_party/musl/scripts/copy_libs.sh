#!/usr/bin/env sh

while getopts "o:i:" arg
do
    case "${arg}" in
        "i")
            LIBS_DIR=${OPTARG}
            ;;
        "o")
            TARGET_DIR=${OPTARG}
            ;;
        ?)
            echo "unkonw argument"
            exit 1
            ;;
    esac
done

if [ ! -d "${TARGET_DIR}" ];then
    mkdir -p ${TARGET_DIR}
fi

cp -r ${LIBS_DIR}/libzipalign_lspath.zip ${TARGET_DIR}
cp -r ${LIBS_DIR}/libzipalign_rpath.zip ${TARGET_DIR}
cp -r ${LIBS_DIR}/libzipalign.zip ${TARGET_DIR}

