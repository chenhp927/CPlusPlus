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

// 主程序全局变量
CInnerApp objApp;

//==============================================================================
//                 
//                    CInnerApp
//                     主程序类
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
	// 本程序的版本信息。!!!!后续程序升级需要在此处修改版本、日期信息!!!!
	/* !!!!!!!!!!!!!!!!!!!!!格式请于示例保持对齐（用空格，不要用tab）!!!!!!!!!!!!!!!
	string strVer = "App: AxxBxxCxx           version 1.1.79     date 2016-08-16" */
	string strVer = "App: CommInner           version 1.0.0      date 2016-09-10";
	strVer += "\n";
	
	// 程序用到的库的版本信息。LIBVER为宏定义，每个库里都实现了自己库的版本信息。
	// !!!!!!如果程序使用的库发生变化，需要同步修改此处的实现!!!!!!!!
	strVer += LIBVER(XMLLIB);		// TinyXml库
	strVer += LIBVER(TOOLLIB);		// Tools库
	strVer += LIBVER(COMMLIB);		// 通信库
	strVer += LIBVER(APPLIB);		// 程序基类库
	
	return strVer;
}

void CInnerApp::InitLogFile()
{
	// ================设置本程序内部用到的日志文件的一些信息===========================
	// 日志较多或者比较重要，需要记录的日志多一些
	CLogger::SetLogFileTagInfo("App", 1024 * 1000 * 20, 10);
		
	// ================设置本程序内各个对象用到的日志文件名称===========================
}

bool CInnerApp::InitComm()
{
	// 获取配置文件的文件名（包括路径）
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
	
	// 初始化服务端网络连接
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
	// 基类初始化
	if(!CUnixApp::Init())
	{
		return false;
	}

	// 检查m_strService的存在性
	if (m_strService.size() <= 0)
	{
		LogImportant("CInnerApp::InitComm(), \n Need startup param, but it is null!");
		return false;
	}
	
	// 初始化系统日志文件信息
	InitLogFile();
	
	// 初始化通信服务
	if (!InitComm())
	{
		return false;
	}	
	
	LogImportant("CInnerApp::Init(), \n Init ok!");
	return true;
}

void CInnerApp::Run()
{
	// 启动服务端
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
	// 定时发送通信状态包
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
	LogDebug("++++++CommInner 所连接的客户端个数为:%d\n", objSid.size());
	
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
			//因为是发送包，故需设置该服务端为业务程序服务端
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
	//objDeviceInfo.SetAbFlag(objSvrAddr.GetAbFlag());
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

void CInnerApp::AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState)
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