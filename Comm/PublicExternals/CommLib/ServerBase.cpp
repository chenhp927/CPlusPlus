//==============================================================================
//                      ServerBase.cpp
//
//begin   : 2016-05-13
//describe:
//==============================================================================

#include "ServerBase.h" 
#include "ProtocolSession.h"
#include "CommUtility.h"
#include "Utility.h"

//==============================================================================
//                 
//                    CServerBase
//                    服务端基类
//
//==============================================================================
CServerBase::CServerBase() :  
		m_nSockfd(INVALID_SOCKET), 
		m_nType(0),
		m_sSvrPort(0),
		m_ClientsPtr(NULL),
		m_nMaxClients(DEF_MAXCLIENTS),  
		m_nCurMaxClientBound(0),
		m_nMaxPacketSize(MAX_PACKETSIZE),
		m_dwSidHigh(0)
{ 
	SetMyClassName("CServerBase");
	CCommUtility::SockStart();

	memset(m_szUnixPath, 0, UNIX_PATH_LEN);	
	memset(m_szSvrIp, 0, sizeof(m_szSvrIp));	

	// 一开始就要有稳定的m_ClientsPtr数组，这样在多线程程序里不用考虑m_ClientsPtr数组的访问冲突，
	// 只需要考虑每个CServerBaseCltObj对象的访问冲突。
	// AllocateClientPool()里面要调用虚拟函数CreateObject，不能放在构造函数里
	// 所以该类对象构造以后，要手动调用Create()函数，在Create()函数中执行AllocateClientPool()
	
} 

CServerBase::~CServerBase() 
{  
	if (m_ClientsPtr) 
	{
		for (int x = 0; x < m_nMaxClients; x++) 
		{ 
			if (m_ClientsPtr[x]) 
			{ 
				CloseClientObj(m_ClientsPtr[x]); 
				delete m_ClientsPtr[x]; 
			} 
		} 
		delete []m_ClientsPtr; 
	}   
}

bool CServerBase::Create()
{
	AllocateClientPool();
	return true;
}

bool CServerBase::Init(const char* szConfigFile)
{
	return true;
} 

string CServerBase::GetDumpParamStr()
{
	char szDump[1024];
	memset(szDump, 0, sizeof(szDump));

	if (m_nType == AF_UNIX)
	{
		snprintf(szDump, sizeof(szDump), "server_param  (server_type=%d; unix_path=%s)", m_nType, m_szUnixPath);
	}
	else if (m_nType == AF_INET)
	{
		snprintf(szDump, sizeof(szDump), "server_param  (server_type=%d; svr_ip=%s; svr_port=%d)", m_nType, m_szSvrIp, m_sSvrPort);
	}
	else
	{
		snprintf(szDump, sizeof(szDump), "server_param  (server_type=%d; unix_path=%s; svr_ip=%s; svr_port=%d)", m_nType, m_szUnixPath, m_szSvrIp, m_sSvrPort);
	}

	return szDump;
}

void CServerBase::DumpParam()
{
	LogImportant("CServerBase::DumpParam(),\n%s", GetDumpParamStr().c_str());
}

string CServerBase::GetTagMyInfo()
{
	char temp[256];
	memset(temp, 0, sizeof(temp));
	
	if (m_nType == AF_INET)
	{
		snprintf(temp, sizeof(temp), "%s:%d", m_szSvrIp, m_sSvrPort);
	}
	else if (m_nType == AF_UNIX)
	{
		snprintf(temp, sizeof(temp), "%s", m_szUnixPath);
	}
	
	return temp;
}

void CServerBase::SetType(int nType)
{
	assert(nType == AF_INET || nType == AF_UNIX);
	m_nType = nType;
}

int CServerBase::GetType()
{
	return m_nType;
}

bool CServerBase::SetUnixPath(const char* szPath)
{
	if (!szPath || strlen(szPath) >= UNIX_PATH_LEN) 
	{  
		LogImportant("CServerBase::SetUnixPath(), \n set unix path fail!");  
		return false;  
	}  
	
	m_nType = AF_UNIX; 
	strncpy(m_szUnixPath, szPath, UNIX_PATH_LEN);
	
	return true;
}

const char* CServerBase::GetUnixPath()
{
	return m_szUnixPath;
}

bool CServerBase::SetSvrIp(const char* szSvrIp)
{
	assert(szSvrIp);
	if (!CCommUtility::IsValidIP(szSvrIp)) 
	{  
		LogImportant("CServerBase::SetSvrIp(), \n invalid server ip=%s", szSvrIp);
		return false;  
	} 
	
	m_nType = AF_INET; 
	strncpy(m_szSvrIp, szSvrIp, IP_LEN);
	
	return true;
}

string CServerBase::GetSvrIp()
{
	return m_szSvrIp;
}

bool CServerBase::SetSvrPort(unsigned short sPort)
{
	if (sPort < 1024)  
	{  
		LogImportant("CServerBase::SetSvrPort(), \n invalid server port=%d, right range(1024, ~).", sPort);  
		return false;  
	}  
	
	m_sSvrPort = sPort;  
	
	return true;
}

int CServerBase::GetSvrPort()
{
	return static_cast<int>(m_sSvrPort);
}

bool CServerBase::SetMaxClients(int nMax) 
{ 
	if (nMax <= 0 || nMax > MAX_CLIENTS) 
	{
		return false; 
	}
	else
	{  
		m_nMaxClients = nMax;  
	}
	
	return true;
} 

int CServerBase::GetMaxClients() 
{ 
	return m_nMaxClients;  
} 

int CServerBase::GetMaxClientBound()
{
	return m_nCurMaxClientBound;
}

void CServerBase::SetMaxPacketSize(int nSize)
{
	assert(nSize > 0);
	m_nMaxPacketSize = nSize;
}

int CServerBase::GetMaxPacketSize()
{
	return m_nMaxPacketSize;
}

void CServerBase::SetSidHigh(DWORD dw)
{
	m_dwSidHigh = dw;
}

DWORD CServerBase::GetSidHigh()
{
	return m_dwSidHigh;
}

void CServerBase::ClearRecvQueue() 
{ 
	// 队列清空前输出日志
	int nLastPktCount = m_RecvQueue.GetSize();
	if (nLastPktCount > 0)
	{
		LogImportant("CServerBase::ClearRecvQueue(), \n server(%s) \n queue_size=%d, will throw packets ...", 
			GetTagMyInfo().c_str(), nLastPktCount);
	}

	// 执行队列清空操作
	CBuffer* pBuf = NULL; 
	while (!m_RecvQueue.IsEmpty()) 
	{ 
		if (m_RecvQueue.GetHead(pBuf)) 
		{
// 			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
			pBuf->Release(); 
		}
		else
		{
			break; 
		}
	} 

	// 队列清空后输出日志
	int nThrowPktCount = nLastPktCount - m_RecvQueue.GetSize();
	if (nThrowPktCount > 0 || m_RecvQueue.GetSize() > 0)
	{
		LogImportant("CServerBase::ClearRecvQueue(), \n server(%s) \n succeed to throw %d packets, now queue_size=%d.", 
			GetTagMyInfo().c_str(), nThrowPktCount, m_RecvQueue.GetSize());
	}
} 

bool CServerBase::Startup(unsigned short nPort, const char* szIP)  
{  
	if (SOCKET_VALID(m_nSockfd)) 
	{
		LogImportant("CServerBase::Startup(), \n server(%s) \n fail to startup because sockfd is invalid!",
			GetTagMyInfo().c_str()); 
		return false; 
	} 

	if (!SetSvrIp(szIP) || !SetSvrPort(nPort))
	{
		return false;
	}
			
	if (!OnStart()) 
	{
		return false; 
	}
	
	return MakeInetSocket();  
}  
 
