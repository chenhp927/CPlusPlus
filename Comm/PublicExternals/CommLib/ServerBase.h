//==============================================================================
//                      ServerBase.h
//
//begin   : 2016-05-13
//describe:
//==============================================================================
#ifndef SELFDEF_SERVER_BASE_H 
#define SELFDEF_SERVER_BASE_H 
 
#include <vector>

#include "TypeDef.h"
#include "SockMacro.h" 
#include "Buffer.h" 
#include "Queue.h" 
#include "Fdset.h" 
#include "Sid.h"
#include "ObjectBase.h"
#include "DataStreamStat.h"

using namespace std;

//==============================================================================
//                 
//                    CServerBase
//                    服务端基类
//
//==============================================================================
class CServerBaseCltObj;
class MY_EXT_CLASS CServerBase : public CObjectBase
{ 
public: 
	CServerBase(); 
	virtual ~CServerBase(); 

	// 该函数在该类构造后要马上手动调用一次，可以看做构造函数的一部分，目的是
	// 为了在构造过程中执行虚函数CreateObject（因为构造函数中要避免调用虚函数）
	// 该函数在调用成功后可以调用多次，都返回成功
	bool Create();

protected:  
	/* 下列的实现为服务端网络连接启动、关闭的相关处理 */
	virtual bool OnStart(); 
	virtual bool MakeInetSocket();  
	virtual bool MakeUnixSocket();

	/* 下列的实现为接入客户端连接建立、关闭的相关处理 */
	virtual void Accept(); 
	virtual void OnInetAccept(int nSockfd, struct sockaddr_in& stuSockAddr); 
	virtual void OnUnixAccept(int nSockfd); 
	virtual void DenyClient(int nSockfd, struct sockaddr_in* stuSockAddr, int nReason);
	virtual void OnFdExcept();  
	virtual void OnSessionConnect(CSid objSid); 
	virtual void OnSessionClose(CSid objSid); 
	virtual bool IsClientIpValid(struct sockaddr_in& stuSockAddr);  
	virtual bool IsTooMuchSameClient(struct sockaddr_in& stuSockAddr);  

	/* 下列的实现为接收数据的相关处理 */
	virtual int  Read(CServerBaseCltObj* pCltObj); 
	virtual void OnRead(CServerBaseCltObj* pCltObj); 
	virtual void OnReadCompleted(CServerBaseCltObj* pCltObj); 
	virtual void OnReadClientClose(CServerBaseCltObj* pCltObj); 
	virtual void OnReadError(CServerBaseCltObj* pCltObj); 
	void ClearRecvQueue();
	
	// 根据协议计算数据包的长度，以便于本客户端基类根据接收的数据进行数据包截
	// 取，若根据已接收的数据还无法计算数据包长度时，返回-1，等再次接收部分数
	// 据后再进行计算。
	// 派生类必须实现本函数。
	virtual int GetRecvPacketSize(CBuffer* pBuf) = 0;
	
	// 处理接收到的完整数据包。将数据包直接放入接收队列。
	virtual void ProcessStreamPacket(CServerBaseCltObj* pCltObj, CBuffer* pBuf);
	
	// 将接收到的数据包放入接收队列。
	void AddRecvPacketToQueue(CServerBaseCltObj* pCltObj, CBuffer* pBuf);

	/* 下列的实现为发送数据的相关处理 */
	virtual bool Write(CServerBaseCltObj* pCltObj); 
	virtual void OnWrite(CServerBaseCltObj* pCltObj, CBuffer* pBuf) ; 
	virtual void OnWriteCompleted(CServerBaseCltObj* pCltObj, CBuffer* pBuf); 
	virtual void OnWritePart(CServerBaseCltObj* pCltObj, CBuffer* pBuf, int nSend); 
	virtual void OnWriteError(CServerBaseCltObj* pCltObj, CBuffer* pBuf); 

	/* 下列的实现为巡检的相关处理 */
	virtual void OnPatrol();  
	virtual void PatrolClientObject(CServerBaseCltObj* pCltObj);

	/* 下列的实现为接入客户端对象的相关处理 */
	bool AllocateClientPool();
	virtual CServerBaseCltObj* AssignClientObject(); 
	virtual CServerBaseCltObj* CreateClientObject(); 
	void UpdateMaxClientBoundOnConnect(CServerBaseCltObj* pCltObj);
	void UpdateMaxClientBoundOnClose(CServerBaseCltObj* pCltObj);

public: 
	/* 下列的接口为网络参数初始化的相关处理 */
	// 根据配置文件进行基本参数的初始化，本基类未实现，需要派生类根据情况实现
	virtual bool Init(const char* szConfigFile);

	/* 下列的接口为基本属性设置和获取的相关处理 */
	void SetType(int nType);
	int GetType();
	bool SetUnixPath(const char* szPath);
	const char* GetUnixPath();
	bool SetSvrIp(const char* szSvrIp);
	string GetSvrIp();
	bool SetSvrPort(unsigned short sPort);
	int GetSvrPort();
	bool SetMaxClients(int nMax);
	int GetMaxClients();
	int GetMaxClientBound();
	void SetMaxPacketSize(int nSize);
	int GetMaxPacketSize();
	void SetSidHigh(DWORD dw);
	DWORD GetSidHigh();

