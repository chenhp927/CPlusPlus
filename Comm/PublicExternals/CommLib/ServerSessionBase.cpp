//==============================================================================
//                      ServerSessionBase.cpp
//
//begin   : 2016-09-08
//describe:
//==============================================================================
 
#include "ServerSessionBase.h"
#include "Utility.h"
#include "LocalDeviceConf.h"

//==============================================================================
//                 
//                    CServerSessionBase
//           会话层服务端基类（带协商、心跳、发送确认的机制）
//
//==============================================================================
CServerSessionBase::CServerSessionBase() :  
		m_nHeartInterval(DEF_HEARTINTERVAL), 
		m_nHeartTimeout(DEF_HEARTTIMEOUT),
		m_nAckInterval(DEF_ACKINTERVAL),
		m_nAckTimeout(DEF_ACKTIMEOUT),
		m_byWindowSize(DEF_COMMWINSIZE)
{ 
	SetMyClassName("CServerSessionBase");

	SetAppServer(true);
} 

CServerSessionBase::~CServerSessionBase() 
{  
}

string CServerSessionBase::GetDumpParamStr()
{
	char szDump[1024];
	memset(szDump, 0, sizeof(szDump));
	
	snprintf(szDump, sizeof(szDump), "%s\nsession_param (heart_interval=%d; heart_timeout=%d; ack_interval=%d; ack_timeout=%d; window_size=%d)\n", 
		CServerBase::GetDumpParamStr().c_str(), m_nHeartInterval, m_nHeartTimeout, m_nAckInterval, m_nAckTimeout, m_byWindowSize);
	
	return szDump;
}

void CServerSessionBase::DumpParam()
{
	LogImportant("CServerSessionBase::DumpParam(),\n%s", GetDumpParamStr().c_str());
}
 
void CServerSessionBase::SetHeartAttributes(int nHeartInterval, int nHeartTimeout) 
{ 
	m_nHeartInterval = nHeartInterval; 
	m_nHeartTimeout = nHeartTimeout; 
} 
 
void CServerSessionBase::SetAckAttributes(BYTE byWindowSize, int nAckInterval, 
							int nAckTimeout) 
{ 
	m_byWindowSize = byWindowSize;
	m_nAckInterval = nAckInterval; 
	m_nAckTimeout = nAckTimeout; 
} 
 
void CServerSessionBase::OnSessionConnect(CSid objSid)
{
	CServerBaseCltObj* pCltObjBase = GetClientObj(objSid);
	assert(pCltObjBase);
	
	CServerSessionBaseCltObj* pCltObj = (CServerSessionBaseCltObj* )pCltObjBase;
	pCltObj->SetWindowSize(m_byWindowSize);
}
  
void CServerSessionBase::OnSessionReady(CSid objSid)
{
}
  
int CServerSessionBase::GetRecvPacketSize(CBuffer* pBuf)
{
	int nPacketSize = CProtocolSession::StreamPacketSize(pBuf->GetBuffer()); 
	return nPacketSize;
} 
 
void CServerSessionBase::ProcessStreamPacket(CServerBaseCltObj* pCltObjBase, CBuffer* pBuf) 
{ 
	assert(pCltObjBase && pBuf); 

	CServerSessionBaseCltObj* pCltObj = (CServerSessionBaseCltObj* )pCltObjBase;

	LogDebug("CServerSessionBase::ProcessStreamPacket(), \n connection(%s) \n recv a packet, len=%d ", 
		pCltObj->GetTagConnInfo().c_str(), pBuf->GetUsed());

	if (!CProtocolSession::IsValid(pBuf->GetBuffer(), pBuf->GetUsed()))
	{
		LogImportant("CServerSessionBase::ProcessStreamPacket(), \n connection(%s) \n recv a packet which is invalid!",
			pCltObj->GetTagConnInfo().c_str()); 
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		pBuf->Release();
	} 
	else if (!AuthLevel(pCltObj, pBuf))
	{
		LogImportant("CServerSessionBase::ProcessStreamPacket(), \n connection(%s) \n AuthLevel() fail!",
			pCltObj->GetTagConnInfo().c_str());
		
		pBuf->Release();
	}
	else if (CProtocolSession::IsHeart(pBuf->GetBuffer())) 
	{ 
		OnRecvHeart(pCltObj, pBuf); 
		pBuf->Release(); 
	} 
	else if (CProtocolSession::IsData(pBuf->GetBuffer())) 
	{ 
		OnRecvData(pCltObj, pBuf); 
	} 
	else if (CProtocolSession::IsDataAck(pBuf->GetBuffer())) 
	{ 
		OnRecvDataAck(pCltObj, pBuf); 
		pBuf->Release(); 
	} 
	else if (CProtocolSession::IsNegReq(pBuf->GetBuffer())) 
	{ 
		OnRecvNegReq(pCltObj, pBuf); 
		pBuf->Release(); 
	} 
	else 
	{ 
		OnRecvOtherPacket(pCltObj, pBuf); 
		pBuf->Release(); 
	} 
} 

