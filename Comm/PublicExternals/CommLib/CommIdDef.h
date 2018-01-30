//==============================================================================
//                      CommIdDef.h
//
//begin   : 2016-09-13
//describe:
//==============================================================================
#ifndef SELFDEF_COMMIDDEF_H 
#define SELFDEF_COMMIDDEF_H 
 
#include <string.h>
#include <string>
#include <vector>

#include "TypeDef.h"

#define SERVERID_FOR_NULL				1	// 未知程序的服务端ID
#define SERVERID_FOR_COMMINNER			2	// 内部数据转发服务的服务端ID
#define SERVERID_FOR_COMMOUTER			3	// 外部数据转发服务的服务端ID
#define SERVERID_FOR_COMMTERM			4	// 终端数据转发服务的服务端ID
#define SERVERID_FOR_COMMDUAL			5	// 双机同步交互服务的服务端ID

#define CLIENTID_FOR_NULL				11	// 未知程序的客户端ID
#define CLIENTID_FOR_SVRAPP				12	// 服务类业务程序的客户端ID
#define CLIENTID_FOR_TERMAPP			13	// 终端类业务程序的客户端ID
#define CLIENTID_FOR_COMMOUTER			21	// 外部数据转发服务的客户端ID
#define CLIENTID_FOR_COMMTERM			22	// 终端数据转发服务的客户端ID
#define CLIENTID_FOR_COMMBRIDGE			23	// 外部通道桥接服务的客户端ID
#define CLIENTID_FOR_COMMCHANNEL		24	// 内部通道连通服务的客户端ID
#define CLIENTID_FOR_COMMDUAL			25	// 双机同步交互服务的客户端ID

using namespace std;
//==============================================================================
//                 
//                    CCommIdDef
//                 通信ID定义工具类
//
//==============================================================================
class MY_EXT_CLASS CCommIdDef
{ 
public: 
	CCommIdDef(); 
	virtual ~CCommIdDef(); 
	
public:
	static string GetServerIdValidRangeStr(bool bIsAppSvr = true, BYTE byCheckForWho = SERVERID_FOR_NULL);
	static bool IsServerIdValid(WORD wSvrId, bool bIsAppSvr = true, BYTE byCheckForWho = SERVERID_FOR_NULL);
	
	static string GetCliendIdValidRangeStr(bool bIsAppClt = true, BYTE byCheckForWho = CLIENTID_FOR_NULL);
	static bool IsClientIdValid(WORD wCltId, bool bIsAppClt = true, BYTE byCheckForWho = CLIENTID_FOR_NULL);

}; 
   
#endif 
