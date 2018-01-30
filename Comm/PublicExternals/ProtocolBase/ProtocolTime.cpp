//==============================================================================
//	
//							 ProtocolTime.cpp
//							   
//begin	  : 2016/08/15
//describe: 协议中传输的时间
//
//==============================================================================
#include "ProtocolBase.h"
#include "ProtocolTime.h"

namespace ProtocolBase
{
	CProtocolTime::CProtocolTime()
	{
		timeval t;
#ifdef  UNIX
		gettimeofday(&t, NULL);
		Set(t);
#else
		struct tm tm;
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		tm.tm_year = wtm.wYear - 1900;
		tm.tm_mon = wtm.wMonth - 1;
		tm.tm_mday = wtm.wDay;
		tm.tm_hour = wtm.wHour;
		tm.tm_min = wtm.wMinute;
		tm.tm_sec = wtm.wSecond;
		tm. tm_isdst = -1;

		time_t tTime;
		tTime = mktime(&tm);
		t.tv_sec = (long)tTime;
		t.tv_usec = (long)(wtm.wMilliseconds * 1000);
		Set(t);
#endif
	}

	CProtocolTime::~CProtocolTime()
	{
	}

	bool CProtocolTime::IsValid() const
	{
		CTime cur = CTime::GetCurrentTime();
		return (m_byYear >= cur.GetYear() - 2000 - 1 && m_byYear <= cur.GetYear() - 2000 + 1) &&
			(m_byMonth > 0 && m_byMonth <= 12) &&
			(m_byDay > 0 && m_byDay <= 31) &&
			(m_byHour >= 0 && m_byHour < 24) &&
			(m_byMinute >= 0 && m_byMinute < 60) &&
			(m_bySecond >= 0 && m_bySecond < 60) &&
			(m_wMilliSec >=0 && m_wMilliSec < 10000);
	}

	bool CProtocolTime::IsValidToCTime() const
	{
		if(m_byYear + 2000 == 1970 && m_byMonth == 1 && m_byDay == 1 && m_byHour < 8)
		{
			return false;
		}

		if(m_byYear + 2000 == 2038 && m_byMonth == 1 && m_byDay == 1 && m_byHour >0)
		{
			return false;
		}

		return (m_byYear + 2000 >= 1970 && m_byYear + 2000< 2038) &&
			(m_byMonth > 0 && m_byMonth <= 12) &&
			(m_byDay > 0 && m_byDay <= 31);
	}

	void CProtocolTime::Set(BYTE byYear, BYTE byMonth, BYTE byDay, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec)
	{
		m_byYear = byYear;
		m_byMonth = byMonth;
		m_byDay = byDay;
		m_byHour = byHour;
		m_byMinute = byMinute;
		m_bySecond = bySecond;
		m_wMilliSec = wMilliSec;
	}

	void CProtocolTime::Set(time_t t, WORD wMilliSec)
	{
		CTime tm(t);

		m_byYear = static_cast<BYTE>(tm.GetYear() - 2000);
		m_byMonth = static_cast<BYTE>(tm.GetMonth());
		m_byDay = static_cast<BYTE>(tm.GetDay());
		m_byHour = static_cast<BYTE>(tm.GetHour());
		m_byMinute = static_cast<BYTE>(tm.GetMinute());
		m_bySecond = static_cast<BYTE>(tm.GetSecond());
		m_wMilliSec = wMilliSec;
	}

	void CProtocolTime::Set(timeval t)
	{
		CTime tm(t.tv_sec);

		m_byYear = static_cast<BYTE>(tm.GetYear() - 2000);
		m_byMonth = static_cast<BYTE>(tm.GetMonth());
		m_byDay = static_cast<BYTE>(tm.GetDay());
		m_byHour = static_cast<BYTE>(tm.GetHour());
		m_byMinute = static_cast<BYTE>(tm.GetMinute());
		m_bySecond = static_cast<BYTE>(tm.GetSecond());

		m_wMilliSec = (WORD)(t.tv_usec/1000);
	}
	
	void CProtocolTime::Set(DWORD dwDate, DWORD dwTime)
	{
		m_byYear = static_cast<BYTE>(dwDate/10000 - 2000);
		m_byMonth = static_cast<BYTE>((dwDate%10000)/100);
		m_byDay = static_cast<BYTE>(dwDate%100);
	
		m_byHour = static_cast<BYTE>(dwTime/10000000);
		m_byMinute = static_cast<BYTE>((dwTime%10000000)/100000);
		m_bySecond = static_cast<BYTE>((dwTime%100000)/1000);
		m_wMilliSec = static_cast<WORD>(dwTime%1000);
	}

	void CProtocolTime::SetMilliSec( WORD wMilliSec )
	{
		m_wMilliSec = wMilliSec;
	}

	WORD CProtocolTime::GetYear() const
	{
		return (2000 + m_byYear);
	}
	