bool CServerSessionBase::AuthLevel(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf)
{
	assert(pCltObj && pBuf);
	bool bRet = true;
	if (CProtocolSession::IsNegReq(pBuf->GetBuffer()))
	{
		if (pCltObj->GetConnectStatus() != CS_WAITNEG)
		{
			bRet = false;
			LogImportant("CServerSessionBase::AuthLevel(), \n connection(%s) \n recv a neg_req pkt, but my level is %d, not CS_WAITNEG!", 
						pCltObj->GetTagConnInfo().c_str(), pCltObj->GetConnectStatus());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
	}
	else if (pCltObj->GetConnectStatus() == CS_WAITNEG && !CProtocolSession::IsHeart(pBuf->GetBuffer()))
	{
		bRet = false;
		LogImportant("CServerSessionBase::AuthLevel(), \n connection(%s) \n recv a pkt(not heart), it is not neg_req, but my level is CS_WAITNEG!", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	return bRet;
}
	
void CServerSessionBase::OnRecvHeart(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj);

	LogDebug("CServerSessionBase::OnRecvHeart(), \n connection(%s) \n recv heart pkt.",
		pCltObj->GetTagConnInfo().c_str());
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
} 
 
void CServerSessionBase::OnRecvData(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf);
	
	pCltObj->PushRecvSeq(CProtocolSession::GetPacketSeq(pBuf->GetBuffer()));
	if (pCltObj->IsNeedToAck())
	{
		SendDataAck(pCltObj); 
	}

	if (pBuf->GetUsed() <= CProtocolSession::HeaderSize())
	{
		LogImportant("CServerSessionBase::OnRecvData(), \n connection(%s) \n recv data, len=%d, but no data.", 
			pCltObj->GetTagConnInfo().c_str(), pBuf->GetUsed());
		
		pBuf->Release();
		return;
	}
	
	LogDebug("CServerSessionBase::OnRecvData(), \n connection(%s) \n recv data, len=%d", 
		pCltObj->GetTagConnInfo().c_str(), pBuf->GetUsed());
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());

	HandleUserData(pCltObj, pBuf);
} 

void CServerSessionBase::HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf)
{
	AddRecvPacketToQueue(pCltObj, pBuf);
}

void CServerSessionBase::OnRecvDataAck(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf);
	DWORD dwSeq = CProtocolSession::GetPacketSeq(pBuf->GetBuffer());

	LogDebug("CServerSessionBase::OnRecvDataAck(), \n connection(%s) \n recv data_ack pkt(seq=%d).",
		pCltObj->GetTagConnInfo().c_str(), dwSeq);
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());

	pCltObj->OnDataAck(dwSeq); 
} 

