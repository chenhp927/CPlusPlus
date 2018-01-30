//==============================================================================
//                      TermClient.cpp
//
//begin   : 2016-09-18
//describe:
//==============================================================================

#include "TermClient.h"
#include "CommIdDef.h"

//==============================================================================
//                 
//                    CTermClient
//                   CommTerm�ͻ�����
//
//==============================================================================
CTermClient::CTermClient()
{
	SetMyClassName("CTermClient");
	
	m_strService = "";
}

CTermClient::~CTermClient()
{
}

void CTermClient::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CTermClient::OnConnect(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientSingle::OnConnect(pClient);
	
	// ����������Ӧ
}

void CTermClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CTermClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnClose(pClient);
}

void CTermClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CTermClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientSingle::OnSessionReady(pClient);
	
	// ����������Ӧ
}

void CTermClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CTermClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnSessionClose(pClient);
}

bool CTermClient::Init(string strConfigFile, string strService)
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
		LogImportant("CTermClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("Client");
	if (!txhNode.Element())
	{
		LogImportant("CTermClient::Init(), \n fail to read <%s><Client>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode) || !InitInCondition(txhNode))
	{
		return false;
	}

	if (GetType() != AF_UNIX)
	{
		LogImportant("CTermClient::Init(), \n I need ServerType=AF_UNIX for service=%s!", strService.c_str());
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

bool CTermClient::SendRawData( CBuffer* pBuf )
{
	return CNetClientSingle::SendRawData(pBuf);
}


