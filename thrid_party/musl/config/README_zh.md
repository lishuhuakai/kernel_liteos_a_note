## 简介

本目录放置了部分预置的musl linker配置文件。
关于musl linker配置文件可以参考其[README](../ldso/README.md)说明。


## 说明

1.  musl linker加载的配置文件为：

    ```
    /etc/ld-musl-namespace-$ARCH.ini
    ```

    其中`$ARCH`为系统的CPU架构，比如`arm`或`aarch64`等。

2.  配置动态库查找路径：

    配置文件中`namespace.default.lib.paths`项用于指定musl linker加载动态库时的查找路径，多个路径中间使用`:`号分隔。

    配置文件中`namespace.default.asan.lib.paths`项用于指定musl asan linker加载动态库时的查找路径，多个路径中间使用`:`号分隔，判断是否为asan linker是看linker的名字中是否含有`-asan`，比如`ld-musl-aarch64.so.1`是正常版本的linker，`ld-musl-aarch64-asan.so.1`是asan版本的linker。

3.  关于asan双库版本中的动态库查找路径配置：

    在asan双库版本构建脚本`build/common/asan/build_mixed_asan.sh`中，是通过将asan版本的lib和bin放到`/data`目录下，然后调整系统服务进程的配置文件(`/system/etc/init/*.cfg`及`/vendor/etc/init/*.cfg`)中相关路径来实现的，因此当前目录下的预置配置文件中的`namespace.default.asan.lib.paths`配置并未被使用，而是**自动生成**的。生成规则为将`namespace.default.lib.paths`中所有以`/system/`及`/vendor/`开头的路径换成以`/data/`开头。


## 常见问题说明

**如何构建asan双库版本？**

[参考wiki](https://gitee.com/caoruihong/build/wikis)


## 相关仓

编译构建子系统

[**build**](https://gitee.com/openharmony/build)

