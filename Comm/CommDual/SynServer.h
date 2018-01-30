//==============================================================================
//                      SynServer.h
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_SYNSERVER_H 
#define SELFDEF_SYNSERVER_H 
 
#include "NetServer.h"
 
//==============================================================================
//                 
//                    SynServer
//                   CommDual同步服务端类
//
//==============================================================================
class CSynServer : public CNetServerDouble
{ 
public: 
	CSynServer();
	virtual ~CSynServer(); 
 
protected: 
	/* 下列的实现为服务器地址初始化的相关处理 */
	// 重载基类函数
	virtual string GetServerIdValidRangeStr(bool bIsAppSvr);
	
	// 重载基类函数
	virtual bool IsServerIdValid(bool bIsAppSvr, WORD wSvrId);
	
	/* 下列的实现为接收数据的相关处理 */
	// 重载基类函数
	// 处理从某个客户端（连入本服务器）接收的数据包。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	virtual void OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	
	/* 下列的实现为发送数据的相关处理 */
	// 向<内部数据转发服务>模块发送数据包
	// 该函数传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	void SendDataToLocalServer(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
	/* 下列的实现为接入客户端对象的相关处理 */
	virtual string GetCliendIdValidRangeStr();
	virtual bool IsClientIdValid(bool bIsAppClt, WORD wCltId);

public: 
	/* 下列的接口为参数初始化的相关处理 */
	// 初始化服务器相关的配置信息。
	// strConfigFile：	配置文件名称，包含路径
	// strService	：	服务名称
	bool Init(string strConfigFile, string strService);

	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	// 重载基类函数
	bool Startup();

	// 重载基类函数
	virtual void OnSessionConnect(CServerOneNetCltObj* pCltObj); 

	// 重载基类函数
	virtual void OnSessionReady(CServerOneNetCltObj* pCltObj); 

	// 重载基类函数
	virtual void OnSessionClose(CServerOneNetCltObj* pCltObj); 

	// 处理从本地连接接收的数据包
	void OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);

private: 
	string m_strService;		// 服务名称
}; 


 
#endif 
