//==============================================================================
//                      TaskBase.h
//
//begin   : 2016-08-16
//describe: 处理任务基类头文件
//==============================================================================
#ifndef _MY_TASK_DEAL_BASE__H_20160816
#define _MY_TASK_DEAL_BASE__H_20160816

#include "TypeDef.h"
#include "Buffer.h"
#include "ProtocolForward.h"
#include "ObjectBase.h"

#include <vector>
using namespace std;

// 定义任务接收后续的分包的超时时间，单位：秒
#define PKT_REV_TIMEOUT			10

//==============================================================================
//
//                   CTaskBase
//                  处理任务基类
// 一个任务一次处理一个数据，如果分包了，只能连续收包，如果中间有别的包插入，
// 任务处理失败
//==============================================================================
class CTaskBase : public CObjectBase
{
public:
	CTaskBase();
	virtual ~CTaskBase();

public:
	// 是否能处理此种类型的数据包的接口，纯虚函数，派生类必须自己实现
	virtual bool IsCanProcess(BYTE byType, BYTE byFunc) = 0;
	// 处理数据包，可能被分包了，所以处理的时候从“m_vecRcvPkts”中取
	virtual bool DealPkts() = 0;

	// 定时处理
	virtual void OnTimer();
	// 添加一个数据包
	virtual void AddRecvPkt(CProtocolForward::CForwardAddr& objRecvPktSrcAddr, CBuffer* pBuf);

	// 接收数据包的源地址
	CProtocolForward::CForwardAddr GetRecvPktSrcAddr();

private:
	// 终止这个任务
	virtual void Terminate();
	// 从数据buf中获取总包号和当前包号
	void GetPktCountInfo(CBuffer* pBuf, WORD &wTotal, WORD &wCur);
	// 包序号是否连续
	bool IsPktCoherent();
	// 是否收到了最后一包
	bool IsRevPktLast();
	// 获取数据包的包头信息

private:
	// 接收数据包的源地址：指当前处理的数据的来源信息
	// 对于发送请求应答信息或发送回执信息时，须将这个作为目的地址进行发送
	CProtocolForward::CForwardAddr m_objRecvPktSrcAddr;
	
	// 应用层数据分包时，接收到第一包的时间
	DWORD m_dwFirstTime;				// 
	CBuffer::CAllocator	m_objBufAlloc;	// 内存管理器

protected:
	// 收到的信息包
	vector<CBuffer*> m_vecRcvPkts;
};


//==============================================================================
//
//                   CTaskMgrBase
//                  处理任务管理类
//
//==============================================================================
class CTaskMgrBase : public CObjectBase
{
public:
	CTaskMgrBase();
	virtual ~CTaskMgrBase();
	
public:
	// 分发一个数据包
	virtual void Dispatch(CProtocolForward::CForwardAddr& objRecvPktSrcAddr, CBuffer* pBuf);
	// 定时检查任务
	virtual void OnTimer();
	// 添加一个任务到队列中
	virtual void AddTaskPtr(CTaskBase* pTask);
	// 释放队列中所有任务的内存空间
	virtual void ReleaseAllTaskMem();
	
protected:
	// 子任务
	vector<CTaskBase*> m_vecDeals;
};

#endif
