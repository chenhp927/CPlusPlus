//==============================================================================
//                      NetClient.h
//                       
//begin   : 2016-09-03                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_NETCLIENT_H 
#define SELFDEF_NETCLIENT_H 

#include "ClientForwardBase.h"

//==============================================================================
//                 
//                    CClientOneNet
//                    一个网客户端类
//
//==============================================================================
class CNetClientInterface;
class MY_EXT_CLASS CClientOneNet : public CClientForwardBase
{ 
public: 
	CClientOneNet(CNetClientInterface* pParent, bool bIsForSingleNet = true);
	virtual ~CClientOneNet();
	
protected:
	/* 下列的实现为接入客户端连接建立、关闭的相关处理 */
	// 重载基类函数
	virtual void OnConnect(); 

	// 重载基类函数
	virtual void OnClose(); 

	// 重载基类函数
	virtual void OnSessionReady(); 

	// 重载基类函数
	virtual void OnSessionClose(); 

	/* 下列的实现为接收数据的相关处理 */
	// 重载基类函数
	// 处理接收的用户数据包。
	// 对于单网，处理方式同基类；对于双网，回传给父对象进行处理。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	virtual void HandleUserData(CBuffer* pBuf);
	
protected:
	CNetClientInterface* m_pParent;
	bool m_bIsForSingleNet;
};

//==============================================================================
//                 
//                    CClientInterface
//                    客户端接口类
// 说明：统一单网客户端和双网客户端的接口
//==============================================================================
class MY_EXT_CLASS CNetClientInterface
{
public:
	// 下列所有接口用到的cNetNo，代表网络编号，1代表第一个网，2代表第二个网，-1 返回总的情况
	
	/* 下列的接口为参数初始化的相关处理 */
	// 初始化网络参数。
	virtual bool InitCommParam(TiXmlHandle& txhClient) = 0;

	// 初始化入口条件信息（注册）
	virtual bool InitInCondition(TiXmlHandle& txhClient) = 0;
	
	// 初始化出口条件信息
	virtual bool InitOutCondition(TiXmlHandle& txhClient) = 0;

	/* 下列的接口为基本属性设置和获取的相关处理 */
	virtual int GetType(char cNetNo = 1) = 0;
	virtual const char* GetUnixPath(char cNetNo = 1) = 0;
	virtual string GetSvrIp(char cNetNo = 1) = 0;
	virtual int GetSvrPort(char cNetNo = 1) = 0;
	virtual string GetMyIp(char cNetNo = 1) = 0;
	virtual int GetMyPort(char cNetNo = 1) = 0;
	virtual WORD GetClientId(char cNetNo = 1) = 0;

	/* 下列的接口为出入口信息的相关处理 */
	virtual CCommAddr& GetClientAddr() = 0;
	virtual CForwardCondition* GetOutCondition() = 0;
	virtual CForwardCondition* GetInCondition() = 0;
	virtual bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead) = 0;
	virtual void SetAppClient(bool bIsAppClient = true) = 0; 
	
	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	virtual bool Connect() = 0; 
	virtual void Close() = 0; 
	virtual bool IsConnect(char cNetNo = -1) = 0;
	virtual bool IsReady(char cNetNo = -1) = 0;	
	
	virtual void OnConnect(CClientOneNet* pClient) = 0; 
	virtual void OnClose(CClientOneNet* pClient) = 0; 
	virtual void OnSessionReady(CClientOneNet* pClient) = 0; 
	virtual void OnSessionClose(CClientOneNet* pClient) = 0; 

	/* 下列的接口为通过select机制获取网络消息的相关处理 */
	virtual void SetFdset(CFdset& objFs) = 0; 
	virtual void HandleNetEvent(CFdset& objFs) = 0;  

	/* 下列的接口为接收数据的相关处理 */
	// 处理数据包接收事件。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息
	virtual void OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf) = 0;

	/* 下列的接口为获取接收数据包的相关处理 */
	virtual bool IsHasData() = 0; 

	// 提取接收到的转发层数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的并去除了会话层协议头、转发层协议头信息后数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入传出参数；
	virtual bool RecvData(char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr) = 0; 
	virtual bool RecvData(CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr) = 0; 
	virtual bool RecvData(char* szBuf, int& nBufLen) = 0; 
	virtual bool RecvData(CBuffer* pBuf) = 0; 

	// 提取接收到的原始数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的包含会话层协议头、转发层协议头信息数据包。
	// 下列接口用到的pBuf均为传入传出参数；
	virtual bool RecvRawData(CBuffer* pBuf) = 0; 

	/* 下列的接口为发送数据包的相关处理。*/
	// 发送转发层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被自动添加会话层协议头、转发层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入参数。
	virtual bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false) = 0; 
	virtual bool SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false) = 0; 
	virtual bool SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch = false) = 0; 
	virtual bool SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch = false) = 0; 

	// 发送原始数据包。
	// 传入的buffer（数据包）须是完整包，包中已包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 传入的数据包将会被自动设置双网信息。
	// 下列接口用到的pBuf均为传入参数。
	virtual bool SendRawData(CBuffer* pBuf) = 0; 

	/* 下列的接口为巡检的相关处理 */
	// 周期性调用，负责网络巡检重连、将发送队列中的数据包发送出去
	virtual void DoPatrol() = 0;
	
	/* 下列的接口为日志输出的相关处理 */
	// 在日志中打印基本参数
	virtual string GetDumpParamStr() = 0;
	virtual void DumpParam() = 0;
	virtual void DumpAllocator() = 0;

	// 获取格式化的服务端信息，主要用于日志输出
	virtual string GetTagSvrInfo(char cNetNo = 1) = 0;	
	virtual string GetTagConnInfo(char cNetNo = 1) = 0;	
};