bool CServerBase::Startup(const char* szPath)  
{  
	if (!SetUnixPath(szPath))
	{
		return false;
	}

	if (!OnStart()) 
	{
		return false; 
	}
 
	return MakeUnixSocket();  
}  
 
bool CServerBase::Startup()
{
	if (!OnStart()) 
	{
		return false; 
	}

	if (m_nType == AF_INET)
	{
		return MakeInetSocket();  
	}
	else if (m_nType == AF_UNIX)
	{
		return MakeUnixSocket();
	}
	else
	{
		LogImportant("CServerBase::Startup(), \n server(%s) \n fail to startup because server type=%d is invalid!", 
			GetTagMyInfo().c_str(), m_nType);
		return false;
	}
}

void CServerBase::CloseSvr()
{
	CLOSE_SOCKET(m_nSockfd);

	if (!m_ClientsPtr)
	{
		return;
	}
	
	LogImportant("CServerBase::CloseSvr(), \n so will close all client socket ...");
	CServerBaseCltObj* pCltObj = NULL; 
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
	for (int i = 0; i < nMaxClientBound; i++)  
	{  
		CloseClientObj(m_ClientsPtr[i]); 
	}
}

bool CServerBase::OnStart() 
{ 
	return true;
} 

bool CServerBase::MakeInetSocket()  
{  
	if (m_sSvrPort == 0)
	{
		return false;
	}
		
	m_nSockfd = socket(AF_INET, SOCK_STREAM, 0);  
	if (m_nSockfd == INVALID_SOCKET)  
	{  
		LogImportant("CServerBase::MakeInetSocket(), \n server(%s) \n fail to make socket. errno=%d", 
			GetTagMyInfo().c_str(), ERRNO);  
		return false;  
	}  
  
	if (!CCommUtility::SetNonBlock(m_nSockfd))  
	{  
		LogImportant("CServerBase::MakeInetSocket(), \n server(%s) \n CCommUtility::SetNonBlock() fail.",
			GetTagMyInfo().c_str()); 
		CLOSE_SOCKET(m_nSockfd);
		return false;  
	}  

	if (!CCommUtility::SetSockReuse(m_nSockfd))  
	{  
		LogImportant("CServerBase::MakeInetSocket(), \n server(%s) \n CCommUtility::SetSockReuse() fail.",
			GetTagMyInfo().c_str()); 
		CLOSE_SOCKET(m_nSockfd);
		return false;  
	}  
  
	struct sockaddr_in sin;  
	memset(&sin, 0, sizeof(struct sockaddr_in));  
	sin.sin_family = AF_INET;  
	sin.sin_port = htons(m_sSvrPort);  
	sin.sin_addr.s_addr = inet_addr(m_szSvrIp);  
	if (sin.sin_addr.s_addr == INADDR_NONE)  
		sin.sin_addr.s_addr = htonl(INADDR_ANY);  
#ifdef HP_UX  
	if (bind(m_nSockfd, &sin, sizeof(sin)) == -1)   
#else  
	if (bind(m_nSockfd, (struct sockaddr*)&sin, sizeof(sin)) == -1)  
#endif  
	{  
		LogImportant("CServerBase::MakeInetSocket(), \n server(%s) \n bind() fail!",
			GetTagMyInfo().c_str());
		CLOSE_SOCKET(m_nSockfd);  
		return false;  
	}  
  
	if (listen(m_nSockfd, 5) == -1)  
	{  
		LogImportant("CServerBase::MakeInetSocket(), \n server(%s) \n listen() fail!",
			GetTagMyInfo().c_str());
		m_nSockfd = INVALID_SOCKET;  
		return false;  
	} 
	
	LogImportant("CServerBase::MakeInetSocket(), \n server(%s) \n make socket=%d ok!", 
		GetTagMyInfo().c_str(), m_nSockfd);  
	
	return true;  
}  
  
bool CServerBase::MakeUnixSocket()  
{ 
	if (m_szUnixPath[0] == 0)
	{
		return false;
	}
		
	m_nSockfd = socket(AF_UNIX, SOCK_STREAM, 0);  
	if (m_nSockfd == INVALID_SOCKET)  
	{  
		LogImportant("CServerBase::MakeUnixSocket(), \n server(%s) \n make socket() fail. errno=%d", 
			GetTagMyInfo().c_str(), ERRNO);  
		return false;  
	}  
	if (!CCommUtility::SetNonBlock(m_nSockfd))  
	{  
		LogImportant("CServerBase::MakeUnixSocket(), \n server(%s) \n CCommUtility::SetNonBlock() fail.",
			GetTagMyInfo().c_str()); 
		CLOSE_SOCKET(m_nSockfd);
		return false;  
	}  
  
#if defined(UNIX) || defined(HP_UX)
	struct sockaddr_un sa;  
	memset(&sa, 0, sizeof(struct sockaddr_un));  
	sa.sun_family = AF_UNIX;  
	strcpy(sa.sun_path, m_szUnixPath); 

	char path[UNIX_PATH_LEN];
	strcpy(path, m_szUnixPath);
	CUtility::MakeDir(path);
	unlink(m_szUnixPath); 
#endif

#ifdef HP_UX  
	if (bind(m_nSockfd, &sa, SUN_LEN(&sa)) == -1) 
#endif
#ifdef UNIX
	if (bind(m_nSockfd, (struct sockaddr*)&sa, SUN_LEN(&sa)) == -1)  
#else  
	assert(0);
	if (1)  // windows没有AF_UNIX的Socket
#endif  
	{  
		LogImportant("CServerBase::MakeUnixSocket(), \n server(%s) \n bind() fail!", 
				GetTagMyInfo().c_str());  
		CLOSE_SOCKET(m_nSockfd);  
		return false;  
	}  
  
	if (listen(m_nSockfd, 5) == -1)  
	{  
		LogImportant("CServerBase::MakeUnixSocket(), \n server(%s) \n listen() fail!", 
				GetTagMyInfo().c_str());  
		CLOSE_SOCKET(m_nSockfd);  
		return false;  
	}  

	LogImportant("CServerBase::MakeUnixSocket(), \n server(%s) \n make socket=%d ok!", 
		GetTagMyInfo().c_str(), m_nSockfd);
	
	return true;  
}  
  
void CServerBase::SetFdset(CFdset& objFs) 
{ 
	if (SOCKET_VALID(m_nSockfd)) 
	{ 
		objFs.SetRead(m_nSockfd); 
		objFs.SetExcept(m_nSockfd); 
	} 
	
	if (m_ClientsPtr)
	{
		int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
		for (int i = 0; i < nMaxClientBound; i ++) 
		{ 
			if (!m_ClientsPtr[i] || !m_ClientsPtr[i]->IsSocketValid()) 
			{
				continue; 
			}

			objFs.SetRead(m_ClientsPtr[i]->GetSocket()); 
			objFs.SetExcept(m_ClientsPtr[i]->GetSocket()); 
		} 
	}
} 

void CServerBase::SetListenFdset(CFdset& objFs) 
{ 
	if (SOCKET_VALID(m_nSockfd)) 
	{ 
		objFs.SetRead(m_nSockfd); 
		objFs.SetExcept(m_nSockfd); 
	} 
} 

void CServerBase::SetClientFdset(CFdset& objFs) 
{ 
	if (m_ClientsPtr)
	{
		int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
		for (int i = 0; i < nMaxClientBound; i ++) 
		{ 
			if (!m_ClientsPtr[i] || !m_ClientsPtr[i]->IsConnect()) 
			{
				continue; 
			}

			objFs.SetRead(m_ClientsPtr[i]->GetSocket()); 
			objFs.SetExcept(m_ClientsPtr[i]->GetSocket()); 
		} 
	}
} 

