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

// 主程序全局变量
CChannelApp objApp;

//==============================================================================
//                 
//                    CChannelApp
//                     主程序类
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
	// 本程序的版本信息。!!!!后续程序升级需要在此处修改版本、日期信息!!!!
	/* !!!!!!!!!!!!!!!!!!!!!格式请于示例保持对齐（用空格，不要用tab）!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommChannel         version 1.0.0      date 2016-09-18";
	strVer += "\n";
	
	// 程序用到的库的版本信息。LIBVER为宏定义，每个库里都实现了自己库的版本信息。
	// !!!!!!如果程序使用的库发生变化，需要同步修改此处的实现!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml库
	strVer += LIBVER(TOOLLIB);		// Tools库
	strVer += LIBVER(COMMLIB);		// 通信库
	strVer += LIBVER(APPLIB);		// 程序基类库
	
	return strVer;
}

void CChannelApp::InitLogFile()
{
	// ================设置本程序内部用到的日志文件的一些信息===========================
	// 日志较多或者比较重要，需要记录的日志多一些
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
	// ================设置本程序内各个对象用到的日志文件名称===========================
}

bool CChannelApp::InitComm()
{
	// 获取配置文件的文件名（包括路径）
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
	
	// 初始化左侧客户端网络连接
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
	
	// 初始化右侧客户端网络连接
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
	// 基类初始化
	if(!CUnixApp::Init())
	{
		return false;
	}

	// 检查m_strService的存在性
	if (m_strService.size() <= 0)
	{
		LogImportant("CChannelApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// 初始化系统日志文件信息
	InitLogFile();
	
	// 初始化通信服务
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CChannelApp::Init(), \n Init ok!");
	return true;
}

void CChannelApp::Run()
{
	// 连接客户端
	m_objLeftClient.Connect();
	m_objRightClient.Connect();
	
	// 进入主处理流程，!!!!!注意此处不需要用sleep，并且禁止用sleep!!!!
	while(1)
	{
		// 定时处理网络事件
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
	// 处理事件
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
		// 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
		m_objRightClient.DoPatrol(); 
	}
} 

void CChannelApp::HandleRightClientEvent()
{ 
	// 处理事件
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
		// 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
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

