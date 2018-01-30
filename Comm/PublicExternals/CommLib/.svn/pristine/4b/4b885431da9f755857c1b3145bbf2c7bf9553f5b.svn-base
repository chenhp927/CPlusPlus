//==============================================================================
//                      ProtocolForward.cpp
//
//begin   : 2016-08-31
//describe:
//==============================================================================
#include "ProtocolForward.h"
#include "ForwardCondition.h"

#define FORWARDPKT_TYPE_REGISTER				1
#define FORWARDPKT_TYPE_FORWARD					2
#define FORWARDPKT_TYPE_COMMUSE					3

#define FORWARDTYPE_NOLOCAL						1
#define FORWARDTYPE_LOCAL						2

#define FORWARDPKT_DESTADDR_MAXCOUNT			255

//==============================================================================
//                 
//                    CForwardDataSeq
//                     双网数据序号类
//
//==============================================================================
CProtocolForward::CForwardDataSeq::CForwardDataSeq() 
{ 
	// 初始化为无效序列号0
	Reset();
} 

CProtocolForward::CForwardDataSeq::~CForwardDataSeq() 
{ 
} 

DWORD CProtocolForward::CForwardDataSeq::GetLow()
{
	return m_nSeqLow;
}

DWORD CProtocolForward::CForwardDataSeq::GetHigh()
{
	return m_nSeqHigh;
}

void CProtocolForward::CForwardDataSeq::Reset()
{
	m_nSeqLow = 0;
	m_nSeqHigh = 0;
}

bool CProtocolForward::CForwardDataSeq::IsValid()
{
	return m_nSeqLow != 0 || m_nSeqHigh != 0;
}

bool CProtocolForward::CForwardDataSeq::IsFarawayFrom(const CForwardDataSeq& other)
{
	DWORD nLow = m_nSeqLow > other.m_nSeqLow ? m_nSeqLow - other.m_nSeqLow : other.m_nSeqLow - m_nSeqLow;
	DWORD nHigh = m_nSeqHigh > other.m_nSeqHigh ? m_nSeqHigh - other.m_nSeqHigh : other.m_nSeqHigh - m_nSeqHigh;
	
	if (nHigh == 0)
	{
		return nLow > 50;
	}
	else
	{
		return true;
	}
}

void CProtocolForward::CForwardDataSeq::Inc()
{
	m_nSeqLow++;
	if (m_nSeqLow == 0)
	{
		m_nSeqHigh++;
		if (m_nSeqHigh == 0)
		{
			m_nSeqLow = 1; // 如果序列号回到无效的0后自动变成1
		}
	}
}

CProtocolForward::CForwardDataSeq CProtocolForward::CForwardDataSeq::GetNextSeq()
{
	CForwardDataSeq objDs = *this;
	objDs.Inc();
	return objDs;
}

int CProtocolForward::CForwardDataSeq::Size()
{
	return 8;
}

int CProtocolForward::CForwardDataSeq::ToStreamBuf(char* szBuf)
{
	char* p = szBuf; 

	SETSTREAMINT(p, m_nSeqLow); 
	p += 4;
	
	SETSTREAMINT(p, m_nSeqHigh); 
	p += 4;

	return p - szBuf; 
}

int CProtocolForward::CForwardDataSeq::FromStreamBuf(const char* szBuf)
{
	const char* p = szBuf; 

	m_nSeqLow = GETSTREAMINT(p);
	p += 4; 

	m_nSeqHigh = GETSTREAMINT(p); 
	p += 4; 

	return p - szBuf; 
}

bool CProtocolForward::CForwardDataSeq::operator == (const CForwardDataSeq& other)
{
	return other.m_nSeqLow == m_nSeqLow && other.m_nSeqHigh == m_nSeqHigh;
}

bool CProtocolForward::CForwardDataSeq::operator < (const CForwardDataSeq& other)
{
	if (m_nSeqHigh < other.m_nSeqHigh)
	{
		return true;
	}
	else if (m_nSeqHigh == other.m_nSeqHigh && m_nSeqLow < other.m_nSeqLow)
	{
		return true;
	}

	return false;
}

bool CProtocolForward::CForwardDataSeq::operator > (const CForwardDataSeq& other)
{
	if (m_nSeqHigh > other.m_nSeqHigh)
	{
		return true;
	}
	else if (m_nSeqHigh == other.m_nSeqHigh && m_nSeqLow > other.m_nSeqLow)
	{
		return true;
	}

	return false;
}

