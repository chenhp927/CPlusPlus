//==============================================================================
//                      Logger.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_LOGGER_H 
#define SELFDEF_LOGGER_H 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include <stdarg.h>

#include "TypeDef.h"
#include "PthMutex.h"

#ifndef UNIX
#include <WTYPES.H>
#endif

//------ 定义日志级别LogLevel（取值为：1、2、4、8、16、32 ...） ---------
// 重要日志，假设应用程序设置的LogFileBase = "AAA", 写日志时指定LogFileTag = "XXX"，
// 则本级别的日志记录在 AAA_XXX.log 文件中
#define LL_IMPORTANT		1

// 调试日志，假设应用程序设置的LogFileBase = "AAA", 写日志时指定LogFileTag = "XXX"，
// 则本级别的日志记录在 AAA_XXX.log 和 AAA_XXX_Debug.log 文件中
#define LL_DEBUG			2

#define DEF_LOGFILE_MAXSIZE		1024 * 1000 * 10	// 单个日志文件的最大长度
#define DEF_LOGFILE_BKCOUNT		2					// 日志文件的备份个数
#define DEF_LOGDATA_MAXCOUNT	200					// 记录数据包日志时的最大数据字节个数
#define DEF_LOGFILE_NAMELEN		1024				// 日志文件名称的最大长度
#define DEF_LOGREC_MAXSIZE		2048				// 每次写入的最大信息长度
#define DEF_LOGERR_FILETAG		"_WriteLogErr.log"	// 日志文件错误时输入到本文件

using namespace std;
//==============================================================================
//
//                   CLogger
//                 日志记录器类
//	说明：本类可用于多线程
//==============================================================================
class MY_EXT_CLASS CLogger
{
public:
	//------ 接口 ---------
	// 应用程序启动时必须设置szLogFileBase。
	// szLogFileBase中包含日志文件路径及应用程序名称。如设置
	// szLogFileBase="d:\log\aaa",写日志时指定szLogFileTag为xxx，则自动生成文件名称为
	// d:\log\aaa_xxx.log  d:\log\aaa_xxx_Debug.log
	static void SetLogFileBase(const char* szLogFileBase);
	static string GetLogFileBase();
	
	// 打印之前必须先设置szLogFileTag的信息，如文件的大小，备份个数，否则不打印
	static void SetLogFileTagInfo(const char* szLogFileTag, DWORD dwFileMaxSize = DEF_LOGFILE_MAXSIZE,
		BYTE byFileBkCount = DEF_LOGFILE_BKCOUNT, WORD wDataMaxCount = DEF_LOGDATA_MAXCOUNT);

	static void Write(const char* szLogFileTag, int nLogLevel, const char* szFormat, ...);
	static void Write(const char* szLogFileTag, int nLogLevel, const char* szFormat, va_list vl);
	static void WriteData(const char* szLogFileTag, int nLogLevel, BYTE* pData, DWORD dwDataLen, const char* szDesc = NULL);

private:
	//==============================================================================
	//
	//                   CLogFile
	//                日志文件记录类
	//
	//==============================================================================
	class CLogFile  
	{
	public:
		CLogFile();
		CLogFile(const char* szFileName);
		virtual ~CLogFile();
		
	protected:
		bool MakeWriteString(const char* szContent);
		bool MakeWriteString(const char* szFormat, va_list vl);
		bool MakeWriteString(BYTE *pData, DWORD dwDataLen, const char* szDesc = NULL);
		void MakeSeqNumString(char* szBuf);
		void MakeTimeString(char* szBuf);

		void WriteToFile();
		FILE* OpenLogFile();
		void BackupLogFile();

	public:
		void SetFileMaxSize(DWORD dwFileMaxSize = DEF_LOGFILE_MAXSIZE);  
		void SetFileBkCount(BYTE byFileBkCount = DEF_LOGFILE_BKCOUNT);  
		void SetDataMaxCount(WORD wDataMaxCount = DEF_LOGDATA_MAXCOUNT);  
		string GetFileName();

		virtual void Write(const char* szContent);
		virtual void Write(const char* szFormat, va_list vl);
		virtual void WriteData(BYTE *pData, DWORD dwDataLen, const char* szDesc = NULL);

	protected:
		string  m_strFileName;
		DWORD	m_dwFileMaxSize;
		DWORD	m_dwFileCurSize;
		BYTE	m_byFileBkCount;
		WORD	m_wDataMaxCount;

		FILE*				m_pFile;
		char				m_szLogBuf[DEF_LOGREC_MAXSIZE];

		CPthMutex			m_objMutex;	// 用于控制多线程之间的同步访问
	};


//------ CLogger 类的实现 ---------
public:
	virtual ~CLogger();
	friend class CLogFile;

protected:
	CLogger();
	static CLogger* Instance();

protected:
	static void UpdateLogLevelValid();

	void SetLogFile(const char* szFileName, DWORD dwFileMaxSize, BYTE byFileBkCount, WORD wDataMaxCount);
	string GetLogFileName(const char* szLogFileTag, const char* szLogLevelTag);
	CLogFile* GetLogFile(const char* szFileName);
	vector<CLogFile*> GetLogFiles(const char* szLogFileTag, int nLogLevel);
	void GetImportantFile(const char* szLogFileTag, vector<CLogFile*>& files);
	void GetDebugFile(const char* szLogFileTag, vector<CLogFile*>& files);

	void SaveErrLog(const char* szLog);

protected:
	string				m_strLogFileBase;
	string				m_strLogErrFile;

	static DWORD		m_dwSeqNum;
	vector<CLogFile* >	m_Files;
	
	int					m_nLogLevelValid;	//	允许日志输出的日志级别

	CPthMutex			m_objMutex;	/* 用于控制多线程之间的同步访问。
		特别说明：对于m_Files集合，只对集合本身进行同步控制，不对其
		中的CLogFile*进行控制，其同步控制由该对象自身完成，因此，
		CLogFile*对象创建后不能删除，只能在CLogger析构时删除。
		*/
};


#endif // !defined(SELFDEF_LOGGER_H)
