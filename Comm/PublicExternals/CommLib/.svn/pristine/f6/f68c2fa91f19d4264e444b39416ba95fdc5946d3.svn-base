//==============================================================================
//                      CommUtility.h
//
//begin   : 2016-09-21
//describe:
//==============================================================================
#ifndef SELFDEF_COMMUTILITY_H 
#define SELFDEF_COMMUTILITY_H 

#include "TypeDef.h"

//==============================================================================
//                 
//                    CCommUtility
//                  通信相关通用工具类
//
//==============================================================================
class MY_EXT_CLASS CCommUtility 
{ 
public: 
	CCommUtility(); 
	virtual ~CCommUtility(); 

public: 
	/* 下列的接口为IP地址的相关处理 */
	static bool IsValidIP(const char* szIP); 

	/* 下列的接口为网络Socket的相关处理 */
	static bool SockStart();
	static bool SetNonBlock(int nSockFd, bool bBlock = false);
	static bool SetSockReuse(int nSockFd);
	static bool SetSockBuf(int nSockFd, int nSize = 256*1024);
	static bool SetSockNodelay(int nSockFd);
	static bool CheckSock(int nSockFd, int& nCheckErr);
}; 
 
 
#endif 