std::string CProtocolForward::CForwardDataSeq::GetDumpStr()
{
	char szDumpStr[64];
	memset(szDumpStr, 0, sizeof(szDumpStr));

	snprintf(szDumpStr, sizeof(szDumpStr), "%d,%d", GetHigh(), GetLow());

	return szDumpStr;
}


//==============================================================================
//                 
//                  CForwardAddr
//                  消息地址类
//
//==============================================================================
CProtocolForward::CForwardAddr::CForwardAddr()
{
}

CProtocolForward::CForwardAddr::~CForwardAddr() 
{ 
} 

int CProtocolForward::CForwardAddr::Size()
{
	return 9;
}

int CProtocolForward::CForwardAddr::ToStreamBuf(char* szBuf)
{
	char* p = szBuf; 
	
	*p = m_byBureauId;
	p++;
	
	*p = m_byUnitType;
	p++;
	
	SETSTREAMSHORT(p, m_wUnitId); 
	p += 2;
	
	*p = m_byDevType;
	p++;
	
	SETSTREAMSHORT(p, m_wDevId); 
	p += 2;
	
	SETSTREAMSHORT(p, m_wCltId); 
	p += 2;
	
	return p - szBuf; 
}

int CProtocolForward::CForwardAddr::FromStreamBuf(const char* szBuf)
{
	const char* p = szBuf; 
	
	m_byBureauId = *p;
	p++;
	
	m_byUnitType = *p;
	p++;
	
	m_wUnitId = GETSTREAMSHORT(p); 
	p += 2;
	
	m_byDevType = *p;
	p++;
	
	m_wDevId = GETSTREAMSHORT(p); 
	p += 2;
	
	m_wCltId = GETSTREAMSHORT(p); 
	p += 2;
	
	return p - szBuf; 
}

bool CProtocolForward::CForwardAddr::operator==( const CForwardAddr& other )
{
	bool bEqual = false;
	if ((CCommAddr)*this == (CCommAddr)other)
	{
		bEqual = true;
	}
	
	return bEqual;
}

bool CProtocolForward::CForwardAddr::IsMatched(CCommAddr& objAddr)
{
	if (!CCommAddr::IsMatched(objAddr))
	{
		return false;
	}
	
	return true;
}

//==============================================================================
//                 
//                    CForwardTypeFunc
//                    消息类型码功能码类
//
//==============================================================================
CProtocolForward::CForwardTypeFunc::CForwardTypeFunc()
{
	m_byType = 0;
	m_byFunc = 0;
}

CProtocolForward::CForwardTypeFunc::~CForwardTypeFunc() 
{ 
} 

BYTE CProtocolForward::CForwardTypeFunc::GetType(void)
{
	return m_byType;
}

void CProtocolForward::CForwardTypeFunc::SetType(BYTE byType)
{
	m_byType = byType;
}

BYTE CProtocolForward::CForwardTypeFunc::GetFunc(void)
{
	return m_byFunc;
}

void CProtocolForward::CForwardTypeFunc::SetFunc(BYTE byFunc)
{
	m_byFunc = byFunc;
}

bool CProtocolForward::CForwardTypeFunc::IsMatched(CForwardTypeFunc& obj)
{
	if (m_byType != 0xff && obj.GetType() != 0xff &&
		m_byType != obj.GetType())
	{
		return false;
	}
	
	if (m_byFunc != 0xff && obj.GetFunc() != 0xff &&
		m_byFunc != obj.GetFunc())
	{
		return false;
	}
	
	return true;
}

bool CProtocolForward::CForwardTypeFunc::IsInvalidType(BYTE by)
{
	return by == 0 || by == 0xff;
}

bool CProtocolForward::CForwardTypeFunc::IsInvalidFunc(BYTE by)
{
	return by == 0 || by == 0xff;
}

int CProtocolForward::CForwardTypeFunc::Size()
{
	return 2;
}

int CProtocolForward::CForwardTypeFunc::ToStreamBuf(char* szBuf)
{
	char* p = szBuf; 
	
	*p = m_byType;
	p++;
	
	*p = m_byFunc;
	p++;
	
	return p - szBuf; 
}

