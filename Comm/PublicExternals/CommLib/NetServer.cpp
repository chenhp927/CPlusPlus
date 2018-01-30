//==============================================================================
//                      NetServer.cpp
//                       
//begin   : 2016-09-08                                                 
//describe:                                              
//==============================================================================

#include "NetServer.h"

//==============================================================================
//                 
//                    CServerOneNet
//                    一个网服务端类
//
//==============================================================================
CServerOneNet::CServerOneNet(CNetServerDouble* pParent) : m_pParent(pParent)
{
	SetMyClassName("CServerOneNet");
}

CServerOneNet::~CServerOneNet()
{
	
}

void CServerOneNet::OnSessionConnect(CSid objSid)
{
	CServerForwardBase::OnSessionConnect(objSid);

	if (m_pParent)
	{
		CServerOneNetCltObj* pCltObj = (CServerOneNetCltObj*)GetClientObj(objSid);
		assert(pCltObj);
		assert(pCltObj->GetRelatedServerPtr() == this);
		assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));
		
		m_pParent->OnSessionConnect(pCltObj);
	}
}

void CServerOneNet::OnSessionReady(CSid objSid)
{
	CServerForwardBase::OnSessionReady(objSid);
	
	if (m_pParent)
	{
		CServerOneNetCltObj* pCltObj = (CServerOneNetCltObj*)GetClientObj(objSid);
		assert(pCltObj);
		assert(pCltObj->GetRelatedServerPtr() == this);
		assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));

		m_pParent->OnSessionReady(pCltObj);
	}
}

void CServerOneNet::OnSessionClose(CSid objSid)
{
	if (m_pParent)
	{
		CServerOneNetCltObj* pCltObj = (CServerOneNetCltObj*)GetClientObj(objSid);
		assert(pCltObj);
		assert(pCltObj->GetRelatedServerPtr() == this);
		assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));
		
		m_pParent->OnSessionClose(pCltObj);
	}
	
	CServerForwardBase::OnSessionClose(objSid);
}

void CServerOneNet::HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf)
{
	assert(pCltObj && pBuf);
	assert(((CServerOneNetCltObj*)pCltObj)->GetRelatedServerPtr() == this);
	
	if (m_pParent)
	{
		assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));
		m_pParent->OnRecvPacket((CServerOneNetCltObj*)pCltObj, pBuf);
	}
}

CServerBaseCltObj* CServerOneNet::CreateClientObject()
{
	CServerOneNetCltObj* pCltObj = new CServerOneNetCltObj(this);
	if (pCltObj)
	{
		pCltObj->SetLogFileTag(GetLogFileTag());
	}
	
	return pCltObj;
}

std::string CServerOneNet::GetServerIdValidRangeStr()
{
	return m_pParent->GetServerIdValidRangeStr(IsAppServer());
}

bool CServerOneNet::IsServerIdValid()
{
	return m_pParent->IsServerIdValid(IsAppServer(), GetServerId());
}

std::string CServerOneNet::GetCliendIdValidRangeStr()
{
	return m_pParent->GetCliendIdValidRangeStr();
}

bool CServerOneNet::IsClientIdValid( CServerSessionBaseCltObj* pCltObj )
{
	return m_pParent->IsClientIdValid(pCltObj->IsAppClient(), pCltObj->GetClientId());
}

//==============================================================================
//                 
//                    CServerOneNetCltObj
//                  一个网服务端的客户端对象类
//
//==============================================================================
CServerOneNetCltObj::CServerOneNetCltObj(CServerOneNet* pServer) : 
	CServerForwardBaseCltObj(pServer)
{
	SetMyClassName("CServerOneNetCltObj");
}

CServerOneNetCltObj::~CServerOneNetCltObj()
{
}

//==============================================================================
//                 
//                   CNetClientDouble
//                    双网客户端类
//
//==============================================================================
CNetServerDouble::CNetServerDouble()
{
	SetMyClassName("CNetServerDouble");

	m_pServer1 = new CServerOneNet(this);
	m_pServer2 = new CServerOneNet(this);
	assert(m_pServer1 && m_pServer2);

	m_pServer1->SetLogFileTag(GetLogFileTag());
	m_pServer2->SetLogFileTag(GetLogFileTag());
}

CNetServerDouble::~CNetServerDouble()
{
	if (m_pServer1)
	{
		delete m_pServer1;
	}
	
	if (m_pServer2)
	{
		delete m_pServer2;
	}	
}

bool CNetServerDouble::Create()
{
	int nMaxSize = m_pServer1->GetMaxClients();
	assert(nMaxSize > 0);

	m_pServer1->Create();

	if (!IsRunAsSingleNet())
	{
		assert(m_pServer2->GetMaxClients() == nMaxSize);
		m_pServer2->Create();
	}
	
	nMaxSize = m_pServer1->GetMaxClients();
	for (int x = 0; x < nMaxSize; x++)
	{
		CServerOneNetCltObj* pCltObj = (CServerOneNetCltObj*)m_pServer1->GetClientObjByIndex(x);
		assert(pCltObj);
		assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));

		if (!IsRunAsSingleNet())
		{
			pCltObj = (CServerOneNetCltObj*)m_pServer2->GetClientObjByIndex(x);
			assert(pCltObj);
			assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));
		}
	}
	
	m_vecNetServerDoubleCltObj.resize(nMaxSize);
	for (int y = 0; y < m_vecNetServerDoubleCltObj.size(); y++)
	{
		m_vecNetServerDoubleCltObj[y].SetLogFileTag(GetLogFileTag());
	}

	return true;
}

bool CNetServerDouble::IsRunAsSingleNet()
{
	return m_pServer2->IsNotCfged();
}

bool CNetServerDouble::InitCommParam(TiXmlHandle& txhServer)
{
	bool bInit1 = m_pServer1->InitCommParam(txhServer, 1);
	bool bInit2 = m_pServer2->InitCommParam(txhServer, 2);

	if (bInit1) // 第二个网络的配置允许有误
	{
		Create();
	}

	return bInit1; 
}

bool CNetServerDouble::Startup()
{
	bool bRet = false;
	if (IsRunAsSingleNet())
	{
		bRet = m_pServer1->Startup();
	}
	else
	{
		bRet = m_pServer1->Startup();
		m_pServer2->Startup();
	}

	return bRet;
}

