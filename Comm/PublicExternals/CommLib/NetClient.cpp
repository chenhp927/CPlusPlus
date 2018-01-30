//==============================================================================
//                      NetClient.cpp
//                       
//begin   : 2016-09-03                                                 
//describe:                                              
//==============================================================================

#include "NetClient.h"

//==============================================================================
//                 
//                    CClientOneNet
//                    һ�����ͻ�����
//
//==============================================================================
CClientOneNet::CClientOneNet(CNetClientInterface* pParent, bool bIsForSingleNet) : 
m_pParent(pParent), m_bIsForSingleNet(bIsForSingleNet)
{
	SetMyClassName("CClientOneNet");
	
}

CClientOneNet::~CClientOneNet()
{
	
}

void CClientOneNet::OnConnect()
{
	CClientForwardBase::OnConnect();

	if (m_pParent)
	{
		m_pParent->OnConnect(this);
	}
}

void CClientOneNet::OnClose()
{
	if (m_pParent)
	{
		m_pParent->OnClose(this);
	}
	
	CClientForwardBase::OnClose();
}

void CClientOneNet::OnSessionReady()
{
	CClientForwardBase::OnSessionReady();
	
	if (m_pParent)
	{
		m_pParent->OnSessionReady(this);
	}
}

void CClientOneNet::OnSessionClose()
{
	if (m_pParent)
	{
		m_pParent->OnSessionClose(this);
	}
	
	CClientForwardBase::OnSessionClose();
}

void CClientOneNet::HandleUserData(CBuffer* pBuf)
{
	/* ���ػ��ຯ����������ڽ��յ����ݰ��������ն��У����ڵ�������������
	����������˫������Ҫ�����յ��İ�ֱ�ӷŵ�˫�����н��д���  */
	if (m_bIsForSingleNet)
	{
		CClientForwardBase::HandleUserData(pBuf);
	}
	else
	{
		m_pParent->OnRecvPacket(this, pBuf);
		pBuf->Release();
	}
}

//==============================================================================
//                 
//                    CNetClientSingle
//                      �����ͻ�����
//
//==============================================================================
CNetClientSingle::CNetClientSingle()
{
	SetMyClassName("CNetClientSingle");

	m_pClient = new CClientOneNet(this);
	assert(m_pClient);
	m_pClient->SetLogFileTag(GetLogFileTag());
}

CNetClientSingle::~CNetClientSingle()
{
	if (m_pClient)
	{
		delete m_pClient;
	}
}

bool CNetClientSingle::InitCommParam(TiXmlHandle& txhClient)
{
	return m_pClient->InitCommParam(txhClient);
}

bool CNetClientSingle::InitInCondition(TiXmlHandle& txhClient)
{
	return m_pClient->InitInCondition(txhClient);
}

bool CNetClientSingle::InitOutCondition(TiXmlHandle& txhClient)
{
	return m_pClient->InitOutCondition(txhClient);
}

int CNetClientSingle::GetType(char cNetNo)
{
	return m_pClient->GetType();
}

const char* CNetClientSingle::GetUnixPath(char cNetNo)
{
	return m_pClient->GetUnixPath();
}

string CNetClientSingle::GetSvrIp(char cNetNo)
{
	return m_pClient->GetSvrIp();
}

int CNetClientSingle::GetSvrPort(char cNetNo)
{
	return m_pClient->GetSvrPort();
}

string CNetClientSingle::GetMyIp(char cNetNo)
{
	return m_pClient->GetMyIp();
}

int CNetClientSingle::GetMyPort(char cNetNo)
{
	return m_pClient->GetMyPort();
}

WORD CNetClientSingle::GetClientId(char cNetNo)
{
	return m_pClient->GetClientId();
}

string CNetClientSingle::GetTagSvrInfo(char cNetNo)
{
	return m_pClient->GetTagSvrInfo();
}

string CNetClientSingle::GetTagConnInfo(char cNetNo)
{
	return m_pClient->GetTagConnInfo();
}

string CNetClientSingle::GetDumpParamStr()
{
	char szDump[2024];
	memset(szDump, 0, sizeof(szDump));
	
	snprintf(szDump, sizeof(szDump), "single client info:\n%s", m_pClient->GetDumpParamStr().c_str());
	
	return szDump;
}

void CNetClientSingle::DumpParam()
{
	LogImportant("CNetClientSingle::DumpParam(),\n%s", GetDumpParamStr().c_str());
	m_pClient->DumpRegPacket();
}

CCommAddr& CNetClientSingle::GetClientAddr()
{
	return m_pClient->GetClientAddr();
}

CForwardCondition* CNetClientSingle::GetOutCondition()
{
	return m_pClient->GetOutCondition();
}

CForwardCondition* CNetClientSingle::GetInCondition()
{
	return m_pClient->GetInCondition();
}

