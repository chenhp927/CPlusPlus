//==============================================================================
//                      DataStreamStat.cpp
//
//begin   : 2017-05-05
//describe:
//==============================================================================

#include "DataStreamStat.h" 
#include "Utility.h"
#include "Logger.h"

//==============================================================================
//                 
//                    CDataStreamStat
//                    数据流统计类
//
//==============================================================================
CDataStreamStat::CDataStreamStat() : m_dwPeriod(-1)
{
	SetMyClassName("CDataStreamStat");

	Reset();
} 
 
CDataStreamStat::~CDataStreamStat() 
{ 
} 

void CDataStreamStat::Reset()
{
	m_dwStartTime = CUtility::GetUptime();
	m_dwCount = 0;
	m_dwSize = 0;
	m_dwSizeM = 0;
	m_dwMaxSize = 0;
}

void CDataStreamStat::SetPeriod(DWORD dwPeriod)
{
	m_dwPeriod = dwPeriod;
}

void CDataStreamStat::OnDataStream(DWORD dwStreamSize, string strLogTag)
{
	if (dwStreamSize <= 20)
	{
		return;
	}
	
	m_dwCount++;
	
	m_dwSize += dwStreamSize;
	if (m_dwSize > 1000000)
	{
		m_dwSizeM += m_dwSize / 1000000;
		m_dwSize = m_dwSize % 1000000;
	}
	
	if (dwStreamSize > m_dwMaxSize)
	{
		m_dwMaxSize = dwStreamSize;
	}
	
	DWORD dwSpan = CUtility::GetElapseTime(m_dwStartTime);
	if (dwSpan >= m_dwPeriod)
	{
		LogImportant("%s		period=%d, span_time=%d, count=%d, size=%d M + %d, max_size=%d", 
			strLogTag.c_str(), m_dwPeriod, dwSpan, m_dwCount, m_dwSizeM, m_dwSize, m_dwMaxSize);
		
		Reset();
	}
}

//==============================================================================
//                 
//                    CDataStreamStatMgr
//                    数据流统计管理类
//
//==============================================================================
CDataStreamStatMgr::CDataStreamStatMgr()
{
	SetMyClassName("CDataStreamStatMgr");

	m_objDssHour.SetPeriod(3600);
	m_objDssMinute.SetPeriod(60);
	m_objDssSecond.SetPeriod(5);
} 
 
CDataStreamStatMgr::~CDataStreamStatMgr() 
{ 
} 

void CDataStreamStatMgr::SetLogFileTag(const char* szFileTag)
{
	// 数据统计的日志单独记录在一个日志里
	string strFile = szFileTag;
	strFile += "_Stat";
	// 设置日志文件名称
	CLogger::SetLogFileTagInfo(strFile.c_str(), 3*1024*1024, 3);

	// 设置本类记录日志的文件
	CObjectBase::SetLogFileTag(strFile.c_str());

	// 这只本类管理类记录日志的文件
	m_objDssHour.SetLogFileTag(strFile.c_str());
	m_objDssMinute.SetLogFileTag(strFile.c_str());
	m_objDssSecond.SetLogFileTag(strFile.c_str());
}

void CDataStreamStatMgr::OnDataStream(DWORD dwStreamSize, string strLogTag)
{
	if (dwStreamSize <= 20)
	{
		return;
	}

	m_objDssHour.OnDataStream(dwStreamSize, strLogTag);
	m_objDssMinute.OnDataStream(dwStreamSize, strLogTag);
	m_objDssSecond.OnDataStream(dwStreamSize, strLogTag);
}





