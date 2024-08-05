@REM Local storage directory of dynamic link library
set LOCAL_DYNLIB="your_local_dir"

@REM Remote transmission target directory
set REMOTE_ROOT=/data/tmp/libcgtest
set REMOTE=/data/tmp/libcgtest/libs
set RPATH_TEST_DIR=%REMOTE%/rpath-test
set NS_LIB_ONE_DIR=%REMOTE%/namespace_one_libs
set NS_LIB_TWO_DIR=%REMOTE%/namespace_two_libs
set NS_LIB_TWO_IMPL_DIR=%REMOTE%/namespace_two_impl_libs

hdc_std shell mount -o remount,rw /
hdc_std shell mkdir "data/tmp"
hdc_std shell rm -rf %REMOTE_ROOT%
hdc_std shell mkdir %REMOTE_ROOT%
hdc_std shell mkdir %REMOTE%
hdc_std shell mkdir %RPATH_TEST_DIR%
hdc_std shell mkdir %NS_LIB_ONE_DIR%
hdc_std shell mkdir %NS_LIB_TWO_DIR%
hdc_std shell mkdir %NS_LIB_TWO_IMPL_DIR%

for %%i in (%LOCAL_DYNLIB%\*) do (
    hdc_std file send %%i %REMOTE%
)
hdc_std shell chmod +x %REMOTE%


hdc_std shell mv %REMOTE%/libdlopen_rpath_1.so %RPATH_TEST_DIR%/
hdc_std shell mv %REMOTE%/libdlopen_rpath_2.so %RPATH_TEST_DIR%/
hdc_std shell mv %REMOTE%/libdlopen_rpath_1_1.so %RPATH_TEST_DIR%/
hdc_std shell mv %REMOTE%/libdlopen_rpath_1_2.so %RPATH_TEST_DIR%/
hdc_std shell mv %REMOTE%/libdlopen_rpath_2_1.so %RPATH_TEST_DIR%/

hdc_std shell mv %REMOTE%/libldso_ns_one.so %NS_LIB_ONE_DIR%/
hdc_std shell mv %REMOTE%/libldso_ns_one_impl.so %NS_LIB_ONE_DIR%/

hdc_std shell mv %REMOTE%/libldso_ns_root.so %NS_LIB_TWO_DIR%/
hdc_std shell mv %REMOTE%/libldso_ns_test_permitted_root.so %NS_LIB_TWO_DIR%/
hdc_std shell mv %REMOTE%/libldso_ns_two.so %NS_LIB_TWO_DIR%/

hdc_std shell mv %REMOTE%/libldso_ns_two_impl.so %NS_LIB_TWO_IMPL_DIR%/

pause

