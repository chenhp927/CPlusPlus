#!/bin/sh

#���ݲ�ͬ�İ汾������Ӧ�Ļ����������ر������汾�Ļ�������
#���Ծ�̬��汾
export omb_release_flag=-g
#���Զ�̬��汾
#export omb_release_flag=-g
#export omb_lib_dir=../lib
#���в��Զ�̬��汾
#export omb_release_flag="-Os -DLINUX_LIMITED"
#export omb_lib_dir=../lib
#�����ֳ���̬��汾
#export omb_release_flag="-Os -DLINUX_LIMITED"
#export omb_lib_dir=/ctc_nc/lib


cd ../
#****************************************************************************
# �˶�Ϊ������ÿ���⣬���Ը�����Ҫ���
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