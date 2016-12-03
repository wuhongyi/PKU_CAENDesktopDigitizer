#!/bin/bash

mkdir -p CAENVME
cd CAENVME
tar -xzvf ../tar/CAENVMELib-2.50.tgz
cd ..


mkdir -p CAENComm
cd CAENComm
tar -xvzf ../tar/CAENComm-1.2-build20140211.tgz
cd ..


mkdir -p CAENUpgrader
cd CAENUpgrader
tar -xvzf ../tar/CAENUpgrader-1.6.0-build20161104.tgz
cd ..


mkdir -p CAENDigitizer
cd CAENDigitizer
tar -xvzf ../tar/CAENDigitizer_2.7.6_20161014.tgz
cd ..


mkdir -p CAENUSB
cd CAENUSB
tar -xzvf ../tar/CAENUSBdrvB-1.5.1.tgz
cd ..