//==============================================================================
//                 
//                    CNetClientSingle
//                      单网客户端类
//
//==============================================================================
class MY_EXT_CLASS CNetClientSingle : public CNetClientInterface, public CObjectBase
{
public: 
	CNetClientSingle();
	virtual ~CNetClientSingle();

public:
	// 以下所有接口均为CNetClientInterface的函数重载接口
	// 下列所有接口用到的cNetNo，代表网络编号，1代表第一个网，2代表第二个网，-1 返回总的情况

	/* 下列的接口为参数初始化的相关处理 */
	// 初始化网络参数。
	bool InitCommParam(TiXmlHandle& txhClient);
	
	// 初始化入口条件信息（注册）
	bool InitInCondition(TiXmlHandle& txhClient);
	
	// 初始化出口条件信息
	bool InitOutCondition(TiXmlHandle& txhClient);
	
	/* 下列的接口为基本属性设置和获取的相关处理 */
	int GetType(char cNetNo = 1);
	const char* GetUnixPath(char cNetNo = 1);
	string GetSvrIp(char cNetNo = 1);
	int GetSvrPort(char cNetNo = 1);
	string GetMyIp(char cNetNo = 1);
	int GetMyPort(char cNetNo = 1);
	WORD GetClientId(char cNetNo = 1);
	
	/* 下列的接口为出入口信息的相关处理 */
	CCommAddr& GetClientAddr();
	CForwardCondition* GetOutCondition();
	CForwardCondition* GetInCondition();
	bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead);
	void SetAppClient(bool bIsAppClient = true); 
	
	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	bool Connect(); 
	void Close(); 
	bool IsConnect(char cNetNo = -1);
	bool IsReady(char cNetNo = -1);	
	
	void OnConnect(CClientOneNet* pClient); 
	void OnClose(CClientOneNet* pClient); 
	void OnSessionReady(CClientOneNet* pClient); 
	void OnSessionClose(CClientOneNet* pClient); 
	
	/* 下列的接口为通过select机制获取网络消息的相关处理 */
	void SetFdset(CFdset& objFs); 
	void HandleNetEvent(CFdset& objFs);  
	
	/* 下列的接口为接收数据的相关处理 */
	// 处理数据包接收事件。
	// 单网客户端不需要实现，数据包可以放在m_pClient的接收队列中
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息
	void OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf);
	
	/* 下列的接口为获取接收数据包的相关处理 */
	bool IsHasData(); 
	
	// 提取接收到的转发层数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的并去除了会话层协议头、转发层协议头信息后数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入传出参数；
	bool RecvData(char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(char* szBuf, int& nBufLen); 
	bool RecvData(CBuffer* pBuf); 
	
	// 提取接收到的原始数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的包含会话层协议头、转发层协议头信息数据包。
	// 下列接口用到的pBuf均为传入传出参数；
	bool RecvRawData(CBuffer* pBuf); 
	
	/* 下列的接口为发送数据包的相关处理。*/
	// 发送转发层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被自动添加会话层协议头、转发层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入参数。
	bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch = false); 
	
	// 发送原始数据包。
	// 传入的buffer（数据包）须是完整包，包中已包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 传入的数据包将会被自动设置双网信息。
	// 下列接口用到的pBuf均为传入参数。
	bool SendRawData(CBuffer* pBuf); 
	
	/* 下列的接口为巡检的相关处理 */
	// 周期性调用，负责网络巡检重连、将发送队列中的数据包发送出去
	void DoPatrol();
	
	/* 下列的接口为日志输出的相关处理 */
	// 在日志中打印基本参数
	virtual string GetDumpParamStr();
	void DumpParam();
	virtual void DumpAllocator();
	
	// 获取格式化的信息，主要用于日志输出
	string GetTagSvrInfo(char cNetNo = 1);	
	string GetTagConnInfo(char cNetNo = 1);

	/* 下列的接口为日志文件操作的相关处理 */
	// 重载基类函数
	// 设置日志文件的标识
	virtual void SetLogFileTag(const char* szFileTag);

protected:
	CClientOneNet* m_pClient;
};