	// 本服务端是否没有配置
	bool IsNotCfged();

	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	virtual bool Startup(unsigned short nPort, const char* szIP = "0.0.0.0");  
	virtual bool Startup(const char* szPath); 
	virtual bool Startup();

	// 关闭监听
	virtual void CloseSvr();

	/* 下列的接口为通过select机制获取网络消息的相关处理 */
	void SetFdset(CFdset& objFs); 
	virtual void HandleNetEvent(CFdset& objFs);  
	void SetListenFdset(CFdset& objFs); 
	virtual void HandleListenNetEvent(CFdset& objFsfs);  
	void SetClientFdset(CFdset& objFs); 
	virtual void HandleClientNetEvent(CFdset& objFs);  
	void SetClientFdset(CFdset& objFs, vector<CServerBaseCltObj*>& vecCltObj); 
	virtual void HandleClientNetEvent(CFdset& objFs, vector<CServerBaseCltObj*>& vecCltObj);  

	/* 下列的接口为获取接收数据包的相关处理 */
	bool IsHasData(); 
	
	// 提取接收到的数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf均为传入传出参数；
	// 下列接口用到的szCltIP、nCltPort、objSid为传出参数。
	virtual bool RecvData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort);
	virtual bool RecvData(CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort); 
 	virtual bool RecvData(char* szBuf, int& nBufLen, CSid& objSid); 
	virtual bool RecvData(CBuffer* pBuf, CSid& objSid); 

	/* 下列的接口为发送数据包的相关处理。*/
	// 发送数据包。
	// 传入的buffer（数据包）须是完整包，传入的数据包将会直接放入发送队列，
	// 在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、szCltIP、nCltPort、objSid、pCltObj均为传入参数。
	virtual bool SendData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendData(char* szBuf, int nBufLen, CSid objSid);
	virtual bool SendData(CBuffer* pBuf, CSid objSid);
	virtual bool SendData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);

	/* 下列的接口为巡检的相关处理 */
	// 周期性调用，负责网络巡检重连、将发送队列中的数据包发送出去
	virtual void DoPatrol(); 
	virtual void DoPatrol(vector<CServerBaseCltObj*>& vecCltObj);  

	/* 下列的接口为接入客户端对象信息获取、管理的相关处理 */
	CServerBaseCltObj* GetClientObjByIndex(int nIndex);
	CServerBaseCltObj* GetClientObj(const char* szCltIP,unsigned short nCltPort); 
	CServerBaseCltObj* GetClientObj(CSid& objSid); 
	void CloseClientObj(CSid objSid);
	void CloseClientObj(const char* szCltIP, unsigned short nCltPort); 
	void CloseClientObj(CServerBaseCltObj* pCltObj); 
	std::string GetClientObjIp(CSid objSid);
	bool IsMyClientObj(CServerBaseCltObj* pCltObj);

	/* 下列的接口为日志输出的相关处理 */
	// 在日志中打印基本参数
	virtual string GetDumpParamStr();
	virtual void DumpParam();
	virtual void DumpAllocator();
	
	// 获取格式化的服务端标识信息，主要用于日志输出
	virtual string GetTagMyInfo();
	
protected: 
	int		m_nSockfd; 					// socket文件描述符
	int		m_nType;					// 通信使用的的协议，AF_INET表示IPV4，AF_UNIX表示本地通信 
	char 	m_szUnixPath[UNIX_PATH_LEN];// 服务端监听的地址，m_nType = AF_UNIX时使用
	char	m_szSvrIp[IP_LEN]; 			// 服务端监听的ip，m_nType = AF_INET时使用 
	unsigned short	m_sSvrPort; 				// 服务端监听的端口，m_nType = AF_INET时使用

	DWORD	m_dwSidHigh;				// 用于设置每个会话连接的高位信息
	CServerBaseCltObj **m_ClientsPtr; 	// 连入本服务端的所有客户端信息
	int		m_nMaxClients;  			// 连入本服务端的最大有客户端数量
	int		m_nCurMaxClientBound; 		// 当前最大连接所在的索引+1
   
  	int		m_nMaxPacketSize;			// 数据包最大长度

	// 接收队列中数据包CBuffer的m_nWParam、m_nLParam属性有特殊应用。
	// 从各个客户端对象接收的数据包都将放入本队列，所以需要标记数据包的来源。
	// m_nWParam、m_nLParam用于记录本数据包来源的CSid
 	CQueue<CBuffer *>	m_RecvQueue;  	// 数据包接收队列

}; 

