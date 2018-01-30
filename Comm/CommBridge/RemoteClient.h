//==============================================================================
//                      RemoteClient.h
//
//begin   : 2016-09-18
//describe:
//==============================================================================
#ifndef SELFDEF_REMOTECLIENT_H
#define SELFDEF_REMOTECLIENT_H

#include "NetClient.h"

//==============================================================================
//                 
//                    CRemoteClient
//                   CommBridge远程客户端类
//
//==============================================================================
class CRemoteClient : public CNetClientDouble
{
public:
	CRemoteClient();
	virtual ~CRemoteClient();

private:
	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	// 重载基类函数
	void OnConnect(CClientOneNet* pClient); 

	// 重载基类函数
	void OnClose(CClientOneNet* pClient); 

	// 重载基类函数
	void OnSessionReady(CClientOneNet* pClient); 

	// 重载基类函数
	void OnSessionClose(CClientOneNet* pClient); 

public:
	/* 下列的接口为参数初始化的相关处理 */
	// 初始化客户端相关的配置信息。
	// strConfigFile：	配置文件名称，包含路径
	// strService	：	服务名称
	bool Init(string strConfigFile, string strService);

	/* 下列的接口为发送数据包的相关处理。*/
	// 重载基类函数
	// 发送原始数据包。
	// 传入的buffer（数据包）须是完整包，包中已包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 传入的数据包将会被自动设置双网信息。
	// 下列接口用到的pBuf均为传入参数。
	bool SendRawData(CBuffer* pBuf); 

private:
	string m_strService;		// 服务名称
};

#endif
