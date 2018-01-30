#!/bin/sh

cd ../
#****************************************************************************
# 此段为：编译每个库，与build.sh一致
#****************************************************************************
echo build TinyXml
cd TinyXml
make clean
cd ..
echo

echo build SysLib
cd SysLib
make clean
cd ..
echo

echo build Tools
cd Tools
make clean
cd ..
echo

echo build CommLib
cd CommLib
make clean
cd ..
echo

echo build ProtocolBase
cd ProtocolBase
make clean
cd ..
echo

echo build AppLib
cd AppLib
make clean
cd ..
echo

echo build ProtocolAlarm
cd ProtocolAlarm
make clean
cd ..
echo

echo build ProtocolIndication
cd ProtocolIndication
make clean
cd ..
echo

echo build ProtocolMainten
cd ProtocolBase
make clean
cd ..
echo