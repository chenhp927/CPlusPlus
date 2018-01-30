//================================================================================================
//	
//									ProtocolAlarmInfo.cpp
//							   
//begin	  : 2016-09-11
//describe: 协议报警事件类
//
//================================================================================================
#include "ProtocolAlarm.h"
#include "ProtocolAlarmInfo.h"

namespace ProtocolAlarm
{
	using namespace ProtocolBase;

	//=============================================================================================
	//
	//						报警全体信息【AppAlarmMgr->操作台，运维报警管理->运维终端】
	//
	//=============================================================================================

	//=============================================================================================
	//
	//											CAlarmAllEvt
	//										报警全体信息报警事件
	//
	//=============================================================================================
	CAlarmAllEvt::CAlarmAllEvt()
		: m_bySerialId(0)
		, m_wAlarmType(0)
		, m_wStaId(0)
		, m_strStaName("")
		, m_dwDevId(0)
		, m_byObjType(0)
		, m_dwObjId(0)
		, m_strObjName("")
		, m_byObjVolLevel(0)
		, m_strDesc("")
		, m_dwOperId(0)
		, m_strOperName("")	
		, m_wDispatcherId(0)
		, m_strDisptcherName("")
		, m_byCommit(ALARMCOMMIT_NO)
		, m_byRelieve(ALARMRELIEVE_NO)
	{

	}

	CAlarmAllEvt::~CAlarmAllEvt()
	{

	}
	
	void CAlarmAllEvt::GenerateAlarmid()
	{
		m_AlarmId.Generate();
	}

	void CAlarmAllEvt::SetAlarmId(CMyUuid& AlarmId)
	{
		m_AlarmId = AlarmId;
	}

	void CAlarmAllEvt::SetSerialId(BYTE bySerialId)
	{
		m_bySerialId = bySerialId;
	}

	void CAlarmAllEvt::SetAlarmType(WORD wAlarmType)
	{
		m_wAlarmType = wAlarmType;
	}

	void CAlarmAllEvt::SetTime(time_t tm, WORD wMs)
	{
		m_Time.Set(tm, wMs);
	}
	
	void CAlarmAllEvt::SetTime(timeval t)
	{
		m_Time.Set(t);
	}
	
	void CAlarmAllEvt::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	void CAlarmAllEvt::SetStaName(string strStaName)
	{
		m_strStaName = strStaName;
	}

	void CAlarmAllEvt::SetDevId(DWORD dwSet)
	{
		m_dwDevId = dwSet;
	}

	void CAlarmAllEvt::SetObjType(BYTE byObjType)
	{
		m_byObjType = byObjType;
	}

	void CAlarmAllEvt::SetObjId(DWORD dwObjId)
	{
		m_dwObjId = dwObjId;
	}

	void CAlarmAllEvt::SetObjName(string strObjName)
	{
		m_strObjName = strObjName;
	}

	void CAlarmAllEvt::SetObjVolLevel(BYTE bySet)
	{
		m_byObjVolLevel = bySet;
	}

	void CAlarmAllEvt::SetDesc(string strDesc)
	{
		m_strDesc = strDesc;
	}

	void CAlarmAllEvt::SetOperId(DWORD dwOperId)
	{
		m_dwOperId = dwOperId;
	}

	void CAlarmAllEvt::SetOperName(string strOperName)
	{
		m_strOperName = strOperName;
	}	

	void CAlarmAllEvt::SetDispatcherId(WORD wDisp)
	{
		m_wDispatcherId = wDisp;
	}

	void CAlarmAllEvt::SetDispatcherName(string strDisp)
	{
		m_strDisptcherName = strDisp;
	}

	void CAlarmAllEvt::SetCommitFlag(BYTE byCommit)
	{
		m_byCommit = byCommit;
	}

	void CAlarmAllEvt::SetCommitTime(time_t t, WORD wMs)
	{
		m_CommitTime.Set(t, wMs);
	}