//==============================================================================
//                 
//                    CServerBaseCltObj
//              服务端基类的客户端对象类
//
//==============================================================================
class MY_EXT_CLASS CServerBaseCltObj : public CObjectBase
{ 
public: 
	CServerBaseCltObj(CServerBase* pServer); 
	virtual ~CServerBaseCltObj(); 
 
protected: 
	/* 下列的实现为客户端网络连接启动、关闭、判定的相关处理 */
	virtual void OnConnect(); 
	virtual void OnClose(); 
 
	/* 下列的实现为数据包发送队列操作的相关处理 */
	void ClearSendQueue(); 

	/* 下列的实现为客户端对象清除的相关处理 */
	virtual void ClearCltInfo();

public: 
	/* 下列的接口为重载基类接口 */
	// 设置日志文件的标识
	virtual void SetLogFileTag(const char* szFileTag);

	/* 下列的接口为基本属性设置和获取的相关处理 */
	CSid GetSid();
	int GetSocket();
	bool IsSocketValid();
	void SetPeerName(const char* szPeerName);
	string GetPeerName();
	void SetPeerIP(const char* szPeerIp);
	string GetPeerIp();
	void SetPeerPort(unsigned short sPeerPort);
	int GetPeerPort();
	void SetIndex(int nIndex);
	int GetIndex();
	void SetConnectStatus(int nStatus); 
	int GetConnectStatus();
	void SetSidHigh(DWORD dwSidHigh);
	DWORD GetSidHigh();
 	void SetSendQueueDepth(int nSendQueueDepth);
	int GetSendQueueDepth();
	void  SetRecvTime(); 
	DWORD GetRecvTime();
	void  SetSendTime(); 
	DWORD GetSendTime(); 
	
	/* 下列的接口为客户端网络连接启动、关闭、判定的相关处理 */
	virtual void Connect(int nSockfd, const struct sockaddr_in& stuSockAddr);
	virtual void Connect(int nSockfd);
	virtual bool Close(); 
	bool IsConnect(); 
	
	/* 下列的接口为接收数据的相关处理 */
	// 获取接收数据的临时缓存区
	CBuffer* GetRecvTmpBuf();

	/* 下列的接口为数据包发送队列操作的相关处理 */
	// 下列接口传入的buffer（数据包）须是完整包
	bool PushSendData(CBuffer* pBuf);
	bool IsHasSendData();
	CBuffer* PopSendData();
	bool RollbackSendData(CBuffer* pBuf);
	int GetSendQueueSize();

	// 用户数据队列中最前面的包是否已经完整发送了
	bool IsSendQueueInPart();

	// 获取缓存区分配器
	CBuffer* Allocate();

	/* 下列的接口为获取所属服务器的相关处理 */
	CServerBase* GetRelatedServerPtr();
	
	/* 下列的接口为日志输出的相关处理 */
	// 获取格式化的客户端信息，主要用于日志输出
	virtual string GetTagCltInfo();
	virtual string GetTagConnInfo();
	virtual void DumpAllocator();

	/* 下列的接口为统计网络收发数据包数量的相关处理 */
	// 统计发送包数据
	void StreamStatSend(CBuffer* pBuf);
	void StreamStatRecv(CBuffer* pBuf);

protected:  
	CServerBase* m_pRelatedServerPtr;		// 关联的服务器对象指针，该值在初始化阶段就能固定下来
	int 	m_nCltIndex;					// 该clt对象在服务端数组中的索引，该值在初始化阶段就能固定下来
	DWORD	m_dwSidHigh;					// 连接的会话ID的高位数值，该值在初始化阶段就能固定下来

	CSid 	m_objSid;						// 连接的会话ID
	int 	m_nSockfd;						// socket文件描述符
	char	m_szPeerName[SESSION_NAME_LEN];	// 连入本服务端的客户端的名称
	char	m_szPeerIp[IP_LEN];				// 连入本服务端的客户端的ip
	unsigned short	m_sPeerPort; 			// 连入本服务端的客户端的端口

	int 	m_nConnectStatus; 				// 该客户端与本服务端当前连接状态

 	CBuffer::CAllocator m_objBufAlloc;		// 缓存区分配器
	CBuffer* m_pRecvTmpBuf; 				// 接收数据的临时缓存区

	// 发送队列中数据包CBuffer的m_nWParam、m_nFlag属性有特殊应用。
	// 在通过send()函数将数据包发送到网络上时，由于每次调用并不一定能够将整个数据包发送完整，所以需要进行标记。
	// m_nFlag用于标记本数据包是否为非完整数据包，0-完整数据包；1-非完整数据包
	// m_nWParam用于记录本数据包为完整数据包时的长度
	CQueue<CBuffer *> m_SendQueue; 			// 数据包发送队列
	int m_nSendQueueDepth;					// 发送队列的最大深度

	DWORD	m_dwSendTime; 					// 上次发包时刻
	DWORD	m_dwRecvTime; 					// 上次收包时刻

//	CDataStreamStatMgr	m_objStreamStatSend;// 数据包发送量统计
//	CDataStreamStatMgr	m_objStreamStatRecv;// 数据包接收量统计
}; 
   
#endif 
