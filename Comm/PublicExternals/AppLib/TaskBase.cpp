//==============================================================================
//                      TaskBase.cpp
//
//begin   : 2016-08-16
//describe: �����������ͷ�ļ�
//==============================================================================
#include "TaskBase.h"
#include "Utility.h"
#include "ProtocolHead.h"

using namespace ProtocolBase;

//==============================================================================
//
//                   CTaskBase
//                  �����������
//
//==============================================================================
CTaskBase::CTaskBase()
{
	Terminate();
}

CTaskBase::~CTaskBase()
{
	Terminate();
}

void CTaskBase::AddRecvPkt(CProtocolForward::CForwardAddr& objRecvPktSrcAddr, CBuffer* pBuf)
{
	// �ȸ�������һ���ռ�
	CBuffer *pData = m_objBufAlloc.Allocate();
	int nRet = pData->AddData(pBuf);
	assert(nRet > 0);

	// �����û�а�����ֱ����ӽ�ȥ
	if (m_vecRcvPkts.size() == 0)
	{
		m_dwFirstTime = CUtility::GetUptime();
		m_objRecvPktSrcAddr = objRecvPktSrcAddr;
		m_vecRcvPkts.push_back(pData);
	}
	// ����Ѿ����ˣ������жϺϷ��ԣ������
	else
	{
		if (m_objRecvPktSrcAddr == objRecvPktSrcAddr)	// �Ϸ�
		{
			m_vecRcvPkts.push_back(pData);
		}
		else // ���Ϸ�
		{
			LogImportant("dealtask rev CurPkt is not match with PrePkt!");
			Terminate();
			return;
		}
	}

	// �ж��������û�У���������ˣ�����ô���ӿڣ�û����������ȴ�����
	if (IsRevPktLast())
	{
		// �жϰ����Ƿ������������ʹ�����
		if (IsPktCoherent())
		{
			DealPkts();
		}

		// ֻҪ�յ����һ���˶���������Ĵ���
		Terminate();
	}
}

CProtocolForward::CForwardAddr CTaskBase::GetRecvPktSrcAddr()
{
	return m_objRecvPktSrcAddr;
}

void CTaskBase::GetPktCountInfo(CBuffer* pBuf, WORD &wTotal, WORD &wCur)
{
	wTotal = wCur = 0;
	CProtocolHead pktHead;
	if(pktHead.FromStreamBuf(pBuf->GetBuffer(), pBuf->GetUsed()) < 0)
	{
		return ;
	}
	
	wTotal = pktHead.GetPktCount();
	wCur = pktHead.GetCurPkt();
}

bool CTaskBase::IsPktCoherent()
{
	// ���ֻ��һ�����ݰ���һ���������ġ��󲿷������Ӧ����һ������
	int nPktCount = m_vecRcvPkts.size();
	if (nPktCount == 1)
	{
		return true;
	}

	// �����ֹһ������ȡ��һ��
	WORD wTotalFirst, wCurFirst;
	GetPktCountInfo(m_vecRcvPkts[0], wTotalFirst, wCurFirst);

	//��ֹһ������ȡ���������Ƚ�
	WORD wTotalTmp, wCurTmp;
	for (int i=1; i<nPktCount; i++)
	{
		GetPktCountInfo(m_vecRcvPkts[i], wTotalTmp, wCurTmp);

		wCurFirst++;
		if (wTotalTmp!=wTotalFirst || wCurTmp!=wCurFirst)
		{
			return false;
		}
	}
	
	return true;
}

bool CTaskBase::IsRevPktLast()
{
	int nPktCount = m_vecRcvPkts.size();
	if (nPktCount < 1)
	{
		return false;
	}

	// ȡ���һ���İ�����Ϣ
	WORD wTotal, wCur;
	GetPktCountInfo(m_vecRcvPkts[nPktCount-1], wTotal, wCur);

	return (wTotal==wCur);
}

void CTaskBase::Terminate()
{
	// �������ʱ��
	m_dwFirstTime = 0;

	// ����������
	for(int i=0; i<m_vecRcvPkts.size(); i++)
	{
		m_vecRcvPkts[i]->Release();
	}
	m_vecRcvPkts.clear();
}

void CTaskBase::OnTimer()
{
	// �������ȴ������İ���ʱ�ˣ�����ֹ����
	if (m_dwFirstTime!=0 && CUtility::GetElapseTime(m_dwFirstTime) > PKT_REV_TIMEOUT)
	{
		Terminate();
	}
}


//==============================================================================
//
//                   CTaskMgrBase
//                  ��������������
//
//==============================================================================
CTaskMgrBase::CTaskMgrBase()
{
}

CTaskMgrBase::~CTaskMgrBase()
{
}

void CTaskMgrBase::Dispatch(CProtocolForward::CForwardAddr& objRecvPktSrcAddr, CBuffer* pBuf)
{
	assert(pBuf);

	// ===========ȡ�յ���Ϣ�е�Դ��ַ+Ӧ�ò�����===================================================
	// �������ݰ���Դ��ַ��ָ��ǰ��������ݵ���Դ��Ϣ, ���ڷ�������Ӧ����Ϣ���ͻ�ִ��Ϣʱ���뽫�����ΪĿ�ĵ�ַ���з���
	// Ӧ�ò����ݣ����������롢���������Ϣ�����ݴ���ʹ�á�
	
	// �ж����ݰ��Ƿ����
	CProtocolHead pktHead;
	if(pktHead.FromStreamBuf(pBuf->GetBuffer(), pBuf->GetUsed()) < 0)
	{
		LogImportant("recv data 418 head error!");
		return ;
	}
	
	// ȡ�����롢������
	BYTE byType = pktHead.GetType();
	BYTE byFunc = pktHead.GetFunc();
	
	// �ַ����ݰ�
	bool bSuc = false;
	int nPktCount = m_vecDeals.size();
	for (int i=0; i<nPktCount; i++)
	{
		if (m_vecDeals[i]->IsCanProcess(byType, byFunc))
		{
			m_vecDeals[i]->AddRecvPkt(objRecvPktSrcAddr, pBuf);
			bSuc = true;
			break;
		}
	}
	
	// ʧ�ܵĴ���
	if (!bSuc)
	{
		LogImportant("�յ�418��������0x%x��������=0x%x�޷�����!!", byType, byFunc);
	}
}

void CTaskMgrBase::OnTimer()
{
	// ��ʱ�������е�����
	int nPktCount = m_vecDeals.size();
	for (int i=0; i<nPktCount; i++)
	{
		m_vecDeals[i]->OnTimer();
	}
}

void CTaskMgrBase::AddTaskPtr(CTaskBase* pTask)
{
	m_vecDeals.push_back(pTask);
}

void CTaskMgrBase::ReleaseAllTaskMem()
{
	// �������е������ͷſռ�
	int nPktCount = m_vecDeals.size();
	for (int i=0; i<nPktCount; i++)
	{
		delete m_vecDeals[i];
	}

	m_vecDeals.clear();
}
