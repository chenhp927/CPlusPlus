//==============================================================================
//                      InnerServer.cpp
//                       
//begin   : 2016-09-10                                                 
//describe:                                              
//==============================================================================

#include "InnerServer.h"
#include "CommIdDef.h"


//==============================================================================
//                 
//                    CInnerServer
//                    CommInner服务端类
//
//==============================================================================
CInnerServer::CInnerServer()
{
	SetMyClassName("CInnerServer");

	m_strService = "";

	// 设置本服务端为通信程序服务端
	SetAppServer(false);
} 
 
CInnerServer::~CInnerServer() 
{ 
}

bool CInnerServer::Init(string strConfigFile, string strService)
{

	// 检查参数是否有效
	assert(strConfigFile.size() > 0 && strService.size() > 0);

	// 设置为本服务端为通信程序服务端
	SetAppServer(false);

	// 设置服务名称
	m_strService = strService;
	
	// 加载本模块的私有配置文件，读出连接所需要的信息
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("CInnerServer::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// 找到需要启动的服务对应的配置信息节点
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("Server");
	if (!txhNode.Element())
	{
		LogImportant("CInnerServer::Init(), \n fail to read <%s><Server>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode))
	{
		return false;
	}

	if (GetType() != AF_UNIX)
	{
		LogImportant("CInnerServer::Init(), \n I need ServerType=AF_UNIX for service=%s!", strService.c_str());
		return false;
	}
	
	//设置服务端地址信息
	CSvrConnStateMgr::Instance().SetSvrInfo(GetServerAddr(), GetUnixPath());
	
	return true;
}

CServerBaseCltObj* CInnerServer::CreateClientObject() 
{ 
	CInnerServerCltObj* pCltObj = new CInnerServerCltObj(this);
	if (pCltObj)
	{
		pCltObj->SetLogFileTag(GetLogFileTag());
	}
	
	return pCltObj;
} 

string CInnerServer::GetServerIdValidRangeStr()
{
	string str = "";
	str += CCommIdDef::GetServerIdValidRangeStr(IsAppServer(), SERVERID_FOR_COMMINNER);
	
	return str;
}

bool CInnerServer::IsServerIdValid()
{
	return CCommIdDef::IsServerIdValid(GetServerId(), IsAppServer(), SERVERID_FOR_COMMINNER);
}

string CInnerServer::GetCliendIdValidRangeStr()
{
	string str = "";
	
	// 允许接入的业务程序客户端：无
	str += CCommIdDef::GetCliendIdValidRangeStr(true, CLIENTID_FOR_SVRAPP);
	
	// 允许接入的通信程序客户端：外部数据转发服务、终端数据转发服务、
	// 内部通道连通服务、外部通道桥接服务
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMOUTER);
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMTERM);
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMCHANNEL);
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMBRIDGE);
	
	return str;
}

bool CInnerServer::IsClientIdValid(CServerSessionBaseCltObj* pCltObj)
{
	assert(pCltObj);
	WORD wCltId = pCltObj->GetClientId();
	
	if (pCltObj->IsAppClient())
	{
		// 允许接入的业务程序客户端：服务类业务程序
		return CCommIdDef::IsClientIdValid(wCltId, true, CLIENTID_FOR_SVRAPP);
	}
	else
	{
		// 允许接入的通信程序客户端：外部数据转发服务、终端数据转发服务、
		// 内部通道连通服务、外部通道桥接服务
		if (CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_COMMOUTER) 
			|| CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_COMMTERM) 
			|| CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_COMMCHANNEL) 
			|| CCommIdDef::IsClientIdValid(wCltId, false, CLIENTID_FOR_COMMBRIDGE))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CInnerServer::IsClientIpValid(struct sockaddr_in& stuSockAddr) 
{ 
	return true; 
} 

bool CInnerServer::IsTooMuchSameClient(struct sockaddr_in& stuSockAddr) 
{ 
	return false; 
} 

