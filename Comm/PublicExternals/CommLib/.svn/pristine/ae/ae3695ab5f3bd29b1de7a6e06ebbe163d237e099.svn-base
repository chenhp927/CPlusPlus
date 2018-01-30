//==============================================================================
//                      ServerSessionBase.h
//
//begin   : 2016-09-08
//describe:
//==============================================================================
#ifndef SELFDEF_SERVERSESSIONBASE_H 
#define SELFDEF_SERVERSESSIONBASE_H 
 
#include <vector>

#include "ServerBase.h" 
#include "ProtocolSession.h"
 
using namespace std;
//==============================================================================
//                 
//                    CServerSessionBase
//           会话层服务端基类（带协商、心跳、发送确认的机制）
//
//==============================================================================
class CServerSessionBaseCltObj;
class MY_EXT_CLASS CServerSessionBase : public CServerBase
{ 
public: 
	CServerSessionBase(); 
	virtual ~CServerSessionBase(); 
 
protected:  
	/* 下列的实现为服务器地址初始化的相关处理 */
	bool LoadServerAddr();
	virtual string GetServerIdValidRangeStr();
	virtual bool IsServerIdValid();
	
	/* 下列的实现为接入客户端连接建立、关闭的相关处理 */
	// 重载基类函数
	virtual void OnSessionConnect(CSid objSid); 
	virtual void OnSessionReady(CSid objSid); 

	/* 下列的实现为接收数据的相关处理 */
	// 重载基类函数
	// 根据协议计算数据包的长度，以便于本服务端基类根据接收的数据进行数据包截
	// 取，若根据已接收的数据还无法计算数据包长度时，返回-1，等再次接收部分数
	// 据后再进行计算。
	virtual int GetRecvPacketSize(CBuffer* pBuf);
	
	// 重载基类函数
	// 处理接收到的完整数据包。判定数据包类型并进行相关处理。
	virtual void ProcessStreamPacket(CServerBaseCltObj* pCltObj, CBuffer* pBuf); 

	bool AuthLevel(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf);
	virtual void OnRecvData(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf); 
	virtual void OnRecvHeart(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf); 
	virtual void OnRecvDataAck(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuff); 
	virtual void OnRecvNegReq(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf); 
	virtual void OnRecvOtherPacket(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf); 
	void HeartDead(CServerSessionBaseCltObj* pCltObj);  
	
	// 处理接收的用户数据包。将用户数据包加入接收队列。
	// 派生类可重载本函数对用户数据包进行理。
	virtual void HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf);

	/* 下列的实现为发送数据的相关处理 */
	// 重载基类函数
	// 向网络发送数据之前设置数据编号
	virtual bool Write(CServerBaseCltObj* pCltObj); 

	virtual void SendHeart(CServerSessionBaseCltObj* pCltObj);  
	virtual void SendDataAck(CServerSessionBaseCltObj* pCltObj); 
	void AckFail(CServerSessionBaseCltObj* pCltObj);

	virtual bool WriteSp(CServerSessionBaseCltObj* pCltObj); 
	virtual void OnWriteSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf); 
	virtual void OnWriteCompletedSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf); 
	virtual void OnWritePartSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf, int nSend); 
	virtual void OnWriteErrorSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf); 

	/* 下列的实现为巡检的相关处理 */
	// 重载基类函数
	virtual void PatrolClientObject(CServerBaseCltObj* pCltObj);

	/* 下列的实现为接入客户端对象的相关处理 */
	// 重载基类函数
	virtual CServerBaseCltObj* CreateClientObject(); 
	short GetClientId(CSid objSid);
	virtual bool IsClientIdExist(CServerSessionBaseCltObj* pCltObj);
	virtual string GetCliendIdValidRangeStr();
	virtual bool IsClientIdValid(CServerSessionBaseCltObj* pCltObj);
	
public: 
	/* 下列的接口为基本属性设置和获取的相关处理 */
	void SetHeartAttributes(int nHeartInterval = DEF_HEARTINTERVAL, 
							int nHeartTimeout = DEF_HEARTTIMEOUT); 
	void SetAckAttributes(BYTE byWindowSize = DEF_COMMWINSIZE, 
		int nAckInterval = DEF_ACKINTERVAL, int nAckTimeout = DEF_ACKTIMEOUT); 

	void SetServerId(DWORD dwServerId);
	DWORD GetServerId();
	CCommAddr& GetServerAddr();
	void SetAppServer(bool bIsAppServer = true);
	bool IsAppServer();

	/* 下列的接口为获取接收数据包的相关处理 */
	// 提取接收到的会话层数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的并去除了会话层协议头信息后数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf均为传入传出参数；
	// 下列接口用到的szCltIP、nCltPort、objSid为传出参数
	// 注：要获取含会话层协议头信息的数据包，需调用基类的“RecvData()”接口
 	virtual bool RecvSessionData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort);
	virtual bool RecvSessionData(CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort); 
 	virtual bool RecvSessionData(char* szBuf, int& nBufLen, CSid& objSid); 
	virtual bool RecvSessionData(CBuffer* pBuf, CSid& objSid); 

	/* 下列的接口为发送数据包的相关处理。*/
	// 发送会话层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头信息。
	// 传入的数据包将会被自动添加会话层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、szCltIP、nCltPort、objSid、pCltObj均为传入参数。
	// 注：要发送含会话层协议头信息的数据包，需调用基类的“SendData()”接口
	virtual bool SendSessionData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);
	virtual bool SendSessionData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendSessionData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendSessionData(char* szBuf, int nBufLen, CSid objSid);
	virtual bool SendSessionData(CBuffer* pBuf, CSid objSid);

	/* 下列的接口为日志输出的相关处理 */
	// 重载基类函数
	// 在日志中打印基本参数
	virtual string GetDumpParamStr();
	virtual void DumpParam();

