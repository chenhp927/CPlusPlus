//==============================================================================
//                      ChannelApp.h
//                       
//begin   : 2016-09-18                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_COMMCHANNELAPP_H 
#define SELFDEF_COMMCHANNELAPP_H

#include "UnixApp.h"
#include "ChannelClient.h"

//==============================================================================
//                 
//                    CChannelApp
//                     主程序类
//
//==============================================================================
class CChannelApp : public CUnixApp
{
public:
	CChannelApp();
	virtual ~CChannelApp();

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
	void HandleLeftClientEvent();
	void HandleRightClientEvent();

	// 数据包转发处理
	void ForwardDataFromLeftToRight(CBuffer* pBuf);
	void ForwardDataFromRightToLeft(CBuffer* pBuf);

private:
	CFdset m_objFdset;							// 通信库用
	CBuffer::CAllocator	m_objBufAlloc;			// 内存管理器

	CChannelClient	m_objLeftClient;			// 客户端，连接CommInner程序
	CChannelClient	m_objRightClient;			// 客户端，连接CommInner程序
};

#endif
