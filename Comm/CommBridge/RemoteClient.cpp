//==============================================================================
//                      RemoteClient.cpp
//
//begin   : 2016-09-18
//describe:
//==============================================================================

#include "RemoteClient.h"
#include "CommIdDef.h"

//==============================================================================
//                 
//                    CRemoteClient
//                   CommBridgeԶ�̿ͻ�����
//
//==============================================================================
CRemoteClient::CRemoteClient()
{
	SetMyClassName("CRemoteClient");
	
	m_strService = "";
}

CRemoteClient::~CRemoteClient()
{
}

void CRemoteClient::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CRemoteClient::OnConnect(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientDouble::OnConnect(pClient);
	
	// ����������Ӧ
}

void CRemoteClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CRemoteClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientDouble::OnClose(pClient);
}

void CRemoteClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CRemoteClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientDouble::OnSessionReady(pClient);
	
	// ����������Ӧ
}

void CRemoteClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CRemoteClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());

	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientDouble::OnSessionClose(pClient);
}

bool CRemoteClient::Init(string strConfigFile, string strService)
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
		LogImportant("CRemoteClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("RemoteClient");
	if (!txhNode.Element())
	{
		LogImportant("CRemoteClient::Init(), \n fail to read <%s><RemoteClient>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode) || !InitInCondition(txhNode))
	{
		return false;
	}

	if (GetType() != AF_INET)
	{
		LogImportant("CRemoteClient::Init(), \n I need ServerType=AF_INET for service=%s!", strService.c_str());
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

bool CRemoteClient::SendRawData( CBuffer* pBuf )
{
	return CNetClientDouble::SendRawData(pBuf);
}
