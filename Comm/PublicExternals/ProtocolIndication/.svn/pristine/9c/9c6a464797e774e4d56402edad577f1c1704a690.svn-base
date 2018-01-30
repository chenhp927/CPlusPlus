//=========================================================================================
//	
//							 ProtocolMeasure.h
//							   
// begin    : 2016-08-26
// describe : 协议遥测类
//
//=========================================================================================

#include "ProtocolMeasure.h"

namespace ProtocolIndication
{
	using namespace ProtocolBase;

	//==============================================================================
	//
	//						全体遥测信息【InterSvr104→AppMeasureMgr】
	//
	//==============================================================================

	//==============================================================================
	//	
	//									CMeasureCollAll
	//						全体遥测信息中遥测点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CMeasureCollAll::CMeasureCollAll()
		: m_byFlag(0), m_fValue(0.000f)
	{

	}

	CMeasureCollAll::CMeasureCollAll(BYTE byFlag, float fValue)
	{
		m_byFlag = byFlag;
		m_fValue = fValue;
	}

	CMeasureCollAll::~CMeasureCollAll()
	{

	}

	void CMeasureCollAll::SetFlag(BYTE byFlag)
	{
		m_byFlag = byFlag;
	}

	BYTE CMeasureCollAll::GetFlag() const
	{
		return m_byFlag;
	}

	void CMeasureCollAll::SetValue(float fValue)
	{
		m_fValue = fValue;
	}

	float CMeasureCollAll::GetValue() const
	{
		return m_fValue;
	}

	int CMeasureCollAll::StreamSize()
	{
		return PROTOCOL_MEASUREFLAG_LEN + PROTOCOL_MEASUREVALUE_LEN;	// 遥测值有效性标识长度 + 遥测值长度
	}

	int CMeasureCollAll::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureCollAll len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 遥测值有效性标识
		pBuf[nOffset] = m_byFlag;
		nOffset += PROTOCOL_MEASUREFLAG_LEN;

		// 遥测值
		SETSTREAMFLOAT(pBuf + nOffset, m_fValue);
		nOffset += PROTOCOL_MEASUREVALUE_LEN;