void CServerSessionBase::OnRecvNegReq(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{
	assert(pCltObj && pBuf);
	DWORD dwSeq = CProtocolSession::GetPacketSeq(pBuf->GetBuffer()); 
	pCltObj->InitRecvSeq(dwSeq); 

	CProtocolSession::CCSessionNegReq objNegReq; 
	objNegReq.FromStreamBuf(pBuf->GetBuffer()); 

	// 设置客户端地址
	pCltObj->SetClientAddr(objNegReq.m_objClientAddr);

	// 设置客户端标识
	if (objNegReq.m_byClientTag == CLIENTTAG_APP)
	{
		pCltObj->SetAppClient(true);
	}
	else
	{
		pCltObj->SetAppClient(false);
	}
	
	// 设置客户端的双网配对IP
	in_addr inaddr;
	inaddr.s_addr = objNegReq.m_dwPairNetIp;
	pCltObj->SetPairNetIp(inet_ntoa(inaddr));

	LogImportant("CServerSessionBase::OnRecvNegReq(), \n connection(%s) \n recv neg_req pkt, nextseq=%d, pairnet_ip=%s, is_appclient=%d.", 
		pCltObj->GetTagConnInfo().c_str(), dwSeq, pCltObj->GetPairNetIp().c_str(), pCltObj->IsAppClient());
	LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	 
	// 验证客户端ID是否有效
	WORD wCltId = objNegReq.m_objClientAddr.GetCltId();
	if (!IsClientIdValid(pCltObj))
	{
		LogImportant("CServerSessionBase::OnRecvNegReq(), \n connection(%s) \n recv neg_req pkt, nextseq=%d, client_id=%d, is_appclient=%d, but client_id is out of range %s, so will close client socket ...",
			pCltObj->GetTagConnInfo().c_str(), dwSeq,  pCltObj->GetClientId(), pCltObj->IsAppClient(), GetCliendIdValidRangeStr().c_str());
		
		CloseClientObj(pCltObj);
		return;
	}
	else if (IsClientIdExist(pCltObj))
	{
		LogImportant("CServerSessionBase::OnRecvNegReq(), \n connection(%s) \n recv neg_req pkt, nextseq=%d, client_id=%d, is_appclient=%d, but client_id has existed for the same ip_addr, so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str(), dwSeq,  pCltObj->GetClientId(), pCltObj->IsAppClient());
		
		CloseClientObj(pCltObj);
		return;
	}
	
	// 向客户端回复协商应答信息
	CBuffer* pBufAck = pCltObj->Allocate();
	int nLen = CProtocolSession::MakeNegAckPacket(pBufAck->GetBuffer(), pBufAck->GetBufSize(), 
				pCltObj->GetSendSeq(), pCltObj->GetWindowSize(), 
				m_nAckInterval, m_nAckTimeout, 
				m_nHeartInterval, m_nHeartTimeout); 
	if (nLen > 0) 
	{ 
		pBufAck->Use(nLen); 

		LogImportant("CServerSessionBase::OnRecvNegReq(), \n connection(%s) \n make neg_ack pkt.", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBufAck->GetBuffer(), pBufAck->GetUsed());
		
		pBufAck->SetWParam(pBufAck->GetUsed());
		pBufAck->SetFlag(0);
		pCltObj->GetSpSendQueue().AddTail(pBufAck);
		
		pCltObj->SetConnectStatus(CS_READY);
		OnSessionReady(pCltObj->GetSid());
	} 
	else 
	{ 
		LogImportant("CServerSessionBase::OnRecvNegReq(), \n connection(%s) \n make neg_ack packet fail.", 
			pCltObj->GetTagConnInfo().c_str());
		pBufAck->Release();
	} 
} 
 
void CServerSessionBase::OnRecvOtherPacket(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf);
	LogImportant("CServerSessionBase::OnRecvOtherPacket(), \n connection(%s) \n recv a pkt, but I don't know it!", 
		pCltObj->GetTagConnInfo().c_str()); 
	LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
} 

void CServerSessionBase::HeartDead(CServerSessionBaseCltObj* pCltObj) 
{  
	LogImportant("CServerSessionBase::HeartDead(), \n connection(%s) \n heart_dead, so will close client socket ...",
				pCltObj->GetTagConnInfo().c_str()); 

	CloseClientObj(pCltObj);
} 
 
void CServerSessionBase::SendDataAck(CServerSessionBaseCltObj* pCltObj) 
{ 
	assert(pCltObj);
	
	CBuffer* pBuf = pCltObj->Allocate(); 
	assert(pBuf);
	
	DWORD dwSeq = pCltObj->GetLastRecvSeq();
	int nLen = CProtocolSession::MakeDataAckPacket(pBuf->GetBuffer(), pBuf->GetBufSize(), dwSeq); 
	pBuf->Use(nLen);

	LogDebug("CServerSessionBase::SendDataAck(), \n connection(%s) \n make data_ack pkt(seq=%d).", 
		pCltObj->GetTagConnInfo().c_str(), dwSeq);
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	pCltObj->GetSpSendQueue().AddTail(pBuf);

	pCltObj->ClearRecvWindow();
} 

void CServerSessionBase::PatrolClientObject(CServerBaseCltObj* pCltObjBase)
{
	assert(pCltObjBase);

	CServerSessionBaseCltObj* pCltObj = (CServerSessionBaseCltObj* )pCltObjBase;

	if(pCltObj->GetSendQueueSize() > pCltObj->GetSendQueueDepth())
	{
		LogImportant("CServerSessionBase::PatrolClientObject(), \n connection(%s) \n send queue is too long, so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str());
		CloseClientObj(pCltObj);
	}

	//heart patrol
	DWORD spanRecv = CUtility::GetElapseTime(pCltObj->GetRecvTime());
	DWORD spanSend = CUtility::GetElapseTime(pCltObj->GetSendTime()); 
	if (pCltObj->IsSocketValid() && m_nHeartInterval > 0)
	{
		if (spanRecv >= m_nHeartTimeout) 
		{
			HeartDead(pCltObj); 
		}
		else if (spanSend >= m_nHeartInterval) 
		{
			SendHeart(pCltObj); 
		}
	}
	
	//ackfail patrol 
	if (pCltObj->IsSocketValid() && pCltObj->IsSendWindowFull())
	{
		DWORD spanFull;
		spanFull = CUtility::GetElapseTime(pCltObj->GetSendWindowFullTime());
		if (spanFull > m_nAckTimeout)
		{
			AckFail(pCltObj);
		}
	}
	
	//ack time patrol
	if (pCltObj->IsSocketValid() && pCltObj->GetRecvCount() > 0)
	{
		DWORD spanFirst;
		spanFirst = CUtility::GetElapseTime(pCltObj->GetRecvWindowFirstTime());
		if (spanFirst > m_nAckInterval)
		{
			SendDataAck(pCltObj);
		}
	}

	//如果用户数据队列中最前面的包发送不完整,那么先发送用户数据队列第一包,试n次
	if (pCltObj->IsSocketValid() && pCltObj->IsSendQueueInPart())
	{
		for(int i=0; i<1; i++)
		{
			Write(pCltObj);
		}
	}

	//用户数据队列中最前面的包已经完整发送了,则发送协议发送队列,直到发送干净，才允许发送用户数据队列
	if(pCltObj->IsSocketValid() && !pCltObj->IsSendQueueInPart())
	{
		while (!pCltObj->GetSpSendQueue().IsEmpty())
		{
			if (!WriteSp(pCltObj))
			{
				break;
			}
		}

		//协议队列空了，才允许发送用户数据队列
		if(pCltObj->GetSpSendQueue().IsEmpty())
		{
			while (pCltObj->GetSendQueueSize() > 0)
			{
				if (!Write(pCltObj))
				{
					break;
				}
			}
		}
	}
}

void CServerSessionBase::SendHeart(CServerSessionBaseCltObj* pCltObj) 
{  
	assert(pCltObj);

	CBuffer* pBuf = pCltObj->Allocate();
	assert(pBuf);
	int nLen = CProtocolSession::MakeHeartPacket(pBuf->GetBuffer(), pBuf->GetBufSize()); 
	pBuf->Use(nLen);

	LogImportant("CServerSessionBase::SendHeart(), \n connection(%s) \n make heart pkt.", 
		pCltObj->GetTagConnInfo().c_str());
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	pCltObj->GetSpSendQueue().AddTail(pBuf);
} 

void CServerSessionBase::AckFail(CServerSessionBaseCltObj* pCltObj)
{
	assert(pCltObj);
	
	LogImportant("CServerSessionBase::AckFail(), \n connection(%s) \n ack fail! last_seq=%d, so will close client socket ...",
				pCltObj->GetTagConnInfo().c_str(), pCltObj->GetSendSeq());
	
	CloseClientObj(pCltObj);
}

bool CServerSessionBase::Write(CServerBaseCltObj* pCltObjBase) 
{ 
	assert(pCltObjBase);

	CServerSessionBaseCltObj* pCltObj = (CServerSessionBaseCltObj* )pCltObjBase;

	if(!pCltObj->IsSocketValid())
	{
		LogImportant("CServerSessionBase::Write(), \n connection(%s) \n write not ready, socket is invalid. queue size=%d",
			pCltObj->GetTagConnInfo().c_str(), pCltObj->GetSendQueueSize());
		return false;
	}

	if (!pCltObj->IsReady())
	{
		LogImportant("CServerSessionBase::Write(), \n connection(%s) \n write not ready, conn_status=%d, queue size=%d",
			pCltObj->GetTagConnInfo().c_str(), pCltObj->GetConnectStatus(), pCltObj->GetSendQueueSize());
		return false;
	}

	if(!pCltObj->IsSendQueueInPart() && m_byWindowSize != 0 && pCltObj->GetSendCount() >= m_byWindowSize)
	{
		LogImportant("CServerSessionBase::Write(), \n connection(%s) \n write not ready, send window is full, queue size=%d",
			pCltObj->GetTagConnInfo().c_str(), pCltObj->GetSendQueueSize());
		return false;
	}
	
	CBuffer* pBuf = pCltObj->PopSendData(); 
	if (!pBuf) 
	{
		return false;
	}
	
	if (pBuf->GetFlag() == 0)
	{
		pCltObj->IncSendSeq(); 
		CProtocolSession::SetPacketSeq(pBuf->GetBuffer(), pCltObj->GetSendSeq()); 
		pCltObj->PushSendSeq();
	}

	bool bRet = true;
	if (pBuf->GetUsed() > 0) 
	{ 
		OnWrite(pCltObj, pBuf);  
  
		int iRet = send(pCltObj->GetSocket(), pBuf->GetBuffer(), pBuf->GetUsed(), 0);  
		if (iRet == pBuf->GetUsed())  
		{  
			OnWriteCompleted(pCltObj, pBuf);  
		}  
		else if (iRet > 0) 
		{  
			OnWritePart(pCltObj, pBuf, iRet);
			bRet = false;
		}
		else 
		{
			OnWriteError(pCltObj, pBuf);  
			bRet = false;
		}  
	} 
	else 
	{ 
		pBuf->Release(); 
		LogImportant("CServerSessionBase::Write(), \n connection(%s) \n write error, send buf no data.",
			pCltObj->GetTagConnInfo().c_str()); 
	} 
	
	return bRet;
} 

bool CServerSessionBase::WriteSp(CServerSessionBaseCltObj* pCltObj) 
{ 
	assert(pCltObj);
	
	CBuffer* pBuf; 
	if (!pCltObj->GetSpSendQueue().GetHead(pBuf)) 
	{
		return false; 
	}
	
	bool bRet = true;
	if (pBuf->GetUsed() > 0) 
	{ 
		OnWriteSp(pCltObj, pBuf);  
  
		int nRet = send(pCltObj->GetSocket(), pBuf->GetBuffer(), pBuf->GetUsed(), 0);  
		
		if (nRet == pBuf->GetUsed())  
		{  
			OnWriteCompletedSp(pCltObj, pBuf);  
		}  
		else if (nRet > 0) 
		{  
			OnWritePartSp(pCltObj, pBuf, nRet);
			bRet = false;
		}
		else 
		{
			OnWriteErrorSp(pCltObj, pBuf);  
			bRet = false;
		}  
	} 
	else 
	{ 
		pBuf->Release(); 
		LogImportant("CServerSessionBase::WriteSp(), \n connection(%s) \n write sp error, send buf no data.",
			pCltObj->GetTagConnInfo().c_str()); 
	}
	 
	return bRet;
} 

void CServerSessionBase::OnWriteSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	pCltObj->SetSendTime(); 

// 	LogDebug("CServerSessionBase::OnWriteSp(), \n connection(%s) \n write sp data, len=%d", 
// 		pCltObj->GetTagConnInfo().c_str(), pBuf->GetUsed());
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
} 
 
void CServerSessionBase::OnWriteCompletedSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf);

	LogDebug("CServerSessionBase::OnWriteCompletedSp(), \n connection(%s) \n write sp completed, total=%d, write=%d", 
		pCltObj->GetTagConnInfo().c_str(), pBuf->GetWParam(), pBuf->GetUsed());
	if (!pBuf->GetFlag()) // 只有包头部分打印日志，剩余部分打印日志无意义，而且有时候数量很多
	{
		LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	pBuf->Release(); 
} 
 
