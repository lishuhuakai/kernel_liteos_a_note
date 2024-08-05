SRC_DIR=
DST_DIR=
PORTING=

while getopts "i:o:p:h" arg
do 
    case "${arg}" in
        "i")
            SRC_DIR=${OPTARG}
	    ;;
        "o")
            DST_DIR=${OPTARG}
	    ;;
        "p")
	    PORTING=${OPTARG}
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

if [ ! -d ${DST_DIR} ]; then
    mkdir -p ${DST_DIR}
fi
cp -rfp ${SRC_DIR}/* ${DST_DIR}
cp -rfp ${SRC_DIR}/src/internal/linux/* ${DST_DIR}/src/internal
cp -rfp ${SRC_DIR}/src/hook/linux/* ${DST_DIR}/src/hook
cp -rfp ${SRC_DIR}/crt/linux/* ${DST_DIR}/crt
cp -rfp ${SRC_DIR}/src/linux/arm/linux/* ${DST_DIR}/src/linux/arm
cp -rfp ${SRC_DIR}/src/linux/aarch64/linux/* ${DST_DIR}/src/linux/aarch64
cp -rfp ${SRC_DIR}/src/linux/x86_64/linux/* ${DST_DIR}/src/linux/x86_64
cp -rfp ${SRC_DIR}/src/exit/linux/* ${DST_DIR}/src/exit
cp -rfp ${SRC_DIR}/src/fdsan/linux/* ${DST_DIR}/src/fdsan
cp -rfp ${SRC_DIR}/src/fortify/linux/* ${DST_DIR}/src/fortify
cp -rfp ${SRC_DIR}/src/gwp_asan/linux/* ${DST_DIR}/src/gwp_asan
cp -rfp ${SRC_DIR}/src/hilog/linux/* ${DST_DIR}/src/hilog
cp -rfp ${SRC_DIR}/src/linux/linux/* ${DST_DIR}/src/linux
cp -rfp ${SRC_DIR}/src/network/linux/* ${DST_DIR}/src/network
cp -rfp ${SRC_DIR}/src/syscall_hooks/linux/* ${DST_DIR}/src/syscall_hooks
cp -rfp ${SRC_DIR}/src/signal/linux/* ${DST_DIR}/src/signal
cp -rfp ${SRC_DIR}/src/thread/linux/* ${DST_DIR}/src/thread
cp -rfp ${SRC_DIR}/src/trace/linux/* ${DST_DIR}/src/trace
cp -rfp ${SRC_DIR}/include/trace/linux/* ${DST_DIR}/include/trace
cp -rfp ${SRC_DIR}/src/info/linux/* ${DST_DIR}/src/info
cp -rfp ${SRC_DIR}/ldso/linux/* ${DST_DIR}/ldso
cp -rfp ${SRC_DIR}/include/sys/linux/* ${DST_DIR}/include/sys
cp -rfp ${SRC_DIR}/include/info/linux/* ${DST_DIR}/include/info
cp -rfp ${SRC_DIR}/include/fortify/linux/* ${DST_DIR}/include/fortify
cp -rfp ${SRC_DIR}/include/linux/* ${DST_DIR}/include
cp -rfp ${SRC_DIR}/src/ldso/arm/linux/* ${DST_DIR}/src/ldso/arm
cp -rfp ${SRC_DIR}/src/ldso/aarch64/linux/* ${DST_DIR}/src/ldso/aarch64
cp -rfp ${SRC_DIR}/src/ldso/x86_64/linux/* ${DST_DIR}/src/ldso/x86_64
cp -rfp ${SRC_DIR}/src/misc/aarch64/linux/* ${DST_DIR}/src/misc/aarch64
cp -rfp ${SRC_DIR}/src/malloc/linux/* ${DST_DIR}/src/malloc
cp -rfp ${SRC_DIR}/src/sigchain/linux/* ${DST_DIR}/src/sigchain
cp -rfp ${SRC_DIR}/scripts/${PORTING}/* ${DST_DIR}
