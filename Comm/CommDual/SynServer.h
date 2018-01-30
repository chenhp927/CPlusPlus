//==============================================================================
//                      SynServer.h
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_SYNSERVER_H 
#define SELFDEF_SYNSERVER_H 
 
#include "NetServer.h"
 
//==============================================================================
//                 
//                    SynServer
//                   CommDualͬ���������
//
//==============================================================================
class CSynServer : public CNetServerDouble
{ 
public: 
	CSynServer();
	virtual ~CSynServer(); 
 
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
	void SendDataToLocalServer(CServerOneNetCltObj* pCltObj, CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
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

	// ����ӱ������ӽ��յ����ݰ�
	void OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);

private: 
	string m_strService;		// ��������
}; 


 
#endif 
