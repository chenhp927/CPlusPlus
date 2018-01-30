//==============================================================================
//                      LocalClient.cpp
//
//begin   : 2016-09-18
//describe:
//==============================================================================

#include "LocalClient.h"
#include "CommIdDef.h"

//==============================================================================
//                 
//                    CLocalClient
//                   CommBridge���ؿͻ�����
//
//==============================================================================
CLocalClient::CLocalClient()
{
	SetMyClassName("CLocalClient");
	
	m_strService = "";
}

CLocalClient::~CLocalClient()
{
}

void CLocalClient::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogImportant("CLocalClient::OnConnect(), \n connection(%s)", pClient->GetTagConnInfo().c_str());

	// �Ȼ�����Ӧ
	CNetClientSingle::OnConnect(pClient);

	// ����������Ӧ
}

void CLocalClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogImportant("CLocalClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());

	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnClose(pClient);
}

void CLocalClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogImportant("CLocalClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientSingle::OnSessionReady(pClient);

	// ����������Ӧ
}

void CLocalClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogImportant("CLocalClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnSessionClose(pClient);
}

bool CLocalClient::Init(string strConfigFile, string strService)
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
		LogImportant("CLocalClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("LocalClient");
	if (!txhNode.Element())
	{
		LogImportant("CLocalClient::Init(), \n fail to read <%s><LocalClient>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode) || !InitInCondition(txhNode))
	{
		return false;
	}

	if (GetType() != AF_UNIX)
	{
		LogImportant("CLocalClient::Init(), \n I need ServerType=AF_UNIX for service=%s!", strService.c_str());
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

bool CLocalClient::SendRawData( CBuffer* pBuf )
{
	return CNetClientSingle::SendRawData(pBuf);
}


