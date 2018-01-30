//==============================================================================
//                      BridgeApp.h
//                       
//begin   : 2016-09-18                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_BRIDGEAPP_H 
#define SELFDEF_BRIDGEAPP_H

#include "UnixApp.h"
#include "LocalClient.h"
#include "RemoteClient.h"
#include "ConnState.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;

//==============================================================================
//                 
//                    CBridgeApp
//                     主程序类
//
//==============================================================================
class CBridgeApp : public CUnixApp
{
public:
	CBridgeApp();
	virtual ~CBridgeApp();

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

	// 处理客户端网络事件
	void HandleLocalClientEvent();
	void HandleRemoteClientEvent();

	// 数据包转发处理
	void ForwardDataFromLocalToRemote(CBuffer* pBuf);
	void ForwardDataFromRemoteToLocal(CBuffer* pBuf);

	// 定时器处理
	void OnTimer();
	
	//打包
	bool MakeAllCommStatePkt(CBuffer* pBuf);
	
	//添加客户端连接状态
	void AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState);
	
private:
	CFdset m_objFdset;					// 通信库用
	CBuffer::CAllocator	m_objBufAlloc;	// 内存管理器

	CLocalClient	m_objLocalClient;	// 客户端，连接本地CommInner程序
	CRemoteClient	m_objRemoteClient;	// 客户端，连接远程CommOuter程序
};

#endif