	void CAlarmAllEvt::SetRelieveFlag(BYTE byRelieve)
	{
		m_byRelieve = byRelieve;
	}

	CMyUuid CAlarmAllEvt::GetAlarmId() const
	{
		return m_AlarmId;
	}

	BYTE CAlarmAllEvt::GetSerialId() const
	{
		return m_bySerialId;
	}

	WORD CAlarmAllEvt::GetAlarmType() const
	{
		return m_wAlarmType;
	}

	CProtocolTime CAlarmAllEvt::GetProtoclTime() const
	{
		return m_Time;
	}

	timeval CAlarmAllEvt::GetTime() const
	{
		return m_Time.GetTimeval();
	}

	WORD CAlarmAllEvt::GetStaId() const
	{
		return m_wStaId;
	}

	string CAlarmAllEvt::GetStaName() const
	{
		return m_strStaName;
	}

	DWORD CAlarmAllEvt::GetDevId() const
	{
		return m_dwDevId;
	}

	BYTE CAlarmAllEvt::GetObjType() const
	{
		return m_byObjType;
	}

	DWORD CAlarmAllEvt::GetObjId() const
	{
		return m_dwObjId;
	}

	string CAlarmAllEvt::GetObjName() const
	{
		return m_strObjName;
	}

	BYTE CAlarmAllEvt::GetObjVolLevel() const
	{
		return m_byObjVolLevel;
	}

	DWORD CAlarmAllEvt::GetOperId() const
	{
		return m_dwOperId;
	}

	string CAlarmAllEvt::GetOperName() const
	{
		return m_strOperName;
	}

	string CAlarmAllEvt::GetDesc() const
	{
		return m_strDesc;
	}

	WORD CAlarmAllEvt::GetDispatcherId() const
	{
		return m_wDispatcherId;
	}

	string CAlarmAllEvt::GetDispatcherName() const
	{
		return m_strDisptcherName;
	}

	BYTE CAlarmAllEvt::GetCommitFlag() const
	{
		return m_byCommit;
	}

	CProtocolTime CAlarmAllEvt::GetProCommitTime() const
	{
		return m_CommitTime;
	}

	timeval CAlarmAllEvt::GetCommitTime() const
	{
		return m_CommitTime.GetTimeval();
	}

	BYTE CAlarmAllEvt::GetRelieveFlag() const
	{
		return m_byRelieve;
	}

	int CAlarmAllEvt::StreamSize()
	{
		return (m_AlarmId.StreamSize() + 1 + 2
			+ m_Time.StreamSize() + 2
			+ 1+ m_strStaName.size()
			+ 4 + 1 + 4
			+ 1+m_strObjName.size()
			+ 1	+ 2+m_strDesc.size() + 4 + 1 
			+ m_strOperName.size() + 2 + 1
			+ m_strDisptcherName.size() + 1
			+ m_CommitTime.StreamSize() + 1);
	}

	int CAlarmAllEvt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAlarmAllEvt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 报警事件Id
		int nOffset = 0;
		int nRet = m_AlarmId.ToStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;
	
		// 报警事件序号
		pBuf[nOffset] = m_bySerialId;
		nOffset += 1;

		// 报警事件类型
		SETSTREAMSHORT(pBuf + nOffset, m_wAlarmType);
		nOffset += 2;

		// 发生时间
		int nLenTmp = nBufLen - nOffset;
		nRet = m_Time.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;
		
		// 车站Id
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 车站名称长度
		BYTE byStaLen = MIN((BYTE)m_strStaName.size(), 255);
		pBuf[nOffset] = byStaLen;
		nOffset += 1;

		// 车站名称
		memcpy(pBuf + nOffset, m_strStaName.c_str(), byStaLen);
		nOffset += byStaLen;

		// 报警所属装置ID
		SETSTREAMINT(pBuf + nOffset, m_dwDevId);
		nOffset += 4;

