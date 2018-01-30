//==============================================================================
//                      ChannelApp.cpp
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
#include "ChannelApp.h"

// ������ȫ�ֱ���
CChannelApp objApp;

//==============================================================================
//                 
//                    CChannelApp
//                     ��������
//
//==============================================================================
CChannelApp::CChannelApp()
{
}

CChannelApp::~CChannelApp()
{
}

string CChannelApp::GetAppVersion()
{
	// ������İ汾��Ϣ��!!!!��������������Ҫ�ڴ˴��޸İ汾��������Ϣ!!!!
	/* !!!!!!!!!!!!!!!!!!!!!��ʽ����ʾ�����ֶ��루�ÿո񣬲�Ҫ��tab��!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommChannel         version 1.0.0      date 2016-09-18";
	strVer += "\n";
	
	// �����õ��Ŀ�İ汾��Ϣ��LIBVERΪ�궨�壬ÿ�����ﶼʵ�����Լ���İ汾��Ϣ��
	// !!!!!!�������ʹ�õĿⷢ���仯����Ҫͬ���޸Ĵ˴���ʵ��!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml��
	strVer += LIBVER(TOOLLIB);		// Tools��
	strVer += LIBVER(COMMLIB);		// ͨ�ſ�
	strVer += LIBVER(APPLIB);		// ��������
	
	return strVer;
}

void CChannelApp::InitLogFile()
{
	// ================���ñ������ڲ��õ�����־�ļ���һЩ��Ϣ===========================
	// ��־�϶���߱Ƚ���Ҫ����Ҫ��¼����־��һЩ
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
	// ================���ñ������ڸ��������õ�����־�ļ�����===========================
}

bool CChannelApp::InitComm()
{
	// ��ȡ�����ļ����ļ���������·����
	string strConfFile = CAppPath::GetPrivateConfPath();
	if (strConfFile.length() < 1)
	{
		LogImportant("CChannelApp::InitComm(), \n Can't find private path!");
		return false;
	}
	else
	{
		strConfFile += CONF_PRIVATE_FILE_NAME;
		LogImportant("CChannelApp::InitComm(), \n Find private file=%s", strConfFile.c_str());
	}
	
	// ��ʼ�����ͻ�����������
	m_objLeftClient.SetLogFileTag(GetLogFileTag());
	if (!m_objLeftClient.Init(strConfFile, m_strService, true))
	{
		LogImportant("CChannelApp::InitComm(), \n init left client fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CChannelApp::InitComm(), \n init left client ok ! ");
		m_objLeftClient.DumpParam();
	}
	
	// ��ʼ���Ҳ�ͻ�����������
	m_objRightClient.SetLogFileTag(GetLogFileTag());
	if (!m_objRightClient.Init(strConfFile, m_strService, false))
	{
		LogImportant("CChannelApp::InitComm(), \n init right client fail !"
			"private file=%s, service=%s", strConfFile.c_str(), m_strService.c_str());
		return false;
	}
	else
	{
		LogImportant("CChannelApp::InitComm(), \n init right client ok ! ");
		m_objRightClient.DumpParam();
	}

	return true;
}

bool CChannelApp::Init()
{
	// �����ʼ��
	if(!CUnixApp::Init())
	{
		return false;
	}

	// ���m_strService�Ĵ�����
	if (m_strService.size() <= 0)
	{
		LogImportant("CChannelApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// ��ʼ��ϵͳ��־�ļ���Ϣ
	InitLogFile();
	
	// ��ʼ��ͨ�ŷ���
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CChannelApp::Init(), \n Init ok!");
	return true;
}

void CChannelApp::Run()
{
	// ���ӿͻ���
	m_objLeftClient.Connect();
	m_objRightClient.Connect();
	
	// �������������̣�!!!!!ע��˴�����Ҫ��sleep�����ҽ�ֹ��sleep!!!!
	while(1)
	{
		// ��ʱ���������¼�
		OnTimerCheckNetEvent();

	}
}

void CChannelApp::OnTimerCheckNetEvent()
{
	m_objFdset.Zero();
	m_objLeftClient.SetFdset(m_objFdset);
	m_objRightClient.SetFdset(m_objFdset); 

	int nRet = m_objFdset.Select(SELECT_TIMEOUT_COMMCHANNEL);
	if (nRet >= 0)
	{
		HandleLeftClientEvent();
		HandleRightClientEvent();
	}
	else if (nRet < 0)
	{
		LogImportant("CChannelApp::OnTimerCheckNetEvent(), \n select error = %d!", ERRNO);
	}
}

void CChannelApp::HandleLeftClientEvent()
{ 
	// �����¼�
	m_objLeftClient.HandleNetEvent(m_objFdset); 
	m_objLeftClient.DoPatrol(); 

	bool bHasSendData = false;
	while (m_objLeftClient.IsHasData()) 
	{ 
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		if (m_objLeftClient.RecvRawData(pBuf)) 
		{ 
			ForwardDataFromLeftToRight(pBuf);
			bHasSendData = true;
		} 
		else  
		{ 
			LogImportant("CChannelApp::HandleLeftClientEvent(), \n RecvData() fail!"); 
		} 
		pBuf->Release();
	}

	if (bHasSendData)
	{
		// ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
		m_objRightClient.DoPatrol(); 
	}
} 

void CChannelApp::HandleRightClientEvent()
{ 
	// �����¼�
	m_objRightClient.HandleNetEvent(m_objFdset); 
	m_objRightClient.DoPatrol(); 
	
	bool bHasSendData = false;
	while (m_objRightClient.IsHasData()) 
	{ 
		CBuffer* pBuf = m_objBufAlloc.Allocate();
		if (m_objRightClient.RecvRawData(pBuf)) 
		{ 
			ForwardDataFromRightToLeft(pBuf); 
			bHasSendData = true;
		} 
		else  
		{ 
			LogImportant("CChannelApp::HandleRightClientEvent(), \n RecvData() fail!"); 
		} 
		pBuf->Release();
	}
	
	if (bHasSendData)
	{
		// ��Ϊ������������ѯ���ƣ�Ϊ�����յ������ݾ���ķ��ͳ�ȥ
		m_objLeftClient.DoPatrol(); 
	}
} 

void CChannelApp::ForwardDataFromLeftToRight(CBuffer* pBuf)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc;
	char* pForwardBuf = pBuf->GetBuffer() + CProtocolSession::CSessionHead::HeaderSize();
	int nForwardBufLen = pBuf->GetUsed() - CProtocolSession::CSessionHead::HeaderSize();
	CProtocolForward::GetForwardHeadTypeFunc(pForwardBuf, nForwardBufLen, objTypeFunc);
	
	LogDebug("CChannelApp::ForwardDataFromLeftToRight(), \n will forward pkt(%s) from \n local connection(%s) \n to \n remote connection(%s).", 
		objTypeFunc.GetTagDumpStr().c_str(), m_objLeftClient.GetTagConnInfo().c_str(), m_objRightClient.GetTagConnInfo().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	m_objRightClient.SendRawData(pBuf); 
}

void CChannelApp::ForwardDataFromRightToLeft(CBuffer* pBuf)
{
	CProtocolForward::CForwardTypeFunc objTypeFunc;
	char* pForwardBuf = pBuf->GetBuffer() + CProtocolSession::CSessionHead::HeaderSize();
	int nForwardBufLen = pBuf->GetUsed() - CProtocolSession::CSessionHead::HeaderSize();
	CProtocolForward::GetForwardHeadTypeFunc(pForwardBuf, nForwardBufLen, objTypeFunc);
	
	LogDebug("CChannelApp::ForwardDataFromRightToLeft(), \n will forward pkt(%s) from \n local connection(%s) \n to \n remote connection(%s).", 
		objTypeFunc.GetTagDumpStr().c_str(), m_objRightClient.GetTagConnInfo().c_str(), m_objLeftClient.GetTagConnInfo().c_str());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	
	m_objLeftClient.SendRawData(pBuf); 
}