//==============================================================================
//                 
//                   CNetClientDouble
//                    双网客户端类
//
//==============================================================================
class MY_EXT_CLASS CNetClientDouble : public CNetClientInterface, public CObjectBase
{ 
public: 
	CNetClientDouble();
	virtual ~CNetClientDouble();

protected:
	bool DealSingleRecvSeq(CClientOneNet* pClient, CBuffer* pBuf);
	bool DealDoubleRecvSeq(CClientOneNet* pClient);
	void AddRecvPacketToQueue(CClientOneNet* pClient, CBuffer* pBuf);

public:
	// 以下所有接口均为CNetClientInterface的函数重载接口
	// 下列所有接口用到的cNetNo，代表网络编号，1代表第一个网，2代表第二个网，-1 返回总的情况
	
	/* 下列的接口为参数初始化的相关处理 */
	// 初始化网络参数。
	bool InitCommParam(TiXmlHandle& txhClient);
	
	// 初始化入口条件信息（注册）
	bool InitInCondition(TiXmlHandle& txhClient);
	
	// 初始化出口条件信息
	bool InitOutCondition(TiXmlHandle& txhClient);

	/* 下列的接口为基本属性设置和获取的相关处理 */
	int GetType(char cNetNo = 1);
	const char* GetUnixPath(char cNetNo = 1);
	string GetSvrIp(char cNetNo = 1);
	int GetSvrPort(char cNetNo = 1);
	string GetMyIp(char cNetNo = 1);
	int GetMyPort(char cNetNo = 1);
	WORD GetClientId(char cNetNo = 1);
	
	/* 下列的接口为出入口信息的相关处理 */
	CCommAddr& GetClientAddr();
	CForwardCondition* GetOutCondition();
	CForwardCondition* GetInCondition();
	bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead);
	void SetAppClient(bool bIsAppClient = true); 
	
	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	bool Connect(); 
	void Close(); 
	bool IsConnect(char cNetNo = -1);
	bool IsReady(char cNetNo = -1);	
	
	void OnConnect(CClientOneNet* pClient); 
	void OnClose(CClientOneNet* pClient); 
	void OnSessionReady(CClientOneNet* pClient); 
	void OnSessionClose(CClientOneNet* pClient); 
	
	/* 下列的接口为通过select机制获取网络消息的相关处理 */
	void SetFdset(CFdset& objFs); 
	void HandleNetEvent(CFdset& objFs);  
	
	/* 下列的接口为接收数据的相关处理 */
	// 处理数据包接收事件。
	// 处理双网序号、注册包。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	void OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf); 
	
	/* 下列的接口为获取接收数据包的相关处理 */
	bool IsHasData(); 
	
	// 提取接收到的转发层数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的并去除了会话层协议头、转发层协议头信息后数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入传出参数；
	bool RecvData(char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(char* szBuf, int& nBufLen); 
	bool RecvData(CBuffer* pBuf); 

	// 提取接收到的原始数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包和转发层的注册数据包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的包含会话层协议头、转发层协议头信息数据包。
	// 下列接口用到的pBuf均为传入传出参数；
	bool RecvRawData(CBuffer* pBuf); 

	/* 下列的接口为发送数据包的相关处理。*/
	// 发送转发层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被自动添加会话层协议头、转发层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、objForwardHead均为传入参数。
	bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch = false); 
	
	// 发送原始数据包。
	// 传入的buffer（数据包）须是完整包，包中已包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 传入的数据包将会被自动设置双网信息。
	// 下列接口用到的pBuf均为传入参数。
	bool SendRawData(CBuffer* pBuf); 
	
	/* 下列的接口为巡检的相关处理 */
	// 周期性调用，负责网络巡检重连、将发送队列中的数据包发送出去
	virtual void DoPatrol();
	
	/* 下列的接口为日志输出的相关处理 */
	// 在日志中打印基本参数
	virtual string GetDumpParamStr();
	void DumpParam();
	
	// 获取格式化的信息，主要用于日志输出
	string GetTagSvrInfo(char cNetNo = 1);
	string GetTagConnInfo(char cNetNo = 1);
	virtual void DumpAllocator();

	/* 下列的接口为日志文件操作的相关处理 */
	// 重载基类函数
	// 设置日志文件的标识
	virtual void SetLogFileTag(const char* szFileTag);
	
public:
	// 以下所有接口均为非函数重载接口

	/* 下列的接口为接收队列管理的相关处理 */
	void ClearRecvQueue();

protected:
	CClientOneNet* m_pClient1;
	CClientOneNet* m_pClient2;

	CProtocolForward::CForwardDataSeq m_objRecvSeq1;
	CProtocolForward::CForwardDataSeq m_objRecvSeq2;
	CProtocolForward::CForwardDataSeq m_objRecvSeq;
	CProtocolForward::CForwardDataSeq m_objSendSeq;
	
	CBuffer::CAllocator m_objBufAlloc; 	// 缓存区分配器
	CQueue<CBuffer* > m_RecvQueue; 		// 数据包接收队列
};



#endif 
