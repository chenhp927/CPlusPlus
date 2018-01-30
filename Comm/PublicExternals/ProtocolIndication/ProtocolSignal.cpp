//=========================================================================================
//	
//							 ProtocolSignal.cpp
//							   
// begin    : 2016-08-24
// describe : 协议遥信类
//
//=========================================================================================

#include "ProtocolSignal.h"
#include <string.h>

namespace ProtocolIndication
{
	using namespace ProtocolBase;

	//==============================================================================
	//
	//						遥信全体信息【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalCollAll
	//					全体信息包中遥信点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CStaCollAll::CStaCollAll()
	{
		m_wStationId = 0;
		m_vecSignal.clear();

		m_wRealSigCnt = 0;
		m_dwSigStartAddr = 0;
		m_strData = "";
	}

	CStaCollAll::~CStaCollAll()
	{

	}

	void CStaCollAll::SetStaId(WORD wStationId)
	{
		m_wStationId = wStationId;
	}

	WORD CStaCollAll::GetStaId() const
	{
		return m_wStationId;
	}

	void CStaCollAll::SetSigStartAddr(DWORD dwStart)
	{
		m_dwSigStartAddr = dwStart;
	}

	DWORD CStaCollAll::GetSigStartAddr() const
	{
		return m_dwSigStartAddr;
	}

	WORD CStaCollAll::GetSignalCount() const
	{
		return (WORD)m_vecSignal.size();
	}