void CServerBase::SetClientFdset(CFdset& objFs, vector<CServerBaseCltObj*>& vecCltObj) 
{ 
	for (int i = 0; i < vecCltObj.size(); i ++) 
	{ 
		assert(vecCltObj[i]);
		if (!vecCltObj[i] || !vecCltObj[i]->IsConnect()) 
		{
			continue; 
		}

		objFs.SetRead(vecCltObj[i]->GetSocket()); 
		objFs.SetExcept(vecCltObj[i]->GetSocket()); 
	} 
} 
 

void CServerBase::HandleNetEvent(CFdset& objFs) 
{ 
	if (SOCKET_VALID(m_nSockfd) && objFs.IsSetRead(m_nSockfd)) 
	{  
		Accept();
	} 
	else if (SOCKET_VALID(m_nSockfd) && objFs.IsSetExcept(m_nSockfd)) 
	{ 
		OnFdExcept(); 
	} 
 
	if (m_ClientsPtr)
	{
		int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
		for (int i = 0; i < nMaxClientBound; i ++) 
		{ 
			if (!m_ClientsPtr[i] || !m_ClientsPtr[i]->IsConnect()) 
			{
				continue; 
			}
		 
			if (objFs.IsSetRead(m_ClientsPtr[i]->GetSocket())) 
			{ 
				Read(m_ClientsPtr[i]); 
			} 
			else if (objFs.IsSetExcept(m_ClientsPtr[i]->GetSocket())) 
			{ 
				LogImportant("CServerBase::HandleNetEvent(), \n socket except, so will close client socket ...");
				CloseClientObj(m_ClientsPtr[i]); 
			} 
		}
	} 
} 

void CServerBase::HandleListenNetEvent(CFdset& objFs) 
{ 
	if (SOCKET_VALID(m_nSockfd) && objFs.IsSetRead(m_nSockfd)) 
	{  
		Accept();
	} 
	else if (SOCKET_VALID(m_nSockfd) && objFs.IsSetExcept(m_nSockfd)) 
	{ 
		OnFdExcept(); 
	} 
}

void CServerBase::HandleClientNetEvent(CFdset& objFs) 
{ 
	if (m_ClientsPtr)
	{
		int nMaxClientBound = GetMaxClientBound();
		for (int i = 0; i < nMaxClientBound; i ++) 
		{ 
			if (!m_ClientsPtr[i] || !m_ClientsPtr[i]->IsConnect()) 
			{
				continue; 
			}
		 
			if (objFs.IsSetRead(m_ClientsPtr[i]->GetSocket())) 
			{ 
				Read(m_ClientsPtr[i]); 
			} 
			else if (objFs.IsSetExcept(m_ClientsPtr[i]->GetSocket())) 
			{ 
				LogImportant("CServerBase::HandleClientNetEvent(), \n connection(%s) \n socket except, so will close client socket ...",
					m_ClientsPtr[i]->GetTagConnInfo().c_str());
				CloseClientObj(m_ClientsPtr[i]); 
			} 
		}
	} 
} 
void CServerBase::HandleClientNetEvent(CFdset& objFs, vector<CServerBaseCltObj*>& vecCltObj) 
{ 
	for (int i = 0; i < vecCltObj.size(); i ++) 
	{ 
		assert(vecCltObj[i]);
		if (!vecCltObj[i])
		{
			continue;
		}
		
		if (!vecCltObj[i]->IsConnect())
		{ 
			continue;
		} 
	 
		if (objFs.IsSetRead(vecCltObj[i]->GetSocket())) 
		{ 
			Read(vecCltObj[i]); 
		} 
		else if (objFs.IsSetExcept(vecCltObj[i]->GetSocket())) 
		{ 
			LogImportant("CServerBase::HandleClientNetEvent(2), \n connection(%s) \n socket except, so will close client socket ...",
				vecCltObj[i]->GetTagConnInfo().c_str());
			CloseClientObj(vecCltObj[i]); 
		} 
	}
} 

void CServerBase::Accept()  
{  
	int nCltSockFd = INVALID_SOCKET;  

	if (m_nType == AF_INET) 
	{ 
		struct sockaddr_in sa; 
		socklen_t nLen = sizeof(struct sockaddr_in);
		memset(&sa, 0, nLen);  

#ifdef HP_UX 
		nCltSockFd = accept(m_nSockfd, &sa, &nLen);  
#else 
		nCltSockFd = accept(m_nSockfd, (struct sockaddr *)&sa, &nLen); 
#endif  
		if (SOCKET_VALID(nCltSockFd))
		{ 
			if (!CCommUtility::SetNonBlock(nCltSockFd))
			{
				LogImportant("CServerBase::Accept(), \n server(%s) \n inet server CCommUtility::SetNonBlock() fail.",
					GetTagMyInfo().c_str());
				CLOSE_SOCKET(nCltSockFd);
				return;
			}
			OnInetAccept(nCltSockFd, sa); 
		}
		else
		{
			LogImportant("CServerBase::Accept(), \n server(%s) \n inet server accept fail. errno=%d", 
				GetTagMyInfo().c_str(), ERRNO); 
		}
	} 
	else 
	{ 
		nCltSockFd = accept(m_nSockfd, NULL, NULL); 
		if (SOCKET_VALID(nCltSockFd))
		{ 
			if (!CCommUtility::SetNonBlock(nCltSockFd))
			{
				LogImportant("CServerBase::Accept(), \n server(%s) \n unix server CCommUtility::SetNonBlock() fail.",
					GetTagMyInfo().c_str());
				CLOSE_SOCKET(nCltSockFd);
				return;
			}
			OnUnixAccept(nCltSockFd);
		} 
		else
		{
			LogImportant("CServerBase::Accept(), \n server(%s) \n unix server accept fail. errno=%d", 
				GetTagMyInfo().c_str(), ERRNO); 
		}
	} 
}  
  
void CServerBase::OnInetAccept(int nSockfd, struct sockaddr_in& stuSockAddr)  
{  
	LogImportant("CServerBase::OnInetAccept(), \n server(%s) \n accept a inet client from %s:%d",
		GetTagMyInfo().c_str(), inet_ntoa(stuSockAddr.sin_addr), ntohs(stuSockAddr.sin_port));

	if (IsClientIpValid(stuSockAddr))  
	{  
		if (IsTooMuchSameClient(stuSockAddr))  
		{
			DenyClient(nSockfd, &stuSockAddr, DENY_TOOMUCHSAME);  
		}
		else  
		{  
			CServerBaseCltObj* pCltObj = AssignClientObject();  
			if (pCltObj)  
			{ 
				if (CCommUtility::SetSockBuf(nSockfd)  &&
					CCommUtility::SetSockNodelay(nSockfd))
				{
					UpdateMaxClientBoundOnConnect(pCltObj);
					pCltObj->Connect(nSockfd, stuSockAddr);  
					OnSessionConnect(pCltObj->GetSid());
				}
				else
				{
					LogImportant("CServerBase::OnInetAccept(), \n server(%s) \n set CCommUtility::SetSockBuf() fail!",
						GetTagMyInfo().c_str());
					DenyClient(nSockfd, &stuSockAddr, DENY_NORESOURCE);  
				}
			} 
			else  
			{
 				DenyClient(nSockfd, &stuSockAddr, DENY_NORESOURCE);  
			}
		}  
	}  
	else  
	{
		DenyClient(nSockfd, &stuSockAddr, DENY_ILLEGAL);  
	}
}  
   
void CServerBase::OnUnixAccept(int nSockfd)  
{
	LogImportant("CServerBase::OnUnixAccept(), \n server(%s) \n accept a unix client.",
		GetTagMyInfo().c_str());
	CServerBaseCltObj* pCltObj = AssignClientObject();  
	if (pCltObj)  
	{ 
		if (CCommUtility::SetSockBuf(nSockfd)  &&
			CCommUtility::SetSockNodelay(nSockfd))
		{
			UpdateMaxClientBoundOnConnect(pCltObj);
			pCltObj->Connect(nSockfd);  
			OnSessionConnect(pCltObj->GetSid()); 
		}
		else
		{
			LogImportant("CServerBase::OnUnixAccept(), \n server(%s) \n CCommUtility::SetSockBuf() fail!",
				GetTagMyInfo().c_str());
			DenyClient(nSockfd, NULL, DENY_NORESOURCE);  
		}
	} 
	else  
	{
		DenyClient(nSockfd, NULL, DENY_NORESOURCE);  
	}
}

