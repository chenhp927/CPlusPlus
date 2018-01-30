//==============================================================================
//                      CommIdDef.cpp
//
//begin   : 2016-09-13
//describe:
//==============================================================================
 
#include "CommIdDef.h" 

//==============================================================================
//                 
//                    CCommIdDef
//                 通信ID定义工具类
//
//==============================================================================
CCommIdDef::CCommIdDef()
{  
} 

CCommIdDef::~CCommIdDef() 
{  
}

string CCommIdDef::GetServerIdValidRangeStr(bool bIsAppSvr, BYTE byCheckForWho)
{
	// 业务程序服务端
	if (bIsAppSvr)
	{
		return "(ServerId for App: Null)";
	}
	// 通信程序服务端
	else
	{
		switch (byCheckForWho)
		{
			case SERVERID_FOR_NULL:				// 未知程序的服务端ID
			{
				return "(ServerId for Comm: Null)";
			}

			case SERVERID_FOR_COMMINNER:		// 内部数据转发服务的服务端ID
			{
				return "(ServerId for CommInner: 1-10)";
			}

			case SERVERID_FOR_COMMOUTER:		// 外部数据转发服务的服务端ID
			{
				return "(ServerId for CommOuter: 11-20)";
			}

			case SERVERID_FOR_COMMTERM:			// 终端数据转发服务的服务端ID
			{
				return "(ServerId for CommTerm: 21-30)";
			}

			case SERVERID_FOR_COMMDUAL:			// 双机同步交互服务的服务端ID
			{
				return "(ServerId for CommDual: 51-60)";
			}
			
			default:
			{
				return "(ServerId for Comm: Null)";
			}
		}
	}
	
	return "(Null)";
}

bool CCommIdDef::IsServerIdValid(WORD wSvrId, bool bIsAppSvr, BYTE byCheckForWho)
{
	// 业务程序服务端
	if (bIsAppSvr)
	{
		return false;		
	}
	// 通信程序服务端
	else
	{
		switch (byCheckForWho)
		{
			case SERVERID_FOR_NULL:				// 未知程序的服务端ID
			{
				return false;
			}

			case SERVERID_FOR_COMMINNER:		// 内部数据转发服务的服务端ID
			{
				return wSvrId >= 1 && wSvrId <= 10;
			}

			case SERVERID_FOR_COMMOUTER:		// 外部数据转发服务的服务端ID
			{
				return wSvrId >= 11 && wSvrId <= 20;
			}

			case SERVERID_FOR_COMMTERM:			// 终端数据转发服务的服务端ID
			{
				return wSvrId >= 21 && wSvrId <= 30;
			}

			case SERVERID_FOR_COMMDUAL:			// 双机同步交互服务的服务端ID
			{
				return wSvrId >= 51 && wSvrId <= 60;
			}
				
			default:
			{
				return false;		
			}
		}
	}
	
	return false;
}

string CCommIdDef::GetCliendIdValidRangeStr(bool bIsAppClt, BYTE byCheckForWho)
{
	// 业务程序客户端
	if (bIsAppClt)
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:		// 未知程序的客户端ID
			{
				return "(ClientId for App: Null)";
			}

			case CLIENTID_FOR_SVRAPP:	// 服务类业务程序的客户端ID
			{
				return "(ClientId for SvrApp: 101-200)";
			}

			case CLIENTID_FOR_TERMAPP:	// 终端类业务程序的客户端ID
			{
				return "(ClientId for TermApp: 201-250)";
			}

			default:
			{
				return "(ClientId for App: Null)";		
			}
		}
	}
	// 通信程序客户端
	else
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:			// 未知程序的程序的客户端ID
			{
				return "(ClientId for Comm: Null)";
			}

			case CLIENTID_FOR_COMMOUTER:	// 外部数据转发服务的客户端ID
			{
				return "(ClientId for CommOuter: 11-20)";
			}

			case CLIENTID_FOR_COMMTERM:		// 终端数据转发服务的客户端ID
			{
				return "(ClientId for CommTerm: 21-30)";
			}

			case CLIENTID_FOR_COMMCHANNEL:	// 内部通道连通服务的客户端ID
			{
				return "(ClientId for CommChannel: 31-40)";
			}

			case CLIENTID_FOR_COMMBRIDGE:	// 外部通道桥接服务的客户端ID
			{
				return "(ClientId for CommBridge: 41-50)";
			}

			case CLIENTID_FOR_COMMDUAL:		// 双机同步交互服务的客户端ID
			{
				return "(ClientId for CommDual: 51-60)";
			}
				
			default:
			{
				return "(ClientId for Comm: Null)";
			}
		}
	}
	
	return "(Null)";
}

bool CCommIdDef::IsClientIdValid(WORD wCltId, bool bIsAppClt, BYTE byCheckForWho)
{
	// 业务程序客户端
	if (bIsAppClt)
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:		// 未知程序的客户端ID
			{
				return false;
			}

			case CLIENTID_FOR_SVRAPP:	// 服务类业务程序的客户端ID
			{
				return wCltId >= 101 && wCltId <= 200;
			}

			case CLIENTID_FOR_TERMAPP:	// 终端类业务程序的客户端ID
			{
				return wCltId >= 201 && wCltId <= 250;
			}

			default:
			{
				return false;		
			}
		}
	}
	// 通信程序客户端
	else
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:			// 未知程序的客户端ID
			{
				return false;
			}

			case CLIENTID_FOR_COMMOUTER:	// 外部数据转发服务的客户端ID
			{
				return wCltId >= 11 && wCltId <= 20;
			}

			case CLIENTID_FOR_COMMTERM:		// 终端数据转发服务的客户端ID
			{
				return wCltId >= 21 && wCltId <= 30;
			}

			case CLIENTID_FOR_COMMCHANNEL:	// 内部通道连通服务的客户端ID
			{
				return wCltId >= 31 && wCltId <= 40;
			}

			case CLIENTID_FOR_COMMBRIDGE:	// 外部通道桥接服务的客户端ID
			{
				return wCltId >= 41 && wCltId <= 50;
			}

			case CLIENTID_FOR_COMMDUAL:		// 双机同步交互服务的客户端ID
			{
				return wCltId >= 51 && wCltId <= 60;
			}
				
			default:
			{
				return false;		
			}
		}
	}
	
	return false;
}