void CServerSessionBase::OnWritePartSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf, int nSend) 
{
	LogDebug("CServerSessionBase::OnWritePartSp(), \n connection(%s) \n write sp parted, total=%d, write=%d, left=%d", 
		pCltObj->GetTagConnInfo().c_str(), pBuf->GetWParam(), nSend, pBuf->GetUsed() - nSend);
	if (!pBuf->GetFlag()) // 只有包头部分打印日志，剩余部分打印日志无意义，而且有时候数量很多
	{
		LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	pBuf->Cut(0, nSend);
	if (pBuf->GetUsed() > 0)
	{
		pBuf->SetFlag(1);
		pCltObj->RollbackDataSp(pBuf);

		LogDebug("CServerSessionBase::OnWritePartSp(), \n connection(%s) \n roll back the left sp to send queue, total=%d, left=%d",
			pCltObj->GetTagConnInfo().c_str(), pBuf->GetWParam(), pBuf->GetUsed());
	}
	else
	{
		LogDebug("CServerSessionBase::OnWritePartSp(), \n connection(%s) \n left length=0.", 
			pCltObj->GetTagConnInfo().c_str());

		pBuf->Release();
	}
}

void CServerSessionBase::OnWriteErrorSp(CServerSessionBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	int nErr = ERRNO;

	if (nErr == EAGAIN || nErr == EINTR)
	{
		pCltObj->RollbackDataSp(pBuf);

		LogImportant("CServerSessionBase::OnWriteErrorSp(), \n connection(%s) \n write sp error=%d (EAGAIN or EINTR), put data back to sp send queue.",
			pCltObj->GetTagConnInfo().c_str(), nErr);
	}
	else
	{
		LogImportant("CServerSessionBase::OnWriteErrorSp(), \n connection(%s) \n write sp error=%d , so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str(), nErr);
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());

		pBuf->Release(); 
		CloseClientObj(pCltObj);
	}
} 

CServerBaseCltObj* CServerSessionBase::CreateClientObject() 
{ 
	CServerSessionBaseCltObj* pCltObj = new CServerSessionBaseCltObj(this);
	if (pCltObj)
	{
		pCltObj->SetLogFileTag(GetLogFileTag());
	}
	
	return pCltObj;
} 

short CServerSessionBase::GetClientId(CSid objSid)
{
	CServerSessionBaseCltObj* pCltObj = (CServerSessionBaseCltObj*)GetClientObj(objSid); 
	if (pCltObj) 
	{
		assert(pCltObj->IsMyClassName("CServerSessionBaseCltObj"));
		
		return pCltObj->GetClientId();
	}
	
	return -1;
}

string CServerSessionBase::GetCliendIdValidRangeStr()
{
	return "";
}

bool CServerSessionBase::IsClientIdValid(CServerSessionBaseCltObj* pCltObj)
{
	return false;
}

bool CServerSessionBase::IsClientIdExist(CServerSessionBaseCltObj* pCltObj)
{
	bool bRet = false;
	
	if (!m_ClientsPtr)
	{
		return bRet; 
	}
	
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClientObj的影响
	for (int x = 0; x < nMaxClientBound; x++) 
	{
		CServerSessionBaseCltObj* pCltObjTmp = (CServerSessionBaseCltObj*)m_ClientsPtr[x];
		if (!pCltObjTmp || pCltObjTmp == pCltObj)
		{
			continue;
		}
		assert(pCltObjTmp->IsMyClassName("CServerSessionBaseCltObj"));
		
		if (!pCltObjTmp->IsConnect())
		{
			continue;
		}

// 		if (pCltObjTmp->GetClientAddr() == pCltObj->GetClientAddr())
// 		{
// 			LogImportant("CServerSessionBase::IsClientIdExist(), \n client_addr(%s) "
// 				"has exist, the exist \n connection(%s) \n Sid=%d:%d.",
// 				pCltObj->GetClientAddr().GetDumpStr().c_str(), 
// 				pCltObjTmp->GetTagConnInfo().c_str(), 
// 				pCltObjTmp->GetSid().GetHigh(),	pCltObjTmp->GetSid().GetLow());
// 			bRet = true;
// 			break;
// 		}

		if (strcmp(pCltObjTmp->GetPeerIp().c_str(), pCltObj->GetPeerIp().c_str()) == 0 
			&& pCltObjTmp->GetClientId() == pCltObj->GetClientId()) 
		{ 
			LogImportant("CServerSessionBase::IsClientIdExist(), \n client_id(%d) "
				"has existed for ip_addr(%s), the exist \n connection(%s) \n Sid=%d:%d.",
				pCltObj->GetClientId(),
				pCltObj->GetPeerIp().c_str(), 
				pCltObjTmp->GetTagConnInfo().c_str(), 
				pCltObjTmp->GetSid().GetHigh(),	pCltObjTmp->GetSid().GetLow());
			bRet = true;
			break;
		}
	}
	
	return bRet;
}

bool CServerSessionBase::RecvSessionData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort) 
{
	CSid objSid;
	bool bRet = RecvSessionData(szBuf, nBufLen, objSid);
	
	CServerBaseCltObj* pCltObj = GetClientObj(objSid);
	if (pCltObj)
	{
		memcpy(szCltIP, pCltObj->GetPeerIp().c_str(), pCltObj->GetPeerIp().size());
		nCltPort = static_cast<unsigned short>(pCltObj->GetPeerPort());
	}
	
	return bRet;
}

