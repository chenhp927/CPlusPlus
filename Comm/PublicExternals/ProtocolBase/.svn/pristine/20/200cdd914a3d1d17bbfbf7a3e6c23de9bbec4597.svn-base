//==============================================================================
//	
//							 ProtocolHead.cpp
//							   
//begin	  : 2016/08/13
//describe: 协议基础类
//
//==============================================================================
#include "ProtocolBase.h"
#include "ProtocolHead.h"

namespace ProtocolBase
{
	using namespace std;

	//==============================================================================
	//	
	//							 ReferenceCount.cpp
	//							   
	//begin   : 2016/08/15
	//describe: 引用计数
	//
	//==============================================================================
	CReferenceCount::CReferenceCount()
		: m_nRefCount(0)
	{
	}

	CReferenceCount::~CReferenceCount()
	{
	}

	int CReferenceCount::AddRef()
	{
		return ++m_nRefCount;
	}

	int CReferenceCount::Release()
	{
		return --m_nRefCount;
	}

	int CReferenceCount::GetRef()
	{
		return m_nRefCount;
	}

	void CReferenceCount::Reset()
	{
		m_nRefCount = 0;
	}


	//==============================================================================
	//	
	//							  CProtocolHead
	//							    协议包包头
	//
	//==============================================================================
	CProtocolHead::CProtocolHead()
		: m_byType(0),
		m_byFunc(0),
		m_byVersion(1),  
		m_byPktCount(1),
		m_byCurPkt(1),
		m_dwRequestCode(0),
		m_dwPktLen(0)
	{ 
	} 

	CProtocolHead::CProtocolHead(BYTE byType, BYTE byFunc)
		: m_byType(byType),
		m_byFunc(byFunc),
		m_byVersion(1),
		m_byPktCount(1),
		m_byCurPkt(1),
		m_dwRequestCode(0),
		m_dwPktLen(0)
	{ 
	} 

	CProtocolHead::CProtocolHead(const CProtocolHead &other)
	{
		*this = other;
	}

	CProtocolHead::~CProtocolHead() 
	{ 

	} 
	
	CProtocolHead & CProtocolHead::operator = (const CProtocolHead& other)
	{
		*((CObjectBase*)this) = *((CObjectBase*)&other);

		m_byFunc = other.m_byFunc;
		m_byType = other.m_byType;
		m_byVersion = other.m_byVersion;
		m_byPktCount = other.m_byPktCount;
		m_byCurPkt = other.m_byCurPkt;
		m_dwRequestCode = other.m_dwRequestCode;
		m_dwPktLen = other.m_dwPktLen;
		return *this;
	}

	void CProtocolHead::AddRef()
	{
		m_RefCount.AddRef();
	}

	void CProtocolHead::Release()
	{
		if (m_RefCount.Release() == 0)
		{
			delete this;
		}
	}

	int CProtocolHead::HeaderSize()
	{
		return PROTOCOL_HEAD_LEN + PROTOCOL_TIME_LEN;
	}
	
	int CProtocolHead::StreamSize()
	{ 
		return PROTOCOL_HEAD_LEN + CProtocolTime::StreamSize();
	}	
	
	BYTE CProtocolHead::GetType()
	{ 
		return m_byType;
	}
	
	BYTE CProtocolHead::GetFunc()	
	{ 
		return m_byFunc;
	}
	
	BYTE CProtocolHead::GetVersion()
	{ 
		return m_byVersion; 
	}
	
	BYTE CProtocolHead::GetPktCount()
	{ 
		return m_byPktCount; 
	}
	
	BYTE CProtocolHead::GetCurPkt()
	{ 
		return m_byCurPkt; 
	}	
	
	DWORD CProtocolHead::GetLen()
	{ 
		return m_dwPktLen; 
	}
	
	DWORD CProtocolHead::GetRequestCode()
	{ 
		return m_dwRequestCode; 
	}
	
	CProtocolTime CProtocolHead::GetProtocolTime() const
	{
		return m_Time;
	}

	timeval CProtocolHead::GetTime() const
	{
		return m_Time.GetTimeval();
	}

	void CProtocolHead::SetType(BYTE byType)
	{ 
		m_byType = byType;
	}
	
	void CProtocolHead::SetFunc(BYTE byFunc)
	{ 
		m_byFunc = byFunc;
	}
	
	void CProtocolHead::SetVersion(BYTE byVer)
	{ 
		m_byVersion = byVer; 
	}
	
	void CProtocolHead::SetPktCount(BYTE byPktCount)
	{ 
		m_byPktCount = byPktCount; 
	}
	
	void CProtocolHead::SetCurPkt(BYTE byCurPkt)
	{ 
		m_byCurPkt = byCurPkt; 
	}
	
	void CProtocolHead::SetLen(DWORD dwPktLen)
	{ 
		m_dwPktLen = dwPktLen; 
	}
	
	void CProtocolHead::SetTime(BYTE byYear, BYTE byMonth, BYTE byDay, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec)
	{
		m_Time.Set(byYear, byMonth, byDay, byHour, byMinute, bySecond, wMilliSec);
	}

	void CProtocolHead::SetTime(time_t t, WORD wMilliSec)
	{
		m_Time.Set(t, wMilliSec);
	}

	void CProtocolHead::SetTime(DWORD dwDate, DWORD dwTime)
	{
		m_Time.Set(dwDate, dwTime);
	}

	void CProtocolHead::SetTime(timeval t)
	{
		m_Time.Set(t);
	}

	void CProtocolHead::SetRequestCode(DWORD dwReqCode)	
	{ 
		m_dwRequestCode = dwReqCode; 
	}

