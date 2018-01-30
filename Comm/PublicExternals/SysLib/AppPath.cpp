//==============================================================================
//                      AppPath.cpp
//
//begin   : 2016-09-07
//describe:
//==============================================================================
#ifdef HP_UX
	#include <sys/param.h>
	#include <sys/pstat.h>
#endif

#ifndef UNIX
	#include <direct.h>
#endif

#include <stdlib.h> 
#include <stdio.h> 
#include <limits.h>

#include "AppPath.h"

char CAppPath::m_szWorkDir[FILE_NAME_LEN] = {0};
//==============================================================================
//                 
//                    CAppPath
//                    
//
//==============================================================================
CAppPath::CAppPath()
{
}

CAppPath::~CAppPath()
{
}

#if defined(UNIX) || defined(HP_UX)
bool CAppPath::InitExeDir(const char* szExe)
{
	char* p = realpath(szExe, m_szWorkDir); // ��szExeת������·������m_szWorkDir��
	if (!p)
	{
		return false;
	}
	
	int x = strlen(m_szWorkDir) - 1;
	for ( ; x >= 0; x--)
	{
		if(m_szWorkDir[x] == GetParseChar())
		{
			break;
		}
	}

	if (x == -1)
	{
		return false;
	}

	if (x + 2 > strlen(m_szWorkDir))
	{
		return false;
	}

	m_szWorkDir[x + 1] = 0;

	return true;	
}

string CAppPath::GetWorkDir()
{
	char szPath[FILE_NAME_LEN];
	char szCmd[FILE_NAME_LEN];
	
#ifdef HP_UX
	if (!getcwd(szPath, FILE_NAME_LEN))
	{
		return false;
	}
	
	pid_t p = getpid();
	if (pstat_getcommandline(szCmd, FILE_NAME_LEN, 1, p) == -1)
	{
		return false;
	}
	
	if (szCmd[0] != GetParseChar())
	{
		strcat(szPath, GetParseStr().c_str());
		strcat(szPath, szCmd);
		strcpy(szCmd, szPath);
	}
#else
	if (!getcwd(szCmd, FILE_NAME_LEN))
	{
		return false;
	}
	
	if(szCmd[strlen(szCmd)-1] != GetParseChar())
	{
		strcat(szCmd, GetParseStr().c_str());
	}
#endif
	
	if(m_szWorkDir[0] != 0)
	{
		strcpy(szCmd, m_szWorkDir); // ���ⲿ�����׼ȷ�ĵ�ǰĿ¼��getcwd�������κ�����³�Ϊ��ǰ�����Ŀ¼
	}
	
	char *pSep = strrchr(szCmd, GetParseChar());
	if (pSep)
	{
		*(pSep+1) = '\0';
	}
	
	char* ps = szCmd + 1;
	memset(szPath, 0, FILE_NAME_LEN);
	while (1)
	{
		pSep = strchr(ps, GetParseChar());
		if (!pSep)
		{
			break;
		}
		
		char szFrag[FILE_NAME_LEN];
		memset(szFrag, 0, FILE_NAME_LEN);
		memcpy(szFrag, ps, pSep - ps);
		if (!strcmp(szFrag, "."))
		{
			// do nothing
		}
		else if (!strcmp(szFrag, ".."))
		{
			if (strlen(szPath) > 1)
			{
				char *pp = strrchr(szPath, GetParseChar());
				if (pp)
				{
					*pp = '\0';
				}
			}
		}
		else
		{
			strcat(szPath, GetParseStr().c_str());
			strcat(szPath, szFrag);
		}
		
		ps = pSep + 1;
	}
	
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

#else

bool CAppPath::InitExeDir()
{
	memset(m_szWorkDir, 0, sizeof(m_szWorkDir));

	char* pEnvPath = getenv("SCADA_WORKDIR");

	// ��������˻�����������ͨ����������ȡ������Ŀ¼
	if(pEnvPath)
	{
		// ������������е�·������������·����ʼ��ʧ��
		if (sizeof(m_szWorkDir) <= strlen(pEnvPath))
		{
			return false;
		}

		strncpy(m_szWorkDir, pEnvPath, sizeof(m_szWorkDir));

		if (strlen(m_szWorkDir)>1
			&& m_szWorkDir[strlen(m_szWorkDir)] != GetParseChar())
		{
			m_szWorkDir[strlen(m_szWorkDir)] = GetParseChar();
		}
	}
	else
	{
		DWORD dwDirLength = GetModuleFileName(NULL, (LPSTR)m_szWorkDir, sizeof(m_szWorkDir));

		// ��ֹ·������������·����ʼ��ʧ��
		if (dwDirLength>=sizeof(m_szWorkDir))
		{
			return false;
		}

		// �����һ��·���ָ���֮���ȡ
		char* pSep = strrchr(m_szWorkDir, GetParseChar());
		if (pSep)
		{
			*(pSep+1) = '\0';
		}
	}

	return true;	
}

string CAppPath::GetWorkDir()
{
	return string(m_szWorkDir);
}

#endif

char CAppPath::GetParseChar()
{
#ifdef UNIX
	return '/';
#else
	return '\\';
#endif
}

string CAppPath::GetParseStr()
{
#ifdef UNIX
	return "/";
#else
	return "\\";
#endif
}

string CAppPath::GetBinPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());

	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}

	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}

	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "Bin");
	strcat(szPath, GetParseStr().c_str());

	return szPath;
}

string CAppPath::GetPublicConfPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());
	
	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "PublicConf");
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

string CAppPath::GetPrivateConfPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());
	
	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
#ifdef UNIX
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
#endif
	
	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "PrivateConf");
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

std::string CAppPath::GetDataFilePath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}

	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());

	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}

#ifdef UNIX
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
#endif

	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "DataFile");
	strcat(szPath, GetParseStr().c_str());

	return szPath;
}

string CAppPath::GetLogPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());
	
	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "Log");
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

string CAppPath::GetScriptPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());
	
	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "Script");
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

string CAppPath::GetSockPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());
	
	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "Sock");
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

string CAppPath::GetLockPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());
	
	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "Lock");
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

string CAppPath::GetShmKeyPath()
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}
	
	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());
	
	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}
	
	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, "ShmKey");
	strcat(szPath, GetParseStr().c_str());
	
	return szPath;
}

std::string CAppPath::GetTermProcessPath(string strProcName)
{
	string strPath = GetWorkDir();
	if (strPath.size() <= 0)
	{
		return "";
	}

	char szPath[FILE_NAME_LEN];
	memset(szPath, 0, sizeof(szPath));
	snprintf(szPath, sizeof(szPath), "%s", strPath.c_str());

	char *p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}

	p = strrchr(szPath, GetParseChar());
	if (p)
	{
		*p = '\0';
	}

	strcat(szPath, GetParseStr().c_str());
	strcat(szPath, strProcName.c_str());
	strcat(szPath, GetParseStr().c_str());

	return szPath;
}