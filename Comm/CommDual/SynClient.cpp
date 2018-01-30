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
//                   CommDual同步客户端类
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
	
	// 先基类响应
	CNetClientDouble::OnConnect(pClient);
	
	// 再派生类响应
}

void CSynClient::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CSynClient::OnClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先派生类响应
	// 再基类响应
	CNetClientDouble::OnClose(pClient);
}

void CSynClient::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CSynClient::OnSessionReady(), \n connection(%s)", pClient->GetTagConnInfo().c_str());
	
	// 先基类响应
	CNetClientDouble::OnSessionReady(pClient);
	
	// 再派生类响应
}

void CSynClient::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CSynClient::OnSessionClose(), \n connection(%s)", pClient->GetTagConnInfo().c_str());

	// 先派生类响应
	// 再基类响应
	CNetClientDouble::OnSessionClose(pClient);
}

bool CSynClient::Init(string strConfigFile, string strService)
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
		LogImportant("CSynClient::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// 找到需要启动的服务对应的配置信息节点
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