void CInnerServer::HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf);
	assert(pCltObj->IsMyClassName("CInnerServerCltObj"));

	CInnerServerCltObj* pCommInnerCltObj = (CInnerServerCltObj*)pCltObj;

	// 获取转发层信息位置
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// 如果某个客户端的会话层数据长度有问题，那么该连接有问题，关闭该连接
		LogImportant("CInnerServer::HandleUserData(), \n connection(%s) \n "
			"receive one pkt, but it is not enough to get session head."
			" so will close client socket ...", pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		CloseClientObj(pCltObj);
		pBuf->Release(); 
		return;
	}
	char* p = pBuf->GetBuffer() + CProtocolSession::HeaderSize();

	// 注册包
	if (CProtocolForward::IsMsgRegisterPkt(p, nDataLen))
	{
		// 处理注册信息包
		if (pCommInnerCltObj->HandleRegisterPkt(p, nDataLen))
		{
			LogImportant("CInnerServer::HandleUserData(), \n connection(%s) \n "
				"deal register pkt ok! \n %s", pCltObj->GetTagConnInfo().c_str(),
				pCommInnerCltObj->GetForwardCondition().GetDumpStr().c_str());
		}
		else
		{
			LogImportant("CInnerServer::HandleUserData(), \n connection(%s) \n "
				"deal register pkt fail! ", pCltObj->GetTagConnInfo().c_str());
		}
	}
	// 转发包
	else if (CProtocolForward::IsMsgForwardPkt(p, nDataLen))
	{
		// 解析转发包的包头
		CProtocolForward::CForwardHead objHead;
		if (objHead.FromStreamBuf(p, nDataLen) <= 0)
		{
			// 如果某个客户端的转发层数据长度有问题，那么该连接有问题，关闭该连接
			LogImportant("CInnerServer::HandleUserData(), \n connection(%s) \n "
				"receive one pkt, but it is not enough to get forward head."
				" so will close client socket ...", pCltObj->GetTagConnInfo().c_str());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
			
			CloseClientObj(pCltObj);
		}
		else
		{
			// 处理转发信息包
			ForwardDataToMyOtherClients(pCommInnerCltObj, pBuf, objHead);
		}
	}
	// 未知包
	else
	{
		// 未知包，丢弃
		LogImportant("CInnerServer::HandleUserData(), \n connection(%s) \n "
			"receive one pkt, but it is unknown pkt.", pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	pBuf->Release();
}

void CInnerServer::ForwardDataToMyOtherClients(CInnerServerCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert(pCltObj && pBuf);

	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CInnerServer::ForwardDataToMyOtherClients(), \n will forward pkt(%s) from connection(%s) to my others ...",
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	int nActiveClientCount = 0;
	int nSendClientCount = 0;
	string strSendClientInfo = "";
	
	// 遍历接入我的所有客户端
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
	for (int x = 0; x < nMaxClientBound; x++)
	{
		CInnerServerCltObj* pCltObjTmp = (CInnerServerCltObj*)m_ClientsPtr[x];

		// 客户端未连接，肯定不发给它
		if (pCltObjTmp == NULL || !pCltObjTmp->IsReady())
		{
			continue;
		}
		
		// 自己发的包肯定不会再返回
		if (pCltObjTmp == pCltObj)
		{
			continue;
		}
		
		nActiveClientCount++;
		
		// 检查注册条件
		// 获取客户端的注册条件
		assert(pCltObjTmp->IsMyClassName("CInnerServerCltObj"));
		CForwardCondition objCond = pCltObjTmp->GetForwardCondition();

		// 注册条件不符合，肯定不发给它
		if (!objCond.IsMatched(objHead))
		{
			continue;
		}
		
		// 检查目的地条件
		bool bNeedSend = false;
		// 非业务程序客户端（即通信程序客户端）直接转发，不判定目的地
		if (!pCltObjTmp->IsAppClient())
		{
			bNeedSend = true;
		}
		// 如果数据包没有目的地，则发送给所有注册条件满足的客户端
		else if (objHead.GetDestAddrs().size() <= 0)
		{
			bNeedSend = true;
		}
		// 如果数据包有目的地，则只发送给注册条件符合并且目的地也符合的客户端
		else if (IsDataDestAddrMatchClt(objHead, pCltObjTmp))
		{
			bNeedSend = true;
		}
		
		// 发送
		if (bNeedSend && SendRawData(pBuf->GetBuffer(), pBuf->GetUsed(), pCltObjTmp))
		{
			nSendClientCount++;
			strSendClientInfo += pCltObjTmp->GetTagCltInfo().c_str();
			strSendClientInfo += "\n";
		}
	}

	if (nActiveClientCount <= 0)
	{
		LogImportant("CInnerServer::ForwardDataToMyOtherClients(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	LogDebug("CInnerServer::ForwardDataToMyOtherClients(), \n forward the data to %d clients. they are:\n%s",
		nSendClientCount, strSendClientInfo.c_str());
}

bool CInnerServer::SendRawData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj)
{
	assert(pCltObj);
	assert(IsMyClientObj(pCltObj));
	
	if (!pCltObj->IsConnect()) 
	{ 
		LogImportant("CInnerServer::SendRawData(), \n connection(%s)\n it is disconnected, "
			"can not send data!", pCltObj->GetTagConnInfo().c_str()); 
		return false; 
	} 
	
	if (nBufLen <= 0)
	{
		LogImportant("CInnerServer::SendRawData(), \n connection(%s)\n send buf_len(%d) is error!",
			pCltObj->GetTagConnInfo().c_str(), nBufLen); 
		return false; 
	}
	
	// 先分配足够内存
	CBuffer* pBuf = pCltObj->Allocate(); 
	if (!pBuf)
	{
		return false;
	}
	
	// 再添加数据
	pBuf->AddData(szBuf, nBufLen); 
	
	// 添加到发送队列	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	pCltObj->PushSendData(pBuf);
	
	return true; 
} 

bool CInnerServer::IsDataDestAddrMatchClt(CProtocolForward::CForwardHead& objHead, CInnerServerCltObj* pCltObj)
{
	assert(pCltObj);
	CCommAddr& objCltAddr = pCltObj->GetClientAddr();
	
	vector<CProtocolForward::CForwardAddr> vecDstAddr;
	vecDstAddr = objHead.GetDestAddrs();
	for (int x = 0; x < vecDstAddr.size(); x++)
	{
		CProtocolForward::CForwardAddr* pDstAddr = &vecDstAddr[x];
		if (pDstAddr->IsMatched(objCltAddr))
		{
			return true;
		}
	}
	
	return false;
}

void CInnerServer::OnSessionReady(CSid objSid)
{
	CServerForwardBaseCltObj* pClient = (CServerForwardBaseCltObj*)GetClientObj(objSid);
	if(!pClient)
	{
		return;
	}
	
	m_lstSid.push_back(objSid);
	
	//在这里CommInner的客户端一般为单网，因此将cNetNo设为1
	char cNetNo = 1;
	CSvrConnStateMgr::Instance().OnCltConnEvent(pClient->GetClientAddr(), GetUnixPath(), RM_CONNECTED);
	
	return CServerSessionBase::OnSessionReady(objSid);
}

void CInnerServer::OnSessionClose(CSid objSid)
{
	CServerForwardBaseCltObj* pClient = (CServerForwardBaseCltObj*)GetClientObj(objSid);
	if(!pClient)
	{
		return;
	}
	
	//将objSid从已连接的Sid集合中去掉
	list<CSid>::iterator iter = m_lstSid.begin();
	for(; iter!=m_lstSid.end(); ++iter)
	{
		if((*iter) == objSid)
		{
			m_lstSid.erase(iter);
			break;
		}
	}	
	
	char cNetNo = 1;	
	CSvrConnStateMgr::Instance().OnCltConnEvent(pClient->GetClientAddr(), GetUnixPath(), RM_UNCONNECT);
	
	return CServerSessionBase::OnSessionClose(objSid);
}

list<CSid>& CInnerServer::GetSid()
{
	return m_lstSid;
}

//==============================================================================
//                 
//                    CInnerServerCltObj
//                CommInner服务端类的客户端对象类
//
//==============================================================================
CInnerServerCltObj::CInnerServerCltObj(CInnerServer* pServer) : 
	CServerForwardBaseCltObj(pServer)
{
	SetMyClassName("CInnerServerCltObj");
}

CInnerServerCltObj::~CInnerServerCltObj()
{
}
