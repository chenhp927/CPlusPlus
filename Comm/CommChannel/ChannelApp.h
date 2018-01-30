//==============================================================================
//                      ChannelApp.h
//                       
//begin   : 2016-09-18                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_COMMCHANNELAPP_H 
#define SELFDEF_COMMCHANNELAPP_H

#include "UnixApp.h"
#include "ChannelClient.h"

//==============================================================================
//                 
//                    CChannelApp
//                     ��������
//
//==============================================================================
class CChannelApp : public CUnixApp
{
public:
	CChannelApp();
	virtual ~CChannelApp();

protected:
	// ��ȡ����汾����Ϣ������Ĵ��麯��
	virtual string GetAppVersion();
	
public:
	// �����ʼ������̬
	virtual bool Init();

	// ������Ҫ���߼�ʵ����ڡ���̬
	virtual void Run();

private:
	// ��ʼ����־�������
	void InitLogFile();

	// ��ʼ������ͨ�ŵķ���
	bool InitComm();

	// ��ʱ���������¼�
	void OnTimerCheckNetEvent();

	// ����ͻ��������¼�
	void HandleLeftClientEvent();
	void HandleRightClientEvent();

	// ���ݰ�ת������
	void ForwardDataFromLeftToRight(CBuffer* pBuf);
	void ForwardDataFromRightToLeft(CBuffer* pBuf);

private:
	CFdset m_objFdset;							// ͨ�ſ���
	CBuffer::CAllocator	m_objBufAlloc;			// �ڴ������

	CChannelClient	m_objLeftClient;			// �ͻ��ˣ�����CommInner����
	CChannelClient	m_objRightClient;			// �ͻ��ˣ�����CommInner����
};

#endif