bool CServerSessionBase::RecvSessionData(CBuffer* pBuf, char* szCltIP,unsigned short& nCltPort) 
{  
	CSid objSid;
	bool bRet = RecvSessionData(pBuf, objSid);
	
	CServerBaseCltObj* pCltObj = GetClientObj(objSid);
	if (pCltObj)
	{
		memcpy(szCltIP, pCltObj->GetPeerIp().c_str(), pCltObj->GetPeerIp().size());
		nCltPort = static_cast<unsigned short>(pCltObj->GetPeerPort());
	}
	
	return bRet;
}

bool CServerSessionBase::RecvSessionData(char* szBuf, int& nBufLen, CSid& objSid) 
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
				objSid.Import(pBufInQueue->GetWParam(), pBufInQueue->GetLParam());
				memcpy(szBuf, pData, nDataLen); 
				nBufLen = nDataLen; 
				bRet = true;  
			}
			
			pBufInQueue->Release();
		} 
	} 
	
	return bRet; 
} 
 
bool CServerSessionBase::RecvSessionData(CBuffer* pBuf, CSid& objSid) 
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
			objSid.Import(pBufInQueue->GetWParam(), pBufInQueue->GetLParam());
			pBuf->AddData(pData, nDataLen);
			bRet = true;
		}
		
		pBufInQueue->Release();
	}
	
	return bRet; 
} 

