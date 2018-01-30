//==============================================================================
//                      DualApp.cpp
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================

#include <arpa/inet.h>

#include "PubMacro.h"
#include "XmlLibVer.h"
#include "ToolLibVer.h"
#include "CommLibVer.h"
#include "AppLibVer.h"
#include "Utility.h"
#include "MyDefine.h"
#include "AppPath.h"
#include "DualApp.h"
#include "ConnState.h"

// ������ȫ�ֱ���
CDualApp objApp;

//==============================================================================
//                 
//                    CDualApp
//                     ��������
//
//==============================================================================
CDualApp::CDualApp()
{
}

CDualApp::~CDualApp()
{
}

string CDualApp::GetAppVersion()
{
	// ������İ汾��Ϣ��!!!!��������������Ҫ�ڴ˴��޸İ汾��������Ϣ!!!!
	/* !!!!!!!!!!!!!!!!!!!!!��ʽ����ʾ�����ֶ��루�ÿո񣬲�Ҫ��tab��!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommDual            version 1.0.0      date 2016-10-20";
	strVer += "\n";
	
	// �����õ��Ŀ�İ汾��Ϣ��LIBVERΪ�궨�壬ÿ�����ﶼʵ�����Լ���İ汾��Ϣ��
	// !!!!!!�������ʹ�õĿⷢ���仯����Ҫͬ���޸Ĵ˴���ʵ��!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml��
	strVer += LIBVER(TOOLLIB);		// Tools��
	strVer += LIBVER(COMMLIB);		// ͨ�ſ�
	strVer += LIBVER(APPLIB);		// ��������
	
	return strVer;
}

void CDualApp::InitLogFile()
{
	// ================���ñ������ڲ��õ�����־�ļ���һЩ��Ϣ===========================
	// ��־�϶���߱Ƚ���Ҫ����Ҫ��¼����־��һЩ
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
	// ================���ñ������ڸ��������õ�����־�ļ�����===========================
}

bool CDualApp::InitComm()
{
	// ��ȡ�����ļ����ļ���������·����
	string strConfFile = CAppPath::GetPrivateConfPath();
	if (strConfFile.length() < 1)
	{
		LogImportant("CDualApp::InitComm(), \n Can't find private path!");
		return false;
	}
	else
	{
		strConfFile += CONF_PRIVATE_FILE_NAME;
		LogImportant("CDualApp::InitComm(), \n Find private file=%s", strConfFile.c_str());
	}
	
	// ��ʼ�����ط������������
	m_objLocalServer.SetLogFileTag(GetLogFileTag());
	if (!m_objLocalServer.Init(strConfFile, m_strService))
	{
		LogImportant("CDualApp::InitComm(), \n init local_server fail ! "
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CDualApp::InitComm(), \n init local_server ok ! ");
		m_objLocalServer.DumpParam();
	}
	
	// ��ʼ��ͬ���������������
	m_objSynServer.SetLogFileTag(GetLogFileTag());
	if (!m_objSynServer.Init(strConfFile, m_strService))
	{
		LogImportant("CDualApp::InitComm(), \n init syn_server fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CDualApp::InitComm(), \n init syn_server ok !");
		m_objSynServer.DumpParam();
	}
	
	// ��ʼ��ͬ���ͻ�����������
	m_objSynClient.SetLogFileTag(GetLogFileTag());
	if (!m_objSynClient.Init(strConfFile, m_strService))
	{
		LogImportant("CDualApp::InitComm(), \n init syn_client fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CDualApp::InitComm(), \n init syn_client ok ! ");
		m_objSynClient.DumpParam();
	}

	return true;
}

bool CDualApp::Init()
{
	// �����ʼ��
	if(!CUnixApp::Init())
	{
		return false;
	}

	// ���m_strService�Ĵ�����
	if (m_strService.size() <= 0)
	{
		LogImportant("CDualApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// ��ʼ��ϵͳ��־�ļ���Ϣ
	InitLogFile();
	
	// ��ʼ��ͨ�ŷ���
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CDualApp::Init(), \n Init ok!");
	return true;
}

void CDualApp::Run()
{
	// ���ӿͻ��ˡ������
	m_objLocalServer.Startup();
	m_objSynServer.Startup();
	m_objSynClient.Connect();
	
	// �������������̣�!!!!!ע��˴�����Ҫ��sleep�����ҽ�ֹ��sleep!!!!
	while(1)
	{
		// ��ʱ���������¼�
		OnTimerCheckNetEvent();

		// ��ʱ��
		OnTimer();
	}
}

void CDualApp::OnTimerCheckNetEvent()
{
	m_objFdset.Zero();
	m_objLocalServer.SetFdset(m_objFdset); 
	m_objSynServer.SetFdset(m_objFdset); 
	m_objSynClient.SetFdset(m_objFdset);

	int nRet = m_objFdset.Select(SELECT_TIMEOUT_COMMDUAL);
	if (nRet >= 0)
	{
		HandleLocalServerEvent();
		HandleSynServerEvent();
		HandleSynClientEvent();
	}
	else if (nRet < 0)
	{
		LogImportant("CDualApp::OnTimerCheckNetEvent(), \n select error = %d!", ERRNO);
	}
}

void CDualApp::HandleLocalServerEvent()
{
	m_objLocalServer.HandleListenNetEvent(m_objFdset); 
	m_objLocalServer.HandleClientNetEvent(m_objFdset); 
	m_objLocalServer.DoPatrol();

	m_objSynClient.DoPatrol(); // ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
} 

void CDualApp::HandleSynServerEvent()
{
	m_objSynServer.HandleListenNetEvent(m_objFdset); 
	m_objSynServer.HandleClientNetEvent(m_objFdset); 
	m_objSynServer.DoPatrol();
	
	m_objLocalServer.DoPatrol(); // ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
} 

void CDualApp::HandleSynClientEvent()
{ 
	// �����¼�
	m_objSynClient.HandleNetEvent(m_objFdset); 
	m_objSynClient.DoPatrol(); 

	bool bHasSendData = false;
	while (m_objSynClient.IsHasData()) 
	{ 
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		if (m_objSynClient.RecvRawData(pBuf)) 
		{
			OnRecvDataFromSynClient(pBuf);
			bHasSendData = true;
		} 
		else  
		{ 
			LogImportant("CDualApp::HandleSynClientEvent(), \n RecvData() fail!"); 
		} 
		pBuf->Release();
	}

	if (bHasSendData)
	{
		// ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
		m_objLocalServer.DoPatrol(); 
	}
} 

void CDualApp::OnTimer()
{
	// ��ʱ����ͨ��״̬��
	static DWORD tPrev = CUtility::GetUptime();
	if (CUtility::GetElapseTime(tPrev) > TIMER_COMMSTATE_INTERVAL)
	{
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		bool bRet = MakeAllCommStatePkt(pBuf);
		if(!bRet)
		{
			LogDebug("CInnerApp::HandleClientEvent(), \n Send CommState Pkt fail!");
		}
		
		SendConnStatePkt(pBuf);
		pBuf->Release();
		tPrev = CUtility::GetUptime();
	}
}

void CDualApp::OnRecvDataFromSynClient(CBuffer* pBuf) 
{ 
	assert (pBuf);
	
	// ��ȡת������Ϣλ��
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// ĳ���ͻ��˵ĻỰ�����ݳ���������
		LogImportant("CDualApp::OnRecvDataFromSynConn(), \n connection(%s) \n "
			"receive one pkt, but it is not enough to get session head!", 
			m_objSynClient.GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		return;
	}
	char* p = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
	
	// ת����
	if (CProtocolForward::IsMsgForwardPkt(p, nDataLen))
	{
		// ����ת�����İ�ͷ
		CProtocolForward::CForwardHead objHead;
		if (objHead.FromStreamBuf(p, nDataLen) <= 0)
		{
			// ���ĳ���ͻ��˵�ת�������ݳ��������⣬��ô�����������⣬�رո�����
			LogImportant("CDualApp::OnRecvDataFromSynConn(), \n connection(%s) \n "
				"receive one pkt, but it is not enough to get forward head!", 
				m_objSynClient.GetTagConnInfo().c_str());
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		}
		else
		{
			CProtocolForward::CForwardTypeFunc objTypeFunc = objHead.GetTypeFunc();
			
			LogDebug("CDualApp::OnRecvDataFromSynConn(), \n forward pkt(%s) from my client connection(%s) to local_server!", 
				objTypeFunc.GetTagDumpStr().c_str(), m_objSynClient.GetTagConnInfo().c_str());
			OnRecvDataFromSynConn(pBuf, objHead); 
		}
	}
	// δ֪��
	else
	{
		// δ֪��������
		LogImportant("CDualApp::OnRecvDataFromSynConn(), \n connection(%s) \n "
			"receive one pkt, but it is unknown pkt.", 
			m_objSynClient.GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
	}
}

void CDualApp::OnRecvDataFromSynConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	m_objLocalServer.OnRecvDataFromSynConn(pBuf, objHead);
}

void CDualApp::OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead)
{
	m_objSynServer.OnRecvDataFromLocalConn(pBuf, objHead);
}

void CDualApp::SendConnStatePkt(CBuffer* pBuf)
{
	assert(pBuf);
	list<CSid> objSid = m_objLocalServer.GetSid();
	LogDebug("++++++CommInner �����ӵĿͻ��˸���Ϊ:%d\n", objSid.size());
	
	list<CSid>::iterator iter = objSid.begin();
	
	for(; iter!=objSid.end(); ++iter)
	{
		CServerForwardBaseCltObj* pClient = (CServerForwardBaseCltObj*)m_objLocalServer.GetClientObj((*iter));
		if(!pClient)
		{
		    LogImportant("CInnerApp::SendConnStatePkt(), \n server(%s) \n send error! can not find client for objSid{%d,%d}", 
			m_objLocalServer.GetTagMyInfo().c_str(), (*iter).GetHigh(), (*iter).GetLow()); 
		    return; 
		}
		
		if(pClient->GetClientId() == RMINTERSYS_CLIENT_ID)
		{
			//��Ϊ�Ƿ��Ͱ����������ø÷����Ϊҵ���������
			m_objLocalServer.SetAppServer(true);
			m_objLocalServer.SendForwardData(pBuf, (*iter));
			m_objLocalServer.SetAppServer(false);
				
			break;
		}
	}
	
}

bool CDualApp::MakeAllCommStatePkt(CBuffer* pBuf)
{
	CMainSysSvcConnStatusPkt objMainSysSvcConnStatusPkt;
	
	CSvrConnStateMgr objSvrConnStateMgr = CSvrConnStateMgr::Instance();
	CCommAddr objSvrAddr = objSvrConnStateMgr.GetCommAddr();
	
	//����\��λ����\��λID\�豸����\�豸ID\AB����ʶ
	CDeviceInfo objDeviceInfo;
	objDeviceInfo.SetBureauId(objSvrAddr.GetBureauId());
	objDeviceInfo.SetUnitType(objSvrAddr.GetUnitType());
	objDeviceInfo.SetUnitId(objSvrAddr.GetUnitId());
	objDeviceInfo.SetDeviceType(objSvrAddr.GetDevType());
	objDeviceInfo.SetDeviceId(objSvrAddr.GetDevId());
	if(objSvrAddr.GetAbFlag() == HOST_A_FLAG)
	{
		objDeviceInfo.SetAbFlag(0x00);
	}
	else
	{
		objDeviceInfo.SetAbFlag(0x01);
	}
	//���÷������ĵ�ַ��Ϣ
	objMainSysSvcConnStatusPkt.SetDeviceInfo(objDeviceInfo);
	
	CSvrConnState objSvrConnState;
	//���÷����ID
	CProgramServer objSvc;
	objSvc.SetSvcID(objSvrAddr.GetCltId());
	objSvrConnState.SetProgramServer(objSvc);
	
	//���÷���˵�ַ�Ͷ˿�
	CConnInfo objConnInfo;
	objConnInfo.SetConnType(0x02);
	objConnInfo.SetConnPath(objSvrConnStateMgr.GetUnixPath());
	
	objSvrConnState.SetConnInfo(objConnInfo);

	//��ӿͻ�������״̬
	list<CCltConnState> objListCltConnState = objSvrConnStateMgr.GetConnState();
	list<CCltConnState>::iterator iter = objListCltConnState.begin();
	for (; iter!=objListCltConnState.end(); ++iter)
	{
		CClientConnMainCommtoSys objCltConnMainCommtoSys;
		AddClientConnState(objCltConnMainCommtoSys, (*iter));
		objSvrConnState.AddClient(objCltConnMainCommtoSys);
	}
	//���÷��������״̬
	objMainSysSvcConnStatusPkt.SetSvrConnState(objSvrConnState);
	
	int nBufLen = objMainSysSvcConnStatusPkt.StreamSize();
	pBuf->Extend(nBufLen);
	nBufLen = pBuf->GetBufSize();
	
	int nRet = objMainSysSvcConnStatusPkt.ToStreamBuf(pBuf->GetBuffer(), nBufLen);
	if (nRet == -1)
	{
		LogImportant("Pkt(type=0x%02x,func=0x%02x) ToStreamBuf err, send fail!", 
		objMainSysSvcConnStatusPkt.GetType(), objMainSysSvcConnStatusPkt.GetFunc());
		return false;
	}
	pBuf->Use(nRet);
	
	return true;
}

void CDualApp::AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState)
{
	//���ÿͻ��˵�ַ
	CCommAddr objCommAddr = objCltConnState.m_objCltAddr;
	CForwardAddr objForwardAddr;
	objForwardAddr.SetBureauId(objCommAddr.GetBureauId());
	objForwardAddr.SetUnitType(objCommAddr.GetUnitType());
	objForwardAddr.SetUnitId(objCommAddr.GetUnitId());
	objForwardAddr.SetDevType(objCommAddr.GetDevType());
	objForwardAddr.SetDevId(objCommAddr.GetDevId());
	objForwardAddr.SetCltId(objCommAddr.GetCltId());
	objCltConnMainCommtoSys.SetForwardAddr(objForwardAddr);
	
	//�ͻ�������״̬
	CConnState objConnState;
	objConnState.SetAF_UNIXState(objCltConnState.m_bUnixCltConn);
	objConnState.SetAF_UNIXChgTime(objCltConnState.m_objUnixCltTime);
	objCltConnMainCommtoSys.SetConnState(objConnState);
}