		return nOffset;
	}

	int CMeasureCollAll::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if(nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureCollAll len\n");
			return -1;
		}

		int nOffset = 0;

		// 遥测值有效性标识
		m_byFlag = pBuf[nOffset];
		nOffset += PROTOCOL_MEASUREFLAG_LEN;

		// 遥测值
		GETSTREAMFLOAT(m_fValue, pBuf + nOffset);
		nOffset += PROTOCOL_MEASUREVALUE_LEN;

		return nOffset;
	}

	//==============================================================================
	//	
	//									 CMeasureStaCollAll
	//						全体遥测信息中车站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================

	CMeasureStaCollAll::CMeasureStaCollAll()
		: m_wStaId(0)
		, m_wRealMeaCnt(0)
		, m_dwMeaStartAddr(0)
	{
		m_vecMeasure.clear();
		m_strData = "";
	}

	CMeasureStaCollAll::CMeasureStaCollAll(WORD wStaId)
	{
		m_wStaId = wStaId;
		m_vecMeasure.clear();
		m_strData = "";
	}

	CMeasureStaCollAll::~CMeasureStaCollAll()
	{

	}

	void CMeasureStaCollAll::SetId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	WORD CMeasureStaCollAll::GetId() const
	{
		return m_wStaId;
	}

	void CMeasureStaCollAll::SetMeaStartAddr(DWORD dwStart)
	{
		m_dwMeaStartAddr = dwStart;
	}

	DWORD CMeasureStaCollAll::GetMeaStartAddr() const
	{
		return m_dwMeaStartAddr;
	}

	WORD CMeasureStaCollAll::GetMeasureCount() const
	{ 
		return (WORD)m_vecMeasure.size();
	}

	CMeasureCollAll* CMeasureStaCollAll::GetMeasure(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecMeasure.size())
		{
			return NULL;
		}

		return &m_vecMeasure.at(nIndex);	
	}
		
	void CMeasureStaCollAll::Add(CMeasureCollAll& measure)
	{
		m_vecMeasure.push_back(measure);
	}

	WORD CMeasureStaCollAll::GetRealMeaCnt() const
	{
		return m_wRealMeaCnt;
	}

	std::string CMeasureStaCollAll::GetAllMeas()
	{
		return m_strData;
	}

	void CMeasureStaCollAll::SetAllMeas(string strData)
	{
		m_wRealMeaCnt = strData.length() / CMeasureCollAll::StreamSize();
		m_strData = strData;
	}

	int CMeasureStaCollAll::StreamSize()
	{
		return (int)(PROTOCOL_MEASURESTAID_LEN + PROTOCOL_MEASURENUM_LEN + PROTOCOL_MEASUREADDR_LEN 
			+ CMeasureCollAll::StreamSize() * m_vecMeasure.size());	// 车站Id + 遥测起始地址 + 遥测数量 + 遥测信号 * 遥测数量
	}

	int CMeasureStaCollAll::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureStaCollAll len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 车站ID
		SETSTREAMSHORT(pBuf, m_wStaId);
		nOffset += PROTOCOL_MEASURESTAID_LEN;

		// 遥测起始地址
		SETSTREAMINT(pBuf + nOffset, m_dwMeaStartAddr);
		nOffset += PROTOCOL_MEASUREADDR_LEN;

		// 遥测数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecMeasure.size());
		nOffset += PROTOCOL_MEASURENUM_LEN;
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

		return nOffset;
	}

	int CMeasureStaCollAll::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureStaCollAll len\n");
			return -1;
		}

		int nOffset = 0;

		// 车站ID
		m_wStaId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_MEASURESTAID_LEN;

		// 遥测起始地址
		m_dwMeaStartAddr = GETSTREAMINT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASUREADDR_LEN;
		
		// 遥测数量
		int nMeasureNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURENUM_LEN;

		// 遥测值
		for (int i=0; i<nMeasureNum; i++)
		{
			CMeasureCollAll objMeasure;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objMeasure.FromStreamBuf(pBuf+nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecMeasure.push_back(objMeasure);
			nOffset += nRet;
		}

		return nOffset;
	}

	int CMeasureStaCollAll::StreamSize2()
	{
		return (int)(PROTOCOL_MEASURESTAID_LEN + PROTOCOL_MEASURENUM_LEN + PROTOCOL_MEASUREADDR_LEN
			+ CMeasureCollAll::StreamSize() * m_wRealMeaCnt);	// 车站Id + 遥测起始地址 + 遥测数量 + 遥测信号 * 遥测数量
	}

	int CMeasureStaCollAll::ToStreamBuf2( char* pBuf, int& nBufLen )
	{
		if (nBufLen < (2 + 2 + m_wRealMeaCnt*CMeasureCollAll::StreamSize()))
		{
			LogImportant("ToStreamBuf2: Buflen is less than the CMeasureStaCollAll sta len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nOffset = 0;

		// 车站ID
		SETSTREAMSHORT(pBuf, m_wStaId);
		nOffset += PROTOCOL_MEASURESTAID_LEN;

		// 遥测起始地址
		SETSTREAMINT(pBuf + nOffset, m_dwMeaStartAddr);
		nOffset += PROTOCOL_MEASUREADDR_LEN;

		// 遥测数量
		SETSTREAMSHORT(pBuf + nOffset, m_wRealMeaCnt);
		nOffset += PROTOCOL_MEASURENUM_LEN;
		
		memcpy(pBuf + nOffset, m_strData.data(), m_wRealMeaCnt*CMeasureCollAll::StreamSize());
		nOffset += m_wRealMeaCnt*CMeasureCollAll::StreamSize();		
		
		return nOffset;
	}
	
	int CMeasureStaCollAll::FromStreamBuf2( const char* pBuf, int nBufLen )
	{
		if (nBufLen < StreamSize2())
		{
			LogImportant("FromStreamBuf2: Buflen is less than the CMeasureStaCollAll len\n");
			return -1;
		}
		
		int nOffset = 0;
		// 车站ID
		m_wStaId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_MEASURESTAID_LEN;
		
		// 遥测起始地址
		m_dwMeaStartAddr = GETSTREAMINT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASUREADDR_LEN;

		// 遥测数量
		m_wRealMeaCnt = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURENUM_LEN;
		
		string strData = "";
		strData.resize(m_wRealMeaCnt * CMeasureCollAll::StreamSize());
		memcpy((char*)m_strData.data(), pBuf + nOffset, m_wRealMeaCnt*CMeasureCollAll::StreamSize());
		m_strData = strData;
		nOffset += m_wRealMeaCnt*CMeasureCollAll::StreamSize();	
		
		return nOffset;
	}

	//==============================================================================
	//	
	//								  CMeasureCollAllPkt
	//						全体遥测信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================

	CMeasureCollAllPkt::CMeasureCollAllPkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASALL_INTERTOSVR)	
	{
		m_vecSta.clear();
	}

	CMeasureCollAllPkt::~CMeasureCollAllPkt()
	{

	}

	WORD CMeasureCollAllPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CMeasureStaCollAll* CMeasureCollAllPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	void CMeasureCollAllPkt::Add(CMeasureStaCollAll& sta)
	{
		m_vecSta.push_back(sta);
	}
	
	void CMeasureCollAllPkt::Clear()
	{
		m_vecSta.clear();
	}

	ProtocolBase::CProtocolHead* CMeasureCollAllPkt::Clone()
	{
		CMeasureCollAllPkt* pClone = new CMeasureCollAllPkt();
		*pClone = *this;
		return pClone;
	}

	int CMeasureCollAllPkt::StreamSize()
	{
		int pktHeadLen = CProtocolHead::HeaderSize();
		int nStationSize = 0;
		
		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize();
		}
		
		return pktHeadLen + PROTOCOL_MEASURESTANUM_LEN + nStationSize;	// 包头长度 发送时间长度 +　车站数量 +  车站size
	}

	int CMeasureCollAllPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureCollAllPkt Pkt len\n");
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

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += PROTOCOL_MEASURESTANUM_LEN;

		// 车站信息
		for(int i=0; i<m_vecSta.size(); i++)
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

	int CMeasureCollAllPkt::FromStreamBuf(const char* pBuf, int nBuflen)
	{
		// 包头
		if (nBuflen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureCollAllPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBuflen);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// 车站数量
		int nMeasureStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURESTANUM_LEN;

		// 车站信息
		for (int i=0; i<nMeasureStaNum; i++)
		{
			CMeasureStaCollAll objMeasureSta;
			int nLenTmp = nBuflen - nOffset;
			nRet = objMeasureSta.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			
			m_vecSta.push_back(objMeasureSta);
			nOffset += nRet;
		}
		
		return nOffset;
	}
	
	int CMeasureCollAllPkt::StreamSize2()
	{
		int pktHeadLen = CProtocolHead::HeaderSize();
		int nStationSize = 0;
		
		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize2();
		}
		
		return pktHeadLen + PROTOCOL_MEASURESTANUM_LEN + nStationSize;	// 包头长度 发送时间长度 +　车站数量 +  车站size
	}

	int CMeasureCollAllPkt::ToStreamBuf2( char* pBuf, int& nBufLen )
	{
		if (nBufLen < StreamSize2())
		{
			LogImportant("ToStreamBuf2: Buflen is less than the CMeasureCollAllPkt Pkt len\n");
			
			nBufLen = StreamSize2();
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
		
		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += PROTOCOL_MEASURESTANUM_LEN;
		
		// 车站信息
		for(int i=0; i<m_vecSta.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSta.at(i).ToStreamBuf2(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			
			nOffset += nRet;
		}
		
		return nOffset;
	}
	
	int CMeasureCollAllPkt::FromStreamBuf2( const char* pBuf, int nBufLen )
	{
		// 包头
		if (nBufLen < StreamSize2())
		{
			LogImportant("FromStreamBuf2: Buflen is less than the CMeasureCollAllPkt size len\n");
			return -1;
		}
		
		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}
		
		nOffset += nRet;
		
		// 车站数量
		int nMeasureStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURESTANUM_LEN;
		
		// 车站信息
		for (int i=0; i<nMeasureStaNum; i++)
		{
			CMeasureStaCollAll objMeasureSta;
			int nLenTmp = nBufLen - nOffset;
			nRet = objMeasureSta.FromStreamBuf2(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			
			m_vecSta.push_back(objMeasureSta);
			nOffset += nRet;
		}
		
		return nOffset;
	}

	//==============================================================================
	//
	//						变化遥测信息【InterSvr104→AppMeasureMgr】
	//
	//==============================================================================

	//==============================================================================
	//	
	//								 CMeasureCollChange
	//					变化遥测信息中遥测点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CMeasureCollChange::CMeasureCollChange()
		: m_dwAddr(0), m_byFlag(0), m_fValue(0.000f)
	{

	}
	
	CMeasureCollChange::~CMeasureCollChange()
	{

	}

	void CMeasureCollChange::SetAddr(DWORD dwAddr)
	{
		m_dwAddr = dwAddr;
	}

	DWORD CMeasureCollChange::GetAddr() const
	{
		return m_dwAddr;
	}

	void CMeasureCollChange::SetFlag(BYTE byFlag)
	{
		m_byFlag = byFlag;
	}

	BYTE CMeasureCollChange::GetFlag() const
	{
		return m_byFlag;
	}

	void CMeasureCollChange::SetValue(float fValue)
	{
		m_fValue = fValue;
	}

	float CMeasureCollChange::GetValue() const
	{
		return m_fValue;
	}

	int CMeasureCollChange::StreamSize()
	{
		return 4 + PROTOCOL_MEASUREFLAG_LEN + PROTOCOL_MEASUREVALUE_LEN;
	}

	int CMeasureCollChange::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureCollChange measure value len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 遥测地址
		SETSTREAMINT(pBuf, m_dwAddr);
		nOffset += 4;

		// 遥信值有效性标识
		pBuf[nOffset] = m_byFlag;
		nOffset += PROTOCOL_MEASUREFLAG_LEN;

		// 遥测值
		SETSTREAMFLOAT(pBuf + nOffset, m_fValue);
		nOffset += PROTOCOL_MEASUREVALUE_LEN;
		
		return nOffset;
	}

	int CMeasureCollChange::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 遥测地址
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureCollChange  len\n");
			return -1;
		}

		int nOffset = 0;
		m_dwAddr =  GETSTREAMINT(pBuf);
		nOffset += 4;

		// 遥信值有效性标识
		m_byFlag = pBuf[nOffset];
		nOffset += PROTOCOL_MEASUREFLAG_LEN;

		// 遥测值
		GETSTREAMFLOAT(m_fValue, pBuf + nOffset);
		nOffset += PROTOCOL_MEASUREVALUE_LEN;

		return nOffset;
	}

	//==============================================================================
	//	
	//								CMeasureStaCollChange
	//						变化遥测信息中车站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================

	CMeasureStaCollChange::CMeasureStaCollChange()
		: m_wStaId(0)
	{
		m_vecMeasure.clear();
	}

	CMeasureStaCollChange::CMeasureStaCollChange(WORD wStaId)
	{
		m_wStaId = wStaId;
		m_vecMeasure.clear();
	}

	CMeasureStaCollChange::~CMeasureStaCollChange()
	{

	}

	void CMeasureStaCollChange::SetId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	WORD CMeasureStaCollChange::GetId() const
	{
		return m_wStaId;
	}

	
	WORD CMeasureStaCollChange::GetMeasureCount() const
	{
		return (WORD)m_vecMeasure.size();
	}
	
	CMeasureCollChange* CMeasureStaCollChange::GetMeasure(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecMeasure.size())
		{
			return NULL;
		}

		return &m_vecMeasure.at(nIndex);
	}
		
	void  CMeasureStaCollChange::Add(CMeasureCollChange &measure)
	{
		m_vecMeasure.push_back(measure);
	}

	int CMeasureStaCollChange::StreamSize()
	{
		return (int)(PROTOCOL_MEASURESTAID_LEN + PROTOCOL_MEASURESTANUM_LEN
			+ m_vecMeasure.size() * CMeasureCollChange::StreamSize());
	}

	int CMeasureStaCollChange::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the MeasureStaCollChange Sta len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffet = 0;

		// 车站Id
		SETSTREAMSHORT(pBuf, m_wStaId);
		nOffet += PROTOCOL_MEASURESTAID_LEN;

		// 遥测数量
		SETSTREAMSHORT(pBuf + nOffet, m_vecMeasure.size());
		nOffet += PROTOCOL_MEASURENUM_LEN;

		// 遥测值
		for (int i=0; i<m_vecMeasure.size(); i++)
		{
			int nLenTmp = nBufLen - nOffet;
			int nRet = m_vecMeasure.at(i).ToStreamBuf(pBuf + nOffet, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			
			nOffet += nRet;
		}

		return nOffet;
	}

	int CMeasureStaCollChange::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the MeasureStaCollChange len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_MEASURESTAID_LEN;

		// 遥测数量
		int nMeasureNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURENUM_LEN;

		// 遥测值
		for (int i=0; i<nMeasureNum; i++)
		{
			CMeasureCollChange objMeasure;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objMeasure.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecMeasure.push_back(objMeasure);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//	
	//									CMeasureCollChangePkt
	//							变化遥测信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================

	CMeasureCollChangePkt::CMeasureCollChangePkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASCHG_INTERTOSVR)
	{
		m_vecSta.clear();
	}

	CMeasureCollChangePkt::~CMeasureCollChangePkt()
	{

	}

	WORD CMeasureCollChangePkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}
	
	CMeasureStaCollChange* CMeasureCollChangePkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}
	
	void CMeasureCollChangePkt::Add(CMeasureStaCollChange &sta)
	{
		m_vecSta.push_back(sta);
	}

	ProtocolBase::CProtocolHead* CMeasureCollChangePkt::Clone()
	{
		CMeasureCollChangePkt* pClone = new CMeasureCollChangePkt();
		*pClone = *this;
		return pClone;
	}

	int CMeasureCollChangePkt::StreamSize()
	{
		int pktHeadLen = CProtocolHead::HeaderSize();
		int nStationSize = 0;

		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize();
		}
		
		return pktHeadLen + PROTOCOL_MEASURESTANUM_LEN + nStationSize;	// 包头长度 +　车站数量 +  车站size
	}

	int CMeasureCollChangePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureCollChangePkt Pkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffet = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffet += nRet;

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffet, m_vecSta.size());
		nOffet += PROTOCOL_MEASURESTANUM_LEN;
		
		// 车站信息
		for (int i=0; i<m_vecSta.size(); i++)
		{
			int nLenTmp = nBufLen - nOffet;
			nRet = m_vecSta.at(i).ToStreamBuf(pBuf + nOffet, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffet += nRet;
		}

		return nOffet;
	}

	int CMeasureCollChangePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStramBuf: Buflen is less than the CMeasureCollChangePkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		int nStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURESTANUM_LEN;

		// 车站信息
		for (int i=0; i<nStaNum; i++)
		{
			CMeasureStaCollChange objSta;
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

	//==============================================================================
	//
	//						请求遥测信息【AppMeasureMgr→InterSvr104】
	//
	//==============================================================================


	//==============================================================================
	//
	//									CMeasureCollReqPkt
	//						请求遥测信息包【AppMeasureMgr→InterSvr104】
	//
	//==============================================================================
	CMeasureCollReqPkt::CMeasureCollReqPkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASREQ_SVRTOINTER)
	{
		m_vecSta.clear();
	}
	
	CMeasureCollReqPkt::~CMeasureCollReqPkt()
	{

	}

	WORD CMeasureCollReqPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	WORD CMeasureCollReqPkt::GetSta(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecSta.size());
		return m_vecSta.at(nIndex);
	}
		
	void CMeasureCollReqPkt::Add(WORD sta)
	{
		m_vecSta.push_back(sta);
	}

	ProtocolBase::CProtocolHead* CMeasureCollReqPkt::Clone()
	{
		CMeasureCollReqPkt* pClone = new CMeasureCollReqPkt();
		*pClone = *this;
		return pClone;
	}

	int CMeasureCollReqPkt::StreamSize()
	{
		return (int)(CProtocolHead::StreamSize() + 2 + m_vecSta.size() * 2);
	}

	int CMeasureCollReqPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureCollReqPkt len\n");
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

		// 车站数量
		SETSTREAMSHORT(pBuf+nOffset, m_vecSta.size());
		nOffset += 2;

		// 车站
		for (int  i = 0;  i < m_vecSta.size();  i++)
		{
			SETSTREAMSHORT(pBuf+nOffset, m_vecSta.at(i));			
			nOffset += 2;
		}
		
		return nOffset;
	}

	int CMeasureCollReqPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureCollReqPkt size len\n");
			return -1;
		}

		int nOffset = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (nOffset < 0)
		{
			return -1;
		}

		// 车站数量
		int nStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i = 0; i < nStaNum; i++)
		{
			m_vecSta.push_back(GETSTREAMSHORT(pBuf + nOffset));
			nOffset += 2;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//						全体遥测信息【AppMeasureMgr→操作台】
	//
	//==============================================================================

	//==============================================================================
	//
	//									CMeasureAll
	//						全体遥测信息中遥测点【AppMeasureMgr→操作台】
	//
	//==============================================================================
	CMeasureAll::CMeasureAll()
		: m_dwId(0)
		, m_byQltFlag(0)
		, m_fValue(0.0f)
	{

	}
	
	CMeasureAll::~CMeasureAll()	
	{

	}

	void CMeasureAll::SetId(DWORD dwId)
	{
		m_dwId = dwId;
	}

	void CMeasureAll::SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag)
	{
		m_byQltFlag = eFlag;
	}

	void CMeasureAll::SetValue(float fValue)
	{
		m_fValue = fValue;
	}

	DWORD CMeasureAll::GetId() const
	{
		return m_dwId;
	}
	
	ENUM_QUALITY_FLAG_TYPE CMeasureAll::GetQltFalg() const
	{
		return (ENUM_QUALITY_FLAG_TYPE)m_byQltFlag;
	}

	float CMeasureAll::GetValue() const
	{
		return m_fValue;
	}

	int CMeasureAll::StreamSize()
	{
		return 4 + 1 + 4;
	}

	int CMeasureAll::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf:Buflen is less than the CMeasureAll len\n");
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

	int CMeasureAll::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf:Buflen is less than the CMeasureAll len\n");
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

	//==============================================================================
	//
	//									CMeasureAllSta
	//						全体遥测信息中车站【AppMeasureMgr→操作台】
	//
	//==============================================================================
	CMeasureAllSta::CMeasureAllSta()
		: m_wStaId(0)
	{
		m_vecMeasure.clear();
	}

	CMeasureAllSta::~CMeasureAllSta()
	{

	}

	void CMeasureAllSta::SetId(WORD wSId)
	{
		m_wStaId = wSId;
	}

	void CMeasureAllSta::Add(CMeasureAll Measure)
	{
		m_vecMeasure.push_back(Measure);
	}

	WORD CMeasureAllSta::GetId() const
	{
		return m_wStaId;
	}

	CMeasureAll* CMeasureAllSta::GetMeasure(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecMeasure.size())
		{
			return NULL;
		}

		return &m_vecMeasure.at(nIndex);
	}

	WORD CMeasureAllSta::GetMeasureCount() const
	{
		return (WORD)m_vecMeasure.size();
	}

	void CMeasureAllSta::Clear()
	{
		m_vecMeasure.clear();
	}

	int CMeasureAllSta::StreamSize()
	{
		int nSize = PROTOCOL_MEASURESTAID_LEN + PROTOCOL_MEASURENUM_LEN;
		
		for (int i=0; i<m_vecMeasure.size(); i++)
		{
			nSize += m_vecMeasure[i].StreamSize();
		}
		
		return nSize;
	}

	int CMeasureAllSta::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf:Buflen is less than the CMeasureAllSta len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 遥测数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecMeasure.size());
		nOffset += 2;

		// 遥测
		for (int i=0; i<m_vecMeasure.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecMeasure[i].ToStreamBuf(pBuf+nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CMeasureAllSta::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf:Buflen is less than the CMeasureAllSta len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;
		
		// 遥测数量
		int nMeasureNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥测
		for (int i=0; i<nMeasureNum; i++)
		{
			CMeasureAll objMeasure;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objMeasure.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecMeasure.push_back(objMeasure);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//									CMeasureAllPkt
	//						全体遥测信息包【AppMeasureMgr→操作台】
	//
	//==============================================================================
	CMeasureAllPkt::CMeasureAllPkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASALL_SVRTOTERM)
	{
		m_vecSta.clear();
	}

	CMeasureAllPkt::~CMeasureAllPkt()
	{

	}

	void CMeasureAllPkt::Add(CMeasureAllSta Sta)
	{
		m_vecSta.push_back(Sta);
	}

	CMeasureAllSta* CMeasureAllPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	WORD CMeasureAllPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	ProtocolBase::CProtocolHead* CMeasureAllPkt::Clone()
	{
		CMeasureAllPkt* pClone = new CMeasureAllPkt();
		*pClone = *this;
		return pClone;
	}

	int CMeasureAllPkt::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + nSize;
	}

	int CMeasureAllPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureAllPkt Pkt len\n");
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

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += PROTOCOL_MEASURESTANUM_LEN;

		// 车站信息
		for(int i=0; i<m_vecSta.size(); i++)
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

	int CMeasureAllPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureAllPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		int nMeasureStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURESTANUM_LEN;

		// 车站信息
		for (int i=0; i<nMeasureStaNum; i++)
		{
			CMeasureAllSta objMeasureSta;
			int nLenTmp = nBufLen - nOffset;
			nRet = objMeasureSta.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objMeasureSta);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//								CMeasureChangePkt
	//						变化遥测信息包【AppMeasureMgr→操作台】
	//
	//==============================================================================
	CMeasureChangePkt::CMeasureChangePkt()
	{
		m_byType = TYPE_MEASURE_INFO;
		m_byFunc = FUNC_MEASCHG_SVRTOTERM;
	}

	CMeasureChangePkt::~CMeasureChangePkt()
	{

	}

	//==============================================================================
	//
	//								CMeasSave
	//						遥测信息存储包【AppMeasureMgr→OSM模块】-遥测点
	//
	//==============================================================================
	CMeasSave::CMeasSave()
		: m_dwId(0)
		, m_strName("")
		, m_byLevel(0)
		, m_byQltFlag(0)
		, m_fValue(0.0f)
	{
		// CProtocolTime对象在构造的时候会自动初始化时间，所以此处不需要再初始化时间成员对象
	}
	
	CMeasSave::~CMeasSave()	
	{

	}

	void CMeasSave::SetMeasId(DWORD dwId)
	{
		m_dwId = dwId;
	}

	void CMeasSave::SetSaveTime(timeval tSet)
	{
		m_objTime.Set(tSet);
	}

	void CMeasSave::SetMeasName(string strSet)
	{
		m_strName = strSet;
	}
	
	void CMeasSave::SetMeasUnit(string strSet)
	{
		m_strUnit = strSet;
	}
	
	void CMeasSave::SetMeasType(ENUM_MEASURE_TYPE eSet)
	{
		m_byMeasType = eSet;
	}

	void CMeasSave::SetVolLevel(BYTE bySet)
	{
		m_byLevel = bySet;
	}

	void CMeasSave::SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag)
	{
		m_byQltFlag = eFlag;
	}

	void CMeasSave::SetValue(float fValue)
	{
		m_fValue = fValue;
	}

	DWORD CMeasSave::GetMeasId() const
	{
		return m_dwId;
	}
	
	CProtocolTime* CMeasSave::GetSaveTime()
	{
		return &m_objTime;
	}

	string CMeasSave::GetMeasName() const
	{
		return m_strName;
	}

	string CMeasSave::GetMeasUnit() const
	{
		return m_strUnit;
	}

	ENUM_MEASURE_TYPE CMeasSave::GetMeasType() const
	{
		return (ENUM_MEASURE_TYPE)m_byMeasType;
	}

	BYTE CMeasSave::GetVolLevel() const
	{
		return m_byLevel;
	}

	ENUM_QUALITY_FLAG_TYPE CMeasSave::GetQltFalg() const
	{
		return (ENUM_QUALITY_FLAG_TYPE)m_byQltFlag;
	}

	float CMeasSave::GetValue() const
	{
		return m_fValue;
	}

	int CMeasSave::StreamSize()
	{
		return int(4 + CProtocolTime::StreamSize() + 1+m_strName.length()
			+ 1+m_strUnit.length() + 1 + 1 + 1 + 4);
	}

	int CMeasSave::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf:Buflen is less than the CMeasSave len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 遥测点Id
		int nOffset = 0;
		SETSTREAMINT(pBuf + nOffset, m_dwId);
		nOffset += 4;

		// 时间
		int nTmp = nBufLen-nOffset;
		int nRet = m_objTime.ToStreamBuf(pBuf+nOffset, nTmp);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 遥信名称
		pBuf[nOffset] = (char)m_strName.length();
		nOffset ++;
		memcpy(pBuf + nOffset, m_strName.c_str(), m_strName.length());
		nOffset += (int)m_strName.length();

		// 单位
		pBuf[nOffset] = (char)m_strUnit.length();
		nOffset ++;
		memcpy(pBuf + nOffset, m_strUnit.c_str(), m_strUnit.length());
		nOffset += (int)m_strUnit.length();
		
		// 遥信点类型
		pBuf[nOffset] = m_byMeasType;
		nOffset ++;

		// 遥信电压等级
		pBuf[nOffset] = m_byLevel;
		nOffset ++;

		// 质量标志
		pBuf[nOffset] = m_byQltFlag;
		nOffset += 1;

		// 遥测值
		SETSTREAMFLOAT(pBuf + nOffset, m_fValue);
		nOffset += 4;

		return nOffset;
	}

	int CMeasSave::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf:Buflen is less than the CMeasSave len\n");
			return -1;
		}

		// 遥测点Id
		int nOffset = 0;
		m_dwId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 时间
		int nRet = m_objTime.FromStreamBuf(pBuf+nOffset, nBufLen-nOffset);
		if (nRet < 0)
		{
			return -1;
		}
		nOffset += nRet;

		// 遥信名称
		BYTE byLen = pBuf[nOffset];
		nOffset ++;
		char cObjname[255] = {0};
		memcpy(cObjname, pBuf + nOffset, byLen);
		m_strName = cObjname;
		nOffset += byLen;
		
		// 单位
		byLen = pBuf[nOffset];
		nOffset ++;
		memset(cObjname, 0, 255);
		memcpy(cObjname, pBuf + nOffset, byLen);
		m_strUnit = cObjname;
		nOffset += byLen;
		
		// 遥信点类型
		m_byMeasType = pBuf[nOffset];
		nOffset ++;

		// 遥信电压等级
		m_byLevel = pBuf[nOffset];
		nOffset ++;

		// 质量标志
		m_byQltFlag = pBuf[nOffset];
		nOffset += 1;

		// 遥测值
		GETSTREAMFLOAT(m_fValue, pBuf + nOffset);
		nOffset += 4;

		return nOffset;
	}
	
	
	//==============================================================================
	//
	//								CStaMeasSave
	//						遥测信息存储包【AppMeasureMgr→OSM模块】-车站
	//
	//==============================================================================
	CStaMeasSave::CStaMeasSave()
		: m_wStaId(0)
	{
		m_vecMeasure.clear();
	}

	CStaMeasSave::~CStaMeasSave()
	{

	}

	void CStaMeasSave::SetStaId(WORD wSId)
	{
		m_wStaId = wSId;
	}

	void CStaMeasSave::Add(CMeasSave& objAdd)
	{
		m_vecMeasure.push_back(objAdd);
	}

	WORD CStaMeasSave::GetStaId() const
	{
		return m_wStaId;
	}

	CMeasSave* CStaMeasSave::GetMeas(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecMeasure.size())
		{
			return NULL;
		}

		return &m_vecMeasure.at(nIndex);
	}

	WORD CStaMeasSave::GetMeasCount() const
	{
		return (WORD)m_vecMeasure.size();
	}

	void CStaMeasSave::Clear()
	{
		m_vecMeasure.clear();
	}

	int CStaMeasSave::StreamSize()
	{
		int nSize = PROTOCOL_MEASURESTAID_LEN + PROTOCOL_MEASURENUM_LEN;
		
		for (int i=0; i<m_vecMeasure.size(); i++)
		{
			nSize += m_vecMeasure[i].StreamSize();
		}
		
		return nSize;
	}

	int CStaMeasSave::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf:Buflen is less than the CStaMeasSave len");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 遥测数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecMeasure.size());
		nOffset += 2;

		// 遥测
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

		return nOffset;
	}

	int CStaMeasSave::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf:Buflen is less than the CStaMeasSave len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;
		
		// 遥测数量
		int nMeasureNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥测
		for (int i=0; i<nMeasureNum; i++)
		{
			CMeasSave objMeasure;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objMeasure.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecMeasure.push_back(objMeasure);
			nOffset += nRet;
		}

		return nOffset;
	}
	
	//==============================================================================
	//
	//								CMeasureSavePkt
	//						遥测值信息存储包【AppMeasureMgr→OSM模块】
	//
	//==============================================================================
	CMeasSavePkt::CMeasSavePkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASVAL_SVRTOOSM)
	{
		m_vecSta.clear();
	}

	CMeasSavePkt::~CMeasSavePkt()
	{

	}

	void CMeasSavePkt::Add(CStaMeasSave& objAdd)
	{
		m_vecSta.push_back(objAdd);
	}

	CStaMeasSave* CMeasSavePkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	WORD CMeasSavePkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	void CMeasSavePkt::Clear()
	{
		m_vecSta.clear();
	}

	ProtocolBase::CProtocolHead* CMeasSavePkt::Clone()
	{
		CMeasSavePkt* pClone = new CMeasSavePkt();
		*pClone = *this;
		return pClone;
	}

	int CMeasSavePkt::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + nSize;
	}

	int CMeasSavePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureSavePkt Pkt len\n");
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

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += PROTOCOL_MEASURESTANUM_LEN;

		// 车站信息
		for(int i=0; i<m_vecSta.size(); i++)
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

	int CMeasSavePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureSavePkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		int nMeasureStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_MEASURESTANUM_LEN;

		// 车站信息
		for (int i=0; i<nMeasureStaNum; i++)
		{
			CStaMeasSave objMeasureSta;
			int nLenTmp = nBufLen - nOffset;
			nRet = objMeasureSta.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objMeasureSta);
			nOffset += nRet;
		}

		return nOffset;
	}
	
	//==============================================================================
	//
	//								CMeasLimitSave
	//						遥测越限信息存储包【AppMeasureMgr→OSM模块】-越限记录
	//
	//==============================================================================
	CMeasLimitSave::CMeasLimitSave()
	{
		m_wStaId = 0;
		m_strStaName = "";
		m_dwObjId = 0;
		m_strObjName = "";
		m_byLevel = 0;
		m_byRecoverFlag = 0;
		m_wAlarmType = 0;
		m_byLimitType = 0;
		m_fValue = 0;
		m_strUnit = "";
		m_strDesc = "";
	}
	CMeasLimitSave::~CMeasLimitSave()
	{

	}	
	void CMeasLimitSave::SetStaId(WORD wSet)
	{
		m_wStaId = wSet;
	}
	void CMeasLimitSave::SetStaName(string strSet)
	{
		m_strStaName = strSet;
	}
	void CMeasLimitSave::SetObjId(DWORD dwSet)
	{
		m_dwObjId = dwSet;
	}
	void CMeasLimitSave::SetObjName(string strSet)
	{
		m_strObjName = strSet;
	}
	void CMeasLimitSave::SetVolLevel(BYTE bySet)
	{
		m_byLevel = bySet;
	}
	void CMeasLimitSave::SetSaveTime(timeval tSet)
	{
		m_objTime.Set(tSet);
	}
	void CMeasLimitSave::SetRecoverFlag(ENUM_LIMIT_RECOVER eSet)
	{
		m_byRecoverFlag = eSet;
	}
	void CMeasLimitSave::SetAlarmType(ENUM_ALARM_TYPE eSet)
	{
		m_wAlarmType = eSet;
	}
	void CMeasLimitSave::SetLimitType(ENUM_LIMIT_SAVE_TYPE eSet)
	{
		m_byLimitType = eSet;
	}
	void CMeasLimitSave::SetValue(float fValue)
	{
		m_fValue = fValue;
	}
	void CMeasLimitSave::SetUnit(string strSet)
	{
		m_strUnit = strSet;
	}				
	void CMeasLimitSave::SetDesc(string strSet)
	{
		m_strDesc = strSet;
	}				
	
	WORD CMeasLimitSave::GetStaId()
	{
		return m_wStaId;
	}
	string CMeasLimitSave::GetStaName()
	{
		return m_strStaName;
	}
	DWORD CMeasLimitSave::GetObjId()
	{
		return m_dwObjId;
	}
	string CMeasLimitSave::GetObjName()
	{
		return m_strObjName;
	}
	BYTE CMeasLimitSave::GetVolLevel()
	{
		return m_byLevel;
	}
	CProtocolTime* CMeasLimitSave::GetSaveTime()
	{
		return &m_objTime;
	}
	ENUM_LIMIT_RECOVER CMeasLimitSave::GetRecoverFlag()
	{
		return (ENUM_LIMIT_RECOVER)m_byRecoverFlag;
	}
	ENUM_ALARM_TYPE CMeasLimitSave::GetAlarmType()
	{
		return (ENUM_ALARM_TYPE)m_wAlarmType;
	}
	ENUM_LIMIT_SAVE_TYPE CMeasLimitSave::GetLimitType()
	{
		return (ENUM_LIMIT_SAVE_TYPE)m_byLimitType;
	}
	float CMeasLimitSave::GetValue()
	{
		return m_fValue;
	}
	string CMeasLimitSave::GetUnit()
	{
		return m_strUnit;
	}						
	string CMeasLimitSave::GetDesc()
	{
		return m_strDesc;
	}

	int CMeasLimitSave::StreamSize()
	{
		return 2 + 1+m_strStaName.size() + 4 + 1+m_strObjName.size()
			+ 1 + 8 + 1 + 2 + 1 + 4 + 1+m_strUnit.size() + 1+m_strDesc.size();
	}

	int CMeasLimitSave::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf:Buflen is less than the CMeasLimitSave len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nOffset = 0;

		// 车站ID
		SETSTREAMSHORT(pBuf+nOffset, m_wStaId);
		nOffset += 2;

		// 车站名称
		pBuf[nOffset] = m_strStaName.size();
		nOffset ++;		
		memcpy(pBuf+nOffset, m_strStaName.c_str(), m_strStaName.size());
		nOffset += m_strStaName.size();

		// 对象ID
		SETSTREAMINT(pBuf+nOffset, m_dwObjId);
		nOffset += 4;

		// 对象名称
		pBuf[nOffset] = m_strObjName.size();
		nOffset ++;		
		memcpy(pBuf+nOffset, m_strObjName.c_str(), m_strObjName.size());
		nOffset += m_strObjName.size();

		// 电压等级
		pBuf[nOffset] = m_byLevel;
		nOffset ++;

		// 发生时间
		int nTmp = nBufLen-nOffset;
		int nRet = m_objTime.ToStreamBuf(pBuf+nOffset, nTmp);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 产生恢复标志
		pBuf[nOffset] = m_byRecoverFlag;
		nOffset ++;

		// 报警事件类型
		SETSTREAMSHORT(pBuf + nOffset, m_wAlarmType);
		nOffset += 2;

		// 越限类型
		pBuf[nOffset] = m_byLimitType;
		nOffset ++;

		// 越限值
		SETSTREAMFLOAT(pBuf + nOffset, m_fValue);
		nOffset += 4;

		// 单位
		pBuf[nOffset] = m_strUnit.size();
		nOffset ++;		
		memcpy(pBuf+nOffset, m_strUnit.c_str(), m_strUnit.size());
		nOffset += m_strUnit.size();

		// 描述长度
		pBuf[nOffset] = m_strDesc.size();
		nOffset ++;		
		memcpy(pBuf+nOffset, m_strDesc.c_str(), m_strDesc.size());
		nOffset += m_strDesc.size();
		
		return nOffset;
	}

	int CMeasLimitSave::FromStreamBuf(const char* pBuf, int nBufLen)
	{		
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf:Buflen is less than the CMeasLimitSave len\n");
			return -1;
		}
		
		int nOffset = 0;
		BYTE byLenTmp = 0;
		char cStrTmp[255] = {0};

		// 车站ID
		m_wStaId = GETSTREAMSHORT(pBuf+nOffset);
		nOffset += 2;

		// 车站名称
		byLenTmp = pBuf[nOffset];
		nOffset ++;
		memset(cStrTmp, 0, sizeof(cStrTmp));
		memcpy(cStrTmp, pBuf+nOffset, byLenTmp);
		m_strStaName = cStrTmp;
		nOffset += byLenTmp;

		// 对象ID
		m_dwObjId = GETSTREAMINT(pBuf+nOffset);
		nOffset += 4;

		// 对象名称
		byLenTmp = pBuf[nOffset];
		nOffset ++;
		memset(cStrTmp, 0, sizeof(cStrTmp));
		memcpy(cStrTmp, pBuf+nOffset, byLenTmp);
		m_strObjName = cStrTmp;
		nOffset += byLenTmp;

		// 电压等级
		m_byLevel = pBuf[nOffset];
		nOffset ++;

		// 发生时间
		int nRet = m_objTime.FromStreamBuf(pBuf+nOffset, nBufLen-nOffset);
		if (nRet < 0)
		{
			return -1;
		}
		nOffset += nRet;

		// 产生恢复标志
		m_byRecoverFlag = pBuf[nOffset];
		nOffset ++;

		// 报警事件类型
		m_wAlarmType = GETSTREAMSHORT(pBuf+nOffset);
		nOffset += 2;

		// 越限类型
		m_byLimitType = pBuf[nOffset];
		nOffset ++;

		// 越限值
		GETSTREAMFLOAT(m_fValue, pBuf+nOffset);
		nOffset += 4;

		// 单位
		byLenTmp = pBuf[nOffset];
		nOffset ++;
		memset(cStrTmp, 0, sizeof(cStrTmp));
		memcpy(cStrTmp, pBuf+nOffset, byLenTmp);
		m_strUnit = cStrTmp;
		nOffset += byLenTmp;

		// 描述长度
		byLenTmp = pBuf[nOffset];
		nOffset ++;
		memset(cStrTmp, 0, sizeof(cStrTmp));
		memcpy(cStrTmp, pBuf+nOffset, byLenTmp);
		m_strDesc = cStrTmp;
		nOffset += byLenTmp;
		
		return nOffset;
	}

	//==============================================================================
	//
	//								CMeasLimitSavePkt
	//						遥测越限信息存储包【AppMeasureMgr→OSM模块】
	//
	//==============================================================================
	CMeasLimitSavePkt::CMeasLimitSavePkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASLIMIT_SVRTOOSM)
	{
		m_vecSave.clear();
	}

	CMeasLimitSavePkt::~CMeasLimitSavePkt()
	{

	}

	void CMeasLimitSavePkt::Add(CMeasLimitSave& objAdd)
	{
		m_vecSave.push_back(objAdd);
	}
	
	WORD CMeasLimitSavePkt::GetSaveCount() const
	{
		return (WORD)m_vecSave.size();
	}

	CMeasLimitSave* CMeasLimitSavePkt::GetSave(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSave.size())
		{
			return NULL;
		}

		return &m_vecSave.at(nIndex);
	}

	void CMeasLimitSavePkt::Clear()
	{
		m_vecSave.clear();
	}

	ProtocolBase::CProtocolHead* CMeasLimitSavePkt::Clone()
	{
		CMeasLimitSavePkt* pClone = new CMeasLimitSavePkt();
		*pClone = *this;
		return pClone;
	}

	int CMeasLimitSavePkt::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecSave.size(); i++)
		{
			nSize += m_vecSave.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + nSize;
	}

	int CMeasLimitSavePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasLimitSavePkt Pkt len\n");
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

		// 数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSave.size());
		nOffset += 2;

		// 越限信息
		for(int i=0; i<m_vecSave.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSave.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CMeasLimitSavePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasLimitSavePkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 数量
		int nMeasureStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 越限信息
		for (int i=0; i<nMeasureStaNum; i++)
		{
			CMeasLimitSave objMeasure;
			int nLenTmp = nBufLen - nOffset;
			nRet = objMeasure.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSave.push_back(objMeasure);
			nOffset += nRet;
		}

		return nOffset;
	}


	//==============================================================================
	//
	//								CMeasureSaveReplyPkt
	//						遥测信息存储回执包【OSM模块→AppMeasureMgr】
	//
	//==============================================================================
	CMeasureSaveReplyPkt::CMeasureSaveReplyPkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASSAVE_OSMTOSVR)
		, m_dwRst(0)
		, m_strDesc("")
	{

	}

	CMeasureSaveReplyPkt::~CMeasureSaveReplyPkt()
	{

	}

	void CMeasureSaveReplyPkt::SetRst(DWORD dwRst)
	{
		m_dwRst = dwRst;
	}

	void CMeasureSaveReplyPkt::SetDesc(string strDesc)
	{
		m_strDesc = strDesc;
	}

	DWORD CMeasureSaveReplyPkt::GetRst() const
	{
		return m_dwRst;
	}

	string CMeasureSaveReplyPkt::GetDesc() const
	{
		return m_strDesc;
	}

	ProtocolBase::CProtocolHead* CMeasureSaveReplyPkt::Clone()
	{
		CMeasureSaveReplyPkt* pClone = new CMeasureSaveReplyPkt();
		*pClone = *this;
		return pClone;
	}

	int CMeasureSaveReplyPkt::StreamSize()
	{
		return (int)(CProtocolHead::HeaderSize() + 4 + 2 + m_strDesc.size());
	}

	int CMeasureSaveReplyPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureSaveReplyPkt len\n");
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

		// 接收结果
		SETSTREAMINT(pBuf + nOffset, m_dwRst);
		nOffset += 4;

		// 描述长度
		WORD wDescLen = (WORD)MIN(65535, m_strDesc.size());
		SETSTREAMSHORT(pBuf + nOffset, wDescLen);
		nOffset += 2;

		// 描述
		memcpy(pBuf + nOffset, m_strDesc.c_str(), wDescLen);
		nOffset += wDescLen;

		return nOffset;
	}

	int CMeasureSaveReplyPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureSaveReplyPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 接收结果
		m_dwRst = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 描述长度
		int nDescLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 描述
		char cDesc[65535 + 1] = {0};
		int nDescLenTmp = MIN(65535, nDescLen);
		memcpy(cDesc, pBuf + nOffset, nDescLenTmp);
		cDesc[nDescLenTmp] = '\0';
		m_strDesc = cDesc;
		nOffset += nDescLen;

		return nOffset;
	}

	//==============================================================================
	//
	//								CMeasureHis
	//						遥测历史信息查询【操作台→OSM模块】
	//
	//==============================================================================
	CMeasureHis::CMeasureHis()
		: m_wStaId(0)
	{
		m_vecMessure.clear();
	}

	CMeasureHis::~CMeasureHis()
	{

	}

	void CMeasureHis::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	WORD CMeasureHis::GetStaId() const
	{
		return m_wStaId;
	}

	void CMeasureHis::Add(DWORD dwMeasure)
	{
		m_vecMessure.push_back(dwMeasure);
	}

	WORD CMeasureHis::GetMeasureCount() const
	{
		return (WORD)(m_vecMessure.size());
	}

	DWORD CMeasureHis::GetMeasure(int nIndex) const
	{

		return m_vecMessure.at(nIndex);
	}

	int CMeasureHis::StreamSize()
	{		
		return (int)(2 + 2 + m_vecMessure.size() * 4);
	}

	int CMeasureHis::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureHis len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 遥测数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecMessure.size());
		nOffset += 2;

		// 遥测
		for (int i=0; i<m_vecMessure.size(); i++)
		{
			SETSTREAMINT(pBuf + nOffset, m_vecMessure.at(i));
			nOffset += 4;
		}

		return nOffset;
	}

	int CMeasureHis::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureHis size len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥测数量
		int nMeasureLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥测
		for (int i=0; i<nMeasureLen; i++)
		{
			DWORD dwMeasure = GETSTREAMINT(pBuf + nOffset);
			m_vecMessure.push_back(dwMeasure);
			nOffset += 4;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//								CMeasureHisPkt
	//						遥测历史信息查询包【操作台→OSM模块】
	//
	//==============================================================================
	CMeasureHisPkt::CMeasureHisPkt()
		: CProtocolHead(TYPE_MEASURE_INFO, FUNC_MEASHIS_TERMTOOSM)
	{
		m_vecSta.clear();
	}

	CMeasureHisPkt::~CMeasureHisPkt()
	{

	}

	void CMeasureHisPkt::SetStartTime(time_t t, WORD wMilliSec)
	{
		m_StartTime.Set(t, wMilliSec);
	}

	void CMeasureHisPkt::SetEndTime(time_t t, WORD wMilliSec)
	{
		m_EndTime.Set(t, wMilliSec);
	}

	timeval CMeasureHisPkt::GetStartTime() const
	{
		return m_StartTime.GetTimeval();
	}

	timeval CMeasureHisPkt::GetEndTime() const
	{
		return m_EndTime.GetTimeval();
	}

	void CMeasureHisPkt::Add(CMeasureHis His)
	{
		m_vecSta.push_back(His);
	}

	CMeasureHis* CMeasureHisPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	WORD CMeasureHisPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	int CMeasureHisPkt::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return (int)(CProtocolHead::HeaderSize() + m_StartTime.StreamSize() 
			+ m_EndTime.StreamSize() + 2 + nSize);
	}

	int CMeasureHisPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMeasureHisPkt len\n");
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

		// 起始时间
		int nLenTmp = nBufLen - nOffset;
		nRet = m_StartTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 终止时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_EndTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
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
			nLenTmp  = nBufLen - nOffset;
			nRet = m_vecSta.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}
		return nOffset;
	}

	int CMeasureHisPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMeasureHisPkt size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 起始时间
		int nLenTmp = nBufLen - nOffset;
		nRet = m_StartTime.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 终止时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_EndTime.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		int nStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i=0; i<nStaNum; i++)
		{
			CMeasureHis objHis;
			nLenTmp = nBufLen - nOffset;
			nRet = objHis.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objHis);
			nOffset += nRet;
		}

		return nOffset;
	}

	ProtocolBase::CProtocolHead* CMeasureHisPkt::Clone()
	{
		CMeasureHisPkt* pClone = new CMeasureHisPkt();
		*pClone = *this;
		return pClone;
	}

	//==============================================================================
	//
	//								CMeasureHisReplyPkt
	//						遥测信息查询回执包【OSM模块→操作台】
	//
	//==============================================================================
	CMeasureHisReplyPkt::CMeasureHisReplyPkt()
	{
		m_byType = TYPE_MEASURE_INFO;
		m_byFunc = FUNC_MEASHIS_OSMTOTERM;
	}

	CMeasureHisReplyPkt::~CMeasureHisReplyPkt()
	{

	}

	//==============================================================================
	//
	//						请求遥测信息【操作台→AppMeasureMgr】
	//
	//==============================================================================

	//==============================================================================
	//
	//									CMeasureReqPkt
	//						请求遥测信息包【操作台→AppMeasureMgr】
	//
	//==============================================================================
	CMeasureReqPkt::CMeasureReqPkt()
	{
		m_byType = TYPE_MEASURE_INFO;
		m_byFunc = FUNC_MEASREQ_TERMTOSVR;
	}

	CMeasureReqPkt::~CMeasureReqPkt()
	{

	}

	//==============================================================================
	//
	//								CMeasureCallPkt
	//						遥测总召信息包【InterSvr104/101→AppMeasureMgr】
	//
	//==============================================================================
	CMeasureCallPkt::CMeasureCallPkt()
	{
		m_byType = TYPE_MEASURE_INFO;
		m_byFunc = FUNC_MEACALL_INTERTOSVR;
	}

	CMeasureCallPkt::~CMeasureCallPkt()
	{

	}

	//==============================================================================
	//
	//								CMeasureQueryFailRepPkt
	//						遥测信息查询失败回执包【OSM模块→操作台】
	//
	//==============================================================================
	CMeasureQueryFailRepPkt::CMeasureQueryFailRepPkt()
	{
		m_byType = TYPE_MEASURE_INFO;
		m_byFunc = FUNC_MEASHIS_OSMFAILREPLY;
	}
	
	CMeasureQueryFailRepPkt::~CMeasureQueryFailRepPkt()
	{
		
	}
	
	
}