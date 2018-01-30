//==============================================================================
//                      ServerForwardBase.cpp
//
//begin   : 2016-09-08
//describe:
//==============================================================================

#include "ServerForwardBase.h"
#include "CommUtility.h"
#include "Utility.h"
#include "LocalDeviceConf.h"

//==============================================================================
//                 
//                    ServerForwardBase
//           转发层客户端基类（带双网、转发条件机制）
//
//==============================================================================
CServerForwardBase::CServerForwardBase()
{  
	SetMyClassName("CServerForwardBase");
} 

CServerForwardBase::~CServerForwardBase() 
{  
}

CServerBaseCltObj* CServerForwardBase::CreateClientObject()
{
	CServerForwardBaseCltObj* pCltObj = new CServerForwardBaseCltObj(this);
	if (pCltObj)
	{
		pCltObj->SetLogFileTag(GetLogFileTag());
	}
	
	return pCltObj;
}

bool CServerForwardBase::SetSrcAddrForSendData(char* szBuf, int nBufLen)
{
	if (CProtocolForward::IsMsgForwardPkt(szBuf, nBufLen))
	{
		if (!CProtocolForward::SetSrcAddr(szBuf, nBufLen, m_objServerAddr))
		{
			LogImportant("CServerForwardBase::SetSrcAddrForSendData(), \n server(%s) \n fail to set src_addr for data pkt!",
				GetTagMyInfo().c_str());
			return false;
		}
	}
	
	return true;
}

bool CServerForwardBase::SetTypeFuncForSendData(char* szBuf, int nBufLen)
{
	if (CProtocolForward::IsMsgForwardPkt(szBuf, nBufLen))
	{
		if (!CProtocolForward::UpdateTypeFunc(szBuf, nBufLen))
		{
			LogImportant("CServerForwardBase::SetTypeFuncForSendData(), \n server(%s) \n fail to set type_func info!",
				GetTagMyInfo().c_str());
			return false;
		}
	}
	
	return true;
}

bool CServerForwardBase::CheckSendData(char* szBuf, int nBufLen)
{
	char szErrInfo[1024];
	memset(szErrInfo, 0, sizeof(szErrInfo));
	
	if (!CProtocolForward::IsValid(szBuf, nBufLen, szErrInfo, sizeof(szErrInfo) - 1))
	{
		LogImportant("CServerForwardBase::CheckSendData(), \n server(%s) \n send buf is invalid: %s", 
			GetTagMyInfo().c_str(), szErrInfo);
		return false;
	}
	
	return true;
}

