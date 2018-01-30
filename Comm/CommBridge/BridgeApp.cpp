//==============================================================================
//                      BridgeApp.cpp
//                       
//begin   : 2016-09-18                                                 
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
#include "BridgeApp.h"

// ������ȫ�ֱ���
CBridgeApp objApp;

//==============================================================================
//                 
//                    CBridgeApp
//                     ��������
//
//==============================================================================
CBridgeApp::CBridgeApp()
{
}

CBridgeApp::~CBridgeApp()
{
}

string CBridgeApp::GetAppVersion()
{
	// ������İ汾��Ϣ��!!!!��������������Ҫ�ڴ˴��޸İ汾��������Ϣ!!!!
	/* !!!!!!!!!!!!!!!!!!!!!��ʽ����ʾ�����ֶ��루�ÿո񣬲�Ҫ��tab��!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommBridge	      version 1.0.0      date 2016-09-18";
	strVer += "\n";
	
	// �����õ��Ŀ�İ汾��Ϣ��LIBVERΪ�궨�壬ÿ�����ﶼʵ�����Լ���İ汾��Ϣ��
	// !!!!!!�������ʹ�õĿⷢ���仯����Ҫͬ���޸Ĵ˴���ʵ��!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml��
	strVer += LIBVER(TOOLLIB);		// Tools��
	strVer += LIBVER(COMMLIB);		// ͨ�ſ�
	strVer += LIBVER(APPLIB);		// ��������
	
	return strVer;
}

void CBridgeApp::InitLogFile()
{
	// ================���ñ������ڲ��õ�����־�ļ���һЩ��Ϣ===========================
	// ��־�϶���߱Ƚ���Ҫ����Ҫ��¼����־��һЩ
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
	// ================���ñ������ڸ��������õ�����־�ļ�����===========================
}

bool CBridgeApp::InitComm()
{
	// ��ȡ�����ļ����ļ���������·����
	string strConfFile = CAppPath::GetPrivateConfPath();
	if (strConfFile.length() < 1)
	{
		LogImportant("CBridgeApp::InitComm(), \n Can't find private path!");
		return false;
	}
	else
	{
		strConfFile += CONF_PRIVATE_FILE_NAME;
		LogImportant("CBridgeApp::InitComm(), \n Find private file=%s", strConfFile.c_str());
	}
	
	// ��ʼ�����ؿͻ�����������
	m_objLocalClient.SetLogFileTag(GetLogFileTag());
	if (!m_objLocalClient.Init(strConfFile, m_strService))
	{
		LogImportant("CBridgeApp::InitComm(), \n init local client fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CBridgeApp::InitComm(), \n init local client ok ! ");
		m_objLocalClient.DumpParam();
	}
	
	// ��ʼ��Զ�̿ͻ�����������
	m_objRemoteClient.SetLogFileTag(GetLogFileTag());
	if (!m_objRemoteClient.Init(strConfFile, m_strService))
	{
		LogImportant("CBridgeApp::InitComm(), \n init remote client fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CBridgeApp::InitComm(), \n init remote client ok ! ");
		m_objRemoteClient.DumpParam();
	}
	
	return true;
}

bool CBridgeApp::Init()
{
	// �����ʼ��
	if(!CUnixApp::Init())
	{
		return false;
	}

	// ���m_strService�Ĵ�����
	if (m_strService.size() <= 0)
	{
		LogImportant("CBridgeApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// ��ʼ��ϵͳ��־�ļ���Ϣ
	InitLogFile();
	
	// ��ʼ��ͨ�ŷ���
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CBridgeApp::Init(), \n Init ok!");
	return true;
}

void CBridgeApp::Run()
{
	// ���ӿͻ���
	m_objLocalClient.Connect();
	m_objRemoteClient.Connect();
	
	// �������������̣�!!!!!ע��˴�����Ҫ��sleep�����ҽ�ֹ��sleep!!!!
	while(1)
	{
		// ��ʱ���������¼�
		OnTimerCheckNetEvent();
	}
}

void CBridgeApp::OnTimerCheckNetEvent()
{
	m_objFdset.Zero();
	m_objLocalClient.SetFdset(m_objFdset);
	m_objRemoteClient.SetFdset(m_objFdset); 

	int nRet = m_objFdset.Select(SELECT_TIMEOUT_COMMBRIDGE);
	if (nRet >= 0)
	{
		HandleLocalClientEvent();
		HandleRemoteClientEvent();
	}
	else if (nRet < 0)
	{
		LogImportant("CBridgeApp::OnTimerCheckNetEvent(), \n select error = %d!", ERRNO);
	}
}

void CBridgeApp::HandleLocalClientEvent()
{ 
	// �����¼�
	m_objLocalClient.HandleNetEvent(m_objFdset); 
	m_objLocalClient.DoPatrol(); 

	bool bHasSendData = false;
	while (m_objLocalClient.IsHasData()) 
	{ 
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		if (m_objLocalClient.RecvRawData(pBuf)) 
		{
			ForwardDataFromLocalToRemote(pBuf);
			bHasSendData = true;
		} 
		else  
		{ 
			LogImportant("CBridgeApp::HandleLocalClientEvent(), \n RecvData() fail!"); 
		} 
		pBuf->Release();
	}

	if (bHasSendData)
	{
		// ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
		m_objRemoteClient.DoPatrol(); 
	}
} 

void CBridgeApp::HandleRemoteClientEvent()
{ 
	// �����¼�
	m_objRemoteClient.HandleNetEvent(m_objFdset); 
	m_objRemoteClient.DoPatrol(); 

	bool bHasSendData = false;
	while (m_objRemoteClient.IsHasData()) 
	{ 
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		if (m_objRemoteClient.RecvRawData(pBuf)) 
		{ 
			ForwardDataFromRemoteToLocal(pBuf);
			bHasSendData = true;
		} 
		else  
		{ 
			LogImportant("CBridgeApp::HandleRemoteClientEvent(), \n RecvData() fail!"); 
		} 
		pBuf->Release();
	}

	if (bHasSendData)
	{
		// ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
		m_objLocalClient.DoPatrol(); 
	}
} 

void CBridgeApp::ForwardDataFromLocalToRemote(CBuffer* pBuf)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc;
	char* pForwardBuf = pBuf->GetBuffer() + CProtocolSession::CSessionHead::HeaderSize();
	int nForwardBufLen = pBuf->GetUsed() - CProtocolSession::CSessionHead::HeaderSize();
	CProtocolForward::GetForwardHeadTypeFunc(pForwardBuf, nForwardBufLen, objTypeFunc);

	LogDebug("CBridgeApp::ForwardDataFromLocalToRemote(), \n will forward pkt(%s) from \n local connection(%s) \n to \n remote connection(%s).", 
		objTypeFunc.GetTagDumpStr().c_str(), m_objLocalClient.GetTagConnInfo().c_str(), m_objRemoteClient.GetTagConnInfo().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());

	m_objRemoteClient.SendRawData(pBuf); 
}

void CBridgeApp::ForwardDataFromRemoteToLocal(CBuffer* pBuf)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc;
	char* pForwardBuf = pBuf->GetBuffer() + CProtocolSession::CSessionHead::HeaderSize();
	int nForwardBufLen = pBuf->GetUsed() - CProtocolSession::CSessionHead::HeaderSize();
	CProtocolForward::GetForwardHeadTypeFunc(pForwardBuf, nForwardBufLen, objTypeFunc);
	
	LogDebug("CBridgeApp::ForwardDataFromRemoteToLocal(), \n will forward pkt(%s) from \n remote connection(%s) \n to \n local connection(%s).", 
		objTypeFunc.GetTagDumpStr().c_str(), m_objRemoteClient.GetTagConnInfo().c_str(), m_objLocalClient.GetTagConnInfo().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	m_objLocalClient.SendRawData(pBuf); 
}

void CBridgeApp::OnTimer()
{
	// ��ʱ����ͨ��״̬��
	static DWORD tPrev = CUtility::GetUptime();
	if (CUtility::GetElapseTime(tPrev) > TIMER_COMMSTATE_INTERVAL)
	{
		//CSvrConnStateMgr::Instance().SendAllCommStatePkt();
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		bool bRet = MakeAllCommStatePkt(pBuf);
		if(!bRet)
		{
			LogDebug("CTermApp::HandleClientEvent(), \n Send CommState Pkt fail!");
			pBuf->Release();
			return;
		}
		
		m_objRemoteClient.SetAppClient(true);
		m_objRemoteClient.SendData(pBuf);
		pBuf->Release();
		m_objRemoteClient.SetAppClient(false);
		
		tPrev = CUtility::GetUptime();
	}
}

bool CBridgeApp::MakeAllCommStatePkt(CBuffer* pBuf)
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
	objConnInfo.SetConnType(0x01);
	objConnInfo.SetIp1(objSvrConnStateMgr.GetSvrIp1());
	objConnInfo.SetNet1Port(objSvrConnStateMgr.GetSvrPort1());
	objConnInfo.SetIp2(objSvrConnStateMgr.GetSvrIp2());
	objConnInfo.SetNet2Port(objSvrConnStateMgr.GetSvrPort2());
	
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

void CBridgeApp::AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState)
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
	//���ÿͻ���1��Ip��ַ�Ͷ˿�
	objConnState.SetIP1(objCltConnState.m_strCltIp1);
	objConnState.SetNet1Port(objCltConnState.m_sCltPort1);
	objConnState.SetNet1Conn(objCltConnState.m_bCltConn1);
	objConnState.SetNet1ConnStaChgTime(objCltConnState.m_objCltTime1);
	//���ÿͻ���2��Ip��ַ�Ͷ˿�
	objConnState.SetIP2(objCltConnState.m_strCltIp2);
	objConnState.SetNet2Port(objCltConnState.m_sCltPort2);
	objConnState.SetNet2Conn(objCltConnState.m_bCltConn2);
	objConnState.SetNet2ConnStaChgTime(objCltConnState.m_objCltTime2);
	objCltConnMainCommtoSys.SetConnState(objConnState);
}