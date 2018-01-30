//==============================================================================
//                      OuterClient.cpp
//
//begin   : 2016-09-16
//describe:
//==============================================================================

#include "OuterClient.h"
#include "CommIdDef.h"

//==============================================================================
//                 
//                    COuterClient
//                   CommOuter�ͻ�����
//
//==============================================================================
COuterClient::COuterClient()
{
	SetMyClassName("COuterClient");
	
	m_strService = "";
}

COuterClient::~COuterClient()
{
}

void COuterClient::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("COuterClient::OnConnect(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientSingle::OnConnect(pClient);
	
	// ����������Ӧ
}

void COuterClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("COuterClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnClose(pClient);
}

void COuterClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("COuterClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientSingle::OnSessionReady(pClient);
	
	// ����������Ӧ
}

void COuterClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("COuterClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnSessionClose(pClient);
}

bool COuterClient::Init(string strConfigFile, string strService)
{
	// �������Ƿ���Ч
	assert(strConfigFile.size() > 0 && strService.size() > 0);
	
	// ����Ϊ���ͻ���Ϊͨ�ų���ͻ���
	m_pClient->SetAppClient(false);
	
	// ���÷�������
	m_strService = strService;
	
	// ���ر�ģ���˽�������ļ���������������Ҫ����Ϣ
	bool bRet = false;
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("COuterClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("Client");
	if (!txhNode.Element())
	{
		LogImportant("COuterClient::Init(), \n fail to read <%s><Client>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode) || !InitInCondition(txhNode))
	{
		return false;
	}

	if (GetType() != AF_UNIX)
	{
		LogImportant("COuterClient::Init(), \n I need ServerType=AF_UNIX for service=%s!", strService.c_str());
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

bool COuterClient::SendRawData( CBuffer* pBuf )
{
	return CNetClientSingle::SendRawData(pBuf);
}


