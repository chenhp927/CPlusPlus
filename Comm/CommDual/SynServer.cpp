//==============================================================================
//                      SynServer.cpp
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================

#include "SynServer.h"
#include "CommIdDef.h"
#include "DualApp.h"
#include "ConnState.h"

//==============================================================================
//                 
//                    CSynServer
//                   CommDual同步服务端类
//
//==============================================================================
CSynServer::CSynServer()
{ 
	SetMyClassName("CSynServer");

	m_strService = "";
} 
 
CSynServer::~CSynServer() 
{ 
}

bool CSynServer::Init(string strConfigFile, string strService)
{
	// 检查参数是否有效
	assert(strConfigFile.size() > 0 && strService.size() > 0);

	// 设置为本服务端为通信程序服务端
	m_pServer1->SetAppServer(false);
	m_pServer2->SetAppServer(false);

	// 设置服务名称
	m_strService = strService;
	
	// 加载本模块的私有配置文件，读出连接所需要的信息
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("CSynServer::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// 找到需要启动的服务对应的配置信息节点
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("SynServer");
	if (!txhNode.Element())
	{
		LogImportant("CSynServer::Init(), \n fail to read <%s><SynServer>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode))
	{
		return false;
	}

	if (GetType() != AF_INET)
	{
		LogImportant("CSynServer::Init(), \n I need ServerType=AF_INET for service=%s!", strService.c_str());
		return false;
	}
	
	CSvrConnStateMgr::Instance().SetSvrInfo(m_pServer1->GetServerAddr(), m_pServer1->GetSvrIp(), 
		static_cast<unsigned short>(m_pServer1->GetSvrPort()), m_pServer2->GetSvrIp(), static_cast<unsigned short>(m_pServer2->GetSvrPort()));

	return true;
}

string CSynServer::GetServerIdValidRangeStr(bool bIsAppSvr)
{
	string str = "";
	str += CCommIdDef::GetServerIdValidRangeStr(bIsAppSvr, SERVERID_FOR_COMMDUAL);
	
	return str;
}

bool CSynServer::IsServerIdValid(bool bIsAppSvr, WORD wSvrId)
{
	return CCommIdDef::IsServerIdValid(wSvrId, bIsAppSvr, SERVERID_FOR_COMMDUAL);
}

string CSynServer::GetCliendIdValidRangeStr()
{
	string str = "";

	// 允许接入的业务程序客户端：无
	str += CCommIdDef::GetCliendIdValidRangeStr(true, CLIENTID_FOR_NULL);

	// 允许接入的通信程序客户端：双机同步交互服务
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMDUAL);

	return str;
}

bool CSynServer::IsClientIdValid(bool bIsAppClt, WORD wCltId)
{
	if (bIsAppClt)
	{
		// 允许接入的业务程序客户端：无
		return CCommIdDef::IsClientIdValid(wCltId, true, CLIENTID_FOR_NULL);
	}
	else
	{
		// 允许接入的通信程序客户端：双机同步交互服务
		return CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_COMMDUAL);
	}
}

void CSynServer::OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert (pCltObj && pBuf);

	// 获取转发层信息位置
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// 某个客户端的会话层数据长度有问题
		LogImportant("CSynServer::OnDealRecvData(), \n connection(%s) \n "
			"receive one pkt, but it is not enough to get session head!", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		return;
	}
	char* p = pBuf->GetBuffer() + CProtocolSession::HeaderSize();

	// 转发包
	if (CProtocolForward::IsMsgForwardPkt(p, nDataLen))
	{
		// 解析转发包的包头
		CProtocolForward::CForwardHead objHead;
		if (objHead.FromStreamBuf(p, nDataLen) <= 0)
		{
			// 如果某个客户端的转发层数据长度有问题，那么该连接有问题，关闭该连接
			LogImportant("CSynServer::OnDealRecvData(), \n connection(%s) \n "
				"receive one pkt, but it is not enough to get forward head!", 
				pCltObj->GetTagConnInfo().c_str());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
		else
		{
			CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
			
			LogDebug("CSynServer::OnDealRecvData(), \n connection(%s) \n receive one pkt(%s), I will forward it ...", 
				pCltObj->GetTagConnInfo().c_str(), objTypeFunc.GetTagDumpStr().c_str());
			LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
			
			// 从我的客户端接收的数据一律要向<本地服务端>发送，由<本地服务端>进行处理
			SendDataToLocalServer(pCltObj, pBuf, objHead);
		}
	}
	// 未知包
	else
	{
		// 未知包，丢弃
		LogImportant("CSynServer::OnDealRecvData(), \n connection(%s) \n "
			"receive one pkt, but it is unknown pkt.", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
}

void CSynServer::SendDataToLocalServer(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CSynServer::SendDataToLocalServer(), \n forward pkt(%s) from my server connection(%s) to local_server!", 
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());

	CDualApp* pApp = (CDualApp*)CDualApp::GetApp();
	assert(pApp);
	pApp->OnRecvDataFromSynConn(pBuf, objHead);
} 

bool CSynServer::Startup()
{
	return CNetServerDouble::Startup();
}

void CSynServer::OnSessionConnect( CServerOneNetCltObj* pCltObj )
{
	CNetServerDouble::OnSessionConnect(pCltObj);
}

void CSynServer::OnSessionReady( CServerOneNetCltObj* pCltObj )
{
	CNetServerDouble::OnSessionReady(pCltObj);

	char cNetNo = 0;
	if (pCltObj->GetRelatedServerPtr() == m_pServer1)
	{
		cNetNo = 1;
	}
	else
	{
		cNetNo = 2;
	}
	CSvrConnStateMgr::Instance().OnCltConnEvent(pCltObj->GetClientAddr(), cNetNo, 
		pCltObj->GetPeerIp(), static_cast<unsigned short>(pCltObj->GetPeerPort()), RM_CONNECTED);
}

void CSynServer::OnSessionClose( CServerOneNetCltObj* pCltObj )
{
	char cNetNo = 0;
	if (pCltObj->GetRelatedServerPtr() == m_pServer1)
	{
		cNetNo = 1;
	}
	else
	{
		cNetNo = 2;
	}
	CSvrConnStateMgr::Instance().OnCltConnEvent(pCltObj->GetClientAddr(), cNetNo, 
		pCltObj->GetPeerIp(), static_cast<unsigned short>(pCltObj->GetPeerPort()), RM_UNCONNECT);	

	CNetServerDouble::OnSessionClose(pCltObj);
}

void CSynServer::OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert (pBuf);

	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();

	LogDebug("CSynServer::OnRecvDataFromLocalConn(), \n receive one pkt(%s) from LocalConn, I will forward it ...", 
		objTypeFunc.GetTagDumpStr().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());

	int nActiveClientCount = 0;
	int nSendClientCount = 0;
	string strSendClientInfo = "";

	// 遍历接入我的所有客户端（双网）
	for (int x = 0; x < m_vecNetServerDoubleCltObj.size(); x++)
	{
		CNetServerDoubleCltObj* pNetServerDoubleCltObj = &m_vecNetServerDoubleCltObj[x];

		// 客户端未连接，肯定不发给它
		if (!pNetServerDoubleCltObj->IsReady())
		{
			continue;
		}

		nActiveClientCount++;

		// 获取客户端的注册条件
		CForwardCondition* pCond = pNetServerDoubleCltObj->GetForwardCondition();
		if (!pCond)
		{
			continue;
		}

		// 注册条件不符合，肯定不发给它
		if (!pCond->IsMatched(objHead))
		{
			continue;
		}

		bool bSend = false;
		if (pNetServerDoubleCltObj->SendRawData(pBuf))
		{
			bSend = true;
		}

		if (bSend)
		{
			nSendClientCount++;
			strSendClientInfo += pNetServerDoubleCltObj->GetTagCltInfo().c_str();
			strSendClientInfo += "\n";
		}
	}

	if (nActiveClientCount <= 0)
	{
		LogImportant("CSynServer::OnRecvDataFromLocalConn(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	LogDebug("CSynServer::OnRecvDataFromLocalConn(), \n forward the data to %d clients. they are:\n%s",
		nSendClientCount, strSendClientInfo.c_str());
}