bool CNetClientSingle::CanGoOut(CProtocolForward::CForwardHead& objForwardHead)
{
	return m_pClient->CanGoOut(objForwardHead);
}

void CNetClientSingle::SetAppClient(bool bIsAppClient)
{
	m_pClient->SetAppClient(bIsAppClient);
}

bool CNetClientSingle::Connect()
{
	return m_pClient->Connect();
}

void CNetClientSingle::Close()
{
	m_pClient->Close();	
}
 
bool CNetClientSingle::IsConnect(char cNetNo)
{
	return m_pClient->IsConnect();
}

bool CNetClientSingle::IsReady(char cNetNo)
{
	return m_pClient->IsReady();
}

void CNetClientSingle::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);

	LogDebug("CNetClientSingle::OnConnect(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());
}

void CNetClientSingle::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CNetClientSingle::OnClose(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());
}
 
void CNetClientSingle::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CNetClientSingle::OnSessionReady(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());
}

void CNetClientSingle::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient);
	
	LogDebug("CNetClientSingle::OnSessionClose(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());
}

void CNetClientSingle::SetFdset(CFdset& objFs)
{
	m_pClient->SetFdset(objFs);
}

void CNetClientSingle::HandleNetEvent(CFdset& objFs)
{
	m_pClient->HandleNetEvent(objFs);
}
 
void CNetClientSingle::DoPatrol()
{
	m_pClient->DoPatrol();
}

bool CNetClientSingle::IsHasData()
{
	return m_pClient->IsHasData();
}

void CNetClientSingle::OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf)
{
	// ���봦��
}

bool CNetClientSingle::RecvData( char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr )
{
	CProtocolForward::CForwardHead objHead;
	bool bRet = m_pClient->RecvForwardData(objHead, szBuf, nBufLen);
	if (bRet)
	{
		objSrcAddr = objHead.GetSrcAddr();
	}

	return bRet;
}

bool CNetClientSingle::RecvData( CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr )
{
	CProtocolForward::CForwardHead objHead;
	bool bRet = m_pClient->RecvForwardData(objHead, pBuf);
	if (bRet)
	{
		objSrcAddr = objHead.GetSrcAddr();
	}
	
	return bRet;
}

bool CNetClientSingle::RecvData( char* szBuf, int& nBufLen )
{
	return m_pClient->RecvForwardData(szBuf, nBufLen);
}

bool CNetClientSingle::RecvData( CBuffer* pBuf )
{
	return m_pClient->RecvForwardData(pBuf);
}

bool CNetClientSingle::SendData( char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch/* = false*/)
{
	CProtocolForward::CForwardHead objHead;
	
	// ���ñ���ת�����
	if (bIsSupportLocalDispatch)
	{
		objHead.SetForwardType(2);
	}
	
	// ����Ŀ�ĵ�ַ
	for (int x = 0; x < vecDestAddr.size(); x++)
	{
		objHead.AddDest(vecDestAddr[x]);
	}
	
	// ����
	return m_pClient->SendForwardData(objHead, szBuf, nBufLen);
}

bool CNetClientSingle::SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch/* = false*/)
{
	assert(pBuf);
	
	return SendData(pBuf->GetBuffer(), pBuf->GetUsed(), vecDestAddr, bIsSupportLocalDispatch);
}

bool CNetClientSingle::SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch/* = false*/)
{
	assert(szBuf);
	
	vector<CProtocolForward::CForwardAddr> vecDestAddr;
	return SendData(szBuf, nBufLen, vecDestAddr, bIsSupportLocalDispatch);
}

bool CNetClientSingle::SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch/* = false*/)
{
	assert(pBuf);
	
	vector<CProtocolForward::CForwardAddr> vecDestAddr;
	return SendData(pBuf->GetBuffer(), pBuf->GetUsed(), vecDestAddr, bIsSupportLocalDispatch);
}

bool CNetClientSingle::SendRawData( CBuffer* pBuf )
{
	return m_pClient->SendData(pBuf);
}

bool CNetClientSingle::RecvRawData( CBuffer* pBuf )
{
	return m_pClient->RecvData(pBuf);
}

void CNetClientSingle::SetLogFileTag( const char* szFileTag )
{
	CObjectBase::SetLogFileTag(szFileTag);

	m_pClient->SetLogFileTag(szFileTag);
}

void CNetClientSingle::DumpAllocator()
{
	m_pClient->DumpAllocator();
}

