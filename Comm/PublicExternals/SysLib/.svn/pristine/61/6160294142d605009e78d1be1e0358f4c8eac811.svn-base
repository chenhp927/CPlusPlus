//==============================================================================
//                      Logger.cpp
//
//begin   : 2016-08-23
//describe:
//==============================================================================

#ifdef UNIX
	#include <memory.h>
	#include <stdarg.h>
	#include <dirent.h>
	#include <libgen.h>
	#include "CTime.h"
#else
	#include <direct.h>
	#include <afx.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string>

#include "Utility.h"
#include "Logger.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD CLogger::m_dwSeqNum = 0;

//==============================================================================
//
//                   CLogFile
//                日志文件记录类
//
//==============================================================================
CLogger::CLogFile::CLogFile()
{
	m_strFileName = "";
	m_dwFileMaxSize = DEF_LOGFILE_MAXSIZE;
	m_dwFileCurSize = 0;
	m_byFileBkCount = DEF_LOGFILE_BKCOUNT;
	m_wDataMaxCount = DEF_LOGDATA_MAXCOUNT;
	
	m_pFile = NULL;
}

CLogger::CLogFile::CLogFile(const char* szFileName)
{
	m_strFileName = szFileName;
	m_dwFileMaxSize = DEF_LOGFILE_MAXSIZE;
	m_dwFileCurSize = 0;
	m_byFileBkCount = DEF_LOGFILE_BKCOUNT;
	m_wDataMaxCount = DEF_LOGDATA_MAXCOUNT;
	
	m_pFile = NULL;
}

CLogger::CLogFile::~CLogFile()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	m_objMutex.Unlock();
}

void CLogger::CLogFile::SetFileMaxSize(DWORD dwFileMaxSize)
{
	CAutoLock objAutoLock(&m_objMutex);
	
	m_dwFileMaxSize = dwFileMaxSize;
}  

void CLogger::CLogFile::SetFileBkCount(BYTE byFileBkCount)
{
	CAutoLock objAutoLock(&m_objMutex);
	
	m_byFileBkCount = byFileBkCount;
} 

void CLogger::CLogFile::SetDataMaxCount(WORD wDataMaxCount)
{
	CAutoLock objAutoLock(&m_objMutex);
	
	m_wDataMaxCount = wDataMaxCount;
}   

string CLogger::CLogFile::GetFileName()
{
	CAutoLock objAutoLock(&m_objMutex);
	
	return m_strFileName;
}

void CLogger::CLogFile::Write(const char* szContent)
{
	CAutoLock objAutoLock(&m_objMutex);
	
	if (!MakeWriteString(szContent))
	{
		return;
	}
	
	WriteToFile();
}

void CLogger::CLogFile::Write(const char* szFormat, va_list vl)
{
	CAutoLock objAutoLock(&m_objMutex);

	if (!MakeWriteString(szFormat, vl))
	{
		return;
	}
	
	WriteToFile();
}

void CLogger::CLogFile::WriteData(BYTE *pData, DWORD dwDataLen, const char* szDesc)
{
	CAutoLock objAutoLock(&m_objMutex);
	
	assert(pData);
	
	if (!MakeWriteString(pData, dwDataLen, szDesc))
	{
		return;
	}
	
	WriteToFile();
}

