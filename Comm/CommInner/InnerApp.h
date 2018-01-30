//==============================================================================
//                      InnerApp.h
//                       
//begin   : 2016-09-10                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_INNERAPP_H 
#define SELFDEF_INNERAPP_H

#include "UnixApp.h"
#include "InnerServer.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;
//==============================================================================
//                 
//                    CInnerApp
//                     主程序类
//
//==============================================================================
class CInnerApp : public CUnixApp
{
public:
	CInnerApp();
	virtual ~CInnerApp();

protected:
	// 获取程序版本号信息。基类的纯虚函数
	virtual string GetAppVersion();
	
public:
	// 程序初始化。多态
	virtual bool Init();

	// 程序主要的逻辑实现入口。多态
	virtual void Run();

private:
	// 初始化日志相关内容
	void InitLogFile();

	// 初始化网络通信的服务
	bool InitComm();

	// 定时处理网络事件
	void OnTimerCheckNetEvent();

	// 处理服务端网络事件
	void HandleServerEvent();
	
	// 定时器处理
	void OnTimer();
	
	//发送连接状态包
	void SendConnStatePkt(CBuffer* pBuf);
	
	//打包
	bool MakeAllCommStatePkt(CBuffer* pBuf);
	
	//添加客户端连接状态
	void AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState);

private:
	CFdset m_objFdset;							// 通信库用
	CBuffer::CAllocator	m_objBufAlloc;			// 内存管理器

	CInnerServer m_objServer;					// 网络连接服务端
};

#endif
