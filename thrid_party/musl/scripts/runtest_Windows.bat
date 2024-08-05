@echo off

@REM 本地OpenHarmony源码目录,需根据实际环境修改
set LOCAL=\\wsl$\ubuntu-20.04\home\code\OHOS
@REM 本地编译环境的out目录
set OUTDIR=out\rk3568

@REM 本地用例生成目录
set TESTDIR=%LOCAL%\%OUTDIR%\musl\libc-test
@REM 动态链接库生成目录
set DYNLIB=%LOCAL%\%OUTDIR%\musl\libc-test-lib
@REM 远程传输目标目录
set REMOTE=/data/tests/libc-test
set REMOTESYSTEM=/system/lib
@REM runtest脚本所在目录
set SHDIR=%LOCAL%\third_party\musl\scripts
set ARCH=arm

@REM 检查设备是否连接
echo checking HDC device 
for /F "usebackq delims==" %%c in (`hdc list targets`) DO (
    echo Device list:
    echo %%c | findstr "[Empty]" && goto noDevice || echo %%c && goto CheckDirectory
)

:CheckDirectory
echo checking TESTDIR
if exist %TESTDIR% (
	echo TESTDIR: %TESTDIR%
) else (
	echo Error: path "%TESTDIR%" was not found.
	goto end
)

@REM Xts test require all so should exist in DYNLIB
setlocal enabledelayedexpansion
for %%j in (%TESTDIR%\*) do (
    set tmp_file=%%j
    set tmp_file_suffix=!tmp_file:~-3!
    if "!tmp_file_suffix!" == ".so" (
        set /a TOTAL_SO_IN_TESTDIR+=1
        call set SO_NAMES=%%SO_NAMES%%;%%j
    )
)

if !TOTAL_SO_IN_TESTDIR! gtr 0 (
    echo Error: so shoul not exist in %TESTDIR%, you should modify your part_name in gn to set so in %DYNLIB%.
    echo Total so:%TOTAL_SO_IN_TESTDIR%
    echo So list:&echo\    %SO_NAMES:~1,10000%
    goto end
)

@REM 在单板创建目录, 需要预先创建好才能传输到相应位置。
:hdcStart
for /F "usebackq delims==" %%r in (`hdc shell param get const.product.cpu.abilist`) DO (
    echo %%r | findstr "arm64-v8a" && set ARCH=aarch64 
)
echo detect arch = %ARCH%

echo.
echo now mkdir...
hdc shell rm -rf /data/tests/libc-test
hdc shell mkdir /data/tests
hdc shell mkdir %REMOTE%

@REM 创建临时文件夹,用于存放用例生成的临时文件
hdc shell mkdir /tmp
hdc shell mkdir /dev/shm
echo Mkdir done.
goto hdcSend

