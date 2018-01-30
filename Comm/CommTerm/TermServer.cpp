//==============================================================================
//                      TermServer.cpp
//                       
//begin   : 2016-09-18                                                 
//describe:                                              
//==============================================================================

#include "TermServer.h"
#include "CommIdDef.h"
#include "TermApp.h"
#include "ConnState.h"

//==============================================================================
//                 
//                    CTermServer
//                   CommTerm�������
//
//==============================================================================
CTermServer::CTermServer()
{ 
	SetMyClassName("CTermServer");

	m_strService = "";
} 
 
CTermServer::~CTermServer() 
{ 
}

bool CTermServer::Init(string strConfigFile, string strService)
{
	// �������Ƿ���Ч
	assert(strConfigFile.size() > 0 && strService.size() > 0);

	// ����Ϊ�������Ϊͨ�ų�������
	m_pServer1->SetAppServer(false);
	m_pServer2->SetAppServer(false);

	// ���÷�������
	m_strService = strService;
	
	// ���ر�ģ���˽�������ļ���������������Ҫ����Ϣ
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("CTermServer::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("Server");
	if (!txhNode.Element())
	{
		LogImportant("CTermServer::Init(), \n fail to read <%s><Server>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode))
	{
		return false;
	}

	if (GetType() != AF_INET)
	{
		LogImportant("CTermServer::Init(), \n I need ServerType=AF_INET for service=%s!", strService.c_str());
		return false;
	}
	
	CSvrConnStateMgr::Instance().SetSvrInfo(m_pServer1->GetServerAddr(), m_pServer1->GetSvrIp(), 
		static_cast<unsigned short>(m_pServer1->GetSvrPort()), m_pServer2->GetSvrIp(), static_cast<unsigned short>(m_pServer2->GetSvrPort()));

	return true;
}

string CTermServer::GetServerIdValidRangeStr(bool bIsAppSvr)
{
	string str = "";
	str += CCommIdDef::GetServerIdValidRangeStr(bIsAppSvr, SERVERID_FOR_COMMTERM);
	
	return str;
}

bool CTermServer::IsServerIdValid(bool bIsAppSvr, WORD wSvrId)
{
	return CCommIdDef::IsServerIdValid(wSvrId, bIsAppSvr, SERVERID_FOR_COMMTERM);
}

string CTermServer::GetCliendIdValidRangeStr()
{
	string str = "";

	// ��������ҵ�����ͻ��ˣ��ն���ҵ�����
	str += CCommIdDef::GetCliendIdValidRangeStr(true, CLIENTID_FOR_TERMAPP);

	// ��������ͨ�ų���ͻ��ˣ���
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_NULL);

	return str;
}

bool CTermServer::IsClientIdValid(bool bIsAppClt, WORD wCltId)
{
	if (bIsAppClt)
	{
		// ��������ҵ�����ͻ��ˣ��ն���ҵ�����
		return CCommIdDef::IsClientIdValid(wCltId, true, CLIENTID_FOR_TERMAPP);
	}
	else
	{
		// ��������ͨ�ų���ͻ��ˣ���
		return CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_NULL);
	}
}

void CTermServer::OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert (pCltObj && pBuf);

	// ��ȡת������Ϣλ��
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// ĳ���ͻ��˵ĻỰ�����ݳ���������
		LogImportant("CTermServer::OnDealRecvData(), \n connection(%s) \n "
			"receive one pkt, but it is not enough to get session head!", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		return;
	}
	char* p = pBuf->GetBuffer() + CProtocolSession::HeaderSize();

	// ת����
	if (CProtocolForward::IsMsgForwardPkt(p, nDataLen))
	{
		// ����ת�����İ�ͷ
		CProtocolForward::CForwardHead objHead;
		if (objHead.FromStreamBuf(p, nDataLen) <= 0)
		{
			// ���ĳ���ͻ��˵�ת�������ݳ��������⣬��ô�����������⣬�رո�����
			LogImportant("CTermServer::OnDealRecvData(), \n connection(%s) \n "
				"receive one pkt, but it is not enough to get forward head!", 
				pCltObj->GetTagConnInfo().c_str());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
		else
		{
			CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
			
			LogDebug("CTermServer::OnDealRecvData(), \n connection(%s) \n receive one pkt(%s), I will forward it ...", 
				pCltObj->GetTagConnInfo().c_str(), objTypeFunc.GetTagDumpStr().c_str());
			LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
			
			// ���ҵĿͻ��˽��յ�����һ��Ҫ��<�ڲ�����ת������>ģ�鷢��
			// ������֤���е����ݶ��ܵ�<�ڲ�����ת������>���д���
			SendDataToCommInner(pCltObj, pBuf, objHead);

			// ���ҵ�һ���ͻ��˽��յ����ݣ�������ݰ�֧�ֱ���ת�����ߺ���Ŀ�ĵأ���Ҳ��Ҫ���ҵ������ͻ��˷���
			if (objHead.IsSupportLocalDispatch()
				|| objHead.GetDestAddrs().size() > 0)
			{
				// ����<�ն�����ת������>��˵�������ҵĿͻ��˶���ҵ�����ͻ��ˣ�������Ҫת���ҵ������ͻ���
				ForwardDataToMyOtherClients(pCltObj, pBuf, objHead);
			}
		}
	}
	// δ֪��
	else
	{
		// δ֪��������
		LogImportant("CTermServer::OnDealRecvData(), \n connection(%s) \n "
			"receive one pkt, but it is unknown pkt.", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
}

void CTermServer::SendDataToCommInner(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CTermServer::SendDataToCommInner(), \n forward pkt(%s) from my connection(%s) to CommInner server!", 
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());

	CTermApp* pApp = (CTermApp*)CTermApp::GetApp();
	assert(pApp);
	pApp->SendDataToCommInner(pBuf);
}

void CTermServer::OnRecvDataFromCommInner(CBuffer* pBuf)
{
	assert (pBuf);
	
	// ��ȡת������Ϣλ��
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// ĳ���ͻ��˵ĻỰ�����ݳ���������
		LogImportant("CTermServer::OnRecvDataFromCommInner(), \n "
			"receive one pkt from CommInner, but it is not enough to "
			" get session head!");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		return;
	}
	char* p = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
	
	// ת����
	if (CProtocolForward::IsMsgForwardPkt(p, nDataLen))
	{
		// ����ת�����İ�ͷ
		CProtocolForward::CForwardHead objHead;
		if (objHead.FromStreamBuf(p, nDataLen) <= 0)
		{
			// ���ĳ���ͻ��˵�ת�������ݳ��������⣬��ô�����������⣬�رո�����
			LogImportant("CTermServer::OnRecvDataFromCommInner(), \n "
				"receive one pkt from CommInner, but it is not enough to "
				"get forward head!");
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
		else
		{
			CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
			
			LogDebug("CTermServer::OnRecvDataFromCommInner(), \n receive one pkt(%s) from CommInner, I will forward it ...", 
				objTypeFunc.GetTagDumpStr().c_str());
			LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
			
			ForwardDataToMyClients(pBuf, objHead);
		}
	}
}

void CTermServer::ForwardDataToMyOtherClients(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert(pCltObj && pBuf);

	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CTermServer::ForwardDataToMyOtherClients(), \n will forward pkt(%s) from connection(%s) to my others ...",
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());

	int nActiveClientCount = 0;
	int nSendClientCount = 0;
	string strSendClientInfo = "";

	// ���������ҵ����пͻ��ˣ�˫����
	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		CNetServerDoubleCltObj* pNetServerDoubleCltObj = &m_vecNetServerDoubleCltObj[x];

		// �ͻ���δ���ӣ��϶���������
		if (!pNetServerDoubleCltObj->IsReady())
		{
			continue;
		}
		
		// �Լ����İ��϶������ٷ���
		if (pNetServerDoubleCltObj->GetRelatedCltObjPtr1() == pCltObj
			|| pNetServerDoubleCltObj->GetRelatedCltObjPtr2() == pCltObj)
		{
			continue;
		}

		nActiveClientCount++;

		// ��ȡ�ͻ��˵�ע������
		CForwardCondition* pCond = pNetServerDoubleCltObj->GetForwardCondition();
		if (!pCond)
		{
			continue;
		}

		// ע�����������ϣ��϶���������
		if (!pCond->IsMatched(objHead))
		{
			continue;
		}
		
		bool bSend = false;
		// ������ݰ�û��Ŀ�ĵأ����͸�����ע����������Ŀͻ���
		if (objHead.GetDestAddrs().size() <= 0)
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
		}
		// ������ݰ���Ŀ�ĵأ���ֻ���͸�ע���������ϲ���Ŀ�ĵ�Ҳ���ϵĿͻ���
		else if (IsDataDestAddrMatchClt(objHead, pNetServerDoubleCltObj))
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
		}

		if (bSend)
		{
			nSendClientCount++;
			strSendClientInfo += pNetServerDoubleCltObj->GetTagCltInfo().c_str();
			strSendClientInfo += "\n";
		}
	}

	if (nActiveClientCount <= 0)
	{
		LogImportant("CTermServer::ForwardDataToMyOtherClients(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	LogDebug("CTermServer::ForwardDataToMyOtherClients(), \n forward the data to %d clients. they are:\n%s",
			nSendClientCount, strSendClientInfo.c_str());
}

