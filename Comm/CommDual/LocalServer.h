//==============================================================================
//                      LocalServer.h
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_LOCALSERVER_H 
#define SELFDEF_LOCALSERVER_H 
 
#include "ServerForwardBase.h"
#include "ConnState.h"
 
//==============================================================================
//                 
//                    CLocalServer
//                    CommDual本地服务端类
//
//==============================================================================
class CLocalServerCltObj;
class CLocalServer : public CServerForwardBase
{ 
public: 
	CLocalServer();
	virtual ~CLocalServer(); 
 
protected: 
	/* 下列的实现为服务器地址初始化的相关处理 */
	// 重载基类函数
	virtual string GetServerIdValidRangeStr();
	
	// 重载基类函数
	virtual bool IsServerIdValid();
	
	/* 下列的实现为接收数据的相关处理 */
	// 重载基类函数
	// 处理接收的用户数据包。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	virtual void HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf);
	
	// 发送到同步通道。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	void ForwardDataToSynConn(CLocalServerCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
	// 本地转发。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	void ForwardDataToMyOtherClients(CLocalServerCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);

	/* 下列的实现为接入客户端对象的相关处理 */
	// 重载基类函数
	virtual CServerBaseCltObj* CreateClientObject(); 

	// 重载基类函数
	virtual string GetCliendIdValidRangeStr();

	// 重载基类函数
	virtual bool IsClientIdValid(CServerSessionBaseCltObj* pCltObj);

	// 重载基类函数
	virtual bool IsClientIpValid(struct sockaddr_in& stuSockAddr);  

	// 重载基类函数
	virtual bool IsTooMuchSameClient(struct sockaddr_in& stuSockAddr);  
	
	// 判定数据包的目的地址与客户端是否符合
	bool IsDataDestAddrMatchClt(CProtocolForward::CForwardHead& objHead, CLocalServerCltObj* pCltObj);

	/* 下列的实现为发送数据的相关处理 */
	// 发送原始数据包。
	// 传入的buffer（数据包）须是完整包，包中已包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pCltObj均为传入参数。
	bool SendRawData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);
	
	// 重载基类函数
	virtual void OnSessionReady(CSid objSid); 
	
	// 重载基类函数
	virtual void OnSessionClose(CSid objSid); 
	
public: 
	/* 下列的接口为参数初始化的相关处理 */
	// 初始化服务器相关的配置信息。
	// strConfigFile：	配置文件名称，包含路径
	// strService	：	服务名称
	bool Init(string strConfigFile, string strService);

	// 处理从同步连接接收的数据包
	void OnRecvDataFromSynConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
		
	//获取连接的Sid集合
	list<CSid>& GetSid();
	
private: 
	string m_strService;		// 服务名称
	list<CSid>  m_lstSid;       // 连接的Sid集合
	
}; 

//==============================================================================
//                 
//                    CLocalServerCltObj
//                CommDual本地服务端类的客户端对象类
//
//==============================================================================
class CLocalServerCltObj : public CServerForwardBaseCltObj
{ 
public: 
	CLocalServerCltObj(CLocalServer* pServer); 
	virtual ~CLocalServerCltObj(); 
};
 
#endif 