//==============================================================================
//                 
//                   CNetClientDouble
//                    ˫���ͻ�����
//
//==============================================================================
CNetClientDouble::CNetClientDouble()
{
	SetMyClassName("CNetClientDouble");

	m_pClient1 = new CClientOneNet(this, false);
	m_pClient2 = new CClientOneNet(this, false);
	assert(m_pClient1 && m_pClient2);

	m_pClient1->SetLogFileTag(GetLogFileTag());
	
	m_pClient2->SetLogFileTag(GetLogFileTag());

	m_objRecvSeq1.Reset();
	m_objRecvSeq2.Reset();
	m_objRecvSeq.Reset();
	m_objSendSeq.Reset();
}

CNetClientDouble::~CNetClientDouble()
{
	if (m_pClient1)
	{
		delete m_pClient1;
	}
	
	if (m_pClient2)
	{
		delete m_pClient2;
	}	
}

bool CNetClientDouble::InitCommParam(TiXmlHandle& txhClient)
{
	bool bInit1 = m_pClient1->InitCommParam(txhClient, 1);
	bool bInit2 = m_pClient2->InitCommParam(txhClient, 2);

	m_pClient1->SetPairNetIp(m_pClient2->GetMyIp().c_str());
	m_pClient2->SetPairNetIp(m_pClient1->GetMyIp().c_str());
	
	return bInit1; // �ڶ��������������������
}

bool CNetClientDouble::InitInCondition(TiXmlHandle& txhClient)
{
	bool bInit1 = m_pClient1->InitInCondition(txhClient);
	bool bInit2 = m_pClient2->InitInCondition(txhClient);

	return bInit1; // �ڶ��������������������
}

bool CNetClientDouble::InitOutCondition(TiXmlHandle& txhClient)
{
	bool bInit1 = m_pClient1->InitOutCondition(txhClient);
	bool bInit2 = m_pClient2->InitOutCondition(txhClient);
	
	return bInit1; // �ڶ��������������������
}

int CNetClientDouble::GetType(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetType();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetType();
	}
	else
	{
		return 0;
	}
}

const char* CNetClientDouble::GetUnixPath(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetUnixPath();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetUnixPath();
	}
	else
	{
		return "";
	}
}

string CNetClientDouble::GetSvrIp(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetSvrIp();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetSvrIp();
	}
	else
	{
		return "";
	}
}

int CNetClientDouble::GetSvrPort(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetSvrPort();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetSvrPort();
	}
	else
	{
		return 0;
	}
}

string CNetClientDouble::GetMyIp(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetMyIp();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetMyIp();
	}
	else
	{
		return "";
	}
}

int CNetClientDouble::GetMyPort(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetMyPort();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetMyPort();
	}
	else
	{
		return 0;
	}
}

WORD CNetClientDouble::GetClientId(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetClientId();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetClientId();
	}
	else
	{
		return 0;
	}
}

string CNetClientDouble::GetTagSvrInfo(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetTagSvrInfo();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetTagSvrInfo();
	}
	else
	{
		string str = "net1: ";
		str += m_pClient1->GetTagSvrInfo();
		str += "\t\t net2: ";
		str += m_pClient2->GetTagSvrInfo();

		return str;
	}
}

string CNetClientDouble::GetTagConnInfo(char cNetNo /*= 1*/)
{
	if (cNetNo == 1)
	{
		return m_pClient1->GetTagConnInfo();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->GetTagConnInfo();
	}
	else
	{
		string str = "net1: ";
		str += m_pClient1->GetTagConnInfo();
		str += "\t\t net2: ";
		str += m_pClient2->GetTagConnInfo();
		
		return str;
	}
}

string CNetClientDouble::GetDumpParamStr()
{
	string strInfo;
	
	strInfo += string("double client1 info:\n");
	strInfo += m_pClient1->GetDumpParamStr();

	strInfo += string("double client2 info:\n");
	strInfo += m_pClient2->GetDumpParamStr();

	return strInfo;
}

void CNetClientDouble::DumpParam()
{
	// �����־�Ĵ�С�����ƣ���Ҫ�ֿ�����ͻ��˲�����Ϣ��DumpParamStr��
	// string strInfo = GetDumpParamStr();
	// LogImportant("CNetClientDouble::DumpParam(),\n%s", strInfo.c_str());
	
	assert(m_pClient1);
	assert(m_pClient2);

	string strClient1Info("double client1 info:\n");
	strClient1Info += m_pClient1->GetDumpParamStr();
	LogImportant("CNetClientDouble::DumpParam(),\n%s", strClient1Info.c_str());

	string strClient2Info("double client1 info:\n");
	strClient2Info += m_pClient2->GetDumpParamStr();
	LogImportant("CNetClientDouble::DumpParam(),\n%s", strClient2Info.c_str());

	m_pClient1->DumpRegPacket();
	m_pClient2->DumpRegPacket();
}

CCommAddr& CNetClientDouble::GetClientAddr()
{
	return m_pClient1->GetClientAddr();
}

