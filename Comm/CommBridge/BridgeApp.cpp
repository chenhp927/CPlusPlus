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

// 主程序全局变量
CBridgeApp objApp;

//==============================================================================
//                 
//                    CBridgeApp
//                     主程序类
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
	// 本程序的版本信息。!!!!后续程序升级需要在此处修改版本、日期信息!!!!
	/* !!!!!!!!!!!!!!!!!!!!!格式请于示例保持对齐（用空格，不要用tab）!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommBridge	      version 1.0.0      date 2016-09-18";
	strVer += "\n";
	
	// 程序用到的库的版本信息。LIBVER为宏定义，每个库里都实现了自己库的版本信息。
	// !!!!!!如果程序使用的库发生变化，需要同步修改此处的实现!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml库
	strVer += LIBVER(TOOLLIB);		// Tools库
	strVer += LIBVER(COMMLIB);		// 通信库
	strVer += LIBVER(APPLIB);		// 程序基类库
	
	return strVer;
}

void CBridgeApp::InitLogFile()
{
	// ================设置本程序内部用到的日志文件的一些信息===========================
	// 日志较多或者比较重要，需要记录的日志多一些
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
	// ================设置本程序内各个对象用到的日志文件名称===========================
}

bool CBridgeApp::InitComm()
{
	// 获取配置文件的文件名（包括路径）
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
	
	// 初始化本地客户端网络连接
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
	
	// 初始化远程客户端网络连接
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
	// 基类初始化
	if(!CUnixApp::Init())
	{
		return false;
	}

	// 检查m_strService的存在性
	if (m_strService.size() <= 0)
	{
		LogImportant("CBridgeApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// 初始化系统日志文件信息
	InitLogFile();
	
	// 初始化通信服务
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CBridgeApp::Init(), \n Init ok!");
	return true;
}

void CBridgeApp::Run()
{
	// 连接客户端
	m_objLocalClient.Connect();
	m_objRemoteClient.Connect();
	
	// 进入主处理流程，!!!!!注意此处不需要用sleep，并且禁止用sleep!!!!
	while(1)
	{
		// 定时处理网络事件
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
	// 处理事件
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
		// 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
		m_objRemoteClient.DoPatrol(); 
	}
} 

void CBridgeApp::HandleRemoteClientEvent()
{ 
	// 处理事件
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
		// 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
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
	// 定时发送通信状态包
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
	
	//局码\单位类型\单位ID\设备类型\设备ID\AB机标识
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
	
	//设置服务器的地址信息
	objMainSysSvcConnStatusPkt.SetDeviceInfo(objDeviceInfo);
	
	CSvrConnState objSvrConnState;
	//设置服务端ID
	CProgramServer objSvc;
	objSvc.SetSvcID(objSvrAddr.GetCltId());
	objSvrConnState.SetProgramServer(objSvc);
	
	//设置服务端地址和端口
	CConnInfo objConnInfo;
	objConnInfo.SetConnType(0x01);
	objConnInfo.SetIp1(objSvrConnStateMgr.GetSvrIp1());
	objConnInfo.SetNet1Port(objSvrConnStateMgr.GetSvrPort1());
	objConnInfo.SetIp2(objSvrConnStateMgr.GetSvrIp2());
	objConnInfo.SetNet2Port(objSvrConnStateMgr.GetSvrPort2());
	
	objSvrConnState.SetConnInfo(objConnInfo);

	//添加客户端连接状态
	list<CCltConnState> objListCltConnState = objSvrConnStateMgr.GetConnState();
	list<CCltConnState>::iterator iter = objListCltConnState.begin();
	for (; iter!=objListCltConnState.end(); ++iter)
	{
		CClientConnMainCommtoSys objCltConnMainCommtoSys;
		AddClientConnState(objCltConnMainCommtoSys, (*iter));
		objSvrConnState.AddClient(objCltConnMainCommtoSys);
	}
	//设置服务的连接状态
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
	//设置客户端地址
	CCommAddr objCommAddr = objCltConnState.m_objCltAddr;
	CForwardAddr objForwardAddr;
	objForwardAddr.SetBureauId(objCommAddr.GetBureauId());
	objForwardAddr.SetUnitType(objCommAddr.GetUnitType());
	objForwardAddr.SetUnitId(objCommAddr.GetUnitId());
	objForwardAddr.SetDevType(objCommAddr.GetDevType());
	objForwardAddr.SetDevId(objCommAddr.GetDevId());
	objForwardAddr.SetCltId(objCommAddr.GetCltId());
	objCltConnMainCommtoSys.SetForwardAddr(objForwardAddr);
	
	//客户端连接状态
	CConnState objConnState;
	//设置客户端1的Ip地址和端口
	objConnState.SetIP1(objCltConnState.m_strCltIp1);
	objConnState.SetNet1Port(objCltConnState.m_sCltPort1);
	objConnState.SetNet1Conn(objCltConnState.m_bCltConn1);
	objConnState.SetNet1ConnStaChgTime(objCltConnState.m_objCltTime1);
	//设置客户端2的Ip地址和端口
	objConnState.SetIP2(objCltConnState.m_strCltIp2);
	objConnState.SetNet2Port(objCltConnState.m_sCltPort2);
	objConnState.SetNet2Conn(objCltConnState.m_bCltConn2);
	objConnState.SetNet2ConnStaChgTime(objCltConnState.m_objCltTime2);
	objCltConnMainCommtoSys.SetConnState(objConnState);
}