protected: 
	int 	m_nHeartInterval; 			// 心跳包发送间隔时间
	int 	m_nHeartTimeout; 			// 心跳包接收超时时间
	int		m_nAckInterval;  			// 确认包发送间隔时间
	int		m_nAckTimeout; 				// 确认包接收超时时间
	BYTE	m_byWindowSize;				// 窗口大小
	
	CCommAddr m_objServerAddr;			// 服务端地址，可为应用数据包打上源地址
	bool	m_bIsAppServer;				// 本服务端是否为业务程序服务端
}; 

//==============================================================================
//                 
//                    CServerSessionBaseCltObj
//           会话层服务端基类的客户端对象类（带协商、心跳、发送确认的机制）
//
//==============================================================================
class MY_EXT_CLASS CServerSessionBaseCltObj : public CServerBaseCltObj
{ 
public: 
	CServerSessionBaseCltObj(CServerSessionBase* pServer); 
	virtual ~CServerSessionBaseCltObj(); 
 
protected: 
	/* 下列的实现为客户端网络连接启动、关闭、判定的相关处理 */
	// 重载基类函数
	virtual void OnConnect(); 

	// 重载基类函数
	virtual void OnClose(); 
	
	/* 下列的实现为数据包发送队列操作的相关处理 */
	void ClearSpSendQueue(); 

public: 
	/* 下列的接口为基本属性设置和获取的相关处理 */
	void SetWindowSize(BYTE bySize);
	BYTE GetWindowSize();
	void SetPairNetIp(const char* szIp);
	string GetPairNetIp();
	void SetClientId(short sClientId);
	short GetClientId();
	void SetClientAddr(CCommAddr& objCltAddr);
	CCommAddr& GetClientAddr();
	void SetAppClient(bool bIsAppClient = true);
	bool IsAppClient();

	// 判定本客户端对象与pCltObj客户端对象是否是一对双网连接
	bool IsPairWith(CServerSessionBaseCltObj* pCltObj);	
		
	/* 下列的接口为客户端网络连接启动、关闭、判定的相关处理 */
	// 重载基类函数
	virtual void Connect(int nSockfd, const struct sockaddr_in& stuSockAddr);
	
	// 重载基类函数
	virtual void Connect(int nSockfd);
	
	// 重载基类函数
	virtual bool Close(); 
	bool IsReady();
  
	/* 下列的接口为发送会话层数据的相关处理 */
	DWORD GetSendWindowFullTime();
	DWORD GetSendSeq();
	void IncSendSeq();	
	void PushSendSeq();
	bool IsNeedToAck() const; 
	bool IsSendWindowFull() const;
	BYTE GetSendCount();
	CQueue<CBuffer* >& GetSpSendQueue();
	bool RollbackDataSp(CBuffer* pBuf);
	
	/* 下列的接口为接收会话层数据的相关处理 */
	DWORD GetRecvWindowFirstTime();
	void InitRecvSeq(DWORD dwSeq);
	void PushRecvSeq(DWORD dwSeq);
	DWORD GetLastRecvSeq() const;
	bool IsRecvWindowFull() const; 
	BYTE GetRecvCount() const;
	void ClearRecvWindow();
	void OnDataAck(DWORD dwSeq);

	/* 下列的接口为日志输出的相关处理 */
	// 重载基类函数
	// 获取格式化的客户端信息，主要用于日志输出
	virtual string GetTagCltInfo();	
	virtual string GetTagCltAddrInfo();

protected:  
	// 发送队列中数据包CBuffer的m_nWParam、m_nFlag属性有特殊应用。
	// 在通过send()函数将数据包发送到网络上时，由于每次调用并不一定能够将整个数据包发送完整，所以需要进行标记。
	// m_nFlag用于标记本数据包是否为非完整数据包，0-完整数据包；1-非完整数据包
	// m_nWParam用于记录本数据包为完整数据包时的长度
	CQueue<CBuffer* > m_SpSendQueue;			// 会话层协议发送队列，用以发送协商、心跳、确认包
	
	BYTE m_byWindowSize;						// 窗口大小

	DWORD	m_dwSendWinFullTime;				// 发送窗口中填满窗口那一包的发包时刻				 
	DWORD	m_dwSendSeq; 						// 发送序号
	DWORD	m_SendWindow[MAX_COMMWINSIZE];		// 发送窗口
	BYTE	m_bySendCount; 						// 发送窗口中已发包数量

	DWORD	m_dwRecvWinFirstTime;				// 接收窗口中第一包的收包时刻
	DWORD	m_dwRecvSeq; 						// 接收序号
	DWORD	m_RecvWindow[MAX_COMMWINSIZE];		// 接收窗口
	BYTE	m_byRecvCount;						// 接收窗口中已收包数量

	CCommAddr	m_objClientAddr; 				// 连入本服务端的客户端地址
	bool	m_bIsAppClient;						// 连入本服务端的客户端是否为业务程序客户端
	char	m_szPairNetIp[IP_LEN];				// 连入本服务端的客户端的双网配对ip，可以为空，m_nType = AF_INET时使用
}; 
   
#endif 
