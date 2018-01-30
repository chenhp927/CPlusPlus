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
//                   CommBridge远程客户端类
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
	
	// 先基类响应
	CNetClientDouble::OnConnect(pClient);
	
	// 再派生类响应
}

void CRemoteClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CRemoteClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先派生类响应
	// 再基类响应
	CNetClientDouble::OnClose(pClient);
}

void CRemoteClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CRemoteClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先基类响应
	CNetClientDouble::OnSessionReady(pClient);
	
	// 再派生类响应
}

void CRemoteClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CRemoteClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());

	// 先派生类响应
	// 再基类响应
	CNetClientDouble::OnSessionClose(pClient);
}

bool CRemoteClient::Init(string strConfigFile, string strService)
{
	// 检查参数是否有效
	assert(strConfigFile.size() > 0 && strService.size() > 0);
	
	// 设置为本客户端为通信程序客户端
	m_pClient1->SetAppClient(false);
	m_pClient2->SetAppClient(false);
	
	// 设置服务名称
	m_strService = strService;
	
	// 加载本模块的私有配置文件，读出连接所需要的信息
	bool bRet = false;
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("CRemoteClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// 找到需要启动的服务对应的配置信息节点
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
