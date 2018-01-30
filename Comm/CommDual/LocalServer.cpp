//==============================================================================
//                      LocalServer.cpp
//                       
//begin   : 2016-09-10                                                 
//describe:                                              
//==============================================================================

#include "LocalServer.h"
#include "CommIdDef.h"
#include "DualApp.h"

//==============================================================================
//                 
//                    CLocalServer
//                    CommDual���ط������
//
//==============================================================================
CLocalServer::CLocalServer()
{
	SetMyClassName("CLocalServer");

	m_strService = "";

	// ���ñ������Ϊͨ�ų�������
	SetAppServer(false);
} 
 
CLocalServer::~CLocalServer() 
{ 
}

bool CLocalServer::Init(string strConfigFile, string strService)
{

	// �������Ƿ���Ч
	assert(strConfigFile.size() > 0 && strService.size() > 0);

	// ����Ϊ�������Ϊͨ�ų�������
	SetAppServer(false);

	// ���÷�������
	m_strService = strService;
	
	// ���ر�ģ���˽�������ļ���������������Ҫ����Ϣ
	TiXmlDocument objDoc(strConfigFile.c_str());
	if (!objDoc.LoadFile())
	{
		LogImportant("CLocalServer::Init(), \n fail to load config file=%s!", strConfigFile.c_str());
		return false;
	}

	TiXmlHandle objRoot(objDoc.RootElement());
	// �ҵ���Ҫ�����ķ����Ӧ��������Ϣ�ڵ�
	TiXmlHandle txhNode = objRoot.FirstChild(strService.c_str()).FirstChild("LocalServer");
	if (!txhNode.Element())
	{
		LogImportant("CLocalServer::Init(), \n fail to read <%s><LocalServer>!", strService.c_str());
		return false;
	}
	
	if (!InitCommParam(txhNode))
	{
		return false;
	}

	if (GetType() != AF_UNIX)
	{
		LogImportant("CLocalServer::Init(), \n I need ServerType=AF_UNIX for service=%s!", strService.c_str());
		return false;
	}

    //���÷���˵�ַ��Ϣ
	CSvrConnStateMgr::Instance().SetSvrInfo(GetServerAddr(), GetUnixPath());
		
	return true;
}

CServerBaseCltObj* CLocalServer::CreateClientObject() 
{ 
	CLocalServerCltObj* pCltObj = new CLocalServerCltObj(this);
	if (pCltObj)
	{
		pCltObj->SetLogFileTag(GetLogFileTag());
	}
	
	return pCltObj;
} 

string CLocalServer::GetServerIdValidRangeStr()
{
	string str = "";
	str += CCommIdDef::GetServerIdValidRangeStr(IsAppServer(), SERVERID_FOR_COMMDUAL);
	
	return str;
}

bool CLocalServer::IsServerIdValid()
{
	return CCommIdDef::IsServerIdValid(GetServerId(), IsAppServer(), SERVERID_FOR_COMMDUAL);
}

string CLocalServer::GetCliendIdValidRangeStr()
{
	string str = "";
	
	// ��������ҵ�����ͻ��ˣ�������ҵ�����
	str += CCommIdDef::GetCliendIdValidRangeStr(true, CLIENTID_FOR_SVRAPP);
	
	// ��������ͨ�ų���ͻ��ˣ��ⲿ����ת�������ն�����ת������
	// �ڲ�ͨ����ͨ�����ⲿͨ���Žӷ���
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMOUTER);
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMTERM);
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMCHANNEL);
	str += CCommIdDef::GetCliendIdValidRangeStr(false, CLIENTID_FOR_COMMBRIDGE);
	
	return str;
}

