//================================================================================================
//	
//									ProtocolAlarmInfo.h
//							   
//begin	  : 2016-09-11
//describe: 协议报警事件类
//
//================================================================================================
#ifndef _PROTOCOL_ALARMINFO_H_
#define _PROTOCOL_ALARMINFO_H_

#include "ProtocolHead.h"
#include "ProtocolTime.h"
#include "ProtocolBase.h"
#include "ObjectBase.h"
#include "MyUuid.h"
#include "TypeDef.h"
#include <vector>

namespace ProtocolAlarm
{

	using namespace ProtocolBase;

	//=============================================================================================
	//
	//	CAlarmAllEvt
	//	报警全体信息报警事件
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmAllEvt : public CObjectBase
	{
	public:
		CAlarmAllEvt();
		virtual ~CAlarmAllEvt();

	public:
		void GenerateAlarmid();								// 产生Uuid（报警事件Id）

		void SetAlarmId(CMyUuid& AlarmId);					// 设置报警事件ID
		void SetSerialId(BYTE bySerialId);					// 设置报警事件序号
		void SetAlarmType(WORD wAlarmType);					// 设置报警事件类型
		void SetTime(time_t tm, WORD wMs);					// 设置发生时间，wMs：毫秒（0~999）
		void SetTime(timeval t);
		void SetStaId(WORD wStaId);							// 设置车站ID				
		void SetStaName(string strStaName);					// 设置车站名称
		void SetDevId(DWORD dwSet);							// 设置报警所属装置ID
		void SetObjType(BYTE byObjType);					// 设置对象类型
		void SetObjId(DWORD dwObjId);						// 设置对象ID
		void SetObjName(string strObjName);					// 设置对象名称
		void SetObjVolLevel(BYTE bySet);					// 设置对象的电压等级
		void SetDesc(string strDesc);						// 设置描述
		void SetOperId(DWORD dwOperId);						// 设置操作员ID
		void SetOperName(string strOperName);				// 设置操作员名称
		void SetDispatcherId(WORD wDisp);					// 设置调度台Id
		void SetDispatcherName(string strDisp);				// 设置调度台名称
		void SetCommitFlag(BYTE byCommit);					// 设置确认标识
		void SetCommitTime(time_t t, WORD wMs);				// 设置确认时间
		void SetRelieveFlag(BYTE byRelieve);				// 设置解除标识	

		CMyUuid GetAlarmId() const;							// 获取报警事件ID
		BYTE GetSerialId() const;							// 获取报警事件序号
		WORD GetAlarmType() const;							// 获取报警事件类型
		CProtocolTime GetProtoclTime() const;				// 获取发生时间，返回ProtocolTime对象
		timeval GetTime() const;							// 获取发生时间，返回timeval对象
		WORD GetStaId() const;								// 获取车站ID				
		string GetStaName() const;							// 获取车站名称
		DWORD GetDevId() const;								// 获取报警所属装置ID
		BYTE GetObjType() const;							// 获取对象类型
		DWORD GetObjId() const;								// 获取对象ID
		string GetObjName() const;							// 获取对象名称
		BYTE GetObjVolLevel() const;						// 获取对象的电压等级
		string GetDesc() const;								// 获取描述
		DWORD GetOperId() const;							// 获取操作员ID
		string GetOperName() const;							// 获取操作员名称
		WORD GetDispatcherId() const;						// 获取调度台Id
		string GetDispatcherName() const;					// 获取调度台名称
		BYTE GetCommitFlag() const;							// 获取确认标识
		CProtocolTime GetProCommitTime() const;				// 获取确认时间
		timeval GetCommitTime() const;						// 获取确认时间
		BYTE GetRelieveFlag() const;						// 获取解除标识	

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		CMyUuid m_AlarmId;									// 报警事件ID，报警事件唯一标识
		BYTE m_bySerialId;									// 报警事件序号
		WORD m_wAlarmType;									// 报警事件类型
		CProtocolTime m_Time;								// 发生时间
		WORD m_wStaId;										// 车站ID
		string m_strStaName;								// 车站名称
		DWORD m_dwDevId;									// 报警所属的装置ID
		BYTE m_byObjType;									// 对象类型
		DWORD m_dwObjId;									// 对象ID
		string m_strObjName;								// 对象名称
		BYTE m_byObjVolLevel;								// 对象的电压等级
		string m_strDesc;									// 描述
		DWORD m_dwOperId;									// 操作员ID
		string m_strOperName;								// 操作员名称
		WORD m_wDispatcherId;								// 调度台Id
		string m_strDisptcherName;							// 调度台名称
		BYTE m_byCommit;									// 确认标识
		CProtocolTime m_CommitTime;							// 确认时间
		BYTE m_byRelieve;									// 解除标识
	};

	//=============================================================================================
	//
	//	CAlarmEvtGeneratePkt
	//	报警事件生成包【其他模块->AppAlarmMgr】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmEvtGeneratePkt : public CProtocolHead
	{
	public:
		CAlarmEvtGeneratePkt();
		virtual ~CAlarmEvtGeneratePkt();

	public:
		void SetDestDispatchId(WORD wSet);					// 设置目标调度台ID
		WORD GetDestDispatchId();							// 获取目标调度台ID

		void Add(CAlarmAllEvt& AlarmEvent);					// 添加一条报警事件
		DWORD GetAlarmEvtCount() const;						// 获取报警事件数量
		CAlarmAllEvt* GetAlarmEvt(int nIndex);				// 获取指定下标的报警事件

		void ClearAllAlarm();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wDestDispatchId;								// 目标调度台ID
		vector<CAlarmAllEvt> m_vecAlarmEvt;					// 报警事件
	};