bool CServerSessionBase::SendSessionData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort) 
{  
	return SendSessionData(pBuf->GetBuffer(), pBuf->GetUsed(), szCltIP, nCltPort);
} 

bool CServerSessionBase::SendSessionData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort) 
{  
	CServerBaseCltObj* pCltObj = GetClientObj(szCltIP, nCltPort); 
	if (!pCltObj) 
	{
		LogImportant("CServerSessionBase::SendSessionData(), \n server(%s) \n send error! can not find client for ip_port{%s:%d}", 
			GetTagMyInfo().c_str(), szCltIP, nCltPort); 
		return false; 
	}
	
	return SendSessionData(szBuf, nBufLen, pCltObj);
}

bool CServerSessionBase::SendSessionData(CBuffer* pBuf, CSid objSid) 
{  
	return SendSessionData(pBuf->GetBuffer(), pBuf->GetUsed(), objSid);
} 

bool CServerSessionBase::SendSessionData(char* szBuf, int nBufLen, CSid objSid) 
{  
	CServerBaseCltObj* pCltObj = GetClientObj(objSid); 
	if (!pCltObj) 
	{
		LogImportant("CServerSessionBase::SendSessionData(), \n server(%s) \n send error! can not find client for sid{%d,%d}", 
			GetTagMyInfo().c_str(), objSid.GetHigh(), objSid.GetLow()); 
		return false; 
	}
	
	return SendSessionData(szBuf, nBufLen, pCltObj);
} 

bool CServerSessionBase::SendSessionData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj) 
{  
	assert(pCltObj);
	assert(IsMyClientObj(pCltObj));
	
	if (!pCltObj->IsConnect()) 
	{ 
		LogImportant("CServerSessionBase::SendSessionData(), \n connection(%s) \n it is disconnect, can not send data!", 
			pCltObj->GetTagConnInfo().c_str()); 
		return false; 
	} 
	
	if (nBufLen <= 0)
	{
		LogImportant("CServerSessionBase::SendSessionData(), \n connection(%s) \n send fail! buf_len(%d) is error!", 
			pCltObj->GetTagConnInfo().c_str(), nBufLen); 
		return false; 
	}
	
	int nDataLen = nBufLen;
	int nNeedLen = CProtocolSession::HeaderSize() + nDataLen;
	if (nNeedLen > GetMaxPacketSize())
	{ 
		LogImportant("CServerSessionBase::SendSessionData(), \n connection(%s) \n send fail! pkt need len(%d) is larger than max(%d)!",
			pCltObj->GetTagConnInfo().c_str(), nNeedLen, GetMaxPacketSize()); 
		return false; 
	} 

	// 先分配足够内存
	CBuffer* pBuf = pCltObj->Allocate(); 
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
	pCltObj->PushSendData(pBuf);
	
	return true; 
} 

bool CServerSessionBase::LoadServerAddr()
{
	// 这里读取配置文件获取自身设备地址以给发送的数据填上源地址信息
	m_objServerAddr = CCommAddr(); // 清空
	
	CLocalDeviceConf objLocalDevConf;
	char szErrInfo[1024];
	if (!objLocalDevConf.Load(szErrInfo, sizeof(szErrInfo)))
	{
		LogImportant("CServerSessionBase::LoadServerAddr(), \n %s!", szErrInfo);
		return false;
	}
	
	m_objServerAddr.SetBureauId(objLocalDevConf.m_byBureauId);
	m_objServerAddr.SetUnitType(objLocalDevConf.m_byUnitType);
	m_objServerAddr.SetUnitId(objLocalDevConf.m_wUnitId);
	m_objServerAddr.SetDevType(objLocalDevConf.m_byDevType);
	m_objServerAddr.SetDevId(objLocalDevConf.m_wDevId);
	m_objServerAddr.SetAbFlag(objLocalDevConf.m_byAbType);
	m_objServerAddr.SetCltId(0);
	
	return true;
}

CCommAddr& CServerSessionBase::GetServerAddr()
{
	return m_objServerAddr;
}

void CServerSessionBase::SetAppServer(bool bIsAppServer)
{
	m_bIsAppServer = bIsAppServer;
}

void CServerSessionBase::SetServerId(DWORD dwServerId)
{
	m_objServerAddr.SetCltId(dwServerId);
	SetSidHigh(dwServerId);
}

DWORD CServerSessionBase::GetServerId()
{
	return m_objServerAddr.GetCltId();
}

bool CServerSessionBase::IsAppServer()
{
	return m_bIsAppServer;
}

std::string CServerSessionBase::GetServerIdValidRangeStr()
{
	return "";
}

bool CServerSessionBase::IsServerIdValid()
{
	return false;
}

//==============================================================================
//                 
//                    CServerSessionBaseCltObj
//           会话层服务端基类的客户端对象类（带协商、心跳、发送确认的机制）
//
//==============================================================================
CServerSessionBaseCltObj::CServerSessionBaseCltObj(CServerSessionBase* pServer) :
		CServerBaseCltObj(pServer),
		m_byWindowSize(DEF_COMMWINSIZE),
		m_dwSendWinFullTime(0),
		m_dwSendSeq(INIT_SESSION_SEQ),
		m_bySendCount(0),
		m_dwRecvWinFirstTime(0),
		m_dwRecvSeq(INIT_SESSION_SEQ),
		m_byRecvCount(0)
{
	SetMyClassName("CServerSessionBaseCltObj");

	memset(m_szPairNetIp, 0, sizeof(m_szPairNetIp));
	m_bIsAppClient = false;
}

