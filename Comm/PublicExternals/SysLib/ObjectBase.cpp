//==============================================================================
//                      ObjectBase.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================

#include <string.h> 

#include "ObjectBase.h" 
#include "Logger.h"

//==============================================================================
//                 
//                    CObjectBase
//                    对象基类
//
//==============================================================================
CObjectBase::CObjectBase()
{
	m_strClassName = "";
	SetMyClassName("CObjectBase");

	m_strLogFileTag = "";

#ifdef NOLOG
	m_bEnableLogImportant = false;
	m_bEnableLogDebug = false;
#elif LOGDEBUG
	m_bEnableLogImportant = true;
	m_bEnableLogDebug = true;
#else
	m_bEnableLogImportant = true;
	m_bEnableLogDebug = false;
#endif
} 
 
CObjectBase::~CObjectBase() 
{ 
} 

void CObjectBase::LogImportant(const char* szFormat, ...)
{
	int nLogLevel = 0;
	if (m_bEnableLogImportant)
	{
		nLogLevel |= LL_IMPORTANT;
	}
	
	if (m_bEnableLogDebug)
	{
		nLogLevel |= LL_DEBUG;
	}
	
	va_list vlImpt;
	va_start(vlImpt, szFormat);
	CLogger::Write(m_strLogFileTag.c_str(), nLogLevel, szFormat, vlImpt);
	va_end(vlImpt);
}

void CObjectBase::LogDebug(const char* szFormat, ...)
{
	int nLogLevel = 0;
	if (m_bEnableLogDebug)
	{
		nLogLevel |= LL_DEBUG;
	}

	va_list vlDebug;
	va_start(vlDebug, szFormat);
	CLogger::Write(m_strLogFileTag.c_str(), nLogLevel, szFormat, vlDebug);
	va_end(vlDebug);
}

void CObjectBase::LogDataImportant(const char *pData, int len, const char* szDesc)
{
	int nLogLevel = 0;
	if (m_bEnableLogImportant)
	{
		nLogLevel |= LL_IMPORTANT;
	}
	
	if (m_bEnableLogDebug)
	{
		nLogLevel |= LL_DEBUG;
	}
	
	CLogger::WriteData(m_strLogFileTag.c_str(), nLogLevel, (BYTE*)pData, len, szDesc);
}

void CObjectBase::LogDataDebug(const char *pData, int len, const char* szDesc)
{
	int nLogLevel = 0;
	if (m_bEnableLogDebug)
	{
		nLogLevel |= LL_DEBUG;
	}
	
	CLogger::WriteData(m_strLogFileTag.c_str(), nLogLevel, (BYTE*)pData, len, szDesc);
}

void CObjectBase::SetLogFileTag(const char* szFileTag)
{
	m_strLogFileTag = szFileTag;
}

char* CObjectBase::GetLogFileTag() const
{
	return (char*)m_strLogFileTag.c_str();
}

string CObjectBase::GetLogFileBase()
{
	return CLogger::GetLogFileBase();
}

void CObjectBase::EnableLogImportant(bool bEnable)
{
	m_bEnableLogImportant = bEnable;
}

void CObjectBase::EnableLogDebug(bool bEnable)
{
	m_bEnableLogDebug = bEnable;
}

void CObjectBase::SetMyClassName(string strClassName)
{
	m_strClassName += "<" + strClassName + ">";
}

bool CObjectBase::IsMyClassName(string strClassName)
{
	string strFind = "<" + strClassName + ">";
	if (m_strClassName.find(strFind) != string::npos)
	{
		return true;
	}

	return false;
}






