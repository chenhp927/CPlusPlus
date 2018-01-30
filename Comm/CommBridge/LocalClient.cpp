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
//                   CommBridge本地客户端类
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

	// 先基类响应
	CNetClientSingle::OnConnect(pClient);

	// 再派生类响应
}

void CLocalClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogImportant("CLocalClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());

	// 先派生类响应
	// 再基类响应
	CNetClientSingle::OnClose(pClient);
}

void CLocalClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogImportant("CLocalClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先基类响应
	CNetClientSingle::OnSessionReady(pClient);

	// 再派生类响应
}

void CLocalClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogImportant("CLocalClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先派生类响应
	// 再基类响应
	CNetClientSingle::OnSessionClose(pClient);
}

bool CLocalClient::Init(string strConfigFile, string strService)
{
	// 检查参数是否有效
	assert(strConfigFile.size() > 0 && strService.size() > 0);
	
	// 设置为本客户端为通信程序客户端
	m_pClient->SetAppClient(false);
	
	// 设置服务名称
	m_strService = strService;
	
	// 加载本模块的私有配置文件，读出连接所需要的信息
	bool bRet = false;
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("CLocalClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// 找到需要启动的服务对应的配置信息节点
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