int CProtocolForward::CForwardTypeFunc::FromStreamBuf(const char* szBuf)
{
	const char* p = szBuf; 
	
	m_byType = *p;
	p++;
	
	m_byFunc = *p;
	p++;
	
	return p - szBuf; 
}

std::string CProtocolForward::CForwardTypeFunc::GetTagDumpStr()
{
	char szTmp[16];
	memset(szTmp, 0, sizeof(szTmp));
	snprintf(szTmp, sizeof(szTmp), "%02x, %02x", m_byType, m_byFunc);
	return szTmp;
}

//==============================================================================
//                 
//                     CForwardHead
//                     转发层包头类
//
//==============================================================================
CProtocolForward::CForwardHead::CForwardHead()
{
	m_byPackType = FORWARDPKT_TYPE_FORWARD;
	m_wForwardInfoLen = 0;
	m_byForwardType = FORWARDTYPE_NOLOCAL;
	m_dwMsgBodyLen = 0;
}

CProtocolForward::CForwardHead::~CForwardHead() 
{ 
} 

CProtocolForward::CForwardDataSeq CProtocolForward::CForwardHead::GetDataSeq()
{
	return m_objDataSeq;
}

void CProtocolForward::CForwardHead::SetDataSeq(CForwardDataSeq& objDataSeq)
{
	m_objDataSeq = objDataSeq;
}

BYTE CProtocolForward::CForwardHead::GetPackType()
{
	return m_byPackType;
}

void CProtocolForward::CForwardHead::SetPackType(BYTE by)
{
	m_byPackType = by;
}

bool CProtocolForward::CForwardHead::IsRegisterPack()
{
	return m_byPackType == FORWARDPKT_TYPE_REGISTER;
}

bool CProtocolForward::CForwardHead::IsForwardPack()
{
	return m_byPackType == FORWARDPKT_TYPE_FORWARD;
}

BYTE CProtocolForward::CForwardHead::GetForwardType()
{
	return m_byForwardType;
}

void CProtocolForward::CForwardHead::SetForwardType(BYTE by)
{
	assert(by == FORWARDTYPE_LOCAL || 
		by == FORWARDTYPE_NOLOCAL);
	
	m_byForwardType = by;
}

void CProtocolForward::CForwardHead::SetSupportLocalDispatch(bool bIsSupportLocalDispatch)
{
	if (bIsSupportLocalDispatch)
	{
		m_byForwardType = FORWARDTYPE_LOCAL;
	}
	else
	{
		m_byForwardType = FORWARDTYPE_NOLOCAL;
	}
}

bool CProtocolForward::CForwardHead::IsSupportLocalDispatch()
{
	return m_byForwardType == FORWARDTYPE_LOCAL;
}

void CProtocolForward::CForwardHead::SetTypeFunc(CForwardTypeFunc& objTypeFunc)
{
	m_objTypeFunc = objTypeFunc;
}

void CProtocolForward::CForwardHead::SetTypeFunc(BYTE byType, BYTE byFunc)
{
	m_objTypeFunc.SetType(byType);
	m_objTypeFunc.SetFunc(byFunc);
}

CProtocolForward::CForwardTypeFunc CProtocolForward::CForwardHead::GetTypeFunc()
{
	return m_objTypeFunc;
}

BYTE CProtocolForward::CForwardHead::GetMsgType()
{
	return m_objTypeFunc.GetType();
}

BYTE CProtocolForward::CForwardHead::GetMsgFunc()
{
	return m_objTypeFunc.GetFunc();
}

void CProtocolForward::CForwardHead::SetSrcAddr(CForwardAddr& objSrcAddr)
{
	m_objSrcAddr = objSrcAddr;
}

CProtocolForward::CForwardAddr CProtocolForward::CForwardHead::GetSrcAddr()
{
	return m_objSrcAddr;
}

vector<CProtocolForward::CForwardAddr>& CProtocolForward::CForwardHead::GetDestAddrs()
{
	return m_vecDestAddr;
}

void CProtocolForward::CForwardHead::AddDest(CForwardAddr& objDestAddr)
{
	m_vecDestAddr.push_back(objDestAddr);
}

