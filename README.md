<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 11月 26 16:41:07 2016 (+0800)
;; Last-Updated: 日 4月 16 21:49:33 2017 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 15
;; URL: http://wuhongyi.cn -->

# README

本程序专为 CAEN 桌面版 Digitizer 开发。

Author: **Hongyi Wu(吴鸿毅)**
Email: wuhongyi@qq.com 

----

将所有插件按照固件分为 Standard、PHA、PSD 三种类型（手中当前没有CI固件插件），封装成三个类。可扩展成支持该固件的任何插件。

当前拟支持：

- DT5730  固件：Standard、DPP-PSD (已完成)
- DT5724A 固件：Standard（已完成）、DPP-PHA（待开发）
- DT5742B 固件：Standard（待开发）

方便括展支持所有插件。

----

本程序包含以下文件：

- CAEN（CAEN基础包，ROOT权限下执行里面脚本即可自动安装）
- cmake
- data（默认存放数据文件夹）
- decode（将原始二进制文件转为ROOT文件，需修改decode.cc中的输入输出文件夹路径）
- firmware（存放固件，可通过**CAENUpgraderGUI**写入，没有购买授权的固件每次只能试用30分钟）
- include
- parset（待完成，将用来保存配置参数）
- src
- .gitignore
- CMakeLists.txt
- main.cc
- rootlogoff.C
- rootlogon.C

----

## 运行方法

第一次运行本程序包时(或者更新本程序包版本时)，需安装基本依赖软件(可能依赖软件有更新)。在 **CAEN** 文件夹下有自动安装脚本**setup.sh**，在 ROOT 权限下运行该脚本即可。

在该程序目录下新建名字为**build**文件夹，然后进入该文件夹编译程序，具体如下：

```shell
mkdir build
cd build
cmake ..
make
```



- 从 USB/Optical 中选择连接方式，然后按 **Connect**，如果连接成功，**Connect**按钮将变灰，出现**DeleteConnect**按钮。按钮右边将显示插件及其固件名称。
- 连接成功后，设置将要存放文件路径、文件名信息，按**Complete**按钮，如果成功，将弹出控制界面。
- 选择将要进行获取的channel，以及设置 Record Length 长度。之后按**ProgramDigitizer**按钮，成功之后再按**AllocateMemory**按钮。
- 接下来按**AdjustParFrame**按钮，将会弹出一个临时窗口（不同固件显示的界面不同），用来调节其基本参数。按钮**Load**从插件读取参数并显示到窗口（仅支持标准固件），按钮**Apply**将当前参数写入插件，按钮**Cannel**用来关闭当前临时窗口，按钮**Copy**暂时无效。
  - **PostTrigger**为trigger点之后占总采集波形的百分比。
  - **Trigger**选择ACQ_ONLY为自触发，（x730，x725比较特殊，每两个channel为一组）
  - **Polarity**当输入信号为正信号，选择Positive时为上升沿触发，选择Negative时为下降沿触发，当输入信号为负信号时，情况相反。
  - **DC Offset** 范围为-50 - +50，-50表示动态范围为（-V，0），0表示动态范围为（-V/2，V/2），+50表示动态范围为（0，V）。
  - **Threshold** 表示阈值线在 ADC 上的位置，其量程为（0，2^NBits-1）。对标准固件，只要信号幅度超过该阈值即触发。
- 接下来按**Start/Stop**来启动/停止获取。
- 按**WriteData**将数据写入文本。  
- 在线时时监测每路获取计数率，传输数据量。
- 对Online页面，在线监视某路单波形、累积波形、快速傅里叶变换。可用来快速确定合适阈值，查看信号是否正常等。
  - 选择要监视的 Channel，监视类型**Single/Mutli/Energy/FFT**，选项**Monitor**用来控制是否开启监视。**Clear**用来清屏。 
- 用decode文件夹内小程序将离线数据转为ROOT格式文件。
- （未完成）离线数据分析，梯形算法计算能量，提取上升时间等。
- （未完成）在线计算能量等。


<!-- README.md ends here -->
