## libc-test脚本使用说明

### 一、编译

编译命令，仅编译libc-test测试用例
```
./build.sh --product-name rk3568 --build-target libctest
```
用例可执行文件生成的目录位于  `out/rk3568/musl/libc-test`
以及相关的动态库位于 `out/rk3568/musl/libc-test-lib`

### 二、执行测试

1. 将HDC(Windows版)工具hdc.exe加入到windows环境变量。连接上开发板，保证HDC工具可以连接。

2. 进入musl/scripts，并根据实际环境，修改runtest_Windows.bat中测试用例生成目录完整路径（必要）
   ```bat
   @REM 本地OpenHarmony源码目录,需根据实际环境修改
   set LOCAL=\\wsl$\ubuntu-20.04\home\code\OpenHarmony
   @REM 本地编译out目录
   set OUTDIR=out\rk3568
   ```

3. 将bat脚本复制到windows环境下使用！windows端双击运行runtest_Windows.bat，执行完毕会将详细错误结果信息REPORT返回到当前bat所在目录，并对结果进行统计生成FileList.txt（本次测试执行用例列表）与Summary.txt（统计报告），内容如下：
   $\color{red}{提交PR需在提交里附上对应统计报告！}$
   ```
   SUMMARY
   All: 1177 | Run: 1147 | Skip: 30
   Pass: [1083/1147]
   Fail: [64/1147]
   Check REPORT for more details.

   [Failed List]:
   FAIL __fwritable [signal Segmentation fault]
   FAIL __fwriting [signal Segmentation fault]
   FAIL a_stack_chk_fail [status 255]
   FAIL asctime [status 1]
   ...

   [Skip List]:
   trace_stresstest
   fatal_message
   tgkill_ext
   ...
   ```

   注：此bat文件是从WSL映射目录传输文件到单板目录下，由于WSL无法直接访问windows端口，所以在WSL目录中使用可能无法传输用例。

4. 若要单独执行测试用例，将编译结果目录下的`runtest`与需要执行的用例传输到单板

   单板执行 `./runtest -w '' {filename}`，例如：

   测试成功，无报错信息
   ```
   # ./runtest -w '' memmem
   ```

   测试失败，提示FAIL+用例名+错误码
   ```
   # ./runtest -w '' pthread_atfork-errno-clobber
   ../../third_party/musl/libc-test/src/regression/pthread_atfork-errno-clobber.c:23: (pid = fork()) == -1 failed: fork succeeded despite rlimit
   FAIL pthread_atfork-errno-clobber [status 1]
   ```

### 三、增加用例

举例：在functional目录中新增用例new_test

1. 在libc-test/src/functional目录下新建`new_test.c`

2. 编辑libc-test/src/functional目录下的`test_src_functional.gni`，并在`functional_list`末尾添加`"new_test",`

3. （可选）利用模板添加编译选项，修改`libc-test/test_template.gni`中编译选项内容或针对指定的用例添加，例如：
   ```
   if (target_name == "tls_init") {
     cflags += [ "-Wno-literal-range" ]
     ldflags += [ "-Wl,-rpath=./" ]
     libs += [ "//${root_out_dir}/${test_lib_dir}/libtls_init_dso.so" ]
   }
   ```

3. 执行步骤一（编译）中的编译命令，即可生成对应可执行文件。
