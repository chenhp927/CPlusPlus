//==============================================================================
//                      AppPath.h
//
//begin   : 2016-09-07
//describe:
//==============================================================================
#ifndef SELFDEF_APPPATH_H 
#define SELFDEF_APPPATH_H 

#include <string>
#include <string.h>

#include "TypeDef.h"

using namespace std;
//==============================================================================
//                 
//                    CAppPath
//                    
//
//==============================================================================
class MY_EXT_CLASS CAppPath  
{ 
public: 
	CAppPath(); 
	virtual ~CAppPath();

public:

#ifdef UNIX
 	static bool InitExeDir(const char* szExe);
#else
	static bool InitExeDir();
#endif

	static string GetWorkDir();
	static string GetBinPath();
	static string GetSockPath();
	static string GetLockPath();
	static string GetPublicConfPath();
	static string GetPrivateConfPath();
	static string GetDataFilePath();
	static string GetLogPath();
	static string GetScriptPath();
	static string GetShmKeyPath();
	// 获取进程相同目录下的其他进程目录
	// strProcName为其他进程文件夹名称
	static string GetTermProcessPath(string strProcName);

protected: 
	static char GetParseChar();
	static string GetParseStr();

protected:
	static char m_szWorkDir[FILE_NAME_LEN];
}; 

#endif // SELFDEF_APPPATH_H