void CNetServerDouble::OnSessionConnect(CServerOneNetCltObj* pCltObj)
{
	assert(pCltObj && pCltObj->GetRelatedServerPtr());

	LogDebug("CNetServerDouble::OnSessionConnect(), \n connection(%s) \n it is connecting!", 
		pCltObj->GetTagConnInfo().c_str());
}

void CNetServerDouble::OnSessionReady(CServerOneNetCltObj* pCltObj)
{
	assert(pCltObj && pCltObj->IsReady());

	// 先查找配对的客户端对象，进行绑定
	bool bCltObjExist = false;
	bool bCltObjBind = false;
	CNetServerDoubleCltObj* pNetServerDoubleCltObj = NULL;
	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		pNetServerDoubleCltObj = &m_vecNetServerDoubleCltObj[x];

		if (pCltObj->IsPairWith(pNetServerDoubleCltObj->GetRelatedCltObjPtr1()) 
			|| pCltObj->IsPairWith(pNetServerDoubleCltObj->GetRelatedCltObjPtr2()))
		{
			if (pNetServerDoubleCltObj->AddCltObj(pCltObj))
			{
				bCltObjBind = true;
			}
			else
			{
				bCltObjExist = true;
			}
			
			break;
		}
	}

	// 虽然找到了配对的客户端对象，但是自己已经存在于双网客户端对象中，进行出错处理，关闭该连接
	if (bCltObjExist)
	{
		LogImportant("CNetServerDouble::OnSessionReady(), \n connection(%s) \n it has existed in double client object, "
			"it is error, so will close client socket ...", pCltObj->GetTagConnInfo().c_str());

		// 添加失败，此时应该关闭该客户端
		pCltObj->GetRelatedServerPtr()->CloseClientObj(pCltObj);
	}
	// 找到了配对的客户端对象，并绑定成功
	else if (bCltObjBind)
	{
		assert(pNetServerDoubleCltObj);
		assert(pNetServerDoubleCltObj->GetRelatedCltObjPtr1());
		assert(pNetServerDoubleCltObj->GetRelatedCltObjPtr2());

		LogImportant("CNetServerDouble::OnSessionReady(), \n connection(%s) \n and \n connection(%s) \n they are double client object. make pair ok!", 
			pNetServerDoubleCltObj->GetRelatedCltObjPtr1()->GetTagConnInfo().c_str(),
			pNetServerDoubleCltObj->GetRelatedCltObjPtr2()->GetTagConnInfo().c_str());
	}
	// 找不到配对的，就添加到一个空的双网客户端对象中
	else
	{
		bool bCltObjAdd = false;
		for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
		{
			CNetServerDoubleCltObj* pNetServerDoubleCltObj = &m_vecNetServerDoubleCltObj[x];
			if (pNetServerDoubleCltObj->IsEmpty())
			{
				LogImportant("CNetServerDouble::OnSessionReady(), \n connection(%s) \n it is added in an empty double client object. waiting for make pair ...", 
					pCltObj->GetTagConnInfo().c_str());

				pNetServerDoubleCltObj->SetRelatedCltObjPtr1(pCltObj);
				bCltObjAdd = true;
				break;
			}
		}

		// 添加失败，此时应该关闭该客户端
		if (!bCltObjAdd)
		{
			LogImportant("CNetServerDouble::OnSessionReady(), \n connection(%s) \n fail to be added in an empty double client object, so will close client socket ...", 
				pCltObj->GetTagConnInfo().c_str());
			
			pCltObj->GetRelatedServerPtr()->CloseClientObj(pCltObj);
		}
	}
}

void CNetServerDouble::OnSessionClose(CServerOneNetCltObj* pCltObj)
{
	assert(pCltObj);

	// 客户端对象关闭时，清除注册信息
	pCltObj->ClearRegisterInfo();
	
	// 客户端对象关闭时，将其从所在的双网客户端对象删除
	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		CNetServerDoubleCltObj* pNetServerDoubleCltObj = &m_vecNetServerDoubleCltObj[x];
		if (pNetServerDoubleCltObj->RemoveCltObj(pCltObj))
		{
			break;;
		}
	}
}

void CNetServerDouble::SetFdset(CFdset& objFs)
{
	if (IsRunAsSingleNet())
	{
		m_pServer1->SetFdset(objFs);
	}
	else
	{
		m_pServer1->SetFdset(objFs);
		m_pServer2->SetFdset(objFs);
	}
}

void CNetServerDouble::SetListenFdset(CFdset& objFs)
{
	if (IsRunAsSingleNet())
	{
		m_pServer1->SetListenFdset(objFs);
	}
	else
	{
		m_pServer1->SetListenFdset(objFs);
		m_pServer2->SetListenFdset(objFs);
	}
}

void CNetServerDouble::SetClientFdset(CFdset& objFs)
{
	if (IsRunAsSingleNet())
	{
		m_pServer1->SetClientFdset(objFs);
	}
	else
	{
		m_pServer1->SetClientFdset(objFs);
		m_pServer2->SetClientFdset(objFs);
	}
}

void CNetServerDouble::SetClientFdset(CFdset& objFs, vector<int>& vecCltIndex)
{
	vector<CServerBaseCltObj*> vecCltObj1, vecCltObj2;
	ConvertCltObjs(vecCltIndex, vecCltObj1, vecCltObj2);

	if (IsRunAsSingleNet())
	{
		m_pServer1->SetClientFdset(objFs, vecCltObj1);
	}
	else
	{
		m_pServer1->SetClientFdset(objFs, vecCltObj1);
		m_pServer2->SetClientFdset(objFs, vecCltObj2);
	}
}

void CNetServerDouble::HandleNetEvent(CFdset& objFs)
{
	if (IsRunAsSingleNet())
	{
		m_pServer1->HandleNetEvent(objFs);
	}
	else
	{
		m_pServer1->HandleNetEvent(objFs);
		m_pServer2->HandleNetEvent(objFs);
	}
}

void CNetServerDouble::HandleListenNetEvent(CFdset& objFs)
{
	if (IsRunAsSingleNet())
	{
		m_pServer1->HandleListenNetEvent(objFs);
	}
	else
	{
		m_pServer1->HandleListenNetEvent(objFs);
		m_pServer2->HandleListenNetEvent(objFs);
	}
}