	int CProtocolHead::ToStreamBuf(char* pBuf, int& nBufLen) 
	{ 
		if(nBufLen < StreamSize())
		{
			LogImportant("Buflen is less than the headlen\n");

			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;
		pBuf[nOffset] = m_byType; 
		nOffset += 1;

		pBuf[nOffset] = m_byFunc; 
		nOffset += 1;

		pBuf[nOffset] = m_byVersion;
		nOffset += 1;

		pBuf[nOffset] = m_byPktCount;
		nOffset += 1;

		pBuf[nOffset] = m_byCurPkt;
		nOffset += 1;

		SETSTREAMINT(pBuf + nOffset, m_dwRequestCode);
		nOffset += 4;
		
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_Time.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		SETSTREAMINT(pBuf + nOffset, m_dwPktLen);
		nOffset += 4;

		return nOffset;	
	} 

	int CProtocolHead::FromStreamBuf(const char* pBuf, int nBufLen) 
	{ 
		if(nBufLen < StreamSize())
		{
			LogImportant("Buflen is less than the headlen\n" );

			return -1;
		}

		int nOffset = 0;
		m_byType = static_cast<BYTE>(pBuf[nOffset]); 
		nOffset += 1;

		m_byFunc = static_cast<BYTE>(pBuf[nOffset]); 
		nOffset += 1;

		m_byVersion = static_cast<BYTE>(pBuf[nOffset]);
		nOffset += 1;

		m_byPktCount = static_cast<BYTE>(pBuf[nOffset]);
		nOffset += 1;

		m_byCurPkt = static_cast<BYTE>(pBuf[nOffset]);
		nOffset += 1;

		m_dwRequestCode = (DWORD)GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		int nRet = m_Time.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		m_dwPktLen = (DWORD)GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		return nOffset; 
	}
	
	CProtocolHead * CProtocolHead::Clone()
	{
		CProtocolHead* pClone = new CProtocolHead();
		*pClone = *this;
		return pClone;
	}

	// 取得协议错误描述
	string CProtocolHead::GetProErrDescrip(BYTE byErrType)
	{
		string strErr;
		
		switch(byErrType)
		{
		case VERSION_ERROR:
			strErr = "版本错误";
			break;
		case PACKSUM_ERROR:
			strErr = "包数错误";
			break;
		case PACKLEN_ERROR:
			strErr = "包长错误";
			break;
		case PROTOCOL_OK:
			strErr = "协议包头正确";
			break;
		default:
			strErr = "未知协议错误";
		}
		
		return strErr;
	}

	// 检查协议有效性
	int CProtocolHead::CheckValid(const char* pBuf, int nBufLen)
	{
		if(nBufLen < StreamSize())
		{
			return PACKLEN_ERROR;
		}

		m_byType = static_cast<BYTE>(pBuf[0]); 
		m_byFunc = static_cast<BYTE>(pBuf[1]); 
		m_byVersion = static_cast<BYTE>(pBuf[2]);
		m_byPktCount = static_cast<BYTE>(pBuf[3]); 
		m_byCurPkt = static_cast<BYTE>(pBuf[4]); 
		m_dwRequestCode = (DWORD)GETSTREAMINT(pBuf + 5);
		m_dwPktLen = (DWORD)GETSTREAMINT(pBuf + 9);

		if(m_byVersion != PROTOCOL_VERSION)
		{
			return VERSION_ERROR;
		}

		if (m_byPktCount < 1 || m_byCurPkt < 1)
		{
			return PACKSUM_ERROR;
		}
		else if (m_byCurPkt > m_byPktCount)
		{
			return PACKSUM_ERROR;
		}

		if ((m_dwPktLen + StreamSize() != nBufLen)
			|| (m_dwPktLen > PROTOCOL_INFO_LEN))
		{
			return PACKLEN_ERROR;
		}

		return PROTOCOL_OK; 
	}
	
	BYTE CProtocolHead::GetType(const char* pBuf)  
	{ 
		return *(pBuf); 
	} 
	
	BYTE CProtocolHead::GetFunc(const char* pBuf)  
	{ 
		return *(pBuf + 1); 
	} 

	BYTE CProtocolHead::PacketCount(const char* pBuf)  
	{ 
		return *(pBuf + 3);
	} 

	BYTE CProtocolHead::PacketNum(const char* pBuf)  
	{ 
		return *(pBuf + 4); 
	} 
	
	DWORD CProtocolHead::GetRequestCode(const char* pBuf) 
	{
		return (DWORD)GETSTREAMINT(pBuf + 5); 
	}

	DWORD CProtocolHead::PacketLen(const char* pBuf)  
	{ 
		return (DWORD)GETSTREAMINT(pBuf + 9);
	} 

	int CProtocolHead::UpdatePktCount(BYTE byPktCount, CBuffer *pBuf)
	{
		if (pBuf->GetUsed() < HeaderSize())
		{
			return 0;
		}

		char *pTmp = pBuf->GetBuffer();
		pTmp[3] = byPktCount;  
		return 1;
	}

	int CProtocolHead::UpdateCurPkt(BYTE byCurPkt, CBuffer *pBuf)
	{
		if (pBuf->GetUsed() < HeaderSize())
		{
			return 0;
		}

		char *pTmp = pBuf->GetBuffer();
		pTmp[4] = byCurPkt;
		return 1;
	}

	int CProtocolHead::UpdateLen(DWORD dwPktLen, CBuffer *pBuf)
	{
		if (pBuf->GetUsed() < HeaderSize())
		{
			return 0;
		}

		char *pTmp = pBuf->GetBuffer();
		SETSTREAMINT(pTmp + 9, dwPktLen);	
		return 1;
	}

}