void CTermServer::ForwardDataToMyClients(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert(pBuf);
	
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CTermServer::ForwardDataToMyClients(), \n will forward pkt(%s) from CommInner server to my clients ...",
		objTypeFunc.GetTagDumpStr().c_str());

	int nActiveClientCount = 0;
	int nSendClientCount = 0;
	string strSendClientInfo = "";
	
	// ���������ҵ����пͻ��ˣ�˫����
	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		CNetServerDoubleCltObj* pNetServerDoubleCltObj = &m_vecNetServerDoubleCltObj[x];
		
		// �ͻ���δ���ӣ��϶���������
		if (!pNetServerDoubleCltObj->IsReady())
		{
			continue;
		}
		
		nActiveClientCount++;
		
		// ��ȡ�ͻ��˵�ע������
		CForwardCondition* pCond = pNetServerDoubleCltObj->GetForwardCondition();
		if (!pCond)
		{
			continue;
		}
		
		// ע�����������ϣ��϶���������
		if (!pCond->IsMatched(objHead))
		{
			continue;
		}
		
		bool bSend = false;
		// ������ݰ�û��Ŀ�ĵأ����͸�����ע����������Ŀͻ���
		if (objHead.GetDestAddrs().size() <= 0)
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
		}
		// ������ݰ���Ŀ�ĵأ���ֻ���͸�ע���������ϲ���Ŀ�ĵ�Ҳ���ϵĿͻ���
		else if (IsDataDestAddrMatchClt(objHead, pNetServerDoubleCltObj))
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
		}
		
		if (bSend)
		{
			nSendClientCount++;
			strSendClientInfo += pNetServerDoubleCltObj->GetTagCltInfo().c_str();
			strSendClientInfo += "\n";
		}
	}
	
	if (nActiveClientCount <= 0)
	{
		LogImportant("CTermServer::ForwardDataToMyClients(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	LogDebug("CTermServer::ForwardDataToMyClients(), \n forward the data to %d clients. they are:\n%s",
			nSendClientCount, strSendClientInfo.c_str());
}

