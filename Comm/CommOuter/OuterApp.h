//==============================================================================
//                      OuterApp.h
//                       
//begin   : 2016-09-13                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_OUTERAPP_H 
#define SELFDEF_OUTERAPP_H

#include "UnixApp.h"
#include "OuterServer.h"
#include "OuterClient.h"
#include "ConnState.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;

//==============================================================================
//                 
//                    COuterApp
//                     主程序类
//
//==============================================================================
class COuterApp : public CUnixApp
{
public:
	COuterApp();
	virtual ~COuterApp();

protected:
	// 获取程序版本号信息。基类的纯虚函数
	virtual string GetAppVersion();
	
public:
	// 程序初始化。多态
	virtual bool Init();

	// 程序主要的逻辑实现入口。多态
	virtual void Run();

	// 向CommInner进程发送数据包
	void SendDataToCommInner(CBuffer* pBuf);

private:
	// 初始化日志相关内容
	void InitLogFile();

	// 初始化网络通信的服务
	bool InitComm();

	// 定时处理网络事件
	void OnTimerCheckNetEvent();

	// 处理客户端网络事件
	void HandleClientEvent();

	// 处理服务端网络事件
	void HandleServerEvent();

	// 定时器处理
	void OnTimer();

	//打包
	bool MakeAllCommStatePkt(CBuffer* pBuf);
	
	//添加客户端连接状态
	void AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState);
	
private:
	CFdset m_objFdset;							// 通信库用
	CBuffer::CAllocator	m_objBufAlloc;			// 内存管理器

	COuterClient m_objClient;					// 客户端，连接CommInner程序
	COuterServer m_objServer;					// 服务端，允许INET客户端接入
};

#endif
