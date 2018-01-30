//==============================================================================
//                      CTime.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_CTIME_H 
#define SELFDEF_CTIME_H

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#include <stdlib.h>

//==============================================================================
//                 
//                    CTimeSpan
//                    
//
//==============================================================================
class CTime; 
class CTimeSpan 
{ 
public: 
	CTimeSpan(); 
	CTimeSpan( time_t time ); 
	CTimeSpan( int lDays, int nHours, int nMins, int nSecs ); 
private: 
	struct timeval m_tv; 
	friend class CTime; 
public: 
	int GetDays() const; 
	int GetHours() const; 
	int GetMinutes() const; 
	int GetSeconds() const; 
	int GetMilliseconds() const; 
 
	CTimeSpan operator +( CTimeSpan timeSpan ) const; 
	CTimeSpan operator -( CTimeSpan timeSpan ) const; 
}; 
 
 
//==============================================================================
//                 
//                    CTime
//                    
//
//==============================================================================
class CTime 
{ 
public: 
	CTime(); 
	CTime(time_t t); 
	~CTime(); 
 
private: 
	struct timeval m_tv; 
 
	struct tm* GetLocalTm(struct tm* ptm) const; 
 
public: 
	static CTime GetCurrentTime(); 
	time_t GetTime() const; 
	timeval GetTimeStu() const;
	int	GetYear() const; 
	int GetMonth() const; 
	int GetDay() const; 
	int GetHour() const; 
	int GetMinute() const; 
	int GetSecond() const; 
	int GetMillisecond() const; 
	char *Format(char *buf) const;
	static time_t MakeTime(int y, int m, int d, int h, int mi, int s);

	CTime& operator =( const CTime& timeSrc); 
	CTime operator +( CTimeSpan timeSpan ) const; 
	CTime operator -( CTimeSpan timeSpan ) const; 
	CTimeSpan operator -( CTime time ) const; 
	bool operator ==( CTime time ) const; 
	bool operator !=( CTime time ) const; 
	bool operator <( CTime time ) const; 
	bool operator >( CTime time ) const; 
	bool operator <=( CTime time ) const; 
	bool operator >=( CTime time ) const; 
}; 
 
#endif 
 