bool CServerForwardBase::InitCommParam(TiXmlHandle& txhServer, char cNetNo)
{
	if (!LoadServerAddr())
	{
		return false;		
	}
	
	// <Type> 
	TiXmlElement* pTxeElem = txhServer.FirstChild("Type").Element(); 
	if (pTxeElem) 
	{ 
		if (!strcmp("AF_INET", pTxeElem->GetText()))
		{
			SetType(AF_INET);
		}
		else if (!strcmp("AF_UNIX", pTxeElem->GetText())) 
		{
			SetType(AF_UNIX);
		}
		else 
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n fail to read the value of <Type>%s</Type>.",
				pTxeElem->GetText());
			return false; 
		} 
	} 
	else
	{
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <Type>!");
		return false;
	}

	if (m_nType == AF_INET && cNetNo == 1) 
	{ 
		// <Net1><ServerIp>
		pTxeElem = txhServer.FirstChild("Net1").FirstChild("ServerIp").Element(); 
		if (pTxeElem) 
		{ 
			if (!CCommUtility::IsValidIP(pTxeElem->GetText())) 
			{ 
				LogImportant("CServerForwardBase::InitCommParam(), \n <Net1><ServerIp>%s</ServerIp></Net1> is invalid!", 
					pTxeElem->GetText()); 
				return false; 
			} 
			else 
			{
				SetSvrIp(pTxeElem->GetText());
			} 
		}
		else
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <Net1><ServerIp>!");
			return false;
		}

		// <Net1><ServerPort>
		pTxeElem = txhServer.FirstChild("Net1").FirstChild("ServerPort").Element(); 
		if (pTxeElem) 
		{
			int nPort = CUtility::ATOI(pTxeElem->GetText()); 
			if (nPort < 1024) 
			{ 
				LogImportant("CServerForwardBase::InitCommParam(), \n <Net1><ServerPort>%d</ServerPort></Net1> is out of range(1024-~)!", nPort); 
				return false; 
			} 
			else
			{
				SetSvrPort(nPort);
			}
		}
		else
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <Net1><ServerPort>!");
			return false;
		}
	} 
	else if (m_nType == AF_INET && cNetNo == 2) 
	{
		// <Net2><ServerIp>
		pTxeElem = txhServer.FirstChild("Net2").FirstChild("ServerIp").Element(); 
		if (pTxeElem) 
		{ 
			if (!CCommUtility::IsValidIP(pTxeElem->GetText())) 
			{ 
				LogImportant("CServerForwardBase::InitCommParam(), \n <Net2><ServerIp>%s</ServerIp></Net2> is invalid!", 
					pTxeElem->GetText()); 
				return false; 
			} 
			else 
			{
				SetSvrIp(pTxeElem->GetText());
			} 
		}
		else
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <Net2><ServerIp>!");
			return false;
		}
		
		// <Net2><ServerPort>
		pTxeElem = txhServer.FirstChild("Net2").FirstChild("ServerPort").Element(); 
		if (pTxeElem) 
		{
			int nPort = CUtility::ATOI(pTxeElem->GetText()); 
			if (nPort < 1024) 
			{ 
				LogImportant("CServerForwardBase::InitCommParam(), \n <Net2><ServerPort>%d</ServerPort></Net2> is out of range(1024-~)!", nPort); 
				return false; 
			} 
			else
			{
				SetSvrPort(nPort);
			}
		}
		else
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <Net2><ServerPort>!");
			return false;
		}
	}
	else if (m_nType == AF_UNIX && cNetNo == 1)
	{
		pTxeElem = txhServer.FirstChild("UnixPath").Element(); 
		if (pTxeElem)
		{
			int nSvrPathLen = strlen(pTxeElem->GetText());
			if (nSvrPathLen <= 0 || nSvrPathLen > UNIX_PATH_LEN - 1)
			{
				LogImportant("CServerForwardBase::InitCommParam(), \n <UnixPath>%s</UnixPath> len=%d "
					"is out of the range(1-%d).", pTxeElem->GetText(), nSvrPathLen, UNIX_PATH_LEN - 1);
				return false;
			}
			else
			{
				SetUnixPath(pTxeElem->GetText());
			}
		}
		else
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <UnixPath>!");
			return false;
		}
	} 
	else if (m_nType == AF_UNIX && cNetNo == 2)
	{
		LogImportant("CServerForwardBase::InitCommParam(), \n AF_UNIX server should be single net mode!");
		return false;
	} 

	// <ServerId>
	pTxeElem = txhServer.FirstChild("ServerId").Element();
	if (pTxeElem)
	{
		int nSvrId = CUtility::ATOI(pTxeElem->GetText());
		if (nSvrId < 1 || nSvrId > 32767)
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n <ServerId>%d</ServerId> is out "
				"of the range(1-32767)!", nSvrId);
			return false;
		}
		else
		{
			SetServerId(nSvrId);

			if (!IsServerIdValid())
			{
				LogImportant("CServerForwardBase::InitCommParam(), \n <ServerId>%d</ServerId> is out "
					"of the range %s!", nSvrId, GetServerIdValidRangeStr().c_str());
				return false;
			}

		}
	}
	else
	{
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <ServerId>!");
		return false;
	}

	// <MaxClients>
	pTxeElem = txhServer.FirstChild("MaxClients").Element(); 
	if (pTxeElem)
	{
		int nMaxClt = CUtility::ATOI(pTxeElem->GetText());
		if(nMaxClt < 1 || nMaxClt > DEF_MAXCLIENTS)
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n <MaxClients>%d</MaxClients> "
				"is out of the range(1-%d)!", nMaxClt, DEF_MAXCLIENTS);
			return false;
		}
		else
		{
			SetMaxClients(nMaxClt);
		}
	}
	else
	{
		SetMaxClients(DEF_MAXCLIENTS);
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <MaxClients>! so use default=%d", DEF_MAXCLIENTS);
	}

	// <SessionParam><HeartInterval>
	pTxeElem = txhServer.FirstChild("SessionParam").FirstChild("HeartInterval").Element(); 
	if (pTxeElem)
	{
		int nHeartInter = CUtility::ATOI(pTxeElem->GetText());
		if(nHeartInter < 1 || nHeartInter > 60)
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n <SessionParam><HeartInterval>%d</HeartInterval></SessionParam> "
				"is out of the range(1-60)!", nHeartInter);
			return false;
		}
		else
		{
			m_nHeartInterval = nHeartInter;
		}
	}
	else
	{
		m_nHeartInterval = DEF_HEARTINTERVAL;
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <SessionParam><HeartInterval>! "
			"so use default=%d", DEF_HEARTINTERVAL);
	}
	
	// <SessionParam><HeartTimeout>
	pTxeElem = txhServer.FirstChild("SessionParam").FirstChild("HeartTimeout").Element(); 
	if (pTxeElem)
	{
		int nHeartTimeout = CUtility::ATOI(pTxeElem->GetText());
		if(nHeartTimeout < 1 || nHeartTimeout > 180)
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n <SessionParam><HeartTimeout>%d</HeartTimeout></SessionParam> "
				"is out of the range(1-180)!", nHeartTimeout);
			return false;
		}
		else
		{
			m_nHeartTimeout = nHeartTimeout;
		}
	}
	else
	{
		m_nHeartTimeout = DEF_HEARTTIMEOUT;
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <SessionParam><HeartTimeout>! "
			"so use default=%d", DEF_HEARTTIMEOUT);
	}
	
	// <SessionParam><AckInterval>
	pTxeElem = txhServer.FirstChild("SessionParam").FirstChild("AckInterval").Element(); 
	if (pTxeElem)
	{
		int nAckInterval = CUtility::ATOI(pTxeElem->GetText());
		if(nAckInterval < 1 || nAckInterval > 60)
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n <SessionParam><AckInterval>%d</AckInterval></SessionParam> "
				"is out of the range(1-60)!", nAckInterval);
			return false;
		}
		else
		{
			m_nAckInterval = nAckInterval;
		}
	}
	else
	{
		m_nAckInterval = DEF_ACKINTERVAL;
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <SessionParam><AckInterval>! "
			"so use default=%d", DEF_ACKINTERVAL);
	}
	
	// <SessionParam><AckTimeout>
	pTxeElem = txhServer.FirstChild("SessionParam").FirstChild("AckTimeout").Element(); 
	if (pTxeElem)
	{
		int nAckTimeout = CUtility::ATOI(pTxeElem->GetText());
		if(nAckTimeout < 1 || nAckTimeout > 180)
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n <SessionParam><AckTimeout>%d</AckTimeout></SessionParam> "
				"is out of the range(1-180)!", nAckTimeout);
			return false;
		}
		else
		{
			m_nAckTimeout = nAckTimeout;
		}
	}
	else
	{
		m_nAckTimeout = DEF_ACKTIMEOUT;
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <SessionParam><AckTimeout>! "
			"so use default=%d", DEF_ACKTIMEOUT);
	}

	// <SessionParam><WindowSize>
	pTxeElem = txhServer.FirstChild("SessionParam").FirstChild("WindowSize").Element(); 
	if (pTxeElem)
	{
		int nWindowSize = CUtility::ATOI(pTxeElem->GetText());
		if(nWindowSize < 1 || nWindowSize > 255)
		{
			LogImportant("CServerForwardBase::InitCommParam(), \n <SessionParam><WindowSize>%d</WindowSize></SessionParam> "
				"is out of the range(1-180)!", nWindowSize);
			return false;
		}
		else
		{
			m_byWindowSize = (BYTE)nWindowSize;
		}
	}
	else
	{
		m_byWindowSize = DEF_COMMWINSIZE;
		LogImportant("CServerForwardBase::InitCommParam(), \n fail to read <SessionParam><WindowSize>! "
			"so use default=%d", DEF_COMMWINSIZE);
	}
	
	return true;
}