int CProtocolForward::CForwardHead::StreamSize()
{
	int nLen = CForwardDataSeq::Size();
	nLen += 1 + 2;

	if (m_byPackType != FORWARDPKT_TYPE_FORWARD)
	{
		return nLen;
	}
	
	nLen += 1 + CForwardTypeFunc::Size() + CForwardAddr::Size() + 2;
	nLen += m_vecDestAddr.size() * CForwardAddr::Size();
	nLen += 4;

	return nLen;
}

int CProtocolForward::CForwardHead::FromStreamBuf(const char* szBuf, int nBufLen)
{
	int nOffset = 0;

	if (nBufLen - nOffset < CForwardDataSeq::Size() + 1 + 2)
	{
		return -1;
	}

	// 双网序列号
	m_objDataSeq.FromStreamBuf(szBuf + nOffset);
	nOffset += CForwardDataSeq::Size();
	
	// 包类型
	m_byPackType = szBuf[nOffset];
	nOffset++;

	// 转发信息长度
	m_wForwardInfoLen = GETSTREAMSHORT(szBuf + nOffset);
	nOffset += 2;
	
	if (m_byPackType == FORWARDPKT_TYPE_FORWARD)
	{
		if (nBufLen - nOffset < 1 + CForwardTypeFunc::Size() + CForwardAddr::Size() + 2)
		{
			return -2;
		}
		
		// 转发类型
		m_byForwardType = szBuf[nOffset];
		nOffset++;

		// 类型码功能码
		m_objTypeFunc.FromStreamBuf(szBuf + nOffset);
		nOffset += CForwardTypeFunc::Size();

		// 源地址
		m_objSrcAddr.FromStreamBuf(szBuf + nOffset);
		nOffset += CForwardAddr::Size();
		
		// 目标地址个数
		WORD wDestAddrCount = GETSTREAMSHORT(szBuf + nOffset);
		nOffset += 2;

		// 目标地址集合
		for (int x = 0; x < wDestAddrCount; x++)
		{
			if (nBufLen - nOffset < CForwardAddr::Size())
			{
				return -3;
			}
			
			CForwardAddr objDestAddr;
			objDestAddr.FromStreamBuf(szBuf + nOffset);
			nOffset += CForwardAddr::Size();

			m_vecDestAddr.push_back(objDestAddr);
		}
	}
	
	if (nBufLen - nOffset < 4)
	{
		return -4;
	}
	
	// 消息体长度
	m_dwMsgBodyLen = GETSTREAMINT(szBuf + nOffset);
	nOffset += 4;

	if (nBufLen - nOffset < m_dwMsgBodyLen)
	{
		return -5;
	}

	return nOffset;
}

int CProtocolForward::CForwardHead::ToStreamBuf(char* szBuf, int& nBufLen)
{
	int nNeedLen = StreamSize();
	if (nBufLen < nNeedLen)
	{
		nBufLen = nNeedLen;
		return -1;
	}
	
	int nOffset = 0;

	// 双网序列号
	m_objDataSeq.ToStreamBuf(szBuf + nOffset);
	nOffset += CForwardDataSeq::Size();
	
	// 包类型
	szBuf[nOffset] = m_byPackType;
	nOffset++;
	
	if (m_byPackType == FORWARDPKT_TYPE_FORWARD)
	{
		// 转发信息长度
		m_wForwardInfoLen = 1 + CForwardTypeFunc::Size() + CForwardAddr::Size() + 2 + m_vecDestAddr.size() * CForwardAddr::Size();
		SETSTREAMSHORT(szBuf + nOffset, m_wForwardInfoLen);
		nOffset += 2;
		
		// 转发类型
		szBuf[nOffset] = m_byForwardType;
		nOffset++;
		
		// 类型码功能码
		m_objTypeFunc.ToStreamBuf(szBuf + nOffset);
		nOffset += CForwardTypeFunc::Size();
		
		// 源地址
		m_objSrcAddr.ToStreamBuf(szBuf + nOffset);
		nOffset += CForwardAddr::Size();
		
		// 目标地址个数
		WORD wDestAddrCount = m_vecDestAddr.size();
		SETSTREAMSHORT(szBuf + nOffset, wDestAddrCount);
		nOffset += 2;
		
		// 目标地址集合
		for (int x = 0; x < wDestAddrCount; x++)
		{
			CForwardAddr* pDestAddr = &m_vecDestAddr[x];
			pDestAddr->ToStreamBuf(szBuf + nOffset);
			nOffset += CForwardAddr::Size();
		}
	}
	else
	{
		// 转发信息长度
		m_wForwardInfoLen = 0;
		SETSTREAMSHORT(szBuf + nOffset, m_wForwardInfoLen);
		nOffset += 2;
	}
	
	// 消息体长度
	SETSTREAMINT(szBuf + nOffset, m_dwMsgBodyLen);
	nOffset += 4;

	return nOffset;
}

