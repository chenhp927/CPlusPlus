//==============================================================================
//                      CTime.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================

#include <string.h>
#include "CTime.h"
 
//==============================================================================
//                 
//                    CTimeSpan
//                    
//
//==============================================================================
CTimeSpan::CTimeSpan() 
{ 
	m_tv.tv_sec = 0; 
	m_tv.tv_usec = 0; 
} 
 
CTimeSpan::CTimeSpan( time_t time ) 
{ 
	m_tv.tv_sec = time; 
	m_tv.tv_usec = 0; 
} 
 
CTimeSpan::CTimeSpan( int nDays, int nHours, int nMins, int nSecs ) 
{ 
	m_tv.tv_sec = nSecs + 60 * ( nMins + 60 * ( nHours + 24 * nDays)); 
	m_tv.tv_usec = 0; 
} 
 
int CTimeSpan::GetDays() const 
{ 
	return m_tv.tv_sec / 60 / 60 / 24; 
} 
 
int CTimeSpan::GetHours() const 
{ 
	return m_tv.tv_sec / 60 / 60; 
} 
 
int CTimeSpan::GetMinutes() const 
{ 
	return m_tv.tv_sec / 60; 
} 
	 
int CTimeSpan::GetSeconds() const 
{ 
	return m_tv.tv_sec; 
} 
 
int CTimeSpan::GetMilliseconds() const 
{ 
	return m_tv.tv_sec * 1000 + m_tv.tv_usec / 1000; 
} 
 
CTimeSpan CTimeSpan::operator +( CTimeSpan timeSpan ) const 
{ 
	CTimeSpan span; 
	span.m_tv.tv_sec = m_tv.tv_sec + timeSpan.m_tv.tv_sec; 
	span.m_tv.tv_usec = m_tv.tv_usec + timeSpan.m_tv.tv_usec; 
	if (span.m_tv.tv_usec >= 1000000) 
	{ 
		span.m_tv.tv_sec ++; 
		span.m_tv.tv_usec -= 1000000; 
	} 
	return span; 
} 
 
CTimeSpan CTimeSpan::operator -( CTimeSpan timeSpan ) const 
{ 
	CTimeSpan span; 
	span.m_tv.tv_sec = m_tv.tv_sec - timeSpan.m_tv.tv_sec; 
	span.m_tv.tv_usec = m_tv.tv_usec - timeSpan.m_tv.tv_usec; 
	if (span.m_tv.tv_usec >= 1000000) 
	{ 
		span.m_tv.tv_sec ++; 
		span.m_tv.tv_usec -= 1000000; 
	} 
	return span; 
} 
 
 
//==============================================================================
//                 
//                    CTime
//                    
//
//==============================================================================
CTime::CTime() 
{ 
	m_tv.tv_sec = 0; 
	m_tv.tv_usec = 0; 
} 
 
CTime::CTime(time_t t) 
{ 
	m_tv.tv_sec = t; 
	m_tv.tv_usec = 0; 
} 
 
CTime::~CTime() 
{ 
} 
 
struct tm* CTime::GetLocalTm(struct tm* ptm) const 
{ 
	if (ptm != NULL) 
	{ 
		//struct tm* ptmTemp = localtime(&m_tv.tv_sec); 
#ifdef HP_UX
		struct tm* ptmTemp = localtime(&m_tv.tv_sec); //非线程安全
		if (ptmTemp == NULL) 
			return NULL;    // indicates the m_time was not initialized! 
		*ptm = *ptmTemp; 
		return ptm; 
#else //linux
		struct tm* ptmTemp = localtime_r(&m_tv.tv_sec, ptm); //线程安全
		if (ptmTemp == NULL) 
			return NULL;    // indicates the m_time was not initialized! 
		//*ptm = *ptmTemp; 
		return ptm; 
#endif
	} 
	else 
		return localtime(&m_tv.tv_sec); //非线程安全
} 
 
CTime CTime::GetCurrentTime() 
{ 
	CTime t; 
	gettimeofday(&t.m_tv, NULL); 
	return t; 
} 
 
time_t CTime::GetTime() const 
{ 
	return m_tv.tv_sec; 
} 

timeval CTime::GetTimeStu() const
{
	return m_tv;
}

int	CTime::GetYear() const 
{
	struct tm temp; 
	//return (GetLocalTm(NULL)->tm_year) + 1900; 
	return (GetLocalTm(&temp)->tm_year) + 1900; //为了线程安全
} 
 
int CTime::GetMonth() const 
{ 
	struct tm temp; 
	return GetLocalTm(&temp)->tm_mon + 1; 
} 
 
