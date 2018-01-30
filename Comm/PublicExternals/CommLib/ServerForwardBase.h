//==============================================================================
//                      ServerForwardBase.h
//
//begin   : 2016-09-08
//describe:
//==============================================================================
#ifndef SELFDEF_SERVERFORWARDBASE_H 
#define SELFDEF_SERVERFORWARDBASE_H 
 
#include <vector>

#include "ServerSessionBase.h" 
#include "ProtocolForward.h"
#include "ForwardCondition.h"
 
using namespace std;
//==============================================================================
//                 
//                    CServerForwardBase
//           转发层客户端基类（带双网、转发条件机制）
//
//==============================================================================
class CServerForwardBaseCltObj;
class MY_EXT_CLASS CServerForwardBase : public CServerSessionBase
{ 
public: 
	CServerForwardBase(); 
	virtual ~CServerForwardBase(); 
 
protected: 
	/* 下列的实现为发送数据的相关处理 */
	// 为业务服务端发送的数据设置源地址
	bool SetSrcAddrForSendData(char* szBuf, int nBufLen);

	// 为业务服务端发送的数据设置类型码、功能码
	bool SetTypeFuncForSendData(char* szBuf, int nBufLen);
	bool CheckSendData(char* szBuf, int nBufLen);
	
	/* 下列的实现为接入客户端对象的相关处理 */
	// 重载基类函数
	virtual CServerBaseCltObj* CreateClientObject();
	
public:  
	/* 下列的接口为网络参数初始化的相关处理 */
	// 初始化网络参数。cNetNo代表网络编号，1代表第一个网，2代表第二个网
	bool InitCommParam(TiXmlHandle& txhServer, char cNetNo = 1); 

	/* 下列的接口为获取接收数据包的相关处理 */
	// 提取接收到的转发层数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的并去除了会话层协议头、转发层协议头信息后数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入传出参数；
	// 下列接口用到的szCltIP、nCltPort、objSid为传出参数
	// 注：要获取只包含转发层协议头信息的数据包，需调用基类的“RecvSessionData()”接口
	// 注：要获取包含会话层协议头、转发层协议头信息的数据包，需调用基类的“RecvData()”接口
	virtual bool RecvForwardData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort);
	virtual bool RecvForwardData(CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort); 
	virtual bool RecvForwardData(char* szBuf, int& nBufLen, CSid& objSid); 
	virtual bool RecvForwardData(CBuffer* pBuf, CSid& objSid); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen, CSid& objSid); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, CSid& objSid);

	/* 下列的接口为发送数据包的相关处理。*/
	// 发送转发层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被自动添加会话层协议头、转发层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、szCltIP、nCltPort、objSid、pCltObj、objForwardHead均为传入参数。
	// 注：要发送只包含转发层协议头信息的数据包，需调用基类的“SendSessionData()”接口
	// 注：要发送包含会话层协议头、转发层协议头信息的数据包，需调用基类的“SendData()”接口
	virtual bool SendForwardData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);
	virtual bool SendForwardData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(char* szBuf, int nBufLen, CSid objSid);
	virtual bool SendForwardData(CBuffer* pBuf, CSid objSid);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, CSid objSid);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, CSid objSid);
}; 

//==============================================================================
//                 
//                    CServerForwardBaseCltObj
//           转发层客户端基类的客户端对象类（带双网、转发条件机制）
//
//==============================================================================
class MY_EXT_CLASS CServerForwardBaseCltObj : public CServerSessionBaseCltObj
{ 
public: 
	CServerForwardBaseCltObj(CServerForwardBase* pServer); 
	virtual ~CServerForwardBaseCltObj(); 
 
public: 
	/* 下列的接口为注册信息的相关处理 */
	// 处理注册包。
	// 该函数传入的buffer（数据包）不包含会话层协议头信息
	bool HandleRegisterPkt(char* szBuf, int nBufLen);

	bool IsRegistered(CProtocolForward::CForwardHead& objForwardHead);
	void ClearRegisterInfo();
	CForwardCondition& GetForwardCondition();
	
protected: 
	CForwardCondition	m_objForwardCondition;		// 客户端注册后缓存其转发条件
}; 
   
#endif 