	BYTE CStaCollAll::GetSignal(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecSignal.size());
		return m_vecSignal.at(nIndex);
	}

	void CStaCollAll::AddSignal(BYTE signal)
	{
		m_vecSignal.push_back(signal);
	}
	
	void CStaCollAll::Clear()
	{
		m_vecSignal.clear();
	}

	WORD CStaCollAll::GetSigRealCnt() const
	{
		return m_wRealSigCnt;
	}

	string CStaCollAll::GetAllSigs() const
	{
		return m_strData;
	}
	
	void CStaCollAll::SetAllSigs( string strData )
	{
		m_wRealSigCnt = strData.length() / PROTOCOL_SIGNALVALUE_LEN;
		m_strData = strData;
	}

	int CStaCollAll::StreamSize()
	{
		return (int)(PROTOCOL_SIGNALSTAID_LEN + PROTOCAL_SIGNALADDR_LEN + 2 
			+ PROTOCOL_SIGNALVALUE_LEN*m_vecSignal.size()); // 车站id + 遥信起始地址 + 遥信数量 + 遥信数量 * 遥信值
	}

	int CStaCollAll::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaCollAll signal Sta len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf, m_wStationId);
		nOffset += 2;

		// 遥信起始地址
		SETSTREAMINT(pBuf + nOffset, m_dwSigStartAddr);
		nOffset += PROTOCAL_SIGNALADDR_LEN;
		
		// 遥信数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSignal.size());
		nOffset += 2;

		// 遥信值
		for(int i = 0; i < m_vecSignal.size(); i++)
		{
			pBuf[nOffset] = m_vecSignal.at(i);
			nOffset += PROTOCOL_SIGNALVALUE_LEN;
		}

		return nOffset; 
	}

	int CStaCollAll::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaCollAll len\n");

			return -1;
		}

		int nOffset = 0;
		m_wStationId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// 遥信起始地址
		m_dwSigStartAddr = GETSTREAMINT(pBuf + nOffset);
		nOffset += PROTOCAL_SIGNALADDR_LEN;

		// 遥信数量
		int nSignalNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// 遥信值
		for (int i = 0; i < nSignalNum; i++)
		{
			m_vecSignal.push_back((BYTE)pBuf[nOffset]);
			nOffset += PROTOCOL_SIGNALVALUE_LEN; 
		}

		return nOffset;
	}
	
	int CStaCollAll::StreamSize2()
	{
		return (int)(PROTOCOL_SIGNALSTAID_LEN + PROTOCAL_SIGNALADDR_LEN + 2
			+ PROTOCOL_SIGNALVALUE_LEN*m_wRealSigCnt); // 车站id + 遥信起始地址 + 遥信数量 + 遥信数量 * 遥信值
	}

	int CStaCollAll::ToStreamBuf2( char* pBuf, int& nBufLen )
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf2: Buflen is less than the CStaCollAll signal Sta len\n");
			
			nBufLen = StreamSize();
			return -1;
		}
		
		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStationId);
		nOffset += 2;

		// 遥信起始地址
		SETSTREAMINT(pBuf + nOffset, m_dwSigStartAddr);
		nOffset += PROTOCAL_SIGNALADDR_LEN;
		
		// 遥信数量
		SETSTREAMSHORT(pBuf + nOffset, m_wRealSigCnt);
		nOffset += 2;
		
		// 遥信值
		memcpy(pBuf + nOffset, m_strData.data(), m_wRealSigCnt*PROTOCOL_SIGNALVALUE_LEN);
		nOffset += m_wRealSigCnt*PROTOCOL_SIGNALVALUE_LEN;		
		
		return nOffset;
	}
	
	int CStaCollAll::FromStreamBuf2( const char* pBuf, int nBufLen )
	{
		// 车站Id
		if (nBufLen < PROTOCOL_SIGNALSTAID_LEN)
		{
			LogImportant("FromStreamBuf2: Buflen is less than the CStaCollAll stationId len\n");
			return -1;
		}
		
		int nOffset = 0;
		m_wStationId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// 遥信起始地址
		m_dwSigStartAddr = GETSTREAMINT(pBuf + nOffset);
		nOffset += PROTOCAL_SIGNALADDR_LEN;
		
		// 遥信数量
		if (nBufLen - nOffset < nOffset + PROTOCOL_SIGNALNUM_LEN)
		{
			LogImportant("FromStreamBuf2: Buflen is less than the CStaCollAll signal num len\n");
			return -1;
		}
		m_wRealSigCnt = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// 遥信值
		if (nBufLen - nOffset < m_wRealSigCnt*PROTOCOL_SIGNALVALUE_LEN)
		{
			LogImportant("FromStreamBuf2: Buflen is less than the CStaCollAll signal num\n");
			return -1;
		}

		string strData = "";
		strData.resize(m_wRealSigCnt*PROTOCOL_SIGNALVALUE_LEN);
		memcpy((char*)strData.data(), pBuf + nOffset, m_wRealSigCnt*PROTOCOL_SIGNALVALUE_LEN);
		m_strData = strData;
		nOffset += m_wRealSigCnt*PROTOCOL_SIGNALVALUE_LEN; 

		return nOffset;
	}

	//==============================================================================
	//	
	//									CSignalCollALLPkt
	//						遥信全体信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CSignalCollAllPkt::CSignalCollAllPkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SIGNALALL_INTERTOSVR)
	{				
		m_vecSta.clear();
	}

	CSignalCollAllPkt::~CSignalCollAllPkt()
	{

	}
	
	WORD CSignalCollAllPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}
	
	CStaCollAll* CSignalCollAllPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	void CSignalCollAllPkt::Add(CStaCollAll& sta)
	{
		m_vecSta.push_back(sta);
	}
	
	void CSignalCollAllPkt::Clear()
	{
		m_vecSta.clear();
	}	

	ProtocolBase::CProtocolHead* CSignalCollAllPkt::Clone()
	{
		CSignalCollAllPkt* pClone = new CSignalCollAllPkt();
		*pClone = *this;
		return pClone;
	}

	int CSignalCollAllPkt::StreamSize()
	{
		int pktHeadLen = CProtocolHead::HeaderSize();
		int nStationSize = 0;

		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize();
		}

		return pktHeadLen + PROTOCOL_SIGNALSTANUM_LEN + nStationSize; // 包头长度  +　车站数量 +  车站size
	}

	int CSignalCollAllPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalCollALLPkt Pkt len\n");
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
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
		for (int i = 0; i < m_vecSta.size(); i++)
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

	int CSignalCollAllPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalCollALLPkt size len\n");
			return -1;
		}

		// 包头
		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen); 
		nOffset += nRet;

		// 车站数量
		int nStationNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
		for (int i = 0; i < nStationNum; i++)
		{
			CStaCollAll objStation;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objStation.FromStreamBuf(pBuf + nOffset, nLenTmp);	
			if (-1 == nRet)
			{
				return -1;
			}
			
			m_vecSta.push_back(objStation);
			nOffset += nRet;
		}

		return nOffset;
	}
	
	int CSignalCollAllPkt::StreamSize2()
	{
		int pktHeadLen = CProtocolHead::HeaderSize();
		int nStationSize = 0;
		
		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize2();
		}
		
		return pktHeadLen + PROTOCOL_SIGNALSTANUM_LEN + nStationSize; // 包头长度  +　车站数量 +  车站size
	}

	int CSignalCollAllPkt::ToStreamBuf2( char* pBuf, int& nBufLen )
	{
		if (nBufLen < StreamSize2())
		{
			LogImportant("ToStreamBuf2: Buflen is less than the CSignalCollALLPkt Pkt len\n");	
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
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;
		
		// 车站信息
		for (int i = 0; i < m_vecSta.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			nRet = m_vecSta.at(i).ToStreamBuf2(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			
			nOffset += nRet;
		}
		
		return nOffset;
	}
	
	int CSignalCollAllPkt::FromStreamBuf2( const char* pBuf, int nBufLen )
	{
		// 包头
		if (nBufLen < StreamSize2())
		{
			LogImportant("FromStreamBuf2: Buflen is less than the CSignalCollALLPkt size len\n");
			return -1;
		}
		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen); 
		nOffset += nRet;
		
		// 车站数量
		int nStationNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;
		
		// 车站信息
		for (int i = 0; i < nStationNum; i++)
		{
			CStaCollAll objStation;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objStation.FromStreamBuf2(pBuf + nOffset, nLenTmp);	
			if (-1 == nRet)
			{
				return -1;
			}
			
			m_vecSta.push_back(objStation);
			nOffset += nRet;
		}
		
		return nOffset;
	}

	//==============================================================================
	//
	//						遥信变化信息【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalCollChange
	//						遥信变化信息中遥信点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CSignalCollChange::CSignalCollChange() :
		m_dwSignalAddr(0),m_byValue(0)
	{

	}

	CSignalCollChange::CSignalCollChange(DWORD wSignalAddr, BYTE byValue)
	{
		m_dwSignalAddr = wSignalAddr;
		m_byValue = byValue;
	}

	CSignalCollChange::~CSignalCollChange()
	{

	}

	void CSignalCollChange::SetAddr(DWORD dwSignalAddr)
	{
		m_dwSignalAddr = dwSignalAddr;
	}

	DWORD CSignalCollChange::GetAddr() const
	{
		return m_dwSignalAddr;
	}

	void CSignalCollChange::SetValue(BYTE byValue)
	{
		m_byValue = byValue;
	}

	BYTE CSignalCollChange::GetValue() const
	{
		return m_byValue;
	}

	int CSignalCollChange::StreamSize()
	{
		return PROTOCAL_SIGNALADDR_LEN + PROTOCOL_SIGNALVALUE_LEN;
	}

	int CSignalCollChange::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalCollChange len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 遥信点地址
		SETSTREAMINT(pBuf, m_dwSignalAddr);
		nOffset += 4;

		// 遥信值
		pBuf[nOffset] = m_byValue;
		nOffset += 1;

		return nOffset;
	}

	int CSignalCollChange::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalCollChange signal value len\n");
			return -1;
		}

		// 遥信点地址
		int nOffset = 0;
		m_dwSignalAddr = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 遥信值
		m_byValue = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//==============================================================================
	//	
	//									CStaCollChange
	//						遥信变化信息中厂站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CStaCollChange::CStaCollChange()
	{
		m_wStationId = 0;
		m_vecSignal.clear();
	}

	CStaCollChange::~CStaCollChange()
	{

	}

	void CStaCollChange::SetId(WORD wStationId)
	{
		m_wStationId = wStationId;
	}

	WORD CStaCollChange::GetId() const
	{ 
		return m_wStationId;
	}
	
	WORD CStaCollChange::GetSignalCount() const
	{
		return (WORD)m_vecSignal.size();
	}

	CSignalCollChange* CStaCollChange::GetSignal(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSignal.size())
		{
			return NULL;
		}

		return &m_vecSignal.at(nIndex);	
	}

	void CStaCollChange::Add(CSignalCollChange &signal)
	{
		m_vecSignal.push_back(signal);
	}
	
	void CStaCollChange::Clear()
	{
		m_vecSignal.clear();
	}
	
	int CStaCollChange::StreamSize()
	{
		return (int)(PROTOCOL_SIGNALSTAID_LEN + PROTOCOL_SIGNALNUM_LEN
			+ (CSignalCollChange::StreamSize()) * (m_vecSignal.size())); // 车站id + 遥信数量 + 遥信数量 * 遥信点大小
	}

	int CStaCollChange::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaCollChange Sta len\n");

			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf, m_wStationId);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// 遥信数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSignal.size());
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// 遥信值
		for(int i = 0; i < m_vecSignal.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSignal.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);		
			if (-1 == nRet)
			{
				return -1;
			}
			
			nOffset += nRet;
		}

		return nOffset; 
	}

	int CStaCollChange::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < PROTOCOL_SIGNALSTAID_LEN)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaCollChange staId len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStationId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;
	
		// 遥信数量
		if (nBufLen < nOffset + PROTOCOL_SIGNALNUM_LEN)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaCollChange signal num len\n");
			return -1;
		}

		int nVecSize = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// 遥信点地址
		for (int i = 0; i < nVecSize; i++)
		{
			CSignalCollChange objSignal;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objSignal.FromStreamBuf(pBuf + nOffset, nLenTmp);	
			if (nRet < 0)
			{
				return -1;
			}

			m_vecSignal.push_back(objSignal);
			nOffset += nRet; 
		}

		return nOffset;
	}

	//==============================================================================
	//	
	//									CSignalCollChangePkt
	//						遥信变化信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CSignalCollChangePkt::CSignalCollChangePkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SIGNALCHG_INTERTOSVR)
	{
		m_vecSta.clear();
	}

	CSignalCollChangePkt::~CSignalCollChangePkt()
	{
	}

	WORD CSignalCollChangePkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}
	
	CStaCollChange* CSignalCollChangePkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	void CSignalCollChangePkt::Add(CStaCollChange &sta)
	{
		m_vecSta.push_back(sta);
	}
	
	void CSignalCollChangePkt::Clear()
	{
		m_vecSta.clear();
	}
	
	ProtocolBase::CProtocolHead* CSignalCollChangePkt::Clone()
	{
		CSignalCollChangePkt* pClone = new CSignalCollChangePkt();
		*pClone = *this;
		return pClone;
	}

	int CSignalCollChangePkt::StreamSize()
	{
		int pktHeadLen = CProtocolHead::StreamSize();
		int nStationSize = 0;
	
		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize();
		}

		return pktHeadLen + PROTOCOL_SIGNALSTANUM_LEN + nStationSize; // 包头长度 +　车站数量 +  车站size
	}

	int CSignalCollChangePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalCollChangePkt Pkt len\n");
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
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
		for (int i = 0; i < m_vecSta.size(); i++)
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

	int CSignalCollChangePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalCollChangePkt size len\n");
			return -1;
		}
		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		nOffset += nRet;

		// 车站数量		
		int nStationNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
		for (int i = 0; i < nStationNum; i++)
		{
			CStaCollChange objStation;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objStation.FromStreamBuf(pBuf + nOffset, nLenTmp);
			
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objStation);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//							遥信全体信息【AppSignalMgr→操作台】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalOfSignalAllInfo
	//						遥信全体信息包中遥信点【AppSignalMgr→操作台】
	//
	//==============================================================================
	CSignalOfSignalAllInfo::CSignalOfSignalAllInfo() 
		: m_dwSignalId(0)
		, m_byQltFlag(0)
		, m_byValue(0)
	{
	}

	CSignalOfSignalAllInfo::~CSignalOfSignalAllInfo()
	{

	}

	void CSignalOfSignalAllInfo::SetSignalId(DWORD dwId)
	{
		m_dwSignalId = dwId;
	}

	DWORD CSignalOfSignalAllInfo::GetSignalId() const
	{
		return m_dwSignalId;
	}

	void CSignalOfSignalAllInfo::SetQltFalg(ENUM_QUALITY_FLAG_TYPE eFlag)
	{
		m_byQltFlag = eFlag;
	}

	ENUM_QUALITY_FLAG_TYPE CSignalOfSignalAllInfo::GetQltFlag() const
	{
		return (ENUM_QUALITY_FLAG_TYPE)m_byQltFlag;
	}

	void CSignalOfSignalAllInfo::SetValue(BYTE byValue)
	{
		m_byValue = byValue;
	}

	BYTE CSignalOfSignalAllInfo::GetValue() const
	{
		return m_byValue;
	}

	int CSignalOfSignalAllInfo::StreamSize()
	{
		return 4 + 1 + 1;
	}

	int CSignalOfSignalAllInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalOfSignalAllInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 遥信点Id
		int nOffset = 0;
		SETSTREAMINT(pBuf + nOffset, m_dwSignalId);
		nOffset += 4;

		// 质量标志
		pBuf[nOffset] = m_byQltFlag;
		nOffset += 1;

		// 遥信值
		pBuf[nOffset] = m_byValue;
		nOffset += 1;

		return nOffset;
	}

	int CSignalOfSignalAllInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
	
		// 遥信点Id
		if (nBufLen < nOffset + 4)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalOfSignalAllInfo signal id len\n");
			return -1;
		}

		m_dwSignalId = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 质量标志
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalOfSignalAllInfo quality flag len\n");
			return -1;
		}

		m_byQltFlag = pBuf[nOffset];
		nOffset += 1;

		// 遥信值
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalOfSignalAllInfo quality flag len\n");
			return -1;
		}

		m_byValue = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//==============================================================================
	//	
	//										CStaOfSignalAllInfo
	//							遥信全体信息包中车站【AppSignalMgr→操作台】
	//
	//==============================================================================
	CStaOfSignalAllInfo::CStaOfSignalAllInfo()
		: m_wId(0)
	{
		m_vecSignal.clear();
	}

	CStaOfSignalAllInfo::CStaOfSignalAllInfo(WORD wId)
		: m_wId(wId)
	{
		m_vecSignal.clear();
	}

	CStaOfSignalAllInfo::~CStaOfSignalAllInfo()
	{

	}

	void CStaOfSignalAllInfo::SetId(WORD wId)
	{
		m_wId = wId;
	}

	WORD CStaOfSignalAllInfo::GetId() const
	{
		return m_wId;
	}

	WORD CStaOfSignalAllInfo::GetSignalCount() const
	{
		return (WORD)m_vecSignal.size();
	}

	CSignalOfSignalAllInfo* CStaOfSignalAllInfo::GetSignal(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSignal.size())
		{
			return NULL;
		}

		return &m_vecSignal.at(nIndex);
	}

	void CStaOfSignalAllInfo::Add(CSignalOfSignalAllInfo &signal)
	{
		m_vecSignal.push_back(signal);
	}

	void CStaOfSignalAllInfo::Clear()
	{
		m_vecSignal.clear();
	}

	int CStaOfSignalAllInfo::StreamSize()
	{
		return (int)(2 + 2 + (CSignalOfSignalAllInfo::StreamSize()) * (m_vecSignal.size())); // 车站id + 遥信数量 + 遥信数量 * 遥信点大小
	}

	int CStaOfSignalAllInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaOfSignalAllInfo len\n");

			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf, m_wId);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// 遥信数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSignal.size());
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// 遥信
		for(int i = 0; i < m_vecSignal.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSignal.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset; 
	}

	int CStaOfSignalAllInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站
		if (nBufLen < 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaOfSignalAllInfo sta id len\n");
			return -1;
		}

		int nOffset = 0;
		m_wId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// 遥信数量
		if (nBufLen < nOffset + PROTOCOL_SIGNALNUM_LEN)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaOfSignalAllInfo signal num len\n");
			return -1;
		}

		int nVecSize = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// 遥信值
		for (int i = 0; i < nVecSize; i++)
		{
			CSignalOfSignalAllInfo objSignal;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objSignal.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (nRet < 0)
			{
				return -1;
			}

			m_vecSignal.push_back(objSignal);
			nOffset += nRet; 
		}

		return nOffset;;
	}

	//==============================================================================
	//	
	//										CSignalAllPkt
	//							遥信全体信息包【AppSignalMgr→操作台】
	//
	//==============================================================================
	CSignalAllPkt::CSignalAllPkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SIGNALALL_SVRTOTERM)
	{
		m_vecSta.clear();
	}

	CSignalAllPkt::~CSignalAllPkt()
	{

	}
	
	WORD CSignalAllPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CStaOfSignalAllInfo* CSignalAllPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	void CSignalAllPkt::Add(CStaOfSignalAllInfo &sta)
	{
		m_vecSta.push_back(sta);
	}

	ProtocolBase::CProtocolHead* CSignalAllPkt::Clone()
	{
		CSignalAllPkt* pClone = new CSignalAllPkt();
		*pClone = *this;
		return pClone;
	}

	int CSignalAllPkt::StreamSize()
	{
		int pktHeadLen = CProtocolHead::StreamSize();
		int nStationSize = 0;

		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize();
		}

		return pktHeadLen + 2 + nStationSize; // 包头长度 +　车站数量 +  车站size

	}

	int CSignalAllPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalAllPkt Pkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();
		int nOffset = 0;

		// 包头
		int nRet = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += 2;

		// 车站
		for (int i = 0; i < m_vecSta.size(); i++)
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

	int CSignalAllPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalAllPkt size len\n");
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
		int nStationNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i = 0; i < nStationNum; i++)
		{
			CStaOfSignalAllInfo objStation;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objStation.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSta.push_back(objStation);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//							遥信变化信息【AppSignalMgr→操作台】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalChangePkt
	//							遥信变化信息包【AppSignalMgr→操作台】
	//
	//==============================================================================
	CSignalChangePkt::CSignalChangePkt()
	{
		m_byType = TYPE_SIGNAL_INFO;
		m_byFunc = FUNC_SIGNALCHG_SVRTOTERM;
	}

	CSignalChangePkt::~CSignalChangePkt()
	{

	}

	//==============================================================================
	//
	//							遥信请求信息【AppSignalMgr→InterSvr104】
	//
	//==============================================================================
	//==============================================================================
	//
	//									CSignalCollReqPkt
	//							遥信请求信息包【AppSignalMgr→InterSvr104】
	//
	//==============================================================================
	CSignalCollReqPkt::CSignalCollReqPkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SIGNALREQ_SVRTOINTER)
	{
		m_vecSta.clear();
	}

	CSignalCollReqPkt::~CSignalCollReqPkt()
	{

	}
	
	WORD CSignalCollReqPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}
	
	WORD CSignalCollReqPkt::GetSta(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecSta.size());
		return m_vecSta.at(nIndex);
	}

	void CSignalCollReqPkt::Add(WORD sta)
	{
		m_vecSta.push_back(sta);
	}

	ProtocolBase::CProtocolHead* CSignalCollReqPkt::Clone()
	{
		CSignalCollReqPkt* pClone = new CSignalCollReqPkt();
		*pClone = *this;
		return pClone;
	}

	int CSignalCollReqPkt::StreamSize()
	{
		return (int)(CProtocolHead::StreamSize() + 2 + m_vecSta.size() * 2);
	}

	int CSignalCollReqPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalCollReqPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		// 包头
		int nOffset = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf+nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

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
	
	int CSignalCollReqPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < CProtocolHead::HeaderSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalCollReqPkt head size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf + nOffset, nBufLen);
		nOffset += nRet;

		// 车站数量
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalCollChangePkt sta num len\n");
			return -1;
		}

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
	//							遥信请求信息【操作台→AppSignalMgr】
	//
	//==============================================================================
	//==============================================================================
	//
	//									CSignalReqPkt
	//							遥信请求信息包【操作台→AppSignalMgr】
	//
	//==============================================================================
	CSignalReqPkt::CSignalReqPkt()
	{
		m_byType = TYPE_SIGNAL_INFO;
		m_byFunc = FUNC_SIGNALREQ_TERMTOSVR;
	}

	CSignalReqPkt::~CSignalReqPkt()
	{

	}

	//==============================================================================
	//
	//							遥信变位信息【AppSignalMgr→实时通道】
	//
	//==============================================================================
	//==============================================================================
	//
	//									CSignalChangeState
	//						遥信变位信息中遥信点【AppSignalMgr→实时通道】
	//
	//==============================================================================
	CSignalChangeState::CSignalChangeState()
		: m_dwId(0), m_byValue(0)
	{

	}

	CSignalChangeState::CSignalChangeState(DWORD dwId, BYTE byValue)
		: m_dwId(dwId), m_byValue(byValue)
	{

	}
	
	CSignalChangeState::~CSignalChangeState()
	{

	}

	void CSignalChangeState::SetId(DWORD dwId)
	{
		m_dwId = dwId;
	}

	DWORD CSignalChangeState::GetId() const
	{
		return m_dwId;
	}

	void CSignalChangeState::SetValue(BYTE byValue)
	{
		m_byValue = byValue;
	}

	BYTE CSignalChangeState::GetValue() const
	{
		return m_byValue;
	}

	int CSignalChangeState::StreamSize()
	{
		return 4 + 1;
	}

	int CSignalChangeState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalChangeState len\n");

			nBufLen = StreamSize();
			return -1;
		}
		
		// 遥信点Id
		int nOffset = 0;
		SETSTREAMINT(pBuf, m_dwId);	
		nOffset += 4;

		// 遥信值
		pBuf[nOffset] = m_byValue;
		nOffset += 1;

		return nOffset;
	}

	int CSignalChangeState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalChangeState signal len\n");
			return -1;
		}

		// 遥信点Id
		int nOffset = 0;
		m_dwId = GETSTREAMINT(pBuf);
		nOffset += 4;

		// 遥信值
		m_byValue = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//==============================================================================
	//
	//									CSignalChangeSta
	//						遥信变位信息车站【AppSignalMgr→实时通道】
	//
	//==============================================================================
	CSignalChangeSta::CSignalChangeSta()
		: m_wId(0)
	{
		m_vecSignal.clear();
	}

	CSignalChangeSta::CSignalChangeSta(WORD wId)
		: m_wId(wId)
	{
		m_vecSignal.clear();
	}

	CSignalChangeSta::~CSignalChangeSta()
	{

	}

	void CSignalChangeSta::SetId(WORD wId)
	{
		m_wId = wId;
	}

	WORD CSignalChangeSta::GetId() const
	{
		return m_wId;
	}

	WORD CSignalChangeSta::GetSignalCount() const
	{ 
		return (WORD)m_vecSignal.size();
	}

	CSignalChangeState* CSignalChangeSta::GetSignal(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSignal.size())
		{
			return NULL;
		}

		return &m_vecSignal.at(nIndex);
	}

	void CSignalChangeSta::Add(CSignalChangeState &signal)
	{
		m_vecSignal.push_back(signal);
	}
	
	void CSignalChangeSta::Clear()
	{
		m_vecSignal.clear();	
	}
	
	int CSignalChangeSta::StreamSize()
	{
		return (int)(2 + 2 + m_vecSignal.size() * CSignalChangeState::StreamSize());
	}

	int CSignalChangeSta::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalChangeSta len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf, m_wId);
		nOffset += 2;

		// 遥信数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSignal.size());
		nOffset += 2; 

		// 遥信
		for(int i=0; i<m_vecSignal.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSignal.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CSignalChangeSta::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalChangeSta len\n");

			return -1;
		}

		int nOffset = 0;
		m_wId = GETSTREAMSHORT(pBuf);
		nOffset += 2;

		// 遥信数量
		int nNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥信点
		for(int i=0; i<nNum; i++)
		{
			CSignalChangeState objSignal;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objSignal.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			m_vecSignal.push_back(objSignal);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//								CSignalChangeStatePkt
	//						遥信变位信息包【AppSignalMgr→实时通道】
	//
	//==============================================================================
	CSignalChangeStatePkt::CSignalChangeStatePkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SIGNALCHGSTATE_SVRTOOTHER)
	{
		m_vecSta.clear();
	}

	CSignalChangeStatePkt::~CSignalChangeStatePkt()
	{

	}

	WORD CSignalChangeStatePkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CSignalChangeSta* CSignalChangeStatePkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	void CSignalChangeStatePkt::Add(CSignalChangeSta &sta)
	{
		m_vecSta.push_back(sta);
	}
	
	void CSignalChangeStatePkt::Clear()
	{
		m_vecSta.clear();
	}
	
	ProtocolBase::CProtocolHead* CSignalChangeStatePkt::Clone()
	{
		CSignalChangeStatePkt* pClone = new CSignalChangeStatePkt();
		*pClone = *this;
		return pClone;
	}

	int CSignalChangeStatePkt::StreamSize()
	{
		int nSize = 0;
		for(int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return CProtocolHead::HeaderSize() + 2 + nSize;
	}

	int CSignalChangeStatePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalChangeStatePkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		m_dwPktLen = StreamSize() - CProtocolHead::HeaderSize();

		//  包头
		int nOffset = 0;
		int nRet = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSta.size());
		nOffset += 2;

		// 车站
		for(int i=0; i<m_vecSta.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			nRet = m_vecSta.at(i).ToStreamBuf(pBuf+ nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			
			nOffset += nRet;
		}

		return nOffset;
	}

	int CSignalChangeStatePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalChangeStatePkt size len\n");
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
		int nNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for(int i=0; i<nNum; i++)
		{
			CSignalChangeSta objSta;
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

	//==============================================================================
	//
	//								CSignalCallPkt
	//						遥信总召信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CSignalCallPkt::CSignalCallPkt()
	{
		m_byType = TYPE_SIGNAL_INFO;
		m_byFunc = FUNC_SIGNALCALL_INTERTOSVR;
	}

	CSignalCallPkt::~CSignalCallPkt()
	{

	}
	//==============================================================================
	//
	//						SOE信息【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalSOE
	//						SOE信息包中遥信点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CSignalSOE::CSignalSOE()
		: m_dwAddr(0)
		, m_byValue(0)
	{

	}

	CSignalSOE::~CSignalSOE()
	{
	}

	void CSignalSOE::SetAddr(DWORD dwAddr)
	{
		m_dwAddr = dwAddr;
	}

	DWORD CSignalSOE::GetAddr() const
	{
		return m_dwAddr;
	}
	
	void CSignalSOE::SetValue(BYTE byValue)
	{
		m_byValue = byValue;
	}

	BYTE CSignalSOE::GetValue() const
	{
		return m_byValue;
	}

	void CSignalSOE::SetSOETime(BYTE byYear, BYTE byMonth, BYTE byDay, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec)
	{
		m_SOETime.Set(byYear, byMonth, byDay, byHour, byMinute, bySecond, wMilliSec);
	}

	void CSignalSOE::SetSOETime(time_t t, WORD wMilliSec)
	{
		m_SOETime.Set(t, wMilliSec);
	}

	void CSignalSOE::SetSOETime1(DWORD dwDate, DWORD dwTime)
	{
		m_SOETime.Set(dwDate, dwTime);
	}

	void CSignalSOE::SetSOETime(timeval t)
	{
		m_SOETime.Set(t);
	}

	void CSignalSOE::SetSOETime(CProtocolTime objTime)
	{
		m_SOETime = objTime;
	}

	CProtocolTime CSignalSOE::GetProtocolSOETime() const
	{
		return m_SOETime;
	}

	timeval CSignalSOE::GetSOETime() const
	{
		return m_SOETime.GetTimeval();
	}

	int CSignalSOE::StreamSize()
	{
		return 4 + 1 + CProtocolTime::StreamSize(); // 遥信地址 + 遥信值 ＋ SOE时间　
	}

	int CSignalSOE::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalSOE signal value len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		// 遥信地址
		int nOffset = 0;
		SETSTREAMINT(pBuf, m_dwAddr);
		nOffset += 4;

		// 遥信值
		pBuf[nOffset] = m_byValue;
		nOffset += 1;

		// SOE时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_SOETime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	int CSignalSOE::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSignalSOE len\n");
			return -1;
		}

		int nOffset = 0;

		// 遥信地址
		m_dwAddr = GETSTREAMINT(pBuf+nOffset);
		nOffset += PROTOCAL_SIGNALADDR_LEN;
		
		// 遥信值
		m_byValue = pBuf[nOffset];
		nOffset += PROTOCOL_SIGNALVALUE_LEN;

		// SOE时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_SOETime.FromStreamBuf(pBuf+nOffset, nLenTmp);
		if(-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;
		return nOffset;
	}

	//==============================================================================
	//	
	//									  CStaSOE
	//						SOE信息包中车站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CStaSOE::CStaSOE()
	{
		m_wStaId = 0;
		m_vecSignal.clear();
	}

	CStaSOE::~CStaSOE()
	{

	}

	void CStaSOE::SetStaId(WORD wStaIdSOE)
	{
		m_wStaId = wStaIdSOE;
	}

	WORD CStaSOE::GetStaId() const
	{
		return m_wStaId;
	}

	WORD CStaSOE::GetSignalCount() const
	{
		return (WORD)m_vecSignal.size();
	}
	
	CSignalSOE* CStaSOE::GetSignal(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSignal.size())
		{
			return NULL;
		}

		return &m_vecSignal.at(nIndex);
	}

	void CStaSOE::Add(CSignalSOE &signal)
	{
		m_vecSignal.push_back(signal);
	}

	int CStaSOE::StreamSize()
	{
		return (int)(PROTOCOL_SIGNALSTAID_LEN + PROTOCOL_SIGNALNUM_LEN
			+ m_vecSignal.size() * CSignalSOE::StreamSize()); // 车站id + 遥信个数 + 遥信个数 * 遥信点
	}

	int CStaSOE::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaSOE sta len\n");

			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 车站Id
		SETSTREAMSHORT(pBuf, m_wStaId);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// SOE个数
		SETSTREAMSHORT(pBuf+nOffset, m_vecSignal.size());
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// SOE
		for (int i = 0; i < m_vecSignal.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSignal.at(i).ToStreamBuf(pBuf + nOffset, nBufLen);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CStaSOE::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaSOE len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;
		
		// 车站数量
		WORD nSOENum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// SOE
		for (int i = 0; i < nSOENum; i++)
		{
			CSignalSOE objSignalSOE;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objSignalSOE.FromStreamBuf(pBuf+nOffset, nLenTmp);
			if(-1 == nRet)
			{
				return -1;
			}

			m_vecSignal.push_back(objSignalSOE);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//	
	//										 CSOEPkt
	//						  SOE信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	CSOEPkt::CSOEPkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_COLLECTSOE)
	{
		m_vecSta.clear();
	}

	CSOEPkt::~CSOEPkt()
	{

	}

	WORD CSOEPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}
	
	CStaSOE* CSOEPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	void CSOEPkt::Add(CStaSOE &sta)
	{
		m_vecSta.push_back(sta);
	}

	ProtocolBase::CProtocolHead* CSOEPkt::Clone()
	{
		CSOEPkt* pClone = new CSOEPkt();
		*pClone = *this;
		return pClone;
	}

	int CSOEPkt::StreamSize()
	{
		int pktHeadLen = CProtocolHead::HeaderSize();
		int nStationSize = 0;
		
		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize();
		}
		
		return pktHeadLen + PROTOCOL_SIGNALSTANUM_LEN + nStationSize; // 包头长度  +　车站数量 +  车站size

	}

	int CSOEPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSOEPkt len\n");
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
		SETSTREAMINT(pBuf + nOffset, m_vecSta.size());
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
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

	int CSOEPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOEPkt SOE size len\n");
			return -1;
		}

		int nOffset = 0;
		int nRet = CProtocolHead::FromStreamBuf(pBuf+nOffset, nBufLen);		
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// 车站数量
		WORD nStaNum = GETSTREAMSHORT(pBuf+nOffset);
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
		for(int i=0; i<nStaNum; i++)
		{
			CStaSOE objSta;
			int nLenTmp = nBufLen - nOffset;
			nRet = objSta.FromStreamBuf(pBuf+nOffset, nLenTmp);
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
	//	CSOEReplyPkt
	//	SOE信息回执包【AppSignalMgr→InterSvr104】
	//
	//==============================================================================
	CSOEReplyPkt::CSOEReplyPkt()
	{
		m_byType = TYPE_SIGNAL_INFO;
		m_byFunc = FUNC_SOEREPLY;
	}

	CSOEReplyPkt::~CSOEReplyPkt()
	{

	}

	//==============================================================================
	//
	//						SOE存盘信息【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalSOESave
	//						SOE存盘信息包中遥信点【AppSignalMgr→OSM模块】
	//
	//==============================================================================
	CSOESave::CSOESave()
		: m_dwId(0)
		, m_strName("")
		, m_byLevel(0)
		, m_byValue(0)
	{

	}

	CSOESave::~CSOESave()
	{
	}

	void CSOESave::SetId(DWORD dwId)
	{
		m_dwId = dwId;
	}

	DWORD CSOESave::GetId() const
	{
		return m_dwId;
	}
	
	void CSOESave::SetName(string strSet)
	{
		m_strName = strSet;
	}
	string CSOESave::GetName() const
	{
		return m_strName;
	}
	
	void CSOESave::SetLevel(BYTE bySet)
	{
		m_byLevel = bySet;
	}
	BYTE CSOESave::GetLevel() const
	{
		return m_byLevel;
	}

	void CSOESave::SetValue(BYTE byValue)
	{
		m_byValue = byValue;
	}

	BYTE CSOESave::GetValue() const
	{
		return m_byValue;
	}

	void CSOESave::SetRecTime(BYTE byYear, BYTE byMonth, BYTE byDay, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec)
	{
		m_objRecTime.Set(byYear, byMonth, byDay, byHour, byMinute, bySecond, wMilliSec);
	}
	
	void CSOESave::SetRecTime(time_t t, WORD wMilliSec)
	{
		m_objRecTime.Set(t, wMilliSec);
	}
	
	void CSOESave::SetRecTime1(DWORD dwDate, DWORD dwTime)
	{
		m_objRecTime.Set(dwDate, dwTime);
	}
	
	void CSOESave::SetRecTime(timeval t)
	{
		m_objRecTime.Set(t);
	}
	
	void CSOESave::SetRecTime(CProtocolTime objTime)
	{
		m_objRecTime = objTime;
	}
	
	CProtocolTime* CSOESave::GetRecTime()
	{
		return &m_objRecTime;
	}
	
	timeval CSOESave::GetRecTime1() const
	{
		return m_objRecTime.GetTimeval();
	}

	void CSOESave::SetProductTime(BYTE byYear, BYTE byMonth, BYTE byDay, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec)
	{
		m_objProductTime.Set(byYear, byMonth, byDay, byHour, byMinute, bySecond, wMilliSec);
	}

	void CSOESave::SetProductTime(time_t t, WORD wMilliSec)
	{
		m_objProductTime.Set(t, wMilliSec);
	}

	void CSOESave::SetProductTime1(DWORD dwDate, DWORD dwTime)
	{
		m_objProductTime.Set(dwDate, dwTime);
	}

	void CSOESave::SetProductTime(timeval t)
	{
		m_objProductTime.Set(t);
	}
	
	void CSOESave::SetProductTime(CProtocolTime objTime)
	{
		m_objProductTime = objTime;
	}

	CProtocolTime* CSOESave::GetProductTime()
	{
		return &m_objProductTime;
	}

	timeval CSOESave::GetProductTime1() const
	{
		return m_objProductTime.GetTimeval();
	}

	int CSOESave::StreamSize()
	{
		return (int)(4 + 1+m_strName.length() + 1 + 1 + CProtocolTime::StreamSize() + CProtocolTime::StreamSize());
	}

	int CSOESave::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSignalSOESave len\n");

			nBufLen = StreamSize();
			return -1;
		}

		// 遥信Id
		int nOffset = 0;
		SETSTREAMINT(pBuf, m_dwId);
		nOffset += PROTOCAL_SIGNALADDR_LEN;

		// 遥信名称
		pBuf[nOffset] = m_strName.length();
		nOffset ++;
		memcpy(pBuf + nOffset, m_strName.c_str(), m_strName.length());
		nOffset += m_strName.length();

		// 遥信电压等级
		pBuf[nOffset] = m_byLevel;
		nOffset ++;

		// 遥信值
		pBuf[nOffset] = m_byValue;
		nOffset += PROTOCOL_SIGNALVALUE_LEN;

		// SOE接收的时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_objRecTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// SOE产生的时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_objProductTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	int CSOESave::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf::Buflen is less than the CSignalSOESave signal value len\n");

			return -1;
		}

		int nOffset = 0;

		// 遥信Id
		m_dwId = GETSTREAMINT(pBuf);
		nOffset += PROTOCAL_SIGNALADDR_LEN;

		// 遥信名称
		BYTE byLen = pBuf[nOffset];
		nOffset ++;
		char cObjname[255] = {0};
		memcpy(cObjname, pBuf + nOffset, byLen);
		m_strName = cObjname;
		nOffset += byLen;
		
		// 遥信电压等级
		m_byLevel = pBuf[nOffset];
		nOffset ++;

		// 遥信值
		m_byValue = pBuf[nOffset];
		nOffset += PROTOCOL_SIGNALVALUE_LEN;

		// SOE记录时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_objRecTime.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if(-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// SOE产生时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_objProductTime.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if(-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	//==============================================================================
	//	
	//									  CStaSOESave
	//						SOE存盘信息包中车站【AppSignalMgr→OSM模块】
	//
	//==============================================================================
	CStaSOESave::CStaSOESave()
	{
		m_wStaId = 0;
		m_vecSoe.clear();
	}

	CStaSOESave::~CStaSOESave()
	{

	}

	void CStaSOESave::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	WORD CStaSOESave::GetStaId() const
	{
		return m_wStaId;
	}

	WORD CStaSOESave::GetSoeCount() const
	{
		return (WORD)m_vecSoe.size();
	}

	CSOESave* CStaSOESave::GetSoe(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSoe.size())
		{
			return NULL;
		}

		return &m_vecSoe.at(nIndex);
	}

	void CStaSOESave::Add(CSOESave &signal)
	{
		m_vecSoe.push_back(signal);
	}
	
	void CStaSOESave::Clear()
	{
		m_vecSoe.clear();
	}

	int CStaSOESave::StreamSize()
	{
		int nSize = PROTOCOL_SIGNALSTAID_LEN + PROTOCOL_SIGNALNUM_LEN;

		for (int i=0; i<m_vecSoe.size(); i++)
		{
			nSize += m_vecSoe[i].StreamSize();
		}

		return nSize;
	}

	int CStaSOESave::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaSOESave sta len\n");

			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf, m_wStaId);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// SOE个数
		SETSTREAMSHORT(pBuf+nOffset, m_vecSoe.size());
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// SOE
		for (int i = 0; i < m_vecSoe.size(); i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecSoe.at(i).ToStreamBuf(pBuf + nOffset, nBufLen);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CStaSOESave::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		if (nBufLen < PROTOCOL_SIGNALSTAID_LEN)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaSOESave staId len\n");
			return -1;
		}

		int nOffset = 0;
		m_wStaId = GETSTREAMSHORT(pBuf);
		nOffset += PROTOCOL_SIGNALSTAID_LEN;

		// SOE个数
		if (nBufLen - nOffset < PROTOCOL_SIGNALNUM_LEN)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaSOESave SOE num len\n");
			return -1;
		}

		int nSOENum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += PROTOCOL_SIGNALNUM_LEN;

		// SOE
		for (int i = 0; i < nSOENum; i++)
		{
			CSOESave objSignalSOE;
			int nLenTmp = nBufLen - nOffset;
			int nRet = objSignalSOE.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if(-1 == nRet)
			{
				return -1;
			}

			m_vecSoe.push_back(objSignalSOE);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//	
	//										 CSOESavePkt
	//						  SOE存盘信息包【AppSignalMgr→OSM模块】
	//
	//==============================================================================
	CSOESavePkt::CSOESavePkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SOESAVE)
	{
		m_vecSta.clear();
	}

	CSOESavePkt::~CSOESavePkt()
	{

	}

	WORD CSOESavePkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CStaSOESave* CSOESavePkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	void CSOESavePkt::Add(CStaSOESave &sta)
	{
		m_vecSta.push_back(sta);
	}
	
	void CSOESavePkt::Clear()
	{
		m_vecSta.clear();
	}

	ProtocolBase::CProtocolHead* CSOESavePkt::Clone()
	{
		CSOESavePkt* pClone = new CSOESavePkt();
		*pClone = *this;
		return pClone;
	}

	int CSOESavePkt::StreamSize()
	{
		int nPktHeadLen = CProtocolHead::HeaderSize();
		int nStationSize = 0;
		for(int i = 0; i < m_vecSta.size(); i++)
		{
			nStationSize += m_vecSta.at(i).StreamSize();
		}

		return nPktHeadLen + PROTOCOL_SIGNALSTANUM_LEN + nStationSize; // 包头长度  +　车站数量 +  车站size
	}

	int CSOESavePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSOESavePkt len\n");
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
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
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

	int CSOESavePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOESavePkt SOE size len\n");
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
		nOffset += PROTOCOL_SIGNALSTANUM_LEN;

		// 车站信息
		for(int i=0; i<nStaNum; i++)
		{
			CStaSOESave objSta;
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
	//	CSOESaveReplyPkt
	//	SOE存盘信息回执包【OSM模块→AppSignalMgr】
	//
	//==============================================================================
	CSOESaveReplyPkt::CSOESaveReplyPkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SOESAVEREPLY)
		, m_dwReceiveRslt(0)
		, m_strDesc("")
	{

	}

	CSOESaveReplyPkt::~CSOESaveReplyPkt()
	{

	}
	
	void CSOESaveReplyPkt::SetReceivseRslt(DWORD dwRslt)
	{
		m_dwReceiveRslt = dwRslt;
	}

	void CSOESaveReplyPkt::SetDesc(string strDesc)
	{
		m_strDesc = strDesc;
	}

	DWORD CSOESaveReplyPkt::GetReceiveRslt() const
	{
		return m_dwReceiveRslt;
	}

	string CSOESaveReplyPkt::GetDesc() const
	{
		return m_strDesc;
	}

	CProtocolHead* CSOESaveReplyPkt::Clone()
	{
		CSOESaveReplyPkt* pClone = new CSOESaveReplyPkt();
		*pClone = *this;
		return pClone;
	}

	int CSOESaveReplyPkt::StreamSize()
	{
		return (int)(CProtocolHead::StreamSize() + 4 + 2 + m_strDesc.size());
	}

	int CSOESaveReplyPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSOESaveReplyPkt len\n");
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

		// 接收结果
		SETSTREAMINT(pBuf + nOffset, m_dwReceiveRslt);
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
	
	int CSOESaveReplyPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 包头
		if (nBufLen < CProtocolHead::HeaderSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOESaveReplyPkt header size len\n");
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
		if (nBufLen < nOffset + 4)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOESaveReplyPkt receive result len\n");
			return -1;
		}

		m_dwReceiveRslt = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 描述长度
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOESaveReplyPkt desc size len\n");
			return -1;
		}

		WORD wDescLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 描述
		if (nBufLen < nOffset + wDescLen)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOESaveReplyPkt desc len\n");
			return -1;
		}

		char cDesc[65535 + 1] = {0};
		int nDescLenTmp = MIN(wDescLen, 65535);
		memcpy(cDesc, pBuf + nOffset, nDescLenTmp);
		cDesc[nDescLenTmp] = '\0';
		m_strDesc = cDesc;
		nOffset += wDescLen;

		return nOffset;
	}

	//==============================================================================
	//	
	//	CStaOfSOEQueryReq
	//	SOE查询请求中车站【其他模块→OSM模块】
	//
	//==============================================================================
	CStaOfSOEQueryReq::CStaOfSOEQueryReq()
		: m_wStaId(0)
	{
		m_vecSignal.clear();
	}

	CStaOfSOEQueryReq::~CStaOfSOEQueryReq()
	{

	}

	void CStaOfSOEQueryReq::SetStaId(WORD wStaId)
	{
		m_wStaId = wStaId;
	}

	WORD CStaOfSOEQueryReq::GetStaId() const
	{
		return m_wStaId;
	}

	void CStaOfSOEQueryReq::Add(DWORD dwSignal)
	{
		m_vecSignal.push_back(dwSignal);
	}

	WORD CStaOfSOEQueryReq::GetSignalCount() const
	{
		return (WORD)m_vecSignal.size();
	}

	DWORD CStaOfSOEQueryReq::GetSignal(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecSignal.size());
		return m_vecSignal.at(nIndex);
	}

	int CStaOfSOEQueryReq::StreamSize()
	{
		return (int)(2 + 2 + 4 * m_vecSignal.size());
	}

	int CStaOfSOEQueryReq::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CStaOfSOEQueryReq len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 车站Id
		int nOffset = 0;
		SETSTREAMSHORT(pBuf + nOffset, m_wStaId);
		nOffset += 2;

		// 遥信点数量
		SETSTREAMSHORT(pBuf + nOffset, m_vecSignal.size());
		nOffset += 2;

		// 遥信点
		for (int i=0; i<m_vecSignal.size(); i++)
		{
			SETSTREAMINT(pBuf + nOffset, m_vecSignal.at(i));
			nOffset += 4;
		}

		return nOffset;
	}

	int CStaOfSOEQueryReq::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// 车站Id
		int nOffset = 0;
		if (nBufLen < 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaOfSOEQueryReq sta id len\n");
			return -1;
		}

		m_wStaId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥信点数量
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaOfSOEQueryReq signal num len\n");
			return -1;
		}

		WORD wSignalNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 遥信点
		if (nBufLen < nOffset + wSignalNum* 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CStaOfSOEQueryReq signal len\n");
			return -1;
		}

		for (int i=0; i<wSignalNum; i++)
		{
			DWORD dwSignal = GETSTREAMINT(pBuf + nOffset);
			m_vecSignal.push_back(dwSignal);
			nOffset += 4;
		}

		return nOffset;
	}

	//==============================================================================
	//	
	//	CSOEQueryReqPkt
	//	SOE查询请求包【其他模块→OSM模块】
	//
	//==============================================================================
	CSOEQueryReqPkt::CSOEQueryReqPkt()
		: CProtocolHead(TYPE_SIGNAL_INFO, FUNC_SOEQUERYREQ)
	{
		m_vecSta.clear();
	}

	CSOEQueryReqPkt::~CSOEQueryReqPkt()
	{

	}

	void CSOEQueryReqPkt::SetStartTime(BYTE byYear, BYTE byMonth, BYTE byDay
		, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec)
	{
		m_StartTime.Set(byYear, byMonth, byDay, byHour, byMinute, bySecond, wMilliSec);
	}

	void CSOEQueryReqPkt::SetStartTime(time_t t, WORD wMs)
	{
		m_StartTime.Set(t, wMs);
	}

	void CSOEQueryReqPkt::SetStartTime(timeval t)
	{
		m_StartTime.Set(t);
	}

	void CSOEQueryReqPkt::SetEndTime(BYTE byYear, BYTE byMonth, BYTE byDay
		, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec)
	{
		m_EndTime.Set(byYear, byMonth, byDay, byHour, byMinute, bySecond, wMilliSec);
	}

	void CSOEQueryReqPkt::SetEndTime(time_t t, WORD wMs)
	{
		m_EndTime.Set(t, wMs);
	}

	void CSOEQueryReqPkt::SetEndTime(timeval t)
	{
		m_EndTime.Set(t);
	}

	CProtocolTime CSOEQueryReqPkt::GetStartTime() const
	{
		return m_StartTime;
	}

	time_t CSOEQueryReqPkt::GetStartTime_t() const
	{
		return m_StartTime.GetTime();
	}

	timeval CSOEQueryReqPkt::GetStartTimeval() const
	{
		return m_StartTime.GetTimeval();
	}

	CProtocolTime CSOEQueryReqPkt::GetEndTime() const
	{
		return m_EndTime;
	}

	time_t CSOEQueryReqPkt::GetEndTime_t() const
	{
		return m_EndTime.GetTime();
	}

	timeval CSOEQueryReqPkt::GetEndTimeval() const
	{
		return m_EndTime.GetTimeval();
	}

	void CSOEQueryReqPkt::Add(CStaOfSOEQueryReq Sta)
	{
		m_vecSta.push_back(Sta);
	}

	WORD CSOEQueryReqPkt::GetStaCount() const
	{
		return (WORD)m_vecSta.size();
	}

	CStaOfSOEQueryReq* CSOEQueryReqPkt::GetSta(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecSta.size())
		{
			return NULL;
		}

		return &m_vecSta.at(nIndex);
	}

	CProtocolHead* CSOEQueryReqPkt::Clone()
	{
		CSOEQueryReqPkt* pClone  = new CSOEQueryReqPkt();
		*pClone = *this;
		return pClone;
	}

	int CSOEQueryReqPkt::StreamSize()
	{
		int nSize = 0;
		for (int i=0; i<m_vecSta.size(); i++)
		{
			nSize += m_vecSta.at(i).StreamSize();
		}

		return (int)(CProtocolHead::HeaderSize() 
			+ m_StartTime.StreamSize() 
			+ m_EndTime.StreamSize() + 2 + nSize);
	}

	int CSOEQueryReqPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSOEQueryReqPkt len\n");
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

		// 查询起始时间
		int nLenTmp = nBufLen - nOffset;
		nRet = m_StartTime.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 查询终止时间
		nLenTmp = nBufLen - nOffset;
		nRet = m_EndTime.ToStreamBuf(pBuf + nOffset, nBufLen);
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
			nLenTmp = nBufLen - nOffset;
			nRet = m_vecSta.at(i).ToStreamBuf(pBuf + nOffset, nBufLen);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
		}

		return nOffset;
	}

	int CSOEQueryReqPkt::FromStreamBuf(const char* pBuf, int nBufLen) 
	{
		// 包头
		int nOffset = 0;
		if (nBufLen < CProtocolHead::HeaderSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOEQueryReqPkt header size len\n");
			return -1;
		}

		int nRet = CProtocolHead::FromStreamBuf(pBuf + nOffset, nBufLen);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 查询起始时间
		if (nBufLen < nOffset + m_StartTime.StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOEQueryReqPkt Start time len\n");
			return -1;
		}

		int nLenTmp = nBufLen - nOffset;
		nRet = m_StartTime.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 查询终止时间
		if (nBufLen < nOffset + m_EndTime.StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOEQueryReqPkt End time len\n");
			return -1;
		}

		nLenTmp = nBufLen -nOffset;
		nRet = m_EndTime.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		// 车站数量
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSOEQueryReqPkt sta num len\n");
			return -1;
		}

		WORD wStaNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 车站
		for (int i=0; i<wStaNum; i++)
		{
			CStaOfSOEQueryReq objSta;
			nLenTmp = nBufLen - nOffset;
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
	//	CSOEQueryRsltPkt
	//	SOE查询结果包【OSM模块→其他模块】
	//
	//==============================================================================
	CSOEQueryRsltPkt::CSOEQueryRsltPkt()
	{
		m_byType = TYPE_SIGNAL_INFO;
		m_byFunc = FUNC_SOEQUERYRSLT;
	}

	CSOEQueryRsltPkt::~CSOEQueryRsltPkt()
	{

	}



}