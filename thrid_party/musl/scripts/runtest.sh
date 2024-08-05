
cd /data/tests/libc-test/src
rm /data/tests/libc-test/REPORT
rm /data/tests/libc-test/FileList.txt
rm /data/tests/libc-test/SkipList.txt
touch /data/tests/libc-test/REPORT
touch /data/tests/libc-test/FileList.txt
touch /data/tests/libc-test/SkipList.txt
param set debug.hitrace.tags.enableflags 1

ARCH=arm
ABILIST=$(param get const.product.cpu.abilist)
if [ $ABILIST == "arm64-v8a" ]; then
    ARCH=aarch64
fi

function FileSuffix() {
    local filename="$1"
    if [ -n "$filename" ]; then
        echo "${filename##*.}"
    fi
}

#Test cases that need to be shielded
ShieldedList=("trace_stresstest" "syslog" "vsyslog" "runtest"
#Failed when running by shell, need to be run manually
"tgkill_ext" "stat" "isatty" "ttyname" "a_stack_chk_fail"
"ldso_randomization_manual" "ldso_randomization_test"
"tcgetattr" "tcgetpgrp" "tcgetsid" "tcsendbreak" "tcsetattr" "tcsetpgrp"
#These need run with special condiction
"ipc_msg" "ipc_sem" "ipc_shm" "sem_close-unmap" "sem_open" "pthread_atfork-errno-clobber"
"flockfile-list" "dlns_set_fun_test" "dlns_inherit_test" "dlns_separated_test"
#Some math test cases need to skip.
"acoshl" "asinhl" "erfcl" "fenv" "fma" "fmaf" "fmal" "lgammal" "nearbyint" "nearbyintf"
"nearbyintl" "rint" "rintf" "rintl" "sqrt" "sqrtf" "sqrtl" "tgammal"
#TODO-arm32
"malloc-brk-fail" "pthread_cancel" "res_send"
)

#TODO-aarch64
if [ $ARCH == "aarch64" ]; then
  ShieldedList+=("faccessat" "signal" "unittest_hilog_vsnprint" "yn")
fi

for skiped in ${ShieldedList[*]};do
    echo $skiped >> /data/tests/libc-test/SkipList.txt
done

function ShieldedCases() {
	for filename in ${ShieldedList[*]}
	do
		if [ "$1" = "$filename" ];
		then
			echo "ShieldedCases"
		fi
	done
}

gwpasanTestList=(
    "gwp_asan_buffer_overflow_test"
    "gwp_asan_buffer_underflow_test"
    "gwp_asan_double_free_test"
    "gwp_asan_invalid_free_left_test"
    "gwp_asan_invalid_free_right_test"
    "gwp_asan_use_after_free_test"
    "gwp_asan_unwind_test"
    "gwp_asan_smoke_test"
    "gwp_asan_random_sample_test"
)

function IsGwpasanTest() {
	for filename in ${gwpasanTestList[*]}
	do
		if [ "$1" = "$filename" ];
		then
			echo "yes"
		fi
	done
}

for file in `ls *`
do
	if [ "$(FileSuffix ${file})" = "so" ] \
	|| [ "$(ShieldedCases ${file})" = "ShieldedCases" ] \
	|| [ "$(IsGwpasanTest ${file})" = "yes" ] \
	|| [ -d $file ]
	then
		continue
	elif [ -x $file ] && [ -s $file ]
	then
		echo $file >> /data/tests/libc-test/FileList.txt
		./runtest -w '' -t 30 $file >> /data/tests/libc-test/REPORT
	fi
done

echo "--- gwp_asan test running --- " >> /data/tests/libc-test/REPORT
# gwp_asan test need to be executed at last.
for file in `ls *`
do
	if [ "$(IsGwpasanTest ${file})" = "yes" ]
	then
		echo $file >> /data/tests/libc-test/FileList.txt

		param set gwp_asan.log.path file
		param set gwp_asan.enable.app.${file} true
		if [ "${file}" != "gwp_asan_random_sample_test" ]
		then 
			param set gwp_asan.sample.all true
		fi

		echo "*** ${file} running ***" >> /data/tests/libc-test/REPORT
		./${file} >> /data/tests/libc-test/REPORT
		echo "*** ${file} done *** " >> /data/tests/libc-test/REPORT
		param set gwp_asan.log.path default
		param set gwp_asan.enable.app.${file} false
		if [ "${file}" != "gwp_asan_random_sample_test" ]
		then 
			param set gwp_asan.sample.all false
		fi
	fi
done
echo "--- gwp_asan test done ---" >> /data/tests/libc-test/REPORT