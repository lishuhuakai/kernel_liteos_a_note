# Musl libc

musl，是MIT授权的针对Linux系统调用的API的标准C库实现，适用于广泛的部署环境。musl提供了高效的静态和动态链接支持， 为轻量级代码并且运行时开销低，在正确使用时具有强大的故障安全保证，以及标准一致性意义上的正确性和安全性。musl建立的原则是使用便于理解和维护的简单代码来实现目标。

musl的1.1版本系列涵盖了所有在ISO C99和POSIX 2008基础中定义的接口，以及一些非标准的为了与Linux、BSD和glibc功能兼容的接口。

## 目录结构

```
README_zh.md            #中文说明
include					#musl头文件
ldso					#动态链接器
libc-test				#关于musl的测试用例
ndk_musl_include		#生成ndk版本的头文件
ndk_script				#生成ndk版本的脚本
porting                 #linux liteos平台对musl适配文件
src						#musl文件源
scripts					#测试用脚本
```

## Openharmony对于musl的适配

musl编译框架时编译，使用BUILD.gn进行配置编译。

基于openharmony的需求，为musl新增的特性：

-  加载器地址随机化，RELRO共享机制 
-  提供加载器namespace机制 
-  OHOS容器中能够运行依赖bionic的库 
-  musl全球化接口适配locale数据能力 
-  mallocng堆内存分配器安全增强，默认开启meta指针混淆。地址随机化通过MALLOC_SECURE_ALL宏开关。可在编译命令中增加--gn-args="musl_secure_level=3"开启

等。在新增特性基础上，也进行了对于musl接口功能的完善与错误的修复。

## ICONV支持的字符集编码格式

musl支持的字符集编码格式，以及受支持的别名。
> **注意**
>
> 在进行字符集编码格式转换时，请使用正确的源字符集编码格式，且目标字符集编码格式必须支持这些受转换的字符，否则转换失败。
> 在musl里不支持将源字符集编码格式转换成这五种目标字符集编码格式：gb18030，gbk，gb2312，big5和euckr。

| 编码格式 | 别名                   | musl支持情况|
|:--      |    :--                   |  :--:  |
|utf8     |                          |  支持   |
|wchart   |                          |  支持   |
|ucs2be   |                          |  支持   |
|ucs2le   |                          |  支持   |
|utf16be  |                          |  支持   |
|utf16le  |                          |  支持   |
|ucs4be   |utf32be                   |  支持   |
|ucs4le   |utf32le                   |  支持   |
|ascii    |usascii, iso646, iso646us |  支持   |
|utf16    |                          |  支持   |
|ucs4     |utf32                     |  支持   |
|ucs2     |                          |  支持   |
|eucjp    |                          |  支持   |
|shiftjis |sjis, cp932               |  支持   |
|iso2022jp|                          |  支持   |
|gb18030  |                          |  支持   |
|gbk      |                          |  支持   |
|gb2312   |                          |  支持   |
|big5     |bigfive, cp950, big5hkscs |  支持   |
|euckr    |ksc5601, ksx1001, cp949   |  支持   |
|iso88591 |latin1                    |  支持   |
|iso88592 |                          |  支持   |
|iso88593 |                          |  支持   |
|iso88594 |                          |  支持   |
|iso88595 |                          |  支持   |
|iso88596 |                          |  支持   |
|iso88597 |                          |  支持   |
|iso88598 |                          |  支持   |
|iso88599 |                          |  支持   |
|iso885910|                          |  支持   |
|iso885911|tis620                    |  支持   |
|iso885913|                          |  支持   |
|iso885914|                          |  支持   |
|iso885915|latin9                    |  支持   |
|iso885916|                          |  支持   |
|cp1250   |windows1250               |  支持   |
|cp1251   |windows1251               |  支持   |
|cp1252   |windows1252               |  支持   |
|cp1253   |windows1253               |  支持   |
|cp1254   |windows1254               |  支持   |
|cp1255   |windows1255               |  支持   |
|cp1256   |windows1256               |  支持   |
|cp1257   |windows1257               |  支持   |
|cp1258   |windows1258               |  支持   |
|koi8r    |                          |  支持   |
|koi8u    |                          |  支持   |
|cp437    |                          |  支持   |
|cp850    |                          |  支持   |
|cp866    |                          |  支持   |
|cp1047   |ibm1047                   |  支持   |

## locale数据能力支持的接口

|                 函数                 |               用途              |         返回值        |        支持的字符集       |
| :--------------------------------: | :---------------------------: | :----------------: | :-----------------: |
| int isalnum\_l(int c, locale\_t l) | 根据locale设置判断字符是否是数字或字母（包括大小写） | 参数c是数字或字符返回1，否则返回0 | zh\_CN、zh\_CN.UTF-8 |
|                                    |                               |                    |                     |

## Musl使用文档

有关完整musl目标编译器工具链、系统的信息bootstrapping和基于musl构建的Linux发行版可以在官方网站上找到：http://www.musl-libc.org/

有关基本安装说明，请参阅附带的[INSTALL](https://gitee.com/openharmony/third_party_musl/blob/master/INSTALL)文件。

musl参考手册：https://musl.libc.org/doc/1.1.24/manual.html