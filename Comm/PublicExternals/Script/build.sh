#!/bin/sh

#根据不同的版本，打开相应的环境变量，关闭其他版本的环境变量
#调试静态库版本
export omb_release_flag=-g
#调试动态库版本
#export omb_release_flag=-g
#export omb_lib_dir=../lib
#发行测试动态库版本
#export omb_release_flag="-Os -DLINUX_LIMITED"
#export omb_lib_dir=../lib
#发行现场动态库版本
#export omb_release_flag="-Os -DLINUX_LIMITED"
#export omb_lib_dir=/ctc_nc/lib


cd ../
#****************************************************************************
# 此段为：编译每个库，可以根据需要添加
#****************************************************************************
echo build TinyXml
cd TinyXml
make clean
make
cd ..
echo

echo build SysLib
cd SysLib
make clean
make
cd ..
echo

echo build Tools
cd Tools
make clean
make
cd ..
echo

echo build CommLib
cd CommLib
make clean
make
cd ..
echo

echo build ProtocolBase
cd ProtocolBase
make clean
make
cd ..
echo

echo build AppLib
cd AppLib
make clean
make
cd ..
echo

echo build ProtocolAlarm
cd ProtocolAlarm
make clean
make
cd ..
echo

echo build ProtocolIndication
cd ProtocolIndication
make clean
make
cd ..
echo

echo build ProtocolMainten
cd ProtocolMainten
make clean
make
cd ..
echo