//==============================================================================
//                      ClientSessionBase.cpp
//
//begin   : 2016-08-29
//describe:
//==============================================================================

#include "ClientSessionBase.h" 
#include "Utility.h"
#include "LocalDeviceConf.h"

//==============================================================================
//                 
//                    CClientSessionBase
//           会话层客户端基类（带协商、心跳、发送确认的机制）
//
//==============================================================================
CClientSessionBase::CClientSessionBase() : 
		m_nHeartInterval(DEF_HEARTINTERVAL), 
		m_nHeartTimeout(DEF_HEARTTIMEOUT), 
		m_nAckInterval(DEF_ACKINTERVAL), 
		m_nAckTimeout(DEF_ACKTIMEOUT), 
		m_byWindowSize(DEF_COMMWINSIZE),
		m_dwSendWinFullTime(0),
		m_dwSendSeq(INIT_SESSION_SEQ),  
		m_bySendCount(0),
		m_dwRecvWinFirstTime(0),
		m_dwRecvSeq(INIT_SESSION_SEQ),  
		m_byRecvCount(0)
{ 
	SetMyClassName("CClientSessionBase");
			
	memset(m_szPairNetIp, 0, sizeof(m_szPairNetIp));
	SetAppClient(true);
} 
 
CClientSessionBase::~CClientSessionBase() 
{ 
} 
 
DWORD CClientSessionBase::GetSendWindowFullTime() const
{
	return m_dwSendWinFullTime;
}

DWORD CClientSessionBase::GetRecvWindowFirstTime() const
{
	return m_dwRecvWinFirstTime;
}

bool CClientSessionBase::IsReady() 
{ 
	return GetConnectStatus() == CS_READY;	 
} 

void CClientSessionBase::OnConnect() 
{
	CClientBase::OnConnect();
		
	m_dwSendSeq = INIT_SESSION_SEQ; 
	m_dwRecvSeq = INIT_SESSION_SEQ; 
	m_bySendCount = 0;
	m_byRecvCount = 0;
	SetConnectStatus(CS_WAITNEGACK); 
	SendNegReq();
}

void CClientSessionBase::SendNegReq()
{
	CBuffer* pBuf = m_objBufAlloc.Allocate(); 
	assert(pBuf);

	BYTE byClientTag = CLIENTTAG_APP;	// 业务程序客户端
	if (!m_bIsAppClient)
	{
		byClientTag = CLIENTTAG_COMM;	// 通信程序客户端
	}
	DWORD dwPairNetIp = inet_addr(m_szPairNetIp);

	int len = CProtocolSession::MakeNegReqPacket(pBuf->GetBuffer(), pBuf->GetBufSize(), 
									m_dwSendSeq, m_objClientAddr, byClientTag, dwPairNetIp); 
	pBuf->Use(len);

	LogImportant("CClientSessionBase::SendNegociate(), \n connection(%s) \n make neg_req pkt ok!",
		GetTagConnInfo().c_str());
	LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());

	// 添加到发送队列	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	m_SpSendQueue.AddTail(pBuf);
}

void CClientSessionBase::OnClose() 
{
	ClearSpSendQueue();  
	 
	if (IsReady())
	{
		OnSessionClose();
	}
	
	CClientBase::OnClose();
} 

void CClientSessionBase::ClearSpSendQueue()  
{ 
	// 队列清空前输出日志
	int nLastPktCount = m_SpSendQueue.GetSize();
	if (nLastPktCount > 0)
	{
		if (IsConnect())
		{
			LogImportant("CClientBase::ClearSpSendQueue(), \n connection(%s) \n it is connecting, queue_size=%d, will throw packets ...", 
				GetTagConnInfo().c_str(), nLastPktCount);
		}
		else
		{
			LogImportant("CClientBase::ClearSpSendQueue(), \n connection(%s) \n it is disconnected, queue_size=%d, will throw packets ...", 
				GetTagConnInfo().c_str(), nLastPktCount);
		}
	}

	// 执行队列清空操作
	CBuffer* pBuf = NULL;  
	while (!m_SpSendQueue.IsEmpty()) 
	{ 
		if (m_SpSendQueue.GetHead(pBuf)) 
		{
// 			if (IsConnect())
// 			{
// 				LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
// 			}
			pBuf->Release(); 
		}
		else 
			break; 
	} 

	// 队列清空后输出日志
	int nThrowPktCount = nLastPktCount - m_SpSendQueue.GetSize();
	if (nThrowPktCount > 0 || m_SpSendQueue.GetSize() > 0)
	{
		LogImportant("CClientBase::ClearSpSendQueue(), \n connection(%s) \n succeed to throw %d packets, now queue_size=%d.", 
			GetTagConnInfo().c_str(), nThrowPktCount, m_SpSendQueue.GetSize());
	}
} 
 
