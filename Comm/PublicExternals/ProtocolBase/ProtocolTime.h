#ifndef PROTOCOL_TIME_H
#define PROTOCOL_TIME_H
//==============================================================================
//	
//							 ProtocolTime.h
//							   
//begin	  : 2016/08/15
//describe: 协议中传输的时间
//
//==============================================================================

#ifdef UNIX
#include <sys/time.h>
#include "TypeDef.h"
#include "CTime.h"
#else
#include <afx.h>
#include <WINSOCK2.H>
#endif

#include "Buffer.h" 
#include "ObjectBase.h"
#include "Utility.h"

namespace ProtocolBase
{
	//==============================================================================
	//	
	//							  CProtocolTime
	//							 协议中传输的时间
	//
	//==============================================================================
	class MY_EXT_CLASS CProtocolTime : public CObjectBase
	{
	public:
		CProtocolTime();
		CProtocolTime& operator = (const CProtocolTime& t);
		virtual ~CProtocolTime(); 
		
	public:
		int ToStreamBuf(char* pBuf, int& nBufLen);
		int FromStreamBuf(const char* pBuf, int nBufLen);
		
		static int StreamSize();
		
		bool IsValidToCTime() const;
		bool IsValid() const;
		
		void Set(BYTE byYear, BYTE byMonth, BYTE byDay, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void Set(time_t t, WORD wMilliSec);
		void Set(DWORD dwDate, DWORD dwTime);		// wDate：年月日：20160912；wTime：时分秒毫秒：101010000
		void Set(timeval t);
		void SetMilliSec(WORD wMilliSec);
		time_t GetTime() const;					// 获取从1970年1月1日0时0分0秒到现在时刻的秒数，没有毫秒
		timeval GetTimeval() const;
		WORD GetYear() const;
		BYTE GetMonth() const;
		BYTE GetDay() const;
		BYTE GetHour() const;
		BYTE GetMinute() const;
		BYTE GetSecond() const;
		WORD GetMilliSec() const;
		DWORD GetDwDate() const;
		DWORD GetDwTime() const;

		// 获取时间字符串的接口，如果要显示或者输出到日志中可以使用此接口获取串
		string GetTimeString();
		bool operator==(const CProtocolTime &)const ;
	private:
		BYTE m_byYear;
		BYTE m_byMonth;
		BYTE m_byDay;
		BYTE m_byHour;
		BYTE m_byMinute;
		BYTE m_bySecond;
		WORD m_wMilliSec;
	};
	
	//==============================================================================               
	//                    CQueryTimeToSql
	//					  查询时间转sql语句的工具类
	//==============================================================================
	class MY_EXT_CLASS CQueryTimeToSql
	{
	public:
		CQueryTimeToSql();
		virtual ~CQueryTimeToSql();
		
	public:
		// 获取两个日期间隔的天数
		static WORD CalDiffDays(DWORD dwDateStart, DWORD dwDateEnd);
		
		// 根据查询的时间毫秒数获取响应的sql语句，这个是常规语句
		static string GetNormalSqlCmdByTime(CProtocolTime objBeg, CProtocolTime objEnd,
			string strDbDate, string strDbTime);
		
		// 根据查询的时间毫秒数获取响应的sql语句，这个专用于内部协议中“查询协议”，详见协议章节的说明
		static string GetSpecialSqlCmdByTime(CProtocolTime objBeg, CProtocolTime objEnd,
			string strDbDate, string strDbTime);
		
	private:
		// 根据毫秒数是否为0判断统计方式
		static bool IsPeriodInEveryDay(CProtocolTime& objBeg, CProtocolTime& objEnd);
	};
}
#endif