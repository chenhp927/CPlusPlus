//==============================================================================
//                      OuterServer.cpp
//                       
//begin   : 2016-09-13                                                 
//describe:                                              
//==============================================================================

#include "OuterServer.h"
#include "CommIdDef.h"
#include "OuterApp.h"
#include "ConnState.h"

//==============================================================================
//                 
//                    COuterServer
//                   CommOuter服务端类
//
//==============================================================================
COuterServer::COuterServer()
{ 
	SetMyClassName("COuterServer");

	m_strService = "";
} 
 
COuterServer::~COuterServer() 
{ 
}

bool COuterServer::Init(string strConfigFile, string strService)
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
		LogImportant("COuterServer::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// 找到需要启动的服务对应的配置信息节点
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("Server");
	if (!txhNode.Element())
	{
		LogImportant("COuterServer::Init(), \n fail to read <%s><Server>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode))
	{
		return false;
	}

	if (GetType() != AF_INET)
	{
		LogImportant("COuterServer::Init(), \n I need ServerType=AF_INET for service=%s!", strService.c_str());
		return false;
	}

	CSvrConnStateMgr::Instance().SetSvrInfo(m_pServer1->GetServerAddr(), m_pServer1->GetSvrIp(), 
			static_cast<unsigned short>(m_pServer1->GetSvrPort()), m_pServer2->GetSvrIp(), static_cast<unsigned short>(m_pServer2->GetSvrPort()));
	
	return true;
}

string COuterServer::GetServerIdValidRangeStr(bool bIsAppSvr)
{
	string str = "";
	str += CCommIdDef::GetServerIdValidRangeStr(bIsAppSvr, SERVERID_FOR_COMMOUTER);
	
	return str;
}

bool COuterServer::IsServerIdValid(bool bIsAppSvr, WORD wSvrId)
{
	return CCommIdDef::IsServerIdValid(wSvrId, bIsAppSvr, SERVERID_FOR_COMMOUTER);
}

string COuterServer::GetCliendIdValidRangeStr()
{
	string str = "";

	// 允许接入的业务程序客户端：无
	str += CCommIdDef::GetCliendIdValidRangeStr(true, CLIENTID_FOR_NULL);

	// 允许接入的通信程序客户端：外部通道桥接服务
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMBRIDGE);

	return str;
}

bool COuterServer::IsClientIdValid(bool bIsAppClt, WORD wCltId)
{
	if (bIsAppClt)
	{
		// 允许接入的业务程序客户端：无
		return CCommIdDef::IsClientIdValid(wCltId, true, CLIENTID_FOR_NULL);
	}
	else
	{
		// 允许接入的通信程序客户端：外部通道桥接服务
		return CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_COMMBRIDGE);
	}
}

void COuterServer::OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert (pCltObj && pBuf);

	// 获取转发层信息位置
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// 某个客户端的会话层数据长度有问题
		LogImportant("COuterServer::OnDealRecvData(), \n connection(%s) \n "
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
			LogImportant("COuterServer::OnDealRecvData(), \n connection(%s) \n "
				"receive one pkt, but it is not enough to get forward head!", 
				pCltObj->GetTagConnInfo().c_str());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
		else
		{
			CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
			
			LogDebug("COuterServer::OnDealRecvData(), \n connection(%s) \n receive one pkt(%s), I will forward it ...", 
				pCltObj->GetTagConnInfo().c_str(), objTypeFunc.GetTagDumpStr().c_str());
			LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
			
			// 从我的客户端接收的数据一律要向<内部数据转发服务>模块发送
			// 这样保证所有的数据都能到<内部数据转发服务>进行处理
			SendDataToCommInner(pCltObj, pBuf, objHead);

/* 对于<外部数据转发服务>来说，连入我的客户端都是通信程序客户端，所以没有必要转给我的其他客户端
			// 从我的一个客户端接收的数据，如果数据包支持本地转发或者含有目的地，则也需要向我的其他客户端发送
			if (objHead.IsSupportLocalDispatch()
				|| objHead.GetDestAddrs().size() > 0)
			{
				ForwardDataToMyOtherClients(pCltObj, pBuf, objHead);
			}
*/
		}
	}
	// 未知包
	else
	{
		// 未知包，丢弃
		LogImportant("COuterServer::OnDealRecvData(), \n connection(%s) \n "
			"receive one pkt, but it is unknown pkt.", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
}

void COuterServer::SendDataToCommInner(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();

	LogDebug("COuterServer::SendDataToCommInner(), \n forward pkt(%s) from my connection(%s) to CommInner server!", 
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());
		
	COuterApp* pApp = (COuterApp*)COuterApp::GetApp();
	assert(pApp);
	pApp->SendDataToCommInner(pBuf);
}

void COuterServer::OnRecvDataFromCommInner(CBuffer* pBuf)
{
	assert (pBuf);
	
	// 获取转发层信息位置
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// 某个客户端的会话层数据长度有问题
		LogImportant("COuterServer::OnRecvDataFromCommInner(), \n "
			"receive one pkt from CommInner, but it is not enough to "
			" get session head!");
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
			LogImportant("COuterServer::OnRecvDataFromCommInner(), \n "
				"receive one pkt from CommInner, but it is not enough to "
				"get forward head!");
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
		else
		{
			CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
			
			LogDebug("COuterServer::OnRecvDataFromCommInner(), \n receive one pkt(%s) from CommInner, I will forward it ...", 
				objTypeFunc.GetTagDumpStr().c_str());
			LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
			
			ForwardDataToMyClients(pBuf, objHead);
		}
	}
}

