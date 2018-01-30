//==============================================================================
//                      TaskBase.cpp
//
//begin   : 2016-08-16
//describe: 处理任务基类头文件
//==============================================================================
#include "TaskBase.h"
#include "Utility.h"
#include "ProtocolHead.h"

using namespace ProtocolBase;

//==============================================================================
//
//                   CTaskBase
//                  处理任务基类
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
	// 先给包分配一个空间
	CBuffer *pData = m_objBufAlloc.Allocate();
	int nRet = pData->AddData(pBuf);
	assert(nRet > 0);

	// 如果还没有包，则直接添加进去
	if (m_vecRcvPkts.size() == 0)
	{
		m_dwFirstTime = CUtility::GetUptime();
		m_objRecvPktSrcAddr = objRecvPktSrcAddr;
		m_vecRcvPkts.push_back(pData);
	}
	// 如果已经有了，则先判断合法性，后添加
	else
	{
		if (m_objRecvPktSrcAddr == objRecvPktSrcAddr)	// 合法
		{
			m_vecRcvPkts.push_back(pData);
		}
		else // 不合法
		{
			LogImportant("dealtask rev CurPkt is not match with PrePkt!");
			Terminate();
			return;
		}
	}

	// 判断收完包了没有，如果收完了，则调用处理接口；没收完则继续等待处理
	if (IsRevPktLast())
	{
		// 判断包号是否连续，连续就处理了
		if (IsPktCoherent())
		{
			DealPkts();
		}

		// 只要收到最后一包了都结束任务的处理
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
	// 如果只有一个数据包则一定是连续的。大部分情况下应该是一个包。
	int nPktCount = m_vecRcvPkts.size();
	if (nPktCount == 1)
	{
		return true;
	}

	// 如果不止一个则先取第一个
	WORD wTotalFirst, wCurFirst;
	GetPktCountInfo(m_vecRcvPkts[0], wTotalFirst, wCurFirst);

	//不止一个，再取后面的逐个比较
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

	// 取最后一包的包号信息
	WORD wTotal, wCur;
	GetPktCountInfo(m_vecRcvPkts[nPktCount-1], wTotal, wCur);

	return (wTotal==wCur);
}

void CTaskBase::Terminate()
{
	// 清除接收时间
	m_dwFirstTime = 0;

	// 清除缓冲队列
	for(int i=0; i<m_vecRcvPkts.size(); i++)
	{
		m_vecRcvPkts[i]->Release();
	}
	m_vecRcvPkts.clear();
}

void CTaskBase::OnTimer()
{
	// 如果任务等待后续的包超时了，则终止任务
	if (m_dwFirstTime!=0 && CUtility::GetElapseTime(m_dwFirstTime) > PKT_REV_TIMEOUT)
	{
		Terminate();
	}
}


//==============================================================================
//
//                   CTaskMgrBase
//                  处理任务管理基类
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

	// ===========取收到信息中的源地址+应用层数据===================================================
	// 接收数据包的源地址：指当前处理的数据的来源信息, 对于发送请求应答信息或发送回执信息时，须将这个作为目的地址进行发送
	// 应用层数据：包含功能码、类型码等信息，数据处理使用。
	
	// 判断数据包是否错误
	CProtocolHead pktHead;
	if(pktHead.FromStreamBuf(pBuf->GetBuffer(), pBuf->GetUsed()) < 0)
	{
		LogImportant("recv data 418 head error!");
		return ;
	}
	
	// 取类型码、功能码
	BYTE byType = pktHead.GetType();
	BYTE byFunc = pktHead.GetFunc();
	
	// 分发数据包
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
	
	// 失败的处理
	if (!bSuc)
	{
		LogImportant("收到418包类型码0x%x，功能码=0x%x无法处理!!", byType, byFunc);
	}
}

void CTaskMgrBase::OnTimer()
{
	// 定时遍历所有的任务
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
	// 遍历所有的任务，释放空间
	int nPktCount = m_vecDeals.size();
	for (int i=0; i<nPktCount; i++)
	{
		delete m_vecDeals[i];
	}

	m_vecDeals.clear();
}
