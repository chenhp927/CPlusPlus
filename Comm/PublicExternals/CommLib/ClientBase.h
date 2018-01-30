//==============================================================================
//                      ClientBase.h
//
//begin   : 2016-05-12
//describe:
//==============================================================================
#ifndef SELFDEF_CLIENT_BASE_H 
#define SELFDEF_CLIENT_BASE_H 

#include "SockMacro.h" 

#ifdef UNIX
	#include "CTime.h"
#endif

#include "ObjectBase.h"
#include "Buffer.h" 
#include "Queue.h" 
#include "Fdset.h" 
//#include "DataStreamStat.h"

//==============================================================================
//                 
//                    CClientBase
//                    客户端基类
//
//==============================================================================
class MY_EXT_CLASS CClientBase : public CObjectBase 
{ 
public: 
	CClientBase(); 
	virtual ~CClientBase(); 
 
protected: 
	/* 下列的实现为网络连接启动、关闭的相关处理 */
	virtual bool ConnectToUnixServer(); 
	virtual bool ConnectToInetServer(); 
	virtual void OnConnect(); 
	virtual void OnConnectResponse(); 
	virtual void CloseToServer(); 
	virtual void OnClose(); 

	/* 下列的实现为接收数据的相关处理 */
	virtual int	 Read(); 
	virtual void OnRead(); 
	virtual void OnReadCompleted(); 
	virtual void OnReadError(); 
	
	// 根据协议计算数据包的长度，以便于本客户端基类根据接收的数据进行数据包截
	// 取，若根据已接收的数据还无法计算数据包长度时，返回-1，等再次接收部分数
	// 据后再进行计算。
	// 派生类必须实现本函数。
	virtual int GetRecvPacketSize(CBuffer* pBuf) = 0;

	// 处理接收到的完整数据包。将数据包直接放入接收队列。
	virtual void ProcessStreamPacket(CBuffer* pBuf);

	// 将接收到的数据包放入接收队列。
	virtual void AddRecvPacketToQueue(CBuffer* pBuf);
	void ClearRecvQueue(); 

	/* 下列的实现为发送数据的相关处理 */
 	virtual bool Write(); 
	virtual void OnWrite(CBuffer* pBuf); 
	virtual void OnWriteCompleted(CBuffer* pBuf); 
	virtual void OnWritePart(CBuffer* pBuf, int nSend); 
	virtual void OnWriteError(CBuffer* pBuf); 

	// 用户数据队列中最前面的包是否已经完整发送了
	bool IsSendQueueInPart();
	void ClearSendQueue(); 
	
	/* 下列的实现为巡检的相关处理 */
	// 巡检事件的响应处理函数，派生类实现
	virtual void OnPatrol();

public: 
	/* 下列的接口为重载基类接口 */
	// 设置日志文件的标识
	virtual void SetLogFileTag(const char* szFileTag);

	/* 下列的接口为网络参数初始化的相关处理 */
	// 根据配置文件进行基本参数的初始化，本基类未实现，需要派生类根据情况实现
	virtual bool Init(const char* szConfigFile);
	
	/* 下列的接口为基本属性设置和获取的相关处理 */
	void SetType(int nType);
	int GetType();
	void SetUnixPath(const char* szPath);
	const char* GetUnixPath();
	void SetSvrIp(const char* szIp);
	string GetSvrIp();
	void SetSvrPort(unsigned short sPort);
	int GetSvrPort();
	void SetMyIp(const char* szIp);
	string GetMyIp();
	void SetMyPort(unsigned short sPort);
	int GetMyPort();
	void SetMyConnIp(const char* szIp);
	string GetMyConnIp();
	void SetMyConnPort(unsigned short sPort);
	int GetMyConnPort();
 	void SetConnectStatus(int nStatus);
	int  GetConnectStatus();
	void SetRetryInterval(int nSec); 
	void SetSendQueueDepth(int nDepth);
	int GetSendQueueDepth();
	void SetMaxPacketSize(int nSize);
	int GetMaxPacketSize();

	// 本客户端是否没有配置
	bool IsNotCfged();

	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	virtual bool Connect(); 
	virtual bool Connect(const char* szPath); 
	virtual bool Connect(const char* szSvrIP, unsigned short nPort); 
	virtual void Close(); 
	bool IsConnect(); 

	/* 下列的接口为获取发送、接收时间的相关处理 */
	// 获取发送时间
	const DWORD GetSendTime();
	// 获取接收时间
	const DWORD GetRecvTime();

