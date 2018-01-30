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
//                 ͨ��ID���幤����
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
	// ҵ���������
	if (bIsAppSvr)
	{
		return "(ServerId for App: Null)";
	}
	// ͨ�ų�������
	else
	{
		switch (byCheckForWho)
		{
			case SERVERID_FOR_NULL:				// δ֪����ķ����ID
			{
				return "(ServerId for Comm: Null)";
			}

			case SERVERID_FOR_COMMINNER:		// �ڲ�����ת������ķ����ID
			{
				return "(ServerId for CommInner: 1-10)";
			}

			case SERVERID_FOR_COMMOUTER:		// �ⲿ����ת������ķ����ID
			{
				return "(ServerId for CommOuter: 11-20)";
			}

			case SERVERID_FOR_COMMTERM:			// �ն�����ת������ķ����ID
			{
				return "(ServerId for CommTerm: 21-30)";
			}

			case SERVERID_FOR_COMMDUAL:			// ˫��ͬ����������ķ����ID
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
	// ҵ���������
	if (bIsAppSvr)
	{
		return false;		
	}
	// ͨ�ų�������
	else
	{
		switch (byCheckForWho)
		{
			case SERVERID_FOR_NULL:				// δ֪����ķ����ID
			{
				return false;
			}

			case SERVERID_FOR_COMMINNER:		// �ڲ�����ת������ķ����ID
			{
				return wSvrId >= 1 && wSvrId <= 10;
			}

			case SERVERID_FOR_COMMOUTER:		// �ⲿ����ת������ķ����ID
			{
				return wSvrId >= 11 && wSvrId <= 20;
			}

			case SERVERID_FOR_COMMTERM:			// �ն�����ת������ķ����ID
			{
				return wSvrId >= 21 && wSvrId <= 30;
			}

			case SERVERID_FOR_COMMDUAL:			// ˫��ͬ����������ķ����ID
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
	// ҵ�����ͻ���
	if (bIsAppClt)
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:		// δ֪����Ŀͻ���ID
			{
				return "(ClientId for App: Null)";
			}

			case CLIENTID_FOR_SVRAPP:	// ������ҵ�����Ŀͻ���ID
			{
				return "(ClientId for SvrApp: 101-200)";
			}

			case CLIENTID_FOR_TERMAPP:	// �ն���ҵ�����Ŀͻ���ID
			{
				return "(ClientId for TermApp: 201-250)";
			}

			default:
			{
				return "(ClientId for App: Null)";		
			}
		}
	}
	// ͨ�ų���ͻ���
	else
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:			// δ֪����ĳ���Ŀͻ���ID
			{
				return "(ClientId for Comm: Null)";
			}

			case CLIENTID_FOR_COMMOUTER:	// �ⲿ����ת������Ŀͻ���ID
			{
				return "(ClientId for CommOuter: 11-20)";
			}

			case CLIENTID_FOR_COMMTERM:		// �ն�����ת������Ŀͻ���ID
			{
				return "(ClientId for CommTerm: 21-30)";
			}

			case CLIENTID_FOR_COMMCHANNEL:	// �ڲ�ͨ����ͨ����Ŀͻ���ID
			{
				return "(ClientId for CommChannel: 31-40)";
			}

			case CLIENTID_FOR_COMMBRIDGE:	// �ⲿͨ���Žӷ���Ŀͻ���ID
			{
				return "(ClientId for CommBridge: 41-50)";
			}

			case CLIENTID_FOR_COMMDUAL:		// ˫��ͬ����������Ŀͻ���ID
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
	// ҵ�����ͻ���
	if (bIsAppClt)
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:		// δ֪����Ŀͻ���ID
			{
				return false;
			}

			case CLIENTID_FOR_SVRAPP:	// ������ҵ�����Ŀͻ���ID
			{
				return wCltId >= 101 && wCltId <= 200;
			}

			case CLIENTID_FOR_TERMAPP:	// �ն���ҵ�����Ŀͻ���ID
			{
				return wCltId >= 201 && wCltId <= 250;
			}

			default:
			{
				return false;		
			}
		}
	}
	// ͨ�ų���ͻ���
	else
	{
		switch (byCheckForWho)
		{
			case CLIENTID_FOR_NULL:			// δ֪����Ŀͻ���ID
			{
				return false;
			}

			case CLIENTID_FOR_COMMOUTER:	// �ⲿ����ת������Ŀͻ���ID
			{
				return wCltId >= 11 && wCltId <= 20;
			}

			case CLIENTID_FOR_COMMTERM:		// �ն�����ת������Ŀͻ���ID
			{
				return wCltId >= 21 && wCltId <= 30;
			}

			case CLIENTID_FOR_COMMCHANNEL:	// �ڲ�ͨ����ͨ����Ŀͻ���ID
			{
				return wCltId >= 31 && wCltId <= 40;
			}

			case CLIENTID_FOR_COMMBRIDGE:	// �ⲿͨ���Žӷ���Ŀͻ���ID
			{
				return wCltId >= 41 && wCltId <= 50;
			}

			case CLIENTID_FOR_COMMDUAL:		// ˫��ͬ����������Ŀͻ���ID
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


