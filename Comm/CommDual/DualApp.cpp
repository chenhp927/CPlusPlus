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

// 主程序全局变量
CDualApp objApp;

//==============================================================================
//                 
//                    CDualApp
//                     主程序类
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
	// 本程序的版本信息。!!!!后续程序升级需要在此处修改版本、日期信息!!!!
	/* !!!!!!!!!!!!!!!!!!!!!格式请于示例保持对齐（用空格，不要用tab）!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommDual            version 1.0.0      date 2016-10-20";
	strVer += "\n";
	
	// 程序用到的库的版本信息。LIBVER为宏定义，每个库里都实现了自己库的版本信息。
	// !!!!!!如果程序使用的库发生变化，需要同步修改此处的实现!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml库
	strVer += LIBVER(TOOLLIB);		// Tools库
	strVer += LIBVER(COMMLIB);		// 通信库
	strVer += LIBVER(APPLIB);		// 程序基类库
	
	return strVer;
}

void CDualApp::InitLogFile()
{
	// ================设置本程序内部用到的日志文件的一些信息===========================
	// 日志较多或者比较重要，需要记录的日志多一些
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
	// ================设置本程序内各个对象用到的日志文件名称===========================
}

bool CDualApp::InitComm()
{
	// 获取配置文件的文件名（包括路径）
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
	
	// 初始化本地服务端网络连接
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
	
	// 初始化同步服务端网络连接
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
	
	// 初始化同步客户端网络连接
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
	// 基类初始化
	if(!CUnixApp::Init())
	{
		return false;
	}

	// 检查m_strService的存在性
	if (m_strService.size() <= 0)
	{
		LogImportant("CDualApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// 初始化系统日志文件信息
	InitLogFile();
	
	// 初始化通信服务
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CDualApp::Init(), \n Init ok!");
	return true;
}

void CDualApp::Run()
{
	// 连接客户端、服务端
	m_objLocalServer.Startup();
	m_objSynServer.Startup();
	m_objSynClient.Connect();
	
	// 进入主处理流程，!!!!!注意此处不需要用sleep，并且禁止用sleep!!!!
	while(1)
	{
		// 定时处理网络事件
		OnTimerCheckNetEvent();

		// 定时器
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

	m_objSynClient.DoPatrol(); // 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
} 

void CDualApp::HandleSynServerEvent()
{
	m_objSynServer.HandleListenNetEvent(m_objFdset); 
	m_objSynServer.HandleClientNetEvent(m_objFdset); 
	m_objSynServer.DoPatrol();
	
	m_objLocalServer.DoPatrol(); // 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
} 

void CDualApp::HandleSynClientEvent()
{ 
	// 处理事件
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
		// 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
		m_objLocalServer.DoPatrol(); 
	}
} 

void CDualApp::OnTimer()
{
	// 定时发送通信状态包
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
	
	// 获取转发层信息位置
	int nDataLen = pBuf->GetUsed() - CProtocolSession::HeaderSize();
	if (nDataLen < 0)
	{
		// 某个客户端的会话层数据长度有问题
		LogImportant("CDualApp::OnRecvDataFromSynConn(), \n connection(%s) \n "
			"receive one pkt, but it is not enough to get session head!", 
			m_objSynClient.GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		
		return;
	}
	char* p = pBuf->GetBuffer() + CProtocolSession::HeaderSize();
	
	// 转发包
	if (CProtocolForward::IsMsgForwardPkt(p, nDataLen))
	{
		// 解析转发包的包头
		CProtocolForward::CForwardHead objHead;
		if (objHead.FromStreamBuf(p, nDataLen) <= 0)
		{
			// 如果某个客户端的转发层数据长度有问题，那么该连接有问题，关闭该连接
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
	// 未知包
	else
	{
		// 未知包，丢弃
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
	LogDebug("++++++CommInner 所连接的客户端个数为:%d\n", objSid.size());
	
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
			//因为是发送包，故需设置该服务端为业务程序服务端
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
	objConnInfo.SetConnType(0x02);
	objConnInfo.SetConnPath(objSvrConnStateMgr.GetUnixPath());
	
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

void CDualApp::AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState)
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
	objConnState.SetAF_UNIXState(objCltConnState.m_bUnixCltConn);
	objConnState.SetAF_UNIXChgTime(objCltConnState.m_objUnixCltTime);
	objCltConnMainCommtoSys.SetConnState(objConnState);
}



