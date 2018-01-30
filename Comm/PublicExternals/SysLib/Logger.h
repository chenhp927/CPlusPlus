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

//------ ������־����LogLevel��ȡֵΪ��1��2��4��8��16��32 ...�� ---------
// ��Ҫ��־������Ӧ�ó������õ�LogFileBase = "AAA", д��־ʱָ��LogFileTag = "XXX"��
// �򱾼������־��¼�� AAA_XXX.log �ļ���
#define LL_IMPORTANT		1

// ������־������Ӧ�ó������õ�LogFileBase = "AAA", д��־ʱָ��LogFileTag = "XXX"��
// �򱾼������־��¼�� AAA_XXX.log �� AAA_XXX_Debug.log �ļ���
#define LL_DEBUG			2

#define DEF_LOGFILE_MAXSIZE		1024 * 1000 * 10	// ������־�ļ�����󳤶�
#define DEF_LOGFILE_BKCOUNT		2					// ��־�ļ��ı��ݸ���
#define DEF_LOGDATA_MAXCOUNT	200					// ��¼���ݰ���־ʱ����������ֽڸ���
#define DEF_LOGFILE_NAMELEN		1024				// ��־�ļ����Ƶ���󳤶�
#define DEF_LOGREC_MAXSIZE		2048				// ÿ��д��������Ϣ����
#define DEF_LOGERR_FILETAG		"_WriteLogErr.log"	// ��־�ļ�����ʱ���뵽���ļ�

using namespace std;
//==============================================================================
//
//                   CLogger
//                 ��־��¼����
//	˵������������ڶ��߳�
//==============================================================================
class MY_EXT_CLASS CLogger
{
public:
	//------ �ӿ� ---------
	// Ӧ�ó�������ʱ��������szLogFileBase��
	// szLogFileBase�а�����־�ļ�·����Ӧ�ó������ơ�������
	// szLogFileBase="d:\log\aaa",д��־ʱָ��szLogFileTagΪxxx�����Զ������ļ�����Ϊ
	// d:\log\aaa_xxx.log  d:\log\aaa_xxx_Debug.log
	static void SetLogFileBase(const char* szLogFileBase);
	static string GetLogFileBase();
	
	// ��ӡ֮ǰ����������szLogFileTag����Ϣ�����ļ��Ĵ�С�����ݸ��������򲻴�ӡ
	static void SetLogFileTagInfo(const char* szLogFileTag, DWORD dwFileMaxSize = DEF_LOGFILE_MAXSIZE,
		BYTE byFileBkCount = DEF_LOGFILE_BKCOUNT, WORD wDataMaxCount = DEF_LOGDATA_MAXCOUNT);

	static void Write(const char* szLogFileTag, int nLogLevel, const char* szFormat, ...);
	static void Write(const char* szLogFileTag, int nLogLevel, const char* szFormat, va_list vl);
	static void WriteData(const char* szLogFileTag, int nLogLevel, BYTE* pData, DWORD dwDataLen, const char* szDesc = NULL);

private:
	//==============================================================================
	//
	//                   CLogFile
	//                ��־�ļ���¼��
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

		CPthMutex			m_objMutex;	// ���ڿ��ƶ��߳�֮���ͬ������
	};


//------ CLogger ���ʵ�� ---------
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
	
	int					m_nLogLevelValid;	//	������־�������־����

	CPthMutex			m_objMutex;	/* ���ڿ��ƶ��߳�֮���ͬ�����ʡ�
		�ر�˵��������m_Files���ϣ�ֻ�Լ��ϱ������ͬ�����ƣ�������
		�е�CLogFile*���п��ƣ���ͬ�������ɸö���������ɣ���ˣ�
		CLogFile*���󴴽�����ɾ����ֻ����CLogger����ʱɾ����
		*/
};


#endif // !defined(SELFDEF_LOGGER_H)
