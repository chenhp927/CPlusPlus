//==============================================================================
//                      CProtocolSession.h
//
//begin   : 2016-05-13
//describe:
//==============================================================================
#include <assert.h>
#include <stdio.h>

#include "ProtocolSession.h" 

//==============================================================================
//                 
//                     CSessionHead
//                     会话层包头类
//
//==============================================================================
CProtocolSession::CSessionHead::CSessionHead() :  
		m_dwVersion(CUR_SESSIONVERSION), m_dwDataLen(0), m_cType(0), m_dwSeq(0)
{ 
} 
 
CProtocolSession::CSessionHead::CSessionHead(DWORD dwDataLen, char cType, DWORD dwSeq) :  
		m_dwVersion(CUR_SESSIONVERSION), m_dwDataLen(dwDataLen), m_cType(cType), m_dwSeq(dwSeq)  
{ 
} 
 
CProtocolSession::CSessionHead::~CSessionHead() 
{ 
} 
 
int CProtocolSession::CSessionHead::HeaderSize()
{
	return 13;
}
 
bool CProtocolSession::CSessionHead::IsHeadValid(const char* szBuf, int nLen) 
{ 
	bool bRet = false; 
	if (nLen >= HeaderSize()) 
	{ 
		bRet = true; 
		const char* p = szBuf; 

		int nVer = GETSTREAMINT(p); 
		p += 4; 

		int nDataLen = GETSTREAMINT(p); 
		p += 4; 

	//	char cType = *p; 
		p++;
		
	//	DWORD dwSeq = GETSTREAMINT(p); 
		p += 4; 
 
		if (nVer != CUR_SESSIONVERSION) 
		{ 
			bRet = false; 
		} 
		else if (nDataLen + HeaderSize() != nLen) 
		{ 
			bRet = false; 
		} 
	} 

	return bRet; 
} 
 
int CProtocolSession::CSessionHead::ToStreamBuf(char* szBuf) const 
{ 
	char* p = szBuf;

	SETSTREAMINT(p, m_dwVersion); 
	p += 4; 

	SETSTREAMINT(p, m_dwDataLen); 
	p += 4; 

	*p = m_cType; 
	p++; 

	SETSTREAMINT(p, m_dwSeq); 
	p += 4; 

	return p - szBuf; 
} 
 
int CProtocolSession::CSessionHead::FromStreamBuf(const char* szBuf) 
{ 
	const char* p = szBuf; 

	m_dwVersion = GETSTREAMINT(p);
	p += 4; 

	m_dwDataLen = GETSTREAMINT(p); 
	p += 4; 

	m_cType = *p; 
	p++; 

	m_dwSeq = GETSTREAMINT(p); 
	p += 4; 

	return p - szBuf; 
} 
 
//==============================================================================
//                 
//                     CCSessionNegReq
//                   会话层协商请求包类
//
//==============================================================================
CProtocolSession::CCSessionNegReq::CCSessionNegReq() :  
		CSessionHead(14, SPT_PARAMNEG, 0)  
{ 
	m_byClientTag = CLIENTTAG_APP;
	m_dwPairNetIp = 0;
} 
 
CProtocolSession::CCSessionNegReq::CCSessionNegReq(DWORD dwNextSeq) :  
		CSessionHead(14, SPT_PARAMNEG, dwNextSeq)
{ 
	m_byClientTag = CLIENTTAG_APP;
	m_dwPairNetIp = 0;
} 
 
CProtocolSession::CCSessionNegReq::~CCSessionNegReq() 
{ 
} 
 
int CProtocolSession::CCSessionNegReq::Size() 
{ 
	return HeaderSize() + 14; 
} 
 
bool CProtocolSession::CCSessionNegReq::IsValid(const char* szBuf, int nLen)  
{ 
	if (!IsHeadValid(szBuf, nLen))
	{
		return false;
	}
	
	return nLen == Size(); 
} 
 
int CProtocolSession::CCSessionNegReq::ToStreamBuf(char* szBuf) 
{ 
	int nOffset = CSessionHead::ToStreamBuf(szBuf); 
	char* p = szBuf + nOffset; 
	
	*p = m_objClientAddr.GetBureauId();
	p++;
	
	*p = m_objClientAddr.GetUnitType();
	p++;
	
	SETSTREAMSHORT(p, m_objClientAddr.GetUnitId()); 
	p += 2;
	
	*p = m_objClientAddr.GetDevType();
	p++;
	
	SETSTREAMSHORT(p, m_objClientAddr.GetDevId()); 
	p += 2;
	
	SETSTREAMSHORT(p, m_objClientAddr.GetCltId()); 
	p += 2;
	
	*p = m_byClientTag; 
	p++; 

	SETSTREAMINT(p, m_dwPairNetIp);
	p += 4;

	return p - szBuf; 
} 
 