void CNetServerDouble::HandleClientNetEvent(CFdset& objFs)
{
	if (IsRunAsSingleNet())
	{
		m_pServer1->HandleClientNetEvent(objFs);
	}
	else
	{
		m_pServer1->HandleClientNetEvent(objFs);
		m_pServer2->HandleClientNetEvent(objFs);
	}
}

void CNetServerDouble::HandleClientNetEvent(CFdset& objFs, vector<int>& vecCltIndex)
{
	vector<CServerBaseCltObj*> vecCltObj1, vecCltObj2;
	ConvertCltObjs(vecCltIndex, vecCltObj1, vecCltObj2);
	
	if (IsRunAsSingleNet())
	{
		m_pServer1->HandleClientNetEvent(objFs, vecCltObj1);
	}
	else
	{
		m_pServer1->HandleClientNetEvent(objFs, vecCltObj1);
		m_pServer2->HandleClientNetEvent(objFs, vecCltObj2);
	}
}

void CNetServerDouble::DoPatrol()
{
	if (IsRunAsSingleNet())
	{
		m_pServer1->DoPatrol();
	}
	else
	{
		m_pServer1->DoPatrol();
		m_pServer2->DoPatrol();
	}
}

void CNetServerDouble::DoPatrol(vector<int>& vecCltIndex)
{
	vector<CServerBaseCltObj*> vecCltObj1, vecCltObj2;
	ConvertCltObjs(vecCltIndex, vecCltObj1, vecCltObj2);
	
	if (IsRunAsSingleNet())
	{
		m_pServer1->DoPatrol(vecCltObj1);
	}
	else
	{
		m_pServer1->DoPatrol(vecCltObj1);
		m_pServer2->DoPatrol(vecCltObj2);
	}
}

void CNetServerDouble::ConvertCltObjs(vector<int>& vecCltIndex, vector<CServerBaseCltObj*>& vecCltObj1, vector<CServerBaseCltObj*>& vecCltObj2)
{
	for (int x = 0; x < vecCltIndex.size(); x++)
	{
		CServerOneNetCltObj* pCltObj = (CServerOneNetCltObj*)m_pServer1->GetClientObjByIndex(vecCltIndex[x]);
		assert(pCltObj);
		assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));

		vecCltObj1.push_back(pCltObj);

		if (!IsRunAsSingleNet())
		{
			pCltObj = (CServerOneNetCltObj*)m_pServer2->GetClientObjByIndex(vecCltIndex[x]);
			assert(pCltObj);
			assert(pCltObj->IsMyClassName("CServerOneNetCltObj"));
			
			vecCltObj2.push_back(pCltObj);
		}
	}
}

