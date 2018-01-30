//==============================================================================
//                      CommUtility.cpp
//
//begin   : 2016-09-21
//describe:
//==============================================================================

#include "CommUtility.h" 
#include "SockMacro.h"

//==============================================================================
//                 
//                    CCommUtility
//                  通信相关通用工具类
//
//==============================================================================
CCommUtility::CCommUtility() 
{ 
} 
 
CCommUtility::~CCommUtility() 
{ 
} 
 
bool CCommUtility::IsValidIP(const char* szIP) 
{ 
	if (!szIP)
	{
		return false;
	}

	return inet_addr(szIP) > 0;
}

bool CCommUtility::SockStart()
{
#ifndef UNIX
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return false;
	}
#endif
	
	return true;
}

bool CCommUtility::SetNonBlock(int nSockFd, bool bBlock)  
{
	int nRet = 0;  

#ifdef UNIX
	int nOpt = fcntl(nSockFd, F_GETFL, 0);  
	if (nOpt == -1)  
	{  
		return false;  
	} 
	
	if (bBlock) 
	{
		nRet = fcntl(nSockFd, F_SETFL, nOpt & ~O_NONBLOCK); 
	}
	else
	{
		nRet = fcntl(nSockFd, F_SETFL, nOpt | O_NONBLOCK);
	}

	if (nRet == -1)  
	{  
		return false;  
	}
#else
	unsigned long nMode = 0;
	if (!bBlock)
	{
		nMode = 1;
	}
	
	nRet = ioctlsocket(nSockFd, FIONBIO, &nMode);
	if (nRet != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %d\n", nRet);
		return false;
	}
#endif
	
	return true;  
}  
  
bool CCommUtility::SetSockBuf(int nSockFd, int nSize)
{
	assert(nSize > 0);
	bool bRet = true;
	int nLen = nSize;
	if (setsockopt(nSockFd, SOL_SOCKET, SO_RCVBUF, (const char*)&nLen, sizeof(nLen)))
	{
		perror("set recv buf");
		bRet = false;
	}

	if (bRet)
	{
		nLen = nSize;
		if (setsockopt(nSockFd, SOL_SOCKET, SO_SNDBUF, (const char*)&nLen, sizeof(nLen)))
		{
			perror("set send buf");
			bRet = false;
		}
	}

	return bRet;
}

bool CCommUtility::SetSockReuse(int nSockFd) 
{ 
	int nOpt = 1; 
	return (setsockopt(nSockFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOpt, sizeof(nOpt)) == 0); 
} 
 
bool CCommUtility::SetSockNodelay(int nSockFd) 
{ 
#ifdef HP_UX
	int nOpt = 1; 
	return (setsockopt(nSockFd, IPPROTO_TCP, TCP_NODELAY, (const char*)&nOpt, sizeof(nOpt)) == 0); 
#else
	return true;
#endif	
} 

bool CCommUtility::CheckSock(int nSockFd, int& nCheckErr)
{
	socklen_t nErrLen = sizeof(nCheckErr);
	
	if (getsockopt(nSockFd, SOL_SOCKET, SO_ERROR, (char*)&nCheckErr, &nErrLen) < 0)
	{
		return false;
	}
	else
	{
		return nCheckErr == 0;
	}
}