bool CServerForwardBase::RecvForwardData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort) 
{
	CProtocolForward::CForwardHead objForwardHead;
	return RecvForwardData(objForwardHead, szBuf, nBufLen, szCltIP, nCltPort);
}

bool CServerForwardBase::RecvForwardData(CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort) 
{  
	CProtocolForward::CForwardHead objForwardHead;
	return RecvForwardData(objForwardHead, pBuf, szCltIP, nCltPort);
}

bool CServerForwardBase::RecvForwardData(char* szBuf, int& nBufLen, CSid& objSid) 
{ 
	CProtocolForward::CForwardHead objForwardHead;
	return RecvForwardData(objForwardHead, szBuf, nBufLen, objSid);
}

bool CServerForwardBase::RecvForwardData(CBuffer* pBuf, CSid& objSid) 
{  
	CProtocolForward::CForwardHead objForwardHead;
	return RecvForwardData(objForwardHead, pBuf, objSid);
} 

bool CServerForwardBase::RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort) 
{
	CSid objSid;
	bool bRet = RecvForwardData(objForwardHead, szBuf, nBufLen, objSid);
	
	CServerBaseCltObj* pCltObj = GetClientObj(objSid);
	if (pCltObj)
	{
		memcpy(szCltIP, pCltObj->GetPeerIp().c_str(), pCltObj->GetPeerIp().size());
		nCltPort = static_cast<unsigned short>(pCltObj->GetPeerPort());
	}
	
	return bRet;
}