void  CClientSessionBase::OnSessionReady()
{
	//override it 
}

void CClientSessionBase::OnSessionClose()
{
	//override it 
}

int CClientSessionBase::GetRecvPacketSize(CBuffer* pBuf)
{
	int nPacketSize = CProtocolSession::StreamPacketSize(m_pRecvTmpBuf->GetBuffer()); 
	return nPacketSize;
}

void CClientSessionBase::ProcessStreamPacket(CBuffer* pBuf) 
{ 
	assert(pBuf);
	LogDebug("CClientSessionBase::ProcessStreamPacket(), \n connection(%s) \n recv a packet len=%d", 
		GetTagConnInfo().c_str(), pBuf->GetUsed());
 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed()); 
	
	if (!CProtocolSession::IsValid(pBuf->GetBuffer(), pBuf->GetUsed()))
	{
		LogImportant("CClientSessionBase::ProcessStreamPacket(), \n connection(%s) \n recv a packet which is invalid.",
			GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		pBuf->Release();
	} 
	else if (!AuthLevel(pBuf))
	{
		LogImportant("CClientSessionBase::ProcessStreamPacket(), \n connection(%s) \n AuthLevel() fail, so will close socket ...",
			GetTagConnInfo().c_str());

		pBuf->Release();
		CloseToServer();
	}
	else if (CProtocolSession::IsHeart(pBuf->GetBuffer())) 
	{ 
		OnRecvHeart(pBuf); 
		pBuf->Release(); 
	} 
	else if (CProtocolSession::IsData(pBuf->GetBuffer())) 
	{ 
		OnRecvData(pBuf); 
	} 
	else if (CProtocolSession::IsDataAck(pBuf->GetBuffer())) 
	{ 
		OnRecvDataAck(pBuf); 
		pBuf->Release(); 
	} 
	else if (CProtocolSession::IsNegAck(pBuf->GetBuffer())) 
	{ 
		OnRecvNegAck(pBuf); 
		pBuf->Release(); 
	} 
	else 
	{ 
		OnRecvOtherPacket(pBuf); 
		pBuf->Release(); 
	} 
} 
 
bool CClientSessionBase::AuthLevel(CBuffer* pBuf)
{
	assert(pBuf);
	bool bRet = true;
	if (CProtocolSession::IsNegAck(pBuf->GetBuffer()))
	{
		if (GetConnectStatus() != CS_WAITNEGACK)
		{
			bRet = false;
			LogImportant("CClientSessionBase::AuthLevel(), \n connection(%s) \n recv a neg_ack pkt, but my level is %d, not CS_WAITNEGACK!", 
				GetTagConnInfo().c_str(), GetConnectStatus());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
	}
	else if (GetConnectStatus() == CS_WAITNEGACK && !CProtocolSession::IsHeart(pBuf->GetBuffer()))
	{
		bRet = false;
		LogImportant("CClientSessionBase::AuthLevel(), \n connection(%s) \n recv a pkt(not heart), it is not neg_ack, but my level is CS_WAITNEGACK!", 
			GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	return bRet;
}

void CClientSessionBase::OnRecvHeart(CBuffer* pBuf) 
{ 
	LogDebug("CClientSessionBase::OnRecvHeart(), \n connection(%s) \n recv heart pkt.", 
		GetTagConnInfo().c_str());
//	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
} 
 
void CClientSessionBase::OnRecvData(CBuffer* pBuf) 
{ 
	assert(pBuf);
	PushRecvSeq(CProtocolSession::GetPacketSeq(pBuf->GetBuffer()));
	if (IsNeedToAck())
	{
		AckData();
	}

	if (pBuf->GetUsed() <= CProtocolSession::HeaderSize())
	{
		LogImportant("CClientSessionBase::OnRecvData(), \n connection(%s) \n recv a packet(len=%d), but no data.", 
			GetTagConnInfo().c_str(), pBuf->GetUsed());

		pBuf->Release();
		return;
	}

	LogDebug("CClientSessionBase::OnRecvData(), \n connection(%s) \n recv a packet(len=%d).", 
		GetTagConnInfo().c_str(), pBuf->GetUsed());
//	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed()); 
	
	HandleUserData(pBuf);
} 

void CClientSessionBase::HandleUserData( CBuffer* pBuf )
{
	AddRecvPacketToQueue(pBuf);
}

void CClientSessionBase::OnRecvDataAck(CBuffer* pBuf) 
{ 
	assert(pBuf);
	
	DWORD dwSeq = CProtocolSession::GetPacketSeq(pBuf->GetBuffer());

	LogDebug("CClientSessionBase::OnRecvDataAck(), \n connection(%s) \n recv data_ack pkt(seq=%d).", 
		GetTagConnInfo().c_str(), dwSeq);
//	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());

	if (m_bySendCount > 0)
	{
		bool bGetIt = false;
		int i = 0;
		for (i = m_bySendCount - 1; i >= 0; i--)
		{
			if (m_SendWindow[i] == dwSeq)
			{
				bGetIt = true;
				break;
			}
		}
		if (bGetIt)
		{
			m_bySendCount = m_bySendCount - i - 1;
			if (m_bySendCount > 0)
			{
				memmove(m_SendWindow, &m_SendWindow[i+1], m_bySendCount * sizeof(DWORD));
			}
		}
		else
		{
			LogImportant("CClientSessionBase::OnDataAck(), \n connection(%s) \n recv data_ack pkt(seq=%d), but I can't find it from my window!", 
				GetTagConnInfo().c_str(), dwSeq);
		}
	}
	else
	{
		LogImportant("CClientSessionBase::OnDataAck(), \n connection(%s) \n recv data_ack pkt(seq=%d), but my send window is space!", 
			GetTagConnInfo().c_str(), dwSeq);
	}
} 
 
void CClientSessionBase::OnRecvNegAck(CBuffer* pBuf) 
{ 
	assert(pBuf);

	LogImportant("CClientSessionBase::OnRecvNegAck(), \n connection(%s) \n recv neg_ack pkt.",
		GetTagConnInfo().c_str());
	LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());

	m_dwRecvSeq = CProtocolSession::GetPacketSeq(pBuf->GetBuffer()); 
	m_byWindowSize = CProtocolSession::GetWinSizeFromNegAck(pBuf->GetBuffer()); 
	m_nHeartInterval = CProtocolSession::GetHeartIntervalFromNegAck(pBuf->GetBuffer()); 
	m_nHeartTimeout = CProtocolSession::GetHeartTimeoutFromNegAck(pBuf->GetBuffer()); 
	m_nAckInterval = CProtocolSession::GetAckIntervalFromNegAck(pBuf->GetBuffer()); 
	m_nAckTimeout = CProtocolSession::GetAckTimeoutFromNegAck(pBuf->GetBuffer()); 

	LogImportant("CClientSessionBase::OnRecvNegAck(), \n connection(%s) \n recv neg_ack!\n"
		" next_seq=%d\n window_size=%d\n heart_interval=%d\n heart_timeout=%d\n ack_interval=%d\n ack_timeout=%d\n",
			GetTagConnInfo().c_str(), m_dwRecvSeq, m_byWindowSize, m_nHeartInterval, m_nHeartTimeout, m_nAckInterval, m_nAckTimeout);
	if (!IsReady())
	{
		LogImportant("CClientSessionBase::OnRecvNegAck(), \n connection(%s) \n set connection ready!",
			GetTagConnInfo().c_str());

		SetConnectStatus(CS_READY); 
		OnSessionReady();
	}
} 
 
void CClientSessionBase::OnRecvOtherPacket(CBuffer* pBuf) 
{
	LogImportant("CClientSessionBase::OnRecvOtherPacket(), \n connection(%s) \n recv a pkt, but I don't know it!",
		GetTagConnInfo().c_str());
	LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
}

void CClientSessionBase::PushRecvSeq(DWORD dwSeq)
{
	if (m_byWindowSize > 0 && m_byRecvCount < MAX_COMMWINSIZE)
	{
		m_RecvWindow[m_byRecvCount] = dwSeq;
		m_byRecvCount++;
		if (m_byRecvCount == 1)
		{
			m_dwRecvWinFirstTime = CUtility::GetUptime();
		}
	}
	if ( (m_dwRecvSeq + 1) == dwSeq)
	{
		m_dwRecvSeq++;
	}
	else
	{
		LogImportant("CClientSessionBase::PushRecvSeq(), \n connection(%s) \n recv seq(%d) is not continuous, last_seq=%d. so update it!", 
			GetTagConnInfo().c_str(), dwSeq, m_dwRecvSeq);
		m_dwRecvSeq = dwSeq;
	}
}
 
DWORD CClientSessionBase::GetLastRecvSeq() const
{
	if (m_byRecvCount > 0)
	{
		return m_RecvWindow[m_byRecvCount-1];
	}
	else
	{
		return 0;
	}
}

bool CClientSessionBase::IsSendWindowFull() const
{
	return (m_byWindowSize != 0 && m_bySendCount >= m_byWindowSize);
}

bool CClientSessionBase::IsNeedToAck() const 
{ 
	if (m_byWindowSize == 0)
	{
		return false;
	}
	else if (m_byRecvCount >= m_byWindowSize)
	{
		return true;
	}
	else if (m_byRecvCount > 0)
	{
		DWORD dwSpanFirst = CUtility::GetElapseTime(GetRecvWindowFirstTime());
		if (dwSpanFirst >= m_nAckInterval)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void CClientSessionBase::AckData() 
{ 
	DWORD dwSeq = GetLastRecvSeq(); 
	CBuffer* pBufAck = m_objBufAlloc.Allocate(); 
	assert(pBufAck);

	int nLen = CProtocolSession::MakeDataAckPacket(pBufAck->GetBuffer(), pBufAck->GetBufSize(), dwSeq); 
	pBufAck->Use(nLen);

	LogImportant("CClientSessionBase::AckData(), \n connection(%s) \n make data_ack pkt(seq=%d) ok!", 
		GetTagConnInfo().c_str(), dwSeq);
// 	LogDataDebug(pBufAck->GetBuffer(), pBufAck->GetUsed());
	
	// 添加到发送队列	
	pBufAck->SetWParam(pBufAck->GetUsed());
	pBufAck->SetFlag(0);
	m_SpSendQueue.AddTail(pBufAck);

	m_byRecvCount = 0;
} 

bool CClientSessionBase::RecvSessionData(char* szBuf, int& nBufLen) 
{ 
	bool bRet = false;

	CBuffer* pBufInQueue = NULL; 
	if (m_RecvQueue.PeekHead(pBufInQueue)) 
	{
		int nDataLen = pBufInQueue->GetUsed() - CProtocolSession::HeaderSize();
		if (nDataLen > nBufLen) 
		{ 
			nBufLen = nDataLen;
		}
		else if (m_RecvQueue.GetHead(pBufInQueue)) 
		{
			nDataLen = pBufInQueue->GetUsed() - CProtocolSession::HeaderSize();
			if (nDataLen > 0)
			{
				char* pData = pBufInQueue->GetBuffer() + CProtocolSession::HeaderSize();
				memcpy(szBuf, pData, nDataLen); 
				nBufLen = nDataLen; 
				bRet = true;  
			}
			
			pBufInQueue->Release();
		} 
	} 
	
	return bRet; 
} 

bool CClientSessionBase::RecvSessionData(CBuffer* pBuf) 
{  
	pBuf->Empty();
	
	bool bRet = false;
	CBuffer* pBufInQueue = NULL; 
	if (m_RecvQueue.GetHead(pBufInQueue)) 
	{
		int nDataLen = pBufInQueue->GetUsed() - CProtocolSession::HeaderSize();
		if (nDataLen > 0)
		{
			char* pData = pBufInQueue->GetBuffer() + CProtocolSession::HeaderSize();
			pBuf->AddData(pData, nDataLen);
			bRet = true;
		}

		pBufInQueue->Release();
	}
	
	return bRet; 
} 

bool CClientSessionBase::SendSessionData(char* szBuf, int nBufLen) 
{  
	if (!IsConnect()) 
	{ 
		LogImportant("CClientSessionBase::SendSessionData(), \n connection(%s) \n it is disconnect, can not send data!",
			GetTagConnInfo().c_str()); 
		return false; 
	} 
	
	if (nBufLen <= 0)
	{
		LogImportant("CClientSessionBase::SendSessionData(), \n connection(%s) \n send data fail! buf_len(%d) is error!", 
			GetTagConnInfo().c_str(), nBufLen); 
		return false; 
	}
	
	int nDataLen = nBufLen;
	int nNeedLen = CProtocolSession::HeaderSize() + nDataLen;
	if (nNeedLen > GetMaxPacketSize())
	{ 
		LogImportant("CClientSessionBase::SendSessionData(), \n connection(%s) \n send data fail! pkt need len(%d) is larger than max(%d)!",
			GetTagConnInfo().c_str(), nNeedLen, GetMaxPacketSize()); 
		return false; 
	} 

	// 先分配足够内存
	CBuffer* pBuf = m_objBufAlloc.Allocate(); 
	if (!pBuf)
	{
		return false;
	}
	if (pBuf->GetAvailableSize() < nNeedLen)
	{
		pBuf->Extend(nNeedLen);
	}
	
	// 会话层协议头打包，发送序号暂时设置为0，在发送的时候重新设置
	CProtocolSession::CSessionHead objHead(nDataLen, SPT_DATA, 0); 
	if (objHead.ToStreamBuf(pBuf->GetBuffer()) <= 0)
	{
		pBuf->Release();
		return false;
	}
	pBuf->Use(CProtocolSession::HeaderSize());	

	// 再添加数据
	pBuf->AddData(szBuf, nDataLen); 

	// 添加到发送队列	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	m_SendQueue.AddTail(pBuf);

	if (m_SendQueue.GetSize() > 64)
	{
		LogDebug("CClientSessionBase::SendSessionData(), \n connection(%s) \n m_SendQueue.GetSize()=%d is larger than 64.", 
			GetTagConnInfo().c_str(), m_SendQueue.GetSize());
	}
	
	return true; 
} 
 
bool CClientSessionBase::SendSessionData(CBuffer* pBuf) 
{ 
	return CClientSessionBase::SendSessionData(pBuf->GetBuffer(), pBuf->GetUsed()); 
} 

void CClientSessionBase::DoPatrol() 
{ 
	if (IsNotCfged()) // 没有配置，比如双网做单网使用，后面的操作就都不用了
	{
		return;
	}
	
	if (m_SendQueue.GetSize() > GetSendQueueDepth())
	{
		LogImportant("CClientSessionBase::DoPatrol(), \n connection(%s) \n m_SendQueue.GetSize()=%d is larger than max(%d), so will close socket ...", 
			GetTagConnInfo().c_str(), m_SendQueue.GetSize(), GetSendQueueDepth());
		CloseToServer();
	}

	if (m_nSockfd == INVALID_SOCKET) 
	{ 
		DWORD dwSpan = CUtility::GetElapseTime(m_dwRetryTime);
		if (dwSpan >= m_nRetryInterval) 
		{ 
			if (m_nType == AF_INET)
			{
				ConnectToInetServer(); 
			}
			else if (m_nType == AF_UNIX)
			{
				ConnectToUnixServer();
			}
			m_dwRetryTime = CUtility::GetUptime(); 
		} 
	} 
	else
	{ 
		DWORD dwSpanRecv = CUtility::GetElapseTime(m_dwRecvTime);
		// 1.如果tcp正常连接时心跳超时，且超过了正常心跳超时时间，则进入心跳超时的处理
		// 2.如果tcp处于正在连接的状态，且超过了连接状态的超时时间，则也进入心跳超时的处理
		if (dwSpanRecv >= m_nHeartTimeout
			|| (GetConnectStatus()==CS_CONNECTING && dwSpanRecv>=DEF_CONNECTINGTIMEOUT)) 
		{
			HeartDead();
		}
		
		DWORD dwSpanSend = CUtility::GetElapseTime(m_dwSendTime);
		if (IsConnect() && dwSpanSend >= m_nHeartInterval) 
		{
			SendHeart(); 
		}
		
		if (IsConnect() && IsSendWindowFull())
		{
			DWORD dwSpanFull = CUtility::GetElapseTime(GetSendWindowFullTime());
			if (dwSpanFull >= m_nAckTimeout)
			{
				AckFail();
			}
		}

		if (IsConnect() && IsNeedToAck())
		{
			AckData();
		}

		// 如果用户数据队列中最前面的包发送不完整,那么先发送用户数据队列第一包,试n次
		for (int x = 0; x < 1 && IsSendQueueInPart(); x++)
		{
			Write();
		}
		
		// 用户数据队列中最前面的包已经完整发送了,则发送协议发送队列,直到发送干净，才允许发送用户数据队列
		if (!IsSendQueueInPart())
		{
			while (!m_SpSendQueue.IsEmpty())
			{
				if (!WriteSp())
				{
					break;
				}
			}
	
			//协议队列空了，才允许发送用户数据队列
			if (m_SpSendQueue.IsEmpty())
			{
				while (!m_SendQueue.IsEmpty())
				{
					if (!Write())
					{
						break;
					}
				}
			}
		}
		
		OnPatrol();
	}
} 

void CClientSessionBase::AckFail()
{
	LogImportant("CClientSessionBase::AckFail(), \n connection(%s) \n ack fail! last_seq=%d, so will close socket ...", 
		GetTagConnInfo().c_str(), m_dwSendSeq);
	CloseToServer();	
}

void CClientSessionBase::SendHeart() 
{ 
	CBuffer* pBuf = m_objBufAlloc.Allocate(); 
	assert(pBuf);
	int len = CProtocolSession::MakeHeartPacket(pBuf->GetBuffer(), pBuf->GetBufSize());  
	pBuf->Use(len);

	LogImportant("CClientSessionBase::SendHeart(), \n connection(%s) \n make heart pkt ok!",
		GetTagConnInfo().c_str());
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	// 添加到发送队列	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	m_SpSendQueue.AddTail(pBuf);
} 
 
void CClientSessionBase::HeartDead() 
{ 
	LogImportant("CClientSessionBase::HeartDead(), \n connection(%s) \n heart_dead! so will close socket ...", 
		GetTagConnInfo().c_str());
	CloseToServer(); 
} 

bool CClientSessionBase::Write() 
{ 
	if (!SOCKET_VALID(m_nSockfd))
	{
		return false;
	}
	
	if (!IsReady())
	{
		LogImportant("CClientSessionBase::Write(), \n connection(%s) \n write fail because the connection is not ready.",
			GetTagConnInfo().c_str());
		return false;
	}
	
	if (!IsSendQueueInPart() && m_byWindowSize > 0 && m_bySendCount >= m_byWindowSize)
	{
		LogImportant("CClientSessionBase::Write(), \n connection(%s) \n write fail because the send_window is full(size=%d).",
				GetTagConnInfo().c_str(), m_byWindowSize);
		return false;
	}
 
	CBuffer* pBuf = NULL; 
	if (m_SendQueue.IsEmpty() || !m_SendQueue.GetHead(pBuf)) 
	{
		return false; 
	}
	assert(pBuf);
	
	if (pBuf->GetFlag() == 0)
	{
		m_dwSendSeq++;
		CProtocolSession::SetPacketSeq(pBuf->GetBuffer(), m_dwSendSeq); 
		PushSendSeq();
	}

	if (pBuf->GetUsed() <= 0)
	{
		LogImportant("CClientSessionBase::Write(), \n connection(%s) \n write fail because send buf len=0.",
			GetTagConnInfo().c_str());
		return false;
	}

	OnWrite(pBuf);  
  
	bool bRet = true; 
	int	nSend = send(m_nSockfd, pBuf->GetBuffer(), pBuf->GetUsed(), 0);  

	if (nSend == pBuf->GetUsed())  
	{  
		OnWriteCompleted(pBuf);  
	}  
	else if (nSend > 0)
	{
		OnWritePart(pBuf, nSend);
		bRet = false; 
	}
	else 
	{
		OnWriteError(pBuf);  
		bRet = false; 
	}
	
	return bRet;  
}  

void CClientSessionBase::PushSendSeq()
{
	if (m_byWindowSize > 0 && m_bySendCount < MAX_COMMWINSIZE)
	{
		m_SendWindow[m_bySendCount] = m_dwSendSeq;
		m_bySendCount++;
		if (m_bySendCount >= m_byWindowSize)
		{
			m_dwSendWinFullTime = CUtility::GetUptime();
		}
	}
}

bool CClientSessionBase::WriteSp() 
{ 
	if (!SOCKET_VALID(m_nSockfd))
	{
		return false; 
	}
 
	CBuffer* pBuf; 
	if (m_SpSendQueue.IsEmpty() || !m_SpSendQueue.GetHead(pBuf)) 
	{
		return false; 
	}
	assert(pBuf);

	if (!IsConnect()) 
	{
		LogImportant("CClientSessionBase::WriteSp(), \n connection(%s) \n write sp fail because the connection is not ready.",
			GetTagConnInfo().c_str());
		return false; 
	} 

	if (pBuf->GetUsed() <= 0)
	{
		LogImportant("CClientSessionBase::WriteSp(), \n connection(%s) \n write sp fail because send buf len=0.",
			GetTagConnInfo().c_str());
		return false;
	}

	OnWriteSp(pBuf);  
  
	bool bRet = true; 
	int	nSend = send(m_nSockfd, pBuf->GetBuffer(), pBuf->GetUsed(), 0);  

	if (nSend == pBuf->GetUsed())  
	{  
		OnWriteCompletedSp(pBuf);  
	}  
	else if (nSend > 0)
	{
		OnWritePartSp(pBuf, nSend);
		bRet = false; 
	}
	else 
	{
		OnWriteErrorSp(pBuf);  
		bRet = false; 
	}
	return bRet;  
}  
 
void CClientSessionBase::OnWriteSp(CBuffer* pBuf) 
{ 
	m_dwSendTime = CUtility::GetUptime(); 

// 	LogDebug("CClientSessionBase::OnWriteSp(), \n connection(%s) \n write sp data, len=%d", 
// 		GetTagConnInfo().c_str(), pBuf->GetUsed());
//	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
} 
 
void CClientSessionBase::OnWriteCompletedSp(CBuffer* pBuf) 
{ 
	LogDebug("CClientSessionBase::OnWriteCompletedSp(), \n connection(%s) \n write sp completed, total=%d, write=%d", 
		GetTagConnInfo().c_str(), pBuf->GetWParam(), pBuf->GetUsed());
	if (!pBuf->GetFlag()) // 只有包头部分打印日志，剩余部分打印日志无意义，而且有时候数量很多
	{
		LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	pBuf->Release();
} 
 
void CClientSessionBase::OnWritePartSp(CBuffer* pBuf, int nSend) 
{
	LogDebug("CClientSessionBase::OnWritePartSp(), \n connection(%s) \n write sp parted, total=%d, write=%d, left=%d", 
		GetTagConnInfo().c_str(), pBuf->GetWParam(), nSend, pBuf->GetUsed() - nSend);
	if (!pBuf->GetFlag()) // 只有包头部分打印日志，剩余部分打印日志无意义，而且有时候数量很多
	{
		LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	pBuf->Cut(0, nSend);
	if (pBuf->GetUsed() > 0)
	{
		pBuf->SetFlag(1);
		m_SpSendQueue.AddHead(pBuf);

		LogDebug("CClientSessionBase::OnWritePartSp(), \n connection(%s) \n roll back the left sp to send queue, total=%d, left=%d",
			GetTagConnInfo().c_str(), pBuf->GetWParam(), pBuf->GetUsed());
	}
	else
	{
		LogDebug("CClientSessionBase::OnWritePartSp(), \n connection(%s) \n left length=0.", 
			GetTagConnInfo().c_str());

		pBuf->Release();
	}
}

void CClientSessionBase::OnWriteErrorSp(CBuffer* pBuf) 
{
	int nErr = ERRNO;
	
	if (nErr == EAGAIN || nErr == EINTR)
	{
		m_SpSendQueue.AddHead(pBuf);

		LogImportant("CClientSessionBase::OnWriteErrorSp(), \n connection(%s) \n write sp error=%d (EAGAIN or EINTR), put data back to sp send queue.",
			GetTagConnInfo().c_str(), nErr);
	}
	else 
	{
		LogImportant("CClientSessionBase::OnWriteErrorSp(), \n connection(%s) \n write sp error=%d, so will close socket ...", 
			GetTagConnInfo().c_str(), nErr);
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		pBuf->Release(); 
		CloseToServer(); 
	}
} 

string CClientSessionBase::GetDumpParamStr()
{
	char szDump[1024];
	memset(szDump, 0, sizeof(szDump));
	
	snprintf(szDump, sizeof(szDump), "%s\n"
		"session_param (heart_interval=%d; heart_timeout=%d; ack_interval=%d; ack_timeout=%d; window_size=%d)\n"
		"client_addr   (BureauId=%d; UnitType=%d; UnitId=%d; DevType=%d; DevId=%d; CltId=%d)\n"
		"other_param   (pairnet_ip=%s; is_appclient=%d)\n",
		CClientBase::GetDumpParamStr().c_str(), 
		m_nHeartInterval, m_nHeartTimeout, m_nAckInterval, m_nAckTimeout, m_byWindowSize, 
		m_objClientAddr.GetBureauId(), m_objClientAddr.GetUnitType(), m_objClientAddr.GetUnitId(), 
		m_objClientAddr.GetDevType(), m_objClientAddr.GetDevId(), m_objClientAddr.GetCltId(),
		m_szPairNetIp, m_bIsAppClient);
	
	return szDump;
}

void CClientSessionBase::DumpParam()
{
	LogImportant("CClientSessionBase::DumpParam(),\n%s", GetDumpParamStr().c_str());
}

void CClientSessionBase::SetClientId(short sClientId)
{
	m_objClientAddr.SetCltId(sClientId);
} 

short CClientSessionBase::GetClientId()
{
	return m_objClientAddr.GetCltId();
} 

void CClientSessionBase::SetPairNetIp(const char* szIp)
{
	assert(szIp && strlen(szIp) < sizeof(m_szPairNetIp));
	strcpy(m_szPairNetIp, szIp);
}

string CClientSessionBase::GetPairNetIp()
{
	return m_szPairNetIp;
}

bool CClientSessionBase::LoadClientAddr()
{
	// 这里读取配置文件获取自身设备地址以给发送的数据填上源地址信息
	m_objClientAddr = CCommAddr(); // 清空
	
	CLocalDeviceConf objLocalDevConf;
	char szErrInfo[1024];
	if (!objLocalDevConf.Load(szErrInfo, sizeof(szErrInfo)))
	{
		LogImportant("CClientSessionBase::LoadClientAddr(), \n %s!", szErrInfo);
		return false;
	}
	
	m_objClientAddr.SetBureauId(objLocalDevConf.m_byBureauId);
	m_objClientAddr.SetUnitType(objLocalDevConf.m_byUnitType);
	m_objClientAddr.SetUnitId(objLocalDevConf.m_wUnitId);
	m_objClientAddr.SetDevType(objLocalDevConf.m_byDevType);
	m_objClientAddr.SetDevId(objLocalDevConf.m_wDevId);
	m_objClientAddr.SetCltId(0);
	
	return true;
}

CCommAddr& CClientSessionBase::GetClientAddr()
{
	return m_objClientAddr;
}

void CClientSessionBase::SetAppClient(bool bIsAppClient)
{
	m_bIsAppClient = bIsAppClient;
}

string CClientSessionBase::GetTagMyInfo()
{
	char temp[256];
	memset(temp, 0, sizeof(temp));
	
	if (m_nType == AF_INET)
	{
		if (strlen(m_szMyIp) > 0 && m_sMyPort > 0)
		{
			snprintf(temp, sizeof(temp), "%s:%d:%d", m_szMyIp, m_sMyPort, m_objClientAddr.GetCltId());
		}
		else
		{
			snprintf(temp, sizeof(temp), "R%s:%d:%d", m_szMyConnIp, m_sMyConnPort, m_objClientAddr.GetCltId());
		}
	}
	else if (m_nType == AF_UNIX)
	{
		snprintf(temp, sizeof(temp), "%d", m_objClientAddr.GetCltId());
	}
	
	return temp;
}

string CClientSessionBase::GetTagMyAddrInfo()
{
	char temp[256];
	memset(temp, 0, sizeof(temp));

	snprintf(temp, sizeof(temp), "%d:%d:%d:%d:%d:%d", 
		m_objClientAddr.GetBureauId(), m_objClientAddr.GetUnitType(), m_objClientAddr.GetUnitId(),
		m_objClientAddr.GetDevType(), m_objClientAddr.GetDevId(), m_objClientAddr.GetCltId());

	return temp;
}