CForwardCondition* CNetClientDouble::GetOutCondition()
{
	CForwardCondition* pCond1 = m_pClient1->GetOutCondition();
	CForwardCondition* pCond2 = m_pClient2->GetOutCondition();

	if (pCond1 && !pCond1->IsEmpty())
	{
		return pCond1;
	}
	else if (pCond2 && !pCond2->IsEmpty())
	{
		return pCond2;
	}

	return NULL;
}

CForwardCondition* CNetClientDouble::GetInCondition()
{
	CForwardCondition* pCond1 = m_pClient1->GetInCondition();
	CForwardCondition* pCond2 = m_pClient2->GetInCondition();
	
	if (pCond1 && !pCond1->IsEmpty())
	{
		return pCond1;
	}
	else if (pCond2 && !pCond2->IsEmpty())
	{
		return pCond2;
	}
	
	return NULL;
}

bool CNetClientDouble::CanGoOut(CProtocolForward::CForwardHead& objForwardHead)
{
	bool bCanGoOut1 = m_pClient1->CanGoOut(objForwardHead);
	bool bCanGoOut2 = m_pClient2->CanGoOut(objForwardHead);
	
	return bCanGoOut1 || bCanGoOut2;
}

void CNetClientDouble::SetAppClient(bool bIsAppClient)
{
	m_pClient1->SetAppClient(bIsAppClient);
	m_pClient2->SetAppClient(bIsAppClient);
}

bool CNetClientDouble::Connect()
{
	bool bConn1 = m_pClient1->Connect();
	bool bConn2 = m_pClient2->Connect();
	
	return bConn1 || bConn2;
}

void CNetClientDouble::Close()
{
	m_pClient1->Close();	
	m_pClient2->Close();	
}

bool CNetClientDouble::IsConnect(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->IsConnect();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->IsConnect();
	}
	else
	{
		return m_pClient1->IsConnect() || m_pClient2->IsConnect();
	}
}

bool CNetClientDouble::IsReady(char cNetNo)
{
	if (cNetNo == 1)
	{
		return m_pClient1->IsReady();
	}
	else if (cNetNo == 2)
	{
		return m_pClient2->IsReady();
	}
	else
	{
		return m_pClient1->IsReady() || m_pClient2->IsReady();
	}
}

void CNetClientDouble::OnConnect(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CNetClientDouble::OnConnect(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());
}

void CNetClientDouble::OnClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);

	LogDebug("CNetClientDouble::OnClose(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());

	if (pClient == m_pClient1)
	{
		m_objRecvSeq1.Reset();

		if (!m_pClient2->IsConnect())
		{
			// ˫�����������Ӷ��Ͽ��ˣ���Ҫ��������˫�����к�
			assert(!m_objRecvSeq2.IsValid());
			m_objRecvSeq.Reset();
			m_objSendSeq.Reset();

			ClearRecvQueue();
		}
	}
	else
	{
		assert(pClient == m_pClient2);
		m_objRecvSeq2.Reset();

		if (!m_pClient1->IsConnect())
		{
			// ˫�����������Ӷ��Ͽ��ˣ���Ҫ��������˫�����к�
			assert(!m_objRecvSeq1.IsValid());
			m_objRecvSeq.Reset();
			m_objSendSeq.Reset();

			ClearRecvQueue();
		}
	}
}

void CNetClientDouble::OnSessionReady(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CNetClientDouble::OnSessionReady(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());
}

void CNetClientDouble::OnSessionClose(CClientOneNet* pClient)
{
	assert(pClient == m_pClient1 || pClient == m_pClient2);
	
	LogDebug("CNetClientDouble::OnSessionClose(), \n connection(%s) \n", pClient->GetTagConnInfo().c_str());;
}

void CNetClientDouble::SetFdset(CFdset& objFs)
{
	m_pClient1->SetFdset(objFs);
	m_pClient2->SetFdset(objFs);
}

void CNetClientDouble::HandleNetEvent(CFdset& objFs)
{
	m_pClient1->HandleNetEvent(objFs);
	m_pClient2->HandleNetEvent(objFs);
}

void CNetClientDouble::DoPatrol()
{
	m_pClient1->DoPatrol();
	m_pClient2->DoPatrol();
}

bool CNetClientDouble::IsHasData()
{
	return !m_RecvQueue.IsEmpty();
}