bool CLocalServer::IsClientIdValid(CServerSessionBaseCltObj* pCltObj)
{
	assert(pCltObj);
	WORD wCltId = pCltObj->GetClientId();
	
	if (pCltObj->IsAppClient())
	{
		// ��������ҵ�����ͻ��ˣ�������ҵ�����
		return CCommIdDef::IsClientIdValid(wCltId, true, CLIENTID_FOR_SVRAPP);
	}
	else
	{
		// ��������ͨ�ų���ͻ��ˣ��ⲿ����ת�������ն�����ת������
		// �ڲ�ͨ����ͨ�����ⲿͨ���Žӷ���
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

bool CLocalServer::IsClientIpValid(struct sockaddr_in& stuSockAddr) 
{ 
	return true; 
} 

bool CLocalServer::IsTooMuchSameClient(struct sockaddr_in& stuSockAddr) 
{ 
	return false; 
} 

void CLocalServer::HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf);
	assert(pCltObj->IsMyClassName("CLocalServerCltObj"));

	CLocalServerCltObj* pLocalServerCltObj = (CLocalServerCltObj*)pCltObj;

	// ��ȡת������Ϣλ��
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// ���ĳ���ͻ��˵ĻỰ�����ݳ��������⣬��ô�����������⣬�رո�����
		LogImportant("CLocalServer::HandleUserData(), \n connection(%s) \n "
			"receive one pkt, but it is not enough to get session head."
			" so will close client socket ...", pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		CloseClientObj(pCltObj);
		pBuf->Release(); 
		return;
	}
	char* p = pBuf->GetBuffer() + CProtocolSession::HeaderSize();

	// ע���
	if (CProtocolForward::IsMsgRegisterPkt(p, nDataLen))
	{
		// ����ע����Ϣ��
		if (pLocalServerCltObj->HandleRegisterPkt(p, nDataLen))
		{
			LogImportant("CLocalServer::HandleUserData(), \n connection(%s) \n "
				"deal register pkt ok! \n %s", pCltObj->GetTagConnInfo().c_str(),
				pLocalServerCltObj->GetForwardCondition().GetDumpStr().c_str());
		}
		else
		{
			LogImportant("CLocalServer::HandleUserData(), \n connection(%s) \n "
				"deal register pkt fail! ", pCltObj->GetTagConnInfo().c_str());
		}
	}
	// ת����
	else if (CProtocolForward::IsMsgForwardPkt(p, nDataLen))
	{
		// ����ת�����İ�ͷ
		CProtocolForward::CForwardHead objHead;
		if (objHead.FromStreamBuf(p, nDataLen) <= 0)
		{
			// ���ĳ���ͻ��˵�ת�������ݳ��������⣬��ô�����������⣬�رո�����
			LogImportant("CLocalServer::HandleUserData(), \n connection(%s) \n "
				"receive one pkt, but it is not enough to get forward head."
				" so will close client socket ...", pCltObj->GetTagConnInfo().c_str());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
			
			CloseClientObj(pCltObj);
		}
		else
		{
			// ����ת��
			ForwardDataToMyOtherClients(pLocalServerCltObj, pBuf, objHead);

			// ���͵�ͬ��ͨ��
			ForwardDataToSynConn(pLocalServerCltObj, pBuf, objHead);
		}
	}
	// δ֪��
	else
	{
		// δ֪��������
		LogImportant("CLocalServer::HandleUserData(), \n connection(%s) \n "
			"receive one pkt, but it is unknown pkt.", pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	pBuf->Release();
}

void CLocalServer::ForwardDataToMyOtherClients(CLocalServerCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert(pCltObj && pBuf);
	
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CLocalServer::ForwardDataToMyOtherClients(), \n will forward pkt(%s) from connection(%s) to my others ...",
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	int nActiveClientCount = 0;
	int nSendClientCount = 0;
	string strSendClientInfo = "";
	
	// ���������ҵ����пͻ���
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()�����ķ���ֵ���ܵ�CloseClient��Ӱ��
	for (int x = 0; x < nMaxClientBound; x++)
	{
		CLocalServerCltObj* pCltObjTmp = (CLocalServerCltObj*)m_ClientsPtr[x];
		
		// �ͻ���δ���ӣ��϶���������
		if (pCltObjTmp == NULL || !pCltObjTmp->IsReady())
		{
			continue;
		}
		
		// �Լ����İ��϶������ٷ���
		if (pCltObjTmp == pCltObj)
		{
			continue;
		}
		
		nActiveClientCount++;
		
		// ���ע������
		// ��ȡ�ͻ��˵�ע������
		assert(pCltObjTmp->IsMyClassName("CLocalServerCltObj"));
		CForwardCondition objCond = pCltObjTmp->GetForwardCondition();
		
		// ע�����������ϣ��϶���������
		if (!objCond.IsMatched(objHead))
		{
			continue;
		}
		
		// ���Ŀ�ĵ�����
		bool bNeedSend = false;
		// ��ҵ�����ͻ��ˣ���ͨ�ų���ͻ��ˣ�ֱ��ת�������ж�Ŀ�ĵ�
		if (!pCltObjTmp->IsAppClient())
		{
			bNeedSend = true;
		}
		// ������ݰ�û��Ŀ�ĵأ����͸�����ע����������Ŀͻ���
		else if (objHead.GetDestAddrs().size() <= 0)
		{
			bNeedSend = true;
		}
		// ������ݰ���Ŀ�ĵأ���ֻ���͸�ע���������ϲ���Ŀ�ĵ�Ҳ���ϵĿͻ���
		else if (IsDataDestAddrMatchClt(objHead, pCltObjTmp))
		{
			bNeedSend = true;
		}
		
		// ����
		if (bNeedSend && SendRawData(pBuf->GetBuffer(), pBuf->GetUsed(), pCltObjTmp))
		{
			nSendClientCount++;
			strSendClientInfo += pCltObjTmp->GetTagCltInfo().c_str();
			strSendClientInfo += "\n";
		}
	}
	
	if (nActiveClientCount <= 0)
	{
		LogImportant("CLocalServer::ForwardDataToMyOtherClients(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	LogDebug("CLocalServer::ForwardDataToMyOtherClients(), \n forward the data to %d clients. they are:\n%s",
		nSendClientCount, strSendClientInfo.c_str());
}

void CLocalServer::ForwardDataToSynConn(CLocalServerCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert(pCltObj && pBuf);

	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CLocalServer::ForwardDataToSynConn(), \n forward pkt(%s) from local connection(%s) to syn_conn ...",
		objTypeFunc.GetTagDumpStr().c_str(), pCltObj->GetTagConnInfo().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	CDualApp* pApp = (CDualApp*)CDualApp::GetApp();
	assert(pApp);
	pApp->OnRecvDataFromLocalConn(pBuf, objHead);
}

void CLocalServer::OnRecvDataFromSynConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	assert (pBuf);
	
	CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
	
	LogDebug("CLocalServer::OnRecvDataFromSynConn(), \n will forward pkt(%s) from syn_conn to my clients ...",
		objTypeFunc.GetTagDumpStr().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	int nActiveClientCount = 0;
	int nSendClientCount = 0;
	string strSendClientInfo = "";
	
	// ���������ҵ����пͻ���
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()�����ķ���ֵ���ܵ�CloseClient��Ӱ��
	for (int x = 0; x < nMaxClientBound; x++)
	{
		CLocalServerCltObj* pCltObjTmp = (CLocalServerCltObj*)m_ClientsPtr[x];

		// �ͻ���δ���ӣ��϶���������
		if (pCltObjTmp == NULL || !pCltObjTmp->IsReady())
		{
			continue;
		}
		
		nActiveClientCount++;
		
		// ���ע������
		// ��ȡ�ͻ��˵�ע������
		assert(pCltObjTmp->IsMyClassName("CLocalServerCltObj"));
		CForwardCondition objCond = pCltObjTmp->GetForwardCondition();

		// ע�����������ϣ��϶���������
		if (!objCond.IsMatched(objHead))
		{
			continue;
		}
		
		// ���Ŀ�ĵ�����
		bool bNeedSend = false;
		// ��ҵ�����ͻ��ˣ���ͨ�ų���ͻ��ˣ���ת�������ж�Ŀ�ĵ�
		if (!pCltObjTmp->IsAppClient())
		{
			continue;
		}
		// ������ݰ�û��Ŀ�ĵأ����͸�����ע����������Ŀͻ���
		else if (objHead.GetDestAddrs().size() <= 0)
		{
			bNeedSend = true;
		}
		// ������ݰ���Ŀ�ĵأ���ֻ���͸�ע���������ϲ���Ŀ�ĵ�Ҳ���ϵĿͻ���
		else if (IsDataDestAddrMatchClt(objHead, pCltObjTmp))
		{
			bNeedSend = true;
		}
		
		// ����
		if (bNeedSend && SendRawData(pBuf->GetBuffer(), pBuf->GetUsed(), pCltObjTmp))
		{
			nSendClientCount++;
			strSendClientInfo += pCltObjTmp->GetTagCltInfo().c_str();
			strSendClientInfo += "\n";
		}
	}

	if (nActiveClientCount <= 0)
	{
		LogImportant("CLocalServer::OnRecvDataFromSynConn(), \n no active client need the data.");
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
	
	LogDebug("CLocalServer::OnRecvDataFromSynConn(), \n forward the data to %d clients. they are:\n%s",
		nSendClientCount, strSendClientInfo.c_str());
}

bool CLocalServer::SendRawData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj)
{
	assert(pCltObj);
	assert(IsMyClientObj(pCltObj));
	
	if (!pCltObj->IsConnect()) 
	{ 
		LogImportant("CLocalServer::SendRawData(), \n connection(%s)\n it is disconnected, "
			"can not send data!", pCltObj->GetTagConnInfo().c_str()); 
		return false; 
	} 
	
	if (nBufLen <= 0)
	{
		LogImportant("CLocalServer::SendRawData(), \n connection(%s)\n send buf_len(%d) is error!",
			pCltObj->GetTagConnInfo().c_str(), nBufLen); 
		return false; 
	}
	
	// �ȷ����㹻�ڴ�
	CBuffer* pBuf = pCltObj->Allocate(); 
	if (!pBuf)
	{
		return false;
	}
	
	// ���������
	pBuf->AddData(szBuf, nBufLen); 
	
	// ��ӵ����Ͷ���	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	pCltObj->PushSendData(pBuf);
	
	return true; 
} 

bool CLocalServer::IsDataDestAddrMatchClt(CProtocolForward::CForwardHead& objHead, CLocalServerCltObj* pCltObj)
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

void CLocalServer::OnSessionReady(CSid objSid)
{
	CServerForwardBaseCltObj* pClient = (CServerForwardBaseCltObj*)GetClientObj(objSid);
	if(!pClient)
	{
		return;
	}
	
	m_lstSid.push_back(objSid);
	
	//������CommInner�Ŀͻ���һ��Ϊ��������˽�cNetNo��Ϊ1
	char cNetNo = 1;
	CSvrConnStateMgr::Instance().OnCltConnEvent(pClient->GetClientAddr(), GetUnixPath(), RM_CONNECTED);
	
	return CServerSessionBase::OnSessionReady(objSid);
}

void CLocalServer::OnSessionClose(CSid objSid)
{
	CServerForwardBaseCltObj* pClient = (CServerForwardBaseCltObj*)GetClientObj(objSid);
	if(!pClient)
	{
		return;
	}
	
	//��objSid�������ӵ�Sid������ȥ��
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

list<CSid>& CLocalServer::GetSid()
{
	return m_lstSid;
}

//==============================================================================
//                 
//                    CLocalServerCltObj
//                CommDual���ط������Ŀͻ��˶�����
//
//==============================================================================
CLocalServerCltObj::CLocalServerCltObj(CLocalServer* pServer) : 
	CServerForwardBaseCltObj(pServer)
{
	SetMyClassName("CLocalServerCltObj");
}

CLocalServerCltObj::~CLocalServerCltObj()
{
}
