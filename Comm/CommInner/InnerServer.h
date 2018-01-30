//==============================================================================
//                      InnerServer.h
//                       
//begin   : 2016-09-10                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_INNERSERVER_H 
#define SELFDEF_INNERSERVER_H 
 
#include "ServerForwardBase.h"
#include "ConnState.h"
 
//==============================================================================
//                 
//                    CInnerServer
//                    CommInner�������
//
//==============================================================================
class CInnerServerCltObj;
class CInnerServer : public CServerForwardBase
{ 
public: 
	CInnerServer();
	virtual ~CInnerServer(); 
 
protected: 
	/* ���е�ʵ��Ϊ��������ַ��ʼ������ش��� */
	// ���ػ��ຯ��
	virtual string GetServerIdValidRangeStr();
	
	// ���ػ��ຯ��
	virtual bool IsServerIdValid();
	
	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ���ػ��ຯ��
	// ������յ��û����ݰ���
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	virtual void HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf);
	
	// ����ת������
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	void ForwardDataToMyOtherClients(CInnerServerCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
	// �ж����ݰ���Ŀ�ĵ�ַ��ͻ����Ƿ����
	bool IsDataDestAddrMatchClt(CProtocolForward::CForwardHead& objHead, CInnerServerCltObj* pCltObj);

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ����ԭʼ���ݰ���
	// �����buffer�����ݰ��������������������Ѱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ���뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pCltObj��Ϊ���������
	bool SendRawData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);

	/* ���е�ʵ��Ϊ����ͻ��˶������ش��� */
	// ���ػ��ຯ��
	virtual CServerBaseCltObj* CreateClientObject(); 

	// ���ػ��ຯ��
	virtual string GetCliendIdValidRangeStr();

	// ���ػ��ຯ��
	virtual bool IsClientIdValid(CServerSessionBaseCltObj* pCltObj);

	// ���ػ��ຯ��
	virtual bool IsClientIpValid(struct sockaddr_in& stuSockAddr);  

	// ���ػ��ຯ��
	virtual bool IsTooMuchSameClient(struct sockaddr_in& stuSockAddr);  
	
	// ���ػ��ຯ��
	virtual void OnSessionReady(CSid objSid); 
	
	// ���ػ��ຯ��
	virtual void OnSessionClose(CSid objSid); 
public: 
	/* ���еĽӿ�Ϊ������ʼ������ش��� */
	// ��ʼ����������ص�������Ϣ��
	// strConfigFile��	�����ļ����ƣ�����·��
	// strService	��	��������
	bool Init(string strConfigFile, string strService);
	
	//��ȡ���ӵ�Sid����
	list<CSid>& GetSid();
	
private: 
	string m_strService;		// ��������
	list<CSid>  m_lstSid;       // ���ӵ�Sid����

}; 

//==============================================================================
//                 
//                    CInnerServerCltObj
//                CommInner�������Ŀͻ��˶�����
//
//==============================================================================
class CInnerServerCltObj : public CServerForwardBaseCltObj
{ 
public: 
	CInnerServerCltObj(CInnerServer* pServer); 
	virtual ~CInnerServerCltObj(); 
};
 
#endif 
