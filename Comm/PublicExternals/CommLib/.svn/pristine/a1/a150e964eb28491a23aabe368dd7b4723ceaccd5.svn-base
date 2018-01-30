//==============================================================================
//                      ClientForwardBase.h
//                       
//begin   : 2016-08-30                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_CLIENTFORWARDBASE_H 
#define SELFDEF_CLIENTFORWARDBASE_H 

#include <string>
using namespace std;

#include "ClientSessionBase.h"
#include "ForwardCondition.h"

//==============================================================================
//                 
//                    CClientForwardBase
//           转发层客户端基类（带双网、转发条件机制）
//
//==============================================================================
class TiXmlHandle;
class MY_EXT_CLASS CClientForwardBase : public CClientSessionBase
{ 
public: 
	CClientForwardBase();
	virtual ~CClientForwardBase();

protected:
	/* 下列的实现为参数初始化的相关处理 */
	// 初始化网络参数。cNetNo代表网络编号，1代表第一个网，2代表第二个网
	bool InitClientId(TiXmlHandle& txhClient);
	bool InitClientType(TiXmlHandle& txhClient);
	bool InitServerPath(TiXmlHandle& txhClient);
	bool InitMyIpPort(TiXmlHandle& txhMyIpPort, char cNetNo = 1);
	bool InitServerIpPort(TiXmlHandle& txhServerIpPort, char cNetNo = 1);
		
	/* 下列的实现为接入客户端连接建立、关闭的相关处理 */
	// 重载基类函数
	virtual void OnSessionReady();

	/* 下列的实现为发送数据的相关处理 */
	// 为业务客户端发送的数据设置源地址
	bool SetSrcAddrForSendData(char* szBuf, int nBufLen);

	// 为业务客户端发送的数据设置类型码、功能码
	bool SetTypeFuncForSendData(char* szBuf, int nBufLen);
	bool CheckSendData(char* szBuf, int nBufLen);

public:
	/* 下列的接口为参数初始化的相关处理 */
	// 初始化网络参数。cNetNo代表网络编号，1代表第一个网，2代表第二个网
	bool InitCommParam(TiXmlHandle& txhClient, char cNetNo = 1);

	// 初始化入口条件信息（注册）
	bool InitInCondition(TiXmlHandle& txhClient);

	// 初始化出口条件信息
	bool InitOutCondition(TiXmlHandle& txhClient);

	/* 下列的接口为注册信息、出口条件的相关处理 */
	CForwardCondition* GetInCondition();
	CForwardCondition* GetOutCondition();
	bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead);
	
	/* 下列的接口为获取接收数据包的相关处理 */
	// 提取接收到的转发层数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的并去除了会话层协议头、转发层协议头信息后数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入传出参数；
	// 注：要获取只包含转发层协议头信息的数据包，需调用基类的“RecvSessionData()”接口
	// 注：要获取包含会话层协议头、转发层协议头信息的数据包，需调用基类的“RecvData()”接口
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf); 
	virtual bool RecvForwardData(char* szBuf, int& nBufLen); 
	virtual bool RecvForwardData(CBuffer* pBuf); 

	/* 下列的接口为发送数据包的相关处理。*/
	// 发送转发层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被自动添加会话层协议头、转发层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入参数。
	// 注：要发送只包含转发层协议头信息的数据包，需调用基类的“SendSessionData()”接口
	// 注：要发送包含会话层协议头、转发层协议头信息的数据包，需调用基类的“SendData()”接口
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen); 
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf); 
	virtual bool SendForwardData(char* szBuf, int nBufLen); 
	virtual bool SendForwardData(CBuffer* pBuf); 

	/* 下列的接口为日志输出的相关处理 */
	// 重载基类函数
	// 在日志中打印基本参数
	virtual string GetDumpParamStr();
	virtual void DumpParam();
	virtual void DumpRegPacket();

protected:
	CForwardCondition m_objOutConds;	// 消息发送的出口条件
	CForwardCondition m_objInConds;		// 消息接收的入口条件
	CBuffer* m_pBufRegPacket;			// 客户端注册信息包buffer	
}; 

#endif 
