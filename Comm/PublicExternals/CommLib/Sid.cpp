//==============================================================================
//                      Sid.cpp
//
//begin   : 2016-08-23
//describe:
//==============================================================================

#include "Sid.h"

//==============================================================================
//                 
//                    CSid
//               ª·ª∞id£®session id£©¿‡
//
//==============================================================================
int CSid::m_nCounter = 0; 
 
CSid::CSid() : m_nHigh(0), m_nLow(0) 
{ 
} 
 
CSid::~CSid() 
{ 
} 
 
void CSid::Asign(unsigned int nHigh) 
{
	m_nHigh = nHigh;
	m_nLow = m_nCounter++; 
	if (m_nLow == 0)
	{
		m_nLow = m_nCounter++; 
	}
} 

void CSid::Abolish() 
{ 
	m_nHigh = 0; 
	m_nLow = 0; 
} 

bool CSid::IsValid() const 
{ 
	return (m_nLow != 0); 
} 

int CSid::GetHigh() const
{ 
	return m_nHigh;	
} 

int CSid::GetLow() const
{ 
	return m_nLow;
} 

void CSid::Import(int nHigh, int nLow) 
{ 
	m_nHigh = nHigh; 
	m_nLow = nLow; 
} 
 
int CSid::StreamSize() 
{ 
	return 8;
}

int CSid::ToStreamBuf(char* szBuf) const 
{ 
	SETSTREAMINT(szBuf, m_nHigh); 
	SETSTREAMINT(szBuf + sizeof(m_nHigh), m_nLow); 
	return sizeof(m_nHigh) + sizeof(m_nLow); 
} 

int CSid::FromStreamBuf(const char* szBuf) 
{ 
	m_nHigh = GETSTREAMINT(szBuf); 
	m_nLow = GETSTREAMINT(szBuf + sizeof(m_nHigh)); 
	return sizeof(m_nHigh) + sizeof(m_nLow); 
} 

CSid& CSid::operator=(CSid& objSid)
{
	m_nHigh = objSid.m_nHigh;
	m_nLow = objSid.m_nLow;
	return *this;
}

bool CSid::operator ==(CSid objSid) const 
{ 
	return (m_nHigh == objSid.m_nHigh && m_nLow == objSid.m_nLow); 
} 
 
bool CSid::operator!=(CSid objSid) const 
{ 
	return (m_nHigh != objSid.m_nHigh || m_nLow != objSid.m_nLow); 
} 
 