bool CTermServer::IsDataDestAddrMatchClt(CProtocolForward::CForwardHead& objHead, CNetServerDoubleCltObj* pDoubleCltObj)
{
	assert(pDoubleCltObj);
	CCommAddr* pCltAddr = pDoubleCltObj->GetClientAddr();
	assert(pCltAddr);

	vector<CProtocolForward::CForwardAddr> vecDstAddr;
	vecDstAddr = objHead.GetDestAddrs();
	for (int x = 0; x < vecDstAddr.size(); x++)
	{
		CProtocolForward::CForwardAddr* pDstAddr = &vecDstAddr[x];
		if (pDstAddr->IsMatched(*pCltAddr))
		{
			return true;
		}
	}

	return false;
}

bool CTermServer::Startup()
{
	return CNetServerDouble::Startup();
}

void CTermServer::OnSessionConnect( CServerOneNetCltObj* pCltObj )
{
	CNetServerDouble::OnSessionConnect(pCltObj);
}

void CTermServer::OnSessionReady( CServerOneNetCltObj* pCltObj )
{
	CNetServerDouble::OnSessionReady(pCltObj);

	char cNetNo = 0;
	if (pCltObj->GetRelatedServerPtr() == m_pServer1)
	{
		cNetNo = 1;
	}
	else
	{
		cNetNo = 2;
	}
	CSvrConnStateMgr::Instance().OnCltConnEvent(pCltObj->GetClientAddr(), cNetNo, 
		pCltObj->GetPeerIp(), pCltObj->GetPeerPort(), RM_CONNECTED);
}

void CTermServer::OnSessionClose( CServerOneNetCltObj* pCltObj )
{
	char cNetNo = 0;
	if (pCltObj->GetRelatedServerPtr() == m_pServer1)
	{
		cNetNo = 1;
	}
	else
	{
		cNetNo = 2;
	}
	CSvrConnStateMgr::Instance().OnCltConnEvent(pCltObj->GetClientAddr(), cNetNo, 
		pCltObj->GetPeerIp(), pCltObj->GetPeerPort(), RM_UNCONNECT);	

	CNetServerDouble::OnSessionClose(pCltObj);
}