bool CServerBase::IsClientIpValid(struct sockaddr_in& stuSockAddr)  
{  
	return true;  
}  
  
bool CServerBase::IsTooMuchSameClient(struct sockaddr_in& stuSockAddr)  
{  
	/*
	std::string ip = inet_ntoa(sin.sin_addr);  
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
	for (int i = 0; i < nMaxClientBound; i++)  
	{  
		if (m_ClientsPtr[i] && m_ClientsPtr[i]->IsSocketValid())  
		{  
			if (ip == m_ClientsPtr[i]->PeerIP()) 
			{ 
				return true;  
			}
		}  
	}  
	*/
	return false;  
}   

void CServerBase::DenyClient(int nSockfd, struct sockaddr_in* stuSockAddr, int nReason)
{  
	CLOSE_SOCKET(nSockfd);  
	if (stuSockAddr) 
	{
		char szIP[IP_LEN]; 
		strcpy(szIP, inet_ntoa(stuSockAddr->sin_addr)); 
		if (nReason == DENY_ILLEGAL)
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a client from %s:%d because of illegal", 
				GetTagMyInfo().c_str(), szIP, ntohs(stuSockAddr->sin_port));  
		}
		else if (nReason == DENY_TOOMUCHSAME)
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a client from %s:%d because of too much same", 
				GetTagMyInfo().c_str(), szIP, ntohs(stuSockAddr->sin_port));  
		}
		else if (nReason == DENY_NORESOURCE)
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a client from %s:%d because of no resource", 
				GetTagMyInfo().c_str(), szIP, ntohs(stuSockAddr->sin_port));  
		}
		else
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a client from %s:%d because of %d", 
				GetTagMyInfo().c_str(), szIP, ntohs(stuSockAddr->sin_port), nReason);  
		}
	} 
	else 
	{
		if (nReason == DENY_ILLEGAL)
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a unix client because of illegal",
				GetTagMyInfo().c_str());
		}
		else if (nReason == DENY_TOOMUCHSAME)
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a unix client because of too much same",
				GetTagMyInfo().c_str());
		}
		else if (nReason == DENY_NORESOURCE)
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a unix client because of no resource",
				GetTagMyInfo().c_str());
		}
		else
		{
			LogImportant("CServerBase::DenyClient(), \n server(%s) \n deny a unix client because of %d", 
				GetTagMyInfo().c_str(), nReason);
		}
	}
} 

bool CServerBase::AllocateClientPool() 
{ 
	if (m_ClientsPtr)
	{
		return true;
	}
	
	if (m_nMaxClients <= 0) 
	{
		m_nMaxClients = DEF_MAXCLIENTS;  
	}
	else if (m_nMaxClients > MAX_CLIENTS)  
	{
		m_nMaxClients = MAX_CLIENTS;  
	}
 
	m_ClientsPtr = new CServerBaseCltObj*[m_nMaxClients];  
	if (m_ClientsPtr) 
	{
		memset(m_ClientsPtr, 0, sizeof(CServerBaseCltObj *) * m_nMaxClients);  
	}
	
	for (int x = 0; x < m_nMaxClients; x++)
	{
		CServerBaseCltObj* pCltObj = CreateClientObject(); 
		assert(pCltObj);
		if (pCltObj)
		{
			pCltObj->SetLogFileTag(GetLogFileTag());

			pCltObj->SetSidHigh(m_dwSidHigh);
			m_ClientsPtr[x] = pCltObj;
			pCltObj->SetIndex(x);
		} 
	} 
	
	return m_ClientsPtr != NULL; 
} 

CServerBaseCltObj* CServerBase::AssignClientObject() 
{ 
	CServerBaseCltObj* pCltObj = NULL; 
	if ((!m_ClientsPtr) && !AllocateClientPool()) 
	{
		return NULL; 
	}
		
	for (int x = 0; x < m_nMaxClients; x++) 
	{ 
		assert(m_ClientsPtr[x]);
		if (m_ClientsPtr[x] && !m_ClientsPtr[x]->IsConnect()) 
		{ 
			pCltObj = m_ClientsPtr[x]; 
			break; 
		} 
	}
	
	if (!pCltObj)
	{
		LogImportant("CServerBase::AssignClientObject(), \n server(%s) \n fail to assign client ptr. the client count is reach the max=%d", 
			GetTagMyInfo().c_str(), m_nMaxClients);
	}
	
	return pCltObj; 
} 

CServerBaseCltObj* CServerBase::CreateClientObject() 
{ 
	CServerBaseCltObj *p = new CServerBaseCltObj(this);
	if (p != NULL)
	{
		p->SetLogFileTag(GetLogFileTag());
	}
	
	return p;
} 

void CServerBase::UpdateMaxClientBoundOnConnect(CServerBaseCltObj* pCltObj)
{
	//当客户端连接的时候更新
	m_nCurMaxClientBound = MAX(m_nCurMaxClientBound, pCltObj->GetIndex() + 1);
	LogImportant("CServerBase::UpdateMaxClientBoundOnConnect(), \n server(%s) \n update max client bound=%d", 
		GetTagMyInfo().c_str(), m_nCurMaxClientBound); 
}

void CServerBase::UpdateMaxClientBoundOnClose(CServerBaseCltObj* pCltObj)
{
	if (pCltObj->GetIndex() + 1 != m_nCurMaxClientBound)
	{
		LogImportant("CServerBase::UpdateMaxClientBoundOnClose(1), \n server(%s) \n update max client bound=%d", 
			GetTagMyInfo().c_str(), m_nCurMaxClientBound); 
		return;
	}
		
	//当客户端关闭的时候更新，从后往前找一个没有关闭的客户端的索引
	for (int i = m_nCurMaxClientBound - 1; i >= 0; i--)
	{  
		if (m_ClientsPtr[i] && m_ClientsPtr[i]->IsConnect())  
		{  
			m_nCurMaxClientBound = i + 1;
			LogImportant("CServerBase::UpdateMaxClientBoundOnClose(2), \n server(%s) \n update max client bound=%d", 
				GetTagMyInfo().c_str(), m_nCurMaxClientBound); 
			return;
		}  
	} 
	
	m_nCurMaxClientBound = 0;
	LogImportant("CServerBase::UpdateMaxClientBoundOnClose(3), \n server(%s) \n update max client bound=%d", 
		GetTagMyInfo().c_str(), m_nCurMaxClientBound); 
}

void CServerBase::OnFdExcept() 
{ 
}

void CServerBase::CloseClientObj(CSid objSid) 
{  
	CServerBaseCltObj* pClient = GetClientObj(objSid);
	if (pClient != NULL)
	{
		CloseClientObj(pClient);
	}
}

void CServerBase::CloseClientObj(const char* szCltIP, unsigned short nCltPort)
{
	CServerBaseCltObj* pClient = GetClientObj(szCltIP, nCltPort);
	if (pClient != NULL)
	{
		CloseClientObj(pClient);
	}
}

void CServerBase::CloseClientObj(CServerBaseCltObj* pCltObj) 
{
	if (!IsMyClientObj(pCltObj))
	{
		return;
	}
	
	if (pCltObj->IsSocketValid())
	{ 
		OnSessionClose(pCltObj->GetSid());  
		pCltObj->Close(); 
		
		//某个客户端关闭了，更新一下m_nCurMaxClientBound
		UpdateMaxClientBoundOnClose(pCltObj);
	}
}  
 
