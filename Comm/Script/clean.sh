#!/bin/sh


#程序的临时目录
APP_PATH=""
#程序的临时名称
APP_NAME=""
#清除一个模块，每个程序通用不需要修改
clean_app()
{
	echo build ${APP_NAME}
	cd ${APP_PATH}
	make clean
	cd ../
	echo
}

#编译linux下的库，编译完成后应该回到AppSvr目录，因为后续的程序编译以AppSvr目录为基础
echo build PublicExternals
cd ../PublicExternals/Script
./clean.sh
cd ../../
echo

#=============================以下根据模版添加、修改，以上的内容不允许修改=======================================

#编译模块<内部数据转发服务>...... 
echo build ====CommInner====
APP_PATH="CommInner"
APP_NAME="CommInner"
clean_app

#编译模块<外部数据转发服务>...... 
echo build ====CommOuter====
APP_PATH="CommOuter"
APP_NAME="CommOuter"
clean_app

#编译模块<终端数据转发服务>...... 
echo build ====CommTerm====
APP_PATH="CommTerm"
APP_NAME="CommTerm"
clean_app

#编译模块<内部通道连通服务>...... 
echo build ====CommChannel====
APP_PATH="CommChannel"
APP_NAME="CommChannel"
clean_app

#编译模块<外部通道桥接服务>...... 
echo build ====CommBridge====
APP_PATH="CommBridge"
APP_NAME="CommBridge"
clean_app


