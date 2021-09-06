#!/bin/bash


cd tar
tar -xzvf CAENVMELib-3.3.0-build20210806.tgz
cd CAENVMELib-3.3.0/lib
sh install_x64
cd ../../

tar -xvzf CAENComm-1.5.0-build20210804.tgz
cd CAENComm-1.5.0/lib
sh install_x64
cd ../../

tar -xvzf CAENUpgrader-1.7.1-build20210402.tgz
cd CAENUpgrader-1.7.1
./configure
make -j2
make install
cd ..

tar -xvzf CAENDigitizer-2.17.0-build20210804.tgz
cd CAENDigitizer-2.17.0
sh install_64
cd ..

tar -xzvf CAENUSBdrvB-1.5.4.tgz
cd CAENUSBdrvB-1.5.4
make
make install
cd ..

tar -xzvf wavedump-3.10.3-build20210715.tar.gz
cd wavedump-3.10.3
./configure
make
make install
cd ..

#tar -zxvf CoMPASS-1.0.3.tar.gz -C /opt
#sh /opt/CoMPASS-1.0.3/install.sh

rm -rf CAENVMELib-3.3.0 CAENComm-1.5.0 CAENUpgrader-1.7.1 CAENDigitizer-2.17.0 CAENUSBdrvB-1.5.4 wavedump-3.10.3

echo "Install all software done !"

# A2818Drv-1.20-build20161118.tgz
#将该文件夹复制到 /opt 并安装在该位置
#   cp -r A2818Drv-1.20 /opt
#   cd /opt/A2818Drv-1.20
#   cp ./Makefile.2.6-3.x Makefile
#   make
#设置开机自动执行该脚本
#   emacs /etc/rc.d/rc.local
#添加 sh /opt/A2818Drv-1.19/a2818_load

