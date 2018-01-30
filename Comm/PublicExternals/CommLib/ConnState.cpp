//==============================================================================
//                      ConnState.cpp
//
//begin   : 2016-09-16
//describe:
//==============================================================================
#include "ConnState.h"

//==============================================================================
//                 
//                  CConnState
//                  连接状态类
//
//==============================================================================
CCltConnState::CCltConnState()
{
	m_strCltIp1 = "";
	m_sCltPort1 = 0;
	m_bCltConn1 = RM_UNDEFINED;

	m_strCltIp2 = "";
	m_sCltPort2 = 0;
	m_bCltConn2 = RM_UNDEFINED;
	
	m_bUnixCltConn = RM_UNDEFINED;
}

CCltConnState::~CCltConnState()
{
}

//==============================================================================
//                 
//                    CConnStateMgr
//                    连接状态管理类
//
//==============================================================================
CSvrConnStateMgr::CSvrConnStateMgr()
{
	m_strSvrIp1 = "";
	m_sSvrPort1 = 0;
	m_strSvrIp2 = "";
	m_sSvrPort2 = 0;
	m_strSvrUnixPath = "";
}

CSvrConnStateMgr::~CSvrConnStateMgr()
{
}

CSvrConnStateMgr& CSvrConnStateMgr::Instance()
{
	static CSvrConnStateMgr objMgr;
	return objMgr;
}

void CSvrConnStateMgr::SetSvrInfo( CCommAddr& objSvrAddr, string strSvrIp1, unsigned short sSvrPort1, string strSvrIp2, unsigned short sSvrPort2 )
{
	m_objSvrAddr = objSvrAddr;
	m_strSvrIp1 = strSvrIp1;
	m_sSvrPort1 = sSvrPort1;
	m_strSvrIp2 = strSvrIp2;
	m_sSvrPort2 = sSvrPort2;
}

void CSvrConnStateMgr::SetSvrInfo(CCommAddr& objSvrAddr, string strSvrUnixPath)
{
	m_objSvrAddr = objSvrAddr;
	m_strSvrUnixPath = strSvrUnixPath;
}

bool CSvrConnStateMgr::OnCltConnEvent(CCommAddr& objCltAddr, char cNetNo, string strCltIp, unsigned short sCltPort, BYTE bCltConn)
{
	assert(cNetNo == 1 || cNetNo == 2);

	list<CCltConnState>::iterator it = FindCltConnState(objCltAddr);
	// 对象已经存在
	if(it != m_lstCltConnState.end())
	{
		// 先进行状态更新
		if (cNetNo == 1)
		{
			it->m_strCltIp1 = strCltIp;
			it->m_sCltPort1 = sCltPort;
			it->m_bCltConn1 = bCltConn;
		}
		else
		{
			it->m_strCltIp2 = strCltIp;
			it->m_sCltPort2 = sCltPort;
			it->m_bCltConn2 = bCltConn;
		}

		// 如果双网都断开，则清除
		if (it->m_bCltConn1 == false && it->m_bCltConn2 == false)
		{
			m_lstCltConnState.erase(it);
		}
	}
	// 对象不存在，并且是连接成功事件，则创建新对象并添加到队列中
	else if (bCltConn)
	{
		CCltConnState objCltConnState;
		objCltConnState.m_objCltAddr = objCltAddr;
		if (cNetNo == 1)
		{
			objCltConnState.m_strCltIp1 = strCltIp;
			objCltConnState.m_sCltPort1 = sCltPort;
			objCltConnState.m_bCltConn1 = bCltConn;
		}
		else
		{
			objCltConnState.m_strCltIp2 = strCltIp;
			objCltConnState.m_sCltPort2 = sCltPort;
			objCltConnState.m_bCltConn2 = bCltConn;
		}
		
		m_lstCltConnState.push_back(objCltConnState);
	}

	return true;
}

bool CSvrConnStateMgr::OnCltConnEvent(CCommAddr& objCltAddr, string strCltUnixPath, BYTE bCltConn)
{
	list<CCltConnState>::iterator it = FindCltConnState(objCltAddr);
	// 对象已经存在
	if(it != m_lstCltConnState.end())
	{
		// 先进行状态更新
		it->m_strCltUnixPath = strCltUnixPath;
		it->m_bUnixCltConn   = bCltConn;
	}
	// 对象不存在，并且是连接成功事件，则创建新对象并添加到队列中
	else if (bCltConn)
	{
		CCltConnState objCltConnState;
		objCltConnState.m_objCltAddr = objCltAddr;
		
		objCltConnState.m_strCltUnixPath = strCltUnixPath;
		objCltConnState.m_bUnixCltConn = bCltConn;
		
		m_lstCltConnState.push_back(objCltConnState);
	}

	return true;
}

list<CCltConnState>::iterator CSvrConnStateMgr::FindCltConnState(CCommAddr& objCltAddr)
{
	for (list<CCltConnState>::iterator it = m_lstCltConnState.begin(); it != m_lstCltConnState.end(); it++)
	{
		if (it->m_objCltAddr == objCltAddr)
		{
			return it;
		}
	}

	return m_lstCltConnState.end();
}

void CSvrConnStateMgr::SendAllCommStatePkt()
{
}

CCommAddr& CSvrConnStateMgr::GetCommAddr()
{
	return m_objSvrAddr;
}

string& CSvrConnStateMgr::GetSvrIp1()
{
	return m_strSvrIp1;
}

unsigned short CSvrConnStateMgr::GetSvrPort1()
{
	return m_sSvrPort1;
}

string& CSvrConnStateMgr::GetSvrIp2()
{
	return m_strSvrIp2;
}

unsigned short CSvrConnStateMgr::GetSvrPort2()
{
	return m_sSvrPort2;
}

string CSvrConnStateMgr::GetUnixPath() const
{
	return m_strSvrUnixPath;
}

list<CCltConnState>& CSvrConnStateMgr::GetConnState()
{
	return m_lstCltConnState;
}