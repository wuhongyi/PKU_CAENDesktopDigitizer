<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 11月 26 16:41:07 2016 (+0800)
;; Last-Updated: 三 11月 30 21:47:18 2016 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 6
;; URL: http://wuhongyi.cn -->

# README

本程序专为 CAEN 桌面版 Digitizer 开发。

将所有插件按照固件分为 Standard、PHA、PSD 三种类型（手中当前没有CI固件插件），封装成三个类。可扩展成支持该固件的任何插件。

当前拟支持：

- DT5730 DPP-PSD (开发中，12月上旬完成)
- DT5724A Standard（开发中，12月中下旬完成）
- DT5742B Standard

方便括展支持更多插件。

----

## 运行方法

- 从 USB/Optical 中选择连接方式，然后按 **Connect**，如果连接成功，**Connect**按钮将变灰，出现**DeleteConnect**按钮。按钮右边将显示插件及其固件名称。
- 连接成功后，选择将要进行获取的channel，以及设置 Record Length 长度。之后按**ProgramDigitizer**按钮，成功之后再按**AllocateMemory**按钮。
- 接下来按**AdjustParFrame**按钮，将会弹出一个临时窗口（不同固件显示的界面不同），用来调节其基本参数。按钮**Load**从插件读取参数并显示到窗口，按钮**Apply**将当前参数写入插件，按钮**Cannel**用来关闭当前临时窗口。




<!-- README.md ends here -->