bool CProtocolForward::CForwardHead::IsValid(char* szErrInfo, int nErrLen)
{
	if (m_byPackType != FORWARDPKT_TYPE_REGISTER		&& 
		m_byPackType != FORWARDPKT_TYPE_FORWARD			&&
		m_byPackType != FORWARDPKT_TYPE_COMMUSE)
	{
		snprintf(szErrInfo, nErrLen, "PackType(%d) is invalid!", m_byPackType);
		return false;
	}

	if (m_byPackType == FORWARDPKT_TYPE_FORWARD) // 为转发包
	{
		if (m_wForwardInfoLen == 0)
		{
			snprintf(szErrInfo, nErrLen, "PackType is forward, but ForwardInfoLen(%d) is invalid!", m_wForwardInfoLen);
			return false;
		}

		if (m_byForwardType != FORWARDTYPE_LOCAL	&& 
			m_byForwardType != FORWARDTYPE_NOLOCAL)
		{
			snprintf(szErrInfo, nErrLen, "PackType is forward, but ForwardType(%d) is invalid!", m_byForwardType);
			return false;
		}
		
		if (m_objTypeFunc.GetType() == 0 || m_objTypeFunc.GetType() == 0xff) // type必须要填
		{
			snprintf(szErrInfo, nErrLen, "PackType is forward, but Type(%d) is invalid!", m_objTypeFunc.GetType());
			return false;
		}

		if (m_objTypeFunc.GetFunc() == 0 || m_objTypeFunc.GetFunc() == 0xff) // func必须要填
		{
			snprintf(szErrInfo, nErrLen, "PackType is forward, but Func(%d) is invalid!", m_objTypeFunc.GetFunc());
			return false;
		}

		// 源地址部分的限制，必须要填单位类型、单位id、客户端id
		if (CForwardAddr::IsInvalidUnitType(m_objSrcAddr.GetUnitType())	||
			CForwardAddr::IsInvalidUnitId(m_objSrcAddr.GetUnitId())		||
			CForwardAddr::IsInvalidCltId(m_objSrcAddr.GetCltId()))
		{
			snprintf(szErrInfo, nErrLen, "PackType is forward, but SrcAddr(UnitType=%d, UnitId=%d, CltId=%d) is invalid!",
				m_objSrcAddr.GetUnitType(), m_objSrcAddr.GetUnitId(), m_objSrcAddr.GetCltId());
			return false;
		}

		if (m_vecDestAddr.size() > FORWARDPKT_DESTADDR_MAXCOUNT) // 目标地址个数太多
		{
			snprintf(szErrInfo, nErrLen, "PackType is forward, but DestAddrCount(%d) is invalid! out of range(0,%d)",
				m_vecDestAddr.size(), FORWARDPKT_DESTADDR_MAXCOUNT);
			return false;
		}
	}
	else
	{
		if (m_wForwardInfoLen != 0) // 不为转发包，则转发信息长度应该为0
		{
			snprintf(szErrInfo, nErrLen, "PackType is not forward, but ForwardInfoLen(%d) is invalid! ", m_wForwardInfoLen);
			return false;
		}
	}
	
	return true;
}

void CProtocolForward::CForwardHead::SetMsgBodyLen(DWORD dwLen)
{
	m_dwMsgBodyLen = dwLen;
}

DWORD CProtocolForward::CForwardHead::GetMsgBodyLen()
{
	return m_dwMsgBodyLen;
}

//==============================================================================
//                 
//                     CForwardRegPkt
//                      注册包类
//
//==============================================================================
CProtocolForward::CForwardRegPkt::CForwardRegPkt()
{
	m_dwReqId = 0;
	m_pForwardCond = new CForwardCondition();
}

CProtocolForward::CForwardRegPkt::~CForwardRegPkt()
{
	delete m_pForwardCond;
}

CForwardCondition& CProtocolForward::CForwardRegPkt::GetForwardCond()
{
	return *m_pForwardCond;
}