	/* 下列的接口为通过select机制获取网络消息的相关处理 */
	void SetFdset(CFdset& objFs); 

	// 处理网络事件，并将收到的数据放入接收队列
	virtual void HandleNetEvent(CFdset& objFs); 

	/* 下列的接口为获取接收数据包的相关处理 */
	bool IsHasData(); 

	// 提取接收到的数据包。
	// 从网络上接收的每一个完整包（不包括用于协商、心跳、发送确认等传输机制的包）都会放入接收队列。
	// 下列接口返回的buffer（数据包）都是指从接收队列中取出的数据包。
	// 下列接口用到的szBuf、nBufLen、pBuf均为传入传出参数；
	virtual bool RecvData(char* szBuf, int& nBufLen); 
	virtual bool RecvData(CBuffer* pBuf); 
 
	/* 下列的接口为发送数据包的相关处理。*/
	// 发送数据包。
	// 传入的buffer（数据包）须是完整包，传入的数据包将会直接放入发送队列，
	// 在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf均为传入参数。
	virtual bool SendData(char* szBuf, int nBufLen); 
	virtual bool SendData(CBuffer* pBuf); 

	/* 下列的接口为巡检的相关处理 */
	// 周期性调用，负责网络巡检重连、将发送队列中的数据包发送出去
	virtual void DoPatrol(); 

	/* 下列的接口为日志输出的相关处理 */
	// 在日志中打印基本参数
	virtual string GetDumpParamStr();
	virtual void DumpParam();

	// 获取格式化的服务端标识信息，主要用于日志输出
	virtual string GetTagSvrInfo();

	// 获取格式化的客户端标识信息，主要用于日志输出
	virtual string GetTagMyInfo();
	virtual string GetTagConnInfo();
	virtual void DumpAllocator();
	
protected: 
	int 	m_nSockfd;					// socket文件描述符
	int		m_nType;					// 通信使用的的协议，AF_INET表示IPV4，AF_UNIX表示本地通信 
	char	m_szUnixPath[SOCK_PATH_LEN];// 要连接的服务端的地址，m_nType = AF_UNIX时使用
	char	m_szSvrIp[IP_LEN];			// 要连接的服务端的ip，m_nType = AF_INET时使用
	unsigned short	m_sSvrPort; 		// 要连接的服务端的端口，m_nType = AF_INET时使用
	char	m_szMyIp[IP_LEN];			// 本地绑定的ip，m_nType = AF_INET时使用
	unsigned short	m_sMyPort;			// 本地绑定的端口，m_nType = AF_INET时使用（0-使用随机端口）
	char	m_szMyConnIp[IP_LEN];		// 建立网络连接后的本地ip，m_nType = AF_INET时使用
	unsigned short	m_sMyConnPort;		// 建立网络连接后的本地端口，m_nType = AF_INET时使用（0-使用随机端口）

	int 	m_nConnectStatus; 			// 当前连接状态
 
	DWORD	m_dwRetryTime;				// 上次重连时刻
	int		m_nRetryInterval;			// 尝试重连间隔
 
 	CBuffer::CAllocator m_objBufAlloc; 	// 缓存区分配器
	CBuffer* m_pRecvTmpBuf; 			// 接收数据的临时缓存区

	CQueue<CBuffer* > m_RecvQueue;		// 数据包接收队列

	// 发送队列中数据包CBuffer的m_nWParam、m_nFlag属性有特殊应用。
	// 在通过send()函数将数据包发送到网络上时，由于每次调用并不一定能够将整个数据包发送完整，所以需要进行标记。
	// m_nFlag用于标记本数据包是否为非完整数据包，0-完整数据包；1-非完整数据包
	// m_nWParam用于记录本数据包为完整数据包时的长度
	CQueue<CBuffer* > m_SendQueue;		// 数据包发送队列

	int m_nSendQueueDepth;				// 发送队列的最大深度

 	DWORD	m_dwSendTime; 				// 上次发包时刻
	DWORD	m_dwRecvTime; 				// 上次收包时刻
  
  	int		m_nMaxPacketSize;			// 数据包最大长度

//	CDataStreamStatMgr	m_objStreamStatSend;// 数据包发送量统计
//	CDataStreamStatMgr	m_objStreamStatRecv;// 数据包接收量统计
}; 
 
#endif 
