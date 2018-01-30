//==============================================================================
//                      ServerForwardBase.h
//
//begin   : 2016-09-08
//describe:
//==============================================================================
#ifndef SELFDEF_SERVERFORWARDBASE_H 
#define SELFDEF_SERVERFORWARDBASE_H 
 
#include <vector>

#include "ServerSessionBase.h" 
#include "ProtocolForward.h"
#include "ForwardCondition.h"
 
using namespace std;
//==============================================================================
//                 
//                    CServerForwardBase
//           ת����ͻ��˻��ࣨ��˫����ת���������ƣ�
//
//==============================================================================
class CServerForwardBaseCltObj;
class MY_EXT_CLASS CServerForwardBase : public CServerSessionBase
{ 
public: 
	CServerForwardBase(); 
	virtual ~CServerForwardBase(); 
 
protected: 
	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// Ϊҵ�����˷��͵���������Դ��ַ
	bool SetSrcAddrForSendData(char* szBuf, int nBufLen);

	// Ϊҵ�����˷��͵��������������롢������
	bool SetTypeFuncForSendData(char* szBuf, int nBufLen);
	bool CheckSendData(char* szBuf, int nBufLen);
	
	/* ���е�ʵ��Ϊ����ͻ��˶������ش��� */
	// ���ػ��ຯ��
	virtual CServerBaseCltObj* CreateClientObject();
	
public:  
	/* ���еĽӿ�Ϊ���������ʼ������ش��� */
	// ��ʼ�����������cNetNo���������ţ�1�����һ������2����ڶ�����
	bool InitCommParam(TiXmlHandle& txhServer, char cNetNo = 1); 

	/* ���еĽӿ�Ϊ��ȡ�������ݰ�����ش��� */
	// ��ȡ���յ���ת�������ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���Ĳ�ȥ���˻Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ���
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���봫��������
	// ���нӿ��õ���szCltIP��nCltPort��objSidΪ��������
	// ע��Ҫ��ȡֻ����ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�RecvSessionData()���ӿ�
	// ע��Ҫ��ȡ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�RecvData()���ӿ�
	virtual bool RecvForwardData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort);
	virtual bool RecvForwardData(CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort); 
	virtual bool RecvForwardData(char* szBuf, int& nBufLen, CSid& objSid); 
	virtual bool RecvForwardData(CBuffer* pBuf, CSid& objSid); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen, CSid& objSid); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, CSid& objSid);

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ����ת�������ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��ת����Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��szCltIP��nCltPort��objSid��pCltObj��objForwardHead��Ϊ���������
	// ע��Ҫ����ֻ����ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�SendSessionData()���ӿ�
	// ע��Ҫ���Ͱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�SendData()���ӿ�
	virtual bool SendForwardData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);
	virtual bool SendForwardData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(char* szBuf, int nBufLen, CSid objSid);
	virtual bool SendForwardData(CBuffer* pBuf, CSid objSid);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen, CSid objSid);
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf, CSid objSid);
}; 

//==============================================================================
//                 
//                    CServerForwardBaseCltObj
//           ת����ͻ��˻���Ŀͻ��˶����ࣨ��˫����ת���������ƣ�
//
//==============================================================================
class MY_EXT_CLASS CServerForwardBaseCltObj : public CServerSessionBaseCltObj
{ 
public: 
	CServerForwardBaseCltObj(CServerForwardBase* pServer); 
	virtual ~CServerForwardBaseCltObj(); 
 
public: 
	/* ���еĽӿ�Ϊע����Ϣ����ش��� */
	// ����ע�����
	// �ú��������buffer�����ݰ����������Ự��Э��ͷ��Ϣ
	bool HandleRegisterPkt(char* szBuf, int nBufLen);

	bool IsRegistered(CProtocolForward::CForwardHead& objForwardHead);
	void ClearRegisterInfo();
	CForwardCondition& GetForwardCondition();
	
protected: 
	CForwardCondition	m_objForwardCondition;		// �ͻ���ע��󻺴���ת������
}; 
   
#endif 
