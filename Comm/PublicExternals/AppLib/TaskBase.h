//==============================================================================
//                      TaskBase.h
//
//begin   : 2016-08-16
//describe: �����������ͷ�ļ�
//==============================================================================
#ifndef _MY_TASK_DEAL_BASE__H_20160816
#define _MY_TASK_DEAL_BASE__H_20160816

#include "TypeDef.h"
#include "Buffer.h"
#include "ProtocolForward.h"
#include "ObjectBase.h"

#include <vector>
using namespace std;

// ����������պ����ķְ��ĳ�ʱʱ�䣬��λ����
#define PKT_REV_TIMEOUT			10

//==============================================================================
//
//                   CTaskBase
//                  �����������
// һ������һ�δ���һ�����ݣ�����ְ��ˣ�ֻ�������հ�������м��б�İ����룬
// ������ʧ��
//==============================================================================
class CTaskBase : public CObjectBase
{
public:
	CTaskBase();
	virtual ~CTaskBase();

public:
	// �Ƿ��ܴ���������͵����ݰ��Ľӿڣ����麯��������������Լ�ʵ��
	virtual bool IsCanProcess(BYTE byType, BYTE byFunc) = 0;
	// �������ݰ������ܱ��ְ��ˣ����Դ����ʱ��ӡ�m_vecRcvPkts����ȡ
	virtual bool DealPkts() = 0;

	// ��ʱ����
	virtual void OnTimer();
	// ���һ�����ݰ�
	virtual void AddRecvPkt(CProtocolForward::CForwardAddr& objRecvPktSrcAddr, CBuffer* pBuf);

	// �������ݰ���Դ��ַ
	CProtocolForward::CForwardAddr GetRecvPktSrcAddr();

private:
	// ��ֹ�������
	virtual void Terminate();
	// ������buf�л�ȡ�ܰ��ź͵�ǰ����
	void GetPktCountInfo(CBuffer* pBuf, WORD &wTotal, WORD &wCur);
	// ������Ƿ�����
	bool IsPktCoherent();
	// �Ƿ��յ������һ��
	bool IsRevPktLast();
	// ��ȡ���ݰ��İ�ͷ��Ϣ

private:
	// �������ݰ���Դ��ַ��ָ��ǰ��������ݵ���Դ��Ϣ
	// ���ڷ�������Ӧ����Ϣ���ͻ�ִ��Ϣʱ���뽫�����ΪĿ�ĵ�ַ���з���
	CProtocolForward::CForwardAddr m_objRecvPktSrcAddr;
	
	// Ӧ�ò����ݷְ�ʱ�����յ���һ����ʱ��
	DWORD m_dwFirstTime;				// 
	CBuffer::CAllocator	m_objBufAlloc;	// �ڴ������

protected:
	// �յ�����Ϣ��
	vector<CBuffer*> m_vecRcvPkts;
};


//==============================================================================
//
//                   CTaskMgrBase
//                  �������������
//
//==============================================================================
class CTaskMgrBase : public CObjectBase
{
public:
	CTaskMgrBase();
	virtual ~CTaskMgrBase();
	
public:
	// �ַ�һ�����ݰ�
	virtual void Dispatch(CProtocolForward::CForwardAddr& objRecvPktSrcAddr, CBuffer* pBuf);
	// ��ʱ�������
	virtual void OnTimer();
	// ���һ�����񵽶�����
	virtual void AddTaskPtr(CTaskBase* pTask);
	// �ͷŶ���������������ڴ�ռ�
	virtual void ReleaseAllTaskMem();
	
protected:
	// ������
	vector<CTaskBase*> m_vecDeals;
};

#endif