CServerSessionBaseCltObj::~CServerSessionBaseCltObj()
{
}

void CServerSessionBaseCltObj::Connect(int nSockfd, const struct sockaddr_in& stuSockAddr)
{
	m_dwSendWinFullTime = 0;
	m_dwSendSeq = INIT_SESSION_SEQ;
	m_bySendCount = 0;
	m_dwRecvWinFirstTime = 0;
	m_dwRecvSeq = INIT_SESSION_SEQ;
	m_byRecvCount = 0;
	
	ClearSpSendQueue();
	
	CServerBaseCltObj::Connect(nSockfd, stuSockAddr);
}

void CServerSessionBaseCltObj::Connect(int nSockfd)
{
	m_dwSendWinFullTime = 0;
	m_dwSendSeq = INIT_SESSION_SEQ;
	m_bySendCount = 0;
	m_dwRecvWinFirstTime = 0;
	m_dwRecvSeq = INIT_SESSION_SEQ;
	m_byRecvCount = 0;
	
	ClearSpSendQueue();
	
	CServerBaseCltObj::Connect(nSockfd);
}

bool CServerSessionBaseCltObj::Close()
{
	if (SOCKET_VALID(m_nSockfd))
	{
		m_dwSendWinFullTime = 0;
		m_dwSendSeq = INIT_SESSION_SEQ;
		m_bySendCount = 0;
		m_dwRecvWinFirstTime = 0;
		m_dwRecvSeq = INIT_SESSION_SEQ;
		m_byRecvCount = 0;
		
		ClearSpSendQueue(); 

		return CServerBaseCltObj::Close();
	}
	else
	{
		LogImportant("CServerSessionBaseCltObj::Close(), \n connection(%s) \n close fail because socket is invalid!", 
			GetTagConnInfo().c_str()); 
		
		return false;
	}
}

bool CServerSessionBaseCltObj::IsReady() 
{ 
	return GetConnectStatus() == CS_READY;	 
} 

void CServerSessionBaseCltObj::OnConnect()
{
	CServerBaseCltObj::OnConnect();

	if (m_pRecvTmpBuf && m_pRecvTmpBuf->GetBufSize() < MAX_PACKETSIZE)
	{
		m_pRecvTmpBuf->Extend(MAX_PACKETSIZE - m_pRecvTmpBuf->GetBufSize());
	}

	// 网络重连时清除历史协商信息
	m_objClientAddr = CCommAddr();	
	m_bIsAppClient = false;
	memset(m_szPairNetIp, 0, IP_LEN);

	SetConnectStatus(CS_WAITNEG); 
}

void CServerSessionBaseCltObj::OnClose()
{
	// 网络重连时清除历史协商信息
	m_objClientAddr = CCommAddr();	
	m_bIsAppClient = false;
	memset(m_szPairNetIp, 0, IP_LEN);

	CServerBaseCltObj::OnClose();
}
void CServerSessionBaseCltObj::SetWindowSize(BYTE bySize)
{ 
	m_byWindowSize = bySize;
}
 
BYTE CServerSessionBaseCltObj::GetWindowSize()
{ 
	return m_byWindowSize;	
} 

DWORD CServerSessionBaseCltObj::GetSendWindowFullTime()
{ 
	return m_dwSendWinFullTime;
} 

DWORD CServerSessionBaseCltObj::GetSendSeq()
{
	return m_dwSendSeq;
}

void CServerSessionBaseCltObj::IncSendSeq()
{
	m_dwSendSeq++;
}
	
void CServerSessionBaseCltObj::PushSendSeq()
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

bool CServerSessionBaseCltObj::IsSendWindowFull() const
{
	return (m_byWindowSize != 0 && m_bySendCount >= m_byWindowSize);
}

bool CServerSessionBaseCltObj::IsNeedToAck() const 
{ 
	if (m_byWindowSize == 0)
	{
		return false;
	}
	else if (m_byRecvCount >= m_byWindowSize)
	{
		return true;
	}
	
	return false;
}

BYTE CServerSessionBaseCltObj::GetSendCount()
{
	return m_bySendCount;
}

CQueue<CBuffer* >& CServerSessionBaseCltObj::GetSpSendQueue()
{ 
	return m_SpSendQueue; 
}

bool CServerSessionBaseCltObj::RollbackDataSp(CBuffer* pBuf)
{
	if (m_SpSendQueue.GetSize() >= m_nSendQueueDepth)
	{
		LogImportant("CServerSessionBaseCltObj::RollbackDataSp(), \n connection(%s) \n sp send queue full, clear queue, data lost.", 
			GetTagConnInfo().c_str());
		m_SpSendQueue.Empty();
	}
	
	return m_SpSendQueue.AddHead(pBuf);
}