		// 对象类型
		pBuf[nOffset] = m_byObjType;
		nOffset += 1;

		// 对象Id
		SETSTREAMINT(pBuf + nOffset, m_dwObjId);
		nOffset += 4;

		// 对象名称长度
		BYTE byObjLen = MIN((BYTE)m_strObjName.size(), 255);
		pBuf[nOffset] = byObjLen;
		nOffset += 1;

		// 对象名称
		memcpy(pBuf + nOffset, m_strObjName.c_str(), byObjLen);
		nOffset += byObjLen;
		
		// 对象电压等级
		pBuf[nOffset] = m_byObjVolLevel;
		nOffset += 1;

		// 描述长度
		WORD wDescLen = MIN((WORD)m_strDesc.size(), 65535);
		SETSTREAMSHORT(pBuf + nOffset, wDescLen);
		nOffset += 2;

		// 描述
		memcpy(pBuf + nOffset, m_strDesc.c_str(), wDescLen);
		nOffset += wDescLen;

		// 操作员Id
		SETSTREAMINT(pBuf + nOffset, m_dwOperId);
		nOffset += 4;

		// 操作员Id名称长度
		BYTE byOperLen = MIN((BYTE)m_strOperName.size(), 255);
		pBuf[nOffset] = byOperLen;
		nOffset += 1;

		// 操作员名称
		memcpy(pBuf + nOffset, m_strOperName.c_str(), byOperLen);
		nOffset += byOperLen;

		// 调度台Id
		SETSTREAMSHORT(pBuf + nOffset, m_wDispatcherId);
		nOffset += 2;

		// 调度台名称长度
		BYTE byDispNameLen = (BYTE)MIN(m_strDisptcherName.size(), 255);
		pBuf[nOffset] = byDispNameLen;
		nOffset += 1;

		// 调度台名称
		memcpy(pBuf + nOffset, m_strDisptcherName.c_str(), byDispNameLen);
		nOffset += byDispNameLen;

		// 确认标识
		pBuf[nOffset] = m_byCommit;
		nOffset += 1;

		// 确认时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_CommitTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 解除标识
		pBuf[nOffset] = m_byRelieve;
		nOffset += 1;
		
