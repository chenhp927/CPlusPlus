//==============================================================================
//                      ChannelClient.cpp
//
//begin   : 2016-09-18
//describe:
//==============================================================================

#include "ChannelClient.h"
#include "CommIdDef.h"

//==============================================================================
//                 
//                    CChannelClient
//                   CommChannel�ͻ�����
//
//==============================================================================
CChannelClient::CChannelClient()
{
	SetMyClassName("CChannelClient");
	
	m_strService = "";
}

CChannelClient::~CChannelClient()
{
}

void CChannelClient::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CChannelClient::OnConnect(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientSingle::OnConnect(pClient);
	
	// ����������Ӧ
}

void CChannelClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CChannelClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnClose(pClient);
}

void CChannelClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CChannelClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// �Ȼ�����Ӧ
	CNetClientSingle::OnSessionReady(pClient);
	
	// ����������Ӧ
}

void CChannelClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CChannelClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// ����������Ӧ
	// �ٻ�����Ӧ
	CNetClientSingle::OnSessionClose(pClient);
}

bool CChannelClient::Init(string strConfigFile, string strService, bool bLeftClient)
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
		LogImportant("CChannelClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	string strClientNode = "LeftClient";
	if (!bLeftClient)
	{
		strClientNode = "RightClient";
	}
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild(strClientNode.c_str());
	if (!txhNode.Element())
	{
		LogImportant("CChannelClient::Init(), \n fail to read <%s><%s>!", 
			strService.c_str(), strClientNode.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode) || !InitInCondition(txhNode))
	{
		return false;
	}

	if (GetType() != AF_UNIX)
	{
		LogImportant("CChannelClient::Init(), \n I need ServerType=AF_UNIX for service=%s!", strService.c_str());
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

bool CChannelClient::SendRawData( CBuffer* pBuf )
{
	return CNetClientSingle::SendRawData(pBuf);
}