bool CNetClientDouble::RecvData( char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr )
{
	bool bRet = false;
	
	CBuffer* pBufInQueue = NULL; 
	if (m_RecvQueue.PeekHead(pBufInQueue)) 
	{
		int nDataLen = pBufInQueue->GetUsed() - CProtocolSession::HeaderSize();
		if (nDataLen > nBufLen) 
		{ 
			nBufLen = nDataLen;
		}
		else if (m_RecvQueue.GetHead(pBufInQueue)) 
		{
			nDataLen = pBufInQueue->GetUsed() - CProtocolSession::HeaderSize();
			if (nDataLen > 0)
			{
				CProtocolForward::CForwardHead objForwardHead;
				char* pForward = pBufInQueue->GetBuffer() + CProtocolSession::HeaderSize();
				if (objForwardHead.FromStreamBuf(pForward, nDataLen) > 0)
				{
					int nMsgBodyLen = nDataLen - objForwardHead.StreamSize();
					if (nMsgBodyLen > 0)
					{
						char* pMsgBody = pForward + objForwardHead.StreamSize();
						memcpy(szBuf, pMsgBody, nMsgBodyLen); 
						nBufLen = nMsgBodyLen; 
						bRet = true;  
					}
				}
			}
			
			pBufInQueue->Release();
		} 
	} 
	
	return bRet; 
}

bool CNetClientDouble::RecvData( CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr )
{
	assert(pBuf);
	
	pBuf->Empty();
	
	bool bRet = false;
	CBuffer* pBufInQueue = NULL; 
	if (m_RecvQueue.GetHead(pBufInQueue)) 
	{
		int nDataLen = pBufInQueue->GetUsed() - CProtocolSession::HeaderSize();
		if (nDataLen > 0)
		{
			CProtocolForward::CForwardHead objForwardHead;
			char* pForward = pBufInQueue->GetBuffer() + CProtocolSession::HeaderSize();
			if (objForwardHead.FromStreamBuf(pForward, nDataLen) > 0)
			{
				int nMsgBodyLen = nDataLen - objForwardHead.StreamSize();
				if (nMsgBodyLen > 0)
				{
					objSrcAddr = objForwardHead.GetSrcAddr();
					char* pMsgBody = pForward + objForwardHead.StreamSize();
					pBuf->AddData(pMsgBody, nMsgBodyLen);
					bRet = true;
				}
			}
		}
		
		pBufInQueue->Release();
	}
	
	return bRet; 
}

bool CNetClientDouble::RecvData( char* szBuf, int& nBufLen )
{
	CProtocolForward::CForwardAddr objSrcAddr;
	return RecvData(szBuf, nBufLen, objSrcAddr);
}

bool CNetClientDouble::RecvData( CBuffer* pBuf )
{
	CProtocolForward::CForwardAddr objSrcAddr;
	return RecvData(pBuf, objSrcAddr);
}

bool CNetClientDouble::RecvRawData(CBuffer* pBuf)
{
	assert(pBuf);
	
	pBuf->Empty();
	
	bool bRet = true;
	CBuffer* pBufTmp = NULL; 
	if (m_RecvQueue.GetHead(pBufTmp)) 
	{
		pBuf->AddData(pBufTmp);
		pBufTmp->Release();
	}
	else
	{
		bRet = false;
	}
	
	return bRet; 
}

bool CNetClientDouble::SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch/* = false*/)
{
	assert(szBuf);
	
	CProtocolForward::CForwardHead objHead;

	// ���ñ���ת�����
	if (bIsSupportLocalDispatch)
	{
		objHead.SetForwardType(2);
	}

	// ����˫���������
	CProtocolForward::CForwardDataSeq objDataSeqSended = m_objSendSeq;
	objDataSeqSended.Inc(); // ����֮ǰ��������ȷ��˫���������
	objHead.SetDataSeq(objDataSeqSended);	

	LogDebug("CNetClientDouble::SendData(), \n double nets(%s, %s) \n set double_net_seq=%s ok!",
		m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str(), objDataSeqSended.GetDumpStr().c_str());

	// ����Ŀ�ĵ�ַ
	for (int x = 0; x < vecDestAddr.size(); x++)
	{
		objHead.AddDest(vecDestAddr[x]);
	}
	
	// �ֱ��͵�����������ʵ�����Ƿ��뷢�Ͷ�����
	bool bRet1 = m_pClient1->SendForwardData(objHead, szBuf, nBufLen);
	if (bRet1)
	{
		LogDebug("CNetClientDouble::SendData(), \n connection(%s) \n send ok!", 
			m_pClient1->GetTagConnInfo().c_str());
	}
	else
	{
		LogDebug("CNetClientDouble::SendData(), \n connection(%s) \n send fail!",
			m_pClient1->GetTagConnInfo().c_str());
	}
	
	bool bRet2 = m_pClient2->SendForwardData(objHead, szBuf, nBufLen);
	if (bRet2)
	{
		LogDebug("CNetClientDouble::SendData(), \n connection(%s) \n send ok!",
			m_pClient2->GetTagConnInfo().c_str());
	}
	else
	{
		LogDebug("CNetClientDouble::SendData(), \n connection(%s) \n send fail!",
			m_pClient2->GetTagConnInfo().c_str());
	}
	
	if (bRet1 || bRet2)
	{
		// ֻҪ�ɹ����͵�һ������˫���ķ������кž�Ҫ����
		m_objSendSeq = objDataSeqSended;
		return true;
	}
	else
	{
		LogImportant("CNetClientDouble::SendData(), \n double nets(%s, %s) \n send fail!",
			m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str());
		return false;
	}
}