int CProtocolSession::CCSessionNegReq::FromStreamBuf(const char* szBuf) 
{ 
	int nOffset = CSessionHead::FromStreamBuf(szBuf); 

	const char* p = szBuf + nOffset; 
	
	m_objClientAddr.SetBureauId(*p);
	p++;
	
	m_objClientAddr.SetUnitType(*p);
	p++;
	
	m_objClientAddr.SetUnitId(GETSTREAMSHORT(p)); 
	p += 2;
	
	m_objClientAddr.SetDevType(*p);
	p++;
	
	m_objClientAddr.SetDevId(GETSTREAMSHORT(p)); 
	p += 2;
	
	m_objClientAddr.SetCltId(GETSTREAMSHORT(p)); 
	p += 2;
	
	m_byClientTag = *p; 
	p++; 
	
	m_dwPairNetIp = GETSTREAMINT(p);
	p += 4;

	return p - szBuf; 
} 
 
//==============================================================================
//                 
//                     CCSessionNegAck
//                   会话层协商应答包类
//
//==============================================================================
CProtocolSession::CCSessionNegAck::CCSessionNegAck() :  
		CSessionHead(0, SPT_PARAMACK, 0)  
{ 
	m_dwDataLen = Size() - HeaderSize(); 
} 
 
CProtocolSession::CCSessionNegAck::CCSessionNegAck(DWORD dwNextSeq,
	char cWinSize, char cAckInter, char cAckTimeout, char cHeartInter, char cHeartTimeout) :  
	CSessionHead(0, SPT_PARAMACK, dwNextSeq),  
	m_cSendWindowSize(cWinSize), 
	m_cAckInterval(cAckInter),  
	m_cAckTimeout(cAckTimeout), 
	m_cHeartInterval(cHeartInter),  
	m_cHeartTimeout(cHeartTimeout)  
{	 
	m_dwDataLen = Size() - HeaderSize();	 
} 
 
CProtocolSession::CCSessionNegAck::~CCSessionNegAck() 
{ 
} 
 
int CProtocolSession::CCSessionNegAck::Size()	 
{  
	return HeaderSize() + 5 * sizeof(char);	 
} 
 
bool CProtocolSession::CCSessionNegAck::IsValid(const char* szBuf, int nLen) 
{ 
	if (!IsHeadValid(szBuf, nLen))
	{
		return false;
	}

	return nLen == Size(); 
} 
 
int CProtocolSession::CCSessionNegAck::ToStreamBuf(char* szBuf) 
{ 
	int nLen = CSessionHead::ToStreamBuf(szBuf); 
	char* p = szBuf + nLen; 

	*p = m_cSendWindowSize; 
	p++; 

	*p = m_cAckInterval; 
	p++; 

	*p = m_cAckTimeout; 
	p++; 

	*p = m_cHeartInterval; 
	p++; 

	*p = m_cHeartTimeout; 
	p++; 

	return p - szBuf; 
} 
 
int CProtocolSession::CCSessionNegAck::FromStreamBuf(const char* szBuf) 
{ 
	int nLen = CSessionHead::FromStreamBuf(szBuf); 
	const char* p = szBuf + nLen; 

	m_cSendWindowSize = *p; 
	p++; 

	m_cAckInterval = *p; 
	p++; 

	m_cAckTimeout = *p; 
	p++; 

	m_cHeartInterval = *p; 
	p++; 

	m_cHeartTimeout = *p; 
	p++; 

	return p - szBuf; 
} 
 
//==============================================================================
//                 
//                    CProtocolSession
//                    基本通信会话协议类
//
//==============================================================================
CProtocolSession::CProtocolSession() 
{ 
} 
 
CProtocolSession::~CProtocolSession() 
{ 
} 
 
int CProtocolSession::DataType(const char *szBuf) 
{ 
	CSessionHead objHead; 
	objHead.FromStreamBuf(szBuf); 
	return objHead.m_cType; 
} 

int  CProtocolSession::HeaderSize() 
{ 
	return CSessionHead::HeaderSize(); 
} 

bool CProtocolSession::IsValid(const char *szBuf, int nLen) 
{ 
	bool bRet = true; 
	if (!CSessionHead::IsHeadValid(szBuf, nLen)) 
	{ 
		bRet = false; 
	} 
	else 
	{ 
		char cType = DataType(szBuf); 
		switch (cType) 
		{ 
		case SPT_PARAMNEG: 
			bRet = CCSessionNegReq::IsValid(szBuf, nLen); 
			break; 
			
		case SPT_PARAMACK: 
			bRet = CCSessionNegAck::IsValid(szBuf, nLen); 
			break;
			
		default: 
			break; 
		} 
	} 
	return bRet; 
} 

