//==============================================================================
//                      ConnState.h
//
//begin   : 2016-09-16
//describe:
//==============================================================================
#ifndef SELFDEF_CONNSTATE_H
#define SELFDEF_CONNSTATE_H

#include <string>
#include <list>

#include "SockMacro.h"
#include "CommAddr.h" 
#include "ProtocolTime.h"

using namespace std;
using namespace ProtocolBase;
//==============================================================================
//                 
//                  CCltConnState
//                  客户端连接状态类
//
//==============================================================================
class MY_EXT_CLASS CCltConnState
{
public:
	CCltConnState();
	virtual ~CCltConnState();

public:
	CCommAddr	m_objCltAddr;	// 客户端地址

	string		m_strCltIp1;	// 客户端网1的IP地址
	unsigned short		m_sCltPort1;	// 客户端网1的端口
	BYTE		m_bCltConn1;	// 客户端网1连接状态
	CProtocolTime m_objCltTime1; // 客户端网1连接状态改变时间
	
	string		m_strCltIp2;	// 客户端网2的IP地址
	unsigned short		m_sCltPort2;	// 客户端网2的端口
	BYTE		m_bCltConn2;	// 客户端网2连接状态
	CProtocolTime m_objCltTime2; // 客户端网2连接状态改变时间
	
	string      m_strCltUnixPath;   //Unix客户端连接路径
	BYTE        m_bUnixCltConn; // Unix客户端连接状态
	CProtocolTime m_objUnixCltTime; // Unix客户端连接状态改变时间
};

//==============================================================================
//                 
//                    CSvrConnStateMgr
//                   服务器连接状态管理类
//
//==============================================================================
class MY_EXT_CLASS CSvrConnStateMgr
{
public:
	CSvrConnStateMgr();
	virtual ~CSvrConnStateMgr();

	static CSvrConnStateMgr& Instance();

public:
	void SetSvrInfo(CCommAddr& objSvrAddr, string strSvrIp1, unsigned short sSvrPort1, string strSvrIp2, unsigned short sSvrPort2);
	void SetSvrInfo(CCommAddr& objSvrAddr, string strSvrUnixPath);
	
	// cNetNo代表网络编号，1代表第一个网，2代表第二个网
	bool OnCltConnEvent(CCommAddr& objCltAddr, char cNetNo, string strCltIp, unsigned short sCltPort, BYTE bCltConn);
	bool OnCltConnEvent(CCommAddr& objCltAddr, string strCltUnixPath, BYTE bCltConn);
	
	void SendAllCommStatePkt();
	
	
	//属性接口
	//获取服务端地址
	CCommAddr& GetCommAddr();
	//获取服务端网1的IP地址
	string& GetSvrIp1();
	//获取服务端网1的端口
	unsigned short GetSvrPort1();
	//获取服务端网2的IP地址
	string& GetSvrIp2();
	//获取服务端网2的端口
	unsigned short GetSvrPort2();
	//获取服务端Unix连接路径
	string GetUnixPath() const;
	//获取客户端连接状态列表
	list<CCltConnState>& GetConnState();
	
private:
	list<CCltConnState>::iterator FindCltConnState(CCommAddr& objCltAddr);

private:
	CCommAddr	m_objSvrAddr;	// 服务端地址
	string		m_strSvrIp1;	// 服务端网1的IP地址
	unsigned short		m_sSvrPort1;	// 服务端网1的端口
	string		m_strSvrIp2;	// 服务端网2的IP地址
	unsigned short		m_sSvrPort2;	// 服务端网2的端口

    string      m_strSvrUnixPath;       // AF_UNIX连接路径
    
	// 客户端连接状态列表，其元素为某个客户端双网连接状态。
	// 当双网之一连接成功后动态创建一个插入其中，当另一个网连接成功后更新；
	// 当双网连接都断开后删除
	list<CCltConnState> m_lstCltConnState;
};

#endif
