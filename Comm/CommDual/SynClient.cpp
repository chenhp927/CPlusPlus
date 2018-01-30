//==============================================================================
//                      SynClient.cpp
//
//begin   : 2016-10-20
//describe:
//==============================================================================

#include "SynClient.h"
#include "CommIdDef.h"

//==============================================================================
//                 
//                    CSynClient
//                   CommDualͬ���ͻ�����
//
//==============================================================================
CSynClient::CSynClient()
{
	SetMyClassName("CSynClient");
	
	m_strService = "";
}

CSynClient::~CSynClient()
{
}

void CSynClient::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CSynClient::OnConnect(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientDouble::OnConnect(pClient);
	
	// ����������Ӧ
}

void CSynClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CSynClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientDouble::OnClose(pClient);
}

void CSynClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CSynClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientDouble::OnSessionReady(pClient);
	
	// ����������Ӧ
}

void CSynClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CSynClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());

	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientDouble::OnSessionClose(pClient);
}

bool CSynClient::Init(string strConfigFile, string strService)
{
	// �������Ƿ���Ч
	assert(strConfigFile.size() > 0 && strService.size() > 0);
	
	// ����Ϊ���ͻ���Ϊͨ�ų���ͻ���
	m_pClient1->SetAppClient(false);
	m_pClient2->SetAppClient(false);
	
	// ���÷�������
	m_strService = strService;
	
	// ���ر�ģ���˽�������ļ���������������Ҫ����Ϣ
	bool bRet = false;
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("CSynClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("SynClient");
	if (!txhNode.Element())
	{
		LogImportant("CSynClient::Init(), \n fail to read <%s><SynClient>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode) || !InitInCondition(txhNode))
	{
		return false;
	}

	if (GetType() != AF_INET)
	{
		LogImportant("CSynClient::Init(), \n I need ServerType=AF_INET for service=%s!", strService.c_str());
		return false;
	}
	
	if (GetInCondition())
	{
		GetInCondition()->SetEmptyAllowAll(false);
	}
	if (GetOutCondition())
	{
		GetOutCondition()->SetEmptyAllowAll(true);
	}
	
	return true;
}

bool CSynClient::SendRawData(CBuffer* pBuf)
{
	return CNetClientDouble::SendRawData(pBuf);
}

void CSynClient::OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert (pBuf);
	
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CSynClient::OnRecvDataFromLocalConn(), \n send pkt(%s) to connection(%s) ...", 
		objTypeFunc.GetTagDumpStr().c_str(), GetTagConnInfo().c_str());
	
	SendRawData(pBuf);
}
