//==============================================================================
//                      BridgeApp.h
//                       
//begin   : 2016-09-18                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_BRIDGEAPP_H 
#define SELFDEF_BRIDGEAPP_H

#include "UnixApp.h"
#include "LocalClient.h"
#include "RemoteClient.h"
#include "ConnState.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;

//==============================================================================
//                 
//                    CBridgeApp
//                     ��������
//
//==============================================================================
class CBridgeApp : public CUnixApp
{
public:
	CBridgeApp();
	virtual ~CBridgeApp();

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
	void HandleLocalClientEvent();
	void HandleRemoteClientEvent();

	// ���ݰ�ת������
	void ForwardDataFromLocalToRemote(CBuffer* pBuf);
	void ForwardDataFromRemoteToLocal(CBuffer* pBuf);

	// ��ʱ������
	void OnTimer();
	
	//���
	bool MakeAllCommStatePkt(CBuffer* pBuf);
	
	//��ӿͻ�������״̬
	void AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState);
	
private:
	CFdset m_objFdset;					// ͨ�ſ���
	CBuffer::CAllocator	m_objBufAlloc;	// �ڴ������

	CLocalClient	m_objLocalClient;	// �ͻ��ˣ����ӱ���CommInner����
	CRemoteClient	m_objRemoteClient;	// �ͻ��ˣ�����Զ��CommOuter����
};

#endif
