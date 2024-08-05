# Local storage directory of dynamic link library
LOCAL_DYNLIB="your_local_dir"

# Remote transmission target directory
REMOTE_ROOT=/data/tmp/libcgtest
REMOTE=/data/tmp/libcgtest/libs
RPATH_TEST_DIR=${REMOTE}/rpath-test
NS_LIB_ONE_DIR=${REMOTE}/namespace_one_libs
NS_LIB_TWO_DIR=${REMOTE}/namespace_two_libs
NS_LIB_TWO_IMPL_DIR=${REMOTE}/namespace_two_impl_libs

CMD=hdc

${CMD} shell mount -o remount,rw /
${CMD} shell mkdir data/tmp
${CMD} shell rm -rf ${REMOTE_ROOT}
${CMD} shell mkdir ${REMOTE_ROOT}
${CMD} shell mkdir ${REMOTE}
${CMD} shell mkdir ${RPATH_TEST_DIR}
${CMD} shell mkdir ${NS_LIB_ONE_DIR}
${CMD} shell mkdir ${NS_LIB_TWO_DIR}
${CMD} shell mkdir ${NS_LIB_TWO_IMPL_DIR}

for lib in ${LOCAL_DYNLIB}/*; do
    echo $lib
    ${CMD} file send $lib ${REMOTE}/
done
${CMD} shell chmod +x ${REMOTE}

${CMD} shell mv ${REMOTE}/libdlopen_rpath_1.so ${RPATH_TEST_DIR}/
${CMD} shell mv ${REMOTE}/libdlopen_rpath_2.so ${RPATH_TEST_DIR}/
${CMD} shell mv ${REMOTE}/libdlopen_rpath_1_1.so ${RPATH_TEST_DIR}/
${CMD} shell mv ${REMOTE}/libdlopen_rpath_1_2.so ${RPATH_TEST_DIR}/
${CMD} shell mv ${REMOTE}/libdlopen_rpath_2_1.so ${RPATH_TEST_DIR}/

${CMD} shell mv ${REMOTE}/libldso_ns_one.so ${NS_LIB_ONE_DIR}/
${CMD} shell mv ${REMOTE}/libldso_ns_one_impl.so ${NS_LIB_ONE_DIR}/

${CMD} shell mv ${REMOTE}/libldso_ns_root.so ${NS_LIB_TWO_DIR}/
${CMD} shell mv ${REMOTE}/libldso_ns_test_permitted_root.so ${NS_LIB_TWO_DIR}/
${CMD} shell mv ${REMOTE}/libldso_ns_two.so ${NS_LIB_TWO_DIR}/

${CMD} shell mv ${REMOTE}/libldso_ns_two_impl.so ${NS_LIB_TWO_IMPL_DIR}/