int CServerBase::Read(CServerBaseCltObj* pCltObj) 
{  
	assert(pCltObj);
	if (!pCltObj->IsSocketValid())	 
	{
		return -1;
	} 
	 
	OnRead(pCltObj); 
 
	CBuffer* pBuf = pCltObj->GetRecvTmpBuf(); 
	if (pBuf->GetAvailableSize() == 0) 
	{
		pBuf->Extend(); 
	}
 
	int nRet = recv(pCltObj->GetSocket(), pBuf->GetAvailableBuf(),  
					pBuf->GetAvailableSize(), 0); 
	if (nRet > 0) 
	{ 
		pBuf->Use(nRet); 

		LogDebug("CServerBase::Read(), \n connection(%s) \n read=%d, now recv_buffer=%d", 
			pCltObj->GetTagConnInfo().c_str(), nRet, pBuf->GetUsed()); 
//		LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
		
		OnReadCompleted(pCltObj); 
	} 
	else if (nRet == 0) 
	{ 
		LogImportant("CServerBase::Read(), \n connection(%s) \n read disconnect [recv()=0] from client, so will close client socket ...", 
			pCltObj->GetTagConnInfo().c_str());

		OnReadClientClose(pCltObj);
	} 
	else 
	{ 
		OnReadError(pCltObj); 
	} 
	 
	return nRet; 
}   

void CServerBase::OnRead(CServerBaseCltObj* pCltObj) 
{ 
	assert(pCltObj);
	pCltObj->SetRecvTime(); 

// 	LogDebug("CServerBase::OnRead(), \n connection(%s) \n cur read data len=%d",
// 			pCltObj->GetTagConnInfo().c_str(), pCltObj->GetRecvTmpBuf()->GetUsed());
} 
 
void CServerBase::OnReadCompleted(CServerBaseCltObj* pCltObj) 
{ 
	assert(pCltObj);

	CBuffer* pBuf = pCltObj->GetRecvTmpBuf(); 
	assert(pBuf);

	while(pBuf->GetUsed() > 0) 
	{ 
		int nPacketSize = GetRecvPacketSize(pBuf); 
		if (nPacketSize <= 0 || nPacketSize > GetMaxPacketSize()) 
		{ 
			LogImportant("CServerBase::OnReadCompleted(), \n connection(%s) \n the length(%d) in packet is larger than the max(%d), so will close socket ...", 
				pCltObj->GetTagConnInfo().c_str(), nPacketSize, GetMaxPacketSize()); 
			LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());

			CloseClientObj(pCltObj); 
			break; 
		} 
		else if (nPacketSize > pBuf->GetUsed())  // 数据包未接收完整
		{ 
			break; 
		} 
	 	else
		{ 
			CBuffer *pNewBuf = pBuf->Split(nPacketSize); 

			// 统计数据包接收量
//			pCltObj->StreamStatRecv(pNewBuf);

			LogDebug("CServerBase::OnReadCompleted(), \n connection(%s) \n split one pkt length=%d, now recv_buffer=%d", 
				pCltObj->GetTagConnInfo().c_str(), pNewBuf->GetUsed(), pBuf->GetUsed());
			
			ProcessStreamPacket(pCltObj, pNewBuf); 
		} 
	}
} 
  
void CServerBase::ProcessStreamPacket(CServerBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf); 
	LogDebug("CServerBase::ProcessStreamPacket(), \n connection(%s) \n recv a packet, len=%d.", 
		pCltObj->GetTagConnInfo().c_str(), pBuf->GetUsed());
	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());

	AddRecvPacketToQueue(pCltObj, pBuf);
} 

void CServerBase::AddRecvPacketToQueue(CServerBaseCltObj* pCltObj, CBuffer* pBuf)
{
	CSid objSid = pCltObj->GetSid(); 
	pBuf->SetWParam(objSid.GetHigh()); 
	pBuf->SetLParam(objSid.GetLow()); 

	if (!m_RecvQueue.AddTail(pBuf)) 
	{  
		LogImportant("CServerBase::ProcessStreamPacket(), \n connection(%s) \n Add data to recv queue fail.", 
			pCltObj->GetTagConnInfo().c_str());
		LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
		pBuf->Release(); 

		if (m_RecvQueue.IsOverflow()) 
		{
			LogImportant("CServerBase::ProcessStreamPacket(), \n connection(%s) \n recv queue(size=%d) overflow! so clear it!", 
				pCltObj->GetTagConnInfo().c_str(), m_RecvQueue.GetSize());
			ClearRecvQueue(); 
		}
	}
}

void CServerBase::OnReadClientClose(CServerBaseCltObj* pCltObj)
{
	assert (pCltObj);

	CloseClientObj(pCltObj); 
}

void CServerBase::OnReadError(CServerBaseCltObj* pCltObj) 
{ 
	int nErr = ERRNO;
	
	if (nErr != EAGAIN)
	{
		LogImportant("CServerBase::OnReadError(), \n connection(%s) \n read error=%d, so will close client socket ...",
			pCltObj->GetTagConnInfo().c_str(), nErr);
		CloseClientObj(pCltObj); 
	}
	else
	{
		LogImportant("CServerBase::OnReadError(), \n connection(%s) \n read error=%d (EAGAIN) from client(%s), continue ...", 
			pCltObj->GetTagConnInfo().c_str(), nErr);
	}
} 
  
bool CServerBase::IsHasData()
{
	return !m_RecvQueue.IsEmpty();
}

bool CServerBase::RecvData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort) 
{
	CSid objSid;
	bool bRet = RecvData(szBuf, nBufLen, objSid);
	
	CServerBaseCltObj* pCltObj = GetClientObj(objSid);
	if (pCltObj)
	{
		memcpy(szCltIP, pCltObj->GetPeerIp().c_str(), pCltObj->GetPeerIp().size());
		nCltPort = static_cast<unsigned short>(pCltObj->GetPeerPort());
	}
	
	return bRet;
}

bool CServerBase::RecvData(CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort) 
{  
	CSid objSid;
	bool bRet = RecvData(pBuf, objSid);
	
	CServerBaseCltObj* pCltObj = GetClientObj(objSid);
	if (pCltObj)
	{
		memcpy(szCltIP, pCltObj->GetPeerIp().c_str(), pCltObj->GetPeerIp().size());
		nCltPort = static_cast<unsigned short>(pCltObj->GetPeerPort());
	}
	
	return bRet;
}

bool CServerBase::RecvData(char* szBuf, int& nBufLen, CSid& objSid) 
{ 
	CBuffer* pBuf; 
	bool bRet = false; 
	if (m_RecvQueue.PeekHead(pBuf)) 
	{ 
		if (nBufLen < pBuf->GetUsed()) 
		{ 
			nBufLen = pBuf->GetUsed();
		}
		else if (m_RecvQueue.GetHead(pBuf)) 
		{
			objSid.Import(pBuf->GetWParam(), pBuf->GetLParam());
			memcpy(szBuf, pBuf->GetBuffer(), pBuf->GetUsed()); 
			nBufLen = pBuf->GetUsed(); 
			pBuf->Release();
			bRet = true;  
		} 
		else 
		{ 
			nBufLen = 0;
		} 
	} 
	else
	{
		nBufLen = 0;
	}
	
	return bRet; 
} 
 
bool CServerBase::RecvData(CBuffer* pBuf, CSid& objSid) 
{  
	bool bRet = false;
	CBuffer *pRecv; 
	pBuf->Empty();
	if (m_RecvQueue.GetHead(pRecv)) 
	{
		objSid.Import(pRecv->GetWParam(), pRecv->GetLParam());
		pBuf->AddData(pRecv);
		pRecv->Release();
		bRet = true;
	}
	
	return bRet; 
} 

bool CServerBase::SendData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort) 
{  
	return SendData(pBuf->GetBuffer(), pBuf->GetUsed(), szCltIP, nCltPort);
} 