bool CServerForwardBase::RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort) 
{  
	CSid objSid;
	bool bRet = RecvForwardData(objForwardHead, pBuf, objSid);
	
	CServerBaseCltObj* pCltObj = GetClientObj(objSid);
	if (pCltObj)
	{
		memcpy(szCltIP, pCltObj->GetPeerIp().c_str(), pCltObj->GetPeerIp().size());
		nCltPort = static_cast<unsigned short>(pCltObj->GetPeerPort());
	}
	
	return bRet;
}

bool CServerForwardBase::RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen, CSid& objSid) 
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
				char* pForward = pBufInQueue->GetBuffer() + CProtocolSession::HeaderSize();
				if (objForwardHead.FromStreamBuf(pForward, nDataLen) > 0)
				{
					int nMsgBodyLen = nDataLen - objForwardHead.StreamSize();
					if (nMsgBodyLen > 0)
					{
						char* pMsgBody = pForward + objForwardHead.StreamSize();
						objSid.Import(pBufInQueue->GetWParam(), pBufInQueue->GetLParam());
						memcpy(szBuf, pMsgBody, nMsgBodyLen); 
						nBufLen = nMsgBodyLen; 
						bRet = true;  
					}
				}
			}
			
			pBufInQueue->Release();
		} 
	} 
	
	return bRet; 
} 
 