int CProtocolSession::StreamPacketSize(const char* szBuf) 
{ 
	return HeaderSize() + StreamDataSize(szBuf); 
} 

int CProtocolSession::StreamDataSize(const char* szBuf) 
{ 
	CSessionHead objHead; 
	objHead.FromStreamBuf(szBuf); 
	return objHead.m_dwDataLen; 
} 

int  CProtocolSession::MinPacketSize()	 
{ 
	return HeaderSize(); 
} 

int  CProtocolSession::MaxPacketSize()	 
{ 
	return MAX_SESSIONPACKETSIZE; 
} 

bool CProtocolSession::IsHeart(const char* pPkt) 
{ 
	return DataType(pPkt) == SPT_HEART; 
} 

bool CProtocolSession::IsData(const char* pPkt) 
{ 
	return DataType(pPkt) == SPT_DATA; 
} 

bool CProtocolSession::IsDataAck(const char* pPkt) 
{ 
	return DataType(pPkt) == SPT_DATAACK; 
} 

bool CProtocolSession::IsNegReq(const char* pPkt) 
{ 
	return DataType(pPkt) == SPT_PARAMNEG; 
} 

bool CProtocolSession::IsNegAck(const char* pPkt) 
{ 
	return DataType(pPkt) == SPT_PARAMACK; 
} 
 
int CProtocolSession::MakeHeartPacket(char* szBuf, int nLen) 
{ 
	assert(nLen >= HeaderSize()); 

	CSessionHead objHead(0, SPT_HEART, 0); 
	return objHead.ToStreamBuf(szBuf); 
} 
 
int CProtocolSession::MakeDataAckPacket(char* szBuf, int nLen, DWORD dwSeq) 
{ 
	assert(nLen >= HeaderSize()); 

	CSessionHead objHead(0, SPT_DATAACK, dwSeq); 
	return objHead.ToStreamBuf(szBuf); 
} 
 
int CProtocolSession::MakeNegReqPacket(char* szBuf, int nLen, DWORD dwNextSeq, 
		CCommAddr& objCltAddr, BYTE byCltTag, DWORD dwPairNetIp) 
{ 
	assert(nLen >= CCSessionNegReq::Size()); 

	CCSessionNegReq objNegReq(dwNextSeq); 
	objNegReq.m_objClientAddr = objCltAddr;
	objNegReq.m_byClientTag = byCltTag;
	objNegReq.m_dwPairNetIp = dwPairNetIp;

	return objNegReq.ToStreamBuf(szBuf); 
} 
 
int CProtocolSession::MakeNegAckPacket(char* szBuf, int nLen,  
					DWORD dwNextSeq, char cWinSize, char cAckInter, 
					  char cAckTimeout, char cHeartInter, char cHeartTimeout) 
{ 
	assert(nLen >= CCSessionNegAck::Size()); 

	CCSessionNegAck objNegAck(dwNextSeq, cWinSize, cAckInter, cAckTimeout, cHeartInter, cHeartTimeout); 
	return objNegAck.ToStreamBuf(szBuf); 
} 
 
DWORD CProtocolSession::GetPacketSeq(const char* szBuf) 
{ 
	return GETSTREAMINT(szBuf + 9);
} 

void CProtocolSession::SetPacketSeq(char* szBuf, DWORD dwSeq)
{
	SETSTREAMINT(szBuf + 9, dwSeq);
}

char CProtocolSession::GetWinSizeFromNegAck(const char* szBuf) 
{ 
	CCSessionNegAck objNegAck; 
	objNegAck.FromStreamBuf(szBuf); 
	return objNegAck.m_cSendWindowSize; 
} 
 
char CProtocolSession::GetAckIntervalFromNegAck(const char* szBuf) 
{ 
	CCSessionNegAck objNegAck; 
	objNegAck.FromStreamBuf(szBuf); 
	return objNegAck.m_cAckInterval; 
} 
 
char CProtocolSession::GetAckTimeoutFromNegAck(const char* szBuf) 
{ 
	CCSessionNegAck objNegAck; 
	objNegAck.FromStreamBuf(szBuf); 
	return objNegAck.m_cAckTimeout; 
} 
 
char CProtocolSession::GetHeartIntervalFromNegAck(const char* szBuf) 
{ 
	CCSessionNegAck objNegAck; 
	objNegAck.FromStreamBuf(szBuf); 
	return objNegAck.m_cHeartInterval; 
} 
 
char CProtocolSession::GetHeartTimeoutFromNegAck(const char* szBuf) 
{ 
	CCSessionNegAck objNegAck; 
	objNegAck.FromStreamBuf(szBuf); 
	return objNegAck.m_cHeartTimeout; 
} 