bool CNetClientDouble::SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch/* = false*/)
{
	assert(pBuf);
	
	return SendData(pBuf->GetBuffer(), pBuf->GetUsed(), vecDestAddr, bIsSupportLocalDispatch);
}

bool CNetClientDouble::SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch/* = false*/)
{
	assert(szBuf);

	vector<CProtocolForward::CForwardAddr> vecDestAddr;
	return SendData(szBuf, nBufLen, vecDestAddr, bIsSupportLocalDispatch);
}

bool CNetClientDouble::SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch/* = false*/)
{
	assert(pBuf);

	vector<CProtocolForward::CForwardAddr> vecDestAddr;
	return SendData(pBuf->GetBuffer(), pBuf->GetUsed(), vecDestAddr, bIsSupportLocalDispatch);
}

bool CNetClientDouble::SendRawData(CBuffer* pBuf)
{
	assert(pBuf);
	
	// ��ȡת������Ϣλ��
	char* pForword = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
	int nForwardLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nForwardLen <= 0)
	{
		LogImportant("CNetClientDouble::SendRawData(), \n double nets(%s, %s) \n send data fail! the buf is not enough to get forward_head.", 
			m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str());
		
		return false;
	}
	
	// ����֮ǰ��������ȷ��˫���������
	CProtocolForward::CForwardDataSeq objDataSeqSended = m_objSendSeq;
	objDataSeqSended.Inc(); 
	
	// �����ݰ�����˫���������
	if (!CProtocolForward::SetDataSeq(pForword, nForwardLen, objDataSeqSended))
	{
		LogImportant("CNetClientDouble::SendRawData(), \n double nets(%s, %s) \n send data fail! fail to set double_net_seq.",
			m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str());
		return false;
	}
	else
	{
		LogDebug("CNetClientDouble::SendRawData(), \n double nets(%s, %s) \n set double_net_seq=%s ok!",
			m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str(), objDataSeqSended.GetDumpStr().c_str());
	}
	
	// �ֱ��͵�����������ʵ�����Ƿ��뷢�Ͷ�����
	bool bRet1 = m_pClient1->SendData(pBuf);
	if (bRet1)
	{
		LogDebug("CNetClientDouble::SendRawData(), \n connection(%s) \n send ok!", 
			m_pClient1->GetTagConnInfo().c_str());
	}
	else
	{
		LogDebug("CNetClientDouble::SendRawData(), \n connection(%s) \n send fail!", 
			m_pClient1->GetTagConnInfo().c_str());
	}
	
	bool bRet2 = m_pClient2->SendData(pBuf);
	if (bRet2)
	{
		LogDebug("CNetClientDouble::SendRawData(), \n connection(%s) \n send ok!", 
			m_pClient2->GetTagConnInfo().c_str());
	}
	else
	{
		LogDebug("CNetClientDouble::SendRawData(), \n connection(%s) \n send fail!", 
			m_pClient2->GetTagConnInfo().c_str());
	}
	
	if (bRet1 || bRet2)
	{
		// ֻҪ�ɹ����͵�һ������˫���ķ������кž�Ҫ����
		m_objSendSeq = objDataSeqSended;
		return true;
	}
	else
	{
		LogImportant("CNetClientDouble::SendRawData(), \n double nets(%s, %s) \n send fail!",
			m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str());
		return false;
	}
	
}

void CNetClientDouble::OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf)
{
	if (!DealSingleRecvSeq(pClient, pBuf))
	{
		return;
	}

	if (!DealDoubleRecvSeq(pClient))
	{
		return;
	}

	// �����ݰ����ӵ�������
	AddRecvPacketToQueue(pClient, pBuf);
}

