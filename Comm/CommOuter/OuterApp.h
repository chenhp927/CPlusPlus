//==============================================================================
//                      OuterApp.h
//                       
//begin   : 2016-09-13                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_OUTERAPP_H 
#define SELFDEF_OUTERAPP_H

#include "UnixApp.h"
#include "OuterServer.h"
#include "OuterClient.h"
#include "ConnState.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;

//==============================================================================
//                 
//                    COuterApp
//                     ��������
//
//==============================================================================
class COuterApp : public CUnixApp
{
public:
	COuterApp();
	virtual ~COuterApp();

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

	COuterClient m_objClient;					// �ͻ��ˣ�����CommInner����
	COuterServer m_objServer;					// ����ˣ�����INET�ͻ��˽���
};

#endif