DWORD CProtocolForward::CForwardRegPkt::GetReqId()
{
	return m_dwReqId;
}

void CProtocolForward::CForwardRegPkt::SetReqId(DWORD dw)
{
	m_dwReqId = dw;
}

int CProtocolForward::CForwardRegPkt::FromStreamBuf(const char* szBuf, int nBufLen)
{
	if (nBufLen < 6) // min size
	{
		return -1;
	}
	
	int nOffset = 0;
	
	m_dwReqId = GETSTREAMINT(szBuf + nOffset);
	nOffset += 4;
	
	WORD wXmlLen = GETSTREAMSHORT(szBuf + nOffset);
	nOffset += 2;
	
	if (nBufLen - nOffset < wXmlLen)
	{
		return -1;
	}
	
	if (wXmlLen > 0)
	{
		char szXmlStr[0xffff+1];
		memset(szXmlStr, 0, sizeof(szXmlStr));
		memcpy(szXmlStr, szBuf + nOffset, wXmlLen);
		szXmlStr[wXmlLen] = 0;
		nOffset += wXmlLen;
		
		bool bRet = GetForwardCond().Init(szXmlStr);
		if (!bRet)
		{
			return -1;
		}
	}

	return nOffset;
}

int CProtocolForward::CForwardRegPkt::ToStreamBuf(char* szBuf, int& nBufLen)
{
	string& strXmlStr = GetForwardCond().GetXmlStr();
	if (strXmlStr.size() >= 0xffff)
	{
		return -1;
	}

	WORD wXmlLen = strXmlStr.size();
	if (nBufLen < 6 + wXmlLen)
	{
		nBufLen = 6 + wXmlLen;
		return -1;
	}
	
	int nOffset = 0;
	
	SETSTREAMINT(szBuf + nOffset, m_dwReqId);
	nOffset += 4;
	
	SETSTREAMSHORT(szBuf + nOffset, wXmlLen);
	nOffset += 2;
	
	if (wXmlLen > 0)
	{
		memcpy(szBuf + nOffset, strXmlStr.c_str(), wXmlLen);
		nOffset += wXmlLen;
	}

	return nOffset;
}

int CProtocolForward::CForwardRegPkt::ToStreamBuf(CBuffer* pBuf)
{
	int nBufLen = pBuf->GetAvailableSize();
	int nLen = ToStreamBuf(pBuf->GetAvailableBuf(), nBufLen);
	if (nLen < 0)
	{
		pBuf->Extend(nBufLen);
		nBufLen = pBuf->GetAvailableSize();
		nLen = ToStreamBuf(pBuf->GetAvailableBuf(), nBufLen);
		if (nLen < 0)
		{
			return -1;
		}
	}

	pBuf->Use(nLen);

	return nLen;
}

//==============================================================================
//                 
//                    CProtocolForward
//                     转发层协议类
//
//==============================================================================
CProtocolForward::CProtocolForward() 
{ 
} 

CProtocolForward::~CProtocolForward() 
{ 
} 

bool CProtocolForward::IsMsgForwardPkt(const char* szBuf, int nLen)
{
	if (!szBuf || nLen < 9)
	{
		return false;
	}

	return ((BYTE)(szBuf[8])) == FORWARDPKT_TYPE_FORWARD;
}

bool CProtocolForward::IsMsgRegisterPkt(const char* szBuf, int nLen)
{
	if (!szBuf || nLen < 9)
	{
		return false;
	}
	
	return ((BYTE)(szBuf[8])) == FORWARDPKT_TYPE_REGISTER;
}

