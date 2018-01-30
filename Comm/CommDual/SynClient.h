//==============================================================================
//                      SynClient.h
//
//begin   : 2016-10-20
//describe:
//==============================================================================
#ifndef SELFDEF_SYNCLIENT_H
#define SELFDEF_SYNCLIENT_H

#include "NetClient.h"

//==============================================================================
//                 
//                    CSynClient
//                   CommDualͬ���ͻ�����
//
//==============================================================================
class CSynClient : public CNetClientDouble
{
public:
	CSynClient();
	virtual ~CSynClient();

private:
	/* ���еĽӿ�Ϊ���������������رա��ж�����ش��� */
	// ���ػ��ຯ��
	void OnConnect(CClientOneNet* pClient); 

	// ���ػ��ຯ��
	void OnClose(CClientOneNet* pClient); 

	// ���ػ��ຯ��
	void OnSessionReady(CClientOneNet* pClient); 

	// ���ػ��ຯ��
	void OnSessionClose(CClientOneNet* pClient); 

public:
	/* ���еĽӿ�Ϊ������ʼ������ش��� */
	// ��ʼ���ͻ�����ص�������Ϣ��
	// strConfigFile��	�����ļ����ƣ�����·��
	// strService	��	��������
	bool Init(string strConfigFile, string strService);

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ���ػ��ຯ��
	// ����ԭʼ���ݰ���
	// �����buffer�����ݰ��������������������Ѱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ���뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ��������ݰ����ᱻ�Զ�����˫����Ϣ��
	// ���нӿ��õ���pBuf��Ϊ���������
	bool SendRawData(CBuffer* pBuf); 

	// ����ӱ������ӽ��յ����ݰ�
	void OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);

private:
	string m_strService;		// ��������
};

#endif
