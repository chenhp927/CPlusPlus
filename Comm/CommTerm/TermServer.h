//==============================================================================
//                      TermServer.h
//                       
//begin   : 2016-09-18                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_TERMSERVER_H 
#define SELFDEF_TERMSERVER_H 
 
#include "NetServer.h"
 
//==============================================================================
//                 
//                    CTermServer
//                   CommTerm�������
//
//==============================================================================
class CTermServer : public CNetServerDouble
{ 
public: 
	CTermServer();
	virtual ~CTermServer(); 
 
protected: 
	/* ���е�ʵ��Ϊ��������ַ��ʼ������ش��� */
	// ���ػ��ຯ��
	virtual string GetServerIdValidRangeStr(bool bIsAppSvr);
	
	// ���ػ��ຯ��
	virtual bool IsServerIdValid(bool bIsAppSvr, WORD wSvrId);
	
	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ���ػ��ຯ��
	// �����ĳ���ͻ��ˣ����뱾�����������յ����ݰ���
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	virtual void OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	
	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ��<�ڲ�����ת������>ģ�鷢�����ݰ�
	// �ú��������buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	void SendDataToCommInner(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
	// �����յ����ݰ����ҵĿͻ���ת����
	// �ú��������buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	void ForwardDataToMyClients(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
	// �����յ����ݰ����ҵ������ͻ���ת����
	// �ú��������buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	void ForwardDataToMyOtherClients(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
	// �ж����ݰ���Ŀ�ĵ�ַ��ͻ����Ƿ����
	bool IsDataDestAddrMatchClt(CProtocolForward::CForwardHead& objHead, CNetServerDoubleCltObj* pDoubleCltObj);
	
	/* ���е�ʵ��Ϊ����ͻ��˶������ش��� */
	virtual string GetCliendIdValidRangeStr();
	virtual bool IsClientIdValid(bool bIsAppClt, WORD wCltId);

public: 
	/* ���еĽӿ�Ϊ������ʼ������ش��� */
	// ��ʼ����������ص�������Ϣ��
	// strConfigFile��	�����ļ����ƣ�����·��
	// strService	��	��������
	bool Init(string strConfigFile, string strService);

	/* ���еĽӿ�Ϊ���������������رա��ж�����ش��� */
	// ���ػ��ຯ��
	bool Startup();

	// ���ػ��ຯ��
	virtual void OnSessionConnect(CServerOneNetCltObj* pCltObj); 

	// ���ػ��ຯ��
	virtual void OnSessionReady(CServerOneNetCltObj* pCltObj); 

	// ���ػ��ຯ��
	virtual void OnSessionClose(CServerOneNetCltObj* pCltObj); 

	// �����<�ڲ�����ת������>ģ����յ����ݰ�
	// �ú��������buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	void OnRecvDataFromCommInner(CBuffer* pBuf);
	
private: 
	string m_strService;		// ��������
}; 


 
#endif 