void CLogger::CLogFile::WriteToFile()
{
	FILE* pFile = OpenLogFile();
	if (!pFile)
	{
		char szErrLog[DEF_LOGREC_MAXSIZE];
		snprintf(szErrLog, DEF_LOGREC_MAXSIZE, "CLogger fail to 'OpenLogFile' (file_name=%s). \n write log: \n %s\n", 
			m_strFileName.c_str(), m_szLogBuf);
		CLogger::Instance()->SaveErrLog(szErrLog);
			
		return;
	}
	
	if(fprintf(pFile, m_szLogBuf) < 0)
	{
		//写文件出问题了,关闭文件句柄,下一次会重新打开,希望能改善情况
		char szErrLog[DEF_LOGREC_MAXSIZE];
		snprintf(szErrLog, DEF_LOGREC_MAXSIZE, "CLogger fail to 'fprintf' (file_name=%s). \n write log: \n %s\n", 
			m_strFileName.c_str(), m_szLogBuf);
		CLogger::Instance()->SaveErrLog(szErrLog);
		
		fclose(pFile);
		pFile = NULL;
		
		m_pFile = NULL;
	}
	
	if(pFile && fflush(pFile) != 0)
	{
		char szErrLog[DEF_LOGREC_MAXSIZE];
		snprintf(szErrLog, DEF_LOGREC_MAXSIZE, "CLogger fail to 'fflush' (file_name=%s). \n write log: \n %s\n", 
			m_strFileName.c_str(), m_szLogBuf);
		CLogger::Instance()->SaveErrLog(szErrLog);
		
		fclose(pFile);
		pFile = NULL;
		
		m_pFile = NULL;
	}
}

void CLogger::CLogFile::MakeSeqNumString(char* szBuf)
{
	assert(szBuf);

	sprintf(szBuf, "\n\n%010d ", m_dwSeqNum); 
}

void CLogger::CLogFile::MakeTimeString(char* szBuf)
{
	assert(szBuf);
	
	CTime objCurTime = CTime::GetCurrentTime(); 
	sprintf(szBuf, "\n%02d-%02d-%02d %02d:%02d:%02d", 
		objCurTime.GetYear(), objCurTime.GetMonth(), objCurTime.GetDay(),
		objCurTime.GetHour(), objCurTime.GetMinute(), objCurTime.GetSecond()); 

	// 设置毫秒
#ifdef UNIX
	int nMs = objCurTime.GetMillisecond();
#else
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	int nMs = wtm.wMilliseconds;
#endif
	char* p = szBuf + strlen(szBuf);
	sprintf(p, ".%03d", nMs); 
}

bool CLogger::CLogFile::MakeWriteString(const char* szContent)
{
	memset(m_szLogBuf, 0, sizeof(m_szLogBuf));
	
	// 构造记录序号字符串
	char* p = m_szLogBuf;
	MakeSeqNumString(p); 
	
	// 构造当前时间字符串
	p = m_szLogBuf + strlen(m_szLogBuf);
	MakeTimeString(p); 

	// 构造日志内容字符串
	p = m_szLogBuf + strlen(m_szLogBuf);
	int nSize = sizeof(m_szLogBuf) - strlen(m_szLogBuf);
	int nRet = snprintf(p, nSize, "\n%s", szContent);
	if (nRet < 0)
	{
		char szErrLog[DEF_LOGREC_MAXSIZE];
		snprintf(szErrLog, sizeof(szErrLog), "CLogger fail to 'vsnprintf' to file (file_name=%s). \n write log: \n %s\n", 
			m_strFileName.c_str(), m_szLogBuf);
		CLogger::Instance()->SaveErrLog(szErrLog);
		
		return false;
	}
	
	return true;
}

bool CLogger::CLogFile::MakeWriteString(const char* szFormat, va_list vl)
{
	memset(m_szLogBuf, 0, sizeof(m_szLogBuf));
	
	// 构造记录序号字符串
	char* p = m_szLogBuf;
	MakeSeqNumString(p); 
	
	// 构造当前时间字符串
	p = m_szLogBuf + strlen(m_szLogBuf);
	MakeTimeString(p); 

	// 构造日志内容字符串
	p = m_szLogBuf + strlen(m_szLogBuf);
	int nSize = sizeof(m_szLogBuf) - strlen(m_szLogBuf);
	int nRet = vsnprintf(p, nSize, szFormat, vl);
	if (nRet < 0)
	{
		char szErrLog[DEF_LOGREC_MAXSIZE];
		snprintf(szErrLog, sizeof(szErrLog), "CLogger fail to 'vsnprintf' to file (file_name=%s). \n write log: \n %s\n", 
			m_strFileName.c_str(), m_szLogBuf);
		CLogger::Instance()->SaveErrLog(szErrLog);
		
		return false;
	}

	return true;
}