int CTime::GetDay() const 
{ 
	struct tm temp; 
	return GetLocalTm(&temp)->tm_mday; 
} 
 
int CTime::GetHour() const 
{ 
	struct tm temp; 
	return GetLocalTm(&temp)->tm_hour; 
} 
 
int CTime::GetMinute() const 
{ 
	struct tm temp; 
	return GetLocalTm(&temp)->tm_min; 
} 
 
int CTime::GetSecond() const 
{ 
	struct tm temp; 
	return GetLocalTm(&temp)->tm_sec;  
} 
 
int CTime::GetMillisecond() const 
{ 
	return m_tv.tv_usec / 1000; 
} 
 
char *CTime::Format(char *buf) const 
{ 
	time_t t = m_tv.tv_sec; 
	//strcpy(buf, ctime(&t)); 
#ifdef HP_UX
	strcpy(buf, ctime(&t)); //非线程安全
#else //linux
	ctime_r(&t, buf); //线程安全
#endif
	buf[24] = 0; 
	return buf; 
} 

time_t CTime::MakeTime(int y, int m, int d, int h, int mi, int s)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	tm.tm_year = y - 1900;
	tm.tm_mon = m - 1;
	tm.tm_mday = d;
	tm.tm_hour = h;
	tm.tm_min = mi;
	tm.tm_sec = s;
	return mktime(&tm);
}

CTime& CTime::operator =( const CTime& timeSrc) 
{ 
	m_tv.tv_sec = timeSrc.m_tv.tv_sec; 
	m_tv.tv_usec = timeSrc.m_tv.tv_usec; 
	return *this; 
} 
 
CTime CTime::operator +( CTimeSpan timeSpan ) const 
{ 
	CTime t = *this; 
	t.m_tv.tv_sec += timeSpan.m_tv.tv_sec; 
	t.m_tv.tv_usec += timeSpan.m_tv.tv_usec; 
	if (t.m_tv.tv_usec >= 1000000) 
	{ 
		t.m_tv.tv_usec -= 1000000; 
		t.m_tv.tv_sec ++; 
	} 
	return t; 
} 
 
CTime CTime::operator -( CTimeSpan timeSpan ) const 
{ 
	CTime t = *this; 
	t.m_tv.tv_sec -= timeSpan.m_tv.tv_sec; 
	t.m_tv.tv_usec -= timeSpan.m_tv.tv_usec; 
	if (t.m_tv.tv_usec >= 1000000) 
	{ 
		t.m_tv.tv_usec -= 1000000; 
		t.m_tv.tv_sec ++; 
	} 
	return t; 
} 
 
CTimeSpan CTime::operator -( CTime time ) const 
{ 
	CTimeSpan span; 
	span.m_tv.tv_sec = m_tv.tv_sec - time.m_tv.tv_sec; 
	span.m_tv.tv_usec = m_tv.tv_usec - time.m_tv.tv_usec; 
	return span; 
} 
 
bool CTime::operator ==( CTime time ) const 
{ 
	return m_tv.tv_sec == time.m_tv.tv_sec && 
		  m_tv.tv_usec == time.m_tv.tv_usec; 
} 
 
bool CTime::operator !=( CTime time ) const 
{ 
	return m_tv.tv_sec != time.m_tv.tv_sec || 
		  m_tv.tv_usec != time.m_tv.tv_usec; 
} 
 
bool CTime::operator <( CTime time ) const 
{ 
	return (m_tv.tv_sec < time.m_tv.tv_sec) || 
		(m_tv.tv_sec == time.m_tv.tv_sec && m_tv.tv_usec < time.m_tv.tv_usec); 
} 
 
bool CTime::operator >( CTime time ) const 
{ 
	return (m_tv.tv_sec > time.m_tv.tv_sec) || 
		(m_tv.tv_sec == time.m_tv.tv_sec && m_tv.tv_usec > time.m_tv.tv_usec); 
} 
 
bool CTime::operator <=( CTime time ) const 
{ 
	return (m_tv.tv_sec < time.m_tv.tv_sec) || 
		(m_tv.tv_sec == time.m_tv.tv_sec && m_tv.tv_usec <= time.m_tv.tv_usec); 
} 
 
bool CTime::operator >=( CTime time ) const 
{ 
	return (m_tv.tv_sec > time.m_tv.tv_sec) || 
		(m_tv.tv_sec == time.m_tv.tv_sec && m_tv.tv_usec >= time.m_tv.tv_usec); 
} 
 