bool CServerForwardBase::RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, CSid& objSid) 
{  
	assert(pBuf);
	
	pBuf->Empty();
	
	bool bRet = false;
	CBuffer* pBufInQueue = NULL; 
	if (m_RecvQueue.GetHead(pBufInQueue)) 
	{
		int nDataLen = pBufInQueue->GetUsed() - CProtocolSession::HeaderSize();
		if (nDataLen > 0)
		{
			char* pForward = pBufInQueue->GetBuffer() + CProtocolSession::HeaderSize();
			if (objForwardHead.FromStreamBuf(pForward, nDataLen) > 0)
			{
				int nMsgBodyLen = nDataLen - objForwardHead.StreamSize();
				if (nMsgBodyLen > 0)
				{
					char* pMsgBody = pForward + objForwardHead.StreamSize();
					objSid.Import(pBufInQueue->GetWParam(), pBufInQueue->GetLParam());
					pBuf->AddData(pMsgBody, nMsgBodyLen);
					bRet = true;
				}
			}
		}
		
		pBufInQueue->Release();
	}
	
	return bRet; 
} 

bool CServerForwardBase::SendForwardData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort) 
{  
	CProtocolForward::CForwardHead objForwardHead;
	return SendForwardData(objForwardHead, pBuf, szCltIP, nCltPort);
} 

bool CServerForwardBase::SendForwardData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort) 
{  
	CProtocolForward::CForwardHead objForwardHead;
	return SendForwardData(objForwardHead, szBuf, nBufLen, szCltIP, nCltPort);
}

bool CServerForwardBase::SendForwardData(CBuffer* pBuf, CSid objSid) 
{  
	CProtocolForward::CForwardHead objForwardHead;
	return SendForwardData(objForwardHead, pBuf, objSid);
} 

bool CServerForwardBase::SendForwardData(char* szBuf, int nBufLen, CSid objSid) 
{  
	CProtocolForward::CForwardHead objForwardHead;
	return SendForwardData(objForwardHead, szBuf, nBufLen, objSid);
} 

bool CServerForwardBase::SendForwardData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj)
{
	CProtocolForward::CForwardHead objForwardHead;
	return SendForwardData(objForwardHead,  szBuf, nBufLen, pCltObj);
}

bool CServerForwardBase::SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort) 
{  
	return SendForwardData(objForwardHead, pBuf->GetBuffer(), pBuf->GetUsed(), szCltIP, nCltPort);
} 

bool CServerForwardBase::SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort) 
{  
	CServerBaseCltObj* pCltObj = GetClientObj(szCltIP, nCltPort); 
	if (!pCltObj) 
	{
		LogImportant("CServerForwardBase::SendForwardData(), \n server(%s) \n send error! can not find client for ip_port{%s:%d}", 
			GetTagMyInfo().c_str(), szCltIP, nCltPort); 
		return false; 
	}
	
	return SendForwardData(objForwardHead, szBuf, nBufLen, pCltObj);
}

bool CServerForwardBase::SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, CSid objSid) 
{  
	return SendForwardData(objForwardHead, pBuf->GetBuffer(), pBuf->GetUsed(), objSid);
} 

bool CServerForwardBase::SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, CSid objSid) 
{  
	CServerBaseCltObj* pCltObj = GetClientObj(objSid); 
	if (!pCltObj) 
	{
		LogImportant("CServerForwardBase::SendForwardData(), \n server(%s) \n send error! can not find client for objSid{%d,%d}", 
			GetTagMyInfo().c_str(), objSid.GetHigh(), objSid.GetLow()); 
		return false; 
	}
	
	return SendForwardData(objForwardHead, szBuf, nBufLen, pCltObj);
} 

