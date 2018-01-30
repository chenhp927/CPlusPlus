#!/bin/sh

#������ɺ����Ĵ��Ŀ¼��
BIN_DIR="/Scada/AppSvrA/Bin/"

#�������ʱĿ¼
APP_PATH=""
#�������ʱ����
APP_NAME=""
#����һ��ģ�飬ÿ������ͨ�ò���Ҫ�޸�
make_app()
{
	echo build ${APP_NAME}
	cd ${APP_PATH}
	make clean
	make
	echo copy ${APP_NAME} to ${BIN_DIR}
	cp ${APP_NAME} ${BIN_DIR}
	cd ../
	echo
}

#����linux�µĿ⣬������ɺ�Ӧ�ûص�AppSvrĿ¼����Ϊ�����ĳ��������AppSvrĿ¼Ϊ����
echo build PublicExternals
cd ../PublicExternals/Script
./build.sh
cd ../../
echo

#=============================���¸���ģ����ӡ��޸ģ����ϵ����ݲ������޸�=======================================

#����ģ��<�ڲ�����ת������>...... 
echo build ====CommInner====
APP_PATH="CommInner"
APP_NAME="CommInner"
make_app

#����ģ��<�ⲿ����ת������>...... 
echo build ====CommOuter====
APP_PATH="CommOuter"
APP_NAME="CommOuter"
make_app

#����ģ��<�ն�����ת������>...... 
echo build ====CommTerm====
APP_PATH="CommTerm"
APP_NAME="CommTerm"
make_app

#����ģ��<�ڲ�ͨ����ͨ����>...... 
echo build ====CommChannel====
APP_PATH="CommChannel"
APP_NAME="CommChannel"
make_app

#����ģ��<�ⲿͨ���Žӷ���>...... 
echo build ====CommBridge====
APP_PATH="CommBridge"
APP_NAME="CommBridge"
make_app

