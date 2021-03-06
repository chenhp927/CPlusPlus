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
//                   CommChannel客户端类
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
	
	// 先基类响应
	CNetClientSingle::OnConnect(pClient);
	
	// 再派生类响应
}

void CChannelClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CChannelClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先派生类响应
	// 再基类响应
	CNetClientSingle::OnClose(pClient);
}

void CChannelClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CChannelClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先基类响应
	CNetClientSingle::OnSessionReady(pClient);
	
	// 再派生类响应
}

void CChannelClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CChannelClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先派生类响应
	// 再基类响应
	CNetClientSingle::OnSessionClose(pClient);
}

bool CChannelClient::Init(string strConfigFile, string strService, bool bLeftClient)
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
		LogImportant("CChannelClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// 找到需要启动的服务对应的配置信息节点
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