bool CServerForwardBase::SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj)
{
	assert(pCltObj);
	assert(IsMyClientObj(pCltObj));

	if (!pCltObj->IsConnect()) 
	{ 
		LogImportant("CServerForwardBase::SendForwardData(), \n connection(%s) \n it is disconnect, can not send data!",
			pCltObj->GetTagConnInfo().c_str()); 
		return false; 
	} 
	
	if (nBufLen <= 0)
	{
		LogImportant("CServerForwardBase::SendForwardData(), \n connection(%s) \n buf_len(%d) is error!", 
			pCltObj->GetTagConnInfo().c_str(), nBufLen); 
		return false; 
	}
	
	int nMsgBodyLen = nBufLen;
	int nDataLen = objForwardHead.StreamSize() + nMsgBodyLen;
	int nNeedLen = CProtocolSession::HeaderSize() + nDataLen;
	if (nNeedLen > GetMaxPacketSize())
	{ 
		LogImportant("CServerForwardBase::SendForwardData(), \n connection(%s) \n pkt need len(%d) is larger than max(%d)!",
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
	
	// 转发层协议头打包
	objForwardHead.SetMsgBodyLen(nMsgBodyLen);
	int nBufLeft = pBuf->GetAvailableSize();
	int nForwardHeadLen = objForwardHead.ToStreamBuf(pBuf->GetBuffer() + CProtocolSession::HeaderSize(), nBufLeft);
	if (nForwardHeadLen <= 0)
	{
		pBuf->Release();
		return false;
	}
	pBuf->Use(nForwardHeadLen);
	
	// 消息体打包
	pBuf->AddData(szBuf, nBufLen);
	
	// 对于业务程序服务端模块，需要对即将发送的数据设置源地址、类型码功能码信息，并进行严格检查
	if (m_bIsAppServer) 
	{
		char* pForward = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
		int nLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
		if (!SetSrcAddrForSendData(pForward, nLen))
		{
		}
		
		if (!SetTypeFuncForSendData(pForward, nLen))
		{
		}
		
		if (!CheckSendData(pForward, nLen))
		{
			LogImportant("CServerForwardBase::SendForwardData(), \n connection(%s) \n check data fail, can not send data!",
				pCltObj->GetTagConnInfo().c_str());
			pBuf->Release();
			return false;
		}
	}	
	
	// 添加到发送队列	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	pCltObj->PushSendData(pBuf);
	
	return true; 
}

//==============================================================================
//                 
//                    CServerForwardBaseCltObj
//           转发层客户端基类的客户端对象类（带双网、转发条件机制）
//
//==============================================================================
CServerForwardBaseCltObj::CServerForwardBaseCltObj(CServerForwardBase* pServer) :
	CServerSessionBaseCltObj(pServer)
{
	SetMyClassName("CServerForwardBaseCltObj");
}

CServerForwardBaseCltObj::~CServerForwardBaseCltObj()
{
}

bool CServerForwardBaseCltObj::HandleRegisterPkt(char* szBuf, int nBufLen)
{
	LogImportant("CServerForwardBaseCltObj::HandleRegisterPkt(), \n connection(%s) \n handle register pkt!",
		GetTagConnInfo().c_str());
	
	string strXmlRegStr = CProtocolForward::GetXmlRegStr(szBuf, nBufLen);
	if (strXmlRegStr.size() > 0)
	{
		if (m_objForwardCondition.Init(strXmlRegStr.c_str()))
		{
			LogImportant("CServerForwardBaseCltObj::HandleRegisterPkt(), \n connection(%s) \n parse register pkt ok!\n %s", 
				GetTagConnInfo().c_str(), m_objForwardCondition.GetDumpStr().c_str());
		}
		else
		{
			LogImportant("CServerForwardBaseCltObj::HandleRegisterPkt(), \n connection(%s) \n parse xml_reg_str fail! xml_reg_str=%s",
				GetTagConnInfo().c_str(), strXmlRegStr.c_str());
			return false;
		}
	}
	else
	{
		LogImportant("CServerForwardBaseCltObj::HandleRegisterPkt(), \n connection(%s) \n parse register pkt fail! xml_reg_strlen=0",
			GetTagConnInfo().c_str());
		return false;
	}

	return true;
}

bool CServerForwardBaseCltObj::IsRegistered(CProtocolForward::CForwardHead& objForwardHead)
{
	return m_objForwardCondition.IsMatched(objForwardHead);
}

void CServerForwardBaseCltObj::ClearRegisterInfo()
{
	m_objForwardCondition.Clear();
}

CForwardCondition& CServerForwardBaseCltObj::GetForwardCondition()
{
	return m_objForwardCondition;
}