bool CProtocolForward::IsValid(const char* szBuf, int nBufLen, char* szErrInfo, int nErrLen)
{
	if (!szBuf || nBufLen <= 0)
	{
		snprintf(szErrInfo, nErrLen, "input parameter is error! Buf is null or BufLen is zero!");
		return false;
	}

	CForwardHead objForwardHead;
	int nHeadLen = objForwardHead.FromStreamBuf(szBuf, nBufLen); // 消息头
	if (nHeadLen <= 0)
	{
		snprintf(szErrInfo, nErrLen, "CForwardHead::FromStream()=%d! ", nHeadLen);
		return false;
	}

	if (!objForwardHead.IsValid(szErrInfo, nErrLen))
	{
		return false;
	}

	if (objForwardHead.IsForwardPack())
	{
		if (nBufLen < nHeadLen + 2)
		{
			snprintf(szErrInfo, nErrLen, "len is not enough to get type_func after forward head! ");
			return false;
		}

		BYTE byType = (BYTE)szBuf[nHeadLen + 0];
		BYTE byFunc = (BYTE)szBuf[nHeadLen + 1];

		if (objForwardHead.GetTypeFunc().GetType() != byType ||
			objForwardHead.GetTypeFunc().GetFunc() != byFunc)
		{
			snprintf(szErrInfo, nErrLen, "forward head(type=%d,func=%d) is not equal data body(type=%d,func=%d)! ", 
				objForwardHead.GetTypeFunc().GetType(), objForwardHead.GetTypeFunc().GetFunc(), byType, byFunc);
			return false;
		}
	}

	return true;
}

bool CProtocolForward::GetDataSeq(const char* szBuf, int nLen, CForwardDataSeq& objDataSeq)
{
	if (!szBuf || nLen < CForwardDataSeq::Size())
	{
		return false;
	}
	
	if (objDataSeq.FromStreamBuf(szBuf) < 0)
	{
		return false;
	}
	
	return true;
}

bool CProtocolForward::SetDataSeq( char* szBuf, int nLen, CForwardDataSeq& objDataSeq )
{
	if (!szBuf || nLen < CForwardDataSeq::Size())
	{
		return false;
	}
	
	if (objDataSeq.ToStreamBuf(szBuf) < 0)
	{
		return false;
	}
	
	return true;
}

bool CProtocolForward::GetSrcAddr(const char* szBuf, int nLen, CCommAddr& objSrcAddr)
{
	if (!szBuf || nLen <= 0)
	{
		return false;
	}

	if (!IsMsgForwardPkt(szBuf, nLen))
	{
		return false;
	}

	int nOffset = CForwardDataSeq::Size() + 1 + 2 + 1 + CForwardTypeFunc::Size();

	if (nLen < nOffset + CForwardAddr::Size())
	{
		return false;
	}

	CForwardAddr objForwardAddr;
	if (objForwardAddr.FromStreamBuf(szBuf + nOffset) < 0)
	{
		return false;
	}
	objSrcAddr = (CCommAddr)objForwardAddr;

	return true;
}

bool CProtocolForward::SetSrcAddr(char* szBuf, int nLen, CCommAddr& objSrcAddr)
{
	if (!szBuf || nLen <= 0)
	{
		return false;
	}
	
	if (!IsMsgForwardPkt(szBuf, nLen))
	{
		return false;
	}
	
	int nOffset = CForwardDataSeq::Size() + 1 + 2 + 1 + CForwardTypeFunc::Size();
	
	if (nLen < nOffset + CForwardAddr::Size())
	{
		return false;
	}
	
	CForwardAddr objForwardAddr;
	CCommAddr* pCommAddr = (CCommAddr*)&objForwardAddr;
	*pCommAddr = objSrcAddr;
	if (objForwardAddr.ToStreamBuf(szBuf + nOffset) < 0)
	{
		return false;
	}
	
	return true;
}

bool CProtocolForward::UpdateTypeFunc( char* szBuf, int nLen )
{
	if (!szBuf || nLen <= 0)
	{
		return false;
	}
	
	if (!IsMsgForwardPkt(szBuf, nLen))
	{
		return false;
	}
	
	CForwardHead objForwardHead;
	int nHeadLen = objForwardHead.FromStreamBuf(szBuf, nLen); // 消息头
	if (nHeadLen <= 0)
	{
		return false;
	}

	if (nLen < nHeadLen + 2)
	{
		return false;
	}
	
	BYTE byType = (BYTE)szBuf[nHeadLen];
	BYTE byFunc = (BYTE)szBuf[nHeadLen + 1];

	int nOffset = CForwardDataSeq::Size() + 1 + 2 + 1;
	szBuf[nOffset] = byType;
	szBuf[nOffset + 1] = byFunc;

	return true;
}