void CNetServerDouble::OnRecvPacket(CServerOneNetCltObj* pCltObj, CBuffer* pBuf)
{
	assert(pCltObj);

	CServerOneNet* pSvr = (CServerOneNet*)pCltObj->GetRelatedServerPtr();
	assert(pSvr);
	assert(pSvr->IsMyClassName("CServerOneNet"));

	// 获取转发层信息位置
	int nForwardDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nForwardDataLen < 0)
	{
		// 如果某个客户端的会话层数据长度有问题，那么该连接有问题，关闭该连接
		LogImportant("CNetServerDouble::OnRecvPacket(), \n connection(%s) \n receive one pkt, but it is not enough to get session head. so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str());
		
		pSvr->CloseClientObj(pCltObj);
		pBuf->Release(); 
		return;
	}
	char* pForward = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
	
	// 注册包
	if (CProtocolForward::IsMsgRegisterPkt(pForward, nForwardDataLen))
	{
		HandleRegisterPkt(pCltObj, pBuf);
	}
	// 转发包
	else if (CProtocolForward::IsMsgForwardPkt(pForward, nForwardDataLen))
	{
		HandleForwardPkt(pCltObj, pBuf);
	}
	// 未知包
	else
	{
		// 未知包，丢弃
		LogImportant("CNetServerDouble::OnRecvPacket(), \n connection(%s) \n receive one pkt, but it is unknown pkt.", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	pBuf->Release();
}

void CNetServerDouble::HandleRegisterPkt(CServerOneNetCltObj* pCltObj, CBuffer* pBuf)
{
	assert(pCltObj);
	assert(pBuf->GetUsed() > CProtocolSession::HeaderSize());

	LogImportant("CNetServerDouble::HandleRegisterPkt(), \n connection(%s) \n receive one register pkt!", 
		pCltObj->GetTagConnInfo().c_str());
	LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	
	char* pRegBuf = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
	int nRegBufLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	assert(CProtocolForward::IsMsgRegisterPkt(pRegBuf, nRegBufLen));

	CNetServerDoubleCltObj* pNetServerDoubleCltObj = GetNetServerDoubleCltObj(pCltObj);
	assert(pNetServerDoubleCltObj); // 不合格的客户连接早在协商过程中就应该被关闭掉，没有机会发送应用数据

	// 注册包的处理，双网接收的冗余注册包都要处理，以免丢失
	// 此时，需要将双网里的两个单网客户端注册弄成一样
	// 避免将来选择的时候两个单网连接的注册不一致会有问题
	CServerOneNetCltObj* pCltObj1 = pNetServerDoubleCltObj->GetRelatedCltObjPtr1();
	if (pCltObj1)
	{
		if (pCltObj1->HandleRegisterPkt(pRegBuf, nRegBufLen))
		{
			LogImportant("CNetServerDouble::HandleRegisterPkt(), \n connection(%s) \n deal register pkt ok!", 
				pCltObj1->GetTagConnInfo().c_str());
		}
		else
		{
			LogImportant("CNetServerDouble::HandleRegisterPkt(), \n connection(%s) \n deal register pkt fail! so will close client socket ...", 
				pCltObj1->GetTagConnInfo().c_str());
			
			CServerOneNet* pSvr1 = (CServerOneNet*)pCltObj1->GetRelatedServerPtr();
			assert(pSvr1);
			assert(pSvr1->IsMyClassName("CServerOneNet"));
			
			pSvr1->CloseClientObj(pCltObj1);
		}
	}
				
	CServerOneNetCltObj* pCltObj2 = pNetServerDoubleCltObj->GetRelatedCltObjPtr2();
	if (pCltObj2)
	{
		if (pCltObj2->HandleRegisterPkt(pRegBuf, nRegBufLen))
		{
			LogImportant("CNetServerDouble::OnRecvPacket(), \n connection(%s) \n deal register pkt ok!", 
				pCltObj2->GetTagConnInfo().c_str());
		}
		else
		{
			LogImportant("CNetServerDouble::OnRecvPacket(), \n connection(%s) \n deal register pkt fail! so will close client socket ...", 
				pCltObj2->GetTagConnInfo().c_str());
			
			CServerOneNet* pSvr2 = (CServerOneNet*)pCltObj2->GetRelatedServerPtr();
			assert(pSvr2);
			assert(pSvr2->IsMyClassName("CServerOneNet"));
			
			pSvr2->CloseClientObj(pCltObj2);
		}
	}
}

void CNetServerDouble::HandleForwardPkt(CServerOneNetCltObj* pCltObj, CBuffer* pBuf)
{
	assert(pCltObj);

	CNetServerDoubleCltObj* pNetServerDoubleCltObj = GetNetServerDoubleCltObj(pCltObj);
	assert(pNetServerDoubleCltObj); // 不合格的客户连接早在协商过程中就应该被关闭掉，没有机会发送应用数据

	// 处理双网序号
 	bool bRet = pNetServerDoubleCltObj->DealRecvPacketDataSeq(pCltObj, pBuf);
 
 	if (bRet)
	{
		OnDealRecvData(pCltObj, pBuf);
	}
}

CNetServerDoubleCltObj* CNetServerDouble::GetNetServerDoubleCltObj(CServerOneNetCltObj* pCltObj)
{
	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		CNetServerDoubleCltObj& objCltObj = m_vecNetServerDoubleCltObj[x];
		if (objCltObj.IsEmpty())
		{
			continue;
		}
		else if (objCltObj.GetRelatedCltObjPtr1() == pCltObj 
			|| objCltObj.GetRelatedCltObjPtr2() == pCltObj)
		{
			return &objCltObj;
		}
	}

	return NULL;
}

bool CNetServerDouble::SendData( const char* szCltIP, unsigned short nCltPort, char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr )
{
	bool bRet = false;
	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		CNetServerDoubleCltObj& objCltObj = m_vecNetServerDoubleCltObj[x];
		if (objCltObj.IsEmpty())
		{
			continue;
		}

		CServerOneNetCltObj* pCltObj1 = objCltObj.GetRelatedCltObjPtr1();
		CServerOneNetCltObj* pCltObj2 = objCltObj.GetRelatedCltObjPtr2();

		if ((pCltObj1 && strcmp(pCltObj1->GetPeerIp().c_str(), szCltIP) == 0 && pCltObj1->GetPeerPort() == nCltPort)
			|| (pCltObj2 && strcmp(pCltObj2->GetPeerIp().c_str(), szCltIP) == 0 && pCltObj2->GetPeerPort() == nCltPort))
		{
			objCltObj.SendData(szBuf, nBufLen, vecDestAddr);
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool CNetServerDouble::SendData( const char* szCltIP, unsigned short nCltPort, CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr )
{
	return SendData(szCltIP, nCltPort, pBuf->GetBuffer(), pBuf->GetUsed(), vecDestAddr);
}

bool CNetServerDouble::SendData( const char* szCltIP, unsigned short nCltPort, char* szBuf, int nBufLen )
{
	vector<CProtocolForward::CForwardAddr> vecDestAddr;
	return SendData(szCltIP, nCltPort, szBuf, nBufLen, vecDestAddr);
}

bool CNetServerDouble::SendData( const char* szCltIP, unsigned short nCltPort, CBuffer* pBuf )
{
	vector<CProtocolForward::CForwardAddr> vecDestAddr;
	return SendData(szCltIP, nCltPort, pBuf, vecDestAddr);
}

std::string CNetServerDouble::GetServerIdValidRangeStr(bool bIsAppSvr)
{
	return "";
}

bool CNetServerDouble::IsServerIdValid(bool bIsAppSvr, WORD wSvrId)
{
	return false;
}

std::string CNetServerDouble::GetCliendIdValidRangeStr()
{
	return "";
}

bool CNetServerDouble::IsClientIdValid(bool bIsAppClt, WORD wCltId)
{
	return false;
}

string CNetServerDouble::GetDumpParamStr()
{
	char szDump[1024];
	memset(szDump, 0, sizeof(szDump));
	
	snprintf(szDump, sizeof(szDump), "double server1 info:\n%s\ndouble server2 info:\n%s", m_pServer1->GetDumpParamStr().c_str(), m_pServer2->GetDumpParamStr().c_str());
	
	return szDump;
}

void CNetServerDouble::DumpParam()
{
	LogImportant("CNetServerDouble::DumpParam(),\n%s", GetDumpParamStr().c_str());
}

void CNetServerDouble::SetLogFileTag( const char* szFileTag )
{
	CObjectBase::SetLogFileTag(szFileTag);

	m_pServer1->SetLogFileTag(szFileTag);
	m_pServer2->SetLogFileTag(szFileTag);

	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		m_vecNetServerDoubleCltObj[x].SetLogFileTag(szFileTag);
	}
}

int CNetServerDouble::GetType()
{
	if (m_pServer1)
	{
		return m_pServer1->GetType();
	}
	else if (m_pServer2)
	{
		return m_pServer2->GetType();
	}

	return 0;
}

void CNetServerDouble::DumpAllocator()
{
// 	if (m_pServer1)
// 	{
// 		LogImportant("CNetServerDouble::DumpAllocator(), dump Server1 m_objBufAlloc");
// 		m_pServer1->DumpAllocator();
// 	}
// 	
// 	if (m_pServer2)
// 	{
// 		LogImportant("CNetServerDouble::DumpAllocator(), dump Server2 m_objBufAlloc");
// 		m_pServer2->DumpAllocator();
// 	}
}

//==============================================================================
//                 
//                   CNetServerDoubleCltObj
//                 双网服务端的双网客户端对象类
//
//==============================================================================
CNetServerDoubleCltObj::CNetServerDoubleCltObj()
{
	SetMyClassName("CNetServerDoubleCltObj");

	m_pRelatedCltObjPtr1 = NULL;
	m_pRelatedCltObjPtr2 = NULL;
}

CNetServerDoubleCltObj::~CNetServerDoubleCltObj()
{

}

CServerOneNetCltObj* CNetServerDoubleCltObj::GetRelatedCltObjPtr1()
{
	return m_pRelatedCltObjPtr1;
}

void CNetServerDoubleCltObj::SetRelatedCltObjPtr1(CServerOneNetCltObj* p)
{
	m_pRelatedCltObjPtr1 = p;
}

CServerOneNetCltObj* CNetServerDoubleCltObj::GetRelatedCltObjPtr2()
{
	return m_pRelatedCltObjPtr2;
}

void CNetServerDoubleCltObj::SetRelatedCltObjPtr2(CServerOneNetCltObj* p)
{
	m_pRelatedCltObjPtr2 = p;
}

int CNetServerDoubleCltObj::GetClientId(char cNetNo)
{
	if (cNetNo == 1 && m_pRelatedCltObjPtr1)
	{
		return m_pRelatedCltObjPtr1->GetClientId();
	}
	else if (cNetNo == 2 && m_pRelatedCltObjPtr2)
	{
		return m_pRelatedCltObjPtr2->GetClientId();
	}

	return 0;
}

string CNetServerDoubleCltObj::GetPeerIp(char cNetNo)
{
	if (cNetNo == 1 && m_pRelatedCltObjPtr1)
	{
		return m_pRelatedCltObjPtr1->GetPeerIp();
	}
	else if (cNetNo == 2 && m_pRelatedCltObjPtr2)
	{
		return m_pRelatedCltObjPtr2->GetPeerIp();
	}

	return "";
}

int CNetServerDoubleCltObj::GetPeerPort(char cNetNo)
{
	if (cNetNo == 1 && m_pRelatedCltObjPtr1)
	{
		return m_pRelatedCltObjPtr1->GetPeerPort();
	}
	else if (cNetNo == 2 && m_pRelatedCltObjPtr2)
	{
		return m_pRelatedCltObjPtr2->GetPeerPort();
	}

	return 0;
}

CCommAddr* CNetServerDoubleCltObj::GetClientAddr(char cNetNo)
{
	if (cNetNo == 1 && m_pRelatedCltObjPtr1)
	{
		return &m_pRelatedCltObjPtr1->GetClientAddr();
	}
	else if (cNetNo == 2 && m_pRelatedCltObjPtr2)
	{
		return &m_pRelatedCltObjPtr2->GetClientAddr();
	}
	else if (cNetNo == -1)
	{
		if (m_pRelatedCltObjPtr1)
		{
			return &m_pRelatedCltObjPtr1->GetClientAddr();
		}
		else if (m_pRelatedCltObjPtr2)
		{
			return &m_pRelatedCltObjPtr2->GetClientAddr();
		}
	}
	
	return NULL;
}

string CNetServerDoubleCltObj::GetTagCltInfo(char cNetNo)
{
	if (cNetNo == 1)
	{
		if (m_pRelatedCltObjPtr1)
		{
			return m_pRelatedCltObjPtr1->GetTagCltInfo();
		}
		else
		{
			return "";
		}
	}
	else if (cNetNo == 2)
	{
		if (m_pRelatedCltObjPtr2)
		{
			return m_pRelatedCltObjPtr2->GetTagCltInfo();
		}
		else
		{
			return "";
		}
	}
	else
	{
		string str = "net1: ";
		if (m_pRelatedCltObjPtr1)
		{
			str += m_pRelatedCltObjPtr1->GetTagCltInfo();
		}
		
		str += "\t\t net2: ";
		
		if (m_pRelatedCltObjPtr2)
		{
			str += m_pRelatedCltObjPtr2->GetTagCltInfo();
		}
		
		return str;
	}
}

string CNetServerDoubleCltObj::GetTagConnInfo(char cNetNo)
{
	if (cNetNo == 1)
	{
		if (m_pRelatedCltObjPtr1)
		{
			return m_pRelatedCltObjPtr1->GetTagConnInfo();
		}
		else
		{
			return "";
		}
	}
	else if (cNetNo == 2)
	{
		if (m_pRelatedCltObjPtr2)
		{
			return m_pRelatedCltObjPtr2->GetTagConnInfo();
		}
		else
		{
			return "";
		}
	}
	else
	{
		string str = "net1: ";
		if (m_pRelatedCltObjPtr1)
		{
			str += m_pRelatedCltObjPtr1->GetTagConnInfo();
		}
		
		str += "\t\t net2: ";
		
		if (m_pRelatedCltObjPtr2)
		{
			str += m_pRelatedCltObjPtr2->GetTagConnInfo();
		}
		
		return str;
	}
}

bool CNetServerDoubleCltObj::IsReady(char cNetNo)
{
	bool bReady1 = false;
	if (m_pRelatedCltObjPtr1)
	{
		bReady1 = m_pRelatedCltObjPtr1->IsReady();
	}
	
	bool bReady2 = false;
	if (m_pRelatedCltObjPtr2)
	{
		bReady2 = m_pRelatedCltObjPtr2->IsReady();
	}
	
	if (cNetNo == 1)
	{
		return bReady1;
	}
	else if (cNetNo == 2)
	{
		return bReady2;
	}
	else
	{
		return bReady1 ||  bReady2;
	}
}

CForwardCondition* CNetServerDoubleCltObj::GetForwardCondition(char cNetNo)
{
	if (cNetNo == 1 && m_pRelatedCltObjPtr1)
	{
		return &m_pRelatedCltObjPtr1->GetForwardCondition();
	}
	else if (cNetNo == 2 && m_pRelatedCltObjPtr2)
	{
		return &m_pRelatedCltObjPtr2->GetForwardCondition();
	}
	else if (cNetNo == -1)
	{
		if (m_pRelatedCltObjPtr1)
		{
			return &m_pRelatedCltObjPtr1->GetForwardCondition();
		}
		else if (m_pRelatedCltObjPtr2)
		{
			return &m_pRelatedCltObjPtr2->GetForwardCondition();
		}
	}
	
	return NULL;
}

bool CNetServerDoubleCltObj::IsRegistered(CProtocolForward::CForwardHead& objForwardHead)
{
	if (m_pRelatedCltObjPtr1 && m_pRelatedCltObjPtr1->IsRegistered(objForwardHead))
	{
		return true;
	}

	if (m_pRelatedCltObjPtr2 && m_pRelatedCltObjPtr2->IsRegistered(objForwardHead))
	{
		return true;
	}

	return false;
}

bool CNetServerDoubleCltObj::IsEmpty()
{
	return m_pRelatedCltObjPtr1 == NULL && m_pRelatedCltObjPtr2 == NULL;
}

bool CNetServerDoubleCltObj::IsFull()
{
	return m_pRelatedCltObjPtr1!= NULL && m_pRelatedCltObjPtr2 != NULL;
}

bool CNetServerDoubleCltObj::AddCltObj(CServerOneNetCltObj* pCltObj)
{
	// 能进入本接口，则说明新的连接与本双网有关，所以如果有一个连接是空的，那么直接添加即可
	if (m_pRelatedCltObjPtr1 == NULL)
	{
		m_pRelatedCltObjPtr1 = pCltObj;
		assert(!m_objRecvSeq1.IsValid());

		return true;
	}
	else if (m_pRelatedCltObjPtr2 == NULL)
	{
		m_pRelatedCltObjPtr2 = pCltObj;
		assert(!m_objRecvSeq2.IsValid());

		return true;
	}
	
	// 如果没有连接是空的，则说明有一个连接需要被踢掉（客户端采用随机端口时可能有这种情况）
	// 此时根据新客户端的IP地址去判断要删除那个连接（把相同IP的删掉）
	if (strcmp(m_pRelatedCltObjPtr1->GetPeerIp().c_str(), pCltObj->GetPeerIp().c_str()) == 0)
	{
		// 先关闭
		m_pRelatedCltObjPtr1->Close();
		// 再替换
		m_pRelatedCltObjPtr1 = pCltObj;
		assert(!m_objRecvSeq1.IsValid());
		
		return true;
	}
	else if (strcmp(m_pRelatedCltObjPtr2->GetPeerIp().c_str(), pCltObj->GetPeerIp().c_str()) == 0)
	{
		// 先关闭
		m_pRelatedCltObjPtr2->Close();
		// 再替换
		m_pRelatedCltObjPtr2 = pCltObj;
		assert(!m_objRecvSeq2.IsValid());
		
		return true;
	}

	// 否则虽然能配对，但是还是不能添加，此时输出失败
	LogImportant("++++CNetServerDoubleCltObj::AddCltObj(CServerOneNetCltObj* pCltObj) return false\n"\
		"  old ip1=%s, ip2=%s\n  new ip1=%s, ip2=%s",
		m_pRelatedCltObjPtr1->GetPeerIp().c_str(), m_pRelatedCltObjPtr2->GetPeerIp().c_str(),
		pCltObj->GetPeerIp().c_str(), pCltObj->GetPairNetIp().c_str());

	return false;
}

bool CNetServerDoubleCltObj::RemoveCltObj(CServerOneNetCltObj* pCltObj)
{
	if (m_pRelatedCltObjPtr1 && m_pRelatedCltObjPtr1 == pCltObj)
	{
		m_pRelatedCltObjPtr1 = NULL;
		m_objRecvSeq1.Reset();

		if (m_pRelatedCltObjPtr2 == NULL)
		{
			// 双网的两个连接都断开了，需要重新设置双网序列号
			assert(!m_objRecvSeq2.IsValid());
			m_objRecvSeq.Reset();
			m_objSendSeq.Reset();
		}
		
		return true;
	}

	if (m_pRelatedCltObjPtr2 && m_pRelatedCltObjPtr2 == pCltObj)
	{
		m_pRelatedCltObjPtr2 = NULL;
		m_objRecvSeq2.Reset();

		if (m_pRelatedCltObjPtr1 == NULL)
		{
			// 双网的两个连接都断开了，需要重新设置双网序列号
			assert(!m_objRecvSeq1.IsValid());
			m_objRecvSeq.Reset();
			m_objSendSeq.Reset();
		}

		return true;
	}

	return false;
}

bool CNetServerDoubleCltObj::DealRecvPacketDataSeq(CServerOneNetCltObj* pCltObj, CBuffer* pBuf)
{
	assert(pCltObj);
	if (pCltObj != m_pRelatedCltObjPtr1 && pCltObj != m_pRelatedCltObjPtr2)
	{
		return false;
	}

	if (!DealSingleRecvSeq(pCltObj, pBuf))
	{
		return false;
	}
	
	if (!DealDoubleRecvSeq(pCltObj))
	{
		return false;
	}

	return true;
}

bool CNetServerDoubleCltObj::DealSingleRecvSeq(CServerOneNetCltObj* pCltObj, CBuffer* pBuf)
{
	assert(pCltObj);

	CProtocolForward::CForwardDataSeq* pLastDataSeq = &m_objRecvSeq1;
	if (pCltObj == m_pRelatedCltObjPtr2)
	{
		pLastDataSeq = &m_objRecvSeq2;
	}
	else
	{
		assert(pCltObj == m_pRelatedCltObjPtr1);
	}

	CServerOneNet* pSvr = (CServerOneNet*)pCltObj->GetRelatedServerPtr();
	assert(pSvr);
	assert(pSvr->IsMyClassName("CServerOneNet"));
	
	// 从数据包中获取双网序列号
	CProtocolForward::CForwardDataSeq objDataSeqInPkt;
	int nOffset = CProtocolSession::HeaderSize() + CProtocolForward::CForwardDataSeq::Size();
	if (pBuf->GetUsed() < nOffset)
	{
		// 如果某个客户端的数据长度有问题，那么该连接有问题，关闭该连接
		LogImportant("CNetServerDoubleCltObj::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but it is not enough to get double_net_seq. so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str());
		
		pSvr->CloseClientObj(pCltObj);
		return false;
	}
	
	nOffset = CProtocolSession::HeaderSize();
	if (!CProtocolForward::GetDataSeq(pBuf->GetBuffer() + nOffset, pBuf->GetUsed() - nOffset, objDataSeqInPkt))
	{
		LogImportant("CNetServerDoubleCltObj::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but get double_net_seq fail! so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str());
		
		pSvr->CloseClientObj(pCltObj);
		return false;
	}
	
	// 检查该客户端连接接收到的数据序列号是否有效
	if (!objDataSeqInPkt.IsValid()) 
	{
		LogImportant("CNetServerDoubleCltObj::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but it's double_net_seq is invalid! so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str());

		pSvr->CloseClientObj(pCltObj);
		return false;
	}

	LogDebug("CNetServerDoubleCltObj::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s), my last(%s).", 
		pCltObj->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
	
	// 如果已经收到的序列号是个无效的，那么可以认为刚刚初始化完毕
	if (!pLastDataSeq->IsValid()) 
	{
		LogDebug("CNetServerDoubleCltObj::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s), my last(%s) is invalid, so update it.", 
			pCltObj->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
		
		// 更新单个连接的双网序列号
		*pLastDataSeq = objDataSeqInPkt;

		return true;
	}
	
	// 检查该客户端连接的双网序列号的连续性
	if (pLastDataSeq->GetNextSeq() == objDataSeqInPkt)
	{
		LogDebug("CNetServerDoubleCltObj::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s) is continuous with my last(%s), so update it.", 
			pCltObj->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());

		// 更新单个连接的双网序列号
		*pLastDataSeq = objDataSeqInPkt;

		return true;
	}
	else
	{
		LogImportant("CNetServerDoubleCltObj::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is not continuous with my last(%s)! so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());

		pSvr->CloseClientObj(pCltObj);
		return false;
	}
}

bool CNetServerDoubleCltObj::DealDoubleRecvSeq(CServerOneNetCltObj* pCltObj)
{
	assert(pCltObj);
	
	CProtocolForward::CForwardDataSeq* pLastDataSeq = &m_objRecvSeq;
	CProtocolForward::CForwardDataSeq objNeedDataSeq = pLastDataSeq->GetNextSeq();

	CProtocolForward::CForwardDataSeq* pRecvDataSeq = &m_objRecvSeq1;
	CProtocolForward::CForwardDataSeq* pPairDataSeq = &m_objRecvSeq2;
	if (pCltObj == m_pRelatedCltObjPtr2)
	{
		pRecvDataSeq = &m_objRecvSeq2;
		pPairDataSeq = &m_objRecvSeq1;
	}
	else
	{
		assert(pCltObj == m_pRelatedCltObjPtr1);
	}
	
	// 双网建立后接收到第一包
	if (!pLastDataSeq->IsValid())
	{
		LogDebug("CNetServerDoubleCltObj::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s), my last(%s) is invalid, so update it.", 
			pCltObj->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
		
		// 更新双网数据序列号
		*pLastDataSeq = *pRecvDataSeq;
		
		return true;
	}
	// 接收到的包正好是下一包
	else if (*pRecvDataSeq == objNeedDataSeq)
	{
		LogDebug("CNetServerDoubleCltObj::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s) is continuous with my last(%s), so update it.", 
			pCltObj->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
		
		// 更新双网数据序列号
		*pLastDataSeq = *pRecvDataSeq;
		
		return true;
	}
	// 接收的序列号距离下一包太远
	else if (pRecvDataSeq->IsFarawayFrom(objNeedDataSeq)) 
	{
		if (pRecvDataSeq->GetHigh() == 0 && pRecvDataSeq->GetLow() == 1)
		{
			LogImportant("CNetServerDoubleCltObj::DealDoubleRecvSeq(), \n connection(%s) \n receive the first pkt, but received double_net_seq(%s) is far away from my needed(%s)! so will close double client socket ...", 
				pCltObj->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
			
			if (m_pRelatedCltObjPtr1)
			{
				m_pRelatedCltObjPtr1->GetRelatedServerPtr()->CloseClientObj(m_pRelatedCltObjPtr1);
			}
			if (m_pRelatedCltObjPtr2)
			{
				m_pRelatedCltObjPtr2->GetRelatedServerPtr()->CloseClientObj(m_pRelatedCltObjPtr2);
			}
		}
		else
		{
			LogImportant("CNetServerDoubleCltObj::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is far away from my needed(%s)! so will close client socket ...", 
				pCltObj->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
			
			pCltObj->GetRelatedServerPtr()->CloseClientObj(pCltObj);
		}
	}
	// 接收到的包序列号比下一包要大
	else if (*pRecvDataSeq > objNeedDataSeq) 
	{
		if (!pPairDataSeq->IsValid() || *pPairDataSeq > objNeedDataSeq)
		{
			// 此时，两个client的接收序列号都大于等待的接收序列号
			// 将不可能等到下一包，此时要断开两个双网连接。
			LogImportant("CNetServerDoubleCltObj::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but both received double_net_seq(%s) and pair client double_net_seq(%s) are bigger than my needed(%s)! so close will close double client socket ...", 
				pCltObj->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), pPairDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
			
			if (m_pRelatedCltObjPtr1)
			{
				m_pRelatedCltObjPtr1->GetRelatedServerPtr()->CloseClientObj(m_pRelatedCltObjPtr1);
			}
			if (m_pRelatedCltObjPtr2)
			{
				m_pRelatedCltObjPtr2->GetRelatedServerPtr()->CloseClientObj(m_pRelatedCltObjPtr2);
			}
		}
		else
		{
			// 此时，虽然当前client的接收序列号大于等待的接收序列号，但另一个client的接收序号不不大于等待的接收序号，
			// 所以还有可能等到下一包，此时只需丢掉当前接收的错误序号的包即可。
			LogDebug("CNetServerDoubleCltObj::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is bigger than my needed(%s), so throw the pkt!", 
				pCltObj->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
		}
	}
	// 接收到的包序列号比下一包要小
	else if (*pRecvDataSeq < objNeedDataSeq) 
	{
		// 该client有可能递增赶上接收序列号
		LogDebug("CNetServerDoubleCltObj::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is smaller than my needed(%s), so throw the pkt!", 
			pCltObj->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
	}

	return false;
}

bool CNetServerDoubleCltObj::SendData( char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr )
{
	assert(szBuf);
	
	// 添加双网数据序号
	CProtocolForward::CForwardHead objHead;
	CProtocolForward::CForwardDataSeq objDataSeqSended = m_objSendSeq;
	objDataSeqSended.Inc(); // 发送之前，生成正确的双网数据序号
	objHead.SetDataSeq(objDataSeqSended);	

	LogDebug("CNetServerDoubleCltObj::SendData(), \n double nets(%s) \n set double_net_seq=%s ok!",
		GetTagConnInfo().c_str(), objDataSeqSended.GetDumpStr().c_str());
	
	// 添加目的地址
	for (int x = 0; x < vecDestAddr.size(); x++)
	{
		objHead.AddDest(vecDestAddr[x]);
	}

	// 分别发送到两个单网，实际上是放入发送队列里
	bool bRet1 = false;
	if (m_pRelatedCltObjPtr1)
	{
		CServerOneNet* pSvr1 = (CServerOneNet*)m_pRelatedCltObjPtr1->GetRelatedServerPtr();
		assert(pSvr1);
		assert(pSvr1->IsMyClassName("CServerOneNet"));

		bRet1 = pSvr1->SendForwardData(objHead, szBuf, nBufLen, m_pRelatedCltObjPtr1);
		if(bRet1)
		{
			LogDebug("CNetServerDoubleCltObj::SendData(), \n connection(%s) \n send ok!",
				m_pRelatedCltObjPtr1->GetTagConnInfo().c_str());
		}
		else
		{
			LogDebug("CNetServerDoubleCltObj::SendData(), \n connection(%s) \n send fail!",
				m_pRelatedCltObjPtr1->GetTagConnInfo().c_str());
		}
	}
	
	bool bRet2 = false;
	if (m_pRelatedCltObjPtr2)
	{
		CServerOneNet* pSvr2 = (CServerOneNet*)m_pRelatedCltObjPtr2->GetRelatedServerPtr();
		assert(pSvr2);
		assert(pSvr2->IsMyClassName("CServerOneNet"));
		
		bRet2 = pSvr2->SendForwardData(objHead, szBuf, nBufLen, m_pRelatedCltObjPtr2);
		if (bRet2)
		{
			LogDebug("CNetServerDoubleCltObj::SendData(), \n connection(%s) \n send ok!",
				m_pRelatedCltObjPtr2->GetTagConnInfo().c_str());
		}
		else
		{
			LogDebug("CNetServerDoubleCltObj::SendData(), \n connection(%s) \n send fail!",
				m_pRelatedCltObjPtr2->GetTagConnInfo().c_str());
		}
	}
	
	if (bRet1 || bRet2)
	{
		// 只要成功发送到一个网，双网的发送序列号就要递增
		m_objSendSeq = objDataSeqSended;
		return true;
	}
	else
	{
		LogImportant("CNetServerDoubleCltObj::SendData(), \n double nets(%s) \n send fail!",
			GetTagConnInfo().c_str());
		return false;
	}
}

bool CNetServerDoubleCltObj::SendRawData(CBuffer* pBuf)
{
	assert(pBuf);
	
	// 获取转发层信息位置
	char* pForword = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
	int nForwardLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nForwardLen <= 0)
	{
		LogImportant("CNetServerDoubleCltObj::SendRawData(), \n double nets(%s) \n send data fail! send buf is not enough to get forward_head.",
			GetTagConnInfo().c_str());
		
		return false;
	}
	
	// 发送之前，生成正确的双网数据序号
	CProtocolForward::CForwardDataSeq objDataSeqSended = m_objSendSeq;
	objDataSeqSended.Inc(); 

	// 向数据包设置双网数据序号
	if (!CProtocolForward::SetDataSeq(pForword, nForwardLen, objDataSeqSended))
	{
		LogImportant("CNetServerDoubleCltObj::SendRawData(), \n double nets(%s) \n send data fail! fail to set double_net_seq.",
			GetTagConnInfo().c_str());
		return false;
	}
	else
	{
		LogDebug("CNetServerDoubleCltObj::SendRawData(), \n double nets(%s) \n set double_net_seq=%s ok!",
			GetTagConnInfo().c_str(), objDataSeqSended.GetDumpStr().c_str());
	}
	
	// 分别发送到两个单网，实际上是放入发送队列里
	bool bRet1 = false;
	if (m_pRelatedCltObjPtr1)
	{
		CServerOneNet* pSvr1 = (CServerOneNet*)m_pRelatedCltObjPtr1->GetRelatedServerPtr();
		assert(pSvr1);
		assert(pSvr1->IsMyClassName("CServerOneNet"));
		
		bRet1 = pSvr1->SendData(pBuf->GetBuffer(), pBuf->GetUsed(), m_pRelatedCltObjPtr1);
		if(bRet1)
		{
			LogDebug("CNetServerDoubleCltObj::SendRawData(), \n connection(%s) \n send ok!",
				m_pRelatedCltObjPtr1->GetTagConnInfo().c_str());
		}
		else
		{
			LogDebug("CNetServerDoubleCltObj::SendRawData(), \n connection(%s) \n send fail!",
				m_pRelatedCltObjPtr1->GetTagConnInfo().c_str());
		}
	}
	
	bool bRet2 = false;
	if (m_pRelatedCltObjPtr2)
	{
		CServerOneNet* pSvr2 = (CServerOneNet*)m_pRelatedCltObjPtr2->GetRelatedServerPtr();
		assert(pSvr2);
		assert(pSvr2->IsMyClassName("CServerOneNet"));
		
		bRet2 = pSvr2->SendData(pBuf->GetBuffer(), pBuf->GetUsed(), m_pRelatedCltObjPtr2);
		if (bRet2)
		{
			LogDebug("CNetServerDoubleCltObj::SendRawData(), \n connection(%s) \n send ok!",
				m_pRelatedCltObjPtr2->GetTagConnInfo().c_str());
		}
		else
		{
			LogDebug("CNetServerDoubleCltObj::SendRawData(), \n connection(%s) \n send fail!",
				m_pRelatedCltObjPtr2->GetTagConnInfo().c_str());
		}
	}
	
	if (bRet1 || bRet2)
	{
		// 只要成功发送到一个网，双网的发送序列号就要递增
		m_objSendSeq = objDataSeqSended;
		return true;
	}
	else
	{
		LogImportant("CNetServerDoubleCltObj::SendRawData(), double nets(%s) send fail!",
			GetTagConnInfo().c_str());
		return false;
	}
}