bool CServerBase::SendData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort) 
{  
	CServerBaseCltObj* pCltObj = GetClientObj(szCltIP, nCltPort); 
	if (!pCltObj) 
	{
		LogImportant("CServerBase::SendData(), \n server(%s) \n send fail! can not find client for ip_port{%s:%d}", 
			GetTagMyInfo().c_str(), szCltIP, nCltPort); 
		return false; 
	}
	
	return SendData(szBuf, nBufLen, pCltObj);
}

bool CServerBase::SendData(CBuffer* pBuf, CSid objSid) 
{  
	return SendData(pBuf->GetBuffer(), pBuf->GetUsed(), objSid);
} 

bool CServerBase::SendData(char* szBuf, int nBufLen, CSid objSid) 
{  
	CServerBaseCltObj* pCltObj = GetClientObj(objSid); 
	if (!pCltObj) 
	{
		LogImportant("CServerBase::SendData(), \n server(%s) \n send fail! can not find client for objSid{%d,%d}", 
			GetTagMyInfo().c_str(), objSid.GetHigh(), objSid.GetLow()); 
		return false; 
	}
	
	return SendData(szBuf, nBufLen, pCltObj);
} 

bool CServerBase::SendData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj)
{
	assert(pCltObj);
	assert(IsMyClientObj(pCltObj));

	CBuffer* pBuf = pCltObj->Allocate();
	pBuf->AddData(szBuf, nBufLen);

	// 添加到发送队列	
	pBuf->SetWParam(pBuf->GetUsed());
	pBuf->SetFlag(0);
	pCltObj->PushSendData(pBuf);
	
	return true; 
}

void CServerBase::DoPatrol() 
{ 
	if (IsNotCfged())
	{
		return;
	}

	if (m_nSockfd == INVALID_SOCKET)
	{
		Startup();
	}

	if (m_ClientsPtr) 
	{ 
		CServerBaseCltObj* pCltObj = NULL;
		int nMaxClientBound = GetMaxClientBound();
		for (int i = 0; i < nMaxClientBound; i++) 
		{ 
			pCltObj = m_ClientsPtr[i];
			if (pCltObj && pCltObj->IsConnect())
			{
				PatrolClientObject(pCltObj);
			}
		} 
	} 
	
	OnPatrol(); 
} 

void CServerBase::DoPatrol(vector<CServerBaseCltObj*>& vecCltObj)
{ 
	if (IsNotCfged())
	{
		return;
	}

	CServerBaseCltObj* pCltObj = NULL;
	for (int i = 0; i < vecCltObj.size(); i++) 
	{ 
		pCltObj = vecCltObj[i];
		assert(pCltObj);
		if (pCltObj && IsMyClientObj(pCltObj) && pCltObj->IsConnect())
		{
			PatrolClientObject(pCltObj);
		}
	} 
	
	OnPatrol(); 
} 

bool CServerBase::IsNotCfged()
{
	if (m_nType == AF_INET)
	{
		return strlen(m_szSvrIp) <=0 || m_sSvrPort == 0;
	}
	else
	{
		return strlen(m_szUnixPath) <=0;
	}
}

void CServerBase::OnPatrol() 
{ 
} 

void CServerBase::PatrolClientObject(CServerBaseCltObj* pCltObj)
{
	assert(pCltObj);

	if (pCltObj->GetSendQueueSize() > pCltObj->GetSendQueueDepth())
	{
		LogImportant("CServerBase::PatrolClient(), \n connection(%s) \n send queue is too long, so will close client socket ...",
			pCltObj->GetTagConnInfo().c_str());
		CloseClientObj(pCltObj);
	}
	
	if (pCltObj->IsSocketValid())
	{
		while (pCltObj->GetSendQueueSize() > 0)
		{
			if (!Write(pCltObj))
			{
				break;
			}
		}
	}
}

bool CServerBase::Write(CServerBaseCltObj* pCltObj) 
{ 
	assert(pCltObj);
	
	if (!pCltObj->IsSocketValid())
	{
		LogImportant("CServerBase::Write(), \n connection(%s) \n connect is not ready, socket is invalid. send queue size=%d", 
			pCltObj->GetTagConnInfo().c_str(), pCltObj->GetSendQueueSize());
		return false;
	}

	if (!pCltObj->IsConnect())
	{
		LogImportant("CServerBase::Write(), \n connection(%s) \n connect is not ready, conn_status=%d. send queue size=%d",	
			pCltObj->GetTagConnInfo().c_str(), pCltObj->GetConnectStatus(), pCltObj->GetSendQueueSize());
		return false;
	}

	CBuffer* pBuf = pCltObj->PopSendData(); 
	if (!pBuf) 
	{
		return false;
	}
	
	bool bRet = true;
	if (pBuf->GetUsed() > 0) 
	{ 
		OnWrite(pCltObj, pBuf);  
  
		int nRet = send(pCltObj->GetSocket(), pBuf->GetBuffer(), pBuf->GetUsed(), 0);  
		
		if (nRet == pBuf->GetUsed())  
		{  
			OnWriteCompleted(pCltObj, pBuf);  
		}  
		else if (nRet > 0) 
		{  
			OnWritePart(pCltObj, pBuf, nRet);
			bRet = false;
		}
		else 
		{
			OnWriteError(pCltObj, pBuf);  
			bRet = false;
		}  
	} 
	else 
	{ 
		pBuf->Release(); 
		LogImportant("CServerBase::Write(), \n connection(%s) \n write error, send buf no data.",
			pCltObj->GetTagConnInfo().c_str()); 
	} 
	
	return bRet;
} 

void CServerBase::OnWrite(CServerBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	pCltObj->SetSendTime(); 

// 	LogDebug("CServerBase::OnWrite(), \n connection(%s) \n write data len=%d", 
// 			pCltObj->GetTagConnInfo().c_str(), pBuf->GetUsed());
// 	LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
} 
 
