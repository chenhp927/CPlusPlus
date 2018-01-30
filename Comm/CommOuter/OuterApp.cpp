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

// 主程序全局变量
COuterApp objApp;

//==============================================================================
//                 
//                    COuterApp
//                     主程序类
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
	// 本程序的版本信息。!!!!后续程序升级需要在此处修改版本、日期信息!!!!
	/* !!!!!!!!!!!!!!!!!!!!!格式请于示例保持对齐（用空格，不要用tab）!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommOuter           version 1.0.0      date 2016-09-13";
	strVer += "\n";
	
	// 程序用到的库的版本信息。LIBVER为宏定义，每个库里都实现了自己库的版本信息。
	// !!!!!!如果程序使用的库发生变化，需要同步修改此处的实现!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml库
	strVer += LIBVER(TOOLLIB);		// Tools库
	strVer += LIBVER(COMMLIB);		// 通信库
	strVer += LIBVER(APPLIB);		// 程序基类库
	
	return strVer;
}

void COuterApp::InitLogFile()
{
	// ================设置本程序内部用到的日志文件的一些信息===========================
	// 日志较多或者比较重要，需要记录的日志多一些
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
		
	// ================设置本程序内各个对象用到的日志文件名称===========================
}

bool COuterApp::InitComm()
{
	// 获取配置文件的文件名（包括路径）
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
	
	// 初始化客户端网络连接
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
	
	// 初始化服务端网络连接
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
	// 基类初始化
	if(!CUnixApp::Init())
	{
		return false;
	}

	// 检查m_strService的存在性
	if (m_strService.size() <= 0)
	{
		LogImportant("COuterApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// 初始化系统日志文件信息
	InitLogFile();
	
	// 初始化通信服务
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("COuterApp::Init(), \n Init ok!");
	return true;
}

void COuterApp::Run()
{
	// 连接客户端、服务端
	m_objClient.Connect();
	m_objServer.Startup();
	
	// 进入主处理流程，!!!!!注意此处不需要用sleep，并且禁止用sleep!!!!
	while(1)
	{
		// 定时处理网络事件
		OnTimerCheckNetEvent();

		// 定时器
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

	m_objClient.DoPatrol(); // 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
} 

void COuterApp::HandleClientEvent()
{ 
	// 处理事件
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
		// 因为发送数据是轮询机制，为了让收到的数据尽快的发送出去
		m_objServer.DoPatrol(); 
	}
} 

void COuterApp::OnTimer()
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
		
		m_objClient.SetAppClient(true);
		m_objClient.SendData(pBuf);
		pBuf->Release();
		m_objClient.SetAppClient(false);
		
		tPrev = CUtility::GetUptime();
	}
	
	// 打印CBuffer使用情况
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

void COuterApp::AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState)
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
