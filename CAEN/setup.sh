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


tar -xvzf CAENUpgrader-1.6.3-build20170511.tgz
cd CAENUpgrader-1.6.3
./configure
make -j2
make install
cd ..



tar -xvzf CAENDigitizer_2.11.0_20180212.tgz 
cd CAENDigitizer_2.11.0
sh install_64
cd ..



tar -xzvf CAENUSBdrvB-1.5.2.tgz
cd CAENUSBdrvB-1.5.2
make
make install
cd ..


tar -xzvf wavedump-3.8.2.tar.gz
cd wavedump-3.8.2
./configure
make
make install
cd ..

#tar -zxvf CoMPASS-1.0.3.tar.gz -C /opt
#sh /opt/CoMPASS-1.0.3/install.sh




rm -rf CAENVMELib-2.50 CAENComm-1.2 CAENUpgrader-1.6.3 CAENDigitizer_2.11.0 CAENUSBdrvB-1.5.2 wavedump-3.8.2

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

