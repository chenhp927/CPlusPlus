#!/bin/sh


#�������ʱĿ¼
APP_PATH=""
#�������ʱ����
APP_NAME=""
#���һ��ģ�飬ÿ������ͨ�ò���Ҫ�޸�
clean_app()
{
	echo build ${APP_NAME}
	cd ${APP_PATH}
	make clean
	cd ../
	echo
}

#����linux�µĿ⣬������ɺ�Ӧ�ûص�AppSvrĿ¼����Ϊ�����ĳ��������AppSvrĿ¼Ϊ����
echo build PublicExternals
cd ../PublicExternals/Script
./clean.sh
cd ../../
echo

#=============================���¸���ģ����ӡ��޸ģ����ϵ����ݲ������޸�=======================================

#����ģ��<�ڲ�����ת������>...... 
echo build ====CommInner====
APP_PATH="CommInner"
APP_NAME="CommInner"
clean_app

#����ģ��<�ⲿ����ת������>...... 
echo build ====CommOuter====
APP_PATH="CommOuter"
APP_NAME="CommOuter"
clean_app

#����ģ��<�ն�����ת������>...... 
echo build ====CommTerm====
APP_PATH="CommTerm"
APP_NAME="CommTerm"
clean_app

#����ģ��<�ڲ�ͨ����ͨ����>...... 
echo build ====CommChannel====
APP_PATH="CommChannel"
APP_NAME="CommChannel"
clean_app

#����ģ��<�ⲿͨ���Žӷ���>...... 
echo build ====CommBridge====
APP_PATH="CommBridge"
APP_NAME="CommBridge"
clean_app