		return nOffset;
	}

	int CAlarmAllEvt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 报警事件ID
		int nOffset = 0;
		int nRet = m_AlarmId.FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;
		
		// 报警事件序号
		m_bySerialId = pBuf[nOffset];
		nOffset += 1;

		// 报警事件类型
		m_wAlarmType = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 发生时间
		int nLenTmp = nBufLen - nOffset;
		nRet = m_Time.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// 车站Id
		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站名称长度
		BYTE byStaLen = pBuf[nOffset];
		nOffset += 1;

		// 车站名称
		char cStaName[255 + 1] = {0};
		memcpy(cStaName, pBuf + nOffset, byStaLen);
		cStaName[byStaLen] = '\0';
		m_strStaName = cStaName;
		nOffset += byStaLen;

		// 报警所属装置ID
		m_dwDevId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 对象类型
		m_byObjType = pBuf[nOffset];
		nOffset += 1;

		// 对象Id
		m_dwObjId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 对象名称长度
		BYTE byObjNameLen = pBuf[nOffset];
		nOffset += 1;

		// 对象名称
		char cObjname[255 + 1] = {0};
		memcpy(cObjname, pBuf + nOffset, byObjNameLen);
		cObjname[byObjNameLen] = '\0';
		m_strObjName = cObjname;
		nOffset += byObjNameLen;

		// 对象电压等级
		m_byObjVolLevel = pBuf[nOffset];
		nOffset += 1;

		// 描述长度
		WORD wDescLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 描述
		char cObjDesc[65535 + 1] = {0};
		memcpy(cObjDesc, pBuf + nOffset, wDescLen);
		cObjDesc[wDescLen] = '\0';
		m_strDesc = cObjDesc;
		nOffset += wDescLen;

		// 操作员Id
		m_dwOperId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 操作员名称长度
		BYTE byOperLen = pBuf[nOffset];
		nOffset += 1;

		// 操作员名称
		char cOperName[255 + 1] = {0};
		memcpy(cOperName, pBuf + nOffset, byOperLen);
		cOperName[byOperLen] = '\0';
		m_strOperName = cOperName;
		nOffset += byOperLen;

		// 调度台Id
		m_wDispatcherId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 调度台名称长度
		BYTE byDiapNameLen = pBuf[nOffset];
		nOffset += 1;

		// 调度台名称
		char cDispName[255 + 1] = {0};
		memcpy(cDispName, pBuf + nOffset, byDiapNameLen);
		cDispName[byDiapNameLen] = '\0';
		m_strDisptcherName = cDispName;
		nOffset += byDiapNameLen;

		// 确认标识
		m_byCommit = pBuf[nOffset];
		nOffset += 1;

		// 确认时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_CommitTime.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// 解除标识
		m_byRelieve = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//=============================================================================================
	//
	//	CAlarmEvtGeneratePkt
	//	报警事件生成包【其他模块->AppAlarmMgr，其他模块->运维报警管理】
	//
	//=============================================================================================
	CAlarmEvtGeneratePkt::CAlarmEvtGeneratePkt()
		: CProtocolHead(TYPE_ALARM, FUNC_ALARMGENERATE)
	{
		m_vecAlarmEvt.clear();
	}

	CAlarmEvtGeneratePkt::~CAlarmEvtGeneratePkt()
	{

	}

	void CAlarmEvtGeneratePkt::SetDestDispatchId(WORD wSet)
	{
		m_wDestDispatchId = wSet;
	}
	WORD CAlarmEvtGeneratePkt::GetDestDispatchId()
	{
		return m_wDestDispatchId;
	}

	void CAlarmEvtGeneratePkt::Add(CAlarmAllEvt& AlarmEvent)
	{
		m_vecAlarmEvt.push_back(AlarmEvent);
	}

	DWORD CAlarmEvtGeneratePkt::GetAlarmEvtCount() const
	{
		return (DWORD)m_vecAlarmEvt.size();
	}

	CAlarmAllEvt* CAlarmEvtGeneratePkt::GetAlarmEvt(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecAlarmEvt.size())
		{
			return NULL;
		}

		return &m_vecAlarmEvt.at(nIndex);
	}

	void CAlarmEvtGeneratePkt::ClearAllAlarm()
	{
		m_vecAlarmEvt.clear();
	}

	ProtocolBase::CProtocolHead* CAlarmEvtGeneratePkt::Clone()
	{
		CAlarmEvtGeneratePkt* pClone = new CAlarmEvtGeneratePkt();
		*pClone = *this;
		return pClone;
	}

	int CAlarmEvtGeneratePkt::StreamSize()
	{
		int nSize = 0;
		for(int i=0; i<m_vecAlarmEvt.size(); i++)
		{
			nSize += m_vecAlarmEvt.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + 4 + nSize;
	}

	int CAlarmEvtGeneratePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAlarmAllEvt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffset = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 目标调度台ID
		SETSTREAMSHORT(pBuf + nOffset, m_wDestDispatchId);
		nOffset += 2;

		// 报警数量
		DWORD dwLen = (DWORD)MIN(0xffffffff, m_vecAlarmEvt.size());
		SETSTREAMINT(pBuf + nOffset, dwLen);
		nOffset += 4;

		// 报警事件
		for(int i=0; i<dwLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			nRet = m_vecAlarmEvt.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}

		return nOffset;
	}

	int CAlarmEvtGeneratePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecAlarmEvt.clear();

		//  包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAlarmEvtGeneratePkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// 目标调度台ID
		m_wDestDispatchId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 报警数量
		int nAlarmNum = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 报警信息
		for(int i=0; i<nAlarmNum; i++)
		{
			CAlarmAllEvt objEvt;
			int nLenTmp = nBufLen - nOffset;
			nRet = objEvt.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nOffset)
			{
				return -1;
			}
			m_vecAlarmEvt.push_back(objEvt);
			nOffset += nRet;
		}

		return nOffset;
	}


	//=============================================================================================
	//
	//											CAlarmAllPkt
	//						报警全体信息包【AppAlarmMgr->操作台，运维报警管理->运维终端】
	//
	//=============================================================================================
	CAlarmAllPkt::CAlarmAllPkt()
	{
		m_byType = TYPE_ALARM;
		m_byFunc = FUNC_ALARMALL;
	}

	CAlarmAllPkt::~CAlarmAllPkt()
	{

	}

	
	//=============================================================================================
	//
	//											CAlarmChangePkt
	//						报警变化信息包【AppAlarmMgr->操作台，运维报警管理->运维终端】
	//
	//=============================================================================================
	CAlarmChangePkt::CAlarmChangePkt()
	{
		m_byType = TYPE_ALARM;
		m_byFunc = FUNC_ALARMCHANGE;
	}

	CAlarmChangePkt::~CAlarmChangePkt()
	{

	}


	//=============================================================================================
	//
	//											CAlarmReqPkt
	//						报警请求信息包【操作台->AppAlarmMgr，运维终端->运维报警管理】
	//
	//=============================================================================================
	CAlarmReqPkt::CAlarmReqPkt()
		: CProtocolHead(TYPE_ALARM, FUNC_ALARMREQ),
		m_wDispatchId(0)
	{
	}

	CAlarmReqPkt::~CAlarmReqPkt()
	{
	}
	
	void CAlarmReqPkt::SetDispatchId(WORD wSet)
	{
		m_wDispatchId = wSet;
	}

	WORD CAlarmReqPkt::GetDispatchId() const
	{
		return m_wDispatchId;
	}

	ProtocolBase::CProtocolHead* CAlarmReqPkt::Clone()
	{
		CAlarmReqPkt* pClone = new CAlarmReqPkt();
		*pClone = *this;
		return pClone;
	}

	int CAlarmReqPkt::StreamSize()
	{
		return CProtocolHead::HeaderSize() + 2;
	}

	int CAlarmReqPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAlarmReqPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffset = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// ID
		SETSTREAMSHORT(pBuf+nOffset, m_wDispatchId);
		nOffset += 2;

		return nOffset;
	}

	int CAlarmReqPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAlarmReqPkt len\n");
			return -1;
		}
		
		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// ID
		m_wDispatchId = GETSTREAMSHORT(pBuf+nOffset);
		nOffset += 2;

		return nOffset;
	}


	//=============================================================================================
	//
	//	CAlarmCommitInfo
	//	报警事件确认信息【操作台->AppAlarmMgr，运维终端->运维报警管理】
	//
	//=============================================================================================
	CAlarmCommitInfo::CAlarmCommitInfo()
		: m_byIndex(0)
	{
	}
	CAlarmCommitInfo::~CAlarmCommitInfo()
	{
	}
	
	void CAlarmCommitInfo::SetUuid(CMyUuid objSet)
	{
		m_objUuid = objSet;
	}
	CMyUuid* CAlarmCommitInfo::GetUuid()
	{
		return &m_objUuid;
	}
	
	void CAlarmCommitInfo::SetIndex(BYTE bySet)
	{
		m_byIndex = bySet;
	}
	BYTE CAlarmCommitInfo::GetIndex()
	{
		return m_byIndex;
	}
	
	void CAlarmCommitInfo::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}
	WORD CAlarmCommitInfo::GetStaId()
	{
		return m_wStaId;
	}
	
	int CAlarmCommitInfo::StreamSize()
	{
		return CMyUuid::StreamSize()+1+2;
	}
	int CAlarmCommitInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{		
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAlarmCommitInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		// 报警事件Id
		int nOffset = m_objUuid.ToStreamBuf(pBuf, nBufLen);
		if (-1 == nOffset)
		{
			return -1;
		}
		
		// 报警事件序号
		pBuf[nOffset] = m_byIndex;
		nOffset += 1;

		// 车站ID
		SETSTREAMSHORT(pBuf+nOffset, m_wStaId);
		nOffset += 2;

		return nOffset;
	}
	int CAlarmCommitInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 报警事件ID
		int nOffset = m_objUuid.FromStreamBuf(pBuf, nBufLen);
		if (-1 == nOffset)
		{
			return -1;
		}
		
		// 报警事件序号
		m_byIndex = pBuf[nOffset];
		nOffset += 1;

		// 车站ID
		m_wStaId = GETSTREAMSHORT(pBuf+nOffset);
		nOffset += 2;

		return nOffset;
	}


	//=============================================================================================
	//
	//											CAlarmCommitPkt
	//						报警事件确认包【操作台->AppAlarmMgr，运维终端->运维报警管理】
	//
	//=============================================================================================
	CAlarmCommitPkt::CAlarmCommitPkt()
		: CProtocolHead(TYPE_ALARM, FUNC_ALARMCOMMIT)
		, m_dwOperId(0)
		, m_strOperName("")
		, m_wDispId(0)
		, m_strDispName("")
	{
		m_vecAlarmEvt.clear();
	}

	CAlarmCommitPkt::~CAlarmCommitPkt()
	{

	}

	void CAlarmCommitPkt::Add(CAlarmCommitInfo AlarmEvt)
	{
		m_vecAlarmEvt.push_back(AlarmEvt);
	}

	DWORD CAlarmCommitPkt::GetAlarmCount() const
	{
		return m_vecAlarmEvt.size();
	}

	CAlarmCommitInfo* CAlarmCommitPkt::GetAlarm(int nIndex)
	{
		assert(nIndex < m_vecAlarmEvt.size());
		return &m_vecAlarmEvt.at(nIndex);
	}

	void CAlarmCommitPkt::SetOperId(DWORD dwOperId)
	{
		m_dwOperId = dwOperId;
	}
	
	void CAlarmCommitPkt::SetOperName(string strOperName)
	{
		m_strOperName = strOperName;
	}
	
	void CAlarmCommitPkt::SetDispId(WORD wDiapId)
	{
		m_wDispId = wDiapId;
	}
	
	void CAlarmCommitPkt::SetDispName(string strDispName)
	{
		m_strDispName = strDispName;
	}
	
	DWORD CAlarmCommitPkt::GetOperId() const
	{
		return m_dwOperId;
	}
	
	string CAlarmCommitPkt::GetOperName() const
	{
		return m_strOperName;
	}
	
	WORD CAlarmCommitPkt::GetDispId() const
	{
		return m_wDispId;
	}
	
	string CAlarmCommitPkt::GetDispName() const
	{
		return m_strDispName;
	}

	CProtocolHead* CAlarmCommitPkt::Clone()
	{
		CAlarmCommitPkt* pClone = new CAlarmCommitPkt();
		*pClone = *this;
		return pClone;
	}

	int CAlarmCommitPkt::StreamSize()
	{
		int nSize = CProtocolHead::StreamSize();
		nSize = nSize + 4 + 1 + m_strOperName.size() + 2 + 1 + m_strDispName.size() + 4;
		nSize += m_vecAlarmEvt.size() * CAlarmCommitInfo::StreamSize();

		return nSize;
	}

	int CAlarmCommitPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAlarmCommitPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if (-1 == nOffset)
		{
			return -1;
		}
		
		// 操作员Id
		SETSTREAMINT(pBuf + nOffset, m_dwOperId);
		nOffset += 4;
		
		// 操作员Id名称长度
		BYTE byOperLen = MIN((BYTE)m_strOperName.size(), 255);
		pBuf[nOffset] = byOperLen;
		nOffset += 1;
		
		// 操作员名称
		memcpy(pBuf + nOffset, m_strOperName.c_str(), byOperLen);
		nOffset += byOperLen;
		
		// 调度台Id
		SETSTREAMSHORT(pBuf+nOffset, m_wDispId);
		nOffset += 2;
		
		// 调度台名称长度
		BYTE byDispNameLen = (BYTE)MIN(m_strDispName.size(), 255);
		pBuf[nOffset] = byDispNameLen;
		nOffset += 1;
		
		// 调度台名称
		memcpy(pBuf + nOffset, m_strDispName.c_str(), byDispNameLen);
		nOffset += byDispNameLen;

		// 报警数量
		DWORD dwLen = (DWORD)MIN(0xffffffff, m_vecAlarmEvt.size());
		SETSTREAMINT(pBuf + nOffset, dwLen);
		nOffset += 4;

		// 报警事件
		for (int i=0; i<dwLen; i++)
		{
			int nLen = nBufLen - nOffset;
			int nRet = m_vecAlarmEvt.at(i).ToStreamBuf(pBuf + nOffset, nLen);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}

		return nOffset;
	}

	int CAlarmCommitPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecAlarmEvt.clear();

		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAlarmCommitPkt size len\n");
			return -1;
		}	
		
		int nOffset = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nOffset)
		{
			return -1;
		}
		
		// 操作员Id
		m_dwOperId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;
		
		// 操作员名称长度
		BYTE byOperLen = pBuf[nOffset];
		nOffset += 1;
		
		// 操作员名称
		char cOperName[255 + 1] = {0};
		memcpy(cOperName, pBuf + nOffset, byOperLen);
		cOperName[byOperLen] = '\0';
		m_strOperName = cOperName;
		nOffset += byOperLen;
		
		// 调度台Id
		m_wDispId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;
		
		// 调度台名称长度
		BYTE byDiapNameLen = pBuf[nOffset];
		nOffset += 1;
		
		// 调度台名称
		char cDispName[255 + 1] = {0};
		memcpy(cDispName, pBuf + nOffset, byDiapNameLen);
		cDispName[byDiapNameLen] = '\0';
		m_strDispName = cDispName;
		nOffset += byDiapNameLen;
		
		// 报警数量
		DWORD dwAlarmNum = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 报警事件
		CAlarmCommitInfo objAlarmEvt;
		for (int i=0; i<dwAlarmNum; i++)
		{
			int nRet = objAlarmEvt.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			m_vecAlarmEvt.push_back(objAlarmEvt);
			nOffset = nOffset + nRet;
		}

		return nOffset;
	}

	//=============================================================================================
	//
	//	CAlarmSynReqPkt
	//	报警事件请求同步信息【备AppAlarmMgr->主AppAlarmMgr】
	//
	//=============================================================================================
	CAlarmSynReqPkt::CAlarmSynReqPkt()
	{
		m_byType = TYPE_ALARM;
		m_byFunc = FUNC_ALARMSYNREQ;
	}
	CAlarmSynReqPkt::~CAlarmSynReqPkt()
	{
	}

	
	//=============================================================================================
	//
	//	CAlarmSynAllPkt
	//	报警事件全体同步信息【主AppAlarmMgr->备AppAlarmMgr】
	//
	//=============================================================================================
	CAlarmSynAllPkt::CAlarmSynAllPkt()
	{
		m_byType = TYPE_ALARM;
		m_byFunc = FUNC_ALARMSYNALL;
	}
	CAlarmSynAllPkt::~CAlarmSynAllPkt()
	{		
	}
	
	//=============================================================================================
	//
	//	CAlarmSynChgPkt
	//	报警事件变化同步信息【主AppAlarmMgr->备AppAlarmMgr】
	//
	//=============================================================================================
	CAlarmSynChgPkt::CAlarmSynChgPkt()
	{
		m_byType = TYPE_ALARM;
		m_byFunc = FUNC_ALARMSYNCHG;
	}
	CAlarmSynChgPkt::~CAlarmSynChgPkt()
	{		
	}

	//=============================================================================================
	//
	//											CAlarmSavePkt
	//								报警事件存库包【AppAlarmMgr-> OsmAlarm】
	//
	//=============================================================================================
	CAlarmSavePkt::CAlarmSavePkt()
	{
		m_byType = TYPE_ALARM;
		m_byFunc = FUNC_ALARMSAVE;
	}

	CAlarmSavePkt::~CAlarmSavePkt()
	{

	}

	//=============================================================================================
	//
	//											CAlarmUpdatePkt
	//								报警事件更新包【AppAlarmMgr-> OsmAlarm】
	//
	//=============================================================================================
	CAlarmUpdatePkt::CAlarmUpdatePkt()
	{
		m_byType = TYPE_ALARM;
		m_byFunc = FUNC_ALARMUPDATE;
	}

	CAlarmUpdatePkt::~CAlarmUpdatePkt()
	{

	}

	//==============================================================================
	//	
	//	CAlarmSaveRetPkt
	//	报警存盘回执包【OSM模块→AppAlarmMgr】
	//
	//==============================================================================
	CAlarmSaveRetPkt::CAlarmSaveRetPkt()
		: CProtocolHead(TYPE_ALARM, FUNC_ALARMSAVERET)
		, m_dwRecRslt(0)
		, m_strDesc("")
	{

	}

	CAlarmSaveRetPkt::~CAlarmSaveRetPkt()
	{

	}
	
	void CAlarmSaveRetPkt::SetRecRslt(DWORD dwRslt)
	{
		m_dwRecRslt = dwRslt;
	}

	void CAlarmSaveRetPkt::SetDesc(string strDesc)
	{
		m_strDesc = strDesc;
	}

	DWORD CAlarmSaveRetPkt::GetRecRslt() const
	{
		return m_dwRecRslt;
	}

	string CAlarmSaveRetPkt::GetDesc() const
	{
		return m_strDesc;
	}

	CProtocolHead* CAlarmSaveRetPkt::Clone()
	{
		CAlarmSaveRetPkt* pClone = new CAlarmSaveRetPkt();
		*pClone = *this;
		return pClone;
	}

	int CAlarmSaveRetPkt::StreamSize()
	{
		return (int)(CProtocolHead::StreamSize() + 4 + 2 + m_strDesc.size());
	}

	int CAlarmSaveRetPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAlarmSaveRetPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();
		
		// 包头
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if (-1 == nOffset)
		{
			return -1;
		}

		// 接收结果
		SETSTREAMINT(pBuf + nOffset, m_dwRecRslt);
		nOffset += 4;

		// 描述长度
		WORD wDescLen = (WORD)MIN(m_strDesc.size(), 65535);
		SETSTREAMSHORT(pBuf + nOffset, wDescLen);
		nOffset += 2;

		// 描述
		memcpy(pBuf + nOffset, m_strDesc.c_str(), wDescLen);
		nOffset += wDescLen;

		return nOffset;
	}
	
	int CAlarmSaveRetPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAlarmSaveRetPkt header size len\n");
			return -1;
		}

		int nOffset = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nOffset)
		{
			return -1;
		}

		// 接收结果
		m_dwRecRslt = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 描述长度
		WORD wDescLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 描述
		char cDesc[65535 + 1] = {0};
		int nDescLenTmp = MIN(wDescLen, 65535);
		memcpy(cDesc, pBuf + nOffset, nDescLenTmp);
		cDesc[nDescLenTmp] = '\0';
		m_strDesc = cDesc;
		nOffset += wDescLen;

		return nOffset;
	}
}