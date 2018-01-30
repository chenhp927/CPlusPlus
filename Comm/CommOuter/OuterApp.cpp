//==============================================================================
//                      OuterApp.cpp
//                       
//begin   : 2016-09-13                                                 
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
#include "OuterApp.h"
#include "ConnState.h"

// ������ȫ�ֱ���
COuterApp objApp;

//==============================================================================
//                 
//                    COuterApp
//                     ��������
//
//==============================================================================
COuterApp::COuterApp()
{
}

COuterApp::~COuterApp()
{
}

string COuterApp::GetAppVersion()
{
	// ������İ汾��Ϣ��!!!!��������������Ҫ�ڴ˴��޸İ汾��������Ϣ!!!!
	/* !!!!!!!!!!!!!!!!!!!!!��ʽ����ʾ�����ֶ��루�ÿո񣬲�Ҫ��tab��!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommOuter           version 1.0.0      date 2016-09-13";
	strVer += "\n";
	
	// �����õ��Ŀ�İ汾��Ϣ��LIBVERΪ�궨�壬ÿ�����ﶼʵ�����Լ���İ汾��Ϣ��
	// !!!!!!�������ʹ�õĿⷢ���仯����Ҫͬ���޸Ĵ˴���ʵ��!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml��
	strVer += LIBVER(TOOLLIB);		// Tools��
	strVer += LIBVER(COMMLIB);		// ͨ�ſ�
	strVer += LIBVER(APPLIB);		// ��������
	
	return strVer;
}

void COuterApp::InitLogFile()
{
	// ================���ñ������ڲ��õ�����־�ļ���һЩ��Ϣ===========================
	// ��־�϶���߱Ƚ���Ҫ����Ҫ��¼����־��һЩ
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
		
	// ================���ñ������ڸ��������õ�����־�ļ�����===========================
}

bool COuterApp::InitComm()
{
	// ��ȡ�����ļ����ļ���������·����
	string strConfFile = CAppPath::GetPrivateConfPath();
	if (strConfFile.length() < 1)
	{
		LogImportant("COuterApp::InitComm(), \n Can't find private path!");
		return false;
	}
	else
	{
		strConfFile += CONF_PRIVATE_FILE_NAME;
		LogImportant("COuterApp::InitComm(), \n Find private file=%s", strConfFile.c_str());
	}
	
	// ��ʼ���ͻ�����������
	m_objClient.SetLogFileTag(GetLogFileTag());
	if (!m_objClient.Init(strConfFile, m_strService))
	{
		LogImportant("COuterApp::InitComm(), \n init client fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("COuterApp::InitComm(), \n init client ok ! ");
		m_objClient.DumpParam();
	}
	
	// ��ʼ���������������
	m_objServer.SetLogFileTag(GetLogFileTag());
	if (!m_objServer.Init(strConfFile, m_strService))
	{
		LogImportant("COuterApp::InitComm(), \n init server fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("COuterApp::InitComm(), \n init server ok !");
		m_objServer.DumpParam();
	}

	return true;
}

bool COuterApp::Init()
{
	// �����ʼ��
	if(!CUnixApp::Init())
	{
		return false;
	}

	// ���m_strService�Ĵ�����
	if (m_strService.size() <= 0)
	{
		LogImportant("COuterApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// ��ʼ��ϵͳ��־�ļ���Ϣ
	InitLogFile();
	
	// ��ʼ��ͨ�ŷ���
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("COuterApp::Init(), \n Init ok!");
	return true;
}

void COuterApp::Run()
{
	// ���ӿͻ��ˡ������
	m_objClient.Connect();
	m_objServer.Startup();
	
	// �������������̣�!!!!!ע��˴�����Ҫ��sleep�����ҽ�ֹ��sleep!!!!
	while(1)
	{
		// ��ʱ���������¼�
		OnTimerCheckNetEvent();

		// ��ʱ��
		OnTimer();
	}
}

void COuterApp::OnTimerCheckNetEvent()
{
	m_objFdset.Zero();
	m_objClient.SetFdset(m_objFdset);
	m_objServer.SetFdset(m_objFdset); 

	int nRet = m_objFdset.Select(SELECT_TIMEOUT_COMMOUTER);
	if (nRet >= 0)
	{
		HandleClientEvent();
		HandleServerEvent();
	}
	else if (nRet < 0)
	{
		LogImportant("COuterApp::OnTimerCheckNetEvent(), \n select error = %d!", ERRNO);
	}
}

void COuterApp::HandleServerEvent()
{
	m_objServer.HandleListenNetEvent(m_objFdset); 
	m_objServer.HandleClientNetEvent(m_objFdset); 
	m_objServer.DoPatrol();

	m_objClient.DoPatrol(); // ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
} 

void COuterApp::HandleClientEvent()
{ 
	// �����¼�
	m_objClient.HandleNetEvent(m_objFdset); 
	m_objClient.DoPatrol(); 

	bool bHasSendData = false;
	while (m_objClient.IsHasData()) 
	{ 
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		if (m_objClient.RecvRawData(pBuf)) 
		{ 
			m_objServer.OnRecvDataFromCommInner(pBuf); 
			bHasSendData = true;
		} 
		else  
		{ 
			LogImportant("COuterApp::HandleClientEvent(), \n RecvData() fail!"); 
		} 
		pBuf->Release();
	}

	if (bHasSendData)
	{
		// ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
		m_objServer.DoPatrol(); 
	}
} 

void COuterApp::OnTimer()
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
		
		m_objClient.SetAppClient(true);
		m_objClient.SendData(pBuf);
		pBuf->Release();
		m_objClient.SetAppClient(false);
		
		tPrev = CUtility::GetUptime();
	}
	
	// ��ӡCBufferʹ�����
	static DWORD tPrevPrint = CUtility::GetUptime();
	if (CUtility::GetElapseTime(tPrevPrint) > 5)
	{
		tPrevPrint = CUtility::GetUptime();

		LogImportant("CTermApp::OnTimer(), dump CTermApp::m_objBufAlloc\n%s", m_objBufAlloc.GetDumpStr().c_str());

		m_objClient.DumpAllocator();
		m_objServer.DumpAllocator();
	}
	
}

void COuterApp::SendDataToCommInner( CBuffer* pBuf )
{
	m_objClient.SendRawData(pBuf);
}

bool COuterApp::MakeAllCommStatePkt(CBuffer* pBuf)
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

void COuterApp::AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState)
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
