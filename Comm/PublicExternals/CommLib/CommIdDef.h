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

#define SERVERID_FOR_NULL				1	// δ֪����ķ����ID
#define SERVERID_FOR_COMMINNER			2	// �ڲ�����ת������ķ����ID
#define SERVERID_FOR_COMMOUTER			3	// �ⲿ����ת������ķ����ID
#define SERVERID_FOR_COMMTERM			4	// �ն�����ת������ķ����ID
#define SERVERID_FOR_COMMDUAL			5	// ˫��ͬ����������ķ����ID

#define CLIENTID_FOR_NULL				11	// δ֪����Ŀͻ���ID
#define CLIENTID_FOR_SVRAPP				12	// ������ҵ�����Ŀͻ���ID
#define CLIENTID_FOR_TERMAPP			13	// �ն���ҵ�����Ŀͻ���ID
#define CLIENTID_FOR_COMMOUTER			21	// �ⲿ����ת������Ŀͻ���ID
#define CLIENTID_FOR_COMMTERM			22	// �ն�����ת������Ŀͻ���ID
#define CLIENTID_FOR_COMMBRIDGE			23	// �ⲿͨ���Žӷ���Ŀͻ���ID
#define CLIENTID_FOR_COMMCHANNEL		24	// �ڲ�ͨ����ͨ����Ŀͻ���ID
#define CLIENTID_FOR_COMMDUAL			25	// ˫��ͬ����������Ŀͻ���ID

using namespace std;
//==============================================================================
//                 
//                    CCommIdDef
//                 ͨ��ID���幤����
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
