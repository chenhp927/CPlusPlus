//==============================================================================
//                      TermApp.h
//                       
//begin   : 2016-09-18                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_TERMAPP_H 
#define SELFDEF_TERMAPP_H

#include "UnixApp.h"
#include "TermServer.h"
#include "TermClient.h"
#include "ConnState.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;

//==============================================================================
//                 
//                    CTermApp
//                     ��������
//
//==============================================================================
class CTermApp : public CUnixApp
{
public:
	CTermApp();
	virtual ~CTermApp();

protected:
	// ��ȡ����汾����Ϣ������Ĵ��麯��
	virtual string GetAppVersion();
	
public:
	// �����ʼ������̬
	virtual bool Init();

	// ������Ҫ���߼�ʵ����ڡ���̬
	virtual void Run();

	// ��CommInner���̷������ݰ�
	void SendDataToCommInner(CBuffer* pBuf);

private:
	// ��ʼ����־�������
	void InitLogFile();

	// ��ʼ������ͨ�ŵķ���
	bool InitComm();

	// ��ʱ���������¼�
	void OnTimerCheckNetEvent();

	// ����ͻ��������¼�
	void HandleClientEvent();

	// �������������¼�
	void HandleServerEvent();

	// ��ʱ������
	void OnTimer();
	
	//���
	bool MakeAllCommStatePkt(CBuffer* pBuf);
	
	//��ӿͻ�������״̬
	void AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState);
	
private:
	CFdset m_objFdset;							// ͨ�ſ���
	CBuffer::CAllocator	m_objBufAlloc;			// �ڴ������

	CTermClient	m_objClient;					// �ͻ��ˣ�����CommInner����
	CTermServer	m_objServer;					// ����ˣ�����INET�ͻ��˽���
};

#endif