bool CLogger::CLogFile::MakeWriteString(BYTE *pData, DWORD dwDataLen, const char* szDesc)
{
	memset(m_szLogBuf, 0, DEF_LOGREC_MAXSIZE);

	// 构造记录序号字符串
	char* p = m_szLogBuf;
	MakeSeqNumString(p); 
	
	// 构造当前时间字符串
	p = m_szLogBuf + strlen(m_szLogBuf);
	MakeTimeString(p); 
	
	// 构造描述内容字符串
	p = m_szLogBuf + strlen(m_szLogBuf);
	int nSize = sizeof(m_szLogBuf) - strlen(m_szLogBuf);
	if (szDesc)
	{
		snprintf(p, nSize, "\n%s", szDesc);
	}
	
	// 构造Buffer字符串
	int nPos = strlen(m_szLogBuf);
	p = m_szLogBuf + nPos;
	
	DWORD dwLen = dwDataLen;
	if (dwLen > m_wDataMaxCount)
	{
		dwLen = m_wDataMaxCount;
	}
	
	for (DWORD x = 0; x < dwLen; x++)
	{
		if (nPos + 10 > DEF_LOGREC_MAXSIZE)
		{
			break;
		}
		
		if (x % 20 == 0)
		{
			sprintf(p, "\n\t");
			nPos += 2;
			p = m_szLogBuf + nPos;
		}
		
		sprintf(p, "%02X ", pData[x]);
		nPos += 3;
		p = m_szLogBuf + nPos;
	}
	
	return true;
}

