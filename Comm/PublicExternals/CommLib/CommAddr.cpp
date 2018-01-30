//==============================================================================
//                      CommAddr.cpp
//
//begin   : 2016-09-12
//describe:
//==============================================================================
#include "CommAddr.h"

//==============================================================================
//                 
//                  CCommAddr
//                  通信地址类
//
//==============================================================================
CCommAddr::CCommAddr()
{
	m_byBureauId = 0;
	m_byUnitType = 0;
	m_wUnitId = 0;
	m_byDevType = 0;
	m_wDevId = 0;
	m_byAbFlag = 0;
	m_wCltId = 0;
}

CCommAddr::~CCommAddr() 
{ 
} 

BYTE CCommAddr::GetBureauId()
{
	return m_byBureauId;
}

void CCommAddr::SetBureauId(BYTE by)
{
	m_byBureauId = by;
}

BYTE CCommAddr::GetUnitType()
{
	return m_byUnitType;
}

void CCommAddr::SetUnitType(BYTE by)
{
	m_byUnitType = by;
}

WORD CCommAddr::GetUnitId()
{
	return m_wUnitId;
}

void CCommAddr::SetUnitId(WORD w)
{
	m_wUnitId = w;
}

BYTE CCommAddr::GetDevType()
{
	return m_byDevType;
}

void CCommAddr::SetDevType(BYTE by)
{
	m_byDevType = by;
}

WORD CCommAddr::GetDevId()
{
	return m_wDevId;
}

void CCommAddr::SetDevId(WORD w)
{
	m_wDevId = w;
}

BYTE CCommAddr::GetAbFlag()
{
	return m_byAbFlag;
}

void CCommAddr::SetAbFlag( BYTE byAbFlag )
{
	m_byAbFlag = byAbFlag;
}

WORD CCommAddr::GetCltId()
{
	return m_wCltId;
}

void CCommAddr::SetCltId(WORD w)
{
	m_wCltId = w;
}

bool CCommAddr::IsInvalidBureauId(BYTE by)
{
	return by == 0 || by == 0xff;
}

bool CCommAddr::IsInvalidUnitType(BYTE by)
{
	return by == 0 || by == 0xff;
}

bool CCommAddr::IsInvalidUnitId(WORD w)
{
	return w == 0 || w == 0xffff;
}

bool CCommAddr::IsInvalidDevType(BYTE by)
{
	return by == 0 || by == 0xff;
}

bool CCommAddr::IsInvalidDevId(WORD w)
{
	return w == 0 || w == 0xffff;
}

bool CCommAddr::IsInvalidCltId(WORD w)
{
	return w == 0 || w == 0xffff;
}

bool CCommAddr::IsInvalidAbFlag( BYTE byAbFlag )
{
	return byAbFlag == 0 || byAbFlag == 1;
}

string CCommAddr::GetDumpStr()
{
	char szBuf[256];
	memset(szBuf, 0, sizeof(szBuf));

	snprintf(szBuf, sizeof(szBuf), "BureauId=%d, UnitType=%d, UnitId=%d, DevType=%d, DevId=%d, AbFlag=%d, CltId=%d",
		m_byBureauId, m_byUnitType, m_wUnitId, m_byDevType, m_wDevId, m_byAbFlag, m_wCltId);

	return szBuf;
}

bool CCommAddr::operator==( const CCommAddr& objOther )
{
	return objOther.m_byBureauId == m_byBureauId 
		&& objOther.m_byUnitType == m_byUnitType
		&& objOther.m_wUnitId == m_wUnitId
		&& objOther.m_byDevType == m_byDevType
		&& objOther.m_wDevId == m_wDevId
		&& objOther.m_byAbFlag == m_byAbFlag
		&& objOther.m_wCltId == m_wCltId;
}

bool CCommAddr::IsMatched(CCommAddr& obj)
{
	if (m_byBureauId != 0xff && obj.GetBureauId() != 0xff &&
		m_byBureauId != obj.GetBureauId())
	{
		return false;
	}

	if (m_byUnitType != 0xff && obj.GetUnitType() != 0xff &&
		m_byUnitType != obj.GetUnitType())
	{
		return false;
	}
	
	if ( m_wUnitId != 0xffff && obj.GetUnitId() != 0xffff &&
		m_wUnitId != obj.GetUnitId())
	{
		return false;
	}
	
	if (m_byDevType != 0xff && obj.GetDevType() != 0xff &&
		m_byDevType != obj.GetDevType())
	{
		return false;
	}
	
	if (m_wDevId != 0xffff && obj.GetDevId() != 0xffff &&
		m_wDevId != obj.GetDevId())
	{
		return false;
	}

	if (m_byAbFlag != 0xff && obj.GetAbFlag() != 0xff &&
		m_byAbFlag != obj.GetAbFlag())
	{
		return false;
	}

	if (m_wCltId != 0xffff && obj.GetCltId() != 0xffff &&
		m_wCltId != obj.GetCltId())
	{
		return false;
	}

	return true;
}

