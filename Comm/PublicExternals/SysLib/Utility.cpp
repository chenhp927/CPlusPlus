//==============================================================================
//                      Utility.cpp
//
//begin   : 2016-08-23
//describe:
//==============================================================================

#ifdef UNIX
	#include <dirent.h>
	#include <string.h>
	#include <sys/times.h>
	#include <sys/stat.h>
	#include <libgen.h>
	#include <assert.h>

	#include "CTime.h"
#else
	#include <direct.h>
	#include <io.h>
	#include <assert.h>
#endif

#include "Utility.h" 

//==============================================================================
//                 
//                    CUtility
//                    通用工具类
//
//==============================================================================
CUtility::CUtility() 
{ 
} 
 
CUtility::~CUtility() 
{ 
} 

#ifdef UNIX
bool CUtility::MakeDir(const char* szPath)
{
	// 判定传入参数是否合法
	if(!szPath || strlen(szPath) == 0 || strlen(szPath) >= FILE_NAME_LEN)
	{
		return false;
	}
	
	// 系统根目录肯定已经存在
	if(strcmp(szPath, "/") == 0)
	{
		return true;
	}
	
	// 取出父目录
	char szDirCur[FILE_NAME_LEN];
	strcpy(szDirCur, szPath);
	
	char szDirParent[FILE_NAME_LEN];
	strcpy(szDirParent, dirname(szDirCur));
	
	// 检查父目录是否存在
	if (!CUtility::IsDirExist(szDirParent))
	{	
		// 父目录不存在时，递归调用创建父目录
		strcat(szDirParent, "/"); // dirname()返回时字符串结尾不包括'/'，为了保证递归调用中能够区分处理目录和文件名称
		if (!MakeDir(szDirParent))
		{
			return false;
		}
	}
	
	// 如果是目录，去创建
	strcpy(szDirCur, szPath);
	if (szPath[strlen(szPath) - 1] == '/')
	{
		if (mkdir(szDirCur, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// 如果是文件名称，直接返回
	else
	{
		return true;
	}
}
#else

bool CUtility::MakeDir(const char* szPath)
{
	// 去除右侧非路径部分
	CString strPath = szPath;
	int nPos = strPath.ReverseFind('\\');
	if (nPos <= 0)
	{
		return false;
	}
	
	strPath = strPath.Left(nPos + 1);
	
	// 保证最后一个字符是目录符号
	CString strPathTmp = strPath;
	if (strPathTmp.Right(1) != "\\")
	{
		strPathTmp += "\\";
	}
	
	// 从左侧查找第一个目录结束位置
	nPos = strPathTmp.Find('\\');
	if (strPathTmp[nPos - 1] == ':') // 如果是盘符，继续向后找
	{
		nPos = strPathTmp.Find('\\', nPos + 1);
	}
	
	// 取出目录，从左到右顺序建立目录
	CString strDir = strPathTmp.Left(nPos + 1);
	while(strDir.GetLength() > 0)
	{
		// 创建目录
		_mkdir((char*)(LPCTSTR)strDir);
		
		// 获取下一级目录
		nPos = strPathTmp.Find('\\', nPos + 1);
		if (nPos < 0)
		{
			break;
		}
		strDir = strPathTmp.Left(nPos + 1);
	}
	
	return true;
}
#endif

bool CUtility::IsDirExist(const char* szPath)
{
	if (!szPath)
	{
		return false;
	}

#ifdef UNIX
	DIR *pDir = opendir(szPath);
	
	if (!pDir)
	{
		return false;
	}
	closedir(pDir);
#else
	if (access(szPath, 0) != 0)
	{
		return false;
	}
#endif	
	
	return true;
}

int CUtility::ATOI(const char* sz)
{
	if (strlen(sz) <= 1)
	{
		return atoi(sz);
	}

	if ((sz[0] == '0' && (sz[1] == 'x' || sz[1] == 'X')) ||
		(sz[strlen(sz)-1] == 'h' || sz[strlen(sz)-1] == 'H'))
	{
		int n = 0;
		sscanf(sz, "%x", &n);
		return n;
	}
	else
	{
		return atoi(sz);
	}
}

string CUtility::ITOA(unsigned int n, const char* szFormat)
{
	char szTemp[1024];
	sprintf(szTemp, szFormat, n);
	return szTemp;
}

string CUtility::GetHexStr(DWORD dw)
{
	char szTemp[20];
	memset(szTemp, 0, 20);
	sprintf(szTemp, "0X%02x ", dw >> 24 & 0xff);
	sprintf(szTemp + 5, "%02x ", dw >> 16 & 0xff);
	sprintf(szTemp + 8, "%02x ", dw >> 8 & 0xff);
	sprintf(szTemp + 11, "%02x", dw & 0xff);
	return szTemp;
}

bool CUtility::IsDigital(string str)
{
	Trim(str);

	for (int x = 0; x < str.size(); x++)
	{
		if (str[x] < '0' || str[x] > '9')
		{
			return false;
		}
	}

	return true;
}

BYTE CUtility::DecToBCD(BYTE by)
{
	if (by > 0 && by <= 99)
	{
		return by/10*16 + by%10;
	}
	else
	{
		return 0;
	}
}

string CUtility::Trim(string& str)
{
    if (str.empty()) 
	{
        return str;
	}
	
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);

    return str;
}

void CUtility::SplitStr(string strSrc, string strSep, vector<string>& vecRet)
{
	vecRet.clear();

	while(strSrc.length() > 0)
	{
		size_t nFind = strSrc.find(strSep, 0);
		if (nFind == std::string::npos)
		{
			vecRet.push_back(strSrc);
			return;
		}

		vecRet.push_back(strSrc.substr(0, nFind));
		
		strSrc = strSrc.substr(nFind+strSep.length());
	}
}

string CUtility::MakeUpper(string& str)
{
	if (str.empty())
	{
		return str;
	}

	for(unsigned int x = 0 ; x < str.size(); x++)
	{
		if ( str[x] >= 'a' && str[x] <= 'z')
		{
			str[x] = str[x] - 'a' + 'A';
		}
	}

	return str;
}

string CUtility::MakeLower(string& str)
{
	if (str.empty())
	{
		return str;
	}

	for(unsigned int x = 0 ; x < str.size(); x++)
	{
		if ( str[x] >= 'A' && str[x] <= 'Z')
		{
			str[x] = str[x] - 'A' + 'a';
		}
	}

	return str;
}

DWORD CUtility::GetUptime()
{
#ifdef UNIX
	struct tms ts;
	memset(&ts, 0, sizeof(ts));

	clock_t t = times(&ts); 
	if (t == ((clock_t)-1))
	{
		printf("times sys call fail\n");
	}
#else
	DWORD t = ::GetTickCount();
#endif	

	return t;
}

DWORD CUtility::GetElapseTime(DWORD dwPreUptime)
{
#ifdef UNIX
	// 每秒钟的打点计数
	static long lClkTck = sysconf(_SC_CLK_TCK);

	if (lClkTck == -1)
	{
		printf("sysconf for _SC_CLK_TCK fail\n");
	}
#else
	// 每秒钟的打点计数
	long lClkTck = 1000;
#endif	

	DWORD dwCur = CUtility::GetUptime();
	if (dwCur >= dwPreUptime)
	{
		return  (dwCur - dwPreUptime) / lClkTck;
	}
	else
	{
		return  (0xffffffff - dwPreUptime + dwCur) / lClkTck;
	}

}

DWORD CUtility::GetElapseMs(DWORD dwPreUptime)
{
#ifdef UNIX
	// 每个打点计数需要的毫秒数
	static long lMsInTtck = 1000 / sysconf(_SC_CLK_TCK) ;
#else
	// 每个打点计数需要的毫秒数
	long lMsInTtck = 1;
#endif	

	DWORD dwCur = CUtility::GetUptime();
	if (dwCur >= dwPreUptime)
	{
		return  (dwCur - dwPreUptime) * lMsInTtck;
	}
	else
	{
		return  (0xffffffff - dwPreUptime + dwCur) * lMsInTtck;
	}
}

string CUtility::GetTimeStr(time_t t)
{
	CTime ctInput(t);
	char szTemp[30];
	memset(szTemp, 0, 30);
	sprintf(szTemp, "%4d-%02d-%02d %02d:%02d:%02d", ctInput.GetYear(), ctInput.GetMonth(),
					ctInput.GetDay(), ctInput.GetHour(), ctInput.GetMinute(), ctInput.GetSecond());
	return szTemp;
}

void CUtility::SleepSec(BYTE bySec)
{
#ifdef UNIX
	sleep(bySec);
#else
	Sleep(bySec*1000);
#endif	
}

void CUtility::SleepMs(DWORD dwMs)
{
#ifdef UNIX
	usleep(dwMs*1000);
#else
	Sleep(dwMs);
#endif
}

time_t CUtility::GetCtrlDay(time_t t)
{
	CTime ctInput(t);
	time_t tCtrlDay = 0;

#ifdef UNIX
	if (ctInput.GetHour() >= 18)
	{
		CTime ctNextDay(t + 24*3600);
		tCtrlDay = CTime::MakeTime(ctNextDay.GetYear(), ctNextDay.GetMonth(), ctNextDay.GetDay(), 0, 0, 0);
	}
	else
	{
		tCtrlDay = CTime::MakeTime(ctInput.GetYear(), ctInput.GetMonth(), ctInput.GetDay(), 0, 0, 0);
	}
#else
	if (ctInput.GetHour() >= 18)
	{
		CTime ctNextDay(t + 24*3600);
		CTime ctCtlDay(ctNextDay.GetYear(), ctNextDay.GetMonth(), ctNextDay.GetDay(), 0, 0, 0);
		tCtrlDay = ctCtlDay.GetTime();
	}
	else
	{
		CTime ctCtlDay(ctInput.GetYear(), ctInput.GetMonth(), ctInput.GetDay(), 0, 0, 0);
		tCtrlDay = ctCtlDay.GetTime();
	}
#endif	
	
	return tCtrlDay;
}

int CUtility::GetShift(time_t t)
{
	CTime tm(t);
	int shift = 2;
	if (tm.GetHour() >= 18 || tm.GetHour() < 6)
	{
		shift = 1;
	}
	
	return shift;
}

