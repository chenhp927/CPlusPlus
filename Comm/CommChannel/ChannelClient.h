//==============================================================================
//                      ChannelClient.h
//
//begin   : 2016-09-18
//describe:
//==============================================================================
#ifndef SELFDEF_CHANNELCLIENT_H
#define SELFDEF_CHANNELCLIENT_H

#include "NetClient.h"

//==============================================================================
//                 
//                    CChannelClient
//                   CommChannel�ͻ�����
//
//==============================================================================
class CChannelClient : public CNetClientSingle
{
public:
	CChannelClient();
	virtual ~CChannelClient();

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
	bool Init(string strConfigFile, string strService, bool bLeftClient = true);

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ���ػ��ຯ��
	// ����ԭʼ���ݰ���
	// �����buffer�����ݰ��������������������Ѱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ���뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ��������ݰ����ᱻ�Զ�����˫����Ϣ��
	// ���нӿ��õ���pBuf��Ϊ���������
	bool SendRawData(CBuffer* pBuf); 

private:
	string m_strService;		// ��������
};

#endif