	//=============================================================================================
	//
	//	CAlarmAllPkt
	//	报警事件全体信息包【AppAlarmMgr->操作台】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmAllPkt : public CAlarmEvtGeneratePkt
	{
	public:
		CAlarmAllPkt();
	    virtual ~CAlarmAllPkt();
	};

	//=============================================================================================
	//
	//	CAlarmChangePkt
	//	报警事件变化信息包【AppAlarmMgr->操作台】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmChangePkt : public CAlarmEvtGeneratePkt
	{
	public:
		CAlarmChangePkt();
	    virtual ~CAlarmChangePkt();
	};


	//=============================================================================================
	//
	//	CAlarmReqPkt
	//	报警事件请求信息包【操作台->AppAlarmMgr】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmReqPkt : public CProtocolHead
	{
	public:
		CAlarmReqPkt();
	    virtual ~CAlarmReqPkt();

	public:
		void SetDispatchId(WORD wSet);						// 调度台ID
		WORD GetDispatchId() const;							// 调度台ID

		virtual ProtocolBase::CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wDispatchId;									// 调度台ID
	};


	//=============================================================================================
	//
	//	CAlarmCommitInfo
	//	报警事件确认信息【操作台->AppAlarmMgr，运维终端->运维报警管理】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmCommitInfo : public CObjectBase
	{
	public:
		CAlarmCommitInfo();
		virtual ~CAlarmCommitInfo();
		
	public:
		void SetUuid(CMyUuid objSet);						// 设置uuid
		CMyUuid* GetUuid();									// 获取uuid

		void SetIndex(BYTE bySet);							// 设置序号
		BYTE GetIndex();									// 获取序号

		void SetStaId(WORD wStaId);							// 设置车站ID
		WORD GetStaId();									// 获取车站ID

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 
		
	private:
		CMyUuid m_objUuid;									// ID
		BYTE m_byIndex;										// 序号
		WORD m_wStaId;										// 报警车站id
	};
	//=============================================================================================
	//
	//	CAlarmCommitPkt
	//	报警事件确认包【操作台->AppAlarmMgr】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmCommitPkt : public CProtocolHead
	{
	public:
		CAlarmCommitPkt();
		virtual ~CAlarmCommitPkt();

	public:
		void Add(CAlarmCommitInfo AlarmEvt);				// 添加报警
		DWORD GetAlarmCount() const;						// 获取报警数量
		CAlarmCommitInfo* GetAlarm(int nIndex);				// 获取指定下标事件

		void SetOperId(DWORD dwOperId);						// 设置操作员Id
		void SetOperName(string strOperName);				// 设置操作员名称
		void SetDispId(WORD wDiapId);						// 设置调度台Id
		void SetDispName(string strDispName);				// 设置调度台名称
		DWORD GetOperId() const;							// 获取操作员Id
		string GetOperName() const;							// 获取操作员名称
		WORD GetDispId() const;								// 获取调度台Id
		string GetDispName() const;							// 获取调度台名称

		virtual ProtocolBase::CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwOperId;									// 操作员Id
		string m_strOperName;								// 操作员名称
		WORD m_wDispId;										// 调度台Id
		string m_strDispName;								// 调度台名称

		vector<CAlarmCommitInfo> m_vecAlarmEvt;				// 报警事件
	};

	
	//=============================================================================================
	//
	//	CAlarmSynReqPkt
	//	报警事件请求同步信息【备AppAlarmMgr->主AppAlarmMgr】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmSynReqPkt : public CProtocolHead
	{
	public:
		CAlarmSynReqPkt();
		virtual ~CAlarmSynReqPkt();
	};

	//=============================================================================================
	//
	//	CAlarmSynAllPkt
	//	报警事件全体同步信息【主AppAlarmMgr->备AppAlarmMgr】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmSynAllPkt : public CAlarmEvtGeneratePkt
	{
	public:
		CAlarmSynAllPkt();
		virtual ~CAlarmSynAllPkt();
	};

	//=============================================================================================
	//
	//	CAlarmSynChgPkt
	//	报警事件变化同步信息【主AppAlarmMgr->备AppAlarmMgr】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmSynChgPkt : public CAlarmEvtGeneratePkt
	{
	public:
		CAlarmSynChgPkt();
		virtual ~CAlarmSynChgPkt();
	};


	//=============================================================================================
	//
	//	CAlarmSavePkt
	//	报警事件存库包【AppAlarmMgr-> OsmAlarm】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmSavePkt : public CAlarmEvtGeneratePkt
	{
	public:
		CAlarmSavePkt();
	    virtual ~CAlarmSavePkt();
	};


	//=============================================================================================
	//
	//	CAlarmUpdatePkt
	//	报警事件更新存库包【AppAlarmMgr-> OsmAlarm】
	//
	//=============================================================================================
	class MY_EXT_CLASS CAlarmUpdatePkt : public CAlarmEvtGeneratePkt
	{
	public:
		CAlarmUpdatePkt();
	    virtual ~CAlarmUpdatePkt();
	};

	//==============================================================================
	//	
	//	CAlarmSaveRetPkt
	//	报警存盘回执包【OSM模块→AppAlarmMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CAlarmSaveRetPkt : public CProtocolHead
	{
	public:
		CAlarmSaveRetPkt();
		virtual ~CAlarmSaveRetPkt();
		
	public:
		void SetRecRslt(DWORD dwRslt);								// 设置接收结果
		void SetDesc(string strDesc);								// 设置描述
		
		DWORD GetRecRslt() const;									// 获取接收结果
		string GetDesc() const;										// 获取描述
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 
		
	private:
		DWORD m_dwRecRslt;											// 接收结果
		string m_strDesc;											// 描述
	};
}

#endif // _PROTOCOL_ALARMINFO_H_