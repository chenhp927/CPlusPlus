//===================================================================================================
//	
//							 ProtocolDevState.cpp
//							   
//begin	  : 2016-09-18
//describe: 协议设备状态类
//
//===================================================================================================
#include "ProtocolDevState.h"
#include "ProtocolIndication.h"

namespace ProtocolIndication
{
	//===============================================================================================
	//	
	//	CReqStaCommDevPkt
	//	请求站通信设备状态包【其他模块→InterSvr104/101】			   
	//
	//===============================================================================================
	CReqStaCommDevPkt::CReqStaCommDevPkt()
		: CProtocolHead(TYPE_DEVSTATE, FUNC_STACOMM_SVRTOINTER)
	{
		m_vecStaId.clear();
	}

	CReqStaCommDevPkt::~CReqStaCommDevPkt()
	{

	}

	void CReqStaCommDevPkt::Add(WORD wStaId)
	{
		m_vecStaId.push_back(wStaId);
	}

	WORD CReqStaCommDevPkt::GetStaIdCount() const
	{
		return (WORD)m_vecStaId.size();
	}

	WORD CReqStaCommDevPkt::GetStaId(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecStaId.size());
		return m_vecStaId.at(nIndex);
	}

	CProtocolHead* CReqStaCommDevPkt::Clone()
	{
		CReqStaCommDevPkt* pClone = new CReqStaCommDevPkt();
		*pClone = *this;
		return pClone;
	}

	int CReqStaCommDevPkt::StreamSize()
	{
		return (int)(CProtocolHead::HeaderSize() + 2 
			+ m_vecStaId.size() * 2);
	}

	int CReqStaCommDevPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CReqStaCommDevPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		int nOffset = 0;

		// 包头
		int nRet = CProtocolHead::ToStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecStaId.size());
		nOffset += 2;

		// 车站
		for (int i=0; i<m_vecStaId.size(); i++)
		{
			WORD nStaId = m_vecStaId.at(i);
			SETSTREAMSHORT(pBuf + nOffset, nStaId);
			nOffset += 2;
		}

		return nOffset;
	}
	
	int CReqStaCommDevPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqStaCommDevPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;
		
		// 车站数量
		WORD wStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i=0; i<wStaNum; i++)
		{
			WORD wStaId = GETSTREAMSHORT(pBuf + nOffset);
			m_vecStaId.push_back(wStaId);
			nOffset += 2;
		}

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CStaCommDev
	//	站通信设备状态【InterSvr104/101→其他模块】				   
	//
	//===============================================================================================
	CStaCommDev::CStaCommDev()
		: m_wStaId(0)
		, m_strStaName("")
		, m_byABFlag(0)
		, m_byMainStandby1(0)
		, m_byMainStandby2(0)
		, m_byChannel1(0)
		, m_byChannel2(0)
		, m_strChn1Ip1("")
		, m_strChn1Ip2("")
		, m_strChn2Ip1("")
		, m_strChn2Ip2("")
	{

	}

	CStaCommDev::~CStaCommDev()
	{

	}

	bool CStaCommDev::operator==(const CStaCommDev& objOther)
	{
		return(m_wStaId == objOther.m_wStaId
			&& m_strStaName.compare(objOther.m_strStaName) == 0
			&& m_byABFlag == objOther.m_byABFlag
			&& m_byMainStandby1 == objOther.m_byMainStandby1
			&& m_byMainStandby2 == objOther.m_byMainStandby2
			&& m_byChannel1 == objOther.m_byChannel1
			&& m_strChn1Ip1.compare(objOther.m_strChn1Ip1) == 0
			&& m_strChn1Ip2.compare(objOther.m_strChn1Ip2) == 0
			&& m_Channel1ChgTime == objOther.m_Channel1ChgTime
			&& m_byChannel2 == objOther.m_byChannel2
			&& m_strChn2Ip1.compare(objOther.m_strChn2Ip1) == 0
			&& m_strChn2Ip2.compare(objOther.m_strChn2Ip2) == 0
			&& m_Channel2ChgTime == objOther.m_Channel2ChgTime);
	}

	void CStaCommDev::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	void CStaCommDev::SetStaName(string strStaName)
	{
		m_strStaName = strStaName;
	}

	void CStaCommDev::SetABFlag(BYTE byABFlag)
	{
		m_byABFlag = byABFlag;
	}

	void CStaCommDev::SetMainStandby1(BYTE byMainStandby)
	{
		m_byMainStandby1 = byMainStandby;
	}

	void CStaCommDev::SetMainStandby2(BYTE byMainStandby)
	{
		m_byMainStandby2 = byMainStandby;
	}

	void CStaCommDev::SetChannel1(BYTE byChannel1)
	{
		m_byChannel1 = byChannel1;
	}

	void CStaCommDev::SetChn1Ip1(string strIp)
	{
		m_strChn1Ip1 = strIp;
	}

	void CStaCommDev::SetChn1Ip2(string strIp)
	{
		m_strChn1Ip2 = strIp;
	}

	void CStaCommDev::SetChannel1ChgTime(time_t t, WORD wMS)
	{
		m_Channel1ChgTime.Set(t, wMS);
	}

	void CStaCommDev::SetChannel1ChgTime(timeval t)
	{
		m_Channel1ChgTime.Set(t);
	}

	void CStaCommDev::SetChannel2(BYTE byChannel2)
	{
		m_byChannel2 = byChannel2;
	}

	void CStaCommDev::SetChn2Ip1(string strIp)
	{
		m_strChn2Ip1 = strIp;
	}

	void CStaCommDev::SetChn2Ip2(string strIp)
	{
		m_strChn2Ip2 = strIp;
	}

	void CStaCommDev::SetChannel2ChgTime(time_t t, WORD wMS)
	{
		m_Channel2ChgTime.Set(t, wMS);
	}

	void CStaCommDev::SetChannel2ChgTime(timeval t)
	{
		m_Channel2ChgTime.Set(t);
	}

	WORD CStaCommDev::GetStaId() const
	{
		return m_wStaId;
	}

	std::string CStaCommDev::GetStaName() const
	{
		return m_strStaName;
	}

	BYTE CStaCommDev::GetABFlag() const
	{
		return m_byABFlag;
	}

	BYTE CStaCommDev::GetMainStandby1() const
	{
		return m_byMainStandby1;
	}

	BYTE CStaCommDev::GetMainStandby2() const
	{
		return m_byMainStandby2;
	}

	BYTE CStaCommDev::GetChannel1() const
	{
		return m_byChannel1;
	}

	std::string CStaCommDev::GetChn1Ip1() const
	{
		return m_strChn1Ip1;
	}

	std::string CStaCommDev::GetChn1Ip2() const
	{
		return m_strChn1Ip2;
	}

	CProtocolTime CStaCommDev::GetChannel1ProChgTime() const
	{
		return m_Channel1ChgTime;
	}

	time_t CStaCommDev::GetChannel1ChgTime() const
	{
		return m_Channel1ChgTime.GetTime();
	}

	BYTE CStaCommDev::GetChannel2() const
	{
		return m_byChannel2;
	}

	std::string CStaCommDev::GetChn2Ip1() const
	{
		return m_strChn2Ip1;
	}

	std::string CStaCommDev::GetChn2Ip2() const
	{
		return m_strChn2Ip2;
	}

	CProtocolTime CStaCommDev::GetChannel2ProChgTime() const
	{
		return m_Channel2ChgTime;
	}

	time_t CStaCommDev::GetChannel2ChgTime() const
	{
		return m_Channel2ChgTime.GetTime();
	}

	int CStaCommDev::StreamSize()
	{
		return 2 + 1 + m_strStaName.size() 
			+ 1 + 1 + 1 + 1
			+ 1 + m_strChn1Ip1.size()
			+ 1 + m_strChn1Ip2.size()
			+ 1 + m_strChn2Ip1.size()
			+ 1 + m_strChn2Ip2.size()
			+ m_Channel1ChgTime.StreamSize() + 1
			+ m_Channel2ChgTime.StreamSize();
	}

	int CStaCommDev::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaCommDev len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 车站名称长度
		pBuf[nOffset] = m_strStaName.size();
		nOffset += 1;

		// 车站名称
		memcpy(pBuf + nOffset, m_strStaName.c_str(), m_strStaName.size());
		nOffset += m_strStaName.size();

		// AB机标识
		pBuf[nOffset] = m_byABFlag;
		nOffset += 1;

		// 通道1主备状态
		pBuf[nOffset] = m_byMainStandby1;
		nOffset += 1;

		// 通道2主备状态
		pBuf[nOffset] = m_byMainStandby2;
		nOffset += 1;

		// 站通道1通信状态
		pBuf[nOffset] = m_byChannel1;
		nOffset += 1;

		// 站通道1Ip1长度
		pBuf[nOffset] = m_strChn1Ip1.size();
		nOffset += 1;

		// 站通道1Ip1
		memcpy(pBuf + nOffset, m_strChn1Ip1.c_str(), m_strChn1Ip1.size());
		nOffset += m_strChn1Ip1.size();

		// 站通道1Ip2长度
		pBuf[nOffset] = m_strChn1Ip2.size();
		nOffset += 1;

		// 站通道1Ip2
		memcpy(pBuf + nOffset, m_strChn1Ip2.c_str(), m_strChn1Ip2.size());
		nOffset += m_strChn1Ip2.size();

		// 站通道1通信状态变化时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_Channel1ChgTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 站通道2通信状态
		pBuf[nOffset] = m_byChannel2;
		nOffset += 1;

		// 站通道2Ip1长度
		pBuf[nOffset] = m_strChn2Ip1.size();
		nOffset += 1;

		// 站通道2Ip1
		memcpy(pBuf + nOffset, m_strChn2Ip1.c_str(), m_strChn2Ip1.size());
		nOffset += m_strChn2Ip1.size();

		// 站通道2Ip2长度
		pBuf[nOffset] = m_strChn2Ip2.size();
		nOffset += 1;

		// 站通道2Ip2
		memcpy(pBuf + nOffset, m_strChn2Ip2.c_str(), m_strChn2Ip2.size());
		nOffset += m_strChn2Ip2.size();

		// 站通道2通信状态变化时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_Channel2ChgTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		return nOffset;
	}

	int CStaCommDev::FromStreamBuf(const char* pBuf, int nBufLen)
	{		
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaCommDev len\n");
			return -1;
		}

		// 车站ID
		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站名称长度
		BYTE byLen = (BYTE)pBuf[nOffset];
		nOffset += 1;

		// 车站名称		
		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf Error: CStaCommDev Buflen is less than the StaName len\n");
			return -1;
		}
		m_strStaName.resize(byLen);
		memcpy((char*)m_strStaName.c_str(), pBuf + nOffset, byLen);
		nOffset += byLen;

		// AB机标识
		m_byABFlag = pBuf[nOffset];
		nOffset += 1;

		// 通道1主备状态
		m_byMainStandby1 = pBuf[nOffset];
		nOffset += 1;

		// 通道2主备状态
		m_byMainStandby2 = pBuf[nOffset];
		nOffset += 1;

		// 站通道1通信状态
		m_byChannel1 = pBuf[nOffset];
		nOffset += 1;

		// 站通道1Ip1长度
		byLen = (BYTE)pBuf[nOffset];
		nOffset += 1;
		// 站通道1Ip1
		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf Error: CStaCommDev Buflen is less than the Chn1Ip1 len\n");
			return -1;
		}
		m_strChn1Ip1.resize(byLen);
		memcpy((char*)m_strChn1Ip1.c_str(), pBuf + nOffset, byLen);
		nOffset += byLen;

		// 站通道1Ip2长度
		byLen = (BYTE)pBuf[nOffset];
		nOffset += 1;
		// 站通道1Ip2
		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf Error: CStaCommDev Buflen is less than the Chn1Ip2 len\n");
			return -1;
		}
		m_strChn1Ip2.resize(byLen);
		memcpy((char*)m_strChn1Ip2.c_str(), pBuf + nOffset, byLen);
		nOffset += byLen;

		// 站通道1通信状态变化时间
		int nLentmp = nBufLen - nOffset;
		int nRet = m_Channel1ChgTime.FromStreamBuf(pBuf + nOffset, nLentmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 站通道2通信状态
		m_byChannel2 = pBuf[nOffset];
		nOffset += 1;

		// 站通道2Ip1长度
		byLen = (BYTE)pBuf[nOffset];
		nOffset += 1;
		// 站通道2Ip1
		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf Error: CStaCommDev Buflen is less than the Chn2Ip1 len\n");
			return -1;
		}
		m_strChn2Ip1.resize(byLen);
		memcpy((char*)m_strChn2Ip1.c_str(), pBuf + nOffset, byLen);
		nOffset += byLen;

		// 站通道2Ip2长度
		byLen = (BYTE)pBuf[nOffset];
		nOffset += 1;
		// 站通道2Ip2
		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf Error: CStaCommDev Buflen is less than the Chn2Ip2 len\n");
			return -1;
		}
		m_strChn2Ip2.resize(byLen);
		memcpy((char*)m_strChn2Ip2.c_str(), pBuf + nOffset, byLen);
		nOffset += byLen;

		// 站通道2通信状态变化时间
		nLentmp = nBufLen - nOffset;
		nRet = m_Channel2ChgTime.FromStreamBuf(pBuf + nOffset, nLentmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CStaCommDevPkt
	//	站通信设备状态发布包【InterSvr104/101→其他模块】			   
	//
	//===============================================================================================
	CStaCommDevPkt::CStaCommDevPkt()
		: CProtocolHead(TYPE_DEVSTATE, FUNC_STACOMM_INTERTOSVR)
	{
		m_vecSta.clear();
	}

	CStaCommDevPkt::~CStaCommDevPkt()
	{

	}

	void CStaCommDevPkt::Add(CStaCommDev State)
	{
		m_vecSta.push_back(State);
	}

	WORD CStaCommDevPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CStaCommDev* CStaCommDevPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex); 
	}

	CProtocolHead* CStaCommDevPkt::Clone()
	{
		CStaCommDevPkt* pClone = new CStaCommDevPkt();
		*pClone = *this;
		return pClone;
	}

	int CStaCommDevPkt::StreamSize()
	{
		// 计算站通信设备状态所占字节数
		int nSize = 0;
		for (int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + nSize;
	}

	int CStaCommDevPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaCommDevPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffset = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += 2;

		// 车站
		for (int i=0; i<m_vecSta.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			nRet = m_vecSta.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CStaCommDevPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaCommDevPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		WORD wStanum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i=0; i<wStanum; i++)
		{
			CStaCommDev objSta;
			int nLenTmp = nBufLen - nOffset;
			nRet = objSta.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objSta);
			nOffset += nRet;
		}

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CReqAllOperDevPkt
	//	请求全体操控设备状态包【操作台→AppStatusMgr】			   
	//
	//===============================================================================================
	CReqAllOperDevPkt::CReqAllOperDevPkt()
	{
		m_byType = TYPE_DEVSTATE;
		m_byFunc = FUNC_OPERALL_TERMTOSVR;
	}

	CReqAllOperDevPkt::~CReqAllOperDevPkt()
	{

	}

	//==============================================================================
	//
	//	CAllOperDevMeasure
	//	全体操控设备状态中遥测点【AppMeasureMgr→操作台】
	//
	//==============================================================================
	CAllOperDevMeasure::CAllOperDevMeasure()
		: m_dwId(0)
		, m_byQltFlag(0)
		, m_fValue(0.0f)
	{

	}

	CAllOperDevMeasure::~CAllOperDevMeasure()	
	{

	}

	void CAllOperDevMeasure::SetId(DWORD dwId)
	{
		m_dwId = dwId;
	}

	void CAllOperDevMeasure::SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag)
	{
		m_byQltFlag = eFlag;
	}

	void CAllOperDevMeasure::SetValue(float fValue)
	{
		m_fValue = fValue;
	}

	DWORD CAllOperDevMeasure::GetId() const
	{
		return m_dwId;
	}

	ENUM_QUALITY_FLAG_TYPE CAllOperDevMeasure::GetQltFalg() const
	{
		return (ENUM_QUALITY_FLAG_TYPE)m_byQltFlag;
	}

	float CAllOperDevMeasure::GetValue() const
	{
		return m_fValue;
	}

	int CAllOperDevMeasure::StreamSize()
	{
		return 4 + 1 + 4;
	}

	int CAllOperDevMeasure::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf:Buflen is less than the CAllOperDevMeasure len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 遥测点Id
		int nOffset = 0;
		SETSTREAMINT(pBuf + nOffset, m_dwId);
		nOffset += 4;

		// 质量标志
		pBuf[nOffset] = m_byQltFlag;
		nOffset += 1;

		// 遥测值
		SETSTREAMFLOAT(pBuf + nOffset, m_fValue);
		nOffset += 4;

		return nOffset;
	}

	int CAllOperDevMeasure::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf:Buflen is less than the CAllOperDevMeasure len\n");
			return -1;
		}

		// 遥测点Id
		int nOffset = 0;
		m_dwId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 质量标志
		m_byQltFlag = pBuf[nOffset];
		nOffset += 1;

		// 遥测值
		GETSTREAMFLOAT(m_fValue, pBuf + nOffset);
		nOffset += 4;

		return nOffset;
	}


	//===============================================================================================
	//	
	//	CAllOperDevCard
	//	全体操控设备状态中标志牌【AppStatusMgr→操作台】			   
	//
	//===============================================================================================
	CAllOperDevCard::CAllOperDevCard()
		: m_byCardType(0)
		, m_wDevId(0)
		, m_strDispatcherName("")
		, m_strOperName("")
		, m_byOperLevel(0)
		, m_dwQuoteTime(0)
		, m_nOffsetX(0)
		, m_nOffsetY(0)
		, m_strContent("")
	{

	}

	CAllOperDevCard::~CAllOperDevCard()
	{

	}
	
	void CAllOperDevCard::SetCardType(BYTE byCardType)
	{
		m_byCardType = byCardType;
	}

	void CAllOperDevCard::SetCmdCode(string strSet)
	{
		m_strCmdCode = strSet;
	}

	void CAllOperDevCard::SetWorkRange(string strSet)
	{
		m_strWorkRange = strSet;
	}

	void CAllOperDevCard::SetDevId(WORD wSet)
	{
		m_wDevId = wSet;
	}

	void CAllOperDevCard::SetDispatcherName(string strDispatcherName)
	{
		m_strDispatcherName = strDispatcherName;
	}

	void CAllOperDevCard::SetOperName(string strOperName)
	{
		m_strOperName = strOperName;
	}

	void CAllOperDevCard::SetOperLevel(BYTE byOperLevel)
	{
		m_byOperLevel = byOperLevel;
	}
	
	void CAllOperDevCard::SetPassWord(string strSet)
	{
		m_strPassWord = strSet;
	}

	void CAllOperDevCard::SetQuoteTime(DWORD dwQuoteTime)
	{
		m_dwQuoteTime = dwQuoteTime;
	}

	void CAllOperDevCard::SetOffsetX(int nOffsetX)
	{
		m_nOffsetX = nOffsetX;
	}

	void CAllOperDevCard::SetOffsetY(int nOffsetY)
	{
		m_nOffsetY = nOffsetY;
	}

	void CAllOperDevCard::SetContent(string strContent)
	{
		m_strContent = strContent;
	}

	BYTE CAllOperDevCard::GetCardType() const
	{
		return m_byCardType;
	}

	string CAllOperDevCard::GetCmdCode() const
	{
		return m_strCmdCode;
	}

	string CAllOperDevCard::GetWorkRange() const
	{
		return m_strWorkRange;
	}

	WORD CAllOperDevCard::GetDevId() const
	{
		return m_wDevId;
	}

	string CAllOperDevCard::GetDispatcherName() const
	{
		return m_strDispatcherName;
	}

	string CAllOperDevCard::GetOperName() const
	{
		return m_strOperName;
	}

	BYTE CAllOperDevCard::GetOperLevel() const
	{
		return m_byOperLevel;
	}
	
	string CAllOperDevCard::GetPassWord() const
	{
		return m_strPassWord;
	}

	DWORD CAllOperDevCard::GetQuoteTime() const
	{
		return m_dwQuoteTime;
	}

	int CAllOperDevCard::GetOffsetX() const
	{
		return m_nOffsetX;
	}

	int CAllOperDevCard::GetOffsetY() const
	{
		return m_nOffsetY;
	}

	string CAllOperDevCard::GetContent() const
	{
		return m_strContent;
	}

	int CAllOperDevCard::StreamSize()
	{
		return (int)(1 + 1+m_strCmdCode.size()
			+ 1+m_strWorkRange.size()
			+ 2
			+ 1+m_strDispatcherName.size() 
			+ 1+m_strOperName.size()
			+ 1
			+ 1+m_strPassWord.size()
			+ 4 + 4 + 4
			+ 1+m_strContent.size());
	}

	int CAllOperDevCard::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllOperDevCard len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 标志牌类型
		int nOffset = 0;
		pBuf[nOffset] = m_byCardType;
		nOffset += 1;
		
		// 命令编号长度
		BYTE byLen = (BYTE)MIN(m_strCmdCode.size(), 255);
		pBuf[nOffset] = byLen;
		nOffset += 1;
		// 命令编号
		memcpy(pBuf + nOffset, m_strCmdCode.c_str(), byLen);
		nOffset += byLen;

		// 作业范围长度
		byLen = (BYTE)MIN(m_strWorkRange.size(), 255);
		pBuf[nOffset] = byLen;
		nOffset += 1;
		// 作业范围
		memcpy(pBuf + nOffset, m_strWorkRange.c_str(), byLen);
		nOffset += byLen;

		// 设备id
		SETSTREAMSHORT(pBuf + nOffset, m_wDevId);
		nOffset += 2;

		// 调度台名称长度
		byLen = (BYTE)MIN(m_strDispatcherName.size(), 255);
		pBuf[nOffset] = byLen;
		nOffset += 1;
		// 调度台名称
		memcpy(pBuf + nOffset, m_strDispatcherName.c_str(), byLen);
		nOffset += byLen;

		// 操作员名称长度
		byLen = (BYTE)MIN(m_strOperName.size(), 255);
		pBuf[nOffset] = byLen;
		nOffset += 1;
		// 操作员名称
		memcpy(pBuf + nOffset, m_strOperName.c_str(), byLen);
		nOffset += byLen;

		// 操作员级别
		pBuf[nOffset] = m_byOperLevel;
		nOffset += 1;

		// 密码长度
		byLen = m_strPassWord.size();
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// 密码
		memcpy(pBuf + nOffset, m_strPassWord.c_str(), byLen);
		nOffset += byLen;

		// 挂牌时间
		SETSTREAMINT(pBuf + nOffset, m_dwQuoteTime);
		nOffset += 4;

		// 偏移位置X
		SETSTREAMINT(pBuf + nOffset, m_nOffsetX);
		nOffset += 4;

		// 偏移位置Y
		SETSTREAMINT(pBuf + nOffset, m_nOffsetY);
		nOffset += 4;

		// 工作内容长度
		byLen = (BYTE)MIN(m_strContent.size(), 255);
		pBuf[nOffset] = byLen;
		nOffset += 1;
		// 工作内容
		memcpy(pBuf + nOffset, m_strContent.c_str(), byLen);
		nOffset += byLen;

		return nOffset;
	}

	int CAllOperDevCard::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 标志牌类型
		if(nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllOperDevCard len\n");
			return -1;
		}

		int nOffset = 0;
		m_byCardType = pBuf[nOffset];
		nOffset += 1;
		
		// 作业编号长度
		BYTE byLenTmp = pBuf[nOffset];
		nOffset += 1;
		// 作业编号
		char cTemp[255 + 1] = {0};
		memcpy(cTemp, pBuf + nOffset, byLenTmp);
		cTemp[byLenTmp] = '\0';
		m_strCmdCode = cTemp;
		nOffset += byLenTmp;

		// 作业范围长度
		byLenTmp = pBuf[nOffset];
		nOffset += 1;
		// 作业范围
		memcpy(cTemp, pBuf + nOffset, byLenTmp);
		cTemp[byLenTmp] = '\0';
		m_strWorkRange = cTemp;
		nOffset += byLenTmp;

		// 设备ID
		m_wDevId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 调度台名称长度
		byLenTmp = pBuf[nOffset];
		nOffset += 1;
		// 调度台名称
		memcpy(cTemp, pBuf + nOffset, byLenTmp);
		cTemp[byLenTmp] = '\0';
		m_strDispatcherName = cTemp;
		nOffset += byLenTmp;

		// 操作员名称长度
		byLenTmp = pBuf[nOffset];
		nOffset += 1;
		// 操作员名称
		memcpy(cTemp, pBuf + nOffset, byLenTmp);
		cTemp[byLenTmp] = '\0';
		m_strOperName = cTemp;
		nOffset += byLenTmp;

		// 操作员级别
		m_byOperLevel = pBuf[nOffset];
		nOffset += 1;
		
		// 密码长度
		byLenTmp = pBuf[nOffset];
		nOffset += 1;
		
		// 密码字符串
		memcpy(cTemp, pBuf + nOffset, byLenTmp);
		cTemp[byLenTmp] = '\0';
		m_strPassWord = cTemp;
		nOffset += byLenTmp;

		// 挂牌时间
		m_dwQuoteTime = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 偏移位置X
		m_nOffsetX = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 偏移位置Y
		m_nOffsetY = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 工作内容长度
		byLenTmp = pBuf[nOffset];
		nOffset += 1;
		// 工作内容字符串
		memcpy(cTemp, pBuf + nOffset, byLenTmp);
		cTemp[byLenTmp] = '\0';
		m_strContent = cTemp;
		nOffset += byLenTmp;

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CHandSet
	//	全体操控设备状态中手动设置信息【AppStatusMgr→操作台】	   
	//
	//===============================================================================================
	CHandSet::CHandSet()
		: m_dwCollPointId(0)
		, m_byType(0)
		, m_strDispatcherName("")
		, m_strOperName("")
		, m_dwTime(0)
		, m_fValue(0.0)
	{

	}

	CHandSet::~CHandSet()
	{

	}

	void CHandSet::SetCollPointId(DWORD dwCollPointId)
	{
		m_dwCollPointId = dwCollPointId;
	}

	void CHandSet::SetType(ENUM_HAND_SET_TYPE byType)
	{
		m_byType = (BYTE)byType;
	}

	void CHandSet::SetDispatcherName(string strDispatcherName)
	{
		m_strDispatcherName = strDispatcherName;
	}

	void CHandSet::SetOperName(string strOperName)
	{
		m_strOperName = strOperName;
	}

	void CHandSet::SetTime(DWORD dwTm)
	{
		m_dwTime = dwTm;
	}

	void CHandSet::SetValue(float fValue)
	{
		m_fValue = fValue;
	}

	DWORD CHandSet::GetCollPointId() const
	{
		return m_dwCollPointId;
	}

	ENUM_HAND_SET_TYPE CHandSet::GetType() const
	{
		return (ENUM_HAND_SET_TYPE)m_byType;
	}

	string CHandSet::GetDispatcherName() const
	{
		return m_strDispatcherName;
	}

	string CHandSet::GetOperName() const
	{
		return m_strOperName;
	}

	DWORD CHandSet::GetTime() const
	{
		return m_dwTime;
	}

	float CHandSet::GetValue() const
	{
		return m_fValue;
	}

	int CHandSet::StreamSize()
	{
		return (int)(4 + 1 + 1 + m_strDispatcherName.size() + 1
			+ m_strOperName.size() + 4 + 4);
	}

	int CHandSet::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHandSet len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 手动设置采集点ID
		int nOffset = 0;
		SETSTREAMINT(pBuf + nOffset, m_dwCollPointId);
		nOffset += 4;

		// 设置类型
		pBuf[nOffset] = m_byType;
		nOffset += 1;

		// 调度台名称长度
		BYTE byDispatcherLen = (BYTE)MIN(m_strDispatcherName.size(), 255);
		pBuf[nOffset] = byDispatcherLen;
		nOffset += 1;

		// 调度台名称
		memcpy(pBuf + nOffset, m_strDispatcherName.c_str(), byDispatcherLen);
		nOffset += byDispatcherLen;

		// 操作员名称长度
		BYTE byOperName = (BYTE)MIN(m_strOperName.size(), 255);
		pBuf[nOffset] = byOperName;
		nOffset += 1;

		// 操作员名称
		memcpy(pBuf + nOffset, m_strOperName.c_str(), byOperName);
		nOffset += byOperName;

		// 设置时间
		SETSTREAMINT(pBuf + nOffset, m_dwTime);
		nOffset += 4;

		// 设置值
		SETSTREAMFLOAT(pBuf + nOffset, m_fValue);
		nOffset += 4;

		return nOffset;
	}

	int CHandSet::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 手动设置采集点ID
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHandSet id len\n");
			return -1;
		}

		int nOffset = 0;
		m_dwCollPointId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 设置类型
		m_byType = pBuf[nOffset];
		nOffset += 1;

		// 调度台名称长度
		BYTE byLenTmp = pBuf[nOffset];
		nOffset += 1;
		// 调度台名称
		char cTmp[255 + 1] = {0};
		memcpy(cTmp, pBuf + nOffset, byLenTmp);
		cTmp[byLenTmp] = '\0';
		m_strDispatcherName = cTmp;
		nOffset += byLenTmp;

		// 操作员名称长度
		byLenTmp = pBuf[nOffset];
		nOffset += 1;
		// 操作员名称
		memcpy(cTmp, pBuf + nOffset, byLenTmp);
		cTmp[byLenTmp] = '\0';
		m_strOperName = cTmp;
		nOffset += byLenTmp;

		// 设置时间
		m_dwTime = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 设置值
		GETSTREAMFLOAT(m_fValue, pBuf + nOffset);
		nOffset += 4;

		return nOffset;
	}


	//===============================================================================================
	//	
	//	CAllOperDevInfo
	//	全体操控设备状态中操控设备【AppStatusMgr→操作台】	   
	//
	//===============================================================================================
	CAllOperDevInfo::CAllOperDevInfo()
		: m_dwOperDevId(0)
	{
		memset(m_byExpress, 0, sizeof(m_byExpress));
		m_vecMeasure.clear();
		m_vecCard.clear();
		m_vecHandSet.clear();
	}

	CAllOperDevInfo::~CAllOperDevInfo()
	{

	}

	void CAllOperDevInfo::SetOperDevId(DWORD dwOperDevId)
	{
		m_dwOperDevId = dwOperDevId;
	}

	void CAllOperDevInfo::SetExpress(BYTE* pExpress)
	{
		if (NULL == pExpress)
		{
			return;
		}

		memcpy(m_byExpress, pExpress, sizeof(m_byExpress));
	}

	DWORD CAllOperDevInfo::GetOperDevId() const
	{
		return m_dwOperDevId;
	}

	BYTE* CAllOperDevInfo::GetExpress()
	{
		return m_byExpress;
	}

	void CAllOperDevInfo::AddMesure(CAllOperDevMeasure Measure)
	{
		m_vecMeasure.push_back(Measure);
	}

	WORD CAllOperDevInfo::GetMeasureCount() const
	{
		return (WORD)m_vecMeasure.size();
	}

	CAllOperDevMeasure* CAllOperDevInfo::GetMeasure(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecMeasure.size())
		{
			return NULL;
		}

		return &m_vecMeasure.at(nIndex);
	}

	void CAllOperDevInfo::AddCard(CAllOperDevCard Card)
	{
		m_vecCard.push_back(Card);
	}

	BYTE CAllOperDevInfo::GetCardCount() const
	{
		return (BYTE)m_vecCard.size();
	}

	CAllOperDevCard* CAllOperDevInfo::GetCard(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecCard.size())
		{
			return NULL;
		}

		return &m_vecCard.at(nIndex);
	}

	void CAllOperDevInfo::AddHandSet(CHandSet HandSet)
	{
		m_vecHandSet.push_back(HandSet);
	}

	BYTE CAllOperDevInfo::GetHandSetCount() const
	{
		return (BYTE)m_vecHandSet.size();
	}

	CHandSet* CAllOperDevInfo::GetHandSet(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecHandSet.size())
		{
			return NULL;
		}

		return &m_vecHandSet.at(nIndex);
	}

	int CAllOperDevInfo::StreamSize()
	{
		int nMeasureSize = 0;
		int nCardSize = 0;
		int nHandSize = 0;

		// 计算遥测点所占字节数
		for (int i =0; i<m_vecMeasure.size(); i++)
		{
			nMeasureSize += m_vecMeasure.at(i).StreamSize();
		}

		// 计算标志牌所占字节数
		for (int i=0; i<m_vecCard.size(); i++)
		{
			nCardSize += m_vecCard.at(i).StreamSize();
		}

		// 计算手动设置状态所占字节数
		for (int i=0; i<m_vecHandSet.size(); i++)
		{
			nHandSize += m_vecHandSet.at(i).StreamSize();
		}

		return 4 + 8 + 2 + nMeasureSize + 1 + nCardSize + 1 + nHandSize;
	}

	int CAllOperDevInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllOperDevInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 操控设备Id
		int nOffset = 0;
		SETSTREAMINT(pBuf + nOffset, m_dwOperDevId);
		nOffset += 4;

		// 表示信息
		memcpy(pBuf + nOffset, m_byExpress, sizeof(m_byExpress));
		nOffset += sizeof(m_byExpress);

		// 遥测点数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecMeasure.size());
		nOffset += 2;

		// 遥测点信息
		for (int i=0; i<m_vecMeasure.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecMeasure.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		// 标志牌数量
		pBuf[nOffset] = (BYTE)m_vecCard.size();
		nOffset += 1;

		// 标志牌信息
		for (int i=0; i<m_vecCard.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecCard.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		// 手动设置状态数量
		pBuf[nOffset] = (BYTE)m_vecHandSet.size();
		nOffset += 1;

		// 手动设置状态
		for (int i=0; i<m_vecHandSet.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecHandSet.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CAllOperDevInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 操控设备ID
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllOperDevInfo len\n");
			return -1;
		}

		int nOffset = 0;
		m_dwOperDevId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;
		
		// 表示信息
		memcpy(m_byExpress, pBuf + nOffset, 8);
		nOffset += 8;

		// 遥测点数量
		WORD wMeasureNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥测点信息
		for (int i=0; i<wMeasureNum; i++)
		{
			CAllOperDevMeasure objMeasure;
			int nLentmp = nBufLen - nOffset;
			int nRet = objMeasure.FromStreamBuf(pBuf + nOffset, nBufLen);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecMeasure.push_back(objMeasure);
			nOffset += nRet;
		}

		// 标志牌信息数量
		BYTE byCardLen = pBuf[nOffset];
		nOffset += 1;

		// 标志牌信息
		for (int i=0; i<byCardLen; i++)
		{
			CAllOperDevCard objCard;
			int nLentmp = nBufLen - nOffset;
			int nRet = objCard.FromStreamBuf(pBuf + nOffset, nBufLen);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecCard.push_back(objCard);
			nOffset += nRet;
		}

		// 手动设置状态数量
		BYTE byHandSetLen = pBuf[nOffset];
		nOffset += 1;

		// 手动设置状态
		for (int i=0; i<byHandSetLen; i++)
		{
			CHandSet objHandSet;
			int nLentmp = nBufLen - nOffset;
			int nRet = objHandSet.FromStreamBuf(pBuf + nOffset, nBufLen);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecHandSet.push_back(objHandSet);
			nOffset += nRet;
		}

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CAllOperDevSta
	//	全体操控设备状态中车站【AppStatusMgr→操作台】		   
	//
	//===============================================================================================
	CAllOperDevSta::CAllOperDevSta()
		: m_wStaId(0)
	{
		m_vecOperDev.clear();
	}

	CAllOperDevSta::~CAllOperDevSta()
	{

	}

	void CAllOperDevSta::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	WORD CAllOperDevSta::GetStaId() const
	{
		return m_wStaId;
	}

	void CAllOperDevSta::AddOperDev(CAllOperDevInfo OperDev)
	{
		m_vecOperDev.push_back(OperDev);
	}

	WORD CAllOperDevSta::GetOperDevCount() const
	{
		return (WORD)m_vecOperDev.size();
	}

	CAllOperDevInfo* CAllOperDevSta::GetOperDev(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecOperDev.size())
		{
			return NULL;
		}

		return &m_vecOperDev.at(nIndex);
	}

	int CAllOperDevSta::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecOperDev.size(); i++)
		{
			nSize += m_vecOperDev.at(i).StreamSize();
		}

		return 2 + 2 + nSize;
	}

	int CAllOperDevSta::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllOperDevSta len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站ID
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 操控设备数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecOperDev.size());
		nOffset += 2;

		// 操控设备
		for (int i=0; i<m_vecOperDev.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecOperDev.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CAllOperDevSta::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站ID
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllOperDevSta sta id len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 操控设备数量
		WORD wOperDevNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 操控设备
		for (int i=0; i<wOperDevNum; i++)
		{
			CAllOperDevInfo objOperDev;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objOperDev.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecOperDev.push_back(objOperDev);
			nOffset += nRet;
		}

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CAllOperDevPkt
	//	全体操控设备状态包【AppStatusMgr→操作台】			   
	//
	//===============================================================================================
	CAllOperDevPkt::CAllOperDevPkt()
		: CProtocolHead(TYPE_DEVSTATE, FUNC_OPERALL_SVRTOTERM)
	{
		m_vecSta.clear();
	}

	CAllOperDevPkt::~CAllOperDevPkt()
	{

	}

	void CAllOperDevPkt::AddSta(CAllOperDevSta Sta)
	{
		m_vecSta.push_back(Sta);
	}

	WORD CAllOperDevPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CAllOperDevSta* CAllOperDevPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	CProtocolHead* CAllOperDevPkt::Clone()
	{
		CAllOperDevPkt* pClone = new CAllOperDevPkt();
		*pClone = *this;
		return pClone;
	}

	int CAllOperDevPkt::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + nSize;
	}

	int CAllOperDevPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllOperDevPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffset = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += 2;

		// 车站
		for (int i=0; i<m_vecSta.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			nRet = m_vecSta.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CAllOperDevPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllOperDevPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		WORD wStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i=0; i<wStaNum; i++)
		{
			CAllOperDevSta objSta;
			int nLenTmp = nBufLen - nOffset;
			nRet = objSta.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objSta);
			nOffset += nRet;
		}

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CChgOperDevPkt
	//	变化操控设备状态包【AppStatusMgr→操作台】			   
	//
	//===============================================================================================
	CChgOperDevPkt::CChgOperDevPkt()
	{
		m_byType = TYPE_DEVSTATE;
		m_byFunc = FUNC_OPERCHG_SVRTOTERM;
	}

	CChgOperDevPkt::~CChgOperDevPkt()
	{

	}

	//===============================================================================================
	//	
	//	CReqAllDispDevPkt
	//	请求全体电显设备状态包【操作台→AppStatusMgr】		   
	//
	//===============================================================================================
	CReqAllDispDevPkt::CReqAllDispDevPkt()
	{
		m_byType = TYPE_DEVSTATE;
		m_byFunc = FUNC_DISPALL_TERMTOSVR;
	}

	CReqAllDispDevPkt::~CReqAllDispDevPkt()
	{

	}

	//===============================================================================================
	//	
	//	CAllDispDevInfo
	//	全体电显设备状态中电显设备【AppStatusMgr→操作台】
	//
	//===============================================================================================
	CAllDispDevInfo::CAllDispDevInfo()
		: m_dwId(0)
		, m_byValue(0)
	{

	}

	CAllDispDevInfo::~CAllDispDevInfo()
	{

	}

	void CAllDispDevInfo::SetId(DWORD dwId)
	{
		m_dwId = dwId;
	}

	void CAllDispDevInfo::SetValue(BYTE byValue)
	{
		m_byValue = byValue;
	}

	DWORD CAllDispDevInfo::GetId() const
	{
		return m_dwId;
	}

	BYTE CAllDispDevInfo::GetValue() const
	{
		return m_byValue;
	}

	int CAllDispDevInfo::StreamSize()
	{
		return 4 + 1;
	}

	int CAllDispDevInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllDispDevInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 电显设备Id
		int nOffset = 0;
		SETSTREAMINT(pBuf + nOffset, m_dwId);
		nOffset += 4;

		// 电显设备状态值
		pBuf[nOffset] = m_byValue;
		nOffset += 1;

		return nOffset;
	}

	int CAllDispDevInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 电显设备ID
		int nOffset = 0;
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllDispDevInfo len\n");
			return -1;
		}

		m_dwId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 电显设备状态值
		m_byValue = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CAllDispDevSta
	//	全体电显设备状态中车站【AppStatusMgr→操作台】
	//
	//===============================================================================================
	CAllDispDevSta::CAllDispDevSta()
		: m_wStaId(0)
	{
		m_vecDispDev.clear();
	}

	CAllDispDevSta::~CAllDispDevSta()
	{

	}

	void CAllDispDevSta::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	WORD CAllDispDevSta::GetStaId()
	{
		return m_wStaId;
	}

	void CAllDispDevSta::AddDispDev(CAllDispDevInfo DispDev)
	{
		m_vecDispDev.push_back(DispDev);
	}

	WORD CAllDispDevSta::GetDispDevCount() const
	{
		return (WORD)m_vecDispDev.size();
	}

	CAllDispDevInfo* CAllDispDevSta::GetDispDev(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecDispDev.size())
		{
			return NULL;
		}

		return &m_vecDispDev.at(nIndex);
	}

	int CAllDispDevSta::StreamSize()
	{
		return (int)(2 + 2 + m_vecDispDev.size() * CAllDispDevInfo::StreamSize());
	}

	int CAllDispDevSta::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllDispDevSta len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站ID
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 电显设备数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecDispDev.size());
		nOffset += 2;

		// 电显设备
		for (int i=0; i<m_vecDispDev.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecDispDev.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CAllDispDevSta::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站ID
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllDispDevSta len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 电显设备数量
		WORD wDispDevLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 电显设备
		for (int i=0; i<wDispDevLen; i++)
		{
			CAllDispDevInfo objDispDev;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objDispDev.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecDispDev.push_back(objDispDev);
			nOffset += nRet;
		}

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CAllDispDevPkt
	//	全体电显设备状态包【AppStatusMgr→操作台】
	//
	//===============================================================================================
	CAllDispDevPkt::CAllDispDevPkt()
		: CProtocolHead(TYPE_DEVSTATE, FUNC_DISPALL_SVRTOTERM)
	{
		m_vecSta.clear();
	}

	CAllDispDevPkt::~CAllDispDevPkt()
	{

	}

	void CAllDispDevPkt::AddSta(CAllDispDevSta Sta)
	{
		m_vecSta.push_back(Sta);
	}

	WORD CAllDispDevPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CAllDispDevSta* CAllDispDevPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	CProtocolHead* CAllDispDevPkt::Clone()
	{
		CAllDispDevPkt* pClone = new CAllDispDevPkt();
		*pClone = *this;
		return pClone;
	}

	int CAllDispDevPkt::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + nSize;
	}

	int CAllDispDevPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllDispDevPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffset = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += 2;

		// 车站
		for (int i=0; i<m_vecSta.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSta.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CAllDispDevPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllDispDevPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf+ nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		WORD wStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i=0; i<wStaNum; i++)
		{
			CAllDispDevSta objSta;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objSta.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objSta);
			nOffset += nRet;
		}

		return nOffset;
	}

	//===============================================================================================
	//	
	//	CChgDispDevPkt
	//	变化电显设备状态包【AppStatusMgr→操作台】
	//
	//===============================================================================================
	CChgDispDevPkt::CChgDispDevPkt()
	{
		m_byType = TYPE_DEVSTATE;
		m_byFunc = FUNC_DISPCHG_SVRTOTERM;
	}

	CChgDispDevPkt::~CChgDispDevPkt()
	{

	}

	//===============================================================================================
	//	
	//	CChgDispDevFromSvrPkt
	//	变化电显设备状态包【AppElecCalc→AppStatusMgr】
	//
	//===============================================================================================
	CChgDispDevFromSvrPkt::CChgDispDevFromSvrPkt()
	{
		m_byType = TYPE_DEVSTATE;
		m_byFunc = FUNC_DISPCHG_SVRRTOSVR;
	}

	CChgDispDevFromSvrPkt::~CChgDispDevFromSvrPkt()
	{

	}
}