void CServerSessionBaseCltObj::ClearSpSendQueue()
{
	// 队列清空前输出日志
	int nLastPktCount = m_SpSendQueue.GetSize();
	if (nLastPktCount > 0)
	{
		if (IsConnect())
		{
			LogImportant("CServerSessionBaseCltObj::ClearSpSendQueue(), \n connection(%s) \n it is connecting, queue_size=%d, will throw packets ...", 
				GetTagConnInfo().c_str(), nLastPktCount);
		}
		else
		{
			LogImportant("CServerSessionBaseCltObj::ClearSpSendQueue(), \n connection(%s) \n it is disconnected, queue_size=%d, will throw packets ...", 
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
		LogImportant("CServerSessionBaseCltObj::ClearSpSendQueue(), \n connection(%s) \n succeed to throw %d packets, now queue_size=%d.", 
			GetTagConnInfo().c_str(), nThrowPktCount, m_SpSendQueue.GetSize());
	}
}

DWORD CServerSessionBaseCltObj::GetRecvWindowFirstTime()
{ 
	return m_dwRecvWinFirstTime;
}
 
void CServerSessionBaseCltObj::InitRecvSeq(DWORD dwSeq)
{
	m_dwRecvSeq = dwSeq;
}

void CServerSessionBaseCltObj::PushRecvSeq(DWORD dwSeq)
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
		LogImportant("CServerSessionBaseCltObj::PushRecvSeq(), \n connection(%s) \n recv seq(%d) is not continuous, last_seq=%d. so update it!", 
			GetTagConnInfo().c_str(), dwSeq, m_dwRecvSeq);
		m_dwRecvSeq = dwSeq;
	}
}

DWORD CServerSessionBaseCltObj::GetLastRecvSeq() const
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

bool CServerSessionBaseCltObj::IsRecvWindowFull() const
{
	if (m_byWindowSize == 0)
	{
		return false;
	}
	else if (m_byRecvCount >= m_byWindowSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BYTE CServerSessionBaseCltObj::GetRecvCount() const
{ 
	return m_byRecvCount;
}

void CServerSessionBaseCltObj::ClearRecvWindow()
{ 
	m_byRecvCount = 0;
}

void CServerSessionBaseCltObj::OnDataAck(DWORD dwSeq)
{
	int x = 0;
	if (m_bySendCount > 0)
	{
		bool bGetIt = false;
		for (x = m_bySendCount - 1; x >= 0; x--)
		{
			if (m_SendWindow[x] == dwSeq)
			{
				bGetIt = true;
				break;
			}
		}
		if (bGetIt)
		{
			m_bySendCount = m_bySendCount - x - 1;
			if (m_bySendCount > 0)
			{
				memmove(m_SendWindow, &m_SendWindow[x+1], m_bySendCount*sizeof(DWORD));
			}
		}
		else
		{
			LogImportant("CServerSessionBaseCltObj::OnDataAck(), \n connection(%s) \n recv data_ack pkt(seq=%d), but I can't find it from my window!", 
				GetTagConnInfo().c_str(), dwSeq);
		}
	}
	else
	{
		LogImportant("CServerSessionBaseCltObj::OnDataAck(), \n connection(%s) \n recv data_ack pkt(seq=%d), but my send window is space!", 
			GetTagConnInfo().c_str(), dwSeq);
	}
}

void CServerSessionBaseCltObj::SetPairNetIp(const char* szIp)
{
	assert(szIp && strlen(szIp) < sizeof(m_szPairNetIp));
	strcpy(m_szPairNetIp, szIp);
}

string CServerSessionBaseCltObj::GetPairNetIp()
{
	return m_szPairNetIp;
}

void CServerSessionBaseCltObj::SetClientId(short sClientId)
{
	m_objClientAddr.SetCltId(sClientId);
} 

short CServerSessionBaseCltObj::GetClientId()
{
	return m_objClientAddr.GetCltId();
} 

bool CServerSessionBaseCltObj::IsAppClient()
{
	return m_bIsAppClient;
}

void CServerSessionBaseCltObj::SetClientAddr( CCommAddr& objCltAddr )
{
	m_objClientAddr = objCltAddr;
}

CCommAddr& CServerSessionBaseCltObj::GetClientAddr()
{
	return m_objClientAddr;
}

void CServerSessionBaseCltObj::SetAppClient(bool bIsAppClient)
{
	m_bIsAppClient = bIsAppClient;
}

bool CServerSessionBaseCltObj::IsPairWith(CServerSessionBaseCltObj* pCltObj)
{
	if (!pCltObj || pCltObj == this)
	{
		return false;
	}
	
	if (GetClientAddr() == pCltObj->GetClientAddr() 
		&& strcmp(GetPairNetIp().c_str(), pCltObj->GetPeerIp().c_str()) == 0
		&& strcmp(GetPeerIp().c_str(), pCltObj->GetPairNetIp().c_str()) == 0)
	{
		return true;
	}
	
	return false;
}

string CServerSessionBaseCltObj::GetTagCltInfo()
{
	char temp[256];
	memset(temp, 0, sizeof(temp));
	
	if (m_pRelatedServerPtr->GetType() == AF_INET)
	{
		snprintf(temp, sizeof(temp), "%s:%d:%d", m_szPeerIp, m_sPeerPort, m_objClientAddr.GetCltId());
	}
	else
	{
		snprintf(temp, sizeof(temp), "%d", m_objClientAddr.GetCltId());
	}
	
	return temp;
}

std::string CServerSessionBaseCltObj::GetTagCltAddrInfo()
{
	char temp[256];
	memset(temp, 0, sizeof(temp));
	
	snprintf(temp, sizeof(temp), "%d:%d:%d:%d:%d:%d", 
		m_objClientAddr.GetBureauId(), m_objClientAddr.GetUnitType(), m_objClientAddr.GetUnitId(),
		m_objClientAddr.GetDevType(), m_objClientAddr.GetDevId(), m_objClientAddr.GetCltId());

	return temp;
}

