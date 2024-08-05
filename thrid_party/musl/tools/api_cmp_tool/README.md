### 一、环境配置

#### 1、clang

若ubuntu中无clang则需要安装clang

以下为安装命令：

~~~~shell
sudo apt install clang
~~~~

#### 2、python3.8

本程序需要使用python3.8程序和pip3

以下为安装命令：

~~~~shell
sudo apt install python3.8
sudo apt install python3-pip
~~~~

#### 3、安装项目依赖包

使用以下命令来安装项目依赖包

~~~~shell
pip3 install -r requirements.txt
~~~~

### 二、黑名单文件与宏定义文件格式

#### 1、黑名单文件格式

**黑名单文件：**在执行脚本时可以指定文本文件为黑名单文件，在比较时就不会将黑名单文件中出现的符号体现在比较结果中。黑名单文件中每一行为一个忽略比较的符号信息。以下为黑名单文件的样例：

~~~~txt
mq_getattr
getutxent
.....
~~~~
本工具自带一个默认的黑名单文件default_black.txt，后续添加黑名单内容可在此基础上进行添加

#### 2、宏定义文件格式

**宏定义文件：**在执行脚本时可以指定文本文件为宏定义文件，以便于在获取头文件符号列表时可以按照用户希望的方式展开制定宏。宏定义文件中每一行为一个引入的宏定义。以下为宏定义文件的样例：

~~~~txt
_GNU_SOURCE
_BSD_SOURCE
......
~~~~

### 三、执行命令

执行脚本时提示信息如下

~~~~shell
usage: python3.8 compare.py [-h] -l <path> -i <path>
           [-b <path>] [-m <path>] [-o <path>] 
options:
	-h    : print this help message          #打印帮助信息
	-l    : input lib file path              #库文件路径
	-i    : input head file path             #头文件路径
	-b    : input blacklist file path        #黑名单文件路径（可选）
	-m    : input macros file path           #宏定义文件路径（可选）
	-o    : result file output path          #比较结果输出路径（可选）
~~~~

以下为执行命令的例子

~~~~shell
$python3.8 compare.py -l ~/libc.so -i ~/include
$python3.8 compare.py -h
$python3.8 compare.py -l ~/libc.so -i ~/include -b ~/backlist.txt -m macros.txt -o ~/
~~~~