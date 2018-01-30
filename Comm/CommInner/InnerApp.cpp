//==============================================================================
//                      InnerApp.cpp
//                       
//begin   : 2016-09-10                                                 
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
#include "InnerApp.h"

// ������ȫ�ֱ���
CInnerApp objApp;

//==============================================================================
//                 
//                    CInnerApp
//                     ��������
//
//==============================================================================
CInnerApp::CInnerApp()
{
}

CInnerApp::~CInnerApp()
{
}

string CInnerApp::GetAppVersion()
{
	// ������İ汾��Ϣ��!!!!��������������Ҫ�ڴ˴��޸İ汾��������Ϣ!!!!
	/* !!!!!!!!!!!!!!!!!!!!!��ʽ����ʾ�����ֶ��루�ÿո񣬲�Ҫ��tab��!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommInner           version 1.0.0      date 2016-09-10";
	strVer += "\n";
	
	// �����õ��Ŀ�İ汾��Ϣ��LIBVERΪ�궨�壬ÿ�����ﶼʵ�����Լ���İ汾��Ϣ��
	// !!!!!!�������ʹ�õĿⷢ���仯����Ҫͬ���޸Ĵ˴���ʵ��!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml��
	strVer += LIBVER(TOOLLIB);		// Tools��
	strVer += LIBVER(COMMLIB);		// ͨ�ſ�
	strVer += LIBVER(APPLIB);		// ��������
	
	return strVer;
}

void CInnerApp::InitLogFile()
{
	// ================���ñ������ڲ��õ�����־�ļ���һЩ��Ϣ===========================
	// ��־�϶���߱Ƚ���Ҫ����Ҫ��¼����־��һЩ
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
		
	// ================���ñ������ڸ��������õ�����־�ļ�����===========================
}

bool CInnerApp::InitComm()
{
	// ��ȡ�����ļ����ļ���������·����
	string strConfFile = CAppPath::GetPrivateConfPath();
	if (strConfFile.length() < 1)
	{
		LogImportant("CInnerApp::InitComm(), \n Can't find private path!");
		return false;
	}
	else
	{
		strConfFile += CONF_PRIVATE_FILE_NAME;
		LogImportant("CInnerApp::InitComm(), \n Find private file=%s", strConfFile.c_str());
	}
	
	// ��ʼ���������������
	m_objServer.SetLogFileTag(GetLogFileTag());
	if (!m_objServer.Init(strConfFile, m_strService))
	{
		LogImportant("CInnerApp::InitComm(), \n init server fail ! "
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CInnerApp::InitComm(), \n init server ok ! ");
		m_objServer.DumpParam();
	}
	
	return true;
}

bool CInnerApp::Init()
{
	// �����ʼ��
	if(!CUnixApp::Init())
	{
		return false;
	}

	// ���m_strService�Ĵ�����
	if (m_strService.size() <= 0)
	{
		LogImportant("CInnerApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// ��ʼ��ϵͳ��־�ļ���Ϣ
	InitLogFile();
	
	// ��ʼ��ͨ�ŷ���
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CInnerApp::Init(), \n Init ok!");
	return true;
}

void CInnerApp::Run()
{
	// ���������
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

void CInnerApp::OnTimerCheckNetEvent()
{
	m_objFdset.Zero();
	m_objServer.SetFdset(m_objFdset); 

	int nRet = m_objFdset.Select(SELECT_TIMEOUT_COMMINNER);
	if (nRet >= 0)
	{
		HandleServerEvent();
	}
	else if (nRet < 0)
	{
		LogImportant("CInnerApp::OnTimerCheckNetEvent(), \n select error = %d!", ERRNO);
	}
}

void CInnerApp::HandleServerEvent()
{
	m_objServer.HandleListenNetEvent(m_objFdset); 
	m_objServer.HandleClientNetEvent(m_objFdset); 
	m_objServer.DoPatrol();
}

void CInnerApp::OnTimer()
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
			pBuf->Release();
			return;
		}
		
		SendConnStatePkt(pBuf);
		pBuf->Release();
		tPrev = CUtility::GetUptime();
	}
}

void CInnerApp::SendConnStatePkt(CBuffer* pBuf)
{
	assert(pBuf);
	list<CSid> objSid = m_objServer.GetSid();
	LogDebug("++++++CommInner �����ӵĿͻ��˸���Ϊ:%d\n", objSid.size());
	
	list<CSid>::iterator iter = objSid.begin();
	
	for(; iter!=objSid.end(); ++iter)
	{
		CServerForwardBaseCltObj* pClient = (CServerForwardBaseCltObj*)m_objServer.GetClientObj((*iter));
		if(!pClient)
		{
		    LogImportant("CInnerApp::SendConnStatePkt(), \n server(%s) \n send error! can not find client for objSid{%d,%d}", 
			m_objServer.GetTagMyInfo().c_str(), (*iter).GetHigh(), (*iter).GetLow()); 
		    return; 
		}
		
		if(pClient->GetClientId() == RMINTERSYS_CLIENT_ID)
		{
			//��Ϊ�Ƿ��Ͱ����������ø÷����Ϊҵ���������
			m_objServer.SetAppServer(true);
			m_objServer.SendForwardData(pBuf, (*iter));
			m_objServer.SetAppServer(false);
				
			break;
		}
	}
	
}

bool CInnerApp::MakeAllCommStatePkt(CBuffer* pBuf)
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
	//objDeviceInfo.SetAbFlag(objSvrAddr.GetAbFlag());
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

void CInnerApp::AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState)
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