void COuterServer::ForwardDataToMyOtherClients(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert(pCltObj && pBuf);

	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("COuterServer::ForwardDataToMyOtherClients(), \n will forward pkt(%s) from connection(%s) to my others ...",
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());

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
		
		// 自己发的包肯定不会再返回
		if (pNetServerDoubleCltObj->GetRelatedCltObjPtr1() == pCltObj
			|| pNetServerDoubleCltObj->GetRelatedCltObjPtr2() == pCltObj)
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
		// 如果数据包没有目的地，则发送给所有注册条件满足的客户端
		if (objHead.GetDestAddrs().size() <= 0)
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
		}
		// 如果数据包有目的地，则只发送给注册条件符合并且目的地也符合的客户端
		else if (IsDataDestAddrMatchClt(objHead, pNetServerDoubleCltObj))
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
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
		LogImportant("COuterServer::ForwardDataToMyOtherClients(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	LogDebug("COuterServer::ForwardDataToMyOtherClients(), \n forward the data to %d clients. they are:\n%s",
			nSendClientCount, strSendClientInfo.c_str());
}

void COuterServer::ForwardDataToMyClients(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert(pBuf);
	
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("COuterServer::ForwardDataToMyClients(), \n will forward pkt(%s) from CommInner server to my clients ...",
		objTypeFunc.GetTagDumpStr().c_str());
	
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
		// 如果数据包没有目的地，则发送给所有注册条件满足的客户端
		if (objHead.GetDestAddrs().size() <= 0)
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
		}
		// 如果数据包有目的地，则只发送给注册条件符合并且目的地也符合的客户端
		else if (IsDataDestAddrMatchClt(objHead, pNetServerDoubleCltObj))
		{
			if (pNetServerDoubleCltObj->SendRawData(pBuf))
			{
				bSend = true;
			}
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
		LogImportant("COuterServer::ForwardDataToMyClients(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	LogDebug("COuterServer::ForwardDataToMyClients(), \n forward the data to %d clients. they are:\n%s",
			nSendClientCount, strSendClientInfo.c_str());
}

bool COuterServer::IsDataDestAddrMatchClt(CProtocolForward::CForwardHead& objHead, CNetServerDoubleCltObj* pDoubleCltObj)
{
	assert(pDoubleCltObj);
	CCommAddr* pCltAddr = pDoubleCltObj->GetClientAddr();
	assert(pCltAddr);

	vector<CProtocolForward::CForwardAddr> vecDstAddr;
	vecDstAddr = objHead.GetDestAddrs();
	for (int x = 0; x < vecDstAddr.size(); x++)
	{
		CProtocolForward::CForwardAddr* pDstAddr = &vecDstAddr[x];
		if (pDstAddr->IsMatched(*pCltAddr))
		{
			return true;
		}
	}

	return false;
}

bool COuterServer::Startup()
{
	return CNetServerDouble::Startup();
}

void COuterServer::OnSessionConnect( CServerOneNetCltObj* pCltObj )
{
	CNetServerDouble::OnSessionConnect(pCltObj);
}

void COuterServer::OnSessionReady( CServerOneNetCltObj* pCltObj )
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
		pCltObj->GetPeerIp(), pCltObj->GetPeerPort(), RM_CONNECTED);
}

void COuterServer::OnSessionClose( CServerOneNetCltObj* pCltObj )
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
		pCltObj->GetPeerIp(), pCltObj->GetPeerPort(), RM_UNCONNECT);	

	CNetServerDouble::OnSessionClose(pCltObj);
}

