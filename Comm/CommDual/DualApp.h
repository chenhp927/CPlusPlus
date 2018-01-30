//==============================================================================
//                      DualApp.h
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_DUALAPP_H 
#define SELFDEF_DUALAPP_H

#include "UnixApp.h"
#include "LocalServer.h"
#include "SynServer.h"
#include "SynClient.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;
//==============================================================================
//                 
//                    CDualApp
//                     主程序类
//
//==============================================================================
class CDualApp : public CUnixApp
{
public:
	CDualApp();
	virtual ~CDualApp();

protected:
	// 获取程序版本号信息。基类的纯虚函数
	virtual string GetAppVersion();
	
public:
	// 程序初始化。多态
	virtual bool Init();

	// 程序主要的逻辑实现入口。多态
	virtual void Run();

	// 处理从同步连接接收的数据包
	void OnRecvDataFromSynClient(CBuffer* pBuf);
	void OnRecvDataFromSynConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);

	// 处理从本地连接接收的数据包
	void OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
private:
	// 初始化日志相关内容
	void InitLogFile();

	// 初始化网络通信的服务
	bool InitComm();

	// 定时处理网络事件
	void OnTimerCheckNetEvent();

	// 处理本地服务端网络事件
	void HandleLocalServerEvent();
	
	// 处理同步服务端网络事件
	void HandleSynServerEvent();

	// 处理同步客户端网络事件
	void HandleSynClientEvent();

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

	CLocalServer	m_objLocalServer;			// 本地服务端，接入本地INET业务客户端
	CSynServer		m_objSynServer;				// 同步服务端，接入对等机器的同步客户端
	CSynClient		m_objSynClient;				// 同步客户端，连接对等机器的同步服务端
};

#endif