bool CProtocolForward::MakeRegPack(CBuffer* pBuf, CForwardCondition& objForwardCondition, DWORD dwReqId)
{
	if (!pBuf)
	{
		return false;
	}

	CForwardRegPkt objRegPkt;
	objRegPkt.SetReqId(dwReqId);
	objRegPkt.GetForwardCond() = objForwardCondition;
	
	string& strXmlStr = objRegPkt.GetForwardCond().GetXmlStr();
	if (strXmlStr.size() >= 0xffff)
	{
		return false;
	}
	
	CForwardHead objForwardHead;
	objForwardHead.SetPackType(FORWARDPKT_TYPE_REGISTER);
	objForwardHead.SetMsgBodyLen(6 + strXmlStr.size());

	pBuf->Empty();
	int nBufNeedLen = pBuf->GetAvailableSize();
	int nHeadLen = objForwardHead.ToStreamBuf(pBuf->GetBuffer(), nBufNeedLen);
	if (nHeadLen < 0)
	{
		pBuf->Extend(nBufNeedLen);
		nBufNeedLen = pBuf->GetAvailableSize();
		nHeadLen = objForwardHead.ToStreamBuf(pBuf->GetBuffer(), nBufNeedLen);
		if (nHeadLen < 0)
		{
			pBuf->Empty();
			return false;
		}
	}
	pBuf->Use(nHeadLen);

	nBufNeedLen = pBuf->GetAvailableSize();
	int nRegLen = objRegPkt.ToStreamBuf(pBuf->GetBuffer() + nHeadLen, nBufNeedLen);
	if (nRegLen < 0)
	{
		pBuf->Extend(nBufNeedLen);
		nBufNeedLen = pBuf->GetAvailableSize();
		nRegLen = objRegPkt.ToStreamBuf(pBuf->GetBuffer() + nHeadLen, nBufNeedLen);
		if (nRegLen < 0)
		{
			pBuf->Empty();
			return false;
		}
	}
	pBuf->Use(nRegLen);
	
	return true;
}

string CProtocolForward::GetXmlRegStr(char* szBuf, int nBufLen)
{
	if (!szBuf || nBufLen == 0)
	{
		return "";
	}
	
	int nOffsetXmlLen = 19; // 注册记录长度位置
	if (nBufLen < nOffsetXmlLen + 2)
	{
		return "";
	}

	WORD wXmlLen = GETSTREAMSHORT(szBuf + nOffsetXmlLen);

	if (nBufLen < nOffsetXmlLen + 2 + wXmlLen)
	{
		return "";
	}

	char* sz = new char[wXmlLen + 1];
	memcpy(sz, szBuf + nOffsetXmlLen + 2, wXmlLen);
	sz[wXmlLen] = 0;

	string str;
	str = sz;
	delete []sz;

	return str;
}

bool CProtocolForward::SetXmlRegStr(CBuffer* pBuf, const char* szXmlReg)
{
	if (!pBuf || !szXmlReg)
	{
		return false;
	}

	int nOffsetMsgLen = 11;	// 消息体长度位置
	int nOffsetXmlLen = 19; // 注册记录长度位置
	int nOffsetXml = 21;	// 注册记录位置

	WORD wXmlLen = strlen(szXmlReg);

	pBuf->Empty();
	int nNeedLen = 21 + wXmlLen;
	if (nNeedLen > pBuf->GetBufSize())
	{
		if (!pBuf->Extend(nNeedLen))
		{
			return false;
		}
	}

	int nMsLen = 6 + wXmlLen;
	SETSTREAMINT(pBuf->GetBuffer() + nOffsetMsgLen, nMsLen);		// 修改消息体长度
	SETSTREAMSHORT(pBuf->GetBuffer() + nOffsetXmlLen, wXmlLen);		// 修改注册记录长度
	memcpy(pBuf->GetBuffer() + nOffsetXml, szXmlReg, wXmlLen);
	pBuf->Use(nNeedLen);

	return true;
}

bool CProtocolForward::GetForwardHeadTypeFunc(char* szBuf, int nLen, CForwardTypeFunc& objTypeFunc)
{
	if (!szBuf || nLen <= 0)
	{
		return false;
	}
	
	if (!IsMsgForwardPkt(szBuf, nLen))
	{
		return false;
	}
	
	int nOffset = CForwardDataSeq::Size() + 1 + 2 + 1;
	
	if (nLen < nOffset + CForwardTypeFunc::Size())
	{
		return false;
	}
	
	if (objTypeFunc.FromStreamBuf(szBuf + nOffset) < 0)
	{
		return false;
	}
	
	return true;	
}