	BYTE CProtocolTime::GetMonth() const
	{
		return m_byMonth;
	}
	
	BYTE CProtocolTime::GetDay() const
	{
		return m_byDay;
	}
	
	BYTE CProtocolTime::GetHour() const
	{
		return m_byHour;
	}
	
	BYTE CProtocolTime::GetMinute() const
	{
		return m_byMinute;
	}
	
	BYTE CProtocolTime::GetSecond() const
	{
		return m_bySecond;
	}

	WORD CProtocolTime::GetMilliSec() const
	{
		return m_wMilliSec;
	}

	DWORD CProtocolTime::GetDwDate() const
	{
		char cDate[255];
		snprintf(cDate, 255, "%4d%02d%02d", GetYear(), GetMonth(), GetDay());
		DWORD dwDate = atoi(cDate);
		return dwDate;
	}

	DWORD CProtocolTime::GetDwTime() const
	{
		char cTime[255];
		snprintf(cTime, 255, "%02d%02d%02d%03d", GetHour(), GetMinute(), GetSecond(), GetMilliSec());
		DWORD dwTime = atoi(cTime);
		return dwTime;
	}

	time_t CProtocolTime::GetTime() const
	{
#ifdef UNIX
		return CTime::MakeTime(m_byYear + 2000, m_byMonth, m_byDay, m_byHour, m_byMinute, m_bySecond);
#else
		if (IsValidToCTime())
		{
			CTime tm(m_byYear + 2000, m_byMonth, m_byDay, m_byHour, m_byMinute, m_bySecond);
			return tm.GetTime();
		}
		else
		{
			return 0;
		}

#endif
	}

	timeval CProtocolTime::GetTimeval() const
	{
		timeval tm;
		tm.tv_sec = (long)GetTime();
		tm.tv_usec = m_wMilliSec * 1000;
		return tm;
	}

	bool CProtocolTime::operator==(const CProtocolTime &r)const
	{
		return  r.m_byYear == m_byYear && r.m_byMonth == m_byMonth
				&& r.m_byDay == m_byDay && r.m_byHour == m_byHour
				&& r.m_byMinute == m_byMinute && r.m_bySecond == m_bySecond
				&& r.m_wMilliSec == m_wMilliSec;
	}

	int CProtocolTime::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if(nBufLen < StreamSize())
		{
			LogImportant("Buflen is less than the Time len\n" );

			nBufLen = StreamSize();
			return -1;
		}
		
