//==============================================================================
//                      InnerApp.h
//                       
//begin   : 2016-09-10                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_INNERAPP_H 
#define SELFDEF_INNERAPP_H

#include "UnixApp.h"
#include "InnerServer.h"
#include "ProtocolMaintenComm.h"

using namespace ProtocolMaintenContent;
//==============================================================================
//                 
//                    CInnerApp
//                     ��������
//
//==============================================================================
class CInnerApp : public CUnixApp
{
public:
	CInnerApp();
	virtual ~CInnerApp();

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

	// �������������¼�
	void HandleServerEvent();
	
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

	CInnerServer m_objServer;					// �������ӷ����
};

#endif
