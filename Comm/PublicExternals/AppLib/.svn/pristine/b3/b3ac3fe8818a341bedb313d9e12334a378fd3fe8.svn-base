//==============================================================================
//                      UnixApp.h
//
//begin   : 2016-08-16
//describe:
//==============================================================================
#ifndef MYSELF_UNIXAPP_H
#define MYSELF_UNIXAPP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <libgen.h>
#include <assert.h>
#include <string>
#include <vector>

#ifndef HP_UX
#include <signal.h>
#endif

#include "ProtocolForward.h"
#include "Logger.h"
#include "ObjectBase.h"

typedef void(*MY_FN_SIGHANDLER)(int);

using namespace std;
//==============================================================================
//                 
//                    CUnixApp
//                    unix（包括linux）下的应用程序的通用基类
//
//==============================================================================
class CUnixApp : public CObjectBase
{
public:
	CUnixApp();
	virtual ~CUnixApp();
	static CUnixApp* GetApp();
	
public:
	// 初始化
	virtual bool Init();
	// 程序运行
	virtual void Run();
	// 退出前的清理
	virtual bool Uninit();
	
	// 设置参数相关
	void SetArgs(int argc, char** argv);

	// 获取程序相关的属性
	string GetAppName();
	string GetMainAppName();
	string GetServiceName();
	
	// 得到本机的相关属性
	bool IsRunAtCenter();							//本程序是否运行在中心服务器
	bool IsRunAtStation();							//本程序是否运行在车站分机
	unsigned char GetBureauId();					//得到本机的局码
	unsigned short GetStationId();					//本程序运行所处设备的站码
	CProtocolForward::CForwardAddr GetLocalAddr();	//得到本机的地址信息
	string GetServiceIp1();							//得到服务IP1
	string GetServiceIp2();							//得到服务IP2


protected:
	string ArgsToName(int argc, char **argv, char c='_');
	char** GetArgv();
	int GetArgc();
	
	static void SigHandler(int sig);
	
protected:
	//加载设备信息配置文件
	bool LoadDevInfo(); 
	
	// 获取程序的版本信息，派生类必须实现
	virtual string GetAppVersion() = 0;
	virtual string GetAppUsage();

	//如果是程序的帮助选项，那么在处理之后退出程序
	virtual void CheckHelpOptions(int argc, char **argv);
	//程序开始运行时检查并处理命令行参数
	virtual bool GetOptions(int argc, char **argv);

	virtual void SetAppName(int argc, char **argv);
	virtual void SetLogFile(int argc, char **argv);

	//检查程序实例是否已经运行，是则退出程序，否则继续运行
	//缺省实现为检查，你可以在派生类中派生该函数实现体为空即可不检查
	virtual void SingleCheck(int argc, char **argv);
	
	virtual void SigSet();
	virtual MY_FN_SIGHANDLER GetSigHandler();

protected:
	int m_argc;										// 参数个数
	char** m_argv;									// 参数列表

	string m_strAppName;							// 格式为：“程序名称 参数1 参数2...”
	string m_strMainAppName;						// 格式为：“程序名称”
	string m_strService;							// 服务名称，一般为程序第一个参数	
	
	CProtocolForward::CForwardAddr m_objLocalAddr;	// 本机的地址
	string m_strServiceIp1;							// 本地服务ip1
	string m_strServiceIp2;							// 本地服务ip2
	bool m_bIsDevCfgLoaded;							// 设备信息配置文件是否已经加载
};

#endif