bool CNetClientDouble::DealSingleRecvSeq(CClientOneNet* pClient, CBuffer* pBuf)
{
	CProtocolForward::CForwardDataSeq* pLastDataSeq = &m_objRecvSeq1;
	if (pClient == m_pClient2)
	{
		pLastDataSeq = &m_objRecvSeq2;
	}
	else
	{
		assert(pClient == m_pClient1);
	}
	
	// �����ݰ��л�ȡ˫�����к�
	CProtocolForward::CForwardDataSeq objDataSeqInPkt;
	int nOffset = CProtocolSession::HeaderSize() + CProtocolForward::CForwardDataSeq::Size();
	if (pBuf->GetUsed() < nOffset)
	{
		// ���ĳ���ͻ��˵����ݳ��������⣬��ô�����������⣬�رո�����
		LogImportant("CNetClientDouble::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but it is not enough to get double_net_seq. so will close client socket ...", 
			pClient->GetTagConnInfo().c_str());
		
		pClient->Close();
		return false;
	}
	
	nOffset = CProtocolSession::HeaderSize();
	if (!CProtocolForward::GetDataSeq(pBuf->GetBuffer() + nOffset, pBuf->GetUsed() - nOffset, objDataSeqInPkt))
	{
		LogImportant("CNetClientDouble::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but get double_net_seq fail! so will close client socket ...", 
			pClient->GetTagConnInfo().c_str());
		
		pClient->Close();
		return false;
	}
	
	// ���ÿͻ������ӽ��յ����������к��Ƿ���Ч
	if (!objDataSeqInPkt.IsValid()) 
	{
		LogImportant("CNetClientDouble::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but it's double_net_seq is invalid! so will close client socket ...", 
			pClient->GetTagConnInfo().c_str());

		pClient->Close();
		return false;
	}

	LogDebug("CNetClientDouble::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s), my last(%s).", 
		pClient->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
	
	// ����Ѿ��յ������к��Ǹ���Ч�ģ���ô������Ϊ�ոճ�ʼ�����
	if (!pLastDataSeq->IsValid()) 
	{
		LogDebug("CNetClientDouble::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s), my last(%s) is invalid, so update it.", 
			pClient->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
		
		// ���µ������ӵ�˫�����к�
		*pLastDataSeq = objDataSeqInPkt;

		return true;
	}
	
	// ���ÿͻ������ӵ�˫�����кŵ�������
	if (pLastDataSeq->GetNextSeq() == objDataSeqInPkt)
	{
		LogDebug("CNetClientDouble::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s) is continuous with my last(%s), so update it.", 
			pClient->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());

		// ���µ������ӵ�˫�����к�
		*pLastDataSeq = objDataSeqInPkt;

		return true;
	}
	else
	{
		LogImportant("CNetClientDouble::DealSingleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is not continuous with my last(%s)! so will close client socket ...", 
			pClient->GetTagConnInfo().c_str(), objDataSeqInPkt.GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());

		pClient->Close();
		return false;
	}
}

bool CNetClientDouble::DealDoubleRecvSeq(CClientOneNet* pClient)
{
	CProtocolForward::CForwardDataSeq* pLastDataSeq = &m_objRecvSeq;
	CProtocolForward::CForwardDataSeq objNeedDataSeq = pLastDataSeq->GetNextSeq();

	CProtocolForward::CForwardDataSeq* pRecvDataSeq = &m_objRecvSeq1;
	CProtocolForward::CForwardDataSeq* pPairDataSeq = &m_objRecvSeq2;
	if (pClient == m_pClient2)
	{
		pRecvDataSeq = &m_objRecvSeq2;
		pPairDataSeq = &m_objRecvSeq1;
	}
	else
	{
		assert(pClient == m_pClient1);
	}
	
	// ˫����������յ���һ��
	if (!pLastDataSeq->IsValid())
	{
		LogDebug("CNetClientDouble::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s), my last(%s) is invalid, so update it.", 
			pClient->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
		
		// ����˫���������к�
		*pLastDataSeq = *pRecvDataSeq;
		
		return true;
	}
	// ���յ��İ���������һ��
	else if (*pRecvDataSeq == objNeedDataSeq)
	{
		LogDebug("CNetClientDouble::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, received double_net_seq(%s) is continuous with my last(%s), so update it.", 
			pClient->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), pLastDataSeq->GetDumpStr().c_str());
		
		// ����˫���������к�
		*pLastDataSeq = *pRecvDataSeq;
		
		return true;
	}
	// ���յ����кž�����һ��̫Զ
	else if (pRecvDataSeq->IsFarawayFrom(objNeedDataSeq)) 
	{
		if (pRecvDataSeq->GetHigh() == 0 && pRecvDataSeq->GetLow() == 1)
		{
			LogImportant("CNetClientDouble::DealDoubleRecvSeq(), \n connection(%s) \n receive the first pkt, but received double_net_seq(%s) is far away from my needed(%s)! so will close double client socket ...", 
				pClient->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
			
			m_pClient1->Close();
			m_pClient2->Close();
		}
		else
		{
			LogImportant("CNetClientDouble::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is far away from my needed(%s)! so will close client socket ...", 
				pClient->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
			
			pClient->Close();
		}
	}
	// ���յ��İ����кű���һ��Ҫ��
	else if (*pRecvDataSeq > objNeedDataSeq) 
	{
		if (!pPairDataSeq->IsValid() || *pPairDataSeq > objNeedDataSeq)
		{
			// ��ʱ������client�Ľ������кŶ����ڵȴ��Ľ������к�
			// �������ܵȵ���һ������ʱҪ�Ͽ�����˫�����ӡ�
			LogImportant("CNetClientDouble::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but both received double_net_seq(%s) and pair client double_net_seq(%s) are bigger than my needed(%s)! so close will close double client socket ...", 
				pClient->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), pPairDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
			
			m_pClient1->Close();
			m_pClient2->Close();
		}
		else
		{
			// ��ʱ����Ȼ��ǰclient�Ľ������кŴ��ڵȴ��Ľ������кţ�����һ��client�Ľ�����Ų������ڵȴ��Ľ�����ţ�
			// ���Ի��п��ܵȵ���һ������ʱֻ�趪����ǰ���յĴ�����ŵİ����ɡ�
			LogDebug("CNetClientDouble::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is bigger than my needed(%s), so throw the pkt!", 
				pClient->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
		}
	}
	// ���յ��İ����кű���һ��ҪС
	else if (*pRecvDataSeq < objNeedDataSeq) 
	{
		// ��client�п��ܵ������Ͻ������к�
		LogDebug("CNetClientDouble::DealDoubleRecvSeq(), \n connection(%s) \n receive one pkt, but received double_net_seq(%s) is smaller than my needed(%s), so throw the pkt!", 
			pClient->GetTagConnInfo().c_str(), pRecvDataSeq->GetDumpStr().c_str(), objNeedDataSeq.GetDumpStr().c_str());
	}

	return false;
}

void CNetClientDouble::AddRecvPacketToQueue(CClientOneNet* pClient, CBuffer* pBuf)
{
	assert(pClient && pBuf);

	CBuffer* pRecvData = m_objBufAlloc.Allocate();
	assert(pRecvData);
	pRecvData->AddData(pBuf); // ��������
	
	if (!m_RecvQueue.AddTail(pRecvData)) 
	{
		LogImportant("CNetClientDouble::AddRecvPacketToQueue(), \n connection(%s) \n receive one pkt(len=%d), m_vecRecvQueue.AddTail() fail! ", 
			pClient->GetTagConnInfo().c_str(), pRecvData->GetUsed());
		LogDataImportant(pRecvData->GetBuffer(), pRecvData->GetUsed());
		
		pRecvData->Release();
		
		if (m_RecvQueue.IsOverflow()) 
		{
			LogImportant("CNetClientDouble::AddRecvPacketToQueue(), \n connection(%s) \n receive one pkt(len=%d), m_vecRecvQueue.IsOverflow()!",
				pClient->GetTagConnInfo().c_str(), pBuf->GetUsed());
			ClearRecvQueue(); 
		}
	}  
}

void CNetClientDouble::ClearRecvQueue() 
{ 
	// �������ǰ�����־
	int nLastPktCount = m_RecvQueue.GetSize();
	if (nLastPktCount > 0)
	{
		if (IsConnect())
		{
			LogImportant("CNetClientDouble::ClearRecvQueue(), \n double nets(%s, %s) \n it is connecting, queue_size=%d, will throw packets ...", 
				m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str(), nLastPktCount);
		}
		else
		{
			LogImportant("CNetClientDouble::ClearRecvQueue(), \n double nets(%s, %s) \n it is disconnected, queue_size=%d, will throw packets ...", 
				m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str(), nLastPktCount);
		}
	}
	
	// ִ�ж�����ղ���
	CBuffer* pBuf = NULL; 
	while (!m_RecvQueue.IsEmpty()) 
	{ 
		if (m_RecvQueue.GetHead(pBuf)) 
		{
// 			if (IsConnect())
// 			{
// 				LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
// 			}
			pBuf->Release(); 
		}
		else
		{
			break; 
		}
	} 
	
	// ������պ������־
	int nThrowPktCount = nLastPktCount - m_RecvQueue.GetSize();
	if (nThrowPktCount > 0 || m_RecvQueue.GetSize() > 0)
	{
		LogImportant("CNetClientDouble::ClearRecvQueue(), \n double nets(%s, %s) \n succeed to throw %d packets, now queue_size=%d.", 
			m_pClient1->GetTagConnInfo().c_str(), m_pClient2->GetTagConnInfo().c_str(), nThrowPktCount, m_RecvQueue.GetSize());
	}
}

void CNetClientDouble::SetLogFileTag( const char* szFileTag )
{
	CObjectBase::SetLogFileTag(szFileTag);

	m_pClient1->SetLogFileTag(szFileTag);
	m_pClient2->SetLogFileTag(szFileTag);
}

void CNetClientDouble::DumpAllocator()
{
//	LogImportant("CNetClientDouble::DumpAllocator(), dump CNetClientDouble::m_objBufAlloc\n%s", m_objBufAlloc.GetDumpStr().c_str());

//	m_pClient1->DumpAllocator();
//	m_pClient2->DumpAllocator();
}


 