void CServerBase::OnWriteCompleted(CServerBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	assert(pCltObj && pBuf);

	LogDebug("CServerBase::OnWriteCompleted(), \n connection(%s) \n write pkt completed, total=%d, write=%d", 
		pCltObj->GetTagConnInfo().c_str(), pBuf->GetWParam(), pBuf->GetUsed());
	if (!pBuf->GetFlag()) // 只有包头部分打印日志，剩余部分打印日志无意义，而且有时候数量很多
	{
		LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	// 统计数据包发送量
//	pCltObj->StreamStatSend(pBuf);

	pBuf->Release(); 
} 
 
void CServerBase::OnWritePart(CServerBaseCltObj* pCltObj, CBuffer* pBuf, int nSend) 
{
	LogDebug("CServerBase::OnWritePart(), \n connection(%s) \n write pkt parted, total=%d, write=%d, left=%d", 
		pCltObj->GetTagConnInfo().c_str(), pBuf->GetWParam(), nSend, pBuf->GetUsed() - nSend);
	if (!pBuf->GetFlag()) // 只有包头部分打印日志，剩余部分打印日志无意义，而且有时候数量很多
	{
		LogDataDebug(pBuf->GetBuffer(), pBuf->GetUsed());
	}

	pBuf->Cut(0, nSend);
	if (pBuf->GetUsed() > 0)
	{
		pBuf->SetFlag(1);
		pCltObj->RollbackSendData(pBuf);

		LogDebug("CServerBase:OnWritePart(), \n connection(%s) \n roll back the left data to send queue, total=%d, left=%d",
			pCltObj->GetTagConnInfo().c_str(), pBuf->GetWParam(), pBuf->GetUsed());
	}
	else
	{
		LogDebug("CServerBase::OnWritePart(), \n connection(%s) \n left length=0.", 
			pCltObj->GetTagConnInfo().c_str());

		pBuf->Release();
	}
}

void CServerBase::OnWriteError(CServerBaseCltObj* pCltObj, CBuffer* pBuf) 
{ 
	int nErr = ERRNO;
	
	if (nErr == EAGAIN || nErr == EINTR)
	{
		pCltObj->RollbackSendData(pBuf);
		
		LogImportant("CServerBase::OnWriteError(), \n connection(%s) \n write error=%d (EAGAIN or EINTR), put data back to send queue.",
				pCltObj->GetTagConnInfo().c_str(), nErr);
	}
	else
	{
		LogImportant("CServerBase::OnWriteError(), \n connection(%s) \n write error=%d, so will close client socket ...", 
				pCltObj->GetTagConnInfo().c_str(), nErr);

		pBuf->Release(); 
		CloseClientObj(pCltObj);
	}
} 

CServerBaseCltObj* CServerBase::GetClientObjByIndex(int nIndex)
{
	if (!m_ClientsPtr)
	{
		return NULL;
	}
	
	return m_ClientsPtr[nIndex];
}

CServerBaseCltObj *CServerBase::GetClientObj(CSid& objSid) 
{ 
	if (!m_ClientsPtr)
	{
		return NULL;
	}
		
	CServerBaseCltObj* pCltObj = NULL; 
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
	for (int i = 0; i < nMaxClientBound; i++)  
	{  
		if (m_ClientsPtr[i] && m_ClientsPtr[i]->GetSid() == objSid) 
		{  
			pCltObj = m_ClientsPtr[i];  
			break;  
		}  
	}  
	
	return pCltObj; 
} 

CServerBaseCltObj *CServerBase::GetClientObj(const char* szCltIP, unsigned short nCltPort)
{
	if (!m_ClientsPtr)
	{
		return NULL;
	}
		
	CServerBaseCltObj* pCltObj = NULL; 
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
	for (int i = 0; i < nMaxClientBound; i++)  
	{  
		if (m_ClientsPtr[i] && m_ClientsPtr[i]->GetPeerPort() == static_cast<int>(nCltPort)
			&& strcmp(m_ClientsPtr[i]->GetPeerIp().c_str(), szCltIP) == 0) 
		{  
			pCltObj = m_ClientsPtr[i];  
			break;  
		}  
	}  
	
	return pCltObj; 
}

void CServerBase::OnSessionConnect(CSid objSid) 
{ 
} 

void CServerBase::OnSessionClose(CSid objSid) 
{ 
} 

std::string CServerBase::GetClientObjIp(CSid objSid)
{
	CServerBaseCltObj* pCltObj = GetClientObj(objSid); 
	if (pCltObj) 
	{
		return pCltObj->GetPeerIp();
	}
	
	return "";
}

bool CServerBase::IsMyClientObj( CServerBaseCltObj* pCltObj )
{
	if (!m_ClientsPtr || !pCltObj)
	{
		return NULL;
	}
	
	int nMaxClientBound = GetMaxClientBound(); //GetMaxClientBound()函数的返回值会受到CloseClient的影响
	for (int x = 0; x < nMaxClientBound; x++)  
	{  
		if (m_ClientsPtr[x] && m_ClientsPtr[x] == pCltObj) 
		{  
			return true;  
		}  
	}  
	
	return false; 
}

void CServerBase::DumpAllocator()
{
// 	LogImportant("CServerBase::DumpAllocator(), dump all client_obj m_objBufAlloc");
// 
// 	if (m_ClientsPtr)
// 	{
// 		for (int x = 0; x < m_nMaxClients; x++) 
// 		{ 
// 			CServerBaseCltObj* pCltObj = m_ClientsPtr[x]; 
// 			assert(pCltObj);
// 			
// 			pCltObj->DumpAllocator();
// 		}
// 	}
}

//==============================================================================
//                 
//                    CServerBaseCltObj
//              服务端基类的客户端对象类
//
//==============================================================================
CServerBaseCltObj::CServerBaseCltObj(CServerBase* pServer) :
		m_pRelatedServerPtr(pServer),
		m_nCltIndex(-1),
		m_dwSidHigh(0),
		m_nSendQueueDepth(SEND_QUEUE_DEPTH)
{
	SetMyClassName("CServerBaseCltObj");

	m_pRecvTmpBuf = Allocate();
	assert(m_pRecvTmpBuf);
	
	ClearCltInfo();
}

CServerBaseCltObj::~CServerBaseCltObj()
{
	Close();
	
	if (m_pRecvTmpBuf)
	{
		m_pRecvTmpBuf->Release();
	}
}

void CServerBaseCltObj::ClearCltInfo()
{
	m_objSid.Abolish();
	m_nSockfd = INVALID_SOCKET;

	memset(m_szPeerName, 0, SESSION_NAME_LEN);
	memset(m_szPeerIp, 0, IP_LEN);
	m_sPeerPort = 0;

	SetConnectStatus(CS_DISCONNECT);

	if (m_pRecvTmpBuf)
	{
		m_pRecvTmpBuf->Empty();
		m_pRecvTmpBuf->Shrink();
	}
	
	ClearSendQueue();
	
	m_dwSendTime = CUtility::GetUptime(); 
	m_dwRecvTime = CUtility::GetUptime(); 
}

string CServerBaseCltObj::GetTagCltInfo()
{
	char temp[256];
	memset(temp, 0, sizeof(temp));
	
	snprintf(temp, sizeof(temp), "%s:%d", m_szPeerIp, m_sPeerPort);

	return temp;
}

std::string CServerBaseCltObj::GetTagConnInfo()
{
	assert(m_pRelatedServerPtr);

	string strConn = "";
	strConn += "<s:";
	strConn += m_pRelatedServerPtr->GetTagMyInfo();
	strConn += ">-<c:";
	strConn += GetTagCltInfo();
	strConn += ">";
	return strConn;
}

void CServerBaseCltObj::Connect(int nSockfd, const struct sockaddr_in& stuSockAddr)
{
	ClearCltInfo();

	m_objSid.Asign(m_dwSidHigh);
	m_nSockfd = nSockfd;

	strncpy(m_szPeerIp, inet_ntoa(stuSockAddr.sin_addr), IP_LEN);
	m_sPeerPort = ntohs(stuSockAddr.sin_port);

	SetConnectStatus(CS_CONNECT); 

	m_dwRecvTime = CUtility::GetUptime();
	m_dwSendTime = CUtility::GetUptime();

	OnConnect();
}

void CServerBaseCltObj::Connect(int nSockfd)
{
	ClearCltInfo();

	m_objSid.Asign(m_dwSidHigh);
	m_nSockfd = nSockfd;
	
	SetConnectStatus(CS_CONNECT); 

	m_dwRecvTime = CUtility::GetUptime();
	m_dwSendTime = CUtility::GetUptime();

	OnConnect();
}

bool CServerBaseCltObj::Close()
{
	if (SOCKET_VALID(m_nSockfd))
	{
		CLOSE_SOCKET(m_nSockfd);
		
		LogImportant("CServerBaseCltObj::Close(), \n connection(%s) \n close ok!", 
			GetTagConnInfo().c_str()); 
		OnClose();
		ClearCltInfo();
		
		return true;
	}
	else
	{
		LogImportant("CServerBaseCltObj::Close(), \n connection(%s) \n close fail because socket is invalid!", 
			GetTagConnInfo().c_str()); 
		ClearCltInfo();
		
		return false;
	}
}

bool CServerBaseCltObj::IsConnect() 
{ 
	return GetConnectStatus() >= CS_CONNECT;	 
}

void CServerBaseCltObj::ClearSendQueue()
{
	// 队列清空前输出日志
	int nLastPktCount = m_SendQueue.GetSize();
	if (nLastPktCount > 0)
	{
		if (IsConnect())
		{
			LogImportant("CServerBaseCltObj::ClearSendQueue(), \n connection(%s) \n it is connecting, queue_size=%d, will throw packets ...", 
				GetTagConnInfo().c_str(), nLastPktCount);
		}
		else
		{
			LogImportant("CServerBaseCltObj::ClearSendQueue(), \n connection(%s) \n it is disconnected, queue_size=%d, will throw packets ...", 
				GetTagConnInfo().c_str(), nLastPktCount);
		}
	}

	// 执行队列清空操作
	CBuffer* pBuf = NULL;  
	while (!m_SendQueue.IsEmpty())
	{
		if (m_SendQueue.GetHead(pBuf)) 
		{
// 			if (IsConnect())
// 			{
// 				LogDataImportant(pBuf->GetBuffer(), pBuf->GetUsed());
// 			}
			pBuf->Release(); 
		}
		else 
		{
			break; 
		}
	}

	// 队列清空后输出日志
	int nThrowPktCount = nLastPktCount - m_SendQueue.GetSize();
	if (nThrowPktCount > 0 || m_SendQueue.GetSize() > 0)
	{
		LogImportant("CServerBaseCltObj::ClearSendQueue(), \n connection(%s) \n succeed to throw %d packets, now queue_size=%d.", 
			GetTagConnInfo().c_str(), nThrowPktCount, m_SendQueue.GetSize());
	}
}

void CServerBaseCltObj::OnConnect()
{
}

void CServerBaseCltObj::OnClose()
{
	m_objSid.Abolish();
}

void CServerBaseCltObj::SetLogFileTag(const char* szFileTag)
{
	CObjectBase::SetLogFileTag(szFileTag);

	// 设置数据包发送/接收量统计类日志
//	m_objStreamStatSend.SetLogFileTag(szFileTag);
//	m_objStreamStatRecv.SetLogFileTag(szFileTag);
}

CSid CServerBaseCltObj::GetSid()			
{ 
	return m_objSid;		
} 

int  CServerBaseCltObj::GetSocket() 
{ 
	return m_nSockfd;	
}

bool CServerBaseCltObj::IsSocketValid()	
{ 
	return SOCKET_VALID(m_nSockfd);	
} 

void CServerBaseCltObj::SetPeerName(const char* szPeerName)
{
	assert(szPeerName && strlen(szPeerName) < sizeof(m_szPeerName));
	strcpy(m_szPeerName, szPeerName);
}

string CServerBaseCltObj::GetPeerName()
{ 
	return m_szPeerName;	
} 

void CServerBaseCltObj::SetPeerIP(const char* szPeerIp)
{
	assert(szPeerIp && strlen(szPeerIp) < sizeof(m_szPeerIp));
	strcpy(m_szPeerIp, szPeerIp);
}

string CServerBaseCltObj::GetPeerIp()
{
	return m_szPeerIp;
}

void CServerBaseCltObj::SetPeerPort(unsigned short sPeerPort)
{
	assert(sPeerPort > 1024);
	m_sPeerPort = sPeerPort;
}

int CServerBaseCltObj::GetPeerPort()
{
	return static_cast<int>(m_sPeerPort);
}

void CServerBaseCltObj::SetIndex(int nIndex) 
{
	m_nCltIndex = nIndex; 
}

int CServerBaseCltObj::GetIndex() 
{ 
	return m_nCltIndex; 
}

void CServerBaseCltObj::SetConnectStatus(int nStatus)	
{ 
	m_nConnectStatus = nStatus;	
} 

int CServerBaseCltObj::GetConnectStatus()
{ 
	return m_nConnectStatus;	
} 

void CServerBaseCltObj::SetSidHigh(DWORD dwSidHigh)  
{ 
	m_dwSidHigh = dwSidHigh; 
}

DWORD CServerBaseCltObj::GetSidHigh()
{
	return m_dwSidHigh;
}

void CServerBaseCltObj::SetSendQueueDepth(int nSendQueueDepth)
{
	assert(nSendQueueDepth > 0);
	m_nSendQueueDepth = nSendQueueDepth;
}

int CServerBaseCltObj::GetSendQueueDepth()
{
	return m_nSendQueueDepth;
}

void  CServerBaseCltObj::SetRecvTime()		
{ 
	m_dwRecvTime = CUtility::GetUptime();
} 

DWORD CServerBaseCltObj::GetRecvTime()		
{ 
	return m_dwRecvTime;			
} 

void  CServerBaseCltObj::SetSendTime()		
{ 
	m_dwSendTime = CUtility::GetUptime();
} 

DWORD CServerBaseCltObj::GetSendTime()		
{ 
	return m_dwSendTime;			
} 

CBuffer* CServerBaseCltObj::Allocate()
{
	return m_objBufAlloc.Allocate();
}

CServerBase* CServerBaseCltObj::GetRelatedServerPtr()
{
	return m_pRelatedServerPtr;
}

CBuffer* CServerBaseCltObj::GetRecvTmpBuf()		
{ 
	return m_pRecvTmpBuf;	
} 

bool CServerBaseCltObj::PushSendData(CBuffer* pBuf)
{
	assert(pBuf);
	
	if (m_SendQueue.GetSize() >= m_nSendQueueDepth)
	{
		LogImportant("CServerBaseCltObj::PushSendData(), \n connection(%s) \n send queue is full! queue size=%d, max=%d",
			GetTagConnInfo().c_str(), m_SendQueue.GetSize(), m_nSendQueueDepth);
	}
	
	if (m_SendQueue.GetSize() > 64)
	{
		LogDebug("CServerBaseCltObj::PushSendData(), \n connection(%s) \n send queue size=%d,", 
			GetTagConnInfo().c_str(), m_SendQueue.GetSize());
	}
	
	return m_SendQueue.AddTail(pBuf);
}

int CServerBaseCltObj::GetSendQueueSize()
{ 
	return m_SendQueue.GetSize(); 
}

bool CServerBaseCltObj::IsHasSendData()
{ 
	return !m_SendQueue.IsEmpty();
}

CBuffer* CServerBaseCltObj::PopSendData()
{
	CBuffer* pBuf = NULL;
	m_SendQueue.GetHead(pBuf);
	
	return pBuf;
}

bool CServerBaseCltObj::RollbackSendData(CBuffer* pBuf)
{
	if (m_SendQueue.GetSize() >= m_nSendQueueDepth)
	{
// 		LogImportant("CServerBaseCltObj::RollbackSendData(), \n connection(%s) \n send queue is full, clear queue, data lost.", 
// 			GetTagConnInfo().c_str());
// 		m_SendQueue.Empty();
	}
	
	return m_SendQueue.AddHead(pBuf);
}

bool CServerBaseCltObj::IsSendQueueInPart()
{
	if (m_SendQueue.IsEmpty())
	{
		return false; 
	}
	
	CBuffer* pBuf = NULL; 
	m_SendQueue.PeekHead(pBuf);
	return pBuf->GetFlag() == 1;
}

void CServerBaseCltObj::DumpAllocator()
{
// 	if (m_nCltIndex <= 10 || m_objBufAlloc.GetActiveSize() > 1 || m_objBufAlloc.GetFreeSize() > 1)
// 	{
// 		LogImportant("CServerBaseCltObj::DumpAllocator(), dump CServerBaseCltObj::m_objBufAlloc client_index=%d\nconnection(%s) conn status=%d\n%s", 
// 			m_nCltIndex, GetTagConnInfo().c_str(), IsConnect(), m_objBufAlloc.GetDumpStr().c_str());
// 	}
}

void CServerBaseCltObj::StreamStatSend(CBuffer* pBuf)
{
//	string strStatInfo = GetTagConnInfo();
//	strStatInfo += "  Send";
//	m_objStreamStatSend.OnDataStream(pBuf->GetUsed(), strStatInfo.c_str());
}

void CServerBaseCltObj::StreamStatRecv(CBuffer* pBuf)
{
//	string strStatInfo = GetTagConnInfo();
//	strStatInfo += "  Recv";
//	m_objStreamStatRecv.OnDataStream(pBuf->GetUsed(), strStatInfo.c_str());
}