@REM 传输文件,单板上执行runtest.sh，将结果REPORT返回到.bat所在目录
:hdcSend
hdc file send %TESTDIR% %REMOTE%/src
for %%j in (%DYNLIB%\*) do (
	echo %%j
	hdc file send %%j %REMOTE%/src
)
hdc shell chmod +x %REMOTE%/src/*

hdc shell mount -o rw,remount /
hdc shell chmod 777 /etc
hdc shell cp /etc/ld-musl-namespace-%ARCH%.ini /etc/ld-musl-namespace-%ARCH%.ini.bak
hdc file send %LOCAL%\third_party\musl\config\ld-musl-namespace-%ARCH%-test.ini ^
                /etc/ld-musl-namespace-%ARCH%.ini
hdc shell mkdir %REMOTE%/src/A
hdc shell mkdir %REMOTE%/src/B
hdc shell mkdir %REMOTE%/src/C
hdc shell mkdir %REMOTE%/src/D
hdc shell mkdir -p %REMOTE%/src/rpath_lib/rpath_support_A
hdc shell mkdir -p %REMOTE%/src/rpath_lib/rpath_support_B
hdc shell mkdir -p %REMOTE%/src/rpath_support_C
hdc shell mv %REMOTE%/src/libdlns_dlsym_dep_a.so %REMOTE%/src/A
hdc shell mv %REMOTE%/src/libdlns_dlsym_dep_b.so %REMOTE%/src/B
hdc shell mv %REMOTE%/src/libdlns_dlsym_dep_c.so %REMOTE%/src/C
hdc shell mv %REMOTE%/src/libprimary_absolute.so %REMOTE%/src/rpath_lib
hdc shell mv %REMOTE%/src/libprimary_origin.so %REMOTE%/src/rpath_lib
hdc shell mv %REMOTE%/src/libprimary_multiple.so %REMOTE%/src/rpath_lib
hdc shell mv %REMOTE%/src/libsupport_002.so %REMOTE%/src/rpath_support_C
hdc shell mv %REMOTE%/src/libsupport_001.so %REMOTE%/src/rpath_lib/rpath_support_A
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/B/libB.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/C/libC.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/D/libD.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/sharedlibtest.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/set_lib_path_0200.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/inherit_0300.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/inherit_0700.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/inherit_0800.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/inherit_1100.so
hdc shell cp %REMOTE%/src/libdlopen_ns_dso.so /system/lib
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/lib_for_no_delete.so
hdc shell cp %REMOTE%/src/libdlopen_dso.so %REMOTE%/src/lib_for_dlopen.so
hdc shell cp %REMOTE%/src/libdlopen_ns_dso.so /system/lib64/libdlopen_ns_dso_sys.so
hdc shell cp %REMOTE%/src/libdlopen_ns_dso.so /system/lib/libdlopen_ns_dso_sys.so
hdc shell cp %REMOTE%/src/libdlopen_ns_dso.so /system/lib64/ndk/libdlopen_ns_dso_ndk.so
hdc shell cp %REMOTE%/src/libdlopen_ns_dso.so /system/lib/ndk/libdlopen_ns_dso_ndk.so
hdc shell cp %REMOTE%/src/libdlclose_recursive.so /system/lib64/libdlclose_recursive.so
hdc shell cp %REMOTE%/src/libdlclose_recursive_dlopen_so.so /system/lib64/libdlclose_recursive_dlopen_so.so
hdc shell cp %REMOTE%/src/libdl_gnu_hash_so.so %REMOTE%/src/libdl_gnu_hash_so.so
hdc shell cp %REMOTE%/src/libdl_sysv_hash_so.so %REMOTE%/src/libdl_sysv_hash_so.so

hdc shell mv %REMOTE%/src/zh_CN /tmp/zh_CN

hdc file send %SHDIR%\runtest.sh %REMOTE%/runtest.sh
hdc shell chmod +x %REMOTE%/runtest.sh
hdc shell %REMOTE%/runtest.sh


:Summary
echo hdc file recv %REMOTE%/REPORT %~dp0REPORT
hdc file recv %REMOTE%/REPORT %~dp0REPORT

if exist FileList.txt (
	del FileList.txt
)
if exist Summary.txt (
	del Summary.txt
)
hdc file recv %REMOTE%/FileList.txt %~dp0FileList.txt
hdc file recv %REMOTE%/SkipList.txt %~dp0SkipList.txt

for /f "delims=:" %%a in ('dir /b /a:-d "%TESTDIR%" ^| findstr /n .*') do set all=%%a
for /f %%b in (' find /c /v "" ^<"FileList.txt" ') do set /a run=%%b
for /f %%c in (' find /c "FAIL" ^<"REPORT" ') do set fail=%%c


echo ===================================
set /a pass=%run%-%fail%
set /a skip=%all%-%run%

echo SUMMARY-%ARCH%
echo All: %all% ^| Run: %run% ^| Skip: %skip%
echo Pass: [%pass%/%run%]
echo Fail: [%fail%/%run%]

(
echo SUMMARY-%ARCH%
echo All: %all% ^| Run: %run% ^| Skip: %skip%
echo Pass: [%pass%/%run%]
echo Fail: [%fail%/%run%]
echo Check REPORT for more details.
echo.
)>> Summary.txt

echo [Failed List]: >> Summary.txt
setlocal EnableDelayedExpansion
for /f "delims=" %%i in ('findstr /c:"FAIL" REPORT') do (
    set "F=%%i"
    set "F=!F:*FAIL =FAIL !"
    echo !F! >> Summary.txt
)
echo. >> Summary.txt

echo [Skip List]: >> Summary.txt
copy /b Summary.txt+SkipList.txt
del SkipList.txt
goto end

:noDevice
echo Device not found, please check your device.

:end
echo.
pause
exit