		pBuf[0] = m_byYear;
		pBuf[1] = m_byMonth;
		pBuf[2] = m_byDay;
		pBuf[3] = m_byHour;
		pBuf[4] = m_byMinute;
		pBuf[5] = m_bySecond;
		SETSTREAMSHORT(&pBuf[6], m_wMilliSec);
		return StreamSize();
	}

	int CProtocolTime::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if(nBufLen < StreamSize())
		{
			LogImportant("Buflen is less than the Time len\n" );
			return -1;
		}

		m_byYear = pBuf[0];
		m_byMonth = pBuf[1];
		m_byDay = pBuf[2];
		m_byHour = pBuf[3];
		m_byMinute = pBuf[4];
		m_bySecond = pBuf[5];
		m_wMilliSec = (WORD)GETSTREAMSHORT(&pBuf[6]);
		return StreamSize();
	}

	CProtocolTime& CProtocolTime::operator = (const CProtocolTime& t)
	{
		*((CObjectBase*)this) = *((CObjectBase*)&t);

		m_byYear = t.m_byYear;
		m_byMonth = t.m_byMonth;
		m_byDay	= t.m_byDay;
		m_byHour = t.m_byHour;
		m_byMinute = t.m_byMinute;
		m_bySecond = t.m_bySecond;
		m_wMilliSec = t.m_wMilliSec;
		return *this;
	}
	
	int CProtocolTime::StreamSize()
	{
		return PROTOCOL_TIME_LEN;
	}

	string CProtocolTime::GetTimeString()
	{		
		char cTime[255];
		snprintf(cTime, 255, "%02d-%02d-%02d %02d:%02d:%02d.%03d", 
			m_byYear, m_byMonth, m_byDay, m_byHour, m_byMinute, m_bySecond, m_wMilliSec);

		string strRet = cTime;
		return strRet;
	}

	
	//==============================================================================               
	//                    CQueryTimeToSql
	//					  查询时间转sql语句的工具类
	//==============================================================================
	CQueryTimeToSql::CQueryTimeToSql()
	{
	}
	
	CQueryTimeToSql::~CQueryTimeToSql()
	{
	}
	
	WORD CQueryTimeToSql::CalDiffDays(DWORD dwDateStart, DWORD dwDateEnd)
	{
#ifdef UNIX
		time_t tStart = CTime::MakeTime(dwDateStart/10000, dwDateStart%10000/100, dwDateStart%100, 0, 0, 0);
		time_t tEnd = CTime::MakeTime(dwDateEnd/10000, dwDateEnd%10000/100, dwDateEnd%100, 0, 0, 0);
#else
		CTime tmStart(dwDateStart/10000, dwDateStart%10000/100, dwDateStart%100, 0, 0, 0);
		time_t tStart = tmStart.GetTime();

		CTime tmEnd(dwDateEnd/10000, dwDateEnd%10000/100, dwDateEnd%100, 0, 0, 0);
		time_t tEnd = tmEnd.GetTime();
#endif
		
		// 计算间隔天数
		CTime ctStart(tStart), ctEnd(tEnd);
		CTimeSpan ts = ctEnd - ctStart;
		
		// 返回间隔天数
		WORD wDiffDays = (WORD)ts.GetDays();
		
		return wDiffDays;
	}

	string CQueryTimeToSql::GetNormalSqlCmdByTime(CProtocolTime objBeg, CProtocolTime objEnd,
			string strDbDate, string strDbTime)
	{
		string strCmd = "";

		// 起止日期和时间
		DWORD dwBeginDate = objBeg.GetDwDate();
		DWORD dwBeginTime = objBeg.GetDwTime();
		DWORD dwEndDate = objEnd.GetDwDate();
		DWORD dwEndTime = objEnd.GetDwTime();
		
		// 计算起止时间的间隔天数
		WORD wDiffDays = CalDiffDays(dwBeginDate, dwEndDate);
		strCmd = "( ";
		if (0 == wDiffDays) //不跨天
		{
			strCmd += strDbDate;
			strCmd += "=";
			strCmd += CUtility::ITOA(dwBeginDate);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += ">=";
			strCmd += CUtility::ITOA(dwBeginTime);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += "<=";
			strCmd += CUtility::ITOA(dwEndTime);
		}
		else if (1 == wDiffDays) //跨1天
		{
			strCmd += "(";
			strCmd += strDbDate;
			strCmd += "=";
			strCmd += CUtility::ITOA(dwBeginDate);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += ">=";
			strCmd += CUtility::ITOA(dwBeginTime);
			strCmd += ") or ";
			strCmd += "(";
			strCmd += strDbDate;
			strCmd += "=";
			strCmd += CUtility::ITOA(dwEndDate);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += "<=";
			strCmd += CUtility::ITOA(dwEndTime);
			strCmd += ")";
		}
		else if (2 <= wDiffDays) //跨多天
		{
			strCmd += "(";
			strCmd += strDbDate;
			strCmd += "=";
			strCmd += CUtility::ITOA(dwBeginDate);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += ">=";
			strCmd += CUtility::ITOA(dwBeginTime);
			strCmd += ") or ";
			strCmd += "(";
			strCmd += strDbDate;
			strCmd += ">";
			strCmd += CUtility::ITOA(dwBeginDate);
			strCmd += " and ";
			strCmd += strDbDate;
			strCmd += "<";
			strCmd += CUtility::ITOA(dwEndDate);
			strCmd += ") or (";
			strCmd += strDbDate;
			strCmd += "=";
			strCmd += CUtility::ITOA(dwEndDate);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += "<=";
			strCmd += CUtility::ITOA(dwEndTime);
			strCmd += ")";
		}
		strCmd += " ) ";

		return strCmd;
	}
	
	string CQueryTimeToSql::GetSpecialSqlCmdByTime(CProtocolTime objBeg, CProtocolTime objEnd,
			string strDbDate, string strDbTime)
	{
		string strCmd = "";
		
		// 如果是统计每天的几点~几点
		if (IsPeriodInEveryDay(objBeg, objEnd))
		{
			
			// 起止日期和时间
			DWORD dwBeginDate = objBeg.GetDwDate();
			DWORD dwBeginTime = objBeg.GetDwTime();
			DWORD dwEndDate = objEnd.GetDwDate();
			DWORD dwEndTime = objEnd.GetDwTime();
			
			strCmd = "( ";
			strCmd += strDbDate;
			strCmd += ">=";
			strCmd += CUtility::ITOA(dwBeginDate);
			strCmd += " and ";
			strCmd += strDbDate;
			strCmd += "<=";
			strCmd += CUtility::ITOA(dwEndDate);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += ">=";
			strCmd += CUtility::ITOA(dwBeginTime);
			strCmd += " and ";
			strCmd += strDbTime;
			strCmd += "<=";
			strCmd += CUtility::ITOA(dwEndTime);
			strCmd += " ) ";
			
			return strCmd;
		}
		
		return GetNormalSqlCmdByTime(objBeg, objEnd, strDbDate, strDbTime);
	}

	bool CQueryTimeToSql::IsPeriodInEveryDay(CProtocolTime& objBeg, CProtocolTime& objEnd)
	{
		if (objBeg.GetMilliSec()==0 && objEnd.GetMilliSec()==0)
		{
			return false;
		}

		return true;
	}
}
