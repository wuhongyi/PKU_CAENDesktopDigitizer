#!/bin/bash


cd tar
tar -xzvf CAENVMELib-2.50.tgz
cd CAENVMELib-2.50/lib
sh install_x64
cd ../../

tar -xvzf CAENComm-1.2-build20140211.tgz
cd CAENComm-1.2/lib
sh install_x64
cd ../../


tar -xvzf CAENUpgrader-1.6.0-build20161104.tgz
cd CAENUpgrader-1.6.0
./configure
make -j2
make install
cd ..



tar -xvzf CAENDigitizer_2.7.6_20161014.tgz
cd CAENDigitizer_2.7.6
sh install_64
cd ..



tar -xzvf CAENUSBdrvB-1.5.1.tgz
cd CAENUSBdrvB-1.5.1
make
make install
cd ..

rm -rf CAENVMELib-2.50 CAENComm-1.2 CAENUpgrader-1.6.0 CAENDigitizer_2.7.6 CAENUSBdrvB-1.5.1

echo "Install all software done !"

# A2818Drv-1.19-build20150826.tgz
#将该文件夹复制到 /opt 并安装在该位置
#   cp -r A2818Drv-1.19 /opt
#   cd /opt/A2818Drv-1.19
#   cp ./Makefile.2.6-3.x Makefile
#   make
#设置开机自动执行该脚本
#   emacs /etc/rc.d/rc.local
#添加 sh /opt/A2818Drv-1.19/a2818_load

