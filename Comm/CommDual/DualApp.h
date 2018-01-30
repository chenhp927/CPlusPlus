//==============================================================================
//                      DualApp.h
//                       
//begin   : 2016-10-20                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_DUALAPP_H 
#define SELFDEF_DUALAPP_H

#include "UnixApp.h"
#include "LocalServer.h"
#include "SynServer.h"
#include "SynClient.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;
//==============================================================================
//                 
//                    CDualApp
//                     ��������
//
//==============================================================================
class CDualApp : public CUnixApp
{
public:
	CDualApp();
	virtual ~CDualApp();

protected:
	// ��ȡ����汾����Ϣ������Ĵ��麯��
	virtual string GetAppVersion();
	
public:
	// �����ʼ������̬
	virtual bool Init();

	// ������Ҫ���߼�ʵ����ڡ���̬
	virtual void Run();

	// �����ͬ�����ӽ��յ����ݰ�
	void OnRecvDataFromSynClient(CBuffer* pBuf);
	void OnRecvDataFromSynConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);

	// ����ӱ������ӽ��յ����ݰ�
	void OnRecvDataFromLocalConn(CBuffer* pBuf, CProtocolForward::CForwardHead& objHead);
	
private:
	// ��ʼ����־�������
	void InitLogFile();

	// ��ʼ������ͨ�ŵķ���
	bool InitComm();

	// ��ʱ���������¼�
	void OnTimerCheckNetEvent();

	// �����ط���������¼�
	void HandleLocalServerEvent();
	
	// ����ͬ������������¼�
	void HandleSynServerEvent();

	// ����ͬ���ͻ��������¼�
	void HandleSynClientEvent();

	// ��ʱ������
	void OnTimer();
	
	//��������״̬��
	void SendConnStatePkt(CBuffer* pBuf);
	
	//���
	bool MakeAllCommStatePkt(CBuffer* pBuf);
	
	//��ӿͻ�������״̬
	void AddClientConnState(CClientConnMainCommtoSys& objCltConnMainCommtoSys, const CCltConnState& objCltConnState);
	
private:
	CFdset m_objFdset;							// ͨ�ſ���
	CBuffer::CAllocator	m_objBufAlloc;			// �ڴ������

	CLocalServer	m_objLocalServer;			// ���ط���ˣ����뱾��INETҵ��ͻ���
	CSynServer		m_objSynServer;				// ͬ������ˣ�����ԵȻ�����ͬ���ͻ���
	CSynClient		m_objSynClient;				// ͬ���ͻ��ˣ����ӶԵȻ�����ͬ�������
};

#endif
