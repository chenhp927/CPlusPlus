//==============================================================================
//                      SynServer.cpp
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================

#include "SynServer.h"
#include "CommIdDef.h"
#include "DualApp.h"
#include "ConnState.h"

//==============================================================================
//                 
//                    CSynServer
//                   CommDualͬ���������
//
//==============================================================================
CSynServer::CSynServer()
{ 
	SetMyClassName("CSynServer");

	m_strService = "";
} 
 
CSynServer::~CSynServer() 
{ 
}

bool CSynServer::Init(string strConfigFile, string strService)
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
		LogImportant("CSynServer::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("SynServer");
	if (!txhNode.Element())
	{
		LogImportant("CSynServer::Init(), \n fail to read <%s><SynServer>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode))
	{
		return false;
	}

	if (GetType() != AF_INET)
	{
		LogImportant("CSynServer::Init(), \n I need ServerType=AF_INET for service=%s!", strService.c_str());
		return false;
	}
	
	CSvrConnStateMgr::Instance().SetSvrInfo(m_pServer1->GetServerAddr(), m_pServer1->GetSvrIp(), 
		static_cast<unsigned short>(m_pServer1->GetSvrPort()), m_pServer2->GetSvrIp(), static_cast<unsigned short>(m_pServer2->GetSvrPort()));

	return true;
}

string CSynServer::GetServerIdValidRangeStr(bool bIsAppSvr)
{
	string str = "";
	str += CCommIdDef::GetServerIdValidRangeStr(bIsAppSvr, SERVERID_FOR_COMMDUAL);
	
	return str;
}

bool CSynServer::IsServerIdValid(bool bIsAppSvr, WORD wSvrId)
{
	return CCommIdDef::IsServerIdValid(wSvrId, bIsAppSvr, SERVERID_FOR_COMMDUAL);
}

string CSynServer::GetCliendIdValidRangeStr()
{
	string str = "";

	// ��������ҵ�����ͻ��ˣ���
	str += CCommIdDef::GetCliendIdValidRangeStr(true, CLIENTID_FOR_NULL);

	// ��������ͨ�ų���ͻ��ˣ�˫��ͬ����������
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMDUAL);

	return str;
}

bool CSynServer::IsClientIdValid(bool bIsAppClt, WORD wCltId)
{
	if (bIsAppClt)
	{
		// ��������ҵ�����ͻ��ˣ���
		return CCommIdDef::IsClientIdValid(wCltId, true, CLIENTID_FOR_NULL);
	}
	else
	{
		// ��������ͨ�ų���ͻ��ˣ�˫��ͬ����������
		return CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_COMMDUAL);
	}
}

void CSynServer::OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert (pCltObj && pBuf);

	// ��ȡת������Ϣλ��
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// ĳ���ͻ��˵ĻỰ�����ݳ���������
		LogImportant("CSynServer::OnDealRecvData(), \n connection(%s) \n "
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
			LogImportant("CSynServer::OnDealRecvData(), \n connection(%s) \n "
				"receive one pkt, but it is not enough to get forward head!", 
				pCltObj->GetTagConnInfo().c_str());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
		else
		{
			CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
			
			LogDebug("CSynServer::OnDealRecvData(), \n connection(%s) \n receive one pkt(%s), I will forward it ...", 
				pCltObj->GetTagConnInfo().c_str(), objTypeFunc.GetTagDumpStr().c_str());
			LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
			
			// ���ҵĿͻ��˽��յ�����һ��Ҫ��<���ط����>���ͣ���<���ط����>���д���
			SendDataToLocalServer(pCltObj, pBuf, objHead);
		}
	}
	// δ֪��
	else
	{
		// δ֪��������
		LogImportant("CSynServer::OnDealRecvData(), \n connection(%s) \n "
			"receive one pkt, but it is unknown pkt.", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
}

void CSynServer::SendDataToLocalServer(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CSynServer::SendDataToLocalServer(), \n forward pkt(%s) from my server connection(%s) to local_server!", 
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());

	CDualApp* pApp = (CDualApp*)CDualApp::GetApp();
	assert(pApp);
	pApp->OnRecvDataFromSynConn(pBuf, objHead);
} 

bool CSynServer::Startup()
{
	return CNetServerDouble::Startup();
}

void CSynServer::OnSessionConnect( CServerOneNetCltObj* pCltObj )
{
	CNetServerDouble::OnSessionConnect(pCltObj);
}

void CSynServer::OnSessionReady( CServerOneNetCltObj* pCltObj )
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
		pCltObj->GetPeerIp(), static_cast<unsigned short>(pCltObj->GetPeerPort()), RM_CONNECTED);
}

void CSynServer::OnSessionClose( CServerOneNetCltObj* pCltObj )
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
		pCltObj->GetPeerIp(), static_cast<unsigned short>(pCltObj->GetPeerPort()), RM_UNCONNECT);	

	CNetServerDouble::OnSessionClose(pCltObj);
}

void CSynServer::OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert (pBuf);

	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();

	LogDebug("CSynServer::OnRecvDataFromLocalConn(), \n receive one pkt(%s) from LocalConn, I will forward it ...", 
		objTypeFunc.GetTagDumpStr().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());

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
		if (pNetServerDoubleCltObj->SendRawData(pBuf))
		{
			bSend = true;
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
		LogImportant("CSynServer::OnRecvDataFromLocalConn(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	LogDebug("CSynServer::OnRecvDataFromLocalConn(), \n forward the data to %d clients. they are:\n%s",
		nSendClientCount, strSendClientInfo.c_str());
}