FILE* CLogger::CLogFile::OpenLogFile()
{
	// 获取文件状态	
	struct stat st;
	int ret = stat(m_strFileName.c_str(), &st);
	if(ret == 0) 
	{
		int nLastSize = m_dwFileCurSize;
		m_dwFileCurSize = st.st_size;
		
		//文件存在，超过指定大小，备份
		if (st.st_size >= m_dwFileMaxSize)
		{
			BackupLogFile();
		}
		//文件存在，但上次日志后文件大小未变化，说明上次日志没有成功写入
		//磁盘文件，为了防止文件句柄无效，需重新打开句柄
		else if (nLastSize == m_dwFileCurSize) 
		{
			// 若对象存在，则先关闭
			if(m_pFile)
			{
				fclose(m_pFile);
				m_pFile = NULL;
			}
		}
	}
	else	// 读取文件状态异常，文件不存在
	{
		// 创建日志路径
		CUtility::MakeDir(m_strFileName.c_str());
		
		// 若对象存在，则先关闭
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	if (!m_pFile)
	{
		m_pFile = fopen(m_strFileName.c_str(), "a+t"); 
	}
	
	return m_pFile; 
}

void CLogger::CLogFile::BackupLogFile()
{
	// 先关闭当前文件
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	
	// 截断文件名中的".log"
	char szFileName[DEF_LOGFILE_NAMELEN];
	strcpy(szFileName, m_strFileName.c_str());
	
	char *pDot = NULL;
	pDot = strrchr(szFileName, '.');
	if (pDot)
	{
		pDot[0] = '\0';   
	}

	char szExt[DEF_LOGFILE_NAMELEN];

	// 删除最后旧的一个备份文件
	string strBakLast = szFileName;
	if (m_byFileBkCount > 0)
	{
		sprintf(szExt, "%d.log", m_byFileBkCount);
	}
	else
	{
		sprintf(szExt, ".log");
	}
	strBakLast += szExt;
	remove(strBakLast.c_str());

	// 修改各个备份文件的名称
	string strBak1;
	string strBak2;
	for (int x = m_byFileBkCount - 1; x > 0; x--)
	{
		strBak1 =  szFileName;
		sprintf(szExt, "%d.log", x);
		strBak1 += szExt;

		strBak2 = szFileName;
		sprintf(szExt, "%d.log", x+1);
		strBak2 += szExt;

		rename(strBak1.c_str(), strBak2.c_str());
	}

	// 将当前文件设置为第一个备份
	strBak1 = szFileName;
	strBak1 += "1.log";
	rename(m_strFileName.c_str(), strBak1.c_str());
}

//==============================================================================
//
//                   CLogger
//                 日志记录器类
//
//==============================================================================
CLogger::CLogger()
{
	m_strLogFileBase = "";
	m_strLogErrFile = "";

	m_nLogLevelValid = -1; // 0xffffffff，默认允许所有的日志输出
}

CLogger::~CLogger()
{
	for (int x = 0; x < m_Files.size(); x++)
	{
		CLogFile* pFile = m_Files[x];
		assert(pFile);
		delete pFile;
	}

	m_Files.clear();
}

CLogger* CLogger::Instance()
{
	static CLogger logger;
	return &logger;
}

void CLogger::SetLogFileBase(const char* szLogFileBase)
{
	// 接口加锁
//	CAutoLock objAutoLock(&CLogger::Instance()->m_objMutex);
	CLogger::Instance()->m_objMutex.Lock();

	assert(szLogFileBase);
	// 设置日志基础串
	CLogger::Instance()->m_strLogFileBase = szLogFileBase;

	// 设置错误文件，日志写文件错误时都会记入本文件
	CLogger::Instance()->m_strLogErrFile = szLogFileBase;
	CLogger::Instance()->m_strLogErrFile.append(DEF_LOGERR_FILETAG);
	
	// 创建日志路径
	CUtility::MakeDir(szLogFileBase);

	// 更新允许日志输出的日志级别
	UpdateLogLevelValid();

	// 解锁
	CLogger::Instance()->m_objMutex.Unlock();

	// 设置默认日志文件
	SetLogFileTagInfo("");
}

std::string CLogger::GetLogFileBase()
{
	return CLogger::Instance()->m_strLogFileBase;
}

void CLogger::SetLogFileTagInfo(const char* szLogFileTag, DWORD dwFileMaxSize, BYTE byFileBkCount, WORD wDataMaxCount)
{
	// 接口加锁
	CAutoLock objAutoLock(&CLogger::Instance()->m_objMutex);

	assert(szLogFileTag);

	// 如果szLogFileTag中带有目录，创建目录
#ifdef UNIX
	const char chDirSep = '/';
#else
	const char chDirSep = '\\';
#endif
	string strLogFileTag = szLogFileTag;
	if (strLogFileTag.find(chDirSep) != -1)
	{
		char szFileName[DEF_LOGFILE_NAMELEN];
		memset(szFileName, 0, sizeof(szFileName));

		int nLen = strLogFileTag.find_last_of(chDirSep) - 1;
		memcpy(szFileName, szLogFileTag, nLen);

		CUtility::MakeDir(szFileName);
	}

	// 设置非Debug日志文件
	string strFileName = CLogger::Instance()->GetLogFileName(szLogFileTag, "");
	CLogger::Instance()->SetLogFile(strFileName.c_str(), dwFileMaxSize, byFileBkCount, wDataMaxCount);

	// 设置Debug日志文件
	strFileName = CLogger::Instance()->GetLogFileName(szLogFileTag, "Debug");
	CLogger::Instance()->SetLogFile(strFileName.c_str(), dwFileMaxSize, byFileBkCount, wDataMaxCount);
}
		   
void CLogger::SetLogFile(const char* szFileName, DWORD dwFileMaxSize, BYTE byFileBkCount, WORD wDataMaxCount)
{
	CLogFile* pFile = GetLogFile(szFileName);
	if (!pFile)
	{
		pFile = new CLogFile(szFileName);
		m_Files.push_back(pFile);
	}
	
	if (pFile)
	{
		pFile->SetFileMaxSize(dwFileMaxSize);
		pFile->SetFileBkCount(byFileBkCount);
		pFile->SetDataMaxCount(wDataMaxCount);
	}
}

void CLogger::Write(const char* szLogFileTag, int nLogLevel, const char* szFormat, ...)
{
	va_list vl;
	va_start(vl, szFormat);
	
	Write(szLogFileTag, nLogLevel, szFormat, vl);
	
	va_end(vl);
}

void CLogger::Write(const char* szLogFileTag, int nLogLevel, const char* szFormat, va_list vl)
{
	// 接口加锁
	CAutoLock objAutoLock(&CLogger::Instance()->m_objMutex);

	// 找出要记录的各个日志文件
	vector<CLogFile*> vecFiles = CLogger::Instance()->GetLogFiles(szLogFileTag, nLogLevel);
	if (vecFiles.size() <= 0)
	{
		return;
	}

	// 将变参格式化成字符串
	char szVa[DEF_LOGREC_MAXSIZE];;
	memset(szVa, 0, sizeof(szVa));
	int nRet = vsnprintf(szVa, sizeof(szVa), szFormat, vl);
	if (nRet < 0)
	{
		char szErrLog[DEF_LOGREC_MAXSIZE];
		snprintf(szErrLog, sizeof(szErrLog), "CLogger fail to 'vsnprintf' to file (file_tag=%s). \n write log: \n %s\n", 
			szLogFileTag, szVa);
		CLogger::Instance()->SaveErrLog(szErrLog);
	}

	// 滚动日志记录序号
	m_dwSeqNum++;

	// 将格式化好的字符串写入各个日志文件
	for (int x = 0; x < vecFiles.size(); x++)
	{
		CLogFile* pFile = vecFiles[x];
		assert(pFile);
		pFile->Write(szVa);	// 注：这里不能将变参列表va_list作为参数传入，使用一次时是没有问题的，但是使用多次就有问题了
	}
}

void CLogger::WriteData(const char* szLogFileTag, int nLogLevel, BYTE* pData, DWORD dwDataLen, const char* szDesc)
{
	// 接口加锁
	CAutoLock objAutoLock(&CLogger::Instance()->m_objMutex);

	// 找出要记录的各个日志文件
	vector<CLogFile*> vecFiles = CLogger::Instance()->GetLogFiles(szLogFileTag, nLogLevel);
	if (vecFiles.size() <= 0)
	{
		return;
	}
	
	// 滚动日志记录序号
	m_dwSeqNum++;
	
	// 将数据写入各个日志文件
	for (int x = 0; x < vecFiles.size(); x++)
	{
		CLogFile* pFile = vecFiles[x];
		assert(pFile);
		
		pFile->WriteData(pData, dwDataLen, szDesc);
	}
}

vector<CLogger::CLogFile*> CLogger::GetLogFiles(const char* szLogFileTag, int nLogLevel)
{
	assert(szLogFileTag);
	assert(strlen(szLogFileTag) >= 0);

	vector<CLogFile*> files;
	CLogFile* pFile = NULL;

	if (LL_IMPORTANT & m_nLogLevelValid & nLogLevel)
	{
		GetImportantFile(szLogFileTag, files);
	}

	if (LL_DEBUG & m_nLogLevelValid & nLogLevel)
	{
		GetDebugFile(szLogFileTag, files);
	}

	return files;
}

CLogger::CLogFile* CLogger::GetLogFile(const char* szFileName)
{
	assert(szFileName);
	
	for (int x = 0; x < m_Files.size(); x++)
	{
		CLogFile* pFile = m_Files[x];
		assert(pFile);
		if (strcmp(pFile->GetFileName().c_str(), szFileName) == 0)
		{
			return pFile;
		}
	}

	return NULL;
}

void CLogger::GetImportantFile(const char* szLogFileTag, vector<CLogFile*>& files)
{
	assert(szLogFileTag);
	
	string strFileName = GetLogFileName(szLogFileTag, "");
	
	CLogFile* pFile = GetLogFile(strFileName.c_str());
	if (pFile)
	{
		files.push_back(pFile);
	}
}

void CLogger::GetDebugFile(const char* szLogFileTag, vector<CLogFile*>& files)
{
	assert(szLogFileTag);

	string strFileName = GetLogFileName(szLogFileTag, "Debug");

	CLogFile* pFile = GetLogFile(strFileName.c_str());
	if (pFile)
	{
		files.push_back(pFile);
	}
}

string CLogger::GetLogFileName(const char* szLogFileTag, const char* szLogLevelTag)
{
	char szFileName[DEF_LOGFILE_NAMELEN];
	memset(szFileName, 0, sizeof(szFileName));

	// 设置szLogFileTag
#ifdef UNIX
	char chDirSep = '/';
#else
	char chDirSep = '\\';
#endif
	int nLen = 0;

	if (strlen(szLogFileTag) > 0)
	{
		string strLogFileTag = szLogFileTag;
		if (strLogFileTag.find(chDirSep) != -1)
		{
			// szLogFileTag 包含路径，直接打印szLogFileTag
			strcat(szFileName, szLogFileTag);
		}
		else
		{
			// szLogFileTag 不包含路径，设置为默认目录m_strLogFileBase
			strcat(szFileName, m_strLogFileBase.c_str());
			nLen = strlen(szFileName);
			if (nLen > 0 && szFileName[nLen - 1] != chDirSep)
			{
				strcat(szFileName, "_");
			}
			strcat(szFileName, szLogFileTag);
		}	
	}
	else
	{
		// szLogFileTag = 0，设置为默认目录m_strLogFileBase
		strcat(szFileName, m_strLogFileBase.c_str());
		nLen = strlen(szFileName);
		if (nLen > 0 && szFileName[nLen - 1] == chDirSep)
		{
			strcat(szFileName, "Default");
		}
	}

	// 设置szLogLevelTag
	if (strlen(szLogLevelTag) > 0)
	{
		strcat(szFileName, "_");
		strcat(szFileName, szLogLevelTag);
	}

	// 设置文件扩展名
	strcat(szFileName, ".log");	

	return szFileName;
}

void CLogger::SaveErrLog(const char* szLog)
{
	FILE* pFile = NULL;

	// 获取文件状态	
	struct stat st;
	int ret = stat(m_strLogErrFile.c_str(), &st);
	if(ret == 0) 
	{
		if (st.st_size >= DEF_LOGFILE_MAXSIZE)
		{
			pFile = fopen(m_strLogErrFile.c_str(), "w");
		}
		else
		{
			pFile = fopen(m_strLogErrFile.c_str(), "a");
		}
	}
	else
	{
		pFile = fopen(m_strLogErrFile.c_str(), "w");
	}

	if (pFile)
	{
		char szLogBuf[DEF_LOGREC_MAXSIZE];
		memset(szLogBuf, 0, DEF_LOGREC_MAXSIZE);
		
		char* p = szLogBuf;
#ifdef UNIX
		CTime objCurTime = CTime::GetCurrentTime(); 
		objCurTime.Format(p); 
		strcat(p, "\n");
#else
		sprintf(p, "%s\n", CTime::GetCurrentTime().Format("%c ")); 
#endif
	
		fprintf(pFile, "%s %s\n", szLogBuf, szLog);
		fflush(pFile);
		fclose(pFile);
	}
}

void CLogger::UpdateLogLevelValid()
{
	// 文件
	char szFileName[DEF_LOGFILE_NAMELEN];
	sprintf(szFileName, "%s.valid", CLogger::Instance()->m_strLogFileBase.c_str());

	FILE* pFile = fopen(szFileName, "r");
	if (pFile)
	{
		int nLogLevel = 0;
		if (fscanf(pFile, "%d", &nLogLevel))
		{
			CLogger::Instance()->m_nLogLevelValid = nLogLevel;
		}

		fclose(pFile);
